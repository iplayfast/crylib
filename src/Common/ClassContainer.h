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

#include "ClassObject.h"
//#include "CryArray.h"
//#include "CryBackProp.h"
//#include "CryFuzzy.h"
//#include "CryPattern.h"

namespace Crystal
{

using namespace std;

#ifndef CContainer
#define CContainer "Container"

/// This class is a container of some sort, (either a linked list or an array of some type)
/*!
	Containers are named classes
	Objects are able to Iterate through a container class, calling IteratedFunction for each item contained
*/
class Container : public OwnedObject	//abstract
{
	bool _IsContainer;
	// don't allow copyclass
	Container(Container &NoNO);
public:
	StdFunctionsAbstract(Container,Object);
	class Iterator;
	Container();
	/// will return the original owner of the Iterator
	const Container *GetOwner(Iterator *I) const;
	/// returns true
	virtual bool IsContainer() const;
	virtual void SetIsContainer(bool Enabled);
	/// will return the type of contained type used by the container. eg  CryStream::ListNode
	virtual void GetEleType(String &Result) const = 0;
	/// Create an Iterator class which can be used for travesing the container
	virtual Iterator *_CreateIterator() const = 0;
	/// non virtual function
	Iterator *CreateIterator() const { return _CreateIterator(); }
	/// Will cleanup any iterator data created with _CreateIterator
	virtual void DeleteIterator(Iterator *I) const = 0;

	/// abstract function used by subclasses to go to the first element of a container (list or array doesn't matter)
	virtual bool GotoFirst(Iterator *I) const = 0;   // returns true if success
	/// abstract function used by subclasses to go to the Prev element of a container (list or array doesn't matter)
	virtual bool GotoPrev(Iterator *I) const = 0;   // returns true if success
	/// abstract function used by subclasses to go to the Next element of a container (list or array doesn't matter)
	virtual bool GotoNext(Iterator *I) const = 0;    // returns true if success
	/// abstract function used by subclasses to go to the last element of a container (list or array doesn't matter)
	virtual bool GotoLast(Iterator *Iterator) const = 0;    // returns true if success
	virtual bool GotoN(Iterator *Iterator,int n) const; // returns true if success
	/// abstract function used by subclasses to show if the container has any elements


	virtual bool HasFirst(const Iterator *I) const { return Count()>0; }
	virtual bool HasPrev(const Iterator *I) const = 0;
	virtual bool HasNext(const Iterator *I) const = 0;
        virtual bool HasLast(const Iterator *I) const { return Count()>0; }
        virtual bool HasN(const Iterator *I,unsigned int n) const { return Count()>n; }

	virtual bool IsEmpty(const Iterator *I) const = 0;
	/// will return whatever is located at the Current Iterator
	virtual EmptyObject *GetAtIterator(const Iterator *I) const = 0;
	/// will set a value to the container[Iterator]
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0) = 0;

