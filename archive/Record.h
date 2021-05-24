#pragma once

#include <iostream>
#include "Hash.h"

class Record
{
private:
	unsigned char *digest = nullptr;

public:
	//--------------------------------------------------
	// constructors
	//--------------------------------------------------
	Record();
	Record(const unsigned char *digest);
	Record(const Record& record);
	Record(Record&& record);

	//--------------------------------------------------
	// comparison operators
	//--------------------------------------------------
	
	short operator<=>(const unsigned char *anotherDigest) const;
	short operator<=>(const Record& record) const;

	bool operator<(const unsigned char *anotherDigest) const;
	bool operator==(const unsigned char *anotherDigest) const;
	bool operator>(const unsigned char *anotherDigest) const;

	bool operator<(const Record& Record) const;
	bool operator==(const Record& Record) const;
	bool operator>(const Record& Record) const;

	//--------------------------------------------------
	// stream operators
	//--------------------------------------------------

	friend std::istream& operator>>(std::istream& in, const Record& record);
	friend std::ostream& operator<<(std::ostream& out, const Record& record);

	//--------------------------------------------------
	// destructors 
	//--------------------------------------------------
	~Record();
};
