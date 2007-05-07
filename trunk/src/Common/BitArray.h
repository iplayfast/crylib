
//****************************************************
//Copyright 2004
// Crystal Software (Canada) Inc.
//****************************************************

//standard files
#include	<string.h>
#include	<stdlib.h>

//Crystal files
#include	"ClassObject.h"
#include	"ClassMemStream.h"

namespace Crystal {
using namespace Crystal;
#ifndef CBitArray
#define CBitArray	"BitArray"

class BitArray : public MemStream
{
int BitCount;	// number of bits we are dealing with
class BitIterator: public Iterator
{
int BitIndex;
int Index;
public:
	BitIterator(const Container *Parent) : Iterator(Parent) { Index = BitIndex = 0; }
	virtual Object *Dup() const
{
BitIterator *it = (BitIterator *)GetOrigContainer()->CreateIterator();
	it->BitIndex = BitIndex;
	it->Index = Index;
	return it;

}

friend class BitArray;
};

// ***********************************************************************
// Public Functions
// ***********************************************************************
public:

 /*! Default BitArray constructor */

	BitArray();

 /*!  BitArray destructor */

	~BitArray();


// **************************************************************
// Iterator functions available because this is a container class
// **************************************************************

Container::Iterator *LocIterator;


// ***********************************************************************
// Inherited Functions
// ***********************************************************************

	virtual const char *ChildClassName() const
	{
		return CBitArray;
	}

	virtual bool IsA(const char *GetName) const
	{
		return (::strcmp(GetName,CBitArray)==0) ||
		 MemStream::IsA(GetName);
	}


virtual void SetItemOwnerShip(Iterator *I,bool Owned);
virtual void GetEleType(String &Result) const;
virtual void DeleteIterator(Iterator *I) const;
	/*!Copy this class and any parts of it to Dest*/
virtual bool GetBit(int BitIndex);
virtual void SetBit(int BitIndex,bool Value);
virtual void SetNumBitsInArray(int Length) { BitCount = Length; SetLength(Length / 8 + 1); }
virtual void CopyTo(Object &Dest) const;
virtual void Clear();
virtual void Clear(int amount); // 0 == all, 1.. from start, -1..-N from end
virtual size_t Size() const;
virtual size_t GetItemSize(Iterator *I) const;
virtual size_t Count() const;
virtual bool SaveAsText(Iterator *I,String &ToStream) const;
virtual bool LoadAsText(Iterator *I,String &FromStream);
virtual bool IsObject(const Iterator *I) const;
virtual bool GotoPrev(Iterator *I) const;
virtual bool GotoNext(Iterator *I) const;
virtual bool GotoLast(Iterator *Iterator) const;
virtual bool GotoFirst(Iterator *I) const;
virtual bool GetItemOwnerShip(const Iterator *I) const;
void RemoveAtIterator(Iterator *I);
virtual Iterator *_CreateIterator() const
{
	return new BitIterator(this);
}
BitIterator *CreateIterator() const { return new BitIterator(this); }
virtual EmptyObject *GetAtIterator(const Iterator *I) const;
virtual void AddOwned(EmptyObject *Item,size_t Size);
virtual EmptyObject *Add(EmptyObject *Item,size_t Size);
virtual void AddOwned(EmptyObject *Item,size_t Size,int Index);
virtual EmptyObject *Add(EmptyObject *Item,size_t Size,int Index);
virtual Object *Dup() const;
virtual void AddOwned(Object *Item);
virtual Object *Add(Object *Item);
virtual FunctionDefList *GetFunctions(const char *Type=0) const;

// ***********************************************************************
// Inherited Properties
// ***********************************************************************


// --- Functions for setting and retrieving property values ---
// if a Get function returns a char * that is different
// then the Result, it implies a special situation
// (usually a container class)


virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;
virtual bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);
#ifdef VALIDATING
	virtual bool Test(bool Verbose,Object &ThisObject,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

}; // class BitArray



#endif //TBitArray
}
// End of file