	/// abstract function used by subclasses to remove the item this iterator points at (iterator is still valid)
	virtual void RemoveAtIterator(Iterator *I) = 0;
	/// abstract function used by subclasses to count the number of items held by the container
        virtual size_t Count() const = 0;
	//virtual bool CanDup() const { return false; }	// Containers might contain things that can't be duped, so we must assume they can't
	/// abstract function used by subclasses to clear whatever is contained within the containers
	virtual void Clear() = 0;
	/// return a list of functions contained in this class
	virtual FunctionDefList *GetFunctions(const char *Type=0) const;
	virtual EmptyObject *Add(EmptyObject *Item,size_t Size)  = 0;
	virtual void /*EmptyObject * */AddOwned(EmptyObject *Item,size_t Size) = 0;
	virtual Object *Add(Object *Item) = 0;    // returns Item
	virtual void /*Object * */AddOwned(Object *Item) = 0;   // gives ownership to list
	virtual void SetItemOwnerShip(Iterator *I,bool Owned) = 0;
	virtual bool GetItemOwnerShip(const Iterator *I) const = 0;
	virtual size_t GetItemSize(Iterator *I) const = 0;
	virtual bool IsObject(const Iterator *I) const = 0;
	virtual bool LoadAsText(Iterator *I,String &FromStream) = 0;
	virtual bool SaveAsText(Iterator *I,String &ToStream) const = 0;
	bool IterateThroughAll(EmptyObject *Control); // for each item, will call bool (*FunctionToCall)(EmptyObject *); (unless 0)
	/// returns true if all contained things where iterated through, FunctionToCall returns false if iteration should stop
	//    bool IterateThroughAll(ClassCall *FunctionToCall);
	virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item);
	virtual Object *GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const;
	virtual bool HasProperty(const PropertyParser &PropertyName) const;
	virtual int GetPropertyCount() const;
	//    virtual const char* ChildClassName() const;
	virtual PropertyList* PropertyNames() const;
	virtual bool Sortable() const;
	virtual void Sort(int CompareType=0);
	//! Compare the contained items (Used in sorting)
	virtual int Compare2(int CompareType,const EmptyObject *First,const EmptyObject *Second) const;
	// Compare to another Object
	virtual int CompareLogical(int CompareType,const Object *Test) const;
	virtual bool LessThen(int CompareType,const Object *Test) const;
	virtual bool GreaterThen(int CompareType,const Object *Test) const;
	virtual bool EqualTo(int CompareType,const Object *Test) const;
	bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);
	bool SetProperty(const char *PropertyName,const char *PropertyValue);
	const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;


#ifdef VALIDATING

	virtual bool Test(bool Verbose,Object &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif
	/*! This class derives from EmptyObject and is used to traverse the container, Containers may be used in a generic way by using iterators
	*/
#define TIterator "Iterator"
class Iterator : public Object//EmptyObject
	{
	private:
		const Container *OrigContainer; /* TODO : Add checks to see if interator is still with original container */
	public:
		Iterator(const Container *oc );
		~Iterator() { }
		/// creates a duplicate of this object, is defined as pure in order to force derived classes to handle this.
		virtual Object *Dup() const=0;
		bool IsContainer() const; // true if item is an object and is a container and is currently set as a container (believes it is a container)
		bool IsObject() const;
		bool IsOwned() const;
		bool IsEmpty() const;
		EmptyObject *Get() const;
		Object *GetObject() const;	// if current iterator isn't at an object will give exception
		//void Set(EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0);

		bool GotoFirst();
		bool GotoPrev();
		bool GotoNext();
		bool GotoLast();
		bool GotoN(int N);
		bool HasFirst() const  { return OrigContainer->Count()>0; }
		bool HasPrev() const { return OrigContainer->HasPrev(this); }
		bool HasNext() const { return OrigContainer->HasNext(this); }
		bool HasLast() const { return OrigContainer->Count()>0; }
                bool HasN(unsigned int n) const { return OrigContainer->Count()>n; }

		bool GotoFirstObject(const char *Type=CObject);
		bool GotoPrevObject(const char *Type=CObject);
		bool GotoNextObject(const char *Type=CObject);
		bool GotoLastObject(const char *Type=CObject);
		bool GotoFirstNonObject();
		bool GotoPrevNonObject();
		bool GotoNextNonObject();
		bool GotoLastNonObject();

		size_t GetItemSize();
		//bool LoadAsText(CryString *FromStream) { return OrigContainer->LoadAsText(this,FromStream); }
		bool SaveAsText(String &ToStream);
		const Container *GetOrigContainer() const;
		/// return the type of structure used to control this list (ie listnode eleptr, etc)
		void GetEleType(String &Result) const;
		virtual bool IsA(const char *ClassName) const    // can the object map to a ClassName
		{
			return (strcmp(ClassName,TIterator)==0);
		}
		/// returns a pointer to a string stating the current class Name, EG Object (not CryString)
		const char* ClassName() const
		{
			return TIterator;
		}

		/// returns a pointer to a string stating the ChildClassName, EG CryString (not Object)
        virtual const char *ChildClassName() const
        {
            return TIterator;
        }
	};
};


#endif //TCryContainer
}
