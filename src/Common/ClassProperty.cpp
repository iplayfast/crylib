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
void Property::Init(const char *_Name,const char *_Value)
{
	Value = new String(_Value);
	if (Value==0)
		throw Exception("Out of memory creating CryProperty");

	Name = new String(_Name);
	if (Name==0)
	{
		delete Value;
		throw Exception("Out of memory creating CryProperty");
	}
}
Property::Property(const char *_Name,const char *_Value)
{
	Init(_Name,_Value);
}

Property::Property(const char *_Name)
{
	Init(_Name,"");
}

Property::Property(Property &Copy)
{
	if (&Copy!=this)
	{
	Init(Copy.Name->AsPChar(),Copy.Value->AsPChar());
	};
}

Property::~Property()
{
	delete Value;
	Value = 0;
	delete Name;
	Name = 0;
}

void Property::SetValue(const char *_Value)
{
	delete Value;
	Value = new String(_Value);
}
void Property::SetValue(const Object *_Value)
{
	delete Value;
	Value = _Value->Dup();
}
void Property::SetName(const char *_Name)
{
	*Name = _Name;
}

const Object *Property::GetValue() const
{
	return Value;
}
const char *Property::GetValue(String &Result) const	// return string value or Object name if not CryString
{
	if (Value->IsA(CString)) {
		Result = *(String *) Value;
	}
	else
		Result = Value->ChildClassName();
	return Result.AsPChar();
}
Object *Property::_GetValue() const
{
	return Value;
}
const cbyte* Property::GetRaw() const
{
	return Value->GetRaw();
}

bool Property::HasProperty(const PropertyParser &PropertyName) const
{
#ifdef DEBUG
	if (PropertyName=="ObjectID")	// properties are normally parts of other objects so ObjectID doesn't make sense when saving or loading these
		return false;
#endif
	if (*GetName()=="NoName")
		return true;	// Properties can have any one Property, none has been assigned yet so go for it
    return (PropertyName==*GetName()) || Object::HasProperty(PropertyName);
}

FunctionDefList *Property::GetFunctions(const char *Type) const
{
    // if a type has been defined and it's not this class, check subclasses for it
    if (Type && !IsA(Type))
        return Object::GetFunctions(Type);
    // otherwise get any functions in subclasses
    FunctionDefList *l = Object::GetFunctions();
    String s;
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

const char *Property::GetProperty(String &Result) const
{
	if (Value->IsA(CString))
	{
		Result = (String *)Value;
		return Result.AsPChar();
	}
	Result = Value->ChildClassName();
	return Result.AsPChar();
}

const char *Property::GetProperty(const PropertyParser &PropertyName,String &Result) const
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

bool Property::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
String v(PropertyValue);
	return SetProperty(PropertyName,&v);
}
bool Property::SetProperty(const PropertyParser &PropertyName,const Object* PropertyValue)
{
	if (PropertyName==*GetName())
	{
		delete Value;
		Value = PropertyValue->Dup();
		return true;
	}
	else
	{
		if (Object::HasProperty(PropertyName) && PropertyValue->IsA(CString))
			return Object::SetProperty(PropertyName,*((String *)PropertyValue));
		else
		{
			SetName(PropertyName);
			Value = PropertyValue->Dup();
		// normally we throw an exception, but CryProperty is a special case, as it's properties are user definable
//			throw CryException(this,ExceptionUnknownProperty,"Unknown Property ",PropertyName.AsPChar());
		}
	}

}

PropertyList *Property::PropertyNames() const
{
//	PropertyList *n = CryObject::PropertyNames();
	// avoid ObjectID as it's not valid for properties
	PropertyList *n = new PropertyList();
	if (GetName()!="")
	{
	Object *v = GetValue()->Dup();
	const char *N = GetName()->AsPChar();
		n->AddProperty(GetName()->AsPChar(),v);
		delete v;
	}
	return n;
}

void Property::CopyTo(Object &Dest) const   //copies contents of this to Dest
{
	if (Dest.IsA(CProperty))
	{
	Property *Cast = (Property *)&Dest;
		Cast->SetProperty(Name->AsPChar(),Value);
	}
	String r;
	GetProperty(Name->AsPChar(),r);
	Dest.SetProperty(Name->AsPChar(),r);
}

const String *Property::GetName() const
{
	return Name;
}

Object *Property::Dup() const  /// creates a duplicate of this object
{
    Property *n = new Property(GetName()->AsPChar());
    n->SetValue(Value);
    return n;
}

size_t Property::Size() const
{
	return Name->Size() + Value->Size();
}


///----------------------------------------------------------------------------
//PropertyList
///----------------------------------------------------------------------------

PropertyList::~PropertyList()
{
}

PropertyList::PropertyIterator::PropertyIterator(const PropertyList *container)
		: List::ListIterator((List *)container)
{
};

void PropertyList::PropertyIterator::CopyTo(Object &Dest) const
{
	if (!Dest.IsA(CPropertyIterator))
		List::ListIterator::CopyTo(Dest);
}
Object *PropertyList::PropertyIterator::Dup() const
{
PropertyList *l =(PropertyList *)this->GetOrigContainer();

	PropertyIterator *New = l->CreateIterator();
	CopyTo(*New);
	return New;
}


