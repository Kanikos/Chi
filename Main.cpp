#include <filesystem>
#include <string>
#include "archive/Hash.h"
#include "image/JPEG.h"
#include "image/PNG.h"

void iterateOver(const std::filesystem::path& directory, Hash& hasher)
{
	const std::string acceptableFormats[6] = {".jpeg", ".jpg", ".JPEG", ".JPG", ".png", ".PNG"};

	std::filesystem::directory_iterator dirIterator(directory);
	for(const std::filesystem::path& entry: dirIterator)
	{
		// check if the file is a directory, if so iterate over it
		if(std::filesystem::is_directory(entry) && !std::filesystem::is_symlink(directory))
			iterateOver(entry, hasher);

		// if here check if the file is an image based on extension
		for(unsigned int format = 0; format < 6; format++)
			if(entry.extension() == acceptableFormats[format])
				if(format < 4)
					std::cout << JPEG(entry, hasher) << std::endl;
				else
					std::cout << PNG(entry, hasher) << std::endl;
	}
}

int main(int argc, char** args)
{
	// check the right number of arguments are passed in
	if(argc != 2)
	{
		std::cout << "Parsing Directory Required" << std::endl;
		return 1;
	}

	// initialize the gcrypt library
	Hash::initialize();
	Hash hasher;

	// open the directory arg[1] points to
	// ensuring that such a directory exists
	std::filesystem::path rootDirectory(args[1]);
	if(!std::filesystem::is_directory(rootDirectory))
	{
		std::cout << "Entered Path is not a Directory" << std::endl;
		return 1;
	}

	// iterate through every item in the directory, ignoring symlinks
	// collecting information from them 
	iterateOver(rootDirectory, hasher);

	return 0;
}
