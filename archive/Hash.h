#pragma once

#include <fstream>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <gcrypt.h>

#define MIN_VER "1.9.2"

class Hash
{
private:
	gcry_md_hd_t hasher;

public:
	//--------------------------------------------------
	// properties
	//--------------------------------------------------

	static constexpr size_t BUFFER_SIZE = 1048576;
	static constexpr size_t HASH_SIZE = 32;

	char buffer[BUFFER_SIZE];
	unsigned char *output = nullptr;

	//--------------------------------------------------
	// initializer
	//--------------------------------------------------

	/*
	 * THIS MUST BE CALLED BEFORE USING AN INSTANCE OF THIS CLASS
	 * initializes the gcrypt library
	 */
	static void initialize();

	//--------------------------------------------------
	// constructors
	//  - allocates ~1 MB for hashing
	//--------------------------------------------------

	Hash();

	//--------------------------------------------------
	// hashing functions
	//  - hashes the entire file
	//  - hashing begins where the streams is pointing at
	//--------------------------------------------------

	void digest(std::ifstream& dataStream);
	void digest(FILE *dataStream);

	//--------------------------------------------------
	// stream operators
	//--------------------------------------------------

	friend std::ostream& operator<<(std::ostream& out, const Hash& hash);
	
	//--------------------------------------------------
	// destructors
	//  - frees gcrypt resources
	//--------------------------------------------------
	~Hash();
};
