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

#include "ClassList.h"
#include "ClassString.h"

namespace Crystal
{
using namespace std;
#ifndef CCryProperty
#define CCryProperty    "CryProperty"
#define CCryPropertyList "CryPropertyList"
#define CCryPropertyParser "CryPropertyParser"
#define CPropertyIterator "PropertyIterator"

/// This class has the unusual ability of being able to have a property assigned to it at runtime.
class CryProperty : public Object
{
    CryString *Name;
    Object* Value;
	void Init(const char *_Name,const char *_Value);
public:
	CryProperty()
	{
	Init("NoName","");
	}
	StdFunctionsNoDup(CryProperty,Object);
	CryProperty(const char *_Name,const char *_Value);
	CryProperty(const char *_Name);
	CryProperty(CryProperty &Copy);
	virtual ~CryProperty();
	virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;
	virtual Object *Dup() const;  /// creates a duplicate of this object

	void SetValue(const char *_Value);
	void SetValue(const Object *_Value);
    void SetName(const char *_Name);
    void SetNameValue(const char *_Name,const char *_Value)
    {
        SetValue(_Value);
        SetName(_Name);
    }
	const Object *GetValue() const;
	const char *GetValue(CryString &Result) const;	// return string value or Object name if not CryString
	Object*_GetValue() const;
    virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
	virtual const char *GetProperty(CryString &Result) const;
	virtual const cbyte* GetRaw() const;
    //    virtual const char *ChildClassName() const;
	virtual bool HasProperty(const CryPropertyParser &PropertyName) const;
	virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
    virtual bool SetProperty(const CryPropertyParser &PropertyName,const Object *PropertyValue);
	//virtual bool SetProperty(const char*PropertyName,const CryString &PropertyValue);
	virtual CryPropertyList* PropertyNames() const;
//    virtual bool IsAbstract() const;
    virtual void CopyTo(Object &Dest) const;   //copies contents of this to Dest
    const CryString *GetName() const;
    //    virtual CryObject *Dup() const;  /// creates a duplicate of this object
    virtual size_t Size() const;
}
;  //CryProperty

/// this class is used to easily parse out propertyname[Index]
class CryPropertyParser : public CryString
{
	int Index;	// if -1 then there is no []
	CryPropertyParser() : CryString("")
	{
		Index = -1;
	}
public:
	StdFunctions(CryPropertyParser,CryString);
	CryPropertyParser(const char *Property);
	int GetIndex() const;
	const char *GetPlainProperty() const;
	void GetPlainProperty(CryString &Result) const;
	void Get(CryString &Result) const;
};

/// CryPropertyList will load in the properties of some object
/*! it is useful for
doing comparisons with other objects properties or to set an object's properties
(load one, set another)
*/
class CryPropertyList : protected List
{
	CryPropertyList(CryPropertyList &nope); // avoid copying
public:
	struct PropertyIterator : public List::ListIterator
	{
		PropertyIterator(const CryPropertyList *container);
		~PropertyIterator() { }
		StdFunctionsNoDup(PropertyIterator,List::ListIterator);
		virtual Object *Dup() const;
		virtual void CopyTo(Object &Dest) const;
		bool GotoFirst();
		bool GotoPrev();
		bool GotoNext();
		bool GotoLast();
		size_t GetItemSize();
		CryProperty *_Get() { return (CryProperty *)Get(); }
		const CryString *GetName();
		const char *GetValue(CryString &r);

		void SetName(const char *Name);
		void SetValue(const char *Value);
	};

//	CryList *GetNames() const { return Names; }
	StdFunctions(CryPropertyList,List);
	virtual ~CryPropertyList();
	CryPropertyList();
	virtual Iterator *_CreateIterator() const;
	PropertyIterator *CreateIterator() const;
//	virtual PropertyIterator *_CreateIterator() const;
	virtual void DeleteIterator(Iterator *LI) const;

//	CryContainer::Iterator * CreateNameIterator() const;
//	void DeleteNameIterator(Iterator *I) const;
	/// return the results of the last operation
	virtual CryPropertyList* PropertyNames() const;
	CryPropertyList *Boolean(const Object *O2,BOperation B);
	void Load(const Object *o);
	void Set(Object *Source);
	/// Get the currently held properties that are also in Target, and set them from TheseValues
	void Get(Object *Target);
	/// add a new property and value to the list
	void AddProperty(const char *Name,const char *value);
	/// add a new property to the list (property is owned by list);
	void AddProperty(const char *Name,Object *Value);
	/// add a new property to the list (property is owned by list);
	void AddProperty(CryString *Name,CryString *Value);
	/// add a new property to the list, by giving the name and object that it came from. (Object is asked for Property value)
	void AddPropertyByName(const char *Name,const Object *object);
	virtual Object *_GetPropertyAsObject(const CryPropertyParser &PropertyName) const;
	virtual Object *GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const;
	/// check for a propertyname in the list
	virtual bool HasProperty(const CryPropertyParser &PropertyName) const;
	virtual int Compare(int CompareType,const Object *Test1,const Object *Test2) const;

	/*! returns true if the class in question can have the property */
	virtual bool CanHaveProperty(const CryPropertyParser &PropertyName) const
	{
		return true;
	}

	virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
	/*! will return whether or not the property named in PropertyName is a container */
	virtual bool GetIsPropertyContainer(const CryPropertyParser &PropertyName) const;

	/// return a char * of the value for a property
	const char *GetValueAsPChar(const CryPropertyParser &PropertyName,CryString &Result) const;
	const char *GetValue(const CryPropertyParser &PropertyName,CryString &Result) const;
	virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
	void RenameProperty(const char *OldName,const char *NewName);
	virtual size_t Size() const;
	void RemoveNodeValue(const CryMemStream &Needle);
//	void RemoveAtIterator(Iterator *LI);
	void SetOwner(Object *_Owner) { OwnedObject::SetOwner(_Owner); }
	virtual void CopyTo(Object &Dest) const;
	virtual operator const Object *() const { return this; }
	virtual size_t Count() const { return List::Count(); }

	virtual bool Sortable()const { return true;}
	virtual void Sort(int CompareType=0);
};

#endif // TCryProperty
}
