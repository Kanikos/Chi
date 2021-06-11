#pragma once

#include <cstdio>
#include <filesystem>
#include <jpeglib.h>
#include "../archive/Hash.h"
#include "Image.h"

class JPEG: public Image
{

private:

//--------------------------------------------------
// readImage implementation
//--------------------------------------------------

void readImage(const char *path) override;

public:

//--------------------------------------------------
// constructors
//--------------------------------------------------

JPEG(const std::filesystem::path& path);
JPEG(std::filesystem::path&& path);

JPEG(const JPEG& image);
JPEG(JPEG&& image);
};
