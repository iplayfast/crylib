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
#ifndef THeadImp
#define THeadImp "HeadImp"

#include "CryObject.h"
#include "ClassString.h"

namespace Crystal {

/**
@author Chris Bruner
*/
class HeadImp : public CryString
{
    CryString *Head;
    CryString *Imp;
    virtual void Clear(int amount) {}

public:
StdFunctions(HeadImp,CryString);
    HeadImp();

    ~HeadImp();
    const CryString *GetHead() const;
    const CryString *GetImp() const;
    void SetHead(const char *v);
    void SetImp(const char *v);
	void Clear();
	virtual void CopyTo(CryObject &Object) const;
	//virtual void CopyToStream(CryStream &Dest,CopyStyle Style = NORMAL) const;
	virtual bool CanDup() const { return true; }
	/// will return a property represented as an object, useful for classes which contain properties that are dynamically allocated, as a property that is dynamic is a CryObject and therefore callable
	virtual CryObject *GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const;
	/// will return a pointer to the property if the property is an CryObject (or decendent)
	virtual CryObject *_GetPropertyAsObject(const CryPropertyParser &PropertyName) const;

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

};

}

#endif
