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
CryPropertyList *CodeFactory::DescribeProducts() const
{
    return Products.PropertyNames();
}
CryObject *CodeFactory::Create(const CryPropertyParser &PropertyName,CodeFactory *Parent)
{
    if (PropertyName=="Clear") {
        HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyName);
        if (hp)
            hp->Clear();
        return 0;
    }
    //	if (PropertyName=="VarName")
    //		return 0;
    if (CryFactory::GetMaxCount()) {
        CryObject *c = CryFactory::Create(PropertyName,Parent);   
        return c;
    } else
        return 0;
};

void CodeFactory::CopyTo(CryObject &Dest) const  //copies contents of this to Dest
{
    if (Dest.IsA(TCodeFactory)) {
        CodeFactory *Cast = (CodeFactory *)&Dest;
        CryObject *p = (CryObject *)&Cast->Products;
        Products.CopyTo(*p);
        Cast->_IsPointer = _IsPointer;
        Cast->_IsProperty = _IsProperty;
        Cast->SortValue = SortValue;
        Cast->Name = Name;
        Cast->Parent = Parent;
    }
    CryFactory::CopyTo(Dest);
}

CryObject *CodeFactory::Create(const CryPropertyParser &PropertyName,CryObject *Parent)
{
    if (PropertyName==TCryPropertyList)
        return (CryObject *)&Products;
    if (Parent && Parent->IsA(TCodeFactory))
        return Create(PropertyName,(CodeFactory *)Parent);
    if (PropertyName==THeadImp) {
        return new HeadImp();
        
    }
    if (IsA(PropertyName))
        return this;
    return CryFactory::Create(PropertyName,Parent);
}
const char *CodeFactory::GetName() const
{
    return Name;
}
void CodeFactory::SetName(const char *_Name)
{
    Name = _Name;
}
void CodeFactory::AddProduct(const CryPropertyParser &PropertyName)
{
    Products.AddProperty(PropertyName,new HeadImp());
}
void CodeFactory::AddFactory(CodeFactory *f)
{
    CryFactory::AddFactory(f);
}
bool CodeFactory::CanBuildProduct(const CryPropertyParser &PropertyName)
{
    return Products.HasProperty(PropertyName);
}
void CodeFactory::Clear(const CryPropertyParser &PropertyName)
{
    HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyName);
    if (hp)
        hp->Clear();
}
bool CodeFactory::IsA(const char *ClassName) const    // can the object map to a ClassName
{
    return(strcmp(ClassName,TCodeFactory)==0) || (strcmp(ClassName,Name.AsPChar())==0) || CryFactory::IsA(ClassName);
}


void CodeFactory::Sort(int SortType)
{
    // Only one type of sort so SortType doesn't apply
    CryFactory **NewArray = new CryFactory*[GetMaxCount()];

    for (int i=0;i<GetMaxCount();i++) {
        NewArray[i] = GetFactory(i);
    }
    int Lowest = -1;
    int SortValue = 100000;
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
        SortValue = 100000;
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
void CodeFactory::SetHead(const CryPropertyParser &PropertyName,const char *NewHead)
{
    HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyName);
    if (hp) {
        if (*hp->GetHead()=="")
            hp->SetHead(NewHead);
    }
}
void CodeFactory::SetImp(const CryPropertyParser &PropertyName,const char *NewImp)
{
    HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyName);
    if (hp) {
        if (*hp->GetImp()=="")
            hp->SetImp(NewImp);
    }
}
void CodeFactory::AppendHead(const char *Value)
{
	HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject("Source");
	if (hp) {
		CryString s;
		s = *hp->GetHead();
		s += Value;
		s+="\n";
		hp->SetHead(s);
	}
}
void CodeFactory::AppendImp(const char *Value)
{
	HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject("Source");
	if (hp) {
		CryString s;
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

void CodeFactory::SetHeadImp(const CryPropertyParser &PropertyName,const char *s)
{
    SetHead(PropertyName,s);
    SetImp(PropertyName,s);
}
const CryString *CodeFactory::GetHead(const CryPropertyParser &PropertyName) const
{
    HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyName);
    if (hp)
        return hp->GetHead();
    else
        throw CryException("Property Not found");
}
const CryString *CodeFactory::GetImp(const CryPropertyParser &PropertyName) const
{
    HeadImp *hp = (HeadImp *)Products._GetPropertyAsObject(PropertyName);
    if (hp)
        return hp->GetImp();
    else
        throw CryException("Property Not found");
}
int CodeFactory::GetPropertyCount() const
{
    return CryFactory::GetPropertyCount() + 2; // Name and Products
};
const char * CodeFactory::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
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
        Result = _IsPointer ? "yes" : "no";
        return Result.AsPChar();
    }
    if (PropertyName=="IsProperty") {
        Result = _IsProperty ? "yes" : "no";
        return Result.AsPChar();
    }
    return CryFactory::GetProperty(PropertyName,Result);
};

	/// will return a property represented as an object, useful for classes which contain properties that are dynamically allocated, as a property that is dynamic is a CryObject and therefore callable
CryObject *CodeFactory::GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const
{
	if (PropertyName=="Products") {
		return Products.Dup();
	}
	if (PropertyName=="Name") {
		return Name.Dup();
	}
	return _GetPropertyAsObject(PropertyName)->Dup();
}
	/// will return a pointer to the property if the property is an CryObject (or decendent)
CryObject *CodeFactory::_GetPropertyAsObject(const CryPropertyParser &PropertyName) const
{
	return CryFactory::_GetPropertyAsObject(PropertyName);
}

bool  CodeFactory::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
    if (PropertyName=="Products") {
        AddProduct(PropertyValue);
        return true;
    }
    if (PropertyName=="Name") {
        Name = PropertyValue;
        return true;
    }
    if (PropertyName=="IsPointer") {
        SetIsPointer(PropertyValue=="yes");
        return true;
    }
    if (PropertyName=="IsProperty") {
        SetIsProperty(PropertyValue=="yes");
        return true;
    }
    return CryFactory::SetProperty(PropertyName,PropertyValue);
}


CryPropertyList *CodeFactory::PropertyNames() const
{
    CryPropertyList *pn = CryFactory::PropertyNames();
    pn->AddPropertyByName("Name",this);
    pn->AddPropertyByName("Products",this);
    pn->AddPropertyByName("IsPointer",this);
    pn->AddPropertyByName("IsProperty",this);
    return pn;
    //  pn->AddOwned(new CryString("Values"));
};
bool CodeFactory::HasProperty(const CryPropertyParser &PropertyName) const
{
    return(PropertyName=="Name") ||
    (PropertyName=="Products") ||
    (PropertyName=="IsPointer") ||
    (PropertyName=="IsProperty") ||
    CryFactory::HasProperty(PropertyName);
}
bool CodeFactory::GetIsPropertyContainer(const CryPropertyParser &PropertyName) const
{
    if (PropertyName=="Products")
        return true;
    return CryFactory::GetIsPropertyContainer(PropertyName);
}

CryObject *CodeFactory::Create(CryStream &FromStream)
{
    throw(CryException("Not Implemented"));
}

