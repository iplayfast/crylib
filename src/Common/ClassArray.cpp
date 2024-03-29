/***************************************************************************
 *   Copyright (C) 2006 by Chris Bruner                                    *
 *   chris@Martha.crystal.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdlib.h>

#include "ClassContainer.h"
#include "ClassArray.h"
#include "ClassFunction.h"
#include "ClassProperty.h"
#include "ClassException.h"
#include "ClassXML.h"
using namespace Crystal;
using namespace std;

//
// CrySimpleArray
//

void SimpleArray::CopyTo(Object &Dest) const ///copies contents of this to Dest
{
	if (Dest.IsA(CSimpleArray))
		CopyTo(*(Array *)&Dest);
    if (Dest.IsA(CMemStream))
    {
        MemStream *mDest = (MemStream *) &Dest;
		mDest->Resize(MaxCount * ElementSize);
        mDest->SetRaw(0,GetRaw(),(size_t)MaxCount * ElementSize);
    }
}
FunctionDefList *SimpleArray::GetFunctions(const char *Type) const
{
    // if a type has been defined and it's not this class, check subclasses for it
    if (Type && !IsA(Type))
		return Container::GetFunctions(Type);
	// otherwise get any functions in subclasses
	FunctionDefList *l = Container::GetFunctions();
	String s;
	char *search;
	s += "// Class CrySimpleArray;";
	s +="virtual void CopyTo(Object &Dest) const;";
	s +="int IteratorValue(const Iterator *I) const;";
	s +="virtual Iterator *_CreateIterator() const;";
	s +="void SetAllowResize(bool v);";
	s +="virtual size_t Size() const;";
	s +="virtual size_t Count() const;";
	s +="virtual CryString *GetFunctions() const;";
	s +="virtual void DeleteIterator(Iterator *AI) const;";
	s +="virtual bool GotoFirst(Iterator *AI) const;";
	s +="virtual bool GotoNext(Iterator *I) const;";
	s +="virtual bool GotoPrev(Iterator *I) const;";
	s +="virtual bool GotoLast(Iterator *I) const;";
	s +="virtual bool LoadAsText(int i,CryString &FromStream) = 0;";
	s +="virtual bool SaveAsText(int i,CryString &ToStream) const = 0;";
    s +="virtual bool LoadAsText(Iterator *I,CryString &FromStream);";
    s +="virtual bool SaveAsText(Iterator *I,CryString &ToStream) const;";
	s +="virtual EmptyObject *GetAtIterator(const Iterator *I) const = 0;";
	s +="virtual void Clear();";
	s +="virtual size_t GetItemSize(Iterator *I) const;";
	s +="virtual void SetMax(size_t m);";
	s +="int GetMax() const;";
	s +="bool GetAllowResize() const;";
	s +="virtual const char* ChildClassName() const;";
	s +="virtual bool IsA(const char *_ClassName) const;";
	s +="virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);";
	s +="virtual bool SetPropertyAsObject(const CryPropertyParser &PropertyName,Object *Value);";
	s +="virtual bool SetPropertyAsObject(const CryProperty *Value);";
	s +="virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;";
	s +="virtual bool HasProperty(const CryPropertyParser &PropertyName)const;";
	s +="virtual int GetPropertyCount() const;";
	s +="virtual CryPropertyList* PropertyNames() const;";


	l->LoadFromString(s,";");
	return l;
}
void SimpleArray::SetMax(size_t m)
{
	MaxCount = m;
}
	/// will set a value to the container[Iterator]
void SimpleArray::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
{
unsigned i = IteratorValue(I);
	SetItem(i,Item,IsObject,IsOwned,Size);
}

bool SimpleArray::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
/*	if (PropertyName=="CurrentCount")
	{
		CurrentCount = atoi(PropertyValue);
		return true;
	}
*/	if (PropertyName=="MaxCount")
	{
		SetSize(atoi(PropertyValue));
		return true;
	}
	if (PropertyName=="ElementSize")
	{
		ElementSize = atoi(PropertyValue);
		return true;
	}
	if (PropertyName=="AllowResize")
	{
		AllowResize = strcasecmp(PropertyValue,"Yes")==0;
		return true;
	}
	return Container::SetProperty(PropertyName,PropertyValue);
}

bool SimpleArray::SetPropertyAsObject(const PropertyParser &PropertyName,Object *Value)
{
	if (Value->IsA(CString))
		return SimpleArray::SetProperty(PropertyName,*(String *)Value);
	return false;
}

bool SimpleArray::SetPropertyAsObject(const Property *Value)
{
	return Container::SetPropertyAsObject(Value);
}

