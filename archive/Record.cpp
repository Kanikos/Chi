#include "Record.h"

//--------------------------------------------------
// constructors
//--------------------------------------------------

Record::Record()
: digest(new unsigned char[Hash::HASH_SIZE])
{
	memset(digest, 0, Hash::HASH_SIZE);
}

Record::Record(const unsigned char *digest)
: digest(new unsigned char[Hash::HASH_SIZE])
{
	// copy over the image digest into the internal digest buffer
	memcpy(this->digest, digest, Hash::HASH_SIZE);
}

Record::Record(const Record& record)
: Record(record.digest)
{}

Record::Record(Record&& record)
: digest(record.digest)
{
	record.digest = nullptr;
}

//--------------------------------------------------
// comparison operators
//--------------------------------------------------

short Record::operator<=>(const unsigned char *anotherDigest) const
{
	// compare both digests for discrepencies
	for(size_t i = 0; i < Hash::HASH_SIZE; i++)
		if(digest[i] == anotherDigest[i])
			continue;
		else
		{
			short result = digest[i];
			result -= anotherDigest[i];

			return result;
		}
	
	// if here then the both digests were completely the same, return 0
	return 0;
}

short Record::operator<=>(const Record& record) const
{
	return operator<=>(record.digest);
}

//--------------------------------------------------

bool Record::operator<(const unsigned char *anotherDigest) const
{
	return operator<=>(anotherDigest) < 0;
}

bool Record::operator==(const unsigned char *anotherDigest) const
{
	return operator<=>(anotherDigest) == 0;
}

bool Record::operator>(const unsigned char *anotherDigest) const
{
	return operator<=>(anotherDigest) > 0;
}

//--------------------------------------------------

bool Record::operator<(const Record& record) const
{
	return operator<(record.digest);
}

bool Record::operator==(const Record& record) const
{
	return operator==(record.digest);
}

bool Record::operator>(const Record& record) const
{
	return operator>(record.digest);
}

//--------------------------------------------------
// stream operators
//--------------------------------------------------

std::istream& operator>>(std::istream& in, const Record& record)
{
	return in.read((char*) record.digest, Hash::HASH_SIZE);
}

std::ostream& operator<<(std::ostream& out, const Record& record)
{
	return out.write((const char*) record.digest, Hash::HASH_SIZE);
}

//--------------------------------------------------
// destructors
//--------------------------------------------------

Record::~Record()
{
	delete[] digest;
}
