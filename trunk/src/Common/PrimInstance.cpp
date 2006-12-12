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


#include "PrimInstance.h"
#include "ClassFunction.h"

using namespace Crystal;
using namespace std;


/*===========================================================================
// PrimInstance
============================================================================*/
PrimInstance::PrimInstance(CodeFactory *Parent) : CodeFactory(Parent,CPrimInstance)
{
	eType = Unknown;
	AddProduct(CCountDefine);
	AddProduct(CPrimInstance);
	AddProduct(CDeclaration);
	AddProduct(CConstructor);
	AddProduct(CDestructor);
	AddProduct(CCopyTo);
}
PrimInstance::PrimInstance(CodeFactory *Parent,const char *PrimType,const char *PrimName,const char *_DefaultValue, int _Count,bool IsProperty,bool IsPointer,bool IsArrayPointer) : CodeFactory(Parent,CPrimInstance)
{
	String Type;
	Type = PrimType;
	SetName(PrimName);
	DefaultValue = _DefaultValue;
	CodeFactory::SetCount(_Count);
	Type.Trim();
	eType = Unknown; // default
	SetIsPointer(IsPointer);
	SetIsArrayPointer(IsArrayPointer);
	SetType(PrimType);
	SetCount();
	SetDefaultValue(_DefaultValue);
	SetIsProperty(IsProperty);
	AddProduct(CCountDefine);
	AddProduct(CPrimInstance);
	AddProduct(CDeclaration);
	AddProduct(CConstructor);
	AddProduct(CDestructor);
	AddProduct(CCopyTo);
	if (IsProperty)
	{
		AddProduct(CGetProperty);
		AddProduct(CSetProperty);
	}
}
void PrimInstance::SetDefaultValue(const char *_DefaultValue)
{
	DefaultValue = _DefaultValue;
	switch(eType)
	{
	case Unknown:
		throw Exception("Cannot set default value of unknown type");
	case Char:
		if ((DefaultValue.Length()!=3) ||((DefaultValue.AsPChar()[0]!='\'') && (DefaultValue.AsPChar()[2]!='\'')))
		{
			char ch = DefaultValue.AsPChar()[0];
			if ((ch=='"')  && DefaultValue.Length()>0)
				ch = DefaultValue.AsPChar()[1];
			DefaultValue.Clear();
			if (ch=='\0')
				DefaultValue = "\'\0\'";
			else
				DefaultValue.printf("'%c'",ch);
		}
		break;
		//    case CharStr:
		//DefaultValue.Insert(0,"\"");
		//DefaultValue += "\"";
		//eak
	case 	Int:
		{
			int i = 0;
			DefaultValue.scanf("%d",&i);
			DefaultValue.Clear();
			DefaultValue.printf("%d",i);
		}
		break;

	case Bool:
		if ((DefaultValue!="true") && (DefaultValue!="false"))
			DefaultValue = "false";
		break;
	case Float:
	case Double:
		{
			double d = 0.0;
			DefaultValue.scanf("%lf",&d);
			DefaultValue.Clear();
			DefaultValue.printf("%lf",d);
		}
		break;
	}
}
void PrimInstance::SetCount()
{
	if (this->GetType()=="char")
	{
		if (GetIsPointer() && GetIsArrayPointer())
			eType = Char;
		else
			if ((GetIsPointer() || GetIsArrayPointer()) && (GetCount()>1))
				eType = CharStr;
			else
				eType = Char;
	}
}
const char *PrimInstance::GetType() const
{
	switch(eType)
	{
	case Char:
		return "char";
	case CharStr:
		return "char";
	case Int:
		return "int";
	case Float:
		return "float";
	case Bool:
		return "bool";
	case Double:
		return "double";
	}
	return "unknown";

}
void PrimInstance::SetType(const char *PrimType)
{
	String Type(PrimType);
	if (Type=="class")
	{
		eType = Class;
	}
	if (Type=="bool")
	{
		eType = Bool;
	}
	if (Type=="char")
	{
		if (GetIsPointer() && GetIsArrayPointer())
			eType = Char;
		else
			if ((GetIsPointer() || GetIsArrayPointer()) && (GetCount()>1))
				eType = CharStr;
			else
				eType = Char;
	}
	if (Type=="int")
		eType = Int;
	if (Type=="float")
		eType = Float;
	if (Type=="double")
		eType = Double;
}
/*! will return a const pointer to the property */
Object *PrimInstance::GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const
{
	return CodeFactory::GetCopyOfPropertyAsObject(PropertyName);
}
/*! will return whether or not the property named in PropertyName is a container */
bool PrimInstance::GetIsPropertyContainer(const PropertyParser &PropertyName) const
{
	return false;
}
/*! will return the property named in PropertyName in a string format */
const char *PrimInstance::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (PropertyName=="Type")
	{
		Result = GetType();
		return Result.AsPChar();
	}
	if (PropertyName=="Count")
	{
		Result.printf("%d",GetCount());
		return Result.AsPChar();
	}
	if (PropertyName=="Default")
	{
		Result = DefaultValue;
		return Result.AsPChar();
	}
	if (PropertyName=="ArrayPointer")
	{
		if (IsArrayPointer)
			Result = "Yes";
		else
			Result = "No";
		return Result.AsPChar();
	}
	return CodeFactory::GetProperty(PropertyName,Result);
}
/*! returns true if the class in question has the property */
bool PrimInstance::HasProperty(const PropertyParser &PropertyName) const
{
	return (PropertyName=="Type") ||
		   (PropertyName=="Default") ||
		   (PropertyName=="Count") ||
		   (PropertyName=="ArrayPointer") ||
		   CodeFactory::HasProperty(PropertyName);
}
/*! The count of the properties a class has */
int PrimInstance::GetPropertyCount() const
{
	return CodeFactory::GetPropertyCount() + 4;
}
/*! Make a list of all property names, the function is called from the parent class through each inheritance until it reaches this class, at which point a list is created and filled with any properties on the way back through the inheritance */
CryPropertyList *PrimInstance::PropertyNames() const
{
	CryPropertyList *Names = CodeFactory::PropertyNames();
	Names->AddPropertyByName("Type",this);
	{	// Don't add "Default" by name since it confuses the children classes (which may not have default values)
	   String *v = new String(DefaultValue);
		Names->AddProperty("Default",v);
	}
	Names->AddPropertyByName("Count",this);
	Names->AddPropertyByName("ArrayPointer",this);
	return Names;
}

