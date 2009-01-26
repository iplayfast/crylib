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
#include "CodeFactory.h"


using namespace Crystal;
using namespace std;
/*===========================================================================
// CodeFactory
============================================================================*/
CodeFactory::CodeFactory(CodeFactory *_Parent,const char *_Name)    // ProductCount is the number of products a factory can produce
{
    Name = _Name;
    _IsProperty = false;
    Parent = _Parent;
    Count =1;
    Products.SetOwner(this);
    //AddProduct("Source");
}

Object *CodeFactory::ClassCreate(const PropertyParser &PropertyName,Object *Parent)
{
	if (PropertyName==CCodeFactory && Parent->IsA(CCodeFactory))
		return new CodeFactory((CodeFactory *)Parent,"NoName");
	return Factory::ClassCreate(PropertyName,Parent);
}

CodeFactory *CodeFactory::GetParent() const
{
    return Parent;
}
bool CodeFactory::GetIsProperty() const
{
    return _IsProperty;
}
void CodeFactory::SetIsProperty(bool Value)
{
    _IsProperty = Value;
}
bool CodeFactory::GetIsPointer() const
{
    return _IsPointer;
}
void CodeFactory::SetIsPointer(bool Value)
{
    _IsPointer = Value;
}
CodeFactory::~CodeFactory()
{}
PropertyList *CodeFactory::DescribeProducts() const
{
    return Products.PropertyNames();
}
Object *CodeFactory::Create(const PropertyParser &PropertyName,CodeFactory *Parent)
{
    if (PropertyName=="Clear") {
        HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyName);
        if (hp)
            hp->Clear();
        return 0;
    }
    //	if (PropertyName=="VarName")
    //		return 0;
	if (Factory::GetMaxCount()) {
        Object *c = Factory::Create(PropertyName,Parent);   
        return c;
    } else
        return 0;
};

void CodeFactory::CopyTo(Object &Dest) const  //copies contents of this to Dest
{
    if (Dest.IsA(CCodeFactory)) {
        CodeFactory *Cast = (CodeFactory *)&Dest;
        Object *p = (Object *)&Cast->Products;
        Products.CopyTo(*p);
        Cast->_IsPointer = _IsPointer;
        Cast->_IsProperty = _IsProperty;
        Cast->SortValue = SortValue;
        Cast->Name = Name;
        Cast->Parent = Parent;
    }
    Factory::CopyTo(Dest);
}

Object *CodeFactory::Create(const PropertyParser &PropertyName,Object *Parent)
{
	if (PropertyName==CPropertyList)
        return (Object *)&Products;
    if (Parent && Parent->IsA(CCodeFactory))
        return Create(PropertyName,(CodeFactory *)Parent);
    if (PropertyName==CHeadImp) {
        return new HeadImp();
        
    }
    if (IsA(PropertyName))
        return this;
    return Factory::Create(PropertyName,Parent);
}
const char *CodeFactory::GetName() const
{
    return Name;
}
void CodeFactory::SetName(const char *_Name)
{
    Name = _Name;
}
void CodeFactory::AddProduct(const PropertyParser &PropertyName)
{
    Products.AddPropertyOwned(PropertyName,new HeadImp());
}
void CodeFactory::AddFactory(CodeFactory *f)
{
    Factory::AddFactory(f);
}
bool CodeFactory::CanBuildProduct(const PropertyParser &PropertyName)
{
    return Products.HasProperty(PropertyName);
}
void CodeFactory::Clear(const PropertyParser &PropertyName)
{
    HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyName);
    if (hp)
        hp->Clear();
}
bool CodeFactory::IsA(const char *ClassName) const    // can the object map to a ClassName
{
    return(strcmp(ClassName,CCodeFactory)==0) || (strcmp(ClassName,Name.AsPChar())==0) || Factory::IsA(ClassName);
}


void CodeFactory::Sort(int SortType)
{
    // Only one type of sort so SortType doesn't apply
    Factory **NewArray = new Factory*[GetMaxCount()];

    for (int i=0;i<GetMaxCount();i++) {
        NewArray[i] = GetFactory(i);
    }
    int Lowest = -1;
	int SortValue = MAXINT;
	CodeFactory *cf;
	for (int i=0;i<GetMaxCount();i++) {
		for (int j=0;j<GetMaxCount();j++) {
			cf = (CodeFactory *)NewArray[j];
			if (cf) {
				if (Lowest==-1) {
					Lowest = j;
					SortValue = cf->GetSortValue();
				} else
					if (cf->GetSortValue() < SortValue) {
					SortValue = cf->GetSortValue();
					Lowest = j;
				}
			}
		}
		SetFactory(i,NewArray[Lowest]);
		NewArray[Lowest] = 0;
		SortValue = MAXINT;
		Lowest = -1;
	}
	delete []NewArray;
};
void CodeFactory::SetSortValue(int v)
{
	SortValue = v;
}
int CodeFactory::GetSortValue() const
{
    return SortValue;
}
void CodeFactory::SetHead(const PropertyParser &PropertyName,const char *NewHead)
{
    HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyName);
    if (hp) {
        if (*hp->GetHead()=="")
            hp->SetHead(NewHead);
    }
}
void CodeFactory::SetImp(const PropertyParser &PropertyName,const char *NewImp)
{
    HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyName);
    if (hp) {
        if (*hp->GetImp()=="")
            hp->SetImp(NewImp);
    }
}
void CodeFactory::AppendHead(const char *Value)
{
	HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyParser("Source"));
	if (hp) {
		String s;
		s = *hp->GetHead();
		s += Value;
		s+="\n";
		hp->SetHead(s);
	}
}
void CodeFactory::AppendImp(const char *Value)
{
	HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyParser("Source"));
	if (hp) {
		String s;
		s = *hp->GetImp();
		s += Value;
		s+="\n";
		hp->SetImp(s);
    }
}

