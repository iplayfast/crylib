/***************************************************************************
 *   Copyright (C) 2006 by Chris Bruner                                    *
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




#include <stdarg.h>

#include "ClassObject.h"
#include "ClassBuilder.h"
#include "ClassXML.h"
#include "CryBackProp.h"

#include "ClassInstance.h"
#include "ClassFunction.h"
#include "ClassFileStream.h"
//#define DIAGNOSE

using namespace Crystal;
using namespace std;

/*===========================================================================
// ClassInstance
============================================================================*/


/*ClassInstance::ClassInstance(CodeFactory *Parent,const char *ClassType,const char *ClassName,int _Count,bool _IsProperty,bool _IsPointer,bool _IsArrayPointer,const char *DefaultValue) :
		PrimInstance(Parent,"class",ClassName,DefaultValue, _Count,_IsProperty,_IsPointer,_IsArrayPointer)
{
	p = Object::Create(ClassType,0);
	Type = ClassType;
} */

ClassInstance::~ClassInstance()
{
    delete p;
}

bool ClassInstance::HasProperty(const PropertyParser &PropertyName) const
{
	if (PropertyName=="HasDefault")
		return true;
	if (PropertyName=="Default")
		return HasDefault;
	return PrimInstance::HasProperty(PropertyName);
}

PropertyList *ClassInstance::PropertyNames() const
{
	PropertyList *Names = PrimInstance::PropertyNames();
	Names->AddPropertyByName("HasDefault",this);
	if (!HasDefault)
	{
			Names->RemoveNodeValue(String("Default"));
	}
	return Names;
}
	/*! will return the property named in PropertyName in a string format */
const char *ClassInstance::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (PropertyName=="HasDefault")
	{
		Result = HasDefault ? "Yes" : "No";
		return Result.AsPChar();
	}
	if (PropertyName=="Default")
	{
		if(!HasDefault)
			return PrimInstance::GetProperty(PropertyName,Result);
//			throw CryException("Default Requested, when none was set");
		XML x("obj");
		x.LoadFrom(*p);
		x.SaveTo(Result);
		return Result.AsPChar();
	}
	return PrimInstance::GetProperty(PropertyName,Result);
}

	/*! The count of the properties a class has */
int ClassInstance::GetPropertyCount() const
{
	return 2 + PrimInstance::GetPropertyCount();
}


