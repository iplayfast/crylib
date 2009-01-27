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
#ifndef CProperty
#define CProperty    "Property"
#define CPropertyList "PropertyList"
#define CPropertyParser "PropertyParser"
#define CPropertyIterator "PropertyIterator"

/// This class has the unusual ability of being able to have a property assigned to it at runtime.
class Property : public Object
{
	String *Name;
	Object* Value;
	void Init(const char *_Name,const char *_Value);
public:
	Property()
	{
	Init("NoName","");
	}
	StdFunctionsNoDup(Property,Object);
	Property(const char *_Name,const char *_Value);
	Property(const char *_Name);
	Property(Property &Copy);
	virtual ~Property();
	virtual FunctionDefList *GetFunctions(const char *Type=0) const;
	virtual Object *Dup() const;  /// creates a duplicate of this object

	void SetValue(const char *_Value);
	void SetValue(const Object *_Value);
	void SetValueOwned(Object *_Value);

	void SetName(const char *_Name);
	void SetNameValue(const char *_Name,const char *_Value)
	{
		SetValue(_Value);
		SetName(_Name);
	}
	const Object *GetValue() const;
	const char *GetValue(String &Result) const;	// return string value or Object name if not CryString
	Object*_GetValue() const;
	virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;
	virtual const char *GetProperty(String &Result) const;
	bool GetIsPropertyContainer(const PropertyParser &PropertyName) const;
	Object *GetCopyOfPropertyAsObject(const	PropertyParser &PropertyName) const;

	virtual const cbyte* GetRaw() const;
	//    virtual const char *ChildClassName() const;
	virtual bool HasProperty(const char *_PropertyName) const;
	virtual bool HasProperty(const PropertyParser &PropertyName) const;
	virtual bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);
	virtual bool SetProperty(const PropertyParser &PropertyName,const Object *PropertyValue);
	virtual void SetPropertyOwned(Object *PropertyValue);
	//virtual bool SetProperty(const char*PropertyName,const CryString &PropertyValue);
	virtual PropertyList* PropertyNames() const;
//    virtual bool IsAbstract() const;
    virtual void CopyTo(Object &Dest) const;   //copies contents of this to Dest
    const String *GetName() const;
    //    virtual Object *Dup() const;  /// creates a duplicate of this object
    virtual size_t Size() const;
}
;  //CryProperty

/// this class is used to easily parse out propertyname[Index]
class PropertyParser : public String
{
	int Index;	// if -1 then there is no []
	List Fields;
	PropertyParser() : String("")
	{
		Index = -1;
	}
	void constructParser();
public:
	StdFunctions(PropertyParser,String);
	PropertyParser(const char *Property);
	PropertyParser(const String *Property);

	~PropertyParser(){
	}
    bool operator ==(const char *s) const ;

	int GetIndex() const;
	const char *GetPlainProperty() const;
	void GetPlainProperty(String &Result) const;
	void Get(String &Result) const;
	bool IsMultiField() const { return Fields.HasItems(); }
	List::ListIterator *CreateIterator() const { return Fields.CreateIterator(); }
	void DeleteIterator(List::ListIterator *it) const { Fields.DeleteIterator(it); }
};


/// PropertyList will load in the properties of some object
/*! it is useful for
doing comparisons with other objects properties or to set an object's properties
(load one, set another)
*/
class PropertyList : protected List
{
public:
	struct PropertyIterator : public List::ListIterator
	{
		PropertyIterator(const PropertyList *container);
		~PropertyIterator() { }
		StdFunctionsNoDup(PropertyIterator,List::ListIterator);
		virtual Object *Dup() const;
		virtual void CopyTo(Object &Dest) const;
		bool GotoFirst();
		bool GotoPrev();
		bool GotoNext();
		bool GotoLast();
		size_t GetItemSize();
		Property *_Get() {		return (Property *)Get();		}
		const String *GetName();
		const char *GetValue(String &r);

		void SetName(const char *Name);
		void SetValue(const char *Value);
	};

private:
	PropertyList(PropertyList &nope); // avoid copying
	virtual PropertyIterator* GetPropertyPointerIterator(ListIterator *li,bool ExceptOnNotFound) const;	/// make sure you delete result after using
	virtual PropertyIterator* GetPropertyPointerIterator(const PropertyParser &PropertyName,bool ExceptOnNotFound) const;/// make sure you delete result after using

	virtual Property *GetPropertyPointer(ListIterator *li,bool ExceptOnNotFound) const;
	virtual Property *GetPropertyPointer(const PropertyParser &PropertyName,bool ExceptOnNotFound) const;
	virtual void AddPropertyOwned(ListIterator *li,Object *Value);
	virtual Property *_HasProperty(const PropertyParser &PropertyName) const;// return Property * or 0

public:
	(operator Object *)() { return this; }
	(operator Object &)() { return *this; }
//	CryList *GetNames() const { return Names; }
	StdFunctions(PropertyList,List);
	virtual ~PropertyList();
	PropertyList();
	// Even though property list is a container it is not treaded as such
	// because the properties cannot be indexed
	virtual bool IsContainer() const { return true; }

