#include "Image.h"

//--------------------------------------------------
// constructors
//--------------------------------------------------

Image::Image(const std::filesystem::path& path)
: path(path)
{}

Image::Image(std::filesystem::path&& path)
: path(std::move(path))
{}

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
	return out << image.digest << ": " << image.width << "x" << image.height;
}
