/***************************************************************************
 *   Copyright (C) 2003 by Chris Bruner                                    *
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
#include "CryArray.h"
#include "ClassFunction.h"
#include "ClassProperty.h"
#include "ClassException.h"
#include "CryXML.h"
using namespace Crystal;
using namespace std;

//
// CrySimpleArray
//

void CrySimpleArray::CopyTo(CryObject &Dest) const ///copies contents of this to Dest
{
    if (Dest.IsA(TCrySimpleArray))
		CopyTo(*(CryArray *)&Dest);
    if (Dest.IsA(TCryMemStream))
    {
        CryMemStream *mDest = (CryMemStream *) &Dest;
		mDest->Resize(MaxCount * ElementSize);
        mDest->SetRaw(0,GetRaw(),(size_t)MaxCount * ElementSize);
    }
}
CryFunctionDefList *CrySimpleArray::GetFunctions(const char *Type) const
{
    // if a type has been defined and it's not this class, check subclasses for it
    if (Type && !IsA(Type))
		return CryContainer::GetFunctions(Type);
	// otherwise get any functions in subclasses
	CryFunctionDefList *l = CryContainer::GetFunctions();
	CryString s;
	char *search;
	s += "// Class CrySimpleArray;";
	s +="virtual void CopyTo(CryObject &Dest) const;";
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
	s +="virtual bool SetPropertyAsObject(const CryPropertyParser &PropertyName,CryObject *Value);";
	s +="virtual bool SetPropertyAsObject(CryProperty *Value);";
	s +="virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;";
	s +="virtual bool HasProperty(const CryPropertyParser &PropertyName)const;";
	s +="virtual int GetPropertyCount() const;";
	s +="virtual CryPropertyList* PropertyNames() const;";


	l->LoadFromString(s,";");
	return l;
}
void CrySimpleArray::SetMax(size_t m)
{
	MaxCount = m;
}
	/// will set a value to the container[Iterator]
void CrySimpleArray::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size)
{
unsigned i = IteratorValue(I);
	SetItem(i,Item,IsCryObject,IsOwned,Size);
}

bool CrySimpleArray::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="CurrentCount")
	{
		CurrentCount = atoi(PropertyValue);
		return true;
	}
	if (PropertyName=="MaxCount")
	{
		MaxCount = atoi(PropertyValue);
		return true;
	}
	if (PropertyName=="ElementSize")
	{
		ElementSize = atoi(PropertyValue);
		return true;
	}
	if (PropertyName=="AllowResize")
	{
		AllowResize = atoi(PropertyValue);
		return true;
	}
	return CryContainer::SetProperty(PropertyName,PropertyValue);
}

bool CrySimpleArray::SetPropertyAsObject(const CryPropertyParser &PropertyName,CryObject *Value)
{
	if (Value->IsA(TCryString))
		return CrySimpleArray::SetProperty(PropertyName,*(CryString *)Value);
	return false;
}

bool CrySimpleArray::SetPropertyAsObject(CryProperty *Value)
{
	return CryContainer::SetPropertyAsObject(Value);
}

const char *CrySimpleArray::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
	Result.Clear();
	if (PropertyName=="CurrentCount")
	{
		Result.printf("%d",CurrentCount);
		return Result;
	}
	if (PropertyName=="MaxCount")
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
		Result.printf("%d",AllowResize);
		return Result;
	}
	return CryContainer::GetProperty(PropertyName,Result);
}

bool CrySimpleArray::HasProperty(const CryPropertyParser &PropertyName)const
{
	return ((PropertyName=="CurrentCount") ||
		(PropertyName=="MaxCount") ||
		(PropertyName=="ElementSize") ||
		(PropertyName=="AllowResize") ||
		CryContainer::HasProperty(PropertyName));
}

CryPropertyList* CrySimpleArray::PropertyNames() const
{
	CryPropertyList *n = CryContainer::PropertyNames();
	n->AddPropertyByName("CurrentCount",this);	//current number of objects in the array
	n->AddPropertyByName("MaxCount",this);	// maximum number of objects that can be contained in the array
	n->AddPropertyByName("ElementSize",this);
	n->AddPropertyByName("AllowResize",this);
	return n;
}


#ifdef VALIDATING
bool CrySimpleArray::Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail))
{
/* need to create a test for the following functions
StdFunctionsNoDup(CrySimpleArray,CryContainer);
	CrySimpleArray(size_t _ElementSize = 0)
	{
		ElementSize = _ElementSize;
		CurrentCount = MaxCount = 0;
	}
	virtual void CopyTo(CryObject &Dest) const;  //copies contents of this to Dest
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
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0);
	virtual void SetItem(unsigned int i,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0) = 0;
	virtual void RemoveAtIterator(Iterator *LI) = 0;
	virtual bool IsEmpty(const Iterator *I) const { return GetAtIterator(I)==0; }
	virtual void Clear() = 0;
	virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
	virtual bool SetPropertyAsObject(const CryPropertyParser &PropertyName,CryObject *Value);
	virtual bool SetPropertyAsObject(CryProperty *Value);
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
CryObject * CryTemplateArray<T>::Dup() const
{
	int i, m = GetMax();
	CryTemplateArray<T> *n = new CryTemplateArray<T>(m);
	for(i=0;i<m;i++)
		n->Values[i] = Values[i];
	return n;
} */
#ifdef VALIDATING
template<>
bool CryTemplateArray<int>::Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail))
{
/* need to code tests for the following functions
	StdFunctionsNoDup(CryTemplateArray,CrySimpleArray);
	void SetSize(size_t _Size);
	virtual CryObject *Dup()const; // creates a duplicate of this object
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
	void SetItem(unsigned int i,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size)
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
*/
	return CryContainer::Test(Verbose,Object,CallBack);
}
#endif

