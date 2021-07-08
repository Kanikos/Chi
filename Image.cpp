#include "Image.h"

//--------------------------------------------------
// constructors
//--------------------------------------------------

Image::Image(const std::filesystem::path& path, Hash& hasher)
: path(path)
{
	FILE *imageFile = fopen(path.c_str(), "rb");
	void (*read)(FILE*, size_t&, size_t&) = getReader(path.extension().string());

	if(read == nullptr)
	{
		width = 0;
		height = 0;
		return;
	}

	read(imageFile, width, height);
	hash(imageFile, hasher);
	fclose(imageFile);
}

Image::Image(std::filesystem::path&& path, Hash& hasher)
: path(std::move(path))
{
	FILE *imageFile = fopen(path.c_str(), "rb");
	void (*read)(FILE*, size_t&, size_t&) = getReader(path.extension().string());

	if(read == nullptr)
	{
		width = 0;
		height = 0;
		return;
	}

	read(imageFile, width, height);
	hash(imageFile, hasher);
	fclose(imageFile);
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
// functions that help construct the image
//--------------------------------------------------

void (*Image::getReader(const std::string& extension))(FILE*, size_t&, size_t&)
{
	// determine the image type
	if(extension == ".jpg" || extension == ".jpeg" || extension == ".JPG" || extension == ".JPEG")
		return &readJPG;

	if(extension == ".png" || extension == ".PNG")
		return &readPNG;

	return nullptr;
}

void Image::hash(FILE *imageFile, Hash& hasher)
{
	fseek(imageFile, 0, SEEK_SET);

	hasher.digest(imageFile);
	digest = hasher.output;
}

//--------------------------------------------------
// image readers
//--------------------------------------------------

void Image::readPNG(FILE *imageFile, size_t& width, size_t& height)
{
	constexpr size_t HEADER_SIZE = 8;
	unsigned char header[HEADER_SIZE];
	
	// ensure the entered path corresponds to a png image
	fread(header, sizeof(unsigned char), HEADER_SIZE, imageFile);
	if(png_sig_cmp(header, 0, HEADER_SIZE))
	{
		// TODO: error handling
		// TEMP: dimensions are initialized to zero
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

void Image::readJPG(FILE *imageFile, size_t& width, size_t& height)
{
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

//--------------------------------------------------
// comparison operators
//--------------------------------------------------

bool Image::operator>(const Image& image) const
{
	// priority: width, height, digest
	if(width > image.width)
		return true;
	if(width == image.width && height > image.height)
		return true;
	if(height == image.height && digest > image.digest) 
		return true;

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
	if(width == image.width && height < image.height)
		return true;
	if(height == image.height && digest == image.digest)
		return true;

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
	return out << image.width << "x" << image.height;
}