bool ClassInstance::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="HasDefault")
	{
		HasDefault = strcasecmp(PropertyValue,"Yes")==0;
		return true;
	}
	if (PropertyName=="Type")
	{
		p = Object::Create(PropertyValue,0);
		Type = PropertyValue;
		PrimInstance::SetType("class");
		return true;
	}
	if (PropertyName=="Default")
	{	// if the class couldn't use the default just let it go.
		XML x("obj");
		String  s(PropertyValue);
		x.LoadFrom(s);
		x.SaveTo(*p);
		HasDefault = true;
		return true;
	}
	return PrimInstance::SetProperty(PropertyName,PropertyValue);
}
Object *ClassInstance::Create(const PropertyParser &PropertyName,CodeFactory *Parent)
{
	return PrimInstance::Create(PropertyName,Parent);
/*	if ((strcmp(CClassInstance,PropertyName)==0) && (!Parent->IsA(CClassBuilder)))
	{
		return this;
	}
	if (strcmp(CCountDefine,PropertyName)==0)
	{
		if (GetCount()<2)
			return 0;
		String s;
		s.printf("\n#define %s_LEN %d",GetName(),GetCount());
		SetHead(PropertyName,s.AsPChar());
		Parent->AppendHead(*GetHead(PropertyName));
		return 0;
	}
	if (strcmp(CDeclaration,PropertyName)==0)
	{
		String s;
		if (GetCount()>1)
		{
			//Create(TCountDefine,Parent);
			s.printf("\n%s *%s[%s_LEN];", Type.AsPChar(),GetName(),GetName());
		}
		else
			s.printf("\n%s %s%s;",Type.AsPChar(),GetIsPointer() ? "*" : "",GetName());
		SetHead(PropertyName,s.AsPChar());
		Parent->AppendHead(*GetHead(PropertyName));
		return 0;
	};
	if (strcmp(CConstructor,PropertyName)==0)
	{		// we are construction our variables
		String s;
		s.Clear();
		String C;
		if (GetCount()>1)
		{
			s.printf("\n\t%s = new %s*[%s_LEN];",GetName(),GetType(),GetName());
			s.printf("\n\tfor(int i=0;i<%s_LEN;i++)\n\t\t%s[i] = new %s();",GetName(),GetName(),GetType());
		}
		else
			if (GetIsPointer())
				s.printf("\n%s = new %s%s;",GetName(),Type.AsPChar(),GetName());
		SetImp(PropertyName,s.AsPChar());
		Parent->AppendImp(*GetImp(PropertyName));
		return 0;
	}
	if (strcmp(CDestructor,PropertyName)==0)
	{
		String s;
		if (GetCount()>1)
			s.printf("\n\tfor(int i=0;i<%s_LEN;i++)\n\t\tdelete %s[i];",GetName(),GetName());
		if (GetCount()>1 || GetIsPointer())
			s.printf("\n\tdelete %s%s;",GetCount()>1 ? "[]" : "",GetName());
		SetImp(PropertyName,s.AsPChar());
		Parent->AppendImp(*GetImp(PropertyName));
		return 0;
	}
	if ((strcmp(CSetProperty,PropertyName)==0) && CanBuildProduct(PropertyName))//Can't build if it isn't a property
	{
		String s;
		FunctionDef Func;
		if (GetCount()>1)
		{
			s.printf("void Set%s(int Index,const char *Value);",GetName());
			Func.Parse(s);
			s = Func.GetNPDeclaration();
			SetHead(PropertyName,s.AsPChar());
			s.Clear();
			s = Func.GetImplementedDeclaration(Parent->GetName(),true);
			s.printf("\n{\n#ifdef RangeChecking\n\tif ((Index<0) || (Index>=%s_LEN))\n\t\tthrow CryException(\"Range Error\");\n#endif",
					 GetName(),GetName());
			if (Type==CString)
			{
				s.printf("\n\t%s[Index] = Value;",GetName());
			}
			else
			{
				s.printf("\nCryString a(Value);");
				s.printf("\nCryXML x(\"obj\");");
				s.printf("\n\n\tx.LoadFrom(a);");
				s.printf("\n\tx.SaveTo(%s[Index]);",GetName());
			}
			s.printf("\n}\n");

		}
		else
		{
			s.printf("void Set%s(const char *Value);",GetName());
			Func.Parse(s);
			s = "\n";
			s += Func.GetNPDeclaration();
			SetHead(PropertyName,s.AsPChar());
			s.Clear();
			s = Func.GetImplementedDeclaration(Parent->GetName(),true);
			s.printf("\n{");
			if (Type==CString)
			{
				s.printf("\n\t%s = Value;",GetName());
			}
			else
			{
				s.printf("\nCryString a(Value);");
				s.printf("\nCryXML x(\"obj\");");
				s.printf("\n\n\tx.LoadFrom(a);");
				s.printf("\n\tx.SaveTo(%s);",GetName());
			}
			s.printf("\n}\n");
		}
		SetImp(PropertyName,s);
		Parent->AppendHead(*GetHead(PropertyName));
		Parent->AppendImp(*GetImp(PropertyName));
		return 0;
	}
	if (PropertyName==CCopyTo)
	{
		String s;
		if (GetIsPointer())
		{
			if (GetCount()>1)
				s.printf("\n\t\tfor(int i=0;i<%s_LEN;i++)\n\t\t\t*CastDest->%s[i] = *%s[i];",
						 GetName(),GetName(),GetName());
			else
				s.printf("\n\t\t%sCastDest%s%s = %s%s%s;",
						 (GetIsPointer() || GetIsArrayPointer()) ? "*" : "",
						 GetIsPointer() ? "->" : ".",GetName(),GetIsArrayPointer() ? "*" : "",GetIsPointer() ? "*" : "", GetName());
		}
		else
		{
			if (GetCount()>1)
				s.printf("\n\t\tfor(int i=0;i<%s_LEN;i++)\n\t\t\tCastDest->%s[i] = %s[i];",
						 GetName(),GetName(),GetName());
			else
				s.printf("\n\t\tCastDest->%s = %s;",GetName(),GetName());
		}
		SetImp(PropertyName,s.AsPChar());
		Parent->AppendImp(*GetImp(PropertyName));
		return 0;
	}


	if ((strcmp(CGetProperty,PropertyName)==0) && CanBuildProduct(PropertyName))//Can't build if it isn't a property
	{
		String s;
		FunctionDef Func;
		if (GetCount()>1)
		{
			s.printf("void Get%s(int Index,CryString &Result) const;",GetName());
			Func.Parse(s);
			s = "\n";
			s += Func.GetNPDeclaration();
			SetHead(PropertyName,s.AsPChar());
			s.Clear();
			s = Func.GetImplementedDeclaration(Parent->GetName(),true);
			s.printf("\n{\n#ifdef RangeChecking\n\tif ((Index<0) || (Index>=%s_LEN))\n\t\tthrow CryException(\"Range Error\");\n#endif",
					 GetName(),GetName());
			if (Type==CString)
			{
				s.printf("\n\tResult = %s[Index];",GetName());
			}
			else
			{
				s.printf("\n\t//Result.Clear()");
				s.printf("\nCryXML x(\"obj\");");
				s.printf("\n\tx.LoadFrom(%s[Index]);",GetName());
				s.printf("\n\tx.SaveTo(Result);");
			}
			s.printf("\n}");

        }

        else
        {
            s.printf("\nvoid Get%s(CryString &Result) const;",GetName());
            Func.Parse(s);
            s = "\n";
            s += Func.GetNPDeclaration();
            SetHead(PropertyName,s.AsPChar());
            s.Clear();
            s = Func.GetImplementedDeclaration(Parent->GetName(),true);
            s.printf("\n{");
			if (Type==CString)
			{
				s.printf("\n\tResult = %s;",GetName());
            }
            else
            {
                s.printf("\n\t//Result.Clear()");
                s.printf("\nCryXML x(\"obj\");");
                s.printf("\n\tx.LoadFrom(%s);",GetName());
                s.printf("\n\tx.SaveTo(Result);");
            }
            s.printf("\n}");

        }
        SetImp(PropertyName,s);
        Parent->AppendHead(*GetHead(PropertyName));
        Parent->AppendImp(*GetImp(PropertyName));
        return 0;
	}
	//if (strcmp("VarName",PropertyName)==0)
	//{
	//return &GetName();
	//}
	return 0;
*/	
}




