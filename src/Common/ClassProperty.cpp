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
		throw Exception(ErrorOutOfMemory,"Out of memory creating CryProperty");

	Name = new String(_Name);
	if (Name==0)
	{
		delete Value;
		throw Exception(ErrorOutOfMemory,"Out of memory creating CryProperty");
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

Property::Property(Property &Copy) : Object()
{
	if (&Copy!=this)
	{
	Init(Copy.Name->AsPChar(),Copy.Value->AsPChar());
	};
}

Object *Property::GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const
{
	if (PropertyName==*GetName())
		return Value->Dup();//GetCopyOfPropertyAsObject(PropertyName);
	else
		return Object::GetCopyOfPropertyAsObject(PropertyName);
}

Property::~Property()
{
	delete Value;
	Value = 0;
	delete Name;
	Name = 0;
}

bool Property::HasProperty(const char *_PropertyName) const
{
PropertyParser pp(_PropertyName);
	return HasProperty(pp);
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
void Property::SetValueOwned(Object *_Value)
{
	delete Value;
	Value = _Value;
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
	if (PropertyName=="_Name")
		return true;
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
	s += "virtual void CopyTo(Object &Dest) const;";
	s += "virtual Object *Dup() const;";
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
	Result = "";
	Value->SaveTo(Result);
	return Result.AsPChar();
}

bool Property::GetIsPropertyContainer(const PropertyParser &PropertyName) const
{
	if (PropertyName=="_Name")
		return false;
	if (PropertyName==*GetName())
		return Value->IsContainer();
	else
		return Object::GetIsPropertyContainer(PropertyName);
}

const char *Property::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (PropertyName=="_Name")
	{
		Result = GetName();
		return Result;
	}
	if (Value->HasProperty(PropertyName))	// always let the object give results if able
	{
		return Value->GetProperty(PropertyName,Result);
	}
	if (PropertyName==*GetName())	// if PropertyName is not the stored one, see if this classes property is what is being requested
	{
		if (Value->IsContainer())
		{
		PropertyParser p("Values");
			return Value->GetProperty(p,Result);
		}
		return GetProperty(Result);
	}
	else
	{	
		return Object::GetProperty(PropertyName,Result);
	}
}

bool Property::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
String v(PropertyValue);
	return SetProperty(PropertyName,&v);
}
void Property::SetPropertyOwned(Object *PropertyValue)
{
	delete Value;
	Value = PropertyValue;
}
bool Property::SetProperty(const PropertyParser &PropertyName,const Object* PropertyValue)
{
	if (PropertyName=="_Name")	// this one is automatically assigned to be the property name
	{
		if (PropertyValue->IsA(CString))
		{
			SetName(*(const String *)PropertyValue);
		}
		else
			throw Exception(ErrorGeneral,"Cannot Set Property _Name with %s, Needs to be a String",PropertyValue->ClassName());
		return true;
	}
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
			delete Value;
			Value = PropertyValue->Dup();
// normally we throw an exception, but Property is a special case, as it's properties are user definable
//			throw CryException(this,ExceptionUnknownProperty,"Unknown Property ",PropertyName.AsPChar());
			return true;
		}
	}

}

PropertyList *Property::PropertyNames() const
{
//	PropertyList *n = Object::PropertyNames();
	// avoid ObjectID as it's not valid for properties
	PropertyList *n = new PropertyList();
	if (*GetName()!="")
	{
		n->AddProperty("_Name",GetName()->AsPChar());
		n->AddProperty(GetName()->AsPChar(),GetValue());
//		delete v;
	}
	return n;
}

