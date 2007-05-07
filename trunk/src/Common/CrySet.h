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
#include	"ClassObject.h"
#include	"ClassException.h"
#include	"ClassContainer.h"
#include	"ClassArray.h"


//Class Header

namespace Crystal {
using namespace Crystal;

class CrySet : public TArray<int>
{
	virtual Object *Add(Object *Item,int Index) // not implemented
	{
		throw Exception(this,"Set's don't have indexes");
	}
	virtual EmptyObject *Add(EmptyObject *Item,size_t Size,int Index)
	{
		throw Exception(this,"Set's don't have indexes");
	}

	virtual Object *Add(Object *Item)
	{
		if (Item->IsA(CCrySet))
		{
			Union(*(CrySet *)Item);
			return Item;
		}
		throw Exception(this,"Cannot Add to Set");
	}



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
	virtual void GetEleType(String &Result) const
	{
		Result = "int";	// values in set stored as int
	}
	virtual Object *Create(const PropertyParser &PropertyName,Object *Parent=0)
	{
		if (PropertyName==CCrySet)
			return new CrySet();
		else
			return TArray<int>::Create(PropertyName,Parent);
	}
	static Object *ClassCreate(const PropertyParser &PropertyName,Object *Parent=0)
	{
		return new CrySet();
	}
	virtual bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
	{
		if (PropertyName=="int")
		{
		String v = PropertyValue;
			return LoadAsText((int)Size(),v);
		}
		if (PropertyName=="Size")
		{
			SetAllowResize(true);
			CurrentCount = 0;
			int iCurrentCount = atoi(PropertyValue);
			SetSize(iCurrentCount);
			SetMax(iCurrentCount);
			CurrentCount = iCurrentCount;
			return true;
		}

		return TArray<int>::SetProperty(PropertyName,PropertyValue);
	}
	virtual bool SetPropertyAsObject(const PropertyParser &PropertyName,Object *Value)
	{
		return TArray<int>::SetPropertyAsObject(PropertyName,Value);
	}
	virtual bool SetPropertyAsObject(Property *Value)
	{
		return TArray<int>::SetPropertyAsObject(Value);
	}
	virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const
	{
		if (PropertyName=="Size")
		{
			return TArray<int>::GetProperty("CurrentCount",Result);
		}
		return TArray<int>::GetProperty(PropertyName,Result);
	}
	virtual bool HasProperty(const PropertyParser &PropertyName)const
	{
		if (PropertyName=="Size") return true;
		return TArray<int>::HasProperty(PropertyName);
	}
	virtual int GetPropertyCount() const
	{
		return Object::GetPropertyCount() +2;
	}
	virtual PropertyList* PropertyNames() const
	{
	PropertyList *list = Object::PropertyNames();
		list->AddPropertyByName("Size",this);
		list->AddPropertyByName("Values",this);
		return list;
	}
	virtual EmptyObject *Add(EmptyObject *Item,size_t Size)
	{
		if (Size==sizeof(int))	// make an assumption
		{
			Add(*(int *)Item);
			return Item;
		}
		throw Exception(this,"Don't know how to Add this to Set");
	}

	/// create an object (or container of objects) from the stream
	virtual Object *Create(Stream &FromStream)
	{
		return OwnedObject::Create(FromStream);
	}

#ifdef VALIDATING
	virtual bool Test(bool Verbose,Object &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

};


}; // namespace Crystal


//Footer


#endif // CryIntArray
// End of file

