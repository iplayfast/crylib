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
#include "ClassContainer.h"
#include "ClassProperty.h"
#include "ClassException.h"
#include "ClassFunction.h"

using namespace Crystal;
using namespace std;



//-------------------------------------------------------------------
// CryContainer
//-------------------------------------------------------------------


#ifdef VALIDATING
bool CryContainer::Test(bool Verbose,CryObject &Object, bool (CallBack)(bool Verbose,const char *Result,bool Fail))
{
    bool Fail = false;
    if (Object.IsContainer())
    {
        char Result[200];
        CryContainer *c = (CryContainer *)&Object;
        sprintf(Result,"\nCryContainer Testing:\nObject ClassName %s,ChildClassName %s",
                c->ClassName(),c->ChildClassName());
        if (!CallBack(Verbose,Result,Fail))
            return false;

        Iterator *I = c->_CreateIterator();
        if (!I)
        {
            Fail = true;
            sprintf(Result,"\nContainer Failed to make Iterator");
            if (!CallBack(Verbose,Result,Fail))
                return false;
        }
        if ((I->GetOrigContainer() != c) || (c->GetOwner(I)!=c))
        {
            Fail = true;
            sprintf(Result,"\nIterator Container Failed to make Iterator");
            if (!CallBack(Verbose,Result,Fail))
                return false;
        }
        if (c->Count())
        {
            sprintf(Result,"\nContainer Emtpy Can't test iterator");
            if (!CallBack(Verbose,Result,Fail))
                return false;
        }
        else
        {
            EmptyObject *first = 0,*last = 0;
            if (!c->GotoFirst(I))
            {
                Fail = true;
                sprintf(Result,"\nIterator Container Failed to GotoFirst");
                if (!CallBack(Verbose,Result,Fail))
                    return false;
            }
            else
            {
                last = first = c->GetAtIterator(I);
                if (first==0)
                {
                    Fail = true;
                    sprintf(Result,"\nIterator Container Failed to GetAtIterator(First)");
                    if (!CallBack(Verbose,Result,Fail))
                        return false;
                }
            }

            if (c->Count()==1)
            {
                sprintf(Result,"\nContainer only contains 1 value can't test iterator");
                if (!CallBack(Verbose,Result,Fail))
                    return false;
            }
            else
            {
                for(size_t i=0;i<c->Count();i++)
                    if (!GotoNext(I))    // returns true if success
                    {
                        if (i<c->Count()-1)
                        {
                            Fail = true;
                            sprintf(Result,"\nIterator Failed to GotoNext when available");
                            if (!CallBack(Verbose,Result,Fail))
                                return false;
                        }
                        else
                        {
                            sprintf(Result,"\nGotoNext passed");
                            if (!CallBack(Verbose,Result,Fail))
                                return false;
                        }
                    }
                    else
                        last = c->GetAtIterator(I);
                c->GotoFirst(I);
                if (!c->GotoLast(I))
                {
                    Fail = true;
                    sprintf(Result,"\nIterator Failed to GotoLast when available");
                    if (!CallBack(Verbose,Result,Fail))
                        return false;
                }
                if (c->GetAtIterator(I)!=last)
                {
                    Fail = true;
                    sprintf(Result,"\nIterator Container Failed to GetAtIterator(Last)");
                    if (!CallBack(Verbose,Result,Fail))
                        return false;
                }
            }
        }
        c->DeleteIterator(I);
    }
    return CryObject::Test(Verbose,Object,CallBack);
}
#endif

bool CryContainer::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
    bool result = false;
    if (PropertyName=="Values")
    {
        CryString Temp;
        Temp = PropertyValue;
        Iterator *i = _CreateIterator();
        if (GotoFirst(i))
        {
            do
            {
                result = LoadAsText(i,Temp);
            }
            while(GotoNext(i));
        }
    }
    else
        return CryObject::SetProperty(PropertyName,PropertyValue);
    return result;
}
CryObject *CryContainer::GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const
{
    return CryObject::GetCopyOfPropertyAsObject(PropertyName);
}

