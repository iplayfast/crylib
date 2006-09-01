//****************************************************
//Copyright 2004
// Crystal Software (Canada) Inc.
//****************************************************


//Includes


//Class Instance Includes
#ifndef TCrySet
#define TCrySet	"CrySet"
//standard files
#include	<string.h>
#include	<stdlib.h>
//Crystal files
#include	"CryObject.h"
#include	"ClassException.h"
#include	"ClassContainer.h"
#include	"CryArray.h"


//Class Header

namespace Crystal {
using namespace Crystal;
class CrySet : public CryTemplateArray<int>
{
public:
	int Matches(CrySet &s);	// returns number of elements that match
	int UnMatches(CrySet &s) { return Size() - Matches(s); }
	void Union(CrySet &s);
	void Intersect(CrySet &s);
	void Add(int v);
	void Sub(int v);
	bool IsIn(int v);
	int Pos(int v);	// returns -1 if not present, else returns position of value in set
};


}; // namespace Crystal


//Footer


#endif // CryIntArray
// End of file

