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

typedef void (*ImageReader)(FILE*, size_t&, size_t&, bool&);

class Image
{
private:
	//--------------------------------------------------
	// properties
	//--------------------------------------------------
	
	std::filesystem::path path;
	
	Digest digest;
	size_t width, height;
	bool err = false;

	//--------------------------------------------------
	// constructors
	//--------------------------------------------------

	Image(const std::filesystem::path& path, const Digest& digest, size_t width, size_t height);
	Image(std::filesystem::path&& path, Digest&& digest, size_t width, size_t height);

	//--------------------------------------------------
	// hashing
	//--------------------------------------------------

	void hash(FILE *imageFile, Hash& hasher);

public:
	//--------------------------------------------------	
	// constructors
	//--------------------------------------------------

	Image(const std::filesystem::path& path, FILE *imageFile, Hash& hasher, ImageReader read);
	Image(std::filesystem::path&& path, FILE *imageFile, Hash& hasher, ImageReader read);

	//--------------------------------------------------

	Image(const Image& image);
	Image(Image&& image);

	//--------------------------------------------------
	// image readers
	//--------------------------------------------------

	static ImageReader identify(FILE *file);

	static void readPNG(FILE *path, size_t& width, size_t& height, bool& err);
	static void readJPG(FILE *path, size_t& width, size_t& height, bool& err);

	//--------------------------------------------------
	// saving and getters
	//--------------------------------------------------

	void moveTo(const std::filesystem::path& filepath);

	//--------------------------------------------------

	std::string filename() const;
	std::string extension() const;
	bool error() const;

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
