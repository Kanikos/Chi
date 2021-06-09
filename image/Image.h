#pragma once

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <utility>
#include "../archive/Digest.h"
#include "../archive/Hash.h"

class Image
{
protected:
	//--------------------------------------------------
	// properties
	//--------------------------------------------------
	
	std::filesystem::path path;
	
	Digest digest;
	size_t width, height;

	//--------------------------------------------------
	// constructors
	//--------------------------------------------------

	Image(const std::filesystem::path& path);
	Image(std::filesystem::path&& path);

	Image(const std::filesystem::path& path, const Digest& digest, size_t width, size_t height);
	Image(std::filesystem::path&& path, Digest&& digest, size_t width, size_t height);

	//--------------------------------------------------
	// metadata reader
	//--------------------------------------------------

	virtual void readImage(const char *path) = 0;

public:
	//--------------------------------------------------	
	// constructors
	//--------------------------------------------------

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

	//--------------------------------------------------
	// stream operators
	//--------------------------------------------------

	friend std::ostream& operator<<(std::ostream& out, const Image& image);
};
