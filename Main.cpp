#include <cstdio>
#include <filesystem>
#include <string>
#include "archive/Database.h"
#include "archive/Hash.h"
#include "Image.h"

class ControlImage: public Image
{
private:
	size_t count = 0;
	std::string fileHeader, fileFooter;

public:
	ControlImage(Image&& image, const std::filesystem::path& saveDirectory)
	: Image(std::move(image)), fileHeader(std::move(filename())), fileFooter(std::move(extension()))
	{
		move(*this, saveDirectory);
	}

	//--------------------------------------------------

	void move(Image& image, const std::filesystem::path& saveDirectory)
	{
		// create the filepath to save the file under
		std::filesystem::path filepath(saveDirectory);

		// create the filename that the file will be moved under
		static char nameBuffer[100];
		sprintf(nameBuffer, "%s_%04d%s", fileHeader.c_str(), count, fileFooter.c_str());
		filepath /= nameBuffer;

		image.moveTo(filepath);
		count++;
	}
};

std::filesystem::path saveDirectory;
Database<ControlImage> db;

void iterateOver(const std::filesystem::path& directory, Hash& hasher)
{
	const std::string acceptableFormats[6] = {".jpeg", ".jpg", ".JPEG", ".JPG", ".png", ".PNG"};

	std::filesystem::directory_iterator dirIterator(directory);
	for(const std::filesystem::path& entry: dirIterator)
	{
		// check if the file is a directory, if so iterate over it
		if(std::filesystem::is_directory(entry) && !std::filesystem::is_symlink(directory))
			iterateOver(entry, hasher);

		// if here check if the file is an image
		FILE *file = fopen(entry.c_str(), "rb");
		ImageReader format = Image::identify(file);
		
		if(format != nullptr)
		{
			Image newImage(entry, file, hasher, format);
			if(!newImage.error())
			{
				ControlImage *queryImage = db.find(newImage);
				if(queryImage == nullptr)
					db.insert(ControlImage(std::move(newImage), saveDirectory));
				else
					queryImage->move(newImage, saveDirectory);
			}
		}

		fclose(file);
	}
}

int main(int argc, char** args)
{
	// check the right number of arguments are passed in
	if(argc != 3)
	{
		std::cout << "Parsing Directory Required" << std::endl;
		return 1;
	}

	// initialize the gcrypt library
	Hash::initialize();
	Hash hasher;

	// set the save directory
	// ensure that the save directory exists
	saveDirectory = args[1];
	if(!std::filesystem::is_directory(saveDirectory))
	{
		std::cout << "Entered Save Directory is not a Directory" << std::endl;
		return 1;
	}

	// open the directory arg[1] points to
	// ensuring that such a directory exists
	std::filesystem::path rootDirectory(args[2]);
	if(!std::filesystem::is_directory(rootDirectory))
	{
		std::cout << "Entered Parsing Path is not a Directory" << std::endl;
		return 1;
	}

	// iterate through every item in the directory, ignoring symlinks
	// collecting information from them 
	iterateOver(rootDirectory, hasher);

	// TEMP: print out the largest element and it's dimensions
	std::cout << "SIZE: " << db.size() << "\tMAX: "<< (*db.max()) << std::endl;
	return 0;
}
