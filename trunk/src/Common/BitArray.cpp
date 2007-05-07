
//****************************************************
//Copyright 2006
// Crystal Software (Canada) Inc.
//****************************************************


#include "BitArray.h"
#include "ClassXML.h"	//Needed for SetDefaultValues
#include "ClassFunction.h"
#include "ClassException.h"

using namespace Crystal;

/*! Default BitArray constructor */

#ifdef VALIDATING
bool BitArray::Test(bool Verbose,Object &ThisObject,bool  (CallBack)(bool Verbose,const char *Result,bool fail))
{

char Result[200];
bool Fail = false;
String spn,spv,stemp;

BitArray Copy;
BitArray *OrgObject = 0;
	ThisObject.CopyTo(Copy);
	if (ThisObject.IsA(CBitArray)) {
		OrgObject=(BitArray *)&ThisObject;
	}
	Fail = Copy!=*OrgObject;
	sprintf(Result,"Copy Constructor");
	if (!CallBack(Verbose,Result,Fail))
		return false;
BitArray Prime;
	Prime.SetNumBitsInArray(5);

	Prime.SetBit(2,1);
	for(int i=0;i<4;i++)
	{
		Fail |= (Prime.GetBit(i)) && i!=2;
	}
	sprintf(Result,"Initialize 0, set and get bit");
	if (!CallBack(Verbose,Result,Fail))
		return false;

	try
	{
		Prime.GetBit(5);
		Fail = true;
		sprintf(Result,"Outof range test");
		if (!CallBack(Verbose,Result,Fail))
			return false;
	}
	catch(Exception &e)
	{
		sprintf(Result,"Outof range test");
		if (!CallBack(Verbose,Result,Fail))
			return false;

	}
	return MemStream::Test(Verbose,ThisObject,CallBack);
}

#endif

//constructor
BitArray::BitArray()
{
	LocIterator = _CreateIterator();
	MemStream::Zero();
}// BitArray


/*!  BitArray destructor */

BitArray::~BitArray()
{
    DeleteIterator(LocIterator);
}// ~BitArray


void BitArray::SetItemOwnerShip(Iterator *I,bool Owned)
{
    throw Exception("BitArray always owns it's bits");
}

void BitArray::GetEleType(String &Result) const
{
    Result = "bool";
}

void BitArray::DeleteIterator(Iterator *I) const
{
    delete I;
}

/*!Copy this class and any parts of it to Dest*/
void BitArray::CopyTo(Object &Dest) const
{
    /* If any data is added to this class uncomment this and add the data accordingly
     
        if (Dest.IsA(TBitArray))
        {
        BitArray *CastDest = (BitArray *)&Dest;
        }
    */

    MemStream::CopyTo(Dest); //Copy Base Class
}
void BitArray::Clear(int amount) // 0 == all, 1.. from start, -1..-N from end
{// todo, optimize this
    if (amount==0)
        Clear();
    else {
        if (amount>0)
            for (int i=0;i<amount;i++)
                SetBit(i,false);
        else {
            int i = GetLength();
            for (amount *= -1;amount<i;amount++) {
                SetBit(amount,false);
            }
        }
    }
}

void BitArray::Clear()
{
    MemStream::Clear();
}
size_t BitArray::Size() const
{
    size_t size = MemStream::Size();
    return size;
}

size_t BitArray::GetItemSize(Iterator *I) const
{
    return sizeof(bool);
}

size_t BitArray::Count() const
{
    return Size() * 8;
}
bool BitArray::SaveAsText(Iterator *I,String &ToStream) const
{
    return MemStream::SaveAsText(I,ToStream);
}
bool BitArray::LoadAsText(Iterator *I,String &FromStream)
{
    return MemStream::LoadAsText(I,FromStream);
}
bool BitArray::IsObject(const Iterator *I) const
{
    return false;
}
bool BitArray::GotoPrev(Iterator *I) const
{
    BitIterator *i = (BitIterator *) I;
    if (i->BitIndex>0) {
        i->BitIndex--;
    } else if (i->Index>0) {
        i->Index--;
        i->BitIndex = 8;
    } else {
        return false;
    }
    return true;
}
bool BitArray::GotoNext(Iterator *I) const
{
    BitIterator *i = (BitIterator *) I;
    if (i->BitIndex<7)
        i->BitIndex++;
    else if ((unsigned int) i->Index < Size()) {
        i->Index++;
    } else
        return false;
    return true;
}
bool BitArray::GotoLast(Iterator *Iterator) const
{
    BitIterator *i = (BitIterator *) Iterator;
    if ((i->Index = Size())==0)
        return false;
    i->BitIndex = 7;
    return true;
}
bool BitArray::GotoFirst(Iterator *I) const
{
    BitIterator *i = (BitIterator *) I;
    i->Index = 0;
    i->BitIndex = 0;
    return true;
}
bool BitArray::GetItemOwnerShip(const Iterator *I) const
{
    return true;
}

