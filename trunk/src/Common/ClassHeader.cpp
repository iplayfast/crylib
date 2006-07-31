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
#include "ClassFooter.h"
#include "PrimInstance.h"
#include "ClassInstance.h"
#include "ClassFunction.h"
//#define DIAGNOSE
#include "ClassHeader.h"

using namespace Crystal;
using namespace std;



/*===========================================================================
// ClassHeaderFactory
============================================================================*/


ClassHeaderFactory::ClassHeaderFactory(CodeFactory *Parent) : CodeFactory(Parent,TClassHeaderFactory)
{
    SetSortValue(1);
    AddProduct(TClassHeaderFactory);
    AddProduct(TConstructorStart);
    AddProduct(TConstructorEnd);
    AddProduct(TDestructorStart);
    AddProduct(TDestructorEnd);
    AddFactory(new ClassFooterFactory(this));
}

CryObject *ClassHeaderFactory::Create(const CryPropertyParser &PropertyName,CodeFactory *Parent)
{
    if (!Parent->IsA(TClassBuilder))
    {
        if (strcmp(PropertyName,TInheritedFactory)==0)
        {
            CryFunctionDef dummy("void foo()");
            InheritedFactory *i = new InheritedFactory(this,dummy);
            AddFactory(i);
            return i;
        }
        if (strcmp(PropertyName,TPrimInstance)==0)
        {
            PrimInstance *pi = new PrimInstance(this);
            AddFactory(pi);
            return pi;
        }
    }
    else
    {
        if (strcmp(PropertyName,TClassHeaderFactory)==0)
        {
            if (Parent->IsA(TClassBuilder))
            {
                ClassBuilder *cbParent = (ClassBuilder *)Parent;
                CryString s;
                s.printf("namespace Crystal {\nusing namespace Crystal;\n\nclass %s : public %s\n{\n",
                         cbParent->GetName(),cbParent->GetInheritType());
                SetHead(PropertyName,s);
                Parent->AppendHead(*GetHead(PropertyName));
                Parent->AppendHeadImp("\n\n//Array Length defines\n");
                Create(TCountDefine,Parent);
                Parent->AppendHeadImp("\n\n//Local Variables Declaration\n");
                Create(TDeclaration,Parent);
		Parent->AppendHead("\n\npublic:");
                Parent->AppendHeadImp("\n\n//Constructor/Destructor Functions \n");
                Create(TConstructorStart,Parent);
                Create(TConstructor,Parent);
                Create(TConstructorEnd,Parent);
                Create(TDestructorStart,Parent);
                Create(TDestructor,Parent);
                Create(TDestructorEnd,Parent);
                Parent->AppendHeadImp("\n\n//Inherited Functions\n");
                Create(TInheritedFactory,Parent);
		s = "";
		s.printf("\n\n//Common functions Macro\n//Creates Dup, ClassName,ChildClassName,IsAbstract and IsA() Functions\n\tStdFunctions(%s,%s);", cbParent->GetName(), cbParent->GetInheritType());
		Parent->AppendHead(s.AsPChar());
                Parent->AppendHeadImp("\n\n//Property Access Functions\n");
                Create(TSetProperty,Parent);
                Create(TGetProperty,Parent);
                Parent->AppendHeadImp("\n\n//Class Footer\n");
                Create(TClassFooterFactory,Parent);
            }
            return this;
        }
        if (strcmp(PropertyName,TConstructorStart)==0)
        {
            CryString s;
            s.printf("\t%s();\n",Parent->GetName());
            SetHead(PropertyName,s);
            Parent->AppendHead(*GetHead(PropertyName));
            s.Clear();
            s.printf("\n%s::%s()\n{",Parent->GetName(),Parent->GetName());
            SetImp(PropertyName,s);
            Parent->AppendImp(*GetImp(PropertyName));
            return this;
        }
        if (strcmp(PropertyName,TConstructorEnd)==0)
        {
            CryString s;
            s = "\n}\n\n";
            SetImp(PropertyName,s);
            Parent->AppendImp(*GetImp(PropertyName));
            return this;
        }

        if (strcmp(PropertyName,TDestructorStart)==0)
        {
            CryString s;
            s.printf("\t~%s();\n",Parent->GetName());
            SetHead(PropertyName,s);
            Parent->AppendHead(*GetHead(PropertyName));
            s.Clear();
            s.printf("\n%s::~%s()\n{",Parent->GetName(),Parent->GetName());
            SetImp(PropertyName,s);
            Parent->AppendImp(*GetImp(PropertyName));
            return this;
        }
        if (strcmp(PropertyName,TDestructorEnd)==0)
        {
            CryString s;
            s = "\n}\n\n";
            SetImp(PropertyName,s);
            Parent->AppendImp(*GetImp(PropertyName));
            return this;
        }
    }
    return CodeFactory::Create(PropertyName,Parent);
}
CryObject *ClassHeaderFactory::Create(const CryPropertyParser &PropertyName,CryObject *Parent)
{
	if (PropertyName=="ClassInstance")
	{
	ClassInstance *ci = new ClassInstance(this);
    	AddFactory(ci);
		return ci;
	}
  return CodeFactory::Create(PropertyName,Parent);
}

bool ClassHeaderFactory::Present(const CryObject *Name)// can be either a function def or variable name
{
    CryList::Iterator *i = _CreateIterator();
    CryString *sName = 0;
    CryString Def;
    CryFunctionDef *fdName = 0;
    if (Name->IsA(TCryFunctionDef))
    {
        fdName = (CryFunctionDef *)Name;
        fdName->GetNPDeclaration(Def);
    }
    else
        if (Name->IsA(TCryString))
            sName = (CryString *)Name;
        else
            return false;
    if (i->GotoFirst())
    {
        do
        {
            CodeFactory *cf = (CodeFactory *)i->Get();
            if (fdName && (cf->IsA(TInheritedFactory)))
            {
                CryString lDec;
                InheritedFactory *If = (InheritedFactory *)cf;
                If->GetFunc()->GetNPDeclaration(lDec);
                if (lDec==Def)
                {
                    DeleteIterator(i);
                    return true;
                }
            }
            else
                if (sName)
                {
                    PrimInstance *pi = (PrimInstance *) cf;
                    if (*sName == pi->GetName())
                    {
                        DeleteIterator(i);
                        return true;
                    }
                }
        }
        while(i->GotoNext());
    }
    DeleteIterator(i);
    return false;
}
