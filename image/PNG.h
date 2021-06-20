#pragma once

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <png.h>
#include "Image.h"

class PNG: public Image
{
private:
	//--------------------------------------------------
	// reads png images, extracting width and height
	//--------------------------------------------------

	void readImage(const char* path, Hash& hasher) override;

public:
	//--------------------------------------------------
	// constructors
	//--------------------------------------------------

	PNG(const std::filesystem::path& path, Hash& hasher);
	PNG(std::filesystem::path&& path, Hash& hasher);

	PNG(const PNG& image);
	PNG(PNG&& image);
};