bool PropertyList::PropertyIterator::GotoFirst()
{
	return	List::ListIterator::GotoFirst();
}
bool PropertyList::PropertyIterator::GotoPrev()
{
	return List::ListIterator::GotoPrev();
}
bool PropertyList::PropertyIterator::GotoNext()
{
	return List::ListIterator::GotoNext();
}
bool PropertyList::PropertyIterator::GotoLast()
{
	return List::ListIterator::GotoLast();
}
size_t PropertyList::PropertyIterator::GetItemSize()
{
	Property *p = _Get();
	return p->Size();
}
const String *PropertyList::PropertyIterator::GetName()
{
	Property *p = _Get();
	return p->GetName();
}
const char *PropertyList::PropertyIterator::GetValue(String &r)
{
	Property *p = _Get();
	p->GetProperty(r);
	return r.AsPChar();
}

void PropertyList::PropertyIterator::SetName(const char *Name)
{
	Property *p = _Get();
	p->SetName(Name);
}
void PropertyList::PropertyIterator::SetValue(const char *Value)
{
	Property *p = _Get();
	p->SetValue(Value);
}

//CryContainer::Iterator * PropertyList::CreateNameIterator() const
//{
//	return Names->_CreateIterator();
//}

//void PropertyList::DeleteNameIterator(Iterator *I) const
//{
//    Names->DeleteIterator(I);
//}

PropertyList::PropertyIterator *PropertyList::CreateIterator() const
{
	return (PropertyIterator *)_CreateIterator();
}

Container::Iterator *PropertyList::_CreateIterator() const
{
	return new PropertyIterator(this);
}

void PropertyList::DeleteIterator(Iterator *LI) const
{
	delete (PropertyIterator *)LI;
}
size_t PropertyList::Size() const
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
int PropertyList::Compare(int CompareType,const Object *Test1,const Object *Test2) const
{
	if (CompareType==0) {
		if(Test2->IsA(CMemStream))
		{
		MemStream *t2 = (MemStream *)Test2;
			if (Test1->IsA(CProperty))
			{
			Property *p = (Property *)Test1;
				return (p->GetName()->EqualTo(CompareType,t2));
			}
		}
		else
		{
			if (Test2->IsA(CProperty)) {
			const String *Name = ((Property *)Test2)->GetName();
			if (Test1->IsA(CProperty))
			{
			Property *p = (Property *)Test1;
				return (p->GetName()->EqualTo(CompareType,Name));
			}

			}
		}
	}
	return -1;
}

