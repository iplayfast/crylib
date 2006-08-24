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

#include <ctype.h>

#include "ClassFunction.h"
#include "ClassException.h"
#include "ClassString.h"
#include "ClassProperty.h"

using namespace Crystal;

///----------------------------------------------------------------------------
// CryFunctionDef
///----------------------------------------------------------------------------

int CryFunctionDef::CompareLogical(int CompareType,const CryObject *Test) const
{
    if (Test->IsA(TCryFunctionDef))
    {
        CryFunctionDef *f = (CryFunctionDef *)Test;
        if ((CompareType>4) || (CompareType<0))
            throw CryException("Unexpected Value for Sorting CryFunctionDef");
        switch(CompareType)
        {
        case 0:
            {
                CryString a,b;
                return GetDeclaration(a).strcmp(f->GetDeclaration(b));
            }
        case 1:
            return FunctionName.strcmp(f->FunctionName);
        case 2:
            return ReturnType.strcmp(f->ReturnType);
        case 3:
            {
                int c;
                if (IsConst)
                    c = 1;
                else
                    c = 0;
                int c1;
                if (f->IsConst)
                    c1 = 1;
                else
                    c1 =0;
                return c - c1;
            }
        case 4:
            {
                int c;
                if (IsPure)
                    c = 1;
                else
                    c = 0;
                int c1;
                if (f->IsPure)
                    c1 = 1;
                else
                    c1 =0;
                return c - c1;
            }
        }
    }
    return 0;
}

CryFunctionDef::~CryFunctionDef()
{
	//
}

bool CryFunctionDef::LessThen(int CompareType,const CryObject *Test) const
{
	int c = CompareLogical(CompareType,Test);
	return (c<0);
}
bool CryFunctionDef::GreaterThen(int CompareType,const CryObject *Test) const
{
	int c = CompareLogical(CompareType,Test);
	return (c>0);
}

bool CryFunctionDef::EqualTo(int CompareType,const CryObject *Test) const
{
    int c = CompareLogical(CompareType,Test);
    return (c==0);
}


CryFunctionDef::CryFunctionDef()
{}
CryFunctionDef::CryFunctionDef(const char *Name)
{
    Body="";
    Parse(Name);
}	// Name get's parsed and put into categories

CryFunctionDef::CryFunctionDef(const char *_ReturnType, const char *_FunctionName,bool _IsConst,bool _IsVirtual,bool _IsPure)
{
    IsVirtual = _IsVirtual;
    ReturnType = _ReturnType;
    FunctionName = _FunctionName;
    IsConst = _IsConst;
    IsPure = _IsPure;
    Body = "";
}

const char *CryFunctionDef::GetFunctionName() const
{
    return FunctionName.AsPChar();
};

const CryString &CryFunctionDef::GetDeclaration(CryString &Result) const // eg what goes in the header file
{
    return _GetDeclaration(Result,IsPure);
}
const CryString &CryFunctionDef::_GetDeclaration(CryString &Result,bool IsPure) const
{
    bool NeedsSpace = _NeedsSpace();
    Result.Clear();
    Result.printf("%s%s%s%s%s%s;",
                  IsVirtual ? "virtual " : "",
                  ReturnType.AsPChar(),
                  NeedsSpace ? " " : "",
                  FunctionName.AsPChar(),
                  IsConst ? " const" : "",
                  IsPure ? " = 0" : "");
    Result.Trim();
    Result.Replace("  "," "); // get rid of double spaces
    return Result;
}

const CryString &CryFunctionDef::GetImplementedDeclaration(const char *ClassName,
        bool ShowReturnType)  // eg what goes in the body
{
    return GetImplementedDeclaration(ClassName,*this,ShowReturnType);
}


const CryString &CryFunctionDef::GetImplementedDeclaration(const char *ClassName,CryString &Result,bool ShowReturnType) const // eg what goes in the body
{
    bool NeedsSpace = _NeedsSpace();
    if (!ShowReturnType)
        NeedsSpace =false;
    Result.Clear();
    Result.printf("%s%s%s::%s%s",
                  ShowReturnType ? ReturnType.AsPChar() : "",
                  NeedsSpace ? " " : "",
                  ClassName,FunctionName.AsPChar(), IsConst ? " const" : "");
    return Result;
}

const CryString &CryFunctionDef::GetNPDeclaration(CryString &Result) const // same as GetDeclaration but without = 0 in the case of pure virtuals
{
    return _GetDeclaration(Result,false);
}

const CryString &CryFunctionDef::GetDeclaration() // eg what goes in the header file
{
    return GetDeclaration(*this);
}

const CryString &CryFunctionDef::GetNPDeclaration() // same as GetDeclaration but without = 0 in the case of pure virtuals
{
    return GetNPDeclaration(*this);
}

bool CryFunctionDef::_NeedsSpace() const
{
    return ReturnType.Pos("*")!=(int) ReturnType.Length()-1;
};

int CryFunctionDef::GetPropertyCount() const
{
    return 7;
}

bool CryFunctionDef::HasProperty(const CryPropertyParser &PropertyName) const
{
    return
        ("IsVirtual"==PropertyName) ||
        ("ReturnType"==PropertyName) ||
        ("FunctionName"==PropertyName) ||
        ("Body"==PropertyName) ||
        ("IsConst"==PropertyName) ||
        ("IsPure"==PropertyName) ||
        ("IsComment"==PropertyName);
}