template<typename T>
void CryTemplateArray<T>::SetSize(size_t _Size)
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
}
template<typename T>
CryTemplateArray<T> &CryTemplateArray<T>::Delete(int start,int amount)
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


template<>
void CryTemplateArray<int>::GetEleType(CryString &Result) const
	{
		Result = "int";
	}
template<>
void CryTemplateArray<float>::GetEleType(CryString &Result) const
	{
		Result = "float";
	}
template<>
void CryTemplateArray<CryString>::GetEleType(CryString &Result) const
	{
		Result = "CryString";
	}

template<>
bool CryTemplateArray<int>::LoadAsText(int i,CryString &FromStream)
{
	int v;
	FromStream.scanf("%d ",&v);
	if (i>=0 && i < GetMax())
	{
		Values[i] = v;
		return true;
	}
	return false;
}
template<>
bool CryTemplateArray<int>::SaveAsText(int i,CryString &ToStream) const
{
	ToStream.printf("%d ",Values[i]);
	return true;
}
template<>
bool CryTemplateArray<float>::LoadAsText(int i,CryString &FromStream)
{
	float v;
	FromStream.scanf("%f ",&v);
	if (i>=0 && i < GetMax())
	{
		Values[i] = v;
		return true;
	}
	return false;
}
template<>
bool CryTemplateArray<float>::SaveAsText(int i,CryString &ToStream) const
{
	ToStream.printf("%f ",Values[i]);
	return true;
}

template<typename T>
void CryTemplateArray<T>::RemoveAtIterator(Iterator *LI)
{
	unsigned int i = IteratorValue(LI);
	for(;i<CurrentCount-1;i++)
	{
		Values[i] = Values[i+1];
	}
	return;
}


//
// CryArray
//
CryArray::CryArray(unsigned int ElementSize) : CrySimpleArray(ElementSize)
{
	pPtr = 0;
	AllowResize = true;
}

CryArray::~CryArray()
{
	if (Count()!=0)
		throw CryException(this, "deleting CryArray, while still having some elements! (Call Clear() first)");

    delete []pPtr;
}
CryFunctionDefList *CryArray::GetFunctions(const char *Type) const
{
    // if a type has been defined and it's not this class, check subclasses for it
    if (Type && !IsA(Type))
        return CrySimpleArray::GetFunctions(Type);
    // otherwise get any functions in subclasses
    CryFunctionDefList *l = CrySimpleArray::GetFunctions();
    CryString s;
    s += "//  Class CryArray;";
    char *search;
    s +="virtual void DestroyArrayItem(CryArray *Owner,EmptyObject *Item) = 0;";
    s +="virtual EmptyObject *CreateArrayItem(CryArray *Owner,bool *IsCryObject) = 0;";
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
    s +="virtual void CopyTo(CryObject &Dest) const;";
    s +="virtual CryObject *Dup()const;";
    s +="virtual const cbyte* GetRaw() const;";
#ifdef VALIDATING

    s +="virtual bool Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool Fail));";
#endif

    s +="virtual const char *ChildClassName() const;";
	s +="virtual bool SetProperty(const char *PropertyName,const CryString &PropertyValue);";
    s +="virtual bool SetPropertyAsObject(const char *PropertyName,CryObject *Value);";
    s +="virtual bool SetPropertyAsObject(CryProperty *Value);";
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
    s +="virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size);";
    s +="virtual CryObject *Add(CryObject *Item);";
    s +="virtual CryObject *AddOwned(CryObject *Item);";
    s +="virtual bool IsEmpty(const Iterator *I) const;";
    s +="virtual void SetItemOwnerShip(Iterator *I,bool Owned);";
    s +="virtual bool GetItemOwnerShip(const Iterator *I) const;";
    s +="virtual bool IsCryObject(const Iterator *I) const;";
    s +="virtual void SetIsCryObject(Iterator *I,bool IsCO);";
    s +="size_t GetItemSize(Iterator *I) const;";
    s +="virtual void Clear();";
    l->LoadFromString(s,";");
    return l;
}
void CryArray::Clear()
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

