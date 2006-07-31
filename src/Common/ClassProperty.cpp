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

#include "ClassProperty.h"
#include "ClassException.h"
#include "ClassFunction.h"

using namespace Crystal;


///----------------------------------------------------------------------------
// CryProperty
///----------------------------------------------------------------------------
void CryProperty::Init(const char *_Name,const char *_Value)
{
    Value = new CryString(_Value);
    if (Value==0)
        throw CryException("Out of memory creating CryProperty");

    Name = new CryString(_Name);
    if (Name==0)
    {
        delete Value;
        throw CryException("Out of memory creating CryProperty");
    }
}
CryProperty::CryProperty(const char *_Name,const char *_Value)
{
	Init(_Name,_Value);
}

CryProperty::CryProperty(const char *_Name)
{
	Init(_Name,"");
}

CryProperty::CryProperty(CryProperty &Copy)
{
    if (&Copy!=this)
    {
	Init(Copy.Name->AsPChar(),Copy.Value->AsPChar());
    };
}

CryProperty::~CryProperty()
{
    delete Value;
    Value = 0;
    delete Name;
    Name = 0;
}

void CryProperty::SetValue(const char *_Value)
{
    *Value = _Value;
}

void CryProperty::SetName(const char *_Name)
{
    *Name = _Name;
}

const CryString &CryProperty::GetValue() const
{
    return *Value;
}

const cbyte* CryProperty::GetRaw() const
{
    return Value->GetRaw();
}

bool CryProperty::HasProperty(const CryPropertyParser &PropertyName) const
{
    return (PropertyName==GetName()) || CryObject::HasProperty(PropertyName);
}

CryFunctionDefList *CryProperty::GetFunctions(const char *Type) const
{
    // if a type has been defined and it's not this class, check subclasses for it
    if (Type && !IsA(Type))
        return CryObject::GetFunctions(Type);
    // otherwise get any functions in subclasses
    CryFunctionDefList *l = CryObject::GetFunctions();
    CryString s;
    s += "//  Class CryProperty;";
    s += "virtual CryString *GetFunctions() const;";
    s += "void SetValue(const char * _Value);";
    s += "const CryString &GetValue() const;";
    s += "virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;";
    s += "virtual const cbyte* GetRaw() const;";
    s += "bool IsA(const char *_ClassName) const;";
    s += "virtual const char *ChildClassName() const;";
    s += "virtual bool HasProperty(const char *PropertyName) const;";
    s += "bool SetProperty(const char*PropertyName,const char *PropertyValue);";
    s += "virtual CryList *PropertyNames() const;";
    s += "virtual void CopyTo(CryObject &Dest) const;";
    s += "virtual CryObject *Dup() const;";
    s += "virtual size_t Size() const;";
    l->LoadFromString(s,";");
    return l;
}

const char *CryProperty::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
    if (PropertyName==GetName())	// if PropertyName is not the stored one, see if this classes property is what is being requested
    {
        Result = Value;
        return Result.AsPChar();
    }
    else
    {
        return CryObject::GetProperty(PropertyName,Result);
    }
}


bool CryProperty::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
    if (PropertyName==GetName())
    {
        *Value = PropertyValue;
        return true;
    }
    else
    {
        if (CryObject::HasProperty(PropertyName))
            return CryObject::SetProperty(PropertyName,PropertyValue);
        else
            throw CryException(this,ExceptionUnknownProperty,"Unknown Property ",PropertyName.AsPChar());
    }

}

CryPropertyList *CryProperty::PropertyNames() const
{
	CryPropertyList *n = CryObject::PropertyNames();
    n->AddProperty(new CryString(GetName()),new CryString(GetValue()));
    return n;
}

void CryProperty::CopyTo(CryObject &Dest) const   //copies contents of this to Dest
{
    Dest.SetProperty(Name->AsPChar(),*Value);
}

const char *CryProperty::GetName() const
{
    return *Name;
}

CryObject *CryProperty::Dup() const  /// creates a duplicate of this object
{
    CryProperty *n = new CryProperty(GetName());
    n->SetValue(*Value);
    return n;
}

size_t CryProperty::Size() const
{
	return strlen(GetName()) + Value->Size();
}


///----------------------------------------------------------------------------
//CryPropertyList
///----------------------------------------------------------------------------

CryPropertyList::~CryPropertyList()
{
	if (Names)
	{
		CryList *t = Names;
		Names = 0;
		delete t;
	}
}