CryPropertyList *CryFunctionDef::PropertyNames() const
{
	CryPropertyList *n = new CryPropertyList();
	n->AddPropertyByName("IsVirtual",this);
	n->AddPropertyByName("ReturnType",this);
	n->AddPropertyByName("FunctionName",this);
	n->AddPropertyByName("Body",this);
	n->AddPropertyByName("IsConst",this);
	n->AddPropertyByName("IsPure",this);
	n->AddPropertyByName("IsComment",this);
	return n;
}

const char * CryFunctionDef::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
    if (PropertyName=="IsVirtual")
    {
        Result.printf(IsVirtual ? "T" : "F");
        return Result.AsPChar();
    }
    if (PropertyName=="ReturnType")
    {
        ReturnType.SaveTo(Result);
        return Result.AsPChar();
    }
    if (PropertyName=="FunctionName")
    {
        FunctionName.SaveTo(Result);
        return Result.AsPChar();
    }
    if (PropertyName=="Body")
    {
        Body.SaveTo(Result);
        return Result.AsPChar();
    }
    if (PropertyName=="IsConst")
    {
        Result.printf(IsConst ? "T" : "F");
        return Result.AsPChar();
    }
    if (PropertyName=="IsPure")
    {
        Result.printf(IsPure ? "T" : "F");
        return Result.AsPChar();
    }
    if (PropertyName=="IsComment")
    {
        Result.printf(IsComment ? "T" : "F");
        return Result.AsPChar();
    }
    throw CryException(this,ExceptionUnknownProperty,"Unknown Property \"%s\"",PropertyName.AsPChar());
}

bool  CryFunctionDef::SetProperty(const CryPropertyParser &PropertyName,const char *_PropertyValue)
{
    CryString PropertyValue;
    PropertyValue = _PropertyValue;
    if (PropertyName=="IsVirtual")
    {
        IsVirtual = PropertyValue=="T";
        return true;
    }
    if (PropertyName=="ReturnType")
    {
        ReturnType.LoadFrom(PropertyValue);
        return true;
    }
    if (PropertyName=="FunctionName")
    {
        FunctionName.LoadFrom(PropertyValue);
        return true;
    }
    if (PropertyName=="Body")
    {
        Body.LoadFrom(PropertyValue);
        return true;
    }
    if (PropertyName=="IsConst")
    {
        IsConst = PropertyValue=="T";
        return true;
    }
    if (PropertyName=="IsPure")
    {
        IsPure = PropertyValue=="T";
        return true;
    }
    if (PropertyName=="IsComment")
    {
        IsComment = PropertyValue=="T";
        return true;
    }
    return false;
}



void CryFunctionDef::Parse(const char *Name)
{
    CryString s;
	s = Name;
	int p = s.Pos(";");
	if (p>-1)
		s.Delete(p,1);
	s.Trim();
	IsComment = (s.Pos("//")==0);
	if (IsComment)
	{
		IsPure = true;  // usually more useful then not
		IsConst = IsVirtual = false;
		FunctionName = s;
		return;
	}

	int lastbracket = s.Pos(")");
	if (s.Pos("= 0")>lastbracket)
	{
		s.Delete(s.Pos("= 0"),s.GetLength());
		IsPure = true;
	}
	else
		IsPure = false;
	if (s.Pos("const",lastbracket)>lastbracket)
	{
		IsConst = true;
		s.Delete(lastbracket + 1,s.GetLength());
	}
	else
		IsConst = false;
	if (s.Pos("virtual ")==0)
	{
		s.Delete(0,8);
		IsVirtual = true;
	}
	else
		IsVirtual = false;
	int i = s.Pos("(") - 1;
	if (i<0)
		i = 0;
	const char *o,*c = s;
    o = c;
    c += i;
    while(c>o)
    {
        if (!isalnum(*c))
        {
            c++;
            break;
        }
        c--;
    }
    i = c - o;
    FunctionName = c;
    FunctionName.Trim();
    ReturnType = s;
    ReturnType.Delete(i,s.GetLength());
    ReturnType.Trim();
}

///----------------------------------------------------------------------------
// CryFunctionDefList
///----------------------------------------------------------------------------
void CryFunctionDefList::LoadFromString(const CryString &Source,const char *Separator)
{
    CryList *l = Source.ListFromString(Separator);
    ListIterator *li = (CryList::ListIterator *) l->CreateIterator();

    if (li->GotoFirst())
    {
        do
        {
            CryString *s = (CryString *)li->Get();
            CryFunctionDef *f = new CryFunctionDef(s->AsPChar());
            {
                ListIterator *li = (CryList::ListIterator *) CreateIterator();
                if (li->GotoFirst())
                {
                    do
                    {
                        CryFunctionDef *o = (CryFunctionDef *)li->Get();
                        if ((o->FunctionName == f->FunctionName) && (o->ReturnType==f->ReturnType))
                        {
                            RemoveAtIterator(li);
                        }
                    }
                    while(li->GotoNext());
                }
                DeleteIterator(li);
            }
            AddOwned(f);
        }
        while(li->GotoNext());
    }
    l->DeleteIterator(li);
    delete l;
}

