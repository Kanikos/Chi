#include "Image.h"

//--------------------------------------------------
// constructors
//--------------------------------------------------

Image::Image(const Digest& digest, size_t width, size_t height)
: digest(digest), width(width), height(height)
{}

Image::Image(Digest&& digest, size_t width, size_t height)
: digest(std::move(digest)), width(width), height(height)
{}

Image::Image(const Image& image)
: Image(image.digest, image.width, image.height)
{}

Image::Image(Image&& image)
: Image(std::move(image.digest), image.width, image.height)
{}

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
// Assignment operator
//--------------------------------------------------

Image& operator=(const Image& image)
{
	digest = image.digest;
	width = image.width;
	height = image.height;

	return *this;
}

Image& operator=(Image&& image)
{
	digest = std::move(image.digest);
	width = image.width;
	height = image.height;

	return *this;
}
