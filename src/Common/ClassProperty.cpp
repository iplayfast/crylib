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
	delete Value;
	Value = new CryString(_Value);
}
void CryProperty::SetValue(const Object *_Value)
{
	delete Value;
	Value = _Value->Dup();
}
void CryProperty::SetName(const char *_Name)
{
	*Name = _Name;
}

const Object *CryProperty::GetValue() const
{
	return Value;
}
const char *CryProperty::GetValue(CryString &Result) const	// return string value or Object name if not CryString
{
	if (Value->IsA(CCryString)) {
		Result = *(CryString *) Value;
	}
	else
		Result = Value->ChildClassName();
	return Result.AsPChar();
}
Object *CryProperty::_GetValue() const
{
	return Value;
}
const cbyte* CryProperty::GetRaw() const
{
	return Value->GetRaw();
}

bool CryProperty::HasProperty(const CryPropertyParser &PropertyName) const
{
#ifdef DEBUG
	if (PropertyName=="ObjectID")	// properties are normally parts of other objects so ObjectID doesn't make sense when saving or loading these
		return false;
#endif
	if (*GetName()=="NoName")
		return true;	// Properties can have any one Property, none has been assigned yet so go for it
    return (PropertyName==*GetName()) || Object::HasProperty(PropertyName);
}

CryFunctionDefList *CryProperty::GetFunctions(const char *Type) const
{
    // if a type has been defined and it's not this class, check subclasses for it
    if (Type && !IsA(Type))
        return Object::GetFunctions(Type);
    // otherwise get any functions in subclasses
    CryFunctionDefList *l = Object::GetFunctions();
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

const char *CryProperty::GetProperty(CryString &Result) const
{
	if (Value->IsA(CCryString))
	{
		Result = (CryString *)Value;
		return Result.AsPChar();
	}
	Result = Value->ChildClassName();
	return Result.AsPChar();
}

const char *CryProperty::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
	if (PropertyName==*GetName())	// if PropertyName is not the stored one, see if this classes property is what is being requested
	{
		return GetProperty(Result);
	}
/*	else
	{
		return CryObject::GetProperty(PropertyName,Result);
	}
	*/
}

bool CryProperty::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
CryString v(PropertyValue);
	return SetProperty(PropertyName,&v);
}
bool CryProperty::SetProperty(const CryPropertyParser &PropertyName,const Object* PropertyValue)
{
	if (PropertyName==*GetName())
	{
		delete Value;
		Value = PropertyValue->Dup();
		return true;
	}
	else
	{
		if (Object::HasProperty(PropertyName) && PropertyValue->IsA(CCryString))
			return Object::SetProperty(PropertyName,*((CryString *)PropertyValue));
		else
		{
			SetName(PropertyName);
			Value = PropertyValue->Dup();
		// normally we throw an exception, but CryProperty is a special case, as it's properties are user definable
//			throw CryException(this,ExceptionUnknownProperty,"Unknown Property ",PropertyName.AsPChar());
		}
	}

}

CryPropertyList *CryProperty::PropertyNames() const
{
//	CryPropertyList *n = CryObject::PropertyNames();
	// avoid ObjectID as it's not valid for properties
	CryPropertyList *n = new CryPropertyList();
	if (GetName()!="")
		n->AddProperty(GetName()->AsPChar(),GetValue()->Dup());
	return n;
}

void CryProperty::CopyTo(Object &Dest) const   //copies contents of this to Dest
{
	if (Dest.IsA(CCryProperty))
	{
	CryProperty *Cast = (CryProperty *)&Dest;
		Cast->SetProperty(Name->AsPChar(),Value);
	}
	CryString r;
	GetProperty(Name->AsPChar(),r);
	Dest.SetProperty(Name->AsPChar(),r);
}

const CryString *CryProperty::GetName() const
{
	return Name;
}

Object *CryProperty::Dup() const  /// creates a duplicate of this object
{
    CryProperty *n = new CryProperty(GetName()->AsPChar());
    n->SetValue(Value);
    return n;
}

size_t CryProperty::Size() const
{
	return Name->Size() + Value->Size();
}


///----------------------------------------------------------------------------
//CryPropertyList
///----------------------------------------------------------------------------

CryPropertyList::~CryPropertyList()
{
}

CryPropertyList::PropertyIterator::PropertyIterator(const CryPropertyList *container)
		: List::ListIterator((List *)container)
{
};

void CryPropertyList::PropertyIterator::CopyTo(Object &Dest) const
{
	if (!Dest.IsA(CPropertyIterator))
		List::ListIterator::CopyTo(Dest);
}
Object *CryPropertyList::PropertyIterator::Dup() const
{
CryPropertyList *l =(CryPropertyList *)this->GetOrigContainer();

	PropertyIterator *New = l->CreateIterator();
	CopyTo(*New);
	return New;
}