void CryArray::DeleteItemOffset(unsigned int i)
{
    while(i<Count()-1)
    {
        pPtr[i] = pPtr[i+1];
        i++;
    }
    CurrentCount--;
}


int CryArray::DeleteItem(unsigned int i)
{
	EmptyObject *item = GetItem(i);
	DestroyArrayItem(this,item);
	DeleteItemOffset(i);
	return CurrentCount;
}
void CryArray::RemoveAtIterator(Iterator *LI)
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


void CryArray::SetMax(size_t m)
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
        NewPtr[i].Item = pPtr[i].Item;
    CrySimpleArray::SetMax(m);
    if (CurrentCount>MaxCount)
        CurrentCount = MaxCount;
    delete pPtr;
    pPtr = NewPtr;
}

void CryArray::SetSize(size_t n) // set the number currently in use (either grow or shrink)
{
    if (n>MaxCount)
    {
        if (AllowResize)
        {
            SetMax(MaxCount + n);
        }
        else
            throw(CryException(CryString("Setting Current Array Count %d  higher then MaxArray Count %d",n,MaxCount)));
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
        bool IsCryObject;
        pPtr[CurrentCount].Item = CreateArrayItem(this,&IsCryObject);
        pPtr[CurrentCount].IsOwned = true;
        pPtr[CurrentCount].IsAutoCreated = true;
        pPtr[CurrentCount].IsCryObject = IsCryObject;
        pPtr[CurrentCount].Size = 0;			// not used if IsAutoCreated
        CurrentCount++;
    }
}
bool CryArray::IsEmpty(const Iterator *I) const
{
    int i = IteratorValue(I);
    return pPtr[i].Item==0;
}
bool CryArray::IsCryObject(const Iterator *I) const
{
    int i = IteratorValue(I);
    return pPtr[i].IsCryObject;
}
void CryArray::SetIsCryObject(Iterator *I,bool IsCO)
{
    int i = IteratorValue(I);
    pPtr[i].IsCryObject = IsCO;
}
size_t CryArray::GetItemSize(Iterator *I) const
{
    int i = IteratorValue(I);
    return pPtr[i].Size;
}

void CryArray::GetEleType(CryString &Result) const
{
    Result = "CryArray::Eleptr";
}

void CryArray::SetItemOwnerShip(Iterator *I,bool IsOwned)
{
    int i = IteratorValue(I);
    pPtr[i].IsOwned = IsOwned;
}

bool CryArray::GetItemOwnerShip(const Iterator *I) const
{
    int i = IteratorValue(I);
    return pPtr[i].IsOwned;
}

EmptyObject *CryArray::Add(EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size)
{
	if (Item==0)
		throw CryException(this,"AddOwned Null ptr added");

	size_t n = CurrentCount + 1;
	if (n>MaxCount)
	{
		if (AllowResize)
		{
			SetMax(MaxCount + 100);
		}
		else
			throw(CryException(CryString("Setting Current Array Count %d  higher then MaxArray Count %d",n,MaxCount)));
	}
	pPtr[CurrentCount].Item = Item;
	pPtr[CurrentCount].Size = Size;
	pPtr[CurrentCount].IsOwned = IsOwned;
	pPtr[CurrentCount].IsCryObject = IsCryObject;
	pPtr[CurrentCount].IsAutoCreated = false;
	CurrentCount = n;
	return Item;
}