const char *SimpleArray::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	Result.Clear();
/*	if (PropertyName=="CurrentCount")	// not included here since subclasses may need to create dynamically so this property is handled by subclasses
	{
		Result.printf("%d",CurrentCount);
		return Result;
	}
*/	if (PropertyName=="MaxCount")
	{
		Result.printf("%d",MaxCount);
		return Result;
	}
	if (PropertyName=="ElementSize")
	{
		Result.printf("%d",ElementSize);
		return Result;
	}
	if (PropertyName=="AllowResize")
	{

		Result.printf("%s",AllowResize ? "Yes" : "No");
		return Result;
	}
	return Container::GetProperty(PropertyName,Result);
}

bool SimpleArray::HasProperty(const PropertyParser &PropertyName)const
{
	return (//(PropertyName=="CurrentCount") ||
		(PropertyName=="MaxCount") ||
		(PropertyName=="ElementSize") ||
		(PropertyName=="AllowResize") ||
		Container::HasProperty(PropertyName));
}

PropertyList* SimpleArray::PropertyNames() const
{
	PropertyList *n = Container::PropertyNames();
//	n->AddPropertyByName("CurrentCount",this);	//current number of objects in the array
	n->AddPropertyByName("MaxCount",this);	// maximum number of objects that can be contained in the array
	n->AddPropertyByName("ElementSize",this);
	n->AddPropertyByName("AllowResize",this);
	return n;
}


#ifdef VALIDATING
bool SimpleArray::Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail))
{
/* need to create a test for the following functions
StdFunctionsNoDup(CrySimpleArray,CryContainer);
	CrySimpleArray(size_t _ElementSize = 0)
	{
		ElementSize = _ElementSize;
		CurrentCount = MaxCount = 0;
	}
	virtual void CopyTo(Object &Dest) const;  //copies contents of this to Dest
	int IteratorValue(const Iterator *I) const
	{
		return ((ArrayIterator *)I)->i;
	}// if Iterator comes from a subclass it must be an ArrayIiterator
	virtual Iterator *_CreateIterator() const
	{
		ArrayIterator  *AI = new ArrayIterator(this);
		AI->i= 0;
		return AI;
	}
	ArrayIterator *CreateIterator() const
	{
		return (ArrayIterator *)_CreateIterator();
	}
	void SetAllowResize(bool v)
	{
		AllowResize = v;
	}
	virtual size_t Size() const
	{
		return CurrentCount;
	}
	virtual size_t Count() const
	{
		return CurrentCount;
	}
	virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;

	virtual void DeleteIterator(Iterator *AI) const
	{
		delete (ArrayIterator *)AI;
	}
	virtual bool GotoFirst(Iterator *AI) const
	{
		((ArrayIterator *)AI)->i = 0;
		return CurrentCount>0;
	}
	virtual bool GotoNext(Iterator *I) const
	{
		ArrayIterator *AI = (ArrayIterator *)I;
		if (AI->i<CurrentCount-1)
		{
			AI->i++;
			return true;
		}
		else
			return false;
	}
	virtual bool GotoPrev(Iterator *I) const   // returns true if success
	{
		ArrayIterator *AI = (ArrayIterator *)I;
		if ((AI->i-1<CurrentCount) && (AI->i>0))
		{
			AI->i--;
			return true;
		}
		else
			return false;
	}
	virtual bool GotoLast(Iterator *I) const
	{
		ArrayIterator *AI = (ArrayIterator *)I;
		AI->i = CurrentCount-1;
		return CurrentCount > 0;
	}
	virtual bool LoadAsText(int i,CryString &FromStream) = 0;
	virtual bool SaveAsText(int i,CryString &ToStream) const = 0;

	virtual bool LoadAsText(Iterator *I,CryString &FromStream)
	{
		return LoadAsText(((ArrayIterator *)I)->i,FromStream);
	}
	virtual bool SaveAsText(Iterator *I,CryString &ToStream) const
	{
		return SaveAsText(((ArrayIterator *)I)->i,ToStream);
	}

	// this class may be used for other things, so this is not used
	virtual EmptyObject *GetAtIterator(const Iterator *I) const = 0;
	/// will set a value to the container[Iterator]
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0);
	virtual void SetItem(unsigned int i,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0) = 0;
	virtual void RemoveAtIterator(Iterator *LI) = 0;
	virtual bool IsEmpty(const Iterator *I) const { return GetAtIterator(I)==0; }
	virtual void Clear() = 0;
	virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
	virtual bool SetPropertyAsObject(const CryPropertyParser &PropertyName,Object *Value);
	virtual bool SetPropertyAsObject(const CryProperty *Value);
	virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
	virtual bool HasProperty(const CryPropertyParser &PropertyName)const;
	virtual int GetPropertyCount() const
	{
		return CryContainer::GetPropertyCount() + 4;
	}
	virtual CryPropertyList* PropertyNames() const;


	virtual size_t GetItemSize(Iterator *I) const
	{
		return ElementSize;
	}
	virtual void SetMax(size_t m);
	int GetMax() const
	{
		return MaxCount;
	}
	bool GetAllowResize() const
	{
		return AllowResize;
	}*/
	return true;
}
#endif

