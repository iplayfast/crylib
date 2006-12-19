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
bool Container::Test(bool Verbose,Object &Object, bool (CallBack)(bool Verbose,const char *Result,bool Fail))
{
    bool Fail = false;
    if (Object.IsContainer())
    {
        char Result[200];
        Container *c = (Container *)&Object;
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
        if (c->Count()==0)
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
    return Object::Test(Verbose,Object,CallBack);
}
#endif

bool Container::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
    bool result = false;
    if (PropertyName=="Values")
    {
        String Temp;
        Temp = PropertyValue;
        Iterator *i = _CreateIterator();
		if (i->GotoFirst())
		{
			do
			{
				result = LoadAsText(i,Temp);
			}
			while(i->GotoNext());
		}
		DeleteIterator(i);
    }
    else
        return Object::SetProperty(PropertyName,PropertyValue);
    return result;
}
Object *Container::GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const
{
    return Object::GetCopyOfPropertyAsObject(PropertyName);
}

const char *Container::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (PropertyName=="Values")
	{
		Result = "[]";  // if Result != what is returned, it's a special situation, in this case pointer to an array of weights
		return "*";
	}

	if (PropertyName=="*Values")	// this is handled on an individual basis instead of passing the array flag back because strings are also containers, and as such would be huge lists of single letters.
    {
		Result.Clear();
		String Temp;
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
		DeleteIterator(i);
        return Result.AsPChar();
	}
    return Object::GetProperty(PropertyName,Result);
}
bool Container::HasProperty(const PropertyParser &PropertyName) const
{
    if (PropertyName=="Values")
		return true;
	if (PropertyName=="*Values")
		return true;

	return Object::HasProperty(PropertyName);
}
int Container::GetPropertyCount() const
{
    return Object::GetPropertyCount() + 1;
}

PropertyList *Container::PropertyNames() const
{
	PropertyList *n = Object::PropertyNames();
	n->AddPropertyByName("Values",this);
	return n;
}

FunctionDefList *Container::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return Object::GetFunctions(Type);
	// otherwise get any functions in subclasses
   FunctionDefList *l = Object::GetFunctions();
    String s;
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
    s += "virtual bool IsObject(const Iterator *I) const = 0;";
    s += "virtual bool LoadAsText(Iterator *I,CryString &FromStream) = 0;";
    s += "virtual bool SaveAsText(Iterator *I,CryString &ToStream) const = 0;";
    s += "bool IterateThroughAll(EmptyObject *Control);";
    s += "virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item);";
    s += "virtual bool SetProperty(const char *PropertyName,const char *PropertyValue);";
    s += "virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;";
    s += "virtual bool HasProperty(const char *PropertyName) const;";
    s += "virtual int GetPropertyCount() const;";
    s += "virtual const char* ChildClassName() const;";
	s += "virtual PropertyList* PropertyNames() const;";

#ifdef VALIDATING

    s += "virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));";
#endif

    l->LoadFromString(s,";");
    return l;
}

// private function not allowed
Container::Container(Container &NoNO) : OwnedObject()
{
	_IsContainer = true;
}

Container::Container()
{
	_IsContainer = true;
}

bool Container::GotoN(Iterator *Iterator,int n) const // returns true if success
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
bool Container::IteratedFunction(EmptyObject *Control,EmptyObject *Item)
{
    return true;
}

bool Container::Sortable() const
{
    return false;
}
void Container::Sort(int CompareType)
{
    return ;
}
//! Compare the contained items (Used in sorting)
int Container::Compare2(int CompareType,const EmptyObject *First,const EmptyObject *Second) const
{
    return 0;
}
// Compare to another Object
int Container::CompareLogical(int CompareType,const Object *Test) const
{
    return 0;
}
bool Container::LessThen(int CompareType,const Object *Test) const
{
	return false;
}
bool Container::GreaterThen(int CompareType,const Object *Test) const
{
	return false;
}
bool Container::EqualTo(int CompareType,const Object *Test) const
{
    return true;
}


const Container *Container::GetOwner(Iterator *I) const
{
    return I->GetOrigContainer();
}

void Container::SetIsContainer(bool Enabled)
{
	 _IsContainer = Enabled; 
}

bool Container::IsContainer() const
{
    return _IsContainer;
}

Container::Iterator::Iterator(const Container *oc )
{
    OrigContainer = oc;
}

bool Container::Iterator::IsObject() const
{
    return OrigContainer->IsObject(this);
}

bool Container::Iterator::IsOwned() const
{
    return OrigContainer->GetItemOwnerShip(this);
}

bool Container::Iterator::IsEmpty() const
{
    return OrigContainer->IsEmpty(this);
}
 // true if item is an object and is a container and is currently set as a container (believes it is a container)
bool Container::Iterator::IsContainer() const
{
	if (IsObject())
	{
		Object *o = (Object *) OrigContainer->GetAtIterator(this);
#ifdef DEBUG
		const char *type = o->ChildClassName();
#endif
// even though it's a container type, it may not currently be a container (eg string defaults to being a non-container)
		return (o && o->IsA(CContainer) && o->IsContainer());
	}
	return false; // not an object so not a container
}
EmptyObject *Container::Iterator::Get() const
{
	return OrigContainer->GetAtIterator(this);
}

//void CryContainer::Iterator::Set(EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size) 
//{
//	OrigContainer->SetAtIterator(this,Item,IsObject,IsOwned,Size );
//}
bool Container::Iterator::GotoN(int n) // returns true if success
{
    return OrigContainer->GotoN(this,n);
}
bool Container::Iterator::GotoFirst()
{
    return OrigContainer->GotoFirst(this);
}

bool Container::Iterator::GotoPrev()
{
    return OrigContainer->GotoPrev(this);
}

bool Container::Iterator::GotoNext()
{
    return OrigContainer->GotoNext(this);
}

bool Container::Iterator::GotoLast()
{
    return OrigContainer->GotoLast(this);
}

size_t Container::Iterator::GetItemSize()
{
    return OrigContainer->GetItemSize(this);
}

//bool LoadAsText(CryString *FromStream) { return OrigContainer->LoadAsText(this,FromStream); }
bool Container::Iterator::SaveAsText(String &ToStream)
{
    return OrigContainer->SaveAsText(this,ToStream);
}

const Container *Container::Iterator::GetOrigContainer() const
{
    return OrigContainer;
}

/// return the type of structure used to control this list (ie listnode eleptr, etc)
void Container::Iterator::GetEleType(String &Result) const
{
	OrigContainer->GetEleType(Result);
} 



/// returns true if all contained things where iterated through, FunctionToCall returns false if iteration should stop
bool Container::IterateThroughAll(EmptyObject *Control)
{
    return Object::IterateThroughAll(this,Control);
}
