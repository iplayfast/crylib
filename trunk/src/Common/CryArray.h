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

#ifndef ARRAY_DEF
#define ARRAY_DEF
#include "ClassContainer.h"
#include "ClassException.h"

extern "C" int (Compare) (const void *ele1,const void *ele2);

namespace Crystal
{
using namespace Crystal;
#define TCryArray    "CryArray"
#define TCryTemplateArray "CryTemplateArray"
#define TCrySimpleArray    "CrySimpleArray"
#define TCryDoubleArray "CryDoubleArray"
#define TCryIntArray "CryIntArray"
#define TCrySimpleArray "CrySimpleArray"

/// a simple array class which uses the iterator functions of CryContainer
class CrySimpleArray : public CryContainer
{
protected:
	size_t CurrentCount;	//current number of objects in the array
	size_t MaxCount;			// maximum number of objects that can be contained in the array
	size_t ElementSize;
	bool AllowResize;

struct ArrayIterator : public Iterator
	{
public:
		size_t i;
		ArrayIterator(const CryContainer *c) : Iterator(c)
		{
			i = 0;
		}
	virtual CryObject *Dup() const
	{
ArrayIterator *it = (ArrayIterator *)GetOrigContainer()->_CreateIterator();
		it->i = i;
		return it;
	}
	};

public:
StdFunctionsNoDup(CrySimpleArray,CryContainer);
	CrySimpleArray(size_t _ElementSize = 0)
	{
		ElementSize = _ElementSize;
		CurrentCount = MaxCount = 0;
	}
	virtual void CopyTo(CryObject &Dest) const;  //copies contents of this to Dest
	int IteratorValue(const Iterator *I) const
	{
		return ((ArrayIterator *)I)->i;
	}// if Iterator comes from a subclass it must be an ArrayIiterator
	virtual Iterator *_CreateIterator() const
	{
		ArrayIterator  *AI = new ArrayIterator(this);
		AI->i= 0;
		return AI;
	}
	ArrayIterator *CreateIterator() const
	{
		return (ArrayIterator *)_CreateIterator();
	}
	void SetAllowResize(bool v)
	{
		AllowResize = v;
	}
	virtual size_t Size() const
	{
		return CurrentCount;
	}
	virtual size_t Count() const
	{
		return CurrentCount;
	}
	virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;

	virtual void DeleteIterator(Iterator *AI) const
	{
		delete (ArrayIterator *)AI;
	}
	virtual bool GotoFirst(Iterator *AI) const
	{
		((ArrayIterator *)AI)->i = 0;
		return CurrentCount>0;
	}
	virtual bool GotoNext(Iterator *I) const
	{
		ArrayIterator *AI = (ArrayIterator *)I;
		if (AI->i<CurrentCount-1)
		{
			AI->i++;
			return true;
		}
		else
			return false;
	}
	virtual bool GotoPrev(Iterator *I) const   // returns true if success
	{
		ArrayIterator *AI = (ArrayIterator *)I;
		if ((AI->i-1<CurrentCount) && (AI->i>0))
		{
			AI->i--;
			return true;
		}
		else
			return false;
	}
	virtual bool GotoLast(Iterator *I) const
	{
		ArrayIterator *AI = (ArrayIterator *)I;
		AI->i = CurrentCount-1;
		return CurrentCount > 0;
	}
	virtual bool LoadAsText(int i,CryString &FromStream) = 0;
	virtual bool SaveAsText(int i,CryString &ToStream) const = 0;

	virtual bool LoadAsText(Iterator *I,CryString &FromStream)
	{
		return LoadAsText(((ArrayIterator *)I)->i,FromStream);
	}
	virtual bool SaveAsText(Iterator *I,CryString &ToStream) const
	{
		return SaveAsText(((ArrayIterator *)I)->i,ToStream);
	}

	// this class may be used for other things, so this is not used
	virtual EmptyObject *GetAtIterator(const Iterator *I) const = 0;
	/// will set a value to the container[Iterator]
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0);
	virtual void SetItem(unsigned int i,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0) = 0;
	virtual void RemoveAtIterator(Iterator *LI) = 0;
	virtual bool IsEmpty(const Iterator *I) const { return GetAtIterator(I)==0; }
	virtual void Clear() = 0;
	virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
	virtual bool SetPropertyAsObject(const CryPropertyParser &PropertyName,CryObject *Value);
	virtual bool SetPropertyAsObject(CryProperty *Value);
	virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
	virtual bool HasProperty(const CryPropertyParser &PropertyName)const;
	virtual int GetPropertyCount() const
	{
		return CryContainer::GetPropertyCount() + 4;
	}
	virtual CryPropertyList* PropertyNames() const;


