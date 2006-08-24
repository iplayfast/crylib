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
#include "CryObject.h"
#include "CryArray.h"
#include "CryBackProp.h"
#include "CryFuzzy.h"
#include "CryPattern.h"
#include "ClassBuilder.h"

namespace Crystal
{
#ifndef TPrimInstance
#define TPrimInstance "PrimInstance"


class PrimInstance : public CodeFactory
{
    enum _eType { Unknown,Char,CharStr,Int,Float,Bool,Double,Class };
    CryString DefaultValue;
    _eType eType;
    bool IsArrayPointer;
public:
StdFunctionsNoDup(PrimInstance,CodeFactory);
	PrimInstance(CodeFactory *Parent);
    PrimInstance(CodeFactory *Parent,const char *PrimType,const char *PrimName,const char *_DefaultValue, int _Count,bool _IsProperty,bool IsPointer,bool IsArrayPointer);
    virtual CryObject *Create(const CryPropertyParser &PropertyName,CodeFactory *Parent);
    void SetIsArrayPointer(bool _IsArrayPointer);
    bool GetIsArrayPointer() const;
	void SetType(const char *PrimType);
	virtual const char *GetType() const;
	void SetCount();
	void SetDefaultValue(const char *_DefaultValue);
        /*! will return a const pointer to the property */
    virtual CryObject *GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const;
    /*! will return whether or not the property named in PropertyName is a container */
    virtual bool GetIsPropertyContainer(const CryPropertyParser &PropertyName) const;
    /*! will return the property named in PropertyName in a string format */
    virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
    /*! returns true if the class in question has the property */
    virtual bool HasProperty(const CryPropertyParser &PropertyName) const;
    /*! The count of the properties a class has */
    virtual int GetPropertyCount() const;
    /*! Make a list of all property names, the function is called from the parent class through each inheritance until it reaches this class, at which point a list is created and filled with any properties on the way back through the inheritance */
	virtual CryPropertyList* PropertyNames() const;
	virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
	virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent)
    {
      return CodeFactory::Create(PropertyName,Parent);
    }
    virtual CryObject *Create(CryStream &FromStream)
    {
      return CodeFactory::Create(FromStream);
    }
    virtual CryObject *Create(const char *FactoryName,const CryPropertyParser &PropertyName,CryObject *Parent)
    {
      return CodeFactory::Create(FactoryName,PropertyName,Parent);
    }
	// routines to do code assignments
virtual 	const char *Code_AssignDefaultValue(CryString &Result) const;
virtual 	const char *Code_SetProperty(CryString &Result) const;
virtual 	const char *Code_GetProperty(CryString &Result) const;
};
#endif
}