	virtual Iterator *_CreateIterator() const;
	PropertyIterator *CreateIterator() const;
//	virtual PropertyIterator *_CreateIterator() const;
	virtual void DeleteIterator(Iterator *LI) const;

//	CryContainer::Iterator * CreateNameIterator() const;
//	void DeleteNameIterator(Iterator *I) const;
	/// return the results of the last operation
	virtual PropertyList *PropertyNames() const;
	virtual PropertyList *Boolean(const Object *O2,BOperation B);
	virtual void Clear() { List::Clear(); }
	void Load(const Object *o);
	void Set(Object *Source);
	bool SetPropertiesFromList(Object *Target);

	void AddOwned(Property *Item)   // gives ownership to list
	{
		List::AddOwned(Item);
	}
	/// Get the currently held properties that are also in Target, and set them from TheseValues
	void Get(Object *Target);
	void AddOwned(Object *Item)   // We only can add Properties
	{
		if (Item->IsA(CProperty))
			AddOwned((Property *)Item);
		else
			throw Exception("Must add Properties only");
	}

	/// add a new property and value to the list
	void AddProperty(const char *Name,const char *value);
	/// add a new property to the list (property is owned by list);
	void AddProperty(const char *Name,const Object *Value);
	/// add a new property to the list (property is owned by list, Value ownership is transfered);
	void AddPropertyOwned(const char *Name,Object *Value);
	void AddPropertyOwned(const PropertyParser &PropertyName,Object *Value);

	/// add a new property to the list (property is owned by list);
	void AddProperty(String *Name,String *Value);
	// Add a new property tot he list, (property is owned by list);
	void AddProperty(PropertyParser &PropertyName,const String *PropertyValue);
	/// add a new property to the list, by giving the name and object that it came from. (Object is asked for Property value)
	void AddPropertyByName(const char *Name,const Object *object);
	virtual Object *_GetPropertyAsObject(const PropertyParser &PropertyName) const;
	virtual Object *GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const;
	/// check for a propertyname in the list
	virtual bool HasProperty(const PropertyParser &PropertyName) const;
	virtual bool HasProperty(const char *PropertyName) const;
	virtual int Compare(int CompareType,const Object *Test1,const Object *Test2) const;

	/*! returns true if the class in question can have the property */
	virtual bool CanHaveProperty(const PropertyParser &PropertyName) const
	{
		return true;
	}
	void RemoveAtIterator(PropertyIterator *LI) { List::RemoveAtIterator(LI); }

	virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;
	/*! will return whether or not the property named in PropertyName is a container */
	virtual bool GetIsPropertyContainer(const PropertyParser &PropertyName) const;

	/// return a char * of the value for a property
	const char *GetValueAsPChar(const PropertyParser &PropertyName,String &Result) const;
	const char *GetValue(const PropertyParser &PropertyName,String &Result) const;
	virtual bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);
	void RenameProperty(const char *OldName,const char *NewName);
	void RemoveProperty(const char *Name);
	virtual size_t Size() const;
	void RemoveNodeValue(const MemStream &Needle);
//	void RemoveAtIterator(Iterator *LI);
	void SetOwner(Object *_Owner) { OwnedObject::SetOwner(_Owner); }
	virtual void CopyTo(Object &Dest) const;
	virtual operator const Object *() const { return this; }
	virtual size_t Count() const { return List::Count(); }

	virtual bool Sortable()const { return true;}
	virtual void Sort(int CompareType=0);
};

class MetaPropertyList : public Object
{
const PropertyList *pl;
public:
	MetaPropertyList(const PropertyList *a) { pl = a; }
	operator const PropertyList *() { return pl; }
	operator const PropertyList &() { return *pl; }
	virtual PropertyList* PropertyNames() const { return ((List *)pl)->PropertyNames(); }
	const char *GetProperty(const PropertyParser &PropertyName,String &Result) const
		{ return ((List *)pl)->GetProperty(PropertyName,Result); }
	bool GetIsPropertyContainer(const PropertyParser &PropertyName) const
		{ return ((List *)pl)->GetIsPropertyContainer(PropertyName); }
	virtual bool HasProperty(const PropertyParser &PropertyName) const
		{ return ((List *)pl)->HasProperty(PropertyName); }
	virtual bool CanHaveProperty(const PropertyParser &PropertyName) const
		{ return ((List *)pl)->CanHaveProperty(PropertyName); }
	virtual Object *_GetPropertyAsObject(const PropertyParser &PropertyName) const
		{ return ((List *)pl)->_GetPropertyAsObject(PropertyName); }
	virtual Object *GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const
		{ return ((List *)pl)->GetCopyOfPropertyAsObject(PropertyName); }
 	virtual bool IsContainer() const { return true; }

};
#endif // TCryProperty
}
