//****************************************************
//Copyright 2004
// Crystal Software (Canada) Inc.
//****************************************************


//Includes


//Class Instance Includes
#ifndef CCrySet
#define CCrySet	"CrySet"
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

class CrySet : public TArray<int>
{
public:
	CrySet(int SetSize=10) : TArray<int>(SetSize) { }
	StdFunctions(CrySet,TArray<int>);
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
	virtual void GetEleType(CryString &Result) const
	{
		Result = "int";	// values in set stored as int
	}
	virtual Object *Create(const CryPropertyParser &PropertyName,Object *Parent=0)
	{
		if (PropertyName==CCrySet)
			return new CrySet();
		else
			return TArray<int>::Create(PropertyName,Parent);
	}
	static Object *ClassCreate(const CryPropertyParser &PropertyName,Object *Parent=0)
	{
		return new CrySet();
	}
	virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
	{
		if (PropertyName=="int")
		{
		CryString v = PropertyValue;
			return LoadAsText((int)Size(),v);
		}
		return TArray<int>::SetProperty(PropertyName,PropertyValue);
	}
	virtual bool SetPropertyAsObject(const CryPropertyParser &PropertyName,Object *Value)
	{
		return TArray<int>::SetPropertyAsObject(PropertyName,Value);
	}
	virtual bool SetPropertyAsObject(CryProperty *Value)
	{
		return TArray<int>::SetPropertyAsObject(Value);
	}
	virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
	{
		return TArray<int>::GetProperty(PropertyName,Result);
	}
	virtual bool HasProperty(const CryPropertyParser &PropertyName)const
	{
		return TArray<int>::HasProperty(PropertyName);
	}
	virtual int GetPropertyCount() const
	{
		return TArray<int>::GetPropertyCount();
	}
	virtual CryPropertyList* PropertyNames() const
	{
		return TArray<int>::PropertyNames();
	}

#ifdef VALIDATING
	virtual bool Test(bool Verbose,Object &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

};


}; // namespace Crystal


//Footer


#endif // CryIntArray
// End of file