//
//	CryTemplateArray
//
/*template<typename T>
Object * CryTemplateArray<T>::Dup() const
{
	int i, m = GetMax();
	CryTemplateArray<T> *n = new CryTemplateArray<T>(m);
	for(i=0;i<m;i++)
		n->Values[i] = Values[i];
	return n;
} */
#ifdef VALIDATING
/*template<>
bool CryTemplateArray<int>::Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail))
{
/* need to code tests for the following functions
	StdFunctionsNoDup(CryTemplateArray,CrySimpleArray);
	void SetSize(size_t _Size);
	virtual Object *Dup()const; // creates a duplicate of this object
	CryTemplateArray<T> &Delete(int start,int amount);

	virtual void Clear() { CurrentCount = 0; }

	CryTemplateArray(int _Size=100) : CrySimpleArray(sizeof(T))
	{
		Values = new T[_Size];
		SetMax(_Size);
		SetSize(_Size);
	}
	~CryTemplateArray()
	{
		delete [] Values;
	}

	virtual void RemoveAtIterator(Iterator *LI);
	virtual bool LoadAsText(int i,CryString &FromStream);
	virtual bool SaveAsText(int i,CryString &ToStream) const;
	void SetItem(unsigned int i,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
	{
		SetValue(i,*(T*)Item);
	}

	const T operator [](int i)
	{
		return Values[i];
	}
	void SetValue(int i,T v)
	{
		Values[i] = v;
	}
	T GetValue(int i) const
	{
		if ((i<0) || ((unsigned )i>=CurrentCount))
			throw CryException(this,"Range Error");
		return Values[i];
	}
	virtual bool IsEmpty(const Iterator *I) const {  return ((ArrayIterator *)I)->i>CurrentCount; }

	return CryContainer::Test(Verbose,Object,CallBack);
}
*/
#endif
/*
template<typename T>
void Crystal::CryTemplateArray<T>::SetSize(size_t _Size)
{
	T *n = new T[_Size];
	SetMax(_Size);
	int CopyAmount = CurrentCount;
	if (_Size<CurrentCount)
		CopyAmount = _Size;
	for(int i=0;i<CopyAmount;i++)
		n[i] = Values[i];
//	for(unsigned int i=CopyAmount;i<_Size;i++)
//		n[i] = 0.0;  // default value for <T>?
	delete []Values;
	CurrentCount = _Size;
	Values = n;
}*/
/*template<typename T>
CryTArray<T> &CryTArray<T>::Delete(int start,int amount)
{
	if (start>=Size()) return *this;
	while(start<Size())
	{
		if (start+amount>=Size()) {
			CurrentCount=start;
			return *this;
		}
		Values[start] = Values[start+amount];
		start++;
	}
	return *this;
}

  */

//
// CryArray
//
Array::Array(unsigned int ElementSize) : SimpleArray(ElementSize)
{
	pPtr = 0;
	AllowResize = true;
}