void PropertyList::RemoveNodeValue(const MemStream &Needle)
{
int n = FindNodeValue(Needle);
	if (n>=0)
	{
		PropertyList::PropertyIterator *i = CreateIterator();
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
void PropertyList::CopyTo(Object &Dest) const
{
	if (Dest.IsA(CPropertyList))
	{
		PropertyList *Cast = (PropertyList *)&Dest;
		List::CopyTo(*Cast);
	}
	else
	throw Exception(this,"Copying from PropertyList to object that is not a PropertyList");
}

/* should be covered in the base class

void PropertyList::RemoveAtIterator(Iterator *LI)
{
	if (LI->IsA(CPropertyIterator))
	{
	PropertyIterator  *pi = (PropertyList::PropertyIterator *)LI;
	CryList *l = (CryList *)	ni->GetOrigContainer();
		l->RemoveAtIterator(ni);
		ni = pi;
		CryList::RemoveAtIterator(ni);
	}
	else
	throw CryException("RemoveAtIterator called with wrong type of iterator");
} */


PropertyList::PropertyList() : List()
{
}

// Set the Target Object's properties with values held here
void PropertyList::Set(Object *Target)
{
PropertyIterator *pi = (PropertyList::PropertyIterator *)_CreateIterator();
String r;
	if (pi->GotoFirst())
	{
		do
		{
		PropertyParser ItemString( ((String *)pi->GetName())->AsPChar());
		if (Target->HasProperty(ItemString))
			Target->SetProperty(ItemString,*(const String *)pi->GetValue(r));
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
}
/// Get the Source's Properties (loose any currently held)
void PropertyList::Get(Object *Source)
{
	Clear();
	PropertyList *Properties = Source->PropertyNames();
	SwapListElements(Properties);
	delete Properties;
}
bool PropertyList::HasProperty(const PropertyParser &PropertyName) const
{
PropertyIterator *pi = CreateIterator();
	if (pi->GotoFirst()) {
		do
		{
		Property *p = pi->_Get();
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
void PropertyList::RenameProperty(const char *OldName,const char *NewName)
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
		throw(Exception("RenameProperty %s not present in PropertyList",OldName));
}

const char *PropertyList::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (HasProperty(PropertyName))
	{
		PropertyList::GetValue(PropertyName,Result);
		if (Result=="[]") // flag to show property is a container
			return "*";
		return Result.AsPChar();
	}
	return List::GetProperty(PropertyName,Result);
}

/*! will return whether or not the property named in PropertyName is a container */
bool PropertyList::GetIsPropertyContainer(const PropertyParser &PropertyName) const
{
	const Object *v = _GetPropertyAsObject(PropertyName);
	return v->IsContainer();
}

Object *PropertyList::GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const
{
PropertyIterator *pi = CreateIterator();
	if (pi->GotoFirst()) {
		do
		{
		Property *p = pi->_Get();
			if (*p->GetName()==PropertyName)
			{
			const Object *Item = p->GetValue();
				DeleteIterator(pi);
				return Item->Dup();
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	throw(Exception("Property %s not present in PropertyList",PropertyName.AsPChar()));
}

Object *PropertyList::_GetPropertyAsObject(const PropertyParser &PropertyName) const
{                                       /* TODO : CryList::ListNode and helper functions should be hidden and anything that currently uses them should use common functions */
PropertyIterator *pi = CreateIterator();
	if (pi->GotoFirst()) {
		do
		{
		Property *p = pi->_Get();
			if (*p->GetName()==PropertyName)
			{
			Object *Item = p->_GetValue();
				DeleteIterator(pi);
				return Item;
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	throw(Exception("Property %s not present in PropertyList",PropertyName.AsPChar()));
}

/// add a new property to the list, by giving the name and object that it came from. (Object is asked for Property value)
void PropertyList::AddPropertyByName(const char *Name,const Object *object)
{
String *v = new String();
	object->GetProperty(Name,*v);
	AddProperty(Name,v);
	delete v;
}

void PropertyList::AddProperty(const char *PropertyName,const char *Value)
{
	if (HasProperty(PropertyName))
		throw(Exception("Property %s already present in PropertyList",PropertyName));
	Property *p = new Property(PropertyName,Value);
	AddOwned(p);
}
void PropertyList::AddProperty(const char *Name,Object *Value)
{
	if (HasProperty(Name))
		throw(Exception("Property %s already present in PropertyList",Name));
	Property *p = new Property(Name);
	p->SetValue(Value);
	AddOwned(p);
}
void PropertyList::AddProperty(String *Name,String *Value)
{
	if (HasProperty(Name->AsPChar()))
	{
		GetValue(Name->AsPChar(),*Value);
			throw(Exception("Property %s(%s) already present in PropertyList",Name->AsPChar()),Value->AsPChar());
	}
	Property *p = new Property(*Name,*Value);
	AddOwned(p);
}
extern int entries;
/// return the results of the last operation (or current properties)
PropertyList *PropertyList::PropertyNames() const
{
	PropertyList *l = (PropertyList *) Dup();
	return l;
}

void PropertyList::Load(const Object *o)
{
List *PropertyNames = o->PropertyNames();
ListIterator *li= PropertyNames->CreateIterator();
	if (li->GotoFirst())
	{
		do
		{
		String v;
		String *n = (String *)li->Get();
			o->GetProperty(n->AsPChar(),v);
			this->AddProperty(n->AsPChar(),v.AsPChar());
		} while(li->GotoNext());
	}
}

PropertyList *PropertyList::Boolean(const Object *O2,BOperation B)
{	// and and or operations implemented
PropertyIterator *pi = CreateIterator();
PropertyList *n = new PropertyList();
	if (B==Or)
		n->Load(O2);
	if (pi->GotoFirst())
	{
		do
		{

			if (O2->HasProperty(pi->GetName()->AsPChar()))
			{	// And (Same properties from both, with same value or "")
			String v,r;
				O2->GetProperty(pi->GetName()->AsPChar(),v);
				if (v!=pi->GetValue(r))
					v = "";
				n->AddProperty(pi->GetName()->AsPChar(),v.AsPChar());
			}
			else
			{
			String r;
				if (B==Or)
					n->AddProperty(pi->GetName()->AsPChar(),pi->GetValue(r));
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	return n;
}

bool PropertyList::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
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
const char *PropertyList::GetValueAsPChar(const PropertyParser &PropertyName,String &Result) const
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

const char *PropertyList::GetValue(const PropertyParser &PropertyName,String &Result) const
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
	throw Exception("Property %s Not found",PropertyName.AsPChar());
}

void PropertyList::Sort(int CompareType)
{
	List::Sort(CompareType);
}



///----------------------------------------------------------------------------
// CryPropertyParser
///----------------------------------------------------------------------------

/// this class is used to easily parse out propertyname[Index]
PropertyParser::PropertyParser(const char *Property) : String(Property)
{
	int StartBracket = this->Pos("[");
	if (StartBracket>-1)
	{
		int StopBracket = this->Pos("]");
		if (StopBracket==-1)
			throw Exception("Bad format of property %s",Property);
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
int PropertyParser::GetIndex() const
{
	return Index;
}
const char *PropertyParser::GetPlainProperty() const
{
	return AsPChar();
}
void PropertyParser::GetPlainProperty(String &Result) const
{
	Result = this;
}
void PropertyParser::Get(String &Result) const
{
	Result = this;
	if (Index!=-1)
		Result.printf("[%d]",Index);
}



