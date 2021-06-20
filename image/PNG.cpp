#include "PNG.h"

//--------------------------------------------------
// constructors
//--------------------------------------------------

PNG::PNG(const std::filesystem::path& path, Hash& hasher)
: Image(path)
{
	readImage(path.c_str(), hasher);
}

PNG::PNG(std::filesystem::path&& path, Hash& hasher)
: Image(std::move(path))
{
	readImage(path.c_str(), hasher);
}

//--------------------------------------------------

PNG::PNG(const PNG& image)
: Image(image)
{}

PNG::PNG(PNG&& image)
: Image(std::move(image))
{}

//--------------------------------------------------
// readImage implementation
//--------------------------------------------------

void PNG::readImage(const char* path, Hash& hasher)
{
	constexpr size_t HEADER_SIZE = 8;

	FILE *imageFile = fopen(path, "rb");
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
	png_info *png_info = png_create_info_struct(png);

	png_init_io(png, imageFile);
	png_set_sig_bytes(png, HEADER_SIZE);
	png_read_info(png, png_info);

	width = png_get_image_width(png, png_info);
	height = png_get_image_height(png, png_info);

	png_destroy_read_struct(&png, &png_info, NULL);

	// set FILE's pointer back to the beginning
	// initialize the digest from hashing the entire image file
	fseek(imageFile, 0, SEEK_SET);

	hasher.digest(imageFile);
	digest = hasher.output;
	
	fclose(imageFile);
}
