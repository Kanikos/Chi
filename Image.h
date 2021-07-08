#pragma once

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <string>
#include <utility>
#include <jpeglib.h>
#include <png.h>
#include "archive/Digest.h"
#include "archive/Hash.h"

class Image
{
private:
	//--------------------------------------------------
	// properties
	//--------------------------------------------------
	
	std::filesystem::path path;
	
	Digest digest;
	size_t width, height;

	//--------------------------------------------------
	// constructors
	//--------------------------------------------------

	Image(const std::filesystem::path& path, const Digest& digest, size_t width, size_t height);
	Image(std::filesystem::path&& path, Digest&& digest, size_t width, size_t height);

	//--------------------------------------------------
	// functions that help construct the image
	//--------------------------------------------------

	void (*getReader(const std::string& extension))(FILE*, size_t&, size_t&);
	void hash(FILE *imageFile, Hash& hasher);

	//--------------------------------------------------
	// image readers
	//--------------------------------------------------

	static void readPNG(FILE *path, size_t& width, size_t& height);
	static void readJPG(FILE *path, size_t& width, size_t& height);

public:
	//--------------------------------------------------	
	// constructors
	//--------------------------------------------------

	Image(const std::filesystem::path& path, Hash& hasher);
	Image(std::filesystem::path&& path, Hash& hasher);

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
