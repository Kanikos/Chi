#include "Image.h"

//--------------------------------------------------
// constructors
//--------------------------------------------------

Image::Image(const std::filesystem::path& path, FILE *imageFile, Hash& hasher, ImageReader read)
: path(path)
{
	read(imageFile, width, height, err);
	hash(imageFile, hasher);
}

Image::Image(std::filesystem::path&& path, FILE *imageFile, Hash& hasher, ImageReader read)
: path(std::move(path))
{
	read(imageFile, width, height, err);
	hash(imageFile, hasher);
}

//--------------------------------------------------

Image::Image(const std::filesystem::path& path, const Digest& digest, size_t width, size_t height)
: path(path), digest(digest), width(width), height(height)
{}

Image::Image(std::filesystem::path&& path, Digest&& digest, size_t width, size_t height)
: path(std::move(path)), digest(std::move(digest)), width(width), height(height)
{}

//--------------------------------------------------

Image::Image(const Image& image)
: Image(image.path, image.digest, image.width, image.height)
{}

Image::Image(Image&& image)
: Image(std::move(image.path), std::move(image.digest), image.width, image.height)
{}

//--------------------------------------------------
// image readers
//--------------------------------------------------

ImageReader Image::identify(FILE *file)
{	
	// magic numbers that identify which type of image file a file is 
	static unsigned int PNG_MAGIC_NUMBER = 0x474E5089;
	static unsigned int JPG_MAGIC_NUMBERS[] = {0x00FFD8FF, 0x00004649, 0x00006669};

	// read the first 4 bytes as an int and determine the file type
	unsigned int magicNumber;
	fread(&magicNumber, 1, sizeof(unsigned int), file);

	if(magicNumber == PNG_MAGIC_NUMBER)
	{
		fseek(file, 0, SEEK_SET);
		return &readPNG;
	}

	magicNumber &= 0x00FFFFFF;
	for(unsigned int i = 0; i < 3; i++)
		if(magicNumber == JPG_MAGIC_NUMBERS[i])
		{
			fseek(file, 0, SEEK_SET);
			return &readJPG;
		}

	return nullptr;
}

void Image::readPNG(FILE *imageFile, size_t& width, size_t& height, bool& err)
{
	constexpr size_t HEADER_SIZE = 8;
	unsigned char header[HEADER_SIZE];
	
	// ensure the entered path corresponds to a png image
	fread(header, sizeof(unsigned char), HEADER_SIZE, imageFile);
	if(png_sig_cmp(header, 0, HEADER_SIZE))
	{
		err = true;

		width = 0;
		height = 0;

		return;
	}

	// read the metadata from the entered image
	png_struct *png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_info *info = png_create_info_struct(png);

	png_init_io(png, imageFile);
	png_set_sig_bytes(png, HEADER_SIZE);
	png_read_info(png, info);

	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);

	png_destroy_read_struct(&png, &info, NULL);
}

void Image::readJPG(FILE *imageFile, size_t& width, size_t& height, bool& err)
{
	// check if the file being read is actually a jpeg
	constexpr unsigned int JPEG_SIG_1 = 0x00FFD8FF;
	constexpr unsigned int JPEG_SIG_2 = 0x00004649;
	constexpr unsigned int JPEG_SIG_3 = 0x00006669;
	unsigned int sigBuffer;

	fseek(imageFile, 0, SEEK_SET);
	fread(&sigBuffer, 1, sizeof(unsigned int), imageFile);

	sigBuffer &= 0x00FFFFFF;
	if(sigBuffer != JPEG_SIG_1 && sigBuffer != JPEG_SIG_2 && sigBuffer != JPEG_SIG_3)
	{
		err = true;

		width = 0;
		height = 0;

		return;
	}

	fseek(imageFile, 0, SEEK_SET);

	// read image header and attempt to extract width and height from it
	jpeg_decompress_struct jpeg;

	jpeg_error_mgr errorHandler;
	jpeg.err = jpeg_std_error(&errorHandler);
	
	jpeg_create_decompress(&jpeg);
	jpeg_stdio_src(&jpeg, imageFile);
	jpeg_read_header(&jpeg, true);
	jpeg_start_decompress(&jpeg);

	width = jpeg.output_width;
	height = jpeg.output_height;

	jpeg_abort_decompress(&jpeg);
	jpeg_destroy_decompress(&jpeg);
}

void Image::hash(FILE *imageFile, Hash& hasher)
{
	fseek(imageFile, 0, SEEK_SET);
	hasher.digest(imageFile);
	digest = hasher.output;
}

//--------------------------------------------------
// move & getter functions
//--------------------------------------------------

void Image::moveTo(const std::filesystem::path& filepath)
{
	std::filesystem::rename(path, filepath);
	path = filepath;
}

//--------------------------------------------------

std::string Image::filename() const
{
	char nameBuffer[84];
	sprintf(nameBuffer, "%08dx%08d_%s", width, height, digest.c_str());

	return std::string(nameBuffer); 
}

std::string Image::extension() const
{
	return path.extension().string();
}

bool Image::error() const
{
	return err;
}

//--------------------------------------------------
// comparison operators
//--------------------------------------------------

bool Image::operator>(const Image& image) const
{
	// priority: width, height, digest
	if(width > image.width)
		return true;
	if(width == image.width)
	{
		if(height > image.height)
			return true;

		if(height == image.height && digest > image.digest)
			return true;
	}

	return false;
}

bool Image::operator==(const Image& image) const
{
	return (width == image.width) && (height == image.height) && (digest == image.digest);
}

bool Image::operator<(const Image& image) const
{
	// priortiy: width, height, digest
	if(width < image.width)
		return true;
	if(width == image.width)
	{
		if(height < image.height)
			return true;

		if(height == image.height && digest < image.digest)
			return true;
	}

	return false;
}

//--------------------------------------------------
// assignment operator
//--------------------------------------------------

Image& Image::operator=(const Image& image)
{
	digest = image.digest;
	width = image.width;
	height = image.height;

	return *this;
}

Image& Image::operator=(Image&& image)
{
	digest = std::move(image.digest);
	width = image.width;
	height = image.height;

	return *this;
}

//--------------------------------------------------
// stream operators
//--------------------------------------------------

std::ostream& operator<<(std::ostream& out, const Image& image)
{
	char dimensionBuffer[15];
	sprintf(dimensionBuffer, "%06dx%06d_", image.width, image.height);
	
	return out << dimensionBuffer << image.digest;
}