CryPropertyList::PropertyIterator::PropertyIterator(const CryPropertyList *container)
		: CryList::ListIterator((CryList *)container)
{
	Names = (CryList::ListIterator *) container->GetNames()->_CreateIterator();
};

void CryPropertyList::PropertyIterator::CopyTo(CryObject &Dest) const
{
	if (!Dest.IsA(TPropertyIterator))
		CryList::ListIterator::CopyTo(Dest);
CryPropertyList::PropertyIterator &d = (CryPropertyList::PropertyIterator &)Dest;
	d.Names->p = Names->p;
	d.p = p;
}
CryObject *CryPropertyList::PropertyIterator::Dup() const
{
CryPropertyList *l =(CryPropertyList *)this->GetOrigContainer();

	PropertyIterator *New = l->CreateIterator();
	CopyTo(*New);
	return New;
}


bool CryPropertyList::PropertyIterator::GotoFirst()
{
	Names->GotoFirst();
	return	CryList::ListIterator::GotoFirst();
}
bool CryPropertyList::PropertyIterator::GotoPrev()
{
	Names->GotoPrev();
	return CryList::ListIterator::GotoPrev();
}
bool CryPropertyList::PropertyIterator::GotoNext()
{
	Names->GotoNext();
	return CryList::ListIterator::GotoNext();
}
bool CryPropertyList::PropertyIterator::GotoLast()
{
	Names->GotoLast();
	return CryList::ListIterator::GotoLast();
}
size_t CryPropertyList::PropertyIterator::GetItemSize()
{
	return CryList::ListIterator::GetItemSize() + Names->GetItemSize();
}
CryString *CryPropertyList::PropertyIterator::GetName()
{
	return (CryString *)Names->Get();
}
CryString *CryPropertyList::PropertyIterator::GetValue()
{
	return (CryString *) CryList::ListIterator::Get();
}

void CryPropertyList::PropertyIterator::SetName(const char *Name)
{
CryPropertyList *l = (CryPropertyList *)Names->GetOrigContainer();
    CryList *n = l->GetNames();
	n->SetAtIterator(Names,new CryString(Name),true,true,0);
//	l->SetAtIterator(this,new CryString(Name),true,true,0);
	//Names->Set(new CryString(Name),true,true,0);
} 
void CryPropertyList::PropertyIterator::SetValue(const char *Value)
{
	CryPropertyList *l = (CryPropertyList *)this->GetOrigContainer();
	CryList *v = (CryList *)l;
		v->SetAtIterator(this,new CryString(Value),true,true,0);
//		l->SetAtIterator(this,new CryString(Value),true,true,0);

	//Set(new CryString(Value),true,true,0);
}

//CryContainer::Iterator * CryPropertyList::CreateNameIterator() const
//{
//	return Names->_CreateIterator();
//}

//void CryPropertyList::DeleteNameIterator(Iterator *I) const
//{
//    Names->DeleteIterator(I);
//}

CryPropertyList::PropertyIterator *CryPropertyList::CreateIterator() const
{
	return (PropertyIterator *)_CreateIterator();
}

CryContainer::Iterator *CryPropertyList::_CreateIterator() const
{
	return new PropertyIterator(this);
}

void CryPropertyList::DeleteIterator(Iterator *LI) const
{
	delete (PropertyIterator *)LI;
}
size_t CryPropertyList::Size() const
{
	return Names->Size() + CryList::Size();
}

void CryPropertyList::RemoveNodeValue(const CryMemStream &Needle)
{
int n = Names->FindNodeValue(Needle);
	if (n>=0)
	{
		CryPropertyList::PropertyIterator *i = CreateIterator();
		if (i->GotoFirst())
		{
			while(n)
			{
				n--;
				if (!i->GotoNext())
				{		// node wasn't found... shouldn't get here
					DeleteIterator(i);
					return;
				}
			}
			RemoveAtIterator(i);
			DeleteIterator(i);
		}
	}
}
void CryPropertyList::CopyTo(CryObject &Dest) const
{
	if (Dest.IsA(TCryPropertyList))
	{
		CryPropertyList *Cast = (CryPropertyList *)&Dest;
		Names->CopyTo(*Cast->Names);
		CryList::CopyTo(*Cast);
	}
	else
	throw CryException(this,"Copying from CryPropertyList to object that is not a PropertyList");
}

