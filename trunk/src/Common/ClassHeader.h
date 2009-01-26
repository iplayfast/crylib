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
#include "ClassObject.h"
#include "ClassArray.h"
#include "CryBackProp.h"
#include "CryFuzzy.h"
#include "CryPattern.h"
#include "ClassBuilder.h"

namespace Crystal
{
#ifndef CClassHeaderFactory
#define CClassHeaderFactory "ClassHeaderFactory"

class ClassHeaderFactory : public CodeFactory
{
public:
StdFunctionsNoDup(ClassHeaderFactory,CodeFactory);
    ClassHeaderFactory(CodeFactory *Parent);
    virtual Object *CreateC(const char *PropertyName,CodeFactory *Parent) { return Create(PropertyParser(PropertyName),Parent); }
	virtual Object *Create(const PropertyParser &PropertyName,CodeFactory *Parent);
    virtual bool Present(const Object *Name);// can be either a function def or variable name
	virtual Object *Create(const PropertyParser &PropertyName,Object *Parent);
    virtual Object *Create(Stream &FromStream)
    {
      return CodeFactory::Create(FromStream);
    }
    virtual Object *Create(const char *FactoryName,const PropertyParser &PropertyName,Object *Parent)
    {
      return CodeFactory::Create(FactoryName,PropertyName,Parent);
    }

};
#endif
}
