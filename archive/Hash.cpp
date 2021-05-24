#include "Hash.h"

//--------------------------------------------------
// static functions / variables
//--------------------------------------------------

void Hash::initialize()
{
	if(!gcry_check_version(MIN_VER))
	{
		std::cout << "Error: Requires libgcrypt version " << MIN_VER << " or higher";
		exit(2);
	}

	gcry_control(GCRYCTL_DISABLE_SECMEM, 0);
	gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
}

//--------------------------------------------------
// instance functions
//--------------------------------------------------

Hash::Hash()
{
	gcry_md_open(&hasher, GCRY_MD_SHA256, 0);
}

void Hash::digest(std::ifstream& dataStream)
{
	gcry_md_reset(hasher);

	// keep hasing chunks of the file until the file is completely hashed
	while(dataStream)
	{
		dataStream.read(buffer, BUFFER_SIZE);
		gcry_md_write(hasher, buffer, dataStream.gcount());
	}
	
	// get a pointer to the resulting hash
	output = gcry_md_read(hasher, GCRY_MD_SHA256);
}

std::ostream& operator<<(std::ostream& out, const Hash& hash)
{
	return out.write((const char*) hash.output, Hash::HASH_SIZE);
}

Hash::~Hash()
{
	gcry_md_close(hasher);
}