void CryPropertyList::RemoveAtIterator(Iterator *LI)
{
	if (LI->IsA(TPropertyIterator))
	{
	PropertyIterator  *pi = (CryPropertyList::PropertyIterator *)LI;
	ListIterator *ni = pi->Names;
	CryList *l = (CryList *)	pi->GetOrigContainer();
		l->RemoveAtIterator(ni);
		CryList::RemoveAtIterator(pi);
	}
	else
	throw CryException("RemoveAtIterator called with wrong type of iterator");
}


CryPropertyList::CryPropertyList() : CryList()
{
	Names = new CryList();
}

// Set the Target Object's properties with values held here
void CryPropertyList::Set(CryObject *Target)
{
PropertyIterator *pi = (CryPropertyList::PropertyIterator *)_CreateIterator();
	if (pi->GotoFirst())
	{
		do
		{
		CryPropertyParser ItemString( ((CryString *)pi->GetName())->AsPChar());
		if (Target->HasProperty(ItemString))
			Target->SetProperty(ItemString,*(const CryString *)pi->GetValue());
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
}
/// Get the Source's Properties (loose any currently held)
void CryPropertyList::Get(CryObject *Source)

{
	Clear();
	Names->Clear();
	CryPropertyList *properties = Source->PropertyNames();
	PropertyIterator *p = properties->CreateIterator();
	if (p->GotoFirst())
	{
		do
		{
			AddProperty(p->GetName(),p->GetValue());
		} while(p->GotoNext());
	}
	properties->DeleteIterator(p);
	delete properties;
}
bool CryPropertyList::HasProperty(const CryPropertyParser &PropertyName) const
{
	if (!Names) return false;
	return !(Names->FindNodeValue(PropertyName)==-1);
}
void CryPropertyList::RenameProperty(const char *OldName,const char *NewName)
{
	PropertyIterator *it = CreateIterator();
	if (it->GotoFirst())
	{
		do
		{
			if (*it->GetName()==OldName)
			{
				it->SetName(NewName);
				DeleteIterator(it);
				return;
			}
		} while(it->GotoNext());
	}
	DeleteIterator(it);
	if (!HasProperty(OldName))
		throw(CryException("RenameProperty %s not present in PropertyList",OldName));
}

const char *CryPropertyList::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
	if (HasProperty(PropertyName))
	{
		CryPropertyList::GetValue(PropertyName,Result);
		if (Result=="[]") // flag to show property is a container
			return "*";
		return Result.AsPChar();
	}
	return CryList::GetProperty(PropertyName,Result);
}

/*! will return whether or not the property named in PropertyName is a container */
bool CryPropertyList::GetIsPropertyContainer(const CryPropertyParser &PropertyName) const
{
	const CryObject *v = _GetPropertyAsObject(PropertyName);
	return v->IsContainer();
}

CryObject *CryPropertyList::GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const
{
int i = Names->FindNodeValue(PropertyName);
	if (i>=0)
	{
		PropertyIterator *it = CreateIterator();
		it->GotoFirst();
		while(i)
		{
			it->GotoNext();
			i--;
		}
		CryObject *Item = it->GetValue();
		DeleteIterator(it);
		return Item->Dup();
	}
	throw(CryException("Property %s not present in PropertyList",PropertyName.AsPChar()));
}

CryObject *CryPropertyList::_GetPropertyAsObject(const CryPropertyParser &PropertyName) const
{                                       /* TODO : CryList::ListNode and helper functions should be hidden and anything that currently uses them should use common functions */
int i = Names->FindNodeValue(PropertyName);
	if (i>=0)
	{
		PropertyIterator *it = CreateIterator();
		it->GotoFirst();
		while(i)
		{
			it->GotoNext();
			i--;
		}
		CryObject *Item = it->GetValue();
		DeleteIterator(it);
		return Item;
	}
	throw(CryException("Property %s not present in PropertyList",PropertyName.AsPChar()));
}

/// add a new property to the list, by giving the name and object that it came from. (Object is asked for Property value)
void CryPropertyList::AddPropertyByName(const char *Name,const CryObject *object)
{
CryString *v = new CryString();
	object->GetProperty(Name,*v);
	AddProperty(Name,v);
}

void CryPropertyList::AddProperty(const char *PropertyName,const char *Value)
{
	if (HasProperty(PropertyName))
		throw(CryException("Property %s already present in PropertyList",PropertyName));
	CryString *n = new CryString(PropertyName);
    Names->AddOwned(n);
    CryString *v = new CryString(Value);
	AddOwned(v);
}
void CryPropertyList::AddProperty(const char *Name,CryObject *Value)
{
	if (HasProperty(Name))
		throw(CryException("Property %s already present in PropertyList",Name));
	CryString *n = new CryString(Name);
	Names->AddOwned(n);
	AddOwned(Value);
}
void CryPropertyList::AddProperty(CryString *Name,CryString *Value)
{
if (HasProperty(Name->AsPChar()))
{
	GetValue(Name->AsPChar(),*Value);
		throw(CryException("Property %s(%s) already present in PropertyList",Name->AsPChar()),Value->AsPChar());
}
	Names->AddOwned(Name);
	AddOwned(Value);
}
extern int entries;
/// return the results of the last operation (or current properties)
CryPropertyList *CryPropertyList::PropertyNames() const
{
	CryPropertyList *l = (CryPropertyList *) Dup();
	return l;
}

void CryPropertyList::Load(const CryObject *o)
{
CryList *PropertyNames = o->PropertyNames();
ListIterator *li= PropertyNames->CreateIterator();
	if (li->GotoFirst())
	{
		do
		{
		CryString v;
		CryString *n = (CryString *)li->Get();
			o->GetProperty(n->AsPChar(),v);
			this->AddProperty(n->AsPChar(),v.AsPChar());
		} while(li->GotoNext());
	}
}

CryPropertyList *CryPropertyList::Boolean(const CryObject *O2,BOperation B)
{	// and and or operations implemented
PropertyIterator *pi = CreateIterator();
CryPropertyList *n = new CryPropertyList();
	if (B==Or)
		n->Load(O2);
	if (pi->GotoFirst())
	{
		do
		{

			if (O2->HasProperty(pi->GetName()->AsPChar()))
			{	// And (Same properties from both, with same value or "")
			CryString v;
				O2->GetProperty(pi->GetName()->AsPChar(),v);
				if (*pi->GetValue()!= v)
					v = "";
				n->AddProperty(pi->GetName()->AsPChar(),v.AsPChar());
			}
			else
			{
				if (B==Or)
					n->AddProperty(pi->GetName()->AsPChar(),pi->GetValue()->AsPChar());
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	return n;
}

bool CryPropertyList::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
PropertyIterator *pi = CreateIterator();
	if (pi->GotoFirst())
	{
		do
		{
			if (*pi->GetName()==PropertyName)
			{
				pi->SetValue(PropertyValue);
				DeleteIterator(pi);
				return true;
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	return false;
}
/// return a char * of the value for a property
const char *CryPropertyList::GetValueAsPChar(const CryPropertyParser &PropertyName) const
{
PropertyIterator *pi = CreateIterator();
	if (pi->GotoFirst())
	{
		do
		{
			if (*pi->GetName()==PropertyName)
			{
			const char *v = pi->GetValue()->AsPChar();
				DeleteIterator(pi);
				return v;
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	return "";
}

CryString *CryPropertyList::GetValue(const CryPropertyParser &PropertyName,CryString &Result) const
{
PropertyIterator *pi = CreateIterator();
	if (pi->GotoFirst())
	{
		do
		{
			if (*pi->GetName()==PropertyName)
			{
			CryString *vi = (CryString *)pi->GetValue();
			Result = *vi;
				DeleteIterator(pi);
				return &Result;
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	throw CryException("Property %s Not found",PropertyName.AsPChar());
}




///----------------------------------------------------------------------------
// CryPropertyParser
///----------------------------------------------------------------------------

/// this class is used to easily parse out propertyname[Index]
CryPropertyParser::CryPropertyParser(const char *Property) : CryString(Property)
{
    int StartBracket = this->Pos("[");
    if (StartBracket>-1)
    {
        int StopBracket = this->Pos("]");
        if (StopBracket==-1)
            throw CryException("Bad format of property %s",Property);
        Index = 0;
        int start = StartBracket+1;
        while(start<StopBracket)
        {
            Index *=10;
            Index += Property[start] - '0';
            start++;
        }
        Delete(StartBracket,GetLength());
    }
    else
        Index = -1;
}
int CryPropertyParser::GetIndex() const
{
    return Index;
}
const char *CryPropertyParser::GetPlainProperty() const
{
    return AsPChar();
}
void CryPropertyParser::GetPlainProperty(CryString &Result) const
{
    Result = this;
}
void CryPropertyParser::Get(CryString &Result) const
{
    Result = this;
    if (Index!=-1)
        Result.printf("[%d]",Index);
}