	virtual size_t GetItemSize(Iterator *I) const
	{
		return ElementSize;
	}
	virtual void SetMax(size_t m);
	int GetMax() const
	{
		return MaxCount;
	}
	bool GetAllowResize() const
	{
		return AllowResize;
	}
#ifdef VALIDATING

	virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

};


template<typename T>
class CryTemplateArray : public CrySimpleArray
{
	T *Values;
	//int Size;
	// not used
	virtual EmptyObject *Add(EmptyObject *Item,size_t Size)
	{
		return Item;
	};
	virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size)
	{
		return Item;
	};

	virtual CryObject *Add(CryObject *Item)
	{
		return Item;
	};
	virtual CryObject *AddOwned(CryObject *Item)
	{
		return Item;
	};
	virtual void SetItemOwnerShip(Iterator *I,bool Owned)
	{}
	;
	virtual bool GetItemOwnerShip(const Iterator *I) const
	{
		return true;
	};
	virtual EmptyObject *GetAtIterator(const Iterator *I) const
	{
		return NULL;
	};
	virtual bool IsCryObject(const Iterator *I) const
	{
		return false;
	};
	virtual void GetEleType(CryString &Result) const;
public:
	CryTemplateArray<T>(CryTemplateArray<T> &Copy);
	StdFunctionsNoDup(CryTemplateArray,CrySimpleArray);
	void SetSize(size_t _Size);
	virtual CryObject *Dup()const; // creates a duplicate of this object
	CryTemplateArray<T> &Delete(int start,int amount);

	virtual void Clear() { CurrentCount = 0; }

	CryTemplateArray(int _Size=100) : CrySimpleArray(sizeof(T))
	{
		Values = new T[_Size];
		SetMax(_Size);
		SetSize(_Size);
	}
	~CryTemplateArray()
	{
		delete [] Values;
	}

	virtual void RemoveAtIterator(Iterator *LI);
	virtual bool LoadAsText(int i,CryString &FromStream);
	virtual bool SaveAsText(int i,CryString &ToStream) const;
	void SetItem(unsigned int i,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size)
	{
		SetValue(i,*(T*)Item);
	}

	const T operator [](int i)
	{
		return Values[i];
	}
	void SetValue(int i,T v)
	{
		Values[i] = v;
	}
	T GetValue(int i) const
	{
		if ((i<0) || ((unsigned )i>=CurrentCount))
			throw CryException(this,"Range Error");
		return Values[i];
	}
	virtual bool IsEmpty(const Iterator *I) const {  return ((ArrayIterator *)I)->i>CurrentCount; }
#ifdef VALIDATING
	virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

};



/// An Array class. Array elements are pointed to, and are created in the derived classes
class CryArray : public CrySimpleArray
{

	struct ElePtr
	{
		EmptyObject *Item;
		size_t Size;
		bool	IsOwned;
		bool IsCryObject;
		bool IsAutoCreated;
	};
	/// pointer to array of objects
	ElePtr  *pPtr;
	/// derived class will handle the destruction of objects contained in array
	virtual void DestroyArrayItem(CryArray *Owner,EmptyObject *Item) = 0;
	/// derived class will handle the creation of objects contained in array
	virtual EmptyObject *CreateArrayItem(CryArray *Owner,bool *IsCryObject) = 0;
	// derived class will handle the display in CryStream the objects contained in array (text assumed)
	virtual void SaveItemTo(const CryArray *Owner,EmptyObject *FromItem,CryStream &ToStream) const = 0;
	// derived class will handle the loading of an Object from the stream, objectmust have already been created
	virtual EmptyObject *LoadItemFrom(CryArray *Owner,EmptyObject *ToItem,CryStream &FromStream) = 0;
  void    DeleteItemOffset(unsigned int i);
  virtual void Sort(int SortType=0) {};

protected:
	virtual ~CryArray();									// derived class's destructor should SetCurrent(0); for proper cleanup
	EmptyObject *Add(EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0);
public:
StdFunctionsNoDup(CryArray,CrySimpleArray);
	CryArray(size_t ElementSize = 1);
	virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;
	void clear()
	{
		SetSize(0);
	}
	EmptyObject *GetItem(unsigned int i) const;						//will return object[i] indexed from 0
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0);

	const void *operator [](int i)
	{
		return GetItem(i);
	}
	void SetMax(size_t m);			// set the number that can be made
	void SetSize(size_t n); // set the number currently in use (either grow or shrink)
	//  size_t GetSize() const { return CurrentCount; }
    int DeleteItem(unsigned int i);
    virtual void CopyTo(CryArray &Dest) const;  //copies contents of this to Dest
    void Sort(int (Compare) (const void *ele1,const void *ele2));

    virtual void CopyTo(CryObject &Dest) const;  //copies contents of this to Dest
    /// logically a virtual function since the elements contained cannot be known in advance (eg  save and load text functions)
    virtual CryObject *Dup()const = 0;

    virtual const cbyte* GetRaw() const
    {
        return (const cbyte*) &pPtr[0];
    }
