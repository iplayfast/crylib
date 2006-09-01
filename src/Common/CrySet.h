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
class CryTArray<int> test;

class CrySet : public CryTArray<int>
{
public:
	StdFunctions(CrySet,CryTArray<int>);
	int Matches(CrySet &s);	// returns number of elements that match
	int UnMatches(CrySet &s) { return Size() - Matches(s); }
	void Union(CrySet &s);
	void Intersect(CrySet &s);
	void Add(int v);
	void Sub(int v);
	bool Has(int v);
	int Pos(int v);	// returns -1 if not present, else returns position of value in set
	bool operator==(CrySet &s);
	bool operator!=(CrySet &s) { return ! (*this==s);};
#ifdef VALIDATING
	virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

};


}; // namespace Crystal


//Footer


#endif // CryIntArray
// End of file