bool PrimInstance::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Type")
	{
		SetType(PropertyValue);
		return true;
	}
	if (PropertyName=="Count")
	{
		String a(PropertyValue);
		int i;
		a.scanf("%d",&i);
		if (i<1)
			throw Exception("SetProperty(Count,%d) must be >0",i);
		CodeFactory::SetCount(i);
		SetCount();
		return true;
	}
	if (PropertyName=="Default")
	{
		SetDefaultValue(PropertyValue);
		return true;
	}
	if (PropertyName=="IsArrayPointer")       //IsArrayPointer
	{
		SetIsArrayPointer(strcasecmp(PropertyValue,"Yes")==0);
		return true;
	}
	return CodeFactory::SetProperty(PropertyName,PropertyValue);
}

const char *PrimInstance::Code_AssignDefaultValue(String &Result) const
{
	Result = "";
	if (GetIsPointer())
	{
		if (GetCount()>1)
			if (eType==Char)
				Result.printf("*%s[i] = %s", GetName(), DefaultValue.AsPChar());// default value will be something like 'c'
			else
				Result.printf("*%s[i] = %s", GetName(), DefaultValue.AsPChar());
		else
			Result.printf("*%s = %s",GetName(),DefaultValue.AsPChar());
	}
	else
	{
		if (eType==Char && GetCount()>1)
			Result.printf("\n\tstrncpy(%s,\"%s\",%s_LEN);",GetName(),DefaultValue.AsPChar(),GetName());
		else
			if (GetCount()>1)
			{
				Result.printf("%s[i] = %s",GetName(),DefaultValue.AsPChar());
            }
            else
            {
                Result.printf("%s = %s", GetName(),DefaultValue.AsPChar());
            }
    }
    return Result.AsPChar();
}

const char *PrimInstance::Code_SetProperty(String &Result) const
{
    String pName;
    pName.printf("%s%s%s",this->GetIsPointer() ? "*" : "",GetName(),((eType!=CharStr) && (GetCount()>1)) ? "[Index]" : "");
    switch(eType)
    {
    case Unknown:
        Result.printf("/* Need to assign %s */",pName.AsPChar());
        break;
    case Char:
        Result.printf("%s = *Value;",pName.AsPChar());
        break;
    case CharStr:
        Result.printf("strncpy(%s,Value,%s_LEN);",pName.AsPChar(),GetName());
        break;
    case Int:
        Result.printf("%s = atoi(Value);",pName.AsPChar());
        break;
    case Float:
        Result.printf("%s = atof(Value);",pName.AsPChar());
        break;
    case Bool:
        Result.printf("%s = (strcasecmp(Value,\"true\")==0);",pName.AsPChar());
        break;
    case Double:
        Result.printf("%s = atod(Value);",pName.AsPChar());
        break;
    }
    return Result.AsPChar();
}