Array::~Array()
{
	if (Count()!=0)
		throw Exception(this, "deleting CryArray, while still having some elements! (Call Clear() first)");

    delete []pPtr;
}
FunctionDefList *Array::GetFunctions(const char *Type) const
{
    // if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
        return SimpleArray::GetFunctions(Type);
    // otherwise get any functions in subclasses
    FunctionDefList *l = SimpleArray::GetFunctions();
    String s;
    s += "//  Class CryArray;";
    char *search;
    s +="virtual void DestroyArrayItem(CryArray *Owner,EmptyObject *Item) = 0;";
    s +="virtual EmptyObject *CreateArrayItem(CryArray *Owner,bool *IsObject) = 0;";
    s +="virtual void SaveItemTo(const CryArray *Owner,EmptyObject *FromItem,CryStream &ToStream) const = 0;";
    s +="virtual EmptyObject *LoadItemFrom(CryArray *Owner,EmptyObject *ToItem,CryStream &FromStream) = 0;";
    s +="virtual CryString *GetFunctions() const;";
    s +="virtual bool IsA(const char *_ClassName) const;";
    s +="void clear();";
    s +="EmptyObject *GetItem(unsigned int i) const;";
    s +="const void *operator [](int i);";
    s +="void SetMax(size_t m);";
    s +="void SetSize(size_t n);";
    s +="int DeleteItem(unsigned int i);";
    s +="virtual void CopyTo(CryArray &Dest) const;";
    s +="void Sort(int (Compare) (const void *ele1,const void *ele2));";
    s +="virtual void CopyTo(Object &Dest) const;";
    s +="virtual Object *Dup()const;";
    s +="virtual const cbyte* GetRaw() const;";
#ifdef VALIDATING

    s +="virtual bool Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool Fail));";
#endif

    s +="virtual const char *ChildClassName() const;";
	s +="virtual bool SetProperty(const char *PropertyName,const CryString &PropertyValue);";
    s +="virtual bool SetPropertyAsObject(const char *PropertyName,Object *Value);";
	s +="virtual bool SetPropertyAsObject(const CryProperty *Value);";
    s +="virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;";
    s +="virtual bool HasProperty(const char *PropertyName)const;";
	s +="virtual int GetPropertyCount() const;";
	s +="virtual CryPropertyList* PropertyNames() const;";
	s +="virtual EmptyObject *GetAtIterator(const Iterator *I) const;";
    s +="virtual bool LoadAsText(int i,CryString &FromStream) = 0;";
    s +="virtual bool SaveAsText(int i,CryString &ToStream) const = 0;";
    s +="virtual bool LoadAsText(Iterator *I,CryString &FromStream);";
    s +="virtual bool SaveAsText(Iterator *I,CryString &ToStream) const;";
    s +="virtual EmptyObject *Add(EmptyObject *Item,size_t Size);";
    s +="virtual void AddOwned(EmptyObject *Item,size_t Size);";
    s +="virtual Object *Add(Object *Item);";
    s +="virtual void AddOwned(Object *Item);";
    s +="virtual bool IsEmpty(const Iterator *I) const;";
    s +="virtual void SetItemOwnerShip(Iterator *I,bool Owned);";
    s +="virtual bool GetItemOwnerShip(const Iterator *I) const;";
    s +="virtual bool IsObject(const Iterator *I) const;";
    s +="virtual void SetIsObject(Iterator *I,bool IsCO);";
    s +="size_t GetItemSize(Iterator *I) const;";
    s +="virtual void Clear();";
    l->LoadFromString(s,";");
    return l;
}
void Array::Clear()
{
    int i,j;
    j = Count();
    for(i=0;i<j;i++)
    {
        EmptyObject *item = pPtr[i].Item;
        DestroyArrayItem(this,item);
    }
    CurrentCount = 0;
}

void Array::DeleteItemOffset(unsigned int i)
{
    while(i<Count()-1)
    {
		pPtr[i] = pPtr[i+1];
        i++;
    }
    CurrentCount--;
}


int Array::DeleteItem(unsigned int i)
{
	EmptyObject *item = GetItem(i);
	DestroyArrayItem(this,item);
	DeleteItemOffset(i);
	return CurrentCount;
}
void Array::RemoveAtIterator(Iterator *LI)
{
//	EmptyObject *r;
	int i = IteratorValue(LI);
	if (!LI->IsOwned())
	{
//		r = GetAtIterator(LI);
		GotoPrev(LI);
		DeleteItemOffset(i);
	}
	else
	{
 //		r = 0;
		LI->GotoPrev();
		DeleteItem(i);
	}
}


void Array::SetMax(size_t m)
{
    ElePtr *NewPtr = new ElePtr[m];
	size_t i;
    for(i=0;i<m;i++)
        NewPtr[i].Item = 0;
	for(i=m;i<MaxCount;i++)
	{
		DestroyArrayItem(this,pPtr[i].Item);
	}
	if (MaxCount>m)
		MaxCount = m;
	for(i=0;i<MaxCount;i++)
	{
		NewPtr[i].Item = pPtr[i].Item;
		NewPtr[i].IsObject = pPtr[i].IsObject;
		NewPtr[i].Size = pPtr[i].Size;
		NewPtr[i].IsOwned = pPtr[i].IsOwned;
		NewPtr[i].IsAutoCreated = pPtr[i].IsAutoCreated;
	}
	SimpleArray::SetMax(m);
	if (CurrentCount>MaxCount)
		CurrentCount = MaxCount;
	delete []pPtr;
	pPtr = NewPtr;
}