void BitArray::RemoveAtIterator(Iterator *I)
{
	return;
}

bool BitArray::GetBit(int BitIndex)
{
	if (BitIndex>=BitCount)
		throw Exception("Range error");

	cbyte Byte = *this[BitIndex / 8];
	return Byte & (1<< (BitIndex & 7));
}

void BitArray::SetBit(int BitIndex,bool Value)
{
	if (BitIndex>=BitCount)
		throw Exception("Range error");
	cbyte Byte = *this[BitIndex / 8];
    if (Value)
        Byte |= (1 << (BitIndex & 7));
    else {
        Byte = ~Byte;
        Byte |= (1 << (BitIndex & 7));
        Byte = ~Byte;
    }
    SetRaw(BitIndex / 8,Byte);
}

EmptyObject *BitArray::GetAtIterator(const Iterator *I) const
{
    throw Exception("Cannot Retrieve EmptyObject * from BitArray");
}
void BitArray::AddOwned(EmptyObject *Item,size_t Size)
{
	AddOwned(Item,Size,-1);
}
EmptyObject *BitArray::Add(EmptyObject *Item,size_t Size)
{
	return Add(Item,Size,-1);
}

void BitArray::AddOwned(EmptyObject *Item,size_t Size,int Index)
{
	throw Exception("Cannot Add EmptyObject * to BitArray");
}

EmptyObject *BitArray::Add(EmptyObject *Item,size_t Size,int Index)
{
    throw Exception("Cannot Add EmptyObject * to BitArray");
}

Object *BitArray::Dup() const
{
    BitArray *New = new BitArray();
    CopyTo(*New);
    return New;
}

void BitArray::AddOwned(Object *Item)
{
    throw Exception("Cannot Add EmptyObject * to BitArray");
}
Object *BitArray::Add(Object *Item)
{
    throw Exception("Cannot Add EmptyObject * to BitArray");
}
FunctionDefList *BitArray::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
    if (Type && !IsA(Type))
        return MemStream::GetFunctions(Type);
    // otherwise get any functions in subclasses
    FunctionDefList *l = MemStream::GetFunctions();
    String s;
    s += "//  BitArray;";
    s += "virtual void SetItemOwnerShip(Iterator *I,bool Owned);";
    s += "virtual void GetEleType(CryString &Result) const;";
    s += "virtual void DeleteIterator(Iterator *I) const;";
    s += "virtual void CopyTo(Object &Dest) const;";
	s += "virtual void Clear();";
	s += "virtual size_t Size() const;";
	s += "virtual size_t GetItemSize(Iterator *I) const;";
	s += "virtual size_t Count() const;";
	s += "virtual bool SaveAsText(Iterator *I,CryString &ToStream) const;";
	s += "virtual bool LoadAsText(Iterator *I,CryString &FromStream);";
	s += "virtual bool IsObject(Iterator *I) const;";
    s += "virtual bool GotoPrev(Iterator *I) const;";
    s += "virtual bool GotoNext(Iterator *I) const;";
    s += "virtual bool GotoLast(Iterator *Iterator) const;";
    s += "virtual bool GotoFirst(Iterator *I) const;";
    s += "virtual bool GetItemOwnerShip(Iterator *I) const;";
    s += "virtual Iterator *_CreateIterator() const;";
    s += "virtual EmptyObject *GetAtIterator(Iterator *I) const;";
    s += "virtual void AddOwned(EmptyObject *Item,size_t Size);";
    s += "virtual EmptyObject *Add(EmptyObject *Item,size_t Size);";
    s += "virtual Object *Dup() const;";
	s += "virtual void AddOwned(Object *Item);";
	s += "virtual Object *Add(Object *Item);";
	s += "virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;";
    s +="virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result)const ;";
    s +="virtual bool SetProperty(const char *PropertyName,CryString &PropertyValue);";
    s +="virtual const char *ChildClassName()const ;";
    s +="virtual bool IsA(const char *GetName)const ;";
    s +="virtual Object *Dup()const ;";
    s +="virtual void CopyTo(Object &Dest)const ;";
    s +="size_t BitArray:: Size()const ;";
    l->LoadFromString(s,";");
    return l;
}


// ***********************************************************************
// Inherited Properties
// ***********************************************************************

const char *BitArray::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
    return MemStream::GetProperty(PropertyName,Result);
} // GetProperty

bool BitArray::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
    return MemStream::SetProperty(PropertyName,PropertyValue);
} // SetProperty



/* BitArray End of file */