bool CryPropertyList::PropertyIterator::GotoFirst()
{
	return	List::ListIterator::GotoFirst();
}
bool CryPropertyList::PropertyIterator::GotoPrev()
{
	return List::ListIterator::GotoPrev();
}
bool CryPropertyList::PropertyIterator::GotoNext()
{
	return List::ListIterator::GotoNext();
}
bool CryPropertyList::PropertyIterator::GotoLast()
{
	return List::ListIterator::GotoLast();
}
size_t CryPropertyList::PropertyIterator::GetItemSize()
{
	CryProperty *p = _Get();
	return p->Size();
}
const CryString *CryPropertyList::PropertyIterator::GetName()
{
	CryProperty *p = _Get();
	return p->GetName();
}
const char *CryPropertyList::PropertyIterator::GetValue(CryString &r)
{
	CryProperty *p = _Get();
	p->GetProperty(r);
	return r.AsPChar();
}

void CryPropertyList::PropertyIterator::SetName(const char *Name)
{
	CryProperty *p = _Get();
	p->SetName(Name);
}
void CryPropertyList::PropertyIterator::SetValue(const char *Value)
{
	CryProperty *p = _Get();
	p->SetValue(Value);
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

Container::Iterator *CryPropertyList::_CreateIterator() const
{
	return new PropertyIterator(this);
}

void CryPropertyList::DeleteIterator(Iterator *LI) const
{
	delete (PropertyIterator *)LI;
}
size_t CryPropertyList::Size() const
{
PropertyIterator *pi = this->CreateIterator();
size_t size = 0;
	if (pi->GotoFirst()) {
	   do
	   {
			size += pi->Size();
	   } while(pi->GotoNext());
	}
	return size;
}
int CryPropertyList::Compare(int CompareType,const Object *Test1,const Object *Test2) const
{
	if (CompareType==0) {
		if(Test2->IsA(CCryMemStream))
		{
		CryMemStream *t2 = (CryMemStream *)Test2;
			if (Test1->IsA(CCryProperty))
			{
			CryProperty *p = (CryProperty *)Test1;
				return (p->GetName()->EqualTo(CompareType,t2));
			}
		}
		else
		{
			if (Test2->IsA(CCryProperty)) {
			const CryString *Name = ((CryProperty *)Test2)->GetName();
			if (Test1->IsA(CCryProperty))
			{
			CryProperty *p = (CryProperty *)Test1;
				return (p->GetName()->EqualTo(CompareType,Name));
			}

			}
        }
	}
	return -1;
}

void CryPropertyList::RemoveNodeValue(const CryMemStream &Needle)
{
int n = FindNodeValue(Needle);
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
void CryPropertyList::CopyTo(Object &Dest) const
{
	if (Dest.IsA(CCryPropertyList))
	{
		CryPropertyList *Cast = (CryPropertyList *)&Dest;
		List::CopyTo(*Cast);
	}
	else
	throw CryException(this,"Copying from CryPropertyList to object that is not a PropertyList");
}

/* should be covered in the base class

void CryPropertyList::RemoveAtIterator(Iterator *LI)
{
	if (LI->IsA(CPropertyIterator))
	{
	PropertyIterator  *pi = (CryPropertyList::PropertyIterator *)LI;
	CryList *l = (CryList *)	ni->GetOrigContainer();
		l->RemoveAtIterator(ni);
		ni = pi;
		CryList::RemoveAtIterator(ni);
	}
	else
	throw CryException("RemoveAtIterator called with wrong type of iterator");
} */


CryPropertyList::CryPropertyList() : List()
{
}

// Set the Target Object's properties with values held here
void CryPropertyList::Set(Object *Target)
{
PropertyIterator *pi = (CryPropertyList::PropertyIterator *)_CreateIterator();
CryString r;
	if (pi->GotoFirst())
	{
		do
		{
		CryPropertyParser ItemString( ((CryString *)pi->GetName())->AsPChar());
		if (Target->HasProperty(ItemString))
			Target->SetProperty(ItemString,*(const CryString *)pi->GetValue(r));
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
}
/// Get the Source's Properties (loose any currently held)
void CryPropertyList::Get(Object *Source)
{
	Clear();
	CryPropertyList *Properties = Source->PropertyNames();
	SwapListElements(Properties);
	delete Properties;
}
bool CryPropertyList::HasProperty(const CryPropertyParser &PropertyName) const
{
PropertyIterator *pi = CreateIterator();
	if (pi->GotoFirst()) {
		do
		{
		CryProperty *p = pi->_Get();
			if (*p->GetName()==PropertyName)
			{
				DeleteIterator(pi);
				return true;
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	return false;
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
	return List::GetProperty(PropertyName,Result);
}

/*! will return whether or not the property named in PropertyName is a container */
bool CryPropertyList::GetIsPropertyContainer(const CryPropertyParser &PropertyName) const
{
	const Object *v = _GetPropertyAsObject(PropertyName);
	return v->IsContainer();
}

Object *CryPropertyList::GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const
{
PropertyIterator *pi = CreateIterator();
	if (pi->GotoFirst()) {
		do
		{
		CryProperty *p = pi->_Get();
			if (*p->GetName()==PropertyName)
			{
			const Object *Item = p->GetValue();
				DeleteIterator(pi);
				return Item->Dup();
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	throw(CryException("Property %s not present in PropertyList",PropertyName.AsPChar()));
}

Object *CryPropertyList::_GetPropertyAsObject(const CryPropertyParser &PropertyName) const
{                                       /* TODO : CryList::ListNode and helper functions should be hidden and anything that currently uses them should use common functions */
PropertyIterator *pi = CreateIterator();
	if (pi->GotoFirst()) {
		do
		{
		CryProperty *p = pi->_Get();
			if (*p->GetName()==PropertyName)
			{
			Object *Item = p->_GetValue();
				DeleteIterator(pi);
				return Item;
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	throw(CryException("Property %s not present in PropertyList",PropertyName.AsPChar()));
}

/// add a new property to the list, by giving the name and object that it came from. (Object is asked for Property value)
void CryPropertyList::AddPropertyByName(const char *Name,const Object *object)
{
CryString *v = new CryString();
	object->GetProperty(Name,*v);
	AddProperty(Name,v);
}

void CryPropertyList::AddProperty(const char *PropertyName,const char *Value)
{
	if (HasProperty(PropertyName))
		throw(CryException("Property %s already present in PropertyList",PropertyName));
	CryProperty *p = new CryProperty(PropertyName,Value);
	AddOwned(p);
}
void CryPropertyList::AddProperty(const char *Name,Object *Value)
{
	if (HasProperty(Name))
		throw(CryException("Property %s already present in PropertyList",Name));
	CryProperty *p = new CryProperty(Name);
	p->SetValue(Value);
	AddOwned(p);
}
void CryPropertyList::AddProperty(CryString *Name,CryString *Value)
{
	if (HasProperty(Name->AsPChar()))
	{
		GetValue(Name->AsPChar(),*Value);
			throw(CryException("Property %s(%s) already present in PropertyList",Name->AsPChar()),Value->AsPChar());
	}
	CryProperty *p = new CryProperty(*Name,*Value);
	AddOwned(p);
}
extern int entries;
/// return the results of the last operation (or current properties)
CryPropertyList *CryPropertyList::PropertyNames() const
{
	CryPropertyList *l = (CryPropertyList *) Dup();
	return l;
}

void CryPropertyList::Load(const Object *o)
{
List *PropertyNames = o->PropertyNames();
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

CryPropertyList *CryPropertyList::Boolean(const Object *O2,BOperation B)
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
			CryString v,r;
				O2->GetProperty(pi->GetName()->AsPChar(),v);
				if (v!=pi->GetValue(r))
					v = "";
				n->AddProperty(pi->GetName()->AsPChar(),v.AsPChar());
			}
			else
			{
			CryString r;
				if (B==Or)
					n->AddProperty(pi->GetName()->AsPChar(),pi->GetValue(r));
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
const char *CryPropertyList::GetValueAsPChar(const CryPropertyParser &PropertyName,CryString &Result) const
{
PropertyIterator *pi = CreateIterator();
	if (pi->GotoFirst())
	{
		do
		{
			if (*pi->GetName()==PropertyName)
			{
			const char *v = pi->GetValue(Result);
				DeleteIterator(pi);
				return v;
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	Result = "";
	return Result.AsPChar();
}

const char *CryPropertyList::GetValue(const CryPropertyParser &PropertyName,CryString &Result) const
{
PropertyIterator *pi = CreateIterator();
	if (pi->GotoFirst())
	{
		do
		{
			if (*pi->GetName()==PropertyName)
			{
				pi->GetValue(Result);
				DeleteIterator(pi);
				return Result.AsPChar();
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	throw CryException("Property %s Not found",PropertyName.AsPChar());
}

void CryPropertyList::Sort(int CompareType)
{
	List::Sort(CompareType);
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



