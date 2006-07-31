
//****************************************************
//Copyright 2004
// Crystal Software (Canada) Inc.
//****************************************************

//standard files
#include	<string.h>
#include	<stdlib.h>

//Crystal files
#include	"CryObject.h"
#include	"ClassMemStream.h"

namespace Crystal {
using namespace Crystal;
#ifndef TBitArray
#define TBitArray	"BitArray"

class BitArray : public CryMemStream
{

class BitIterator: public Iterator
{
int BitIndex;
int Index;
public:
	BitIterator(const CryContainer *Parent) : Iterator(Parent) { Index = BitIndex = 0; }
	virtual CryObject *Dup() const
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

CryContainer::Iterator *LocIterator;


// ***********************************************************************
// Inherited Functions
// ***********************************************************************

	virtual const char *ChildClassName() const
	{
		return TBitArray;
	}

	virtual bool IsA(const char *GetName) const
	{
		return (::strcmp(GetName,TBitArray)==0) ||
		 CryMemStream::IsA(GetName);
	}


virtual void SetItemOwnerShip(Iterator *I,bool Owned);
virtual void GetEleType(CryString &Result) const;
virtual void DeleteIterator(Iterator *I) const;
	/*!Copy this class and any parts of it to Dest*/
virtual bool GetBit(int BitIndex);
virtual void SetBit(int BitIndex,bool Value);
virtual void SetNumBitsInArray(int Length) { SetLength(Length / 8 + 1); }
virtual void CopyTo(CryObject &Dest) const;
virtual void Clear();
virtual void Clear(int amount); // 0 == all, 1.. from start, -1..-N from end
virtual size_t Size() const;
virtual size_t GetItemSize(Iterator *I) const;
virtual size_t Count() const;
virtual bool SaveAsText(Iterator *I,CryString &ToStream) const;
virtual bool LoadAsText(Iterator *I,CryString &FromStream);
virtual bool IsCryObject(const Iterator *I) const;
virtual bool GotoPrev(Iterator *I) const;
virtual bool GotoNext(Iterator *I) const;
virtual bool GotoLast(Iterator *Iterator) const;
virtual bool GotoFirst(Iterator *I) const;
virtual bool GetItemOwnerShip(const Iterator *I) const;
void BitArray::RemoveAtIterator(Iterator *I);
virtual Iterator *_CreateIterator() const
{
	return new BitIterator(this);
}
BitIterator *CreateIterator() const { return new BitIterator(this); }
virtual EmptyObject *GetAtIterator(const Iterator *I) const;
virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size);
virtual EmptyObject *Add(EmptyObject *Item,size_t Size);
virtual CryObject *Dup() const;
virtual CryObject *AddOwned(CryObject *Item);
virtual CryObject *Add(CryObject *Item);
virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;

// ***********************************************************************
// Inherited Properties
// ***********************************************************************


// --- Functions for setting and retrieving property values ---
// if a Get function returns a char * that is different
// then the Result, it implies a special situation
// (usually a container class)


virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);

}; // class BitArray



#endif //TBitArray
}
// End of file
