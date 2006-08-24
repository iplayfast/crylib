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
#ifndef TCodeFactory
#define TCodeFactory "CodeFactory"

#include "CryPattern.h"
#include "HeadImp.h"

namespace Crystal {

/**
@author Chris Bruner
*/

#define TSetProperty "SetProperty"
#define TGetProperty "GetProperty"

class CodeFactory : public CryFactory
{
    CryPropertyList Products;

    bool _IsProperty;
    bool _IsPointer;
    int SortValue;
	int Count;
    void AddFactory(CryFactory *f)
    {}
	
    CryString Name;
    CodeFactory *Parent;
    CodeFactory()
    {} // no no, only used for dup which immediatly does a copyto 
public:
StdFunctionsClassName(CodeFactory);
StdFunctionsChildClassName(CodeFactory);
StdFunctionsDup(CodeFactory);
StdFunctionsIsAbstractFalse();

    CodeFactory(CodeFactory *_Parent,const char *_Name);	// ProductCount is the number of products a factory can produce
    CodeFactory *GetParent() const;
    bool GetIsProperty() const;
    void SetIsProperty(bool Value);
    bool GetIsPointer() const;
    void SetIsPointer(bool Value);
	int GetCount() const { return Count; }
	void SetCount(int Value) { Count = Value; }
    virtual void CopyTo(CryObject &Dest) const;  //copies contents of this to Dest
    virtual bool IsA(const char *Name) const ;
    ~CodeFactory();
    CryPropertyList *DescribeProducts() const;
    virtual CryObject *Create(const CryPropertyParser &PropertyName,CodeFactory *Parent);
    virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent);
    const char *GetName() const;
    void SetName(const char *_Name);
    void AddProduct(const CryPropertyParser &PropertyName);
    void AddFactory(CodeFactory *f);
    bool CanBuildProduct(const CryPropertyParser &PropertyName);
    void Clear(const CryPropertyParser &PropertyName);
    virtual void Sort(int SortType=0);
    void SetSortValue(int v);
    int GetSortValue() const;
    void SetHead(const CryPropertyParser &PropertyName,const char *NewHead);
    void SetImp(const CryPropertyParser &PropertyName,const char *NewImp);
    void AppendHead(const char *Name);
    void AppendImp(const char *Name);
    void AppendHeadImp(const char *s);

    const CryString *GetHead(const CryPropertyParser &PropertyName) const;
    const CryString *GetImp(const CryPropertyParser &PropertyName) const;
    void SetHeadImp(const CryPropertyParser &PropertyName,const char *s);
	int GetPropertyCount() const;
		/// will return a property represented as an object, useful for classes which contain properties that are dynamically allocated, as a property that is dynamic is a CryObject and therefore callable
	virtual CryObject *GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const;
	/// will return a pointer to the property if the property is an CryObject (or decendent)
	virtual CryObject *_GetPropertyAsObject(const CryPropertyParser &PropertyName) const;

	virtual bool  SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
    virtual bool GetIsPropertyContainer(const CryPropertyParser &PropertyName) const;
	virtual const char * GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
	virtual CryPropertyList* PropertyNames() const;
	virtual bool HasProperty(const CryPropertyParser &PropertyName) const;
    virtual void Clear()   {};
    virtual CryObject *Create(CryStream &FromStream);
    virtual CryObject *Create(const char *FactoryName,const CryPropertyParser &PropertyName,CryObject *Parent=0)
    {
      return CryFactory::Create(FactoryName,PropertyName,Parent);
    }

};

}

#endif
