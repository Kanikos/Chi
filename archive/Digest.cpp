#include "Digest.h"

//--------------------------------------------------
// constructors
//--------------------------------------------------

Digest::Digest()
: hash(new unsigned char[Hash::HASH_SIZE])
{
	memset(hash, 0, Hash::HASH_SIZE);
}

Digest::Digest(const unsigned char *hash)
: hash(new unsigned char[Hash::HASH_SIZE])
{
	// copy over the image digest into the internal digest buffer
	memcpy(this->hash, hash, Hash::HASH_SIZE);
}

Digest::Digest(const Digest& digest)
: Digest(digest.hash)
{}

Digest::Digest(Digest&& digest)
: hash(digest.hash)
{
	digest.hash = nullptr;
}

//--------------------------------------------------
// conversion functions
//--------------------------------------------------

const char* Digest::c_str() const
{
	static char hashBuffer[(Hash::HASH_SIZE * 2) + 1];
	char* currByte = hashBuffer;

	if(hash == nullptr)
	{
		memset(hashBuffer, '0', Hash::HASH_SIZE * 2);
		hashBuffer[Hash::HASH_SIZE * 2] = 0;
	}
	else 
		for(size_t i = 0; i < Hash::HASH_SIZE; i++)
		{
			sprintf(currByte, "%02x", hash[i]);
			currByte += 2;
		}	

	return hashBuffer;
}

//--------------------------------------------------
// comparison operators
//--------------------------------------------------

short Digest::operator<=>(const unsigned char *hash) const
{
	// compare both digests for discrepencies
	for(size_t i = 0; i < Hash::HASH_SIZE; i++)
		if(this->hash[i] == hash[i])
			continue;
		else
		{
			short result = this->hash[i];
			result -= hash[i];

			return result;
		}
	
	// if here then the both digests were completely the same, return 0
	return 0;
}

short Digest::operator<=>(const Digest& digest) const
{
	return operator<=>(digest.hash);
}

//--------------------------------------------------

bool Digest::operator<(const unsigned char *hash) const
{
	return operator<=>(hash) < 0;
}

bool Digest::operator==(const unsigned char *hash) const
{
	return operator<=>(hash) == 0;
}

bool Digest::operator>(const unsigned char *hash) const
{
	return operator<=>(hash) > 0;
}

//--------------------------------------------------

bool Digest::operator<(const Digest& digest) const
{
	return operator<(digest.hash);
}

bool Digest::operator==(const Digest& digest) const
{
	return operator==(digest.hash);
}

bool Digest::operator>(const Digest& digest) const
{
	return operator>(digest.hash);
}

//--------------------------------------------------
// assignment operators
//--------------------------------------------------

Digest& Digest::operator=(const void* buffer)
{
	memcpy(hash, buffer, Hash::HASH_SIZE);
	return *this;
}

//--------------------------------------------------

Digest& Digest::operator=(const Digest& digest)
{
	return operator=(digest.hash);
}

Digest& Digest::operator=(Digest&& digest)
{
	// release the internal hash buffer and commandeer digest's hash buffer
	delete hash;
	hash = digest.hash;
	
	digest.hash = nullptr;
	return *this;
}

//--------------------------------------------------
// stream operators
//--------------------------------------------------

std::istream& operator>>(std::istream& in, Digest& digest)
{
	return in.read((const char*) digest.hash, Hash::HASH_SIZE);
}

std::ostream& operator<<(std::ostream& out, const Digest& digest)
{
	return out.write((const char*) digest.hash, Hash::HASH_SIZE);
}

//--------------------------------------------------
// destructors
//--------------------------------------------------

Digest::~Digest()
{
	delete[] hash;
}
