#pragma once

#include <iostream>
#include "Hash.h"

class Digest
{
private:
	unsigned char *hash = nullptr;

public:
	//--------------------------------------------------
	// constructors
	//--------------------------------------------------
	Digest();
	Digest(const unsigned char *hash);
	Digest(const Digest& digest);
	Digest(Digest&& digest);

	//--------------------------------------------------
	// comparison operators
	//--------------------------------------------------
	
	short operator<=>(const unsigned char *hash) const;
	short operator<=>(const Digest& digest) const;

	bool operator<(const unsigned char *hash) const;
	bool operator==(const unsigned char *hash) const;
	bool operator>(const unsigned char *hash) const;

	bool operator<(const Digest& digest) const;
	bool operator==(const Digest& digest) const;
	bool operator>(const Digest& digest) const;

	//--------------------------------------------------
	// assignment operators
	//--------------------------------------------------

	Digest& operator=(const void *buffer);

	Digest& operator=(const Digest& digest);
	Digest& operator=(Digest&& digest);

	//--------------------------------------------------
	// stream operators
	//--------------------------------------------------

	friend std::istream& operator>>(std::istream& in, const Digest& digest);
	friend std::ostream& operator<<(std::ostream& out, const Digest& digest);

	//--------------------------------------------------
	// destructors 
	//--------------------------------------------------
	~Digest();
};