void Property::CopyTo(Object &Dest) const   //copies contents of this to Dest
{
	if (Dest.IsA(CProperty))
	{
	Property *Cast = (Property *)&Dest;
	PropertyParser p(Name->AsPChar());
		Cast->SetProperty(p,Value);
	}
	String r;
	PropertyParser p(Name->AsPChar());
	GetProperty(p,r);
	Dest.SetProperty(p,r);
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
	l->DeleteIterator(New);
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
PropertyIterator *pi = CreateIterator();
size_t size = 0;
	if (pi->GotoFirst()) {
	   do
	   {
			size += pi->Size();
	   } while(pi->GotoNext());
	}
	DeleteIterator(pi);
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
	PropertyIterator *i = CreateIterator();
		if (i->GotoFirst())
		{
			while(n)
			{
				n--;
				if (!i->GotoNext())
				{		// node wasn't found... shouldn't get here
					return;
				}
			}
			RemoveAtIterator(i);
		}
		DeleteIterator(i);
	}
}
void PropertyList::CopyTo(Object &Dest) const
{
	if (Dest.IsA(CPropertyList))
	{
		PropertyList *Cast = (PropertyList *)&Dest;
		PropertyIterator *i = CreateIterator();
		if (i->GotoFirst())
		{
			do
			{
			Property *p = i->_Get();
				p = (Property *)p->Dup();
				Cast->Add(p);
			} while(i->GotoNext());
		}
		DeleteIterator(i);
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
PropertyIterator *pi = this->CreateIterator();
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
	/// make sure you delete result after using
PropertyList::PropertyIterator * PropertyList::GetPropertyPointerIterator(ListIterator *li,bool ExceptOnNotFound) const
{
PropertyIterator *pi = CreateIterator();
	if (pi->GotoFirst())
	{
		do
		{
		Property *p = pi->_Get();
			if (*p->GetName()==*((String *)li->Get()))	// item in list has matched list it item
			{
				if (li->GotoNext())	// more in list it
				{
					if (p->GetValue()->IsA(CPropertyList))	// Item is a property list
					{
						PropertyList *pl = (PropertyList *)p->GetValue();
						return pl->GetPropertyPointerIterator(li,false);	// is if it has it
					}
					// name matched but complete list didn't
					if (!ExceptOnNotFound)
						return 0;
					const char *s =*((String *)li->Get());
					throw Exception(ErrorPropertyNotFound,"property ...][%s] not found",s);
				}
				// no more in list we are done!
				return pi;
			}
		} while(pi->GotoNext());
		if (!ExceptOnNotFound)
			return 0;

		const char *s =*((String *)li->Get());
		throw Exception(ErrorPropertyNotFound,"property [%s] not found",s);
	}
	if (!ExceptOnNotFound)
		return 0;
	const char *s =*((String *)li->Get());
	throw Exception(ErrorPropertyNotFound,"property [%s] not found",s);
}

/// make sure you delete result after using
PropertyList::PropertyIterator * PropertyList::GetPropertyPointerIterator(const PropertyParser &PropertyName,bool ExceptOnNotFound) const
{
	if (PropertyName.IsMultiField())
	{
	ListIterator *li = PropertyName.CreateIterator();
		try
		{
			li->GotoFirst();
			PropertyList::PropertyIterator * pl = GetPropertyPointerIterator(li,ExceptOnNotFound);
			PropertyName.DeleteIterator(li);
			return pl;
		}
		catch(Exception &e)
		{
			PropertyName.DeleteIterator(li);
			throw e;
		}
	}
	PropertyList::PropertyIterator * pi = CreateIterator();
	if (pi->GotoFirst()) {
		do
		{
		Property *p = pi->_Get();
			if (*p->GetName()==PropertyName)
			{
				return pi;
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	if (!ExceptOnNotFound)
    	return 0;
	const char *s =PropertyName;
		throw Exception(ErrorPropertyNotFound,"property %s not found",s);
}


Property *PropertyList::GetPropertyPointer(ListIterator *li,bool ExceptOnNotFound) const
{
PropertyList::PropertyIterator * pi=GetPropertyPointerIterator(li,ExceptOnNotFound);
	if (!pi)
    	return 0;
Property *p = pi->_Get();
	DeleteIterator(pi);
	return p;
}

Property *PropertyList::GetPropertyPointer(const PropertyParser &PropertyName,bool ExceptOnNotFound) const
{
PropertyList::PropertyIterator * pi = GetPropertyPointerIterator(PropertyName,ExceptOnNotFound);
	if (!pi) return 0;
Property *p = pi->_Get();
	DeleteIterator(pi);
	return p;
}

void PropertyList::AddPropertyOwned(ListIterator *li,Object *Value)
{
PropertyIterator *pi = CreateIterator();
	try
	{
	String *PropertyName = ((String *)li->Get());
	if (pi->GotoFirst())
	{
		do
		{
		Property *p = pi->_Get();
			if (*p->GetName()==*PropertyName)	// item in list has matched list it item
			{
				DeleteIterator(pi);	// no matter what we are done with this
				if (li->GotoNext())	// more in list it
				{
					if (p->GetValue()->IsA(CPropertyList))	// Item is a property list
					{
						PropertyList *pl = (PropertyList *)p->GetValue();
						pl->AddPropertyOwned(li,Value);	// is if it has it
					}
					else	// not a list so just add it
					{
						p->SetValueOwned(Value);
					}
					return;
				}
			}
		} while(pi->GotoNext());
		DeleteIterator(pi);
	}
	 // Property not in list

	if (li->GotoNext())  // more fields available in PropertyDefinition
	{
		// no more in list, add new list
		PropertyList *pl = new PropertyList();
		AddProperty(*PropertyName,pl);
		pl->AddPropertyOwned(li,Value);
	}
	else
	{
		AddProperty(*PropertyName,Value);	// add the value to this list
	}
	return;
	}
	catch(Exception &e)
	{
		DeleteIterator(pi);
		throw e;
	}
}
bool PropertyList::HasProperty(const char *PropertyName) const
{
PropertyParser pp(PropertyName);
	return _HasProperty(pp)!=0;
}

bool PropertyList::HasProperty(const PropertyParser &PropertyName) const
{
	return _HasProperty(PropertyName)!=0;
}

Property *PropertyList::_HasProperty(const PropertyParser &PropertyName) const// return Property * or 0
{
	try
	{
	Property *p = GetPropertyPointer(PropertyName,false);
		return p;
	}
	catch(Exception &e)
	{
		if (e.GetErrno()==ErrorPropertyNotFound)
			return 0;
		throw e;
	}
}

void PropertyList::RenameProperty(const char *OldName,const char *NewName)
{
	PropertyParser pp(OldName);
	Property *p = GetPropertyPointer(pp,true);
	p->SetName(NewName);
}
void PropertyList::RemoveProperty(const char *Name)
{
PropertyParser pp(Name);
PropertyList::PropertyIterator *pi=GetPropertyPointerIterator(pp,true);
	RemoveAtIterator(pi);
    	DeleteIterator(pi);
}


const char *PropertyList::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (HasProperty(PropertyName))
	{
		return PropertyList::GetValue(PropertyName,Result);
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
	const Object *v = _GetPropertyAsObject(PropertyName);
	return v->Dup();

}


Object *PropertyList::_GetPropertyAsObject(const PropertyParser &PropertyName) const
{                                       /* TODO : CryList::ListNode and helper functions should be hidden and anything that currently uses them should use common functions */
Property *p = GetPropertyPointer(PropertyName,true);
	Object *Item = p->_GetValue();
		return Item;
}

/// add a new property to the list, by giving the name and object that it came from. (Object is asked for Property value)
void PropertyList::AddPropertyByName(const char *Name,const Object *object)
{
String *Result = new String();
const char *c=object->GetProperty(Name,*Result);
	if ((*c=='*') && (*Result=="[]"))
	{
	PropertyList *pl1 = new PropertyList();// values property
	PropertyList *pl = new PropertyList();	// values[indexed] properties
	Property *p = new Property(Name);
		p->SetValueOwned(pl);
	pl1->AddOwned(p);
	Container *c = (Container *)object;
	Iterator *i = c->_CreateIterator();
	int Index=0;
		try
		{
			if (i->GotoFirst())
			{
				do
				{
				String *v = new String();
				c->SaveAsText(i,*v);
				String NewName;
					NewName.printf("%s_%d",Name,Index++);
				Property *p = new Property(*(&NewName),*v);
					pl->AddOwned(p);
				} while(i->GotoNext());
			}
			c->DeleteIterator(i);
		}
		catch(Exception &e)
		{
			c->DeleteIterator(i);
			if (e.GetErrno()!=ErrorPropertyNotFound)
				throw e;

		}
		AddPropertyOwned(Name,pl1);
	}
	else
	AddPropertyOwned(Name,Result);
}

void PropertyList::AddProperty(const char *PropertyName,const char *Value)
{
	if (HasProperty(PropertyName))
		throw(Exception(ErrorPropertyAlreadyPresent,"Property %s already present in PropertyList",PropertyName));
	Property *p = new Property(PropertyName,Value);
	AddOwned(p);
}
void PropertyList::AddProperty(const char *Name,const Object *Value)
{
	if (HasProperty(Name))
		throw(Exception(ErrorPropertyAlreadyPresent,"Property %s already present in PropertyList",Name));
	Property *p = new Property(Name);
	p->SetValue(Value);
	AddOwned(p);
}
void PropertyList::AddPropertyOwned(const char *Name,Object *Value)
{
const PropertyParser PropertyName(Name);
	AddPropertyOwned(PropertyName,Value);
}

void PropertyList::AddPropertyOwned(const PropertyParser &PropertyName,Object *Value)
{
//	if (HasProperty(PropertyName))
//		throw(Exception(ErrorPropertyAlreadyPresent,"Property %s already present in PropertyList",Name));
Property *p = _HasProperty(PropertyName);
	if (p)
	{
		p->SetValueOwned(Value);
		return;
	}

	if (PropertyName.IsMultiField())
	{
	ListIterator *li = PropertyName.CreateIterator();
		try
		{
			li->GotoFirst();
			AddPropertyOwned(li,Value);
			PropertyName.DeleteIterator(li);
		}
		catch(Exception &e)
		{
			PropertyName.DeleteIterator(li);
			throw e;
		}
	}
	else
	{
		Property *p = new Property(PropertyName);
		p->SetValueOwned(Value);
		AddOwned(p);
	}
}

void PropertyList::AddProperty(String *Name,String *Value)
{
PropertyParser pp(Name->AsPChar());
	if (HasProperty(pp))
	{
		GetValue(pp,*Value);
			throw(Exception(ErrorPropertyAlreadyPresent,"Property %s(%s) already present in PropertyList",Name->AsPChar()),Value->AsPChar());
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
	try
	{
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
		DeleteIterator(li);
	}
	catch(Exception &e)
	{
		DeleteIterator(li);
		throw e;
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

			if (O2->HasProperty(PropertyParser(pi->GetName()->AsPChar())))
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
Property *p;
	try
	{
		p = GetPropertyPointer(PropertyName,true);
		p->SetValue(PropertyValue);
		return true;
	}
	catch(Exception &e)
	{
		if (e.GetErrno()==ErrorPropertyNotFound)
		{
			String *s = new String;
				*s = PropertyValue;
			AddPropertyOwned(PropertyName,s);
			return true;
		}
		throw e;
	}
}

void PropertyList::AddProperty(PropertyParser &PropertyName,const String *PropertyValue)
{
AddPropertyOwned(PropertyName,PropertyValue->Dup());
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
			Property *p = (Property *)pi->GetObject();
				if (p->GetValue()->IsA(CPropertyList))
				{
					Result = "[]";	// mark it's special
					return "*";
				}
				pi->GetValue(Result);
				DeleteIterator(pi);
				return Result.AsPChar();
			}
		} while(pi->GotoNext());
	}
	DeleteIterator(pi);
	throw Exception(ErrorPropertyNotFound,"Property %s Not found",PropertyName.AsPChar());
}

void PropertyList::Sort(int CompareType)
{
	List::Sort(CompareType);
}

bool PropertyList::SetPropertiesFromList(Object *Target)
{
bool Result = false;
PropertyIterator *i = CreateIterator();
		if (i->GotoFirst())
		{
			do
			{
			Property *p = i->_Get();
				if ((*p->GetName()=="Values") && (p->GetValue()->IsA(CPropertyList)))
				{
				PropertyList *pl = (PropertyList *)p->GetValue();
				PropertyIterator *i = pl->CreateIterator();
					if (i->GotoFirst())
					{
						do
						{
						Property *p = i->_Get();
							const String *n = p->GetName();
							String s;
								s = (String *) n;
								s.Replace("_","[");
								s += "]";
								if (p->GetValue()->IsA(CString))
								{
									String *v = (String *)p->GetValue();
									Result |= Target->SetProperty(s.AsPChar(),v->AsPChar());
								}
								else
								{
									p->SetName(s);
									Result |= Target->SetProperty(p);
								}
						} while(i->GotoNext());
					}
                    pl->DeleteIterator(i);
				}
				else
					Target->SetProperty(p);
			} while(i->GotoNext());
		}
		return Result;
}


///----------------------------------------------------------------------------
// PropertyParser
///----------------------------------------------------------------------------


PropertyParser::PropertyParser(const String *Property) : String(Property)
{
	constructParser();
}

/// this class is used to easily parse out propertyname[Index]
PropertyParser::PropertyParser(const char *Property) : String(Property)
{
	constructParser();
}


// format can be string[0-9|text][0-9|text]...

void PropertyParser::constructParser()
{
const char *Property = *this;
	Index = -1;
	int StartBracket = this->Pos("[");
	if (StartBracket>-1)
	{

		TagString *ts = new TagString(this);
		ts->Delete(StartBracket,MAXINT);
		Delete(0,StartBracket);
		if (ts->Length()>0)
			Fields.AddOwned(ts);
		StartBracket = 0;

		while (StartBracket>-1)
		{
			int StopBracket = this->Pos("]");
			if (StopBracket==-1)
				throw Exception("Bad format of property %s",Property);

			int start = StartBracket+1;
			if (Property[start]>='0' && Property[start]<='9')
			{
			int Index=0;
				while(start<StopBracket)
				{
					Index *=10;
					Index += Property[start] - '0';
					start++;
				}
				ts->SetTag(Index);
			}
			else
			{
				ts =new TagString();
				ts->CopyFromStr(*this,StartBracket+1,StopBracket);
				Fields.AddOwned(ts);
			}
			Delete(StartBracket,StopBracket+1);
			StartBracket = this->Pos("[");
			if ((StartBracket==-1) && Length()>0)
			{
				ts = new TagString(this);
				Fields.AddOwned(ts);
				Delete(0,Length());
			}
		}
	}
}

bool PropertyParser::operator ==(const char *s) const
{
	if (IsMultiField())
	{
	bool Result = false;
	Iterator *i = Fields.CreateIterator();
		if (i->GotoFirst())
		{
			do
			{
				if (i->IsObject())
				{
					if (i->GetObject()->IsA(CTagString))
					{
					TagString *ss = (TagString *)i->GetObject();
						Result |= *ss == s;
					}
				}
				else
				{
					throw Exception("Corrupted memory? Shouldn't get here!");
				}
			} while(i->GotoNext());
		}
		Fields.DeleteIterator(i);
		return Result;
	}
	else
	{
	String *ss = (String *)this;
		return *ss == s;
	}
}

int PropertyParser::GetIndex() const
{
	if (IsMultiField())
	{
	int Index = 0;
	Iterator *i = Fields.CreateIterator();
		if (i->GotoFirst())
		{
			if (i->IsObject())
			{
				if (i->GetObject()->IsA(CTagString))
				{
					TagString *ss = (TagString *)i->GetObject();
					Index = ss->GetTag();
				}
			}
		}
		Fields.DeleteIterator(i);
		return Index;	// local var
	}
	return Index;	// class's var
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



