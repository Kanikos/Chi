#pragma once

#include <fstream>
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
	// static functions
	//--------------------------------------------------

	static constexpr unsigned int BUFFER_SIZE = 1048576;
	static constexpr unsigned int HASH_SIZE = 32;

	char buffer[BUFFER_SIZE];
	unsigned char *output = nullptr;

	/*
	 * THIS MUST BE CALLED BEFORE USING AN INSTANCE OF THIS CLASS
	 * initializes the gcrypt library
	 */
	static void initialize();

	//--------------------------------------------------
	// instance functions
	//--------------------------------------------------

	/*
	 * allocates gcrypt resouces for sha256 hashing
	 */
	Hash();

	/*
	 * Hashes an entire file from stream
	 * the resulting hash is stored in the internal buffer
	 */
	void digest(std::ifstream& dataStream);

	/*
	 * enables printing to stream
	 */
	friend std::ostream& operator<<(std::ostream& out, const Hash& hash);

	/*
	 * frees gcrypt resources
	 */
	~Hash();
};
