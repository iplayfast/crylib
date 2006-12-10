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


ClassHeaderFactory::ClassHeaderFactory(CodeFactory *Parent) : CodeFactory(Parent,CClassHeaderFactory)
{
	SetSortValue(0X0001000);
	AddProduct(CClassHeaderFactory);
	AddProduct(CConstructorStart);
	AddProduct(CConstructorEnd);
	AddProduct(CDestructorStart);
	AddProduct(CDestructorEnd);
	AddFactory(new ClassFooterFactory(this));
}

Object *ClassHeaderFactory::Create(const CryPropertyParser &PropertyName,CodeFactory *Parent)
{
	if (!Parent->IsA(CClassBuilder))
	{
		if (strcmp(PropertyName,CInheritedFactory)==0)
		{
            CryFunctionDef dummy("void foo()");
            InheritedFactory *i = new InheritedFactory(this,dummy);
            AddFactory(i);
            return i;
        }
		if (strcmp(PropertyName,CPrimInstance)==0)
		{
			PrimInstance *pi = new PrimInstance(this);
			AddFactory(pi);
			return pi;
		}
	}
	else
	{
		if (strcmp(PropertyName,CClassHeaderFactory)==0)
		{
			if (Parent->IsA(CClassBuilder))
			{
				ClassBuilder *cbParent = (ClassBuilder *)Parent;
				CryString s;
				s.printf("namespace Crystal {\nusing namespace Crystal;\n\nclass %s : public %s\n{\n",
						 cbParent->GetName(),cbParent->GetInheritType());
				SetHead(PropertyName,s);
				Parent->AppendHead(*GetHead(PropertyName));
				Parent->AppendHeadImp("\n\n//Array Length defines");
				Create(CCountDefine,Parent);
				Parent->AppendHeadImp("\n\n//Local Variables Declaration");
				Create(CDeclaration,Parent);
		Parent->AppendHead("\n\npublic:");
				Parent->AppendHeadImp("\n\n//Constructor/Destructor Functions");
				Create(CConstructorStart,Parent);
				Create(CConstructor,Parent);
				Create(CConstructorEnd,Parent);
				Create(CDestructorStart,Parent);
				Create(CDestructor,Parent);
				Create(CDestructorEnd,Parent);
				Parent->AppendHeadImp("\n\n//Inherited Functions");
				Create(CInheritedFactory,Parent);
		s = "";
		s.printf("\n\n//Common functions Macro\n//Creates Dup, ClassName,ChildClassName,IsAbstract and IsA() Functions\n\tStdFunctions(%s,%s);", cbParent->GetName(), cbParent->GetInheritType());
		Parent->AppendHead(s.AsPChar());
				Parent->AppendHeadImp("\n\n//Property Access Functions");
				Create(CSetProperty,Parent);
				Create(CGetProperty,Parent);
				Parent->AppendHeadImp("\n\n//Class Footer");
				Create(CClassFooterFactory,Parent);
			}
			return this;
		}
		if (strcmp(PropertyName,CConstructorStart)==0)
		{
			CryString s;
			s.printf("\t%s();",Parent->GetName());   // eg. Myclass();
			SetHead(PropertyName,s);                 // put it in the header
			Parent->AppendHead(*GetHead(PropertyName));
			s.Clear();
			s.printf("\n%s::%s()\n{",Parent->GetName(),Parent->GetName());  //eg Myclass::Myclass()
			SetImp(PropertyName,s);
			Parent->AppendImp(*GetImp(PropertyName));
			return this;
		}
		if (strcmp(PropertyName,CConstructorEnd)==0)
		{
			CryString s;
			s = "\n}\n\n";
			SetImp(PropertyName,s);
			Parent->AppendImp(*GetImp(PropertyName));
			return this;
		}

		if (strcmp(PropertyName,CDestructorStart)==0)
		{
			CryString s;
			s.printf("\t~%s();",Parent->GetName());
			SetHead(PropertyName,s);
			Parent->AppendHead(*GetHead(PropertyName));
			s.Clear();
			s.printf("\n%s::~%s()\n{",Parent->GetName(),Parent->GetName());
			SetImp(PropertyName,s);
			Parent->AppendImp(*GetImp(PropertyName));
			return this;
		}
		if (strcmp(PropertyName,CDestructorEnd)==0)
		{
			CryString s;
			s = "\n}\n";
			SetImp(PropertyName,s);
			Parent->AppendImp(*GetImp(PropertyName));
			return this;
		}
	}
	return CodeFactory::Create(PropertyName,Parent);
}
Object *ClassHeaderFactory::Create(const CryPropertyParser &PropertyName,Object *Parent)
{
	if (PropertyName=="ClassInstance")
	{
	ClassInstance *ci = new ClassInstance(this);
		AddFactory(ci);
		return ci;
	}
  return CodeFactory::Create(PropertyName,Parent);
}

bool ClassHeaderFactory::Present(const Object *Name)// can be either a function def or variable name
{
	List::Iterator *i = _CreateIterator();
	CryString *sName = 0;
	CryString Def;
	CryFunctionDef *fdName = 0;
	if (Name->IsA(CCryFunctionDef))
	{
		fdName = (CryFunctionDef *)Name;
		fdName->GetNPDeclaration(Def);
	}
	else
		if (Name->IsA(CCryString))
			sName = (CryString *)Name;
		else
			return false;
	if (i->GotoFirst())
	{
		do
		{
			CodeFactory *cf = (CodeFactory *)i->Get();
			if (fdName && (cf->IsA(CInheritedFactory)))
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
