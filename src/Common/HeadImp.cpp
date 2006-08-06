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
#include "HeadImp.h"
#include "ClassProperty.h"


/*===========================================================================
// HeadImp
// Handles strings holding code for the header file and the implementations file
============================================================================*/



//namespace Crystal {
using namespace Crystal;
using namespace std;
HeadImp::HeadImp(): CryString("[]")
{
	Head  = new CryString();
	Imp = new CryString();
}


HeadImp::~HeadImp()
{
	delete Head;
	delete Imp;
}




/**
 * 
 * @return the string representing the code for the header
 */
const CryString *HeadImp::GetHead() const
{
    Head->SeekToStart();
    return Head;
}

/**
 * 
 * @return the string representing the code for the implementation
 */
const CryString *HeadImp::GetImp() const
{
    Imp->SeekToStart();
    return Imp;
}
/**
 * 
 * @param v represents the code the header should have
 */
void HeadImp::SetHead(const char *v)
{
    *((CryString *)Head) = v;
    Head->SeekToStart();
}
/**
 * 
 * @param v represents the code the implementation should have
 */
void HeadImp::SetImp(const char *v)
{
    *((CryString *)Imp) = v;
    Imp->SeekToStart();
}

/**
 * Clear the implementation and header code
 */
void HeadImp::Clear()
{
	Head->Clear();
    Imp->Clear();
}

	/// will return a property represented as an object, useful for classes which contain properties that are dynamically allocated, as a property that is dynamic is a CryObject and therefore callable
CryObject *HeadImp::GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const
{
	if (PropertyName=="Declaration")
		return Head->Dup();
	if (PropertyName=="Implementation")
		return Imp->Dup();
	return CryObject::GetCopyOfPropertyAsObject(PropertyName);
}
	/// will return a pointer to the property if the property is an CryObject (or decendent)
CryObject *HeadImp::_GetPropertyAsObject(const CryPropertyParser &PropertyName) const
{
	if (PropertyName=="Declaration")
		return Head;
	if (PropertyName=="Implementation")
		return Imp;
	return CryObject::_GetPropertyAsObject(PropertyName);
}
/*! will return whether or not the property named in PropertyName is a container */
bool HeadImp::GetIsPropertyContainer(const CryPropertyParser &PropertyName) const
{
    if (PropertyName=="Declaration")
        return false;
    if (PropertyName=="Implementation")
		return false;
    return true;
}
/*! will return the property named in PropertyName in a string format */
const char *HeadImp::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
    if (PropertyName=="Declaration")
    {
        Result = *Head;
        return Result.AsPChar();
    }
    if (PropertyName=="Implementation")
    {
        Result = *Imp;
        return Result.AsPChar();
    }
    return CryObject::GetProperty(PropertyName,Result);
}
/*! returns true if the class in question has the property */
bool HeadImp::HasProperty(const CryPropertyParser &PropertyName) const
{
    return (PropertyName=="Declaration") ||
           (PropertyName=="Implementation") ||
           CryObject::HasProperty(PropertyName);
}

void HeadImp::CopyTo(CryObject &Object) const
{
    if (Object.IsA(THeadImp))
    {
        HeadImp *Dest = (HeadImp *)&Object;
		CryString::CopyTo(*Dest); // copy base class
		Head->CopyTo(*Dest->Head);
		Imp->CopyTo(*Dest->Imp);
	}
    else
        CryString::CopyTo(Object);
}


/*! The count of the properties a class has */
int HeadImp::GetPropertyCount() const
{
    return CryObject::GetPropertyCount() + 2;
}
/*! Make a list of all property names, the function is called from the parent class through each inheritance until it reaches this class, at which point a list is created and filled with any properties on the way back through the inheritance */
CryPropertyList* HeadImp::PropertyNames() const
{
	CryPropertyList *n = CryObject::PropertyNames();
	n->AddPropertyByName("Declaration",this);
	n->AddPropertyByName("Implementation",this);
	return n;
}
bool HeadImp::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
    if (PropertyName=="Declaration")
    {
        SetHead(PropertyValue);
        return true;
    }
    if (PropertyName=="Implementation")
    {
        SetImp(PropertyValue);
        return true;
    }
    return CryObject::SetProperty(PropertyName,PropertyValue);
}

//}
