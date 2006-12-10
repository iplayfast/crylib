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
#include "CodeFactory.h"
#include "ClassProperty.h"
#ifndef CClassBuilder

namespace Crystal
{
#define CClassBuilder "ClassBuilder"

#define CCountDefine "CountDefine"
#define CDeclaration	"Declaration"
#define CConstructor	"Constructor"
#define CDestructor	"Destructor"
#define CConstructorStart	"ConstructorStart"
#define CConstructorEnd	"ConstructorEnd"
#define CDestructorStart	"DestructorStart"
#define CDestructorEnd	"DestructorEnd"




/*! CodeFactory has a local version of the code that would get generated. This
version can be edited or changed after the initial Create of it is done
*/
#define CCodeFactory "CodeFactory"



class ClassBuilder : public CodeFactory
{
	Object *p;
    bool _IsAbstract;	// Is this needed, since ClassBuilder only creates non-abstract classes?
    CryString InheritType;
    CryString Filename;
    void SetDefaultBodies(CryFunctionDefList *AbstractFunctions); // will setup default bodies of functions
    void AssignInheritedElements();
    //CryString ClassName;
public:
StdFunctionsNoDup(ClassBuilder,CodeFactory);
    ClassBuilder();
    ~ClassBuilder();
    const Object *Getp() const;
    void AppendHBOutput(const char *s);
    void Build();
    void SetBaseClass(const char *Type,bool AddStubs,bool AddVirtuals,const char *ClassName);
    void AddClassInstance(const char *ClassType,const char *ClassName,int count,bool IsProperty,bool IsPointer,bool IsArrayPointer,bool HasDefault,const char *DefaultValue);
    void AddPrimInstance(const char *PrimType,const char *PrimName,const char *DefaultValue, int Count,bool IsProperty,bool IsPointer,bool IsArrayPointer);
    void Remove(const char *Description);
	void SetFilename(const char *name);
	const CryString &GetFilename() const;
	//const char *GetName();
	//void SetName(const char *NewClassName);
	const char *GetInheritType();
	virtual void SaveHeaderBody(CryStream &ToStreamHeader,CryStream &ToStreamBody);
	virtual void SaveSource();	// will save the source under Filename.h and Filename.cpp and Filename.xml
	virtual void LoadSource();
	virtual Object *Create(const CryPropertyParser &PropertyName,Object *Parent);
	virtual Object *Create(const CryPropertyParser &PropertyName,CodeFactory *Parent);
	virtual Object *Create(const char *FactoryName,const CryPropertyParser &PropertyName,Object *Parent=0)
	{
	  return CryFactory::Create(FactoryName,PropertyName,Parent);
	}

	virtual Object *Create(CryStream &FromStream);

	int GetPropertyCount() const;
	/// will return a pointer to a dup of the property
	virtual Object *GetCopyOfPropertyAsObject(const CryPropertyParser &a) const;
	/// will return a pointer to the property if the property is a CryObject or desendent
	virtual Object *_GetPropertyAsObject(const CryPropertyParser &a) const;
	virtual bool  SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);

	virtual bool GetIsPropertyContainer(const CryPropertyParser &PropertyName) const;
	virtual const char * GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
	virtual CryPropertyList* PropertyNames() const;
	virtual bool HasProperty(const CryPropertyParser &PropertyName) const;

};

#define CContainerFactory "ContainerFactory"

class ContainerFactory : public CodeFactory
{
	CryString ElementType;
public:
	ContainerFactory(CodeFactory *Parent,Container *c);
	virtual const char *ChildClassName() const
	{
		return CContainerFactory;
	}
};


#define CPropertyFactory "PropertyFactory"
class PropertyFactory : public CodeFactory
{
    CryString Value;
public:
    PropertyFactory(CodeFactory *Parent,const CryString *n,const CryString *v);
    virtual bool IsA(const char *ClassName) const;    // can the object map to a ClassName
    const char *GetPropertyName();
    const CryString *GetPropertyValue();
	virtual const char *ChildClassName() const
    {
		return CPropertyFactory;
    }
};

#define CHeaderFactory  "HeaderFactory"
class HeaderFactory : public CodeFactory
{
    HeaderFactory() :CodeFactory(0,"")
    {} // hide copy constructor
public:
    HeaderFactory(CodeFactory *Parent,CryFunctionDefList *AbstractFunctions);
    virtual Object *Create(const CryPropertyParser &PropertyName,Object *Parent)
    {
      return CodeFactory::Create(PropertyName,Parent);
    }
    virtual Object *Create(CryStream &FromStream)
    {
      return CodeFactory::Create(FromStream);
    }
    virtual Object *Create(const char *FactoryName,const CryPropertyParser &PropertyName,Object *Parent)
    {
      return CodeFactory::Create(FactoryName,PropertyName,Parent);
    }