EmptyObject *CryArray::GetItem(unsigned int i)const //indexed from 0
{
	if (i>=CurrentCount)
	{
		if (CurrentCount==0)
			throw(CryException("Accessing empty Array"));
		else
			throw(CryException("Accessing past end of Array"));
	}
	return pPtr[i].Item;
}

void CryArray::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size)
{
unsigned i = IteratorValue(I);
	SetItem(i,Item,IsCryObject,IsOwned,Size);
}

void CryArray::SetItem(unsigned int i,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size)
{
	if (i>=CurrentCount)
	{
		if (CurrentCount==0)
			throw(CryException("Accessing empty Array"));
		else
			throw(CryException("Accessing past end of Array"));
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
	pPtr[i].IsCryObject = IsCryObject;
	pPtr[i].IsAutoCreated = false;
}

void CryArray::Sort(int (*Compare) (const void *ele1,const void *ele2))
{
    qsort(pPtr,CurrentCount,sizeof(void *),Compare);
}

EmptyObject *CryArray::DupItem(const CryArray::ElePtr  *Node) const
{
    if (Node->IsCryObject)
    {
        return (EmptyObject *)((CryObject *) Node->Item)->Dup();
    }
    else
    {
        char *temp = new char[Node->Size];
        EmptyObject *e = (EmptyObject *)temp;
        memcpy(temp,Node->Item,Node->Size);
        return e;
    }
}


void CryArray::CopyTo(CryArray &Dest) const ///copies contents of this to Dest
{
    if (Dest.ElementSize!=ElementSize)
        throw(CryException("CopyTo: Destination Element size different then source"));
    if (Dest.MaxCount<CurrentCount)
    {
        if (!Dest.AllowResize)
            throw(CryException("CopyTo: Fixed Size Destination Array too small"));
        else
            Dest.SetSize(CurrentCount);
    }
    for(unsigned int i=0;i<CurrentCount;i++)
    {
        CryString s;
        SaveAsText(i,s);
        Dest.LoadAsText(i,s);
    }
    Dest.CurrentCount = CurrentCount;
}
void CryArray::CopyTo(CryObject &Dest) const ///copies contents of this to Dest
{
    if (Dest.IsA(TCryArray))
        CopyTo(*(CryArray *)&Dest);
	if (Dest.IsA(TCryMemStream))
    {
        CryMemStream *mDest = (CryMemStream *) &Dest;
        mDest->Resize(MaxCount * ElementSize);
        mDest->SetRaw(0,GetRaw(),(size_t)MaxCount * ElementSize);
    }
}

CryPropertyList *CryArray::PropertyNames() const
{
	CryPropertyList *n = CryContainer::PropertyNames();
	n->AddPropertyByName("Size",this);
	return n;
}
bool CryArray::HasProperty(const CryPropertyParser &PropertyName) const
{
	/*    const char *name = ChildClassName();
		int l = strlen(name);
		if (strncmp(name,PropertyName,l)==0)*/
	{
		//        name = PropertyName + l;
		if (PropertyName=="Size")
			return true;
	}
    return CryContainer::HasProperty(PropertyName);
}

const char *CryArray::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
	Result.Clear();
	if (PropertyName=="Size")
	{
		Result.printf("%d",Count());
		return Result;
	}
	return CryContainer::GetProperty(PropertyName,Result);
}

bool CryArray::SetPropertyAsObject(const CryPropertyParser &PropertyName,CryObject *Value)
{
	if (Value->IsA(TCryString))
		return CrySimpleArray::SetProperty(PropertyName,*(CryString *)Value);
	return false;
}

bool CryArray::SetPropertyAsObject(CryProperty *Value)
{
	return CrySimpleArray::SetPropertyAsObject(Value);
}

bool CryArray::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Size")
	{
		if (strlen(PropertyValue)==0)
			throw CryException(this,"Cannot Set Size to no value");
		int s = atoi(PropertyValue);
		this->SetSize(s);
		return true;
	}
	if (PropertyName=="Values")
	{
		if (strlen(PropertyValue)==0)
			throw CryException(this,"Cannot Set Value to no value");
		//        this->SaveItemTo(this,NULL,PropertyValue);
		return true;
	}
	if (CryContainer::SetProperty(PropertyName,PropertyValue))
		return true;
	return false;
}


#ifdef VALIDATING
bool CryArray::Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail))
{
	return CryContainer::Test(Verbose,Object,CallBack);
}
#endif



