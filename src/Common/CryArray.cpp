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
        mDest->Resize(MaxCount * _ElementSize);
        mDest->SetRaw(0,GetRaw(),(size_t)MaxCount * _ElementSize);
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
    l->LoadFromString(s,";");
    return l;
}
void CrySimpleArray::SetMax(size_t m)
{
    MaxCount = m;
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
    s +="virtual CryList *PropertyNames() const;";
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
	EmptyObject *r;
	int i = IteratorValue(LI);
	if (!LI->IsOwned())
	{
		r = GetAtIterator(LI);
		GotoPrev(LI);
		DeleteItemOffset(i);
	}
	else
	{
		r = 0;
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
            SetMax(MaxCount + 100);
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
    if (Dest._ElementSize!=_ElementSize)
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
        mDest->Resize(MaxCount * _ElementSize);
        mDest->SetRaw(0,GetRaw(),(size_t)MaxCount * _ElementSize);
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
    s +="virtual CryList *PropertyNames() const;";
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
    /*if (strcmp(PropertyName,"Values")==0)	// Better to not have this in container since container Let container class handle it
    {
        if (strlen(PropertyValue->AsPChar())==0)
            throw CryException(this,"Cannot Set Value to no value");
        double n;
    for (int i=0;i<Size;i++)
    {
    PropertyValue->scanf("%f",&n);
    Values[i] = n;
    }
        return true;
    }*/
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