void Array::SetSize(size_t n) // set the number currently in use (either grow or shrink)
{
	if (n>MaxCount)
	{
		if (AllowResize)
		{
			SetMax(MaxCount + n);
		}
		else
			throw(Exception(String("Setting Current Array Count %d  higher then MaxArray Count %d",n,MaxCount)));
	}
	while (CurrentCount>n)
	{
		if (pPtr[CurrentCount].IsOwned)
		{
			if (pPtr[CurrentCount].IsAutoCreated)
				DestroyArrayItem(this,pPtr[CurrentCount].Item);
			else
				delete pPtr[CurrentCount].Item;
		}
        pPtr[CurrentCount].Item = 0;
        CurrentCount--;
    }

    while(CurrentCount<n)
    {
        bool IsObject;
        pPtr[CurrentCount].Item = CreateArrayItem(this,&IsObject);
        pPtr[CurrentCount].IsOwned = true;
        pPtr[CurrentCount].IsAutoCreated = true;
        pPtr[CurrentCount].IsObject = IsObject;
        pPtr[CurrentCount].Size = 0;			// not used if IsAutoCreated
        CurrentCount++;
    }
}
bool Array::IsEmpty(const Iterator *I) const
{
    int i = IteratorValue(I);
    return pPtr[i].Item==0;
}
bool Array::IsObject(const Iterator *I) const
{
    int i = IteratorValue(I);
    return pPtr[i].IsObject;
}
void Array::SetIsObject(Iterator *I,bool IsCO)
{
    int i = IteratorValue(I);
    pPtr[i].IsObject = IsCO;
}
size_t Array::GetItemSize(Iterator *I) const
{
    int i = IteratorValue(I);
    return pPtr[i].Size;
}

void Array::GetEleType(String &Result) const
{
	Result = "Array::Eleptr";
}

void Array::SetItemOwnerShip(Iterator *I,bool IsOwned)
{
	int i = IteratorValue(I);
	pPtr[i].IsOwned = IsOwned;
}

bool Array::GetItemOwnerShip(const Iterator *I) const
{
	int i = IteratorValue(I);
	return pPtr[i].IsOwned;
}