//
// CryIntArray
//
//Constructor/Destructor Functions

CryIntArray::CryIntArray(int _SetSize) : CrySimpleArray(sizeof(int))
{
	Values = new int[_SetSize];
	SetMax(_SetSize);
	SetSize(_SetSize);
}
void CryIntArray::SetSize(size_t _Size) // set the number currently in use (either grow or shrink)
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
	delete Values;
	CurrentCount = _Size;
	Values = n;
}



CryIntArray::~CryIntArray()
{
		delete [] Values;
}



//Inherited Functions
void CryIntArray::DestroyArrayItem(CryArray *Owner,EmptyObject *Item)
{
	delete Item;
}

EmptyObject *CryIntArray::CreateArrayItem(CryArray *Owner,bool *IsCryObject)
{
	if (IsCryObject) {
		throw CryException(this,"Cannot create CryObject for Set");
	}
	return (EmptyObject *)new int;
}


bool CryIntArray::LoadAsText(int i,CryString &FromStream)
{
	int v;
	FromStream.scanf("%d ",&v);
	if (i>=0 && i < GetMax())
	{
		Values[i] = v;
		return true;
	}
	return false;
}

bool CryIntArray::SaveAsText(int i,CryString &ToStream) const
{
	ToStream.printf("%d ",Values[i]);
	return true;
}

void CryIntArray::GetEleType(CryString &Result) const
{
	Result = "int";
}


EmptyObject *CryIntArray::GetAtIterator(const Iterator *I) const
{
		return NULL;
}
void CryIntArray::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size)
{
unsigned i = IteratorValue(I);
	SetItem(i,Item,IsCryObject,IsOwned,Size);
}

void CryIntArray::SetItem(unsigned int i,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size)
{
		SetValue(i,*(int*)Item);
}

void CryIntArray::RemoveAtIterator(Iterator *LI)
{

}

void CryIntArray::Clear()
{

}
EmptyObject *CryIntArray::Add(EmptyObject *Item,size_t Size)
{
 return 0;	// to do fill in the stub
}
EmptyObject *CryIntArray::AddOwned(EmptyObject *Item,size_t Size)
{
 return 0;	// to do fill in the stub
}
CryObject *CryIntArray::Add(CryObject *Item)    // returns Item
{
 return 0;	// to do fill in the stub
}
CryObject *CryIntArray::AddOwned(CryObject *Item)   // gives ownership to list
{
 return 0;	// to do fill in the stub
}
void CryIntArray::SetItemOwnerShip(Iterator *I,bool Owned)
{

}
bool CryIntArray::GetItemOwnerShip(const Iterator *I) const
{
 return false;	// to do fill in the stub
}
bool CryIntArray::IsCryObject(const Iterator *I) const
{
 return false;	// to do fill in the stub
}
//bool LoadAsText(Iterator *I,CryString &FromStream) = 0;
//bool SaveAsText(Iterator *I,CryString &ToStream) const = 0;

/*!Get the names of all properties of this class*/
CryPropertyList *CryIntArray::PropertyNames() const
{
// get base class's properties
CryPropertyList *Names = CrySimpleArray::PropertyNames();
// now add our own properties (if any)
	Names->AddPropertyByName("Size",this);
// properties that are local to this class
	return Names;
}


void CryIntArray::CopyTo(CryObject &Dest) const
{

	if (Dest.IsA(TCryIntArray))
	{
		// Copy this classes variables first
//		CryIntArray *CastDest = (CryIntArray *)&Dest;

	}
//Now copy the base class
	CrySimpleArray::CopyTo(Dest);

}//CryIntArray


bool CryIntArray::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Size")
	{
		if (strlen(PropertyValue)==0)
			throw CryException(this,"Cannot Set Size to no value");
		int s = atoi(PropertyValue);
		SetSize(s);
		return true;
	}
	if (CrySimpleArray::SetProperty(PropertyName,PropertyValue))
		return true;
	return false;
}


const char *CryIntArray::GetProperty(const char *PropertyName,CryString &Result) const
{
 return 0;	// to do fill in the stub
}


//
//	CryDoubleArray
//
	void CryDoubleArray::GetEleType(CryString &Result) const
	{
		Result = "double";
	};

CryObject * CryDoubleArray::Dup() const
{
	int i, m = GetMax();
	CryDoubleArray *n = new CryDoubleArray(m);
	for(i=0;i<m;i++)
		n->Values[i] = Values[i];
	return n;
}

	/// will set a value to the container[Iterator]
