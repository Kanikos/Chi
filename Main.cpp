#include <fstream>
#include <iostream>
#include "archive/Hash.h"

int main(int argc, char** argv)
{
	// initialize the gcrypt library
	Hash::initialize();

	return 0;
}
