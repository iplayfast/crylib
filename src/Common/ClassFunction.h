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

#include "ClassString.h"
#include "ClassList.h"

namespace Crystal
{
using namespace std;
#ifndef CFunctionDef
#define CFunctionDef "FunctionDef"
#define CFunctionDefList "FunctionDefList"

/*! contains all the info to describe a function definition, used when autobuilding classes
*/
class FunctionDef : public String
{
    bool _NeedsSpace() const;
    const String &_GetDeclaration(String &Result,bool IsPure) const;
public:
    StdFunctions(FunctionDef,String);
    // The full function name eg, virtual size_t CryProperty::Size() const;
    bool IsVirtual;
    String ReturnType;	// eg "size_t", or "const char *"
    String FunctionName; // eg "Size()
    String Body;         // { ... code ... } stuff
    bool IsConst;		// const function
    bool IsPure;		// =0
    bool IsComment; // not a function def at all, just a comment about one!
    FunctionDef();
	FunctionDef(const char *Name);
	~FunctionDef();
    void Parse(const char *Name);
    FunctionDef(const char *_ReturnType, const char *_FunctionName,bool _IsConst = false,bool _IsVirtual = false,bool _IsPure = false);
    virtual int CompareLogical(int CompareType,const Object *Test) const;
    virtual bool LessThen(int CompareType,const Object *Test)const;
	virtual bool GreaterThen(int CompareType,const Object *Test)const;
	virtual bool EqualTo(int CompareType,const Object *Test)const;
    virtual int GetPropertyCount() const;
	virtual bool HasProperty(const PropertyParser &PropertyName) const;
	virtual CryPropertyList* PropertyNames() const;
	virtual const char * GetProperty(const PropertyParser &PropertyName,String &Result) const;
    virtual bool  SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);
    const char *GetFunctionName() const;
    const String &GetDeclaration(String &Result) const; // eg what goes in the header file
    const String &GetImplementedDeclaration(const char *ClassName,String &Result,
            bool ShowReturnType) const; // eg what goes in the body
    const String &GetNPDeclaration(String &Result) const; // same as GetDeclaration but without = 0 in the case of pure virtuals
    const String &GetDeclaration(); // eg what goes in the header file
    const String &GetImplementedDeclaration(const char *ClassName,bool ShowReturnType); // eg what goes in the body
    const String &GetNPDeclaration(); // same as GetDeclaration but without = 0 in the case of pure virtuals
}
;  //CryFunctionDef

class FunctionDefList : public List
{
public:
    StdFunctions(FunctionDefList,List);
    void LoadFromString(const String &Source,const char *Separator);
};

#endif	// TCryFunctionDef
}
