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
//#include "classbuilder.h"
#include "PrimInstance.h"

namespace Crystal {

#ifndef CClassInstance
#define CClassInstance "ClassInstance"

class ClassBuilder;

/**
When designing a new class this class will represent internal instances of classes used by the class. (The same way that you can have int's etc).

@author Chris Bruner
*/
class ClassInstance : public PrimInstance
{
    CryString Type;// TODO Handle Instance as pointer
    Object *p;	// one instance so we can figure out includes and abstract
/// checks for file preexisting, and if not and p->IsA(Type) then adds it.
	bool HasDefault;	// set to true if a default property is set, 
public:
StdFunctionsNoDup(ClassInstance,CodeFactory);
	ClassInstance(CodeFactory *Parent) : PrimInstance(Parent) { p = 0; HasDefault = false;}
//	ClassInstance(CodeFactory *Parent,const char *ClassType,const char *ClassName,int count,bool IsProperty,bool IsPointer,bool IsArrayPointer,const char *DefaultValue);
    virtual Object *Create(const CryPropertyParser &PropertyName,CodeFactory *Parent);
    virtual const char *GetType() const;
	const Object *Getp() const { return p; }
	virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
    virtual bool HasProperty(const CryPropertyParser &PropertyName) const;
    /*! The count of the properties a class has */
    virtual int GetPropertyCount() const;
    /*! will return the property named in PropertyName in a string format */
    virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;

	/*! Make a list of all property names, the function is called from the parent class through each inheritance until it reaches this class, at which point a list is created and filled with any properties on the way back through the inheritance */
	virtual CryPropertyList* PropertyNames() const;

    //bool GetIsProperty() const;
    //void SetIsArrayPointer(bool _IsArrayPointer);
    //bool GetIsArrayPointer() const;
    virtual Object *Create(const CryPropertyParser &PropertyName,Object *Parent)
    {
      return CodeFactory::Create(PropertyName,Parent);
    }
    virtual Object *Create(CryStream &FromStream)
    {
      return CodeFactory::Create(FromStream);
    }
    virtual Object *Create(const char *FactoryName,const CryPropertyParser &PropertyName,Object *Parent)
    {
      return PrimInstance::Create(FactoryName,PropertyName,Parent);
    }
	virtual const char *Code_AssignDefaultValue(CryString &Result) const;
virtual 	const char *Code_SetProperty(CryString &Result) const;
virtual 	const char *Code_GetProperty(CryString &Result) const;

};



#endif
}
