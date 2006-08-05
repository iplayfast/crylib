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
#include "ClassBuilder.h"
#include "CryXML.h"
#include "CryBackProp.h"

//#define DIAGNOSE
#include "ClassFooter.h"

using namespace Crystal;
using namespace std;


/*===========================================================================
// ClassFooterFactory
============================================================================*/

ClassFooterFactory::ClassFooterFactory(CodeFactory *Parent) : CodeFactory(Parent,TClassFooterFactory)
{
    SetSortValue(99);
    AddProduct(TClassFooterFactory);
}

CryObject *ClassFooterFactory::Create(const CryPropertyParser &PropertyName,CodeFactory *Parent)
{
    if (PropertyName==TClassFooterFactory)
    {
        if (!Parent->IsA(TClassBuilder))
            return this;


        CryString s;
        s.printf("}; // class %s\n\n}; // namespace Crystal \n",
                 Parent->GetName());
        SetHead(PropertyName,s);
        Parent->AppendHead(*GetHead(PropertyName));
        return this;
    }
    return CodeFactory::Create(PropertyName,Parent);
}