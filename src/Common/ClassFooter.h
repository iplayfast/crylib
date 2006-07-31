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


namespace Crystal
{
#ifndef TClassFooterFactory
#define TClassFooterFactory "ClassFooterFactory"
class ClassFooterFactory : public CodeFactory
{
public:
     StdFunctionsNoDup(ClassFooterFactory,CodeFactory);
    ClassFooterFactory(CodeFactory *Parent);
    virtual CryObject *Create(const CryPropertyParser &PropertyName,CodeFactory *Parent);
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

};
#endif
}
