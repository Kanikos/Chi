#pragma once

#include <cstddef>
#include <utility>
#include "../archive/Digest.h"
#include "../archive/Hash.h"

class Image
{
protected:
	//--------------------------------------------------
	// properties
	//--------------------------------------------------
	
	Digest digest;
	size_t width, height;

public:
	//--------------------------------------------------	
	// constructors
	//--------------------------------------------------

	Image(const Digest& digest, size_t width, size_t height);
	Image(Digest&& digest, size_t width, size_t height);
	Image(const Image& image);
	Image(Image&& image);

	//--------------------------------------------------
	// comparison operators
	//--------------------------------------------------

	bool operator>(const Image& image) const;
	bool operator==(const Image& image) const;
	bool operator<(const Image& image) const;

	//--------------------------------------------------
	// assignment operators
	//--------------------------------------------------

	Image& operator=(const Image& image);
	Image& operator=(Image&& image);
};