const char *PrimInstance::Code_GetProperty(String &Result) const
{
    if (GetCount()>1)
    {
        switch(eType)
        {
        case Unknown:
            Result.printf("/* Need to retrieve %s */",GetName());
            break;
        case Char:
            Result.printf("char Buff[2];\n\tBuff[1] = '\\0';\n\tBuff[0]=%s%s[Index];\n\tResult = Buff;",GetIsPointer() ? "*" : "",GetName());
            break;
        case CharStr:// can't get here
        case Int:
            Result.printf("Result.printf(\"%%d\",%s%s[Index]);",GetIsPointer() ? "*" : "",GetName());
            break;
        case Float:
            Result.printf("Result.printf(\"%%f\",%s%s[Index]);",GetIsPointer() ? "*" : "",GetName());
            break;
        case Bool:
            Result.printf("if(%s%s[Index])\n\t\tResult = \"true\";\n\telse\n\t\tResult = \"false\";",GetIsPointer() ? "*" : "",GetName());
            break;
        case Double:
            Result.printf("Result.printf(\"%%lf\",%s%s[Index]);",GetIsPointer() ? "*" : "",GetName());
            break;
        }
    }
    else
    {
        switch(eType)
        {
        case Unknown:
            Result.printf("/* Need to assign %s */",GetName());
            break;
        case Char:
            Result.printf("char Buff[2];\n\tBuff[1] = '\0';\n\tBuff[0]=%s; Result = Buff;",GetName());
            break;
        case CharStr:
            Result.printf("Result = %s;",GetName());
            break;
        case Int:
            Result.printf("Result.printf(\"%%d\",%s);",GetName());
            break;
        case Float:
            Result.printf("Result.printf(\"%%f\",%s);",GetName());
            break;
        case Bool:
            Result.printf("if(%s)\n\t\tResult = \"true\";\n\telse\n\t\tResult = \"false\";",GetName());
            break;
        case Double:
            Result.printf("Result.printf(\"%%lf\",%s);",GetName());
            break;
        }

    }
    return Result.AsPChar();
}

