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
#ifndef CHeadImp
#define CHeadImp "HeadImp"

#include "CryObject.h"
#include "ClassString.h"

namespace Crystal {

/**
@author Chris Bruner
*/
class HeadImp : public String
{
    String *Head;
    String *Imp;
    virtual void Clear(int amount) {}

public:
StdFunctions(HeadImp,String);
    HeadImp();

    ~HeadImp();
    const String *GetHead() const;
    const String *GetImp() const;
    void SetHead(const char *v);
    void SetImp(const char *v);
	void Clear();
	virtual void CopyTo(Object &Object) const;
	//virtual void CopyToStream(CryStream &Dest,CopyStyle Style = NORMAL) const;
	virtual bool CanDup() const { return true; }
	/// will return a property represented as an object, useful for classes which contain properties that are dynamically allocated, as a property that is dynamic is a CryObject and therefore callable
	virtual Object *GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const;
	/// will return a pointer to the property if the property is an CryObject (or decendent)
	virtual Object *_GetPropertyAsObject(const PropertyParser &PropertyName) const;

/*! will return whether or not the property named in PropertyName is a container */
    virtual bool GetIsPropertyContainer(const PropertyParser &PropertyName) const;
    /*! will return the property named in PropertyName in a string format */
    virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;
    /*! returns true if the class in question has the property */
    virtual bool HasProperty(const PropertyParser &PropertyName) const;
    /*! The count of the properties a class has */
    virtual int GetPropertyCount() const;
	/*! Make a list of all property names, the function is called from the parent class through each inheritance until it reaches this class, at which point a list is created and filled with any properties on the way back through the inheritance */
	virtual PropertyList* PropertyNames() const;

	virtual bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);

};

}

#endif
