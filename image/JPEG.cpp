#include "JPEG.h"

//--------------------------------------------------
// constructors
//--------------------------------------------------

JPEG::JPEG(const std::filesystem::path& path)
: Image(path)
{
	readImage(path.c_str());
}

JPEG::JPEG(std::filesystem::path&& path)
: Image(std::move(path))
{
	readImage(path.c_str());
}

//--------------------------------------------------

JPEG::JPEG(const JPEG& image)
: Image(image)
{}

JPEG::JPEG(JPEG&& image)
: Image(std::move(image))
{}

//--------------------------------------------------
// readImage implementation
//--------------------------------------------------

void JPEG::readImage(const char *path)
{
	// read image header and attempt to extract width and height from it
	FILE *imageFile = fopen(path, "rb");
	jpeg_decompress_struct jpeg;

	jpeg_error_mgr error;
	jpeg.err = jpeg_std_error(&error);

	jpeg_create_decompress(&jpeg);
	jpeg_stdio_src(&jpeg, imageFile);
	jpeg_read_header(&jpeg, true);
	jpeg_start_decompress(&jpeg);

	width = jpeg.output_width;
	height = jpeg.output_height;

	jpeg_abort_decompress(&jpeg);
	jpeg_destroy_decompress(&jpeg);

	// hash the entire file
	fseek(imageFile, 0, SEEK_SET);

	Hash hasher;
	hasher.digest(imageFile);
	digest = hasher.buffer;

	fclose(imageFile);
}