EmptyObject *Array::Add(EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
{
	if (Item==0)
		throw Exception(this,"Add Null ptr added");

	size_t n = CurrentCount + 1;
	if (n>MaxCount)
	{
		if (AllowResize)
		{
			SetMax(MaxCount + 100);
		}
		else
			throw(Exception(String("Setting Current Array Count %d  higher then MaxArray Count %d",n,MaxCount)));
	}
	pPtr[CurrentCount].Item = Item;
	pPtr[CurrentCount].Size = Size;
	pPtr[CurrentCount].IsOwned = IsOwned;
	pPtr[CurrentCount].IsObject = IsObject;
	pPtr[CurrentCount].IsAutoCreated = false;
	CurrentCount = n;
	return Item;
}
EmptyObject *Array::AddIndex(EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size,int Index)
{
	SetItem(Index,Item,IsObject,IsOwned,Size);
   	return Item;
}

EmptyObject *Array::GetItem(unsigned int i)const //indexed from 0
{
	if (i>=CurrentCount)
	{
		if (CurrentCount==0)
			throw(Exception("Accessing empty Array"));
		else
			throw(Exception("Accessing past end of Array"));
	}
	return pPtr[i].Item;
}

void Array::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
{
unsigned i = IteratorValue(I);
	SetItem(i,Item,IsObject,IsOwned,Size);
}

void Array::SetItem(unsigned int i,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
{
	if (i==CurrentCount && AllowResize)	// allow a resize of 1 greater then current amount
	{
		Add(Item,IsObject,IsOwned,Size);
		return;
	}

	if (i>=CurrentCount)
	{
		if (CurrentCount==0)
			throw(Exception("Accessing empty Array"));
		else
			throw(Exception("Accessing past end of Array"));
	}
	if (pPtr[i].IsOwned)
	{
		EmptyObject *item = GetItem(i);
		DestroyArrayItem(this,item);
	}
	else
	{
		EmptyObject *item = GetItem(i);
		DestroyArrayItem(this,item);
	}
	pPtr[i].Item = Item;
	pPtr[i].Size = Size;
	pPtr[i].IsOwned = IsOwned;
	pPtr[i].IsObject = IsObject;
	pPtr[i].IsAutoCreated = false;
}

void Array::Sort(int (*Compare) (const void *ele1,const void *ele2))
{
    qsort(pPtr,CurrentCount,sizeof(void *),Compare);
}

EmptyObject *Array::DupItem(const Array::ElePtr  *Node) const
{
    if (Node->IsObject)
    {
        return (EmptyObject *)((Object *) Node->Item)->Dup();
    }
    else
    {
        char *temp = new char[Node->Size];
        EmptyObject *e = (EmptyObject *)temp;
        memcpy(temp,Node->Item,Node->Size);
        return e;
    }
}


void Array::CopyTo(Array &Dest) const ///copies contents of this to Dest
{
    if (Dest.ElementSize!=ElementSize)
        throw(Exception("CopyTo: Destination Element size different then source"));
    if (Dest.MaxCount<CurrentCount)
    {
        if (!Dest.AllowResize)
            throw(Exception("CopyTo: Fixed Size Destination Array too small"));
        else
            Dest.SetSize(CurrentCount);
    }
    for(unsigned int i=0;i<CurrentCount;i++)
    {
        String s;
        SaveAsText(i,s);
        Dest.LoadAsText(i,s);
    }
    Dest.CurrentCount = CurrentCount;
}
void Array::CopyTo(Object &Dest) const ///copies contents of this to Dest
{
	if (Dest.IsA(CArray))
		CopyTo(*(Array *)&Dest);
	if (Dest.IsA(CMemStream))
    {
        MemStream *mDest = (MemStream *) &Dest;
        mDest->Resize(MaxCount * ElementSize);
        mDest->SetRaw(0,GetRaw(),(size_t)MaxCount * ElementSize);
    }
}

PropertyList *Array::PropertyNames() const
{
	PropertyList *n = Container::PropertyNames();
	n->AddPropertyByName("Size",this);
	return n;
}
bool Array::HasProperty(const PropertyParser &PropertyName) const
{
	/*    const char *name = ChildClassName();
		int l = strlen(name);
		if (strncmp(name,PropertyName,l)==0)*/
	{
		//        name = PropertyName + l;
		if (PropertyName=="Size")
			return true;
	}
    return Container::HasProperty(PropertyName);
}

const char *Array::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	Result.Clear();
	if (PropertyName=="Size")
	{
		Result.printf("%d",Count());
		return Result;
	}
	return Container::GetProperty(PropertyName,Result);
}

bool Array::SetPropertyAsObject(const PropertyParser &PropertyName,Object *Value)
{
	if (Value->IsA(CString))
		return SimpleArray::SetProperty(PropertyName,*(String *)Value);
	return false;
}

bool Array::SetPropertyAsObject(const Property *Value)
{
	return SimpleArray::SetPropertyAsObject(Value);
}

bool Array::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Size")
	{
		if (strlen(PropertyValue)==0)
			throw Exception(this,"Cannot Set Size to no value");
		int s = atoi(PropertyValue);
		this->SetSize(s);
		return true;
	}
/*	if (PropertyName=="Values")
	{
		if (strlen(PropertyValue)==0)
			throw Exception(this,"Cannot Set Value to no value");
		//        this->SaveItemTo(this,NULL,PropertyValue);
		return true;
	}
*/
	return Container::SetProperty(PropertyName,PropertyValue);
}


#ifdef VALIDATING
bool Array::Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail))
{
	return Container::Test(Verbose,Object,CallBack);
}
#endif



//
// IntArray
//
//Constructor/Destructor Functions

IntArray::IntArray(int _SetSize) : SimpleArray(sizeof(int))
{
	Values = new int[_SetSize];
	SetMax(_SetSize);
	SetSize(_SetSize);
}
void IntArray::SetSize(size_t _Size) // set the number currently in use (either grow or shrink)
{
int *n = new int[_Size];
	SetMax(_Size);
	int CopyAmount = CurrentCount;
	if (_Size<CurrentCount)
		CopyAmount = _Size;
	for(int i=0;i<CopyAmount;i++)
		n[i] = Values[i];
	for(unsigned int i=CopyAmount;i<_Size;i++)
		n[i] = 0;
	delete []Values;
	CurrentCount = _Size;
	Values = n;
}



IntArray::~IntArray()
{
		delete [] Values;
}



//Inherited Functions
void IntArray::DestroyArrayItem(Array *Owner,EmptyObject *Item)
{
	delete Item;
}

EmptyObject *IntArray::CreateArrayItem(Array *Owner,bool *IsObject)
{
	if (IsObject) {
		throw Exception(this,"Cannot create Object for Set");
	}
	return (EmptyObject *)new int;
}


bool IntArray::LoadAsText(int i,String &FromStream)
{
	int v;
	FromStream.scanf("%d ",&v);
	SetValue(i,v);
	return true;
}