#ifdef VALIDATING
    virtual bool Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool Fail));
#endif


	// if this class contains the property name, it will attempt to load it
	// if all is well returns true
	virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
	virtual bool SetPropertyAsObject(const CryPropertyParser &PropertyName,CryObject *Value);
	virtual bool SetPropertyAsObject(CryProperty *Value);
	virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
	virtual bool HasProperty(const CryPropertyParser &PropertyName)const;
	virtual int GetPropertyCount() const
	{
		return CrySimpleArray::GetPropertyCount() + 2;
	}
	virtual CryPropertyList* PropertyNames() const;
	virtual EmptyObject *GetAtIterator(const Iterator *I) const
	{
		int i = IteratorValue(I);
		return GetItem(i);
	}
	/// will set a value to the container[Iterator]
	virtual void SetItem(unsigned int i,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0);
	virtual bool LoadAsText(int i,CryString &FromStream) = 0;
	virtual bool SaveAsText(int i,CryString &ToStream) const = 0;
    virtual bool LoadAsText(Iterator *I,CryString &FromStream)
    {
        return CrySimpleArray::LoadAsText(I,FromStream);
    }
    virtual bool SaveAsText(Iterator *I,CryString &ToStream) const
    {
        return CrySimpleArray::SaveAsText(I,ToStream);
    }
    // if owned Item is deleted, else returned
    virtual void RemoveAtIterator(Iterator *LI);

    /* ToDo: make these */
    // Arrays have these functions in order to faciltate streaming
    virtual EmptyObject *Add(EmptyObject *Item,size_t Size)
    {
        return Add(Item,false,false,Size);
	}
    virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size)
    {
        return Add(Item,false,true,Size);
    }
    virtual CryObject *Add(CryObject *Item)
    {
        return (CryObject *)Add(Item,true,false,0);
    }    // returns Item
    virtual CryObject *AddOwned(CryObject *Item)
    {
        return (CryObject *)Add(Item,true,true,0);
    }   // gives ownership to list
    EmptyObject *CryArray::DupItem(const CryArray::ElePtr  *Node) const;
    virtual void SetItemOwnerShip(Iterator *I,bool Owned);
    virtual bool GetItemOwnerShip(const Iterator *I) const;
    virtual bool IsCryObject(const Iterator *I) const;
    virtual bool IsEmpty(const Iterator *I) const;
	virtual void SetIsCryObject(Iterator *I,bool IsCO);
    size_t GetItemSize(Iterator *I) const;
    virtual void GetEleType(CryString &Result) const;

	virtual void Clear();

};

/// an array of doubles
class CryDoubleArray : public CrySimpleArray
{
	double *Values;
	//int Size;
	// not used
	virtual void SaveItemTo(const CryArray *Owner,EmptyObject *FromItem,CryStream &ToStream) const
		{}
	;
	virtual EmptyObject *LoadItemFrom(CryArray *Owner,EmptyObject *ToItem,CryStream &FromStream)
	{
		return ToItem;
	};
	virtual EmptyObject *Add(EmptyObject *Item,size_t Size)
	{
		return Item;
	};
	virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size)
	{
		return Item;
	};
	virtual CryObject *Add(CryObject *Item)
	{
		return Item;
	};
	virtual CryObject *AddOwned(CryObject *Item)
	{
		return Item;
	};
	virtual void SetItemOwnerShip(Iterator *I,bool Owned)
	{}
	;
	virtual bool GetItemOwnerShip(const Iterator *I) const
	{
		return true;
	};
	virtual EmptyObject *GetAtIterator(const Iterator *I) const
	{
		return NULL;
	};
	virtual bool IsCryObject(const Iterator *I) const
	{
		return false;
	};
	virtual void GetEleType(CryString &Result) const;