void CryDoubleArray::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size)
{
unsigned i = IteratorValue(I);
	SetItem(i,Item,IsCryObject,IsOwned,Size);
}

bool CryDoubleArray::LoadAsText(int i,CryString &FromStream)
{
	double v;
	FromStream.scanf("%f ",&v);
	if (i>=0 && i < GetMax())
	{
		Values[i] = v;
		return true;
	}
	return false;
}
bool CryDoubleArray::SaveAsText(int i,CryString &ToStream) const
{
	ToStream.printf("%f ",Values[i]);
	return true;
}

void CryDoubleArray::SetSize(size_t _Size)
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
bool CryDoubleArray::SetPropertyAsObject(const CryPropertyParser &PropertyName,CryObject *Value)
{
	if (Value->IsA(TCryString))
		return CrySimpleArray::SetProperty(PropertyName,*(CryString *)Value);
	return false;
}
CryFunctionDefList *CryDoubleArray::GetFunctions(const char *Type) const
{
	// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
		return CrySimpleArray::GetFunctions(Type);
	// otherwise get any functions in subclasses
	CryFunctionDefList *l = CrySimpleArray::GetFunctions();
	CryString s;
	s += "// Class CryDoubleArray;";
	char *search;
	s +="void SetSize(size_t _Size);";
	s +="CryString *GetFunctions() const;";
	s +="virtual CryObject *Dup()const;";
	s +="virtual void DestroyArrayItem(CryArray *Owner,EmptyObject *Item);";
	s +="virtual EmptyObject *CreateArrayItem(CryArray *Owner,bool *IsCryObject);";
	s +="virtual bool LoadAsText(int i,CryString &FromStream);";
	s +="virtual bool SaveAsText(int i,CryString &ToStream) const;";
	s +="virtual bool LoadAsText(Iterator *I,CryString &FromStream);";
	s +="virtual bool SaveAsText(Iterator *I,CryString &ToStream) const;";
	s +="const double operator [](int i);";
	s +="void SetValue(int i,double v);";
	s +="double GetValue(int i) const;";
	s +="void Clear();";
	s +="virtual bool SetProperty(const char *PropertyName,const CryString &PropertyValue);";
	s +="virtual bool SetPropertyAsObject(const char *PropertyName,CryObject *Value);";
	s +="virtual bool SetPropertyAsObject(CryProperty *Value);";
	s +="virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;";
	s +="virtual bool HasProperty(const char *PropertyName)const;";
	s +="virtual int GetPropertyCount() const;";
	s +="virtual CryPropertyList* PropertyNames() const;";
	s +="virtual const char* ChildClassName() const;";
	s +="virtual bool IsA(const char *_ClassName) const;";
	l->LoadFromString(s,";");
	return l;
}

// if this class contains the property name, it will attempt to load it
// if all is well returns true
bool CryDoubleArray::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Size")
	{
		if (strlen(PropertyValue)==0)
			throw CryException(this,"Cannot Set Size to no value");
		int s = atoi(PropertyValue);
		SetSize(s);
		return true;
	}
	if (CrySimpleArray::SetProperty(PropertyName,PropertyValue))
		return true;
	return false;
}

void CryDoubleArray::RemoveAtIterator(Iterator *LI)
{
	unsigned int i = IteratorValue(LI);
	for(;i<CurrentCount-1;i++)
	{
		Values[i] = Values[i+1];
	}
	return;
}

const char *CryDoubleArray::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
	if (PropertyName=="Size")
	{
		Result.printf("%d",CurrentCount);
		return Result.AsPChar();
	}
	/*if (strcmp(PropertyName,"Values")==0)	// let Container Class handle it
	{

	}*/
	return CrySimpleArray::GetProperty(PropertyName,Result);
}
bool CryDoubleArray::HasProperty(const CryPropertyParser &PropertyName)const
{
	if (PropertyName=="Size")
		return true;
	return CrySimpleArray::HasProperty(PropertyName);
}

int CryDoubleArray::GetPropertyCount() const
{
	return CrySimpleArray::GetPropertyCount() + 1;
}

CryPropertyList *CryDoubleArray::PropertyNames() const
{
	CryPropertyList *n = CrySimpleArray::PropertyNames();
	n->AddPropertyByName("Size",this);
	return n;
}