    virtual Object *Create(const CryPropertyParser &PropertyName,CodeFactory *Parent);
	virtual const char *ChildClassName() const
	{
		return CHeaderFactory;
	}
	virtual bool IsA(const char *GetName) const
	{
		return ::strcmp(CHeaderFactory,GetName)==0 || CodeFactory::IsA(GetName);
	}
};


#define CFooter "Footer"
class FooterFactory : public CodeFactory
{
public:
    FooterFactory(CodeFactory *Parent);
    virtual Object *Create(const CryPropertyParser &PropertyName,CodeFactory *Parent);
    virtual const char *ChildClassName() const
    {
		return CFooter;
	}
	virtual bool IsA(const char *GetName) const
	{
		return ::strcmp(CFooter,GetName)==0 || CodeFactory::IsA(GetName);
	}
    virtual Object *Create(const CryPropertyParser &PropertyName,Object *Parent)
    {
      return CodeFactory::Create(PropertyName,Parent);
    }
    virtual Object *Create(CryStream &FromStream)
    {
      return CodeFactory::Create(FromStream);
    }
    virtual Object *Create(const char *FactoryName,const CryPropertyParser &PropertyName,Object *Parent)
    {
      return CodeFactory::Create(FactoryName,PropertyName,Parent);
    }

};

#define CIncludesFactory  "IncludesFactory"
class IncludesFactory : public CodeFactory
{
void AddInclude(const Object *p,const char *Type,CryString &CurrentIncludes,const char *File);

public:
    IncludesFactory(CodeFactory *Parent);
    virtual Object *Create(const CryPropertyParser &PropertyName,CodeFactory *Parent);
    virtual Object *Create(const Object *p,const CryPropertyParser &PropertyName, CryString &CurrentIncludes);

    virtual const char *ChildClassName() const
	{
		return CIncludesFactory;
	}
	virtual bool IsA(const char *GetName) const
	{
		return ::strcmp(CIncludesFactory,GetName)==0 || CodeFactory::IsA(GetName);
    }
        virtual Object *Create(const CryPropertyParser &PropertyName,Object *Parent)
    {
      return CodeFactory::Create(PropertyName,Parent);
    }
    virtual Object *Create(CryStream &FromStream)
    {
      return CodeFactory::Create(FromStream);
    }
    virtual Object *Create(const char *FactoryName,const CryPropertyParser &PropertyName,Object *Parent)
	{
	  return CodeFactory::Create(FactoryName,PropertyName,Parent);
	}

};


#define CInheritedFactory "InheritedFactory"
#define CCopyTo         "CopyTo_Function"
#define CCopyToEnd        "CopyTo_End"
class InheritedFactory : public CodeFactory
{
    CryFunctionDef *Func;
public:
    InheritedFactory(CodeFactory *Parent,CryFunctionDef &_Func);
    ~InheritedFactory();
    const CryFunctionDef *GetFunc();
    virtual Object *Create(const CryPropertyParser &PropertyName,CodeFactory *Parent);
    virtual bool IsA(const char *ClassName) const;    // can the object map to a ClassName
    virtual const char *ChildClassName() const
    {
        return CInheritedFactory;
	}
	/// will return a property represented as an object, useful for classes which contain properties that are dynamically allocated, as a property that is dynamic is a CryObject and therefore callable
	virtual Object *GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const;
	/// will return a pointer to the property if the property is an CryObject (or decendent)
	virtual Object *_GetPropertyAsObject(const CryPropertyParser &PropertyName) const;
/*! will return whether or not the property named in PropertyName is a container */
    virtual bool GetIsPropertyContainer(const CryPropertyParser &PropertyName) const;
    /*! will return the property named in PropertyName in a string format */
    virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
    /*! returns true if the class in question has the property */
    virtual bool HasProperty(const CryPropertyParser &PropertyName) const;
    /*! The count of the properties a class has */
    virtual int GetPropertyCount() const;
	/*! Make a list of all property names, the function is called from the parent class through each inheritance until it reaches this class, at which point a list is created and filled with any properties on the way back through the inheritance */
	virtual CryPropertyList* PropertyNames() const;
	virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
    virtual Object *Create(const CryPropertyParser &PropertyName,Object *Parent)
    {
      return CodeFactory::Create(PropertyName,Parent);
    }
    virtual Object *Create(CryStream &FromStream)
    {
      return CodeFactory::Create(FromStream);
    }
    virtual Object *Create(const char *FactoryName,const CryPropertyParser &PropertyName,Object *Parent)
    {
      return CodeFactory::Create(FactoryName,PropertyName,Parent);
    }

};



}// namespace Crystal
#endif	// CClassBuilder