bool IntArray::SaveAsText(int i,String &ToStream) const
{
	ToStream.printf("%d ",Values[i]);
	return true;
}

void IntArray::GetEleType(String &Result) const
{
	Result = "int";
}


EmptyObject *IntArray::GetAtIterator(const Iterator *I) const
{
const SimpleArray::ArrayIterator *ai = (const SimpleArray::ArrayIterator *)I;
	return (EmptyObject *)&Values[ai->i];
}
void IntArray::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
{
unsigned i = IteratorValue(I);
	SetItem(i,Item,IsObject,IsOwned,Size);
}

void IntArray::SetItem(unsigned int i,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
{
		SetValue(i,*(int*)Item);
}

void IntArray::RemoveAtIterator(Iterator *LI)
{

}

void IntArray::Clear()
{

}
EmptyObject *IntArray::Add(EmptyObject *Item,size_t Size)
{
	throw Exception(this,"EmptyObject *IntArray::Add(EmptyObject *Item,size_t Size) not implemented");
//	return SimpleArray::Add(Item,Size);
}
void IntArray::AddOwned(EmptyObject *Item,size_t Size)
{
	throw Exception(this,"EmptyObject *IntArray::AddOwned(EmptyObject *Item,size_t Size) not implemented");
//	return SimpleArray::Add(Item,sizeof(int));
}
Object *IntArray::Add(Object *Item)    // returns Item
{
	throw Exception(this,"Object *IntArray::Add(Object *Item)    // returns Item not implemented");
//	return SimpleArray::Add(Item);
}
void IntArray::AddOwned(Object *Item)   // gives ownership to list
{
	throw Exception(this,"Object *IntArray::AddOwned(Object *Item)   // gives ownership to list not implemented");
//	return SimpleArray::Add(Item);
}
EmptyObject *IntArray::Add(EmptyObject *Item,size_t Size,int Index)
{
	if (sizeof(int)!=Size)
		throw Exception(this,"Trying to add non-int to IntArray");
int v = *(int *)Item;
	Values[Index]=v;
	return Item;
}
void IntArray::AddOwned(EmptyObject *Item,size_t Size,int Index)
{
	delete Add(Item,Size,Index);
}
Object *IntArray::Add(Object *Item,int Index)    // returns Item
{
	throw Exception(this,"Cannot add Object to IntArray");
}
void IntArray::AddOwned(Object *Item,int Index)   // gives ownership to list
{
	throw Exception(this,"Cannot add Object to IntArray");
}
void IntArray::SetItemOwnerShip(Iterator *I,bool Owned)
{
	return;
//	SimpleArray::SetItemOwnerShip(I,Owned);
}
bool IntArray::GetItemOwnerShip(const Iterator *I) const
{		// ints are alway owned
//	return SimpleArray::GetItemOwnerShip(I);
	return true;
}
bool IntArray::IsObject(const Iterator *I) const
{
 return false;	// integers are not objects
}
//bool LoadAsText(Iterator *I,CryString &FromStream) = 0;
//bool SaveAsText(Iterator *I,CryString &ToStream) const = 0;

/*!Get the names of all properties of this class*/
PropertyList *IntArray::PropertyNames() const
{
// get base class's properties
PropertyList *Names = SimpleArray::PropertyNames();
// now add our own properties (if any)
	Names->AddPropertyByName("Size",this);
// properties that are local to this class
	return Names;
}


void IntArray::CopyTo(Object &Dest) const
{

	if (Dest.IsA(CIntArray))
	{
		// Copy this classes variables first
	IntArray *CastDest = (IntArray *)&Dest;
		CastDest->SetSize(Size());
		for(size_t i = 0;i<Size();i++)
			CastDest->SetValue(i,GetValue(i));
		return;

	}
//Now copy the base class
	SimpleArray::CopyTo(Dest);

}//CryIntArray


bool IntArray::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Size")
	{
		if (strlen(PropertyValue)==0)
			throw Exception(this,"Cannot Set Size to no value");
		int s = atoi(PropertyValue);
		SetSize(s);
		return true;
	}
	if (SimpleArray::SetProperty(PropertyName,PropertyValue))
		return true;
	return false;
}


const char *IntArray::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (PropertyName=="Size")
	{
		Result.printf("%d ",CurrentCount);
		return Result;
	}
	return SimpleArray::GetProperty(PropertyName,Result);
}


//
//	DoubleArray
//
void DoubleArray::GetEleType(String &Result) const
{
	Result = "double";
};