void CodeFactory::AppendHeadImp(const char *s)
{
	AppendHead(s);
	AppendImp(s);
}

void CodeFactory::SetHeadImp(const PropertyParser &PropertyName,const char *s)
{
	SetHead(PropertyName,s);
	SetImp(PropertyName,s);
}
const String *CodeFactory::GetHead(const PropertyParser &PropertyName) const
{
	HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyName);
	if (hp)
		return hp->GetHead();
	else
		throw Exception("Property Not found");
}
const String *CodeFactory::GetImp(const PropertyParser &PropertyName) const
{
	HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyName);
	if (hp)
		return hp->GetImp();
	else
		throw Exception("Property Not found");
}
int CodeFactory::GetPropertyCount() const
{
	return Factory::GetPropertyCount() + 2; // Name and Products
};
const char * CodeFactory::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	Result.Clear();
	if (PropertyName=="Name") {   // intercept crycontainer's property for our own
		Result = Name;  // if Result != what is returned, it's a special situation
		return Result.AsPChar();
	}
	if (PropertyName=="Products") {
		Result = "[]";  // signal that Products is a container
		return "*";
	}
	if (PropertyName=="IsPointer") {
		Result = _IsPointer ? "Yes" : "No";
		return Result.AsPChar();
	}
	if (PropertyName=="IsProperty") {
		Result = _IsProperty ? "Yes" : "No";
		return Result.AsPChar();
	}
	return Factory::GetProperty(PropertyName,Result);
};

	/// will return a property represented as an object, useful for classes which contain properties that are dynamically allocated, as a property that is dynamic is a Object and therefore callable
Object *CodeFactory::GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const
{
	if (PropertyName=="Products") {
		return Products.Dup();
	}
	if (PropertyName=="Name") {
		return Name.Dup();
	}
	return _GetPropertyAsObject(PropertyName)->Dup();
}
	/// will return a pointer to the property if the property is an Object (or decendent)
Object *CodeFactory::_GetPropertyAsObject(const PropertyParser &PropertyName) const
{
	return Factory::_GetPropertyAsObject(PropertyName);
}

bool  CodeFactory::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Products") {
		AddProduct(PropertyParser(PropertyValue));
		return true;
	}
	if (PropertyName=="Name") {
		Name = PropertyValue;
		return true;
	}
	if (PropertyName=="IsPointer") {
		SetIsPointer(strcasecmp(PropertyValue,"Yes")==0);
		return true;
	}
	if (PropertyName=="IsProperty") {
		SetIsProperty(strcasecmp(PropertyValue,"Yes")==0);
		if (_IsProperty)
		{
			AddProduct(PropertyParser(CGetProperty));
			AddProduct(PropertyParser(CSetProperty));
		}
		return true;
	}
	return Factory::SetProperty(PropertyName,PropertyValue);
}


PropertyList *CodeFactory::PropertyNames() const
{
	PropertyList *pn = Factory::PropertyNames();
	pn->AddPropertyByName("Name",this);
	pn->AddPropertyByName("Products",this);
	pn->AddPropertyByName("IsPointer",this);
	pn->AddPropertyByName("IsProperty",this);
	return pn;
	//  pn->AddOwned(new String("Values"));
};
bool CodeFactory::HasProperty(const PropertyParser &PropertyName) const
{
	return(PropertyName=="Name") ||
	(PropertyName=="Products") ||
	(PropertyName=="IsPointer") ||
	(PropertyName=="IsProperty") ||
	Factory::HasProperty(PropertyName);
}
bool CodeFactory::GetIsPropertyContainer(const PropertyParser &PropertyName) const
{
	if (PropertyName=="Products")
		return true;
	return Factory::GetIsPropertyContainer(PropertyName);
}

Object *CodeFactory::Create(Stream &FromStream)
{
	throw(Exception("Not Implemented"));
}