Object *PrimInstance::Create(const PropertyParser &PropertyName,CodeFactory *Parent)
{
	if ((PropertyName==CPrimInstance) && (!Parent->IsA(CClassBuilder)))
	{
		return this;
	}

	if (PropertyName==CCountDefine)
	{
		if (GetCount()<2)
			return 0;
		String s;
		s.printf("\n#define %s_LEN %d",GetName(),GetCount());
		SetHead(PropertyName,s.AsPChar());
		Parent->AppendHead(*GetHead(PropertyName));
		return 0;
	}
	if (PropertyName==CDeclaration)
	{
		String s;
		if (GetCount()>1)
		{
			//bool * MyBool[MyBool_LEN];
			s.printf("\n//");
			if (GetIsArrayPointer())
				s.printf(" pointer to an");
			if (GetIsPointer())
				s.printf(" array of pointers");
            else
                s.printf(" array");
            s.printf(" of %s",GetType());
            s.printf("\n%s %s%s%s%s[%s_LEN];",
                     GetType(),
                     GetIsArrayPointer() ? "*" : "",
                     GetIsPointer() ? "*" : "",
                     GetName(),
                     GetIsArrayPointer() ? "; //" : "",
                     GetName());
        }
        else
            s.printf("\n%s %s%s;",GetType(),
                     GetIsPointer() ? "*" : "",
                     GetName());
        SetHead(PropertyName,s.AsPChar());
        Parent->AppendHead(*GetHead(PropertyName));
        return 0;
	};
	if (PropertyName==CConstructor)
	{		// we are construction our variables
		String s;
		String Dummy;

		s.Clear();
		if (GetIsPointer())
		{
			String C;
			if (GetCount()>1)
				C.printf("%s[%s_LEN]",GetIsArrayPointer() ? "*" : "",GetName());
			if (GetIsArrayPointer())
				s.printf("\n\t%s = new %s%s;",
						 GetName(),GetType(),
						 C.AsPChar());
			C = "";
			if (GetCount()>1)
				s.printf("\n\tfor(int i=0;i<%s_LEN;i++)\n\t{\n\t\t%s[i] = %s%s;\n\t\t%s;\n\t}",
						 GetName(),//LEN
						 GetName(),
						 GetIsPointer() ? "new " : "",
						 GetType(),
						 Code_AssignDefaultValue(Dummy));
			else
				s.printf("\n\t%s = new %s;\n\t%s;",GetName(),GetType(),Code_AssignDefaultValue(Dummy));
		}
		else
		{
			if (eType==Char && GetCount()>1)
				s.printf("\n\t%s;",Code_AssignDefaultValue(Dummy));
			else
				if (GetCount()>1)
				{
					if (GetIsArrayPointer())
						s.printf("\n\t%s = new %s[%s_LEN];",GetName(),GetType(),GetName());
			Code_AssignDefaultValue(Dummy);

			bool Comment = (Dummy.Pos("/*")!=-1) && (Dummy.Pos("/*")<5);
					s.printf("\n\t%sfor(int i=0;i<%s_LEN;i++)%s\n\t\t%s;",
				Comment ? "/*" : "",
							 GetName(),
				Comment ? "*/" : "",
							 Dummy.AsPChar());
				}
				else
				{
					if (eType==Char && GetCount()>1)
						s.printf("\n\t%s;",Code_AssignDefaultValue(Dummy));
					s.printf("\n\t%s;", Code_AssignDefaultValue(Dummy));
				}
		}
		SetImp(PropertyName,s.AsPChar());
		Parent->AppendImp(*GetImp(PropertyName));
		return 0;
	}
	if (PropertyName==CDestructor)
	{
		String s;
		if (GetIsPointer())
		{
			if (GetCount()>1)
			{
				if (GetIsPointer())
					s.printf("\n\tfor(int i=0;i<%s_LEN;i++)\n\t\tdelete (%s[i]);",
							 GetName(),GetName());
			}
			else
				s.printf("\n\tdelete %s;",GetName());
		}
		if (GetIsArrayPointer())
			s.printf("\n\tdelete %s%s;",GetCount()>1 ? "[]" : "",GetName());
		SetImp(PropertyName,s.AsPChar());
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
	if ((PropertyName==CSetProperty) && CanBuildProduct(PropertyName))//Can't build if it isn't a property
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
			s.printf("\n{\n#ifdef RangeChecking\n\tif ((Index<0) || (Index>=%s_LEN))\n\t\tthrow Exception(\"Range Error\");\n#endif",
					 GetName(),GetName());
			String temp;
			s += "\n\t";
			Code_SetProperty(temp);
			s += temp;
			s += "\n}\n";
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
			s.printf("\n{\n\t");
			String Temp;
			Code_SetProperty(Temp);
			s += Temp;
			s += "\n}\n";
		}
		SetImp(PropertyName,s);
		Parent->AppendHead(*GetHead(PropertyName));
		Parent->AppendImp(*GetImp(PropertyName));
		return 0;
	}
	if ((PropertyName==CGetProperty) && CanBuildProduct(PropertyName))//Can't build if it isn't a property
	{
		String s;
		FunctionDef Func;
        if (GetCount()>1)
        {
            s.printf("const char *Get%s(int Index,CryString &Result) const;",GetName());
            Func.Parse(s);
            s = "\n";
            s += Func.GetNPDeclaration();
            SetHead(PropertyName,s.AsPChar());
            s.Clear();
            s = Func.GetImplementedDeclaration(Parent->GetName(),true);
            s.printf("\n{\n#ifdef RangeChecking\n\tif ((Index<0) || (Index>=%s_LEN))\n\t\tthrow Exception(\"Range Error\");\n#endif",
                     GetName(),GetName());
            s.printf("\n\t//Result.Clear()\n\t");
            String Temp;
            Code_GetProperty(Temp);
            s += Temp;
            s += "\n\treturn Result.AsPChar();\n}\n";
        }
        else
        {
            s.printf("\nconst char *Get%s(CryString &Result) const;",GetName());
            Func.Parse(s);
            s = "\n";
            s += Func.GetNPDeclaration();
            SetHead(PropertyName,s.AsPChar());
            s.Clear();
            s = Func.GetImplementedDeclaration(Parent->GetName(),true);
            s.printf("\n{\n\t");
            String Temp;
            Code_GetProperty(Temp);
            s += Temp;
            s += "\n\treturn Result.AsPChar();\n}\n";
        }
        SetImp(PropertyName,s);
        Parent->AppendHead(*GetHead(PropertyName));
        Parent->AppendImp(*GetImp(PropertyName));
        return 0;
	}
	//    if (strcmp("VarName",PropertyName)==0)
	//{
	//  return &GetName();
	//}
	return 0;
};

void PrimInstance::SetIsArrayPointer(bool _IsArrayPointer)
{
	IsArrayPointer = _IsArrayPointer;
}
bool PrimInstance::GetIsArrayPointer() const
{
	return IsArrayPointer;
}
