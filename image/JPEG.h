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

void readImage(const char *path, Hash& hasher) override;

public:

//--------------------------------------------------
// constructors
//--------------------------------------------------

JPEG(const std::filesystem::path& path, Hash& hasher);
JPEG(std::filesystem::path&& path, Hash& hasher);

JPEG(const JPEG& image);
JPEG(JPEG&& image);
};