const char *CryContainer::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
    if (PropertyName=="Values")	// this is handled on an individual basis instead of passing the array flag back because strings are also containers, and as such would be huge lists of single letters.
    {
        Result.Clear();
        CryString Temp;
        Iterator *i = _CreateIterator();
        if (GotoFirst(i))
        {
            do
            {
                Temp.Clear();
                SaveAsText(i,Temp);
                Result.printf("%s ",Temp.AsPChar());
            }
            while(GotoNext(i));
        }
        delete i;
        return Result.AsPChar();
	}
    return CryObject::GetProperty(PropertyName,Result);
}
bool CryContainer::HasProperty(const CryPropertyParser &PropertyName) const
{
    if (PropertyName=="Values")
        return true;
    return CryObject::HasProperty(PropertyName);
}
int CryContainer::GetPropertyCount() const
{
    return CryObject::GetPropertyCount() + 1;
}
CryPropertyList *CryContainer::PropertyNames() const
{
	CryPropertyList *n = CryObject::PropertyNames();
	n->AddPropertyByName("Values",this);
	return n;
}

CryFunctionDefList *CryContainer::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return CryObject::GetFunctions(Type);
	// otherwise get any functions in subclasses
   CryFunctionDefList *l = CryObject::GetFunctions();
    CryString s;
    s += "//Class CryContainer;";
    s += "virtual void GetEleType(CryString &Result) const = 0;";
    s += "const CryContainer *GetOwner(Iterator *I) const;";
    s += "virtual bool IsContainer() const;";
	s += "virtual Iterator *_CreateIterator() const = 0;";
    s += "virtual void DeleteIterator(Iterator *I) const = 0;";
    s += "virtual bool GotoFirst(Iterator *I) const = 0;";
    s += "virtual bool GotoPrev(Iterator *I) const = 0;";
    s += "virtual bool GotoNext(Iterator *I) const = 0;";
    s += "virtual bool GotoLast(Iterator *Iterator) const = 0;";
    s += "virtual EmptyObject *GetAtIterator(Iterator *I) const = 0;";
    s += "virtual size_t Count() const = 0;";
    s += "virtual void Clear() = 0;";
    s += "virtual CryString *GetFunctions();";
    s += "virtual EmptyObject *Add(EmptyObject *Item,size_t Size) = 0;";
    s += "virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size) = 0;";
    s += "virtual CryObject *Add(CryObject *Item) = 0;";
    s += "virtual CryObject *AddOwned(CryObject *Item) = 0;";
    s += "virtual void SetItemOwnerShip(Iterator *I,bool Owned) = 0;";
    s += "virtual bool GetItemOwnerShip(const Iterator *I) const = 0;";
    s += "virtual size_t GetItemSize(Iterator *I) const = 0;";
    s += "virtual bool IsCryObject(const Iterator *I) const = 0;";
    s += "virtual bool LoadAsText(Iterator *I,CryString &FromStream) = 0;";
    s += "virtual bool SaveAsText(Iterator *I,CryString &ToStream) const = 0;";
    s += "bool IterateThroughAll(EmptyObject *Control);";
    s += "virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item);";
    s += "virtual bool SetProperty(const char *PropertyName,const char *PropertyValue);";
    s += "virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;";
    s += "virtual bool HasProperty(const char *PropertyName) const;";
    s += "virtual int GetPropertyCount() const;";
    s += "virtual const char* ChildClassName() const;";
	s += "virtual CryPropertyList* PropertyNames() const;";

#ifdef VALIDATING

    s += "virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));";
#endif

    l->LoadFromString(s,";");
    return l;
}

// private function not allowed
CryContainer::CryContainer(CryContainer &NoNO) : CryOwnedObject()
{
	_IsContainer = true;
}

CryContainer::CryContainer()
{
	_IsContainer = true;
}