const char *ClassInstance::Code_AssignDefaultValue(String &Result) const
{
	String Default;
	if (HasDefault)
		GetProperty("Default",Default);
	Result = "";
	if (p->IsA(CString))
		Result.printf(" = \"%s\"",Default.AsPChar());
    else
	if (p->IsA(CProperty))
	{
		if (GetCount()>1)
			Result.printf(".SetNameValue(CryString(\"%s_%%d\",i),\"%s\")",GetName(),Default.AsPChar());
		else	
			Result.printf(".SetNameValue(\"%s\",\"%s\")",GetName(),Default.AsPChar());
	}
	else
        Result.printf("// Add Default values here \n\t");
	
    Default = Result;
	
    Result = "";
    if (p->IsContainer() && !p->IsA(CString))
	Result.printf("// %s Container needs your personal attention for default values\n//",GetType());
    if (GetIsPointer())
    {
        if (GetCount()>1)
            Result.printf("*%s[i]%s", GetName(), Default.AsPChar());
        else
            Result.printf("*%s%s",GetName(),Default.AsPChar());
    }
    else
    {
        if (GetCount()>1)
        {
            Result.printf("%s[i]%s",GetName(),Default.AsPChar());
        }
        else
        {
            Result.printf("%s%s", GetName(),Default.AsPChar());
		
        }
    }
    return Result.AsPChar();
}


const char *ClassInstance::GetType() const
{
    return Type.AsPChar();
}
/*bool ClassInstance::GetIsProperty() const
{
    return IsProperty;
}
void ClassInstance::SetIsArrayPointer(bool _IsArrayPointer)
{
    IsArrayPointer = _IsArrayPointer;
}
bool ClassInstance::GetIsArrayPointer() const
{
    return IsArrayPointer;
}
*/


const char *ClassInstance::Code_SetProperty(String &Result) const
{
    String pName;
String ExtraCode;
	if (!p->IsA(CString))
		ExtraCode = ".SetValue(Value)";
	else	ExtraCode= "= Value";

bool Comment = false; //! p->IsA(CString);
    pName.printf("%s%s%s%s",
	Comment ? "/*" : "",
	this->GetIsPointer() ? "*" : "",GetName(),GetCount()>1 ? "[Index]" : "");
    Result.printf("%s%s;%s",pName.AsPChar(),ExtraCode.AsPChar(),Comment ? "*/" : "");
    return Result.AsPChar();
}
const char *ClassInstance::Code_GetProperty(String &Result) const
{
    String pName;
	String ExtraCode;
bool Comment = false;//! p->IsA(CString);
	if (!p->IsA(CString))
		ExtraCode = ".GetValue()";
	else	ExtraCode= "";
    pName.printf("%s%s%s%s",
	Comment ? "/*" : "",
	this->GetIsPointer() ? "*" : "",GetName(),GetCount()>1 ? "[Index]" : "");
    Result.printf("Result = %s%s;%s",pName.AsPChar(),ExtraCode.AsPChar(),Comment ? "*/" : "");
    return Result.AsPChar();
}