Object * DoubleArray::Dup() const
{
	int i, m = GetMax();
	DoubleArray *n = new DoubleArray(m);
	for(i=0;i<m;i++)
		n->Values[i] = Values[i];
	return n;
}

	/// will set a value to the container[Iterator]
void DoubleArray::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
{
unsigned i = IteratorValue(I);
	SetItem(i,Item,IsObject,IsOwned,Size);
}

bool DoubleArray::LoadAsText(int i,String &FromStream)
{
	double v;
	FromStream.scanf("%f ",&v);
	SetValue(i,v);
	return true;
}
                                        
bool DoubleArray::SaveAsText(int i,String &ToStream) const
{
	ToStream.printf("%f ",Values[i]);
	return true;
}

void DoubleArray::SetSize(size_t _Size)
{
	double *n = new double[_Size];
	SetMax(_Size);
	int CopyAmount = CurrentCount;
	if (_Size<CurrentCount)
		CopyAmount = _Size;
	for(int i=0;i<CopyAmount;i++)
		n[i] = Values[i];
	for(unsigned int i=CopyAmount;i<_Size;i++)
		n[i] = 0.0;
	delete Values;
	CurrentCount = _Size;
	Values = n;
}
bool DoubleArray::SetPropertyAsObject(const PropertyParser &PropertyName,Object *Value)
{
	if (Value->IsA(CString))
		return SimpleArray::SetProperty(PropertyName,*(String *)Value);
	return false;
}
FunctionDefList *DoubleArray::GetFunctions(const char *Type) const
{
	// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
		return SimpleArray::GetFunctions(Type);
	// otherwise get any functions in subclasses
	FunctionDefList *l = SimpleArray::GetFunctions();
	String s;
	s += "// Class DoubleArray;";
	char *search;
	s +="void SetSize(size_t _Size);";
	s +="String *GetFunctions() const;";
	s +="virtual Object *Dup()const;";
	s +="virtual void DestroyArrayItem(Array *Owner,EmptyObject *Item);";
	s +="virtual EmptyObject *CreateArrayItem(Array *Owner,bool *IsObject);";
	s +="virtual bool LoadAsText(int i,String &FromStream);";
	s +="virtual bool SaveAsText(int i,String &ToStream) const;";
	s +="virtual bool LoadAsText(Iterator *I,String &FromStream);";
	s +="virtual bool SaveAsText(Iterator *I,String &ToStream) const;";
	s +="const double operator [](int i);";
	s +="void SetValue(int i,double v);";
	s +="double GetValue(int i) const;";
	s +="void Clear();";
	s +="virtual bool SetProperty(const char *PropertyName,const String &PropertyValue);";
	s +="virtual bool SetPropertyAsObject(const char *PropertyName,Object *Value);";
	s +="virtual bool SetPropertyAsObject(const Property *Value);";
	s +="virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;";
	s +="virtual bool HasProperty(const char *PropertyName)const;";
	s +="virtual int GetPropertyCount() const;";
	s +="virtual PropertyList* PropertyNames() const;";
	s +="virtual const char* ChildClassName() const;";
	s +="virtual bool IsA(const char *_ClassName) const;";
	l->LoadFromString(s,";");
	return l;
}

// if this class contains the property name, it will attempt to load it
// if all is well returns true
bool DoubleArray::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Size")
	{
		if (strlen(PropertyValue)==0)
			throw Exception(this,"Cannot Set Size to no value");
		int s = atoi(PropertyValue);
		SetSize(s);
		return true;
	}
	if (SimpleArray::SetProperty(PropertyName,PropertyValue))
		return true;
	return false;
}

void DoubleArray::RemoveAtIterator(Iterator *LI)
{
	unsigned int i = IteratorValue(LI);
	for(;i<CurrentCount-1;i++)
	{
		Values[i] = Values[i+1];
	}
	return;
}

const char *DoubleArray::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (PropertyName=="Size")
	{
		Result.printf("%d",CurrentCount);
		return Result.AsPChar();
	}
	/*if (strcmp(PropertyName,"Values")==0)	// let Container Class handle it
	{

	}*/
	return SimpleArray::GetProperty(PropertyName,Result);
}
bool DoubleArray::HasProperty(const PropertyParser &PropertyName)const
{
	if (PropertyName=="Size")
		return true;
	return SimpleArray::HasProperty(PropertyName);
}

int DoubleArray::GetPropertyCount() const
{
	return SimpleArray::GetPropertyCount() + 1;
}

PropertyList *DoubleArray::PropertyNames() const
{
	PropertyList *n = SimpleArray::PropertyNames();
	n->AddPropertyByName("Size",this);
	return n;
}
