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
// stream operators
//--------------------------------------------------

std::istream& operator>>(std::istream& in, const Digest& digest)
{
	return in.read((char*) digest.hash, Hash::HASH_SIZE);
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