public:
StdFunctionsNoDup(CryDoubleArray,CrySimpleArray);
	void SetSize(size_t _Size);
	virtual CryObject *Dup()const; // creates a duplicate of this object

	virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;

	CryDoubleArray(int _Size=100) : CrySimpleArray(sizeof(double))
	{
		Values = new double[_Size];
		SetMax(_Size);
		SetSize(_Size);
	}
	~CryDoubleArray()
	{
		delete [] Values;
	}

	// always returns 0 (not creating a pointer to a double)
	virtual void RemoveAtIterator(Iterator *LI);
	/// will set a value to the container[Iterator]
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0);

	virtual void DestroyArrayItem(CryArray *Owner,EmptyObject *Item)
	{
		delete Item;
	};
	virtual EmptyObject *CreateArrayItem(CryArray *Owner,bool *IsCryObject)
	{
		*IsCryObject = false;
		double *d = new double;
		return (EmptyObject *)d;
	};
	virtual bool LoadAsText(int i,CryString &FromStream);
	virtual bool SaveAsText(int i,CryString &ToStream) const;
	virtual bool LoadAsText(Iterator *I,CryString &FromStream)
	{
		return LoadAsText(((ArrayIterator *)I)->i,FromStream);
	}
	virtual bool SaveAsText(Iterator *I,CryString &ToStream) const
	{
		return SaveAsText(((ArrayIterator *)I)->i,ToStream);
	}
	void SetItem(unsigned int i,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size)
	{
		SetValue(i,*(double *)Item);
	}

	const double operator [](int i)
	{
		return Values[i];
	}
	void SetValue(int i,double v)
	{
		Values[i] = v;
	}
	double GetValue(int i) const
	{
		if ((i<0) || ((unsigned )i>=CurrentCount))
			printf("here");
		return Values[i];
	}
	void Clear()
{ }	// means nothing do a double

	// if this class contains the property name, it will attempt to load it
	// if all is well returns true
	virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
	virtual bool SetPropertyAsObject(const CryPropertyParser &PropertyName,CryObject *Value);
	virtual bool SetPropertyAsObject(CryProperty *Value)
	{
		return CrySimpleArray::SetPropertyAsObject(Value);
	}

	virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
	virtual bool HasProperty(const CryPropertyParser &PropertyName)const;
	virtual int GetPropertyCount() const;
	virtual CryPropertyList* PropertyNames() const;
	virtual bool IsEmpty(const Iterator *I) const {  return ((ArrayIterator *)I)->i>CurrentCount; }
};


class CryIntegerArray : public CryTemplateArray<int>
{
public:

};
class CryIntArray : public CrySimpleArray
{
int *Values;


//Array Length defines


//Local Variables Declaration


public:


//Constructor/Destructor Functions
	CryIntArray(int _SetSize=10);
	~CryIntArray();

	void SetSize(size_t _Size);

//Inherited Functions
virtual void DestroyArrayItem(CryArray *Owner,EmptyObject *Item);
virtual EmptyObject *CreateArrayItem(CryArray *Owner,bool *IsCryObject);
//virtual void SaveItemTo(const CryArray *Owner,EmptyObject *FromItem,CryStream &ToStream) const;
//virtual EmptyObject *LoadItemFrom(CryArray *Owner,EmptyObject *ToItem,CryStream &FromStream);
virtual bool LoadAsText(int i,CryString &FromStream);
virtual bool SaveAsText(int i,CryString &ToStream) const;
virtual void GetEleType(CryString &Result) const;
//virtual void DeleteIterator(Iterator *I) const;
//virtual bool GotoFirst(Iterator *I) const;
//virtual bool GotoLast(Iterator *Iterator) const;
virtual EmptyObject *GetAtIterator(const Iterator *I) const;
virtual CryPropertyList *PropertyNames() const;

virtual void CopyTo(CryObject &Dest) const;

virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0);
virtual void SetItem(unsigned int i,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0);
virtual void RemoveAtIterator(Iterator *LI);
virtual void Clear();
virtual const char *GetProperty(const char *PropertyName,CryString &Result) const;

virtual EmptyObject *Add(EmptyObject *Item,size_t Size);
	virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size);
virtual CryObject *Add(CryObject *Item);    // returns Item
virtual CryObject *AddOwned(CryObject *Item);   // gives ownership to list
virtual void SetItemOwnerShip(Iterator *I,bool Owned);
virtual bool GetItemOwnerShip(const Iterator *I) const;
virtual bool IsCryObject(const Iterator *I) const;
	void SetValue(int i,int v)
	{
		Values[i] = v;
	}
	int GetValue(int i) const
	{
		if ((i<0) || ((unsigned )i>=CurrentCount))
			throw CryException(
			this,"Range check");
		return Values[i];
	}

//Common functions Macro
//Creates Dup, ClassName,ChildClassName,IsAbstract and IsA() Functions
	StdFunctions(CryIntArray,CrySimpleArray);


//Property Access Functions


//Class Footer
}; // class CryIntArray


} // end namespace Crystal
#endif //ARRAY_DEF