bool CryContainer::GotoN(Iterator *Iterator,int n) const // returns true if success
{
	if (Iterator->GotoFirst())
	{
		while(n)
		{
			if (!Iterator->GotoNext())
				return false;
			n--;
		}
		return true;
	}
	return false;
}
bool CryContainer::IteratedFunction(EmptyObject *Control,EmptyObject *Item)
{
    return true;
}

bool CryContainer::Sortable() const
{
    return false;
}
void CryContainer::Sort(int CompareType)
{
    return ;
}
//! Compare the contained items (Used in sorting)
int CryContainer::Compare2(int CompareType,const EmptyObject *First,const EmptyObject *Second) const
{
    return 0;
}
// Compare to another Object
int CryContainer::CompareLogical(int CompareType,const CryObject *Test) const
{
    return 0;
}
bool CryContainer::LessThen(int CompareType,const CryObject *Test) const
{
	return false;
}
bool CryContainer::GreaterThen(int CompareType,const CryObject *Test) const
{
	return false;
}
bool CryContainer::EqualTo(int CompareType,const CryObject *Test) const
{
    return true;
}


const CryContainer *CryContainer::GetOwner(Iterator *I) const
{
    return I->GetOrigContainer();
}

void CryContainer::SetIsContainer(bool Enabled)
{
	 _IsContainer = Enabled; 
}

bool CryContainer::IsContainer() const
{
    return _IsContainer;
}

CryContainer::Iterator::Iterator(const CryContainer *oc )
{
    OrigContainer = oc;
}

bool CryContainer::Iterator::IsCryObject() const
{
    return OrigContainer->IsCryObject(this);
}

bool CryContainer::Iterator::IsOwned() const
{
    return OrigContainer->GetItemOwnerShip(this);
}

bool CryContainer::Iterator::IsEmpty() const
{
    return OrigContainer->IsEmpty(this);
}
 // true if item is an object and is a container and is currently set as a container (believes it is a container)
bool CryContainer::Iterator::IsContainer() const
{
	if (IsCryObject())
	{
		CryObject *o = (CryObject *) OrigContainer->GetAtIterator(this);
#ifdef DEBUG
		const char *type = o->ChildClassName();
#endif
// even though it's a container type, it may not currently be a container (eg string defaults to being a non-container)
		return (o && o->IsA(TCryContainer) && o->IsContainer());
	}
	return false; // not an object so not a container
}
EmptyObject *CryContainer::Iterator::Get() const
{
	return OrigContainer->GetAtIterator(this);
}

//void CryContainer::Iterator::Set(EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size) 
//{
//	OrigContainer->SetAtIterator(this,Item,IsCryObject,IsOwned,Size );
//}
bool CryContainer::Iterator::GotoN(int n) // returns true if success
{
    return OrigContainer->GotoN(this,n);
}
bool CryContainer::Iterator::GotoFirst()
{
    return OrigContainer->GotoFirst(this);
}

bool CryContainer::Iterator::GotoPrev()
{
    return OrigContainer->GotoPrev(this);
}

bool CryContainer::Iterator::GotoNext()
{
    return OrigContainer->GotoNext(this);
}

bool CryContainer::Iterator::GotoLast()
{
    return OrigContainer->GotoLast(this);
}

size_t CryContainer::Iterator::GetItemSize()
{
    return OrigContainer->GetItemSize(this);
}

//bool LoadAsText(CryString *FromStream) { return OrigContainer->LoadAsText(this,FromStream); }
bool CryContainer::Iterator::SaveAsText(CryString &ToStream)
{
    return OrigContainer->SaveAsText(this,ToStream);
}

const CryContainer *CryContainer::Iterator::GetOrigContainer() const
{
    return OrigContainer;
}

/// return the type of structure used to control this list (ie listnode eleptr, etc)
void CryContainer::Iterator::GetEleType(CryString &Result) const
{
    OrigContainer->GetEleType(Result);
}



/// returns true if all contained things where iterated through, FunctionToCall returns false if iteration should stop
bool CryContainer::IterateThroughAll(EmptyObject *Control)
{
    return CryObject::IterateThroughAll(this,Control);
}
