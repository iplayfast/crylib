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
#include <stdlib.h>
#include "ClassObject.h"
#include "ClassContainer.h"
#include "ClassException.h"
#include "ClassString.h"
#include "ClassProperty.h"

extern "C" int (Compare) (const void *ele1,const void *ele2);

namespace Crystal
{
using namespace Crystal;
#define CArray    "Array"
#define CTArray "TArray<>"
#define CSimpleArray    "SimpleArray"
#define CDoubleArray "DoubleArray"
#define CIntArray "IntArray"

extern int gint;
/// a simple array class which uses the iterator functions of CryContainer
class SimpleArray : public Container
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
		ArrayIterator(const Container *c) : Iterator(c)
		{
			i = 0;
		}
	virtual Object *Dup() const
	{
ArrayIterator *it = (ArrayIterator *)GetOrigContainer()->_CreateIterator();
		it->i = i;
		return it;
	}
	};

public:
StdFunctionsNoDup(SimpleArray,Container);
	SimpleArray(size_t _ElementSize = 0)
	{
		ElementSize = _ElementSize;
		CurrentCount = MaxCount = 0;
	}
	virtual void CopyTo(Object &Dest) const;  //copies contents of this to Dest
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
	size_t GetElementSize() const { return ElementSize;    }
	virtual FunctionDefList *GetFunctions(const char *Type=0) const;

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
	virtual bool LoadAsText(int i,String &FromStream) = 0;
	virtual bool SaveAsText(int i,String &ToStream) const = 0;

	virtual bool LoadAsText(Iterator *I,String &FromStream)
	{
		return LoadAsText(((ArrayIterator *)I)->i,FromStream);
	}
	virtual bool SaveAsText(Iterator *I,String &ToStream) const
	{
		return SaveAsText(((ArrayIterator *)I)->i,ToStream);
	}

	// this class may be used for other things, so this is not used
	virtual EmptyObject *GetAtIterator(const Iterator *I) const = 0;
	/// will set a value to the container[Iterator]
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0);
	virtual void SetItem(unsigned int i,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0) = 0;
	virtual void RemoveAtIterator(Iterator *LI) = 0;
	virtual bool IsEmpty(const Iterator *I) const { return GetAtIterator(I)==0; }
	virtual void Clear() = 0;
	virtual bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);
	virtual bool SetPropertyAsObject(const PropertyParser &PropertyName,Object *Value);
	virtual bool SetPropertyAsObject(Property *Value);
	virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;
	virtual bool HasProperty(const PropertyParser &PropertyName)const;
	virtual int GetPropertyCount() const
	{
		return Container::GetPropertyCount() + 3;
	}
	virtual PropertyList* PropertyNames() const;

	virtual void SetSize(size_t _Size)	=0;


	
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
	virtual bool Test(bool Verbose,Object &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

};


template<typename T>
class TArray : public SimpleArray
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

	virtual Object *Add(Object *Item)
	{
		return Item;
	};
	virtual Object *AddOwned(Object *Item)
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
		return (EmptyObject *) &Values[IteratorValue(I)];
    	}
	virtual bool IsObject(const Iterator *I) const
	{
		return false;
	};
public:
	virtual void GetEleType(String &Result) const
	{
		throw Exception("GetEleType needs to be added from CryTArray");
	}

		/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the Parent must be the appropriete container type or a derived class which can create the object (if the default class can't) */
	virtual Object *Create(const PropertyParser &PropertyName,Object *Parent=0)
	{
		return SimpleArray::Create(PropertyName,Parent);
//		throw Exception("Create needs to be added from CryTArray");
	}
	static Object *ClassCreate(const PropertyParser &PropertyName,Object *Parent=0)
	{
		return SimpleArray::Create(PropertyName,Parent);
//		throw Exception("ClassCreate needs to be added from CryTArray");
	}

	TArray<T>(const TArray<T> &Copy)
	{
		SetMax(Copy.Size());
		ElementSize = Copy.ElementSize;
		CurrentCount = Copy.Size();
		Values = new T[Copy.Size()];
		for(int i=0;i<Copy.Size();i++)
			Values[i] = Copy.Values[i];
	}
	TArray<T> &operator =(const TArray<T> &From)
	{
		From.CopyTo(*this);
		return *this;
	}

	StdFunctionsNoDup(TArray,SimpleArray);
	virtual void CopyTo(Object &Dest) const  //copies contents of this to Dest
	{
		if (Dest.IsA(CTArray)) {
		SimpleArray *t = (SimpleArray *)&Dest;
			if (t->GetElementSize()==GetElementSize()) {	// if element sizes are the same, then we assume theyare the same type of elements...
			TArray<T> *t = (TArray<T> *)&Dest;
				t->SetSize(Size());
				memcpy(t->Values,Values,GetElementSize() * Size());
				t->CurrentCount = CurrentCount;
				return;
			}
			throw Exception("Copying to an array of different type");
		}
		SimpleArray::CopyTo(Dest);
	}

	T GetDefault() const
	{
	// default value for <T>
		throw Exception("Class needs to be expanded");
	}
//	void SetSize(size_t _Size);
	void SetSize(size_t _Size)
	{
		if ((MaxCount < 1000) && (MaxCount>_Size)) {
			return;	// we're good
		}
		T *n = new T[_Size];
		SetMax(_Size);
		int CopyAmount = CurrentCount;
		if (_Size<CurrentCount)
			CopyAmount = _Size;
		for(int i=0;i<CopyAmount;i++)
			n[i] = Values[i];
	//	for(unsigned int i=CopyAmount;i<_Size;i++)
	//		n[i] = GetDefault();  // default value for <T>
		delete []Values;
		Values = n;
	}

	virtual bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
	{
		if (PropertyName=="CurrentCount")
		{
			CurrentCount = atoi(PropertyValue);
			return true;
		}
		if (PropertyName=="Type")	// read only
			return true;
		return SimpleArray::SetProperty(PropertyName,PropertyValue);
	}

	virtual bool HasProperty(const PropertyParser &PropertyName)const
	{
		return PropertyName=="Type" ||
				PropertyName=="CurrentCount" ||
			SimpleArray::HasProperty(PropertyName);
	}


	virtual int GetPropertyCount() const
	{
		return SimpleArray::GetPropertyCount() + 2;
	}



	PropertyList* PropertyNames() const
	{
			PropertyList *n = SimpleArray::PropertyNames();
			n->AddPropertyByName("Type",this);
			n->AddPropertyByName("CurrentCount",this);
	//		n->AddPropertyByName("Values",this);
			return n;
	}
	const char *GetProperty(const PropertyParser &PropertyName,String &Result) const
	{
		Result.Clear();
		if (PropertyName=="Type")
		{
			GetEleType(Result);
			return Result;
		}
		if (PropertyName=="CurrentCount")
		{
			Result.printf("%d",CurrentCount);
			return Result.AsPChar();
		}
/*		if (PropertyName=="Values") {
			Result = "[]";  // if Result != what is returned, it's a special situation
			return "*";
		}*/
		return SimpleArray::GetProperty(PropertyName,Result);
	}
	virtual Object *Dup()const // creates a duplicate of this object
	{
		return new TArray<T>(*this);
	}

	virtual void Clear() { CurrentCount = 0; }

	TArray(int _Size=100) : SimpleArray(sizeof(T))
	{
		Values = 0;
		SetSize(_Size+1);
	}
	~TArray()
	{
		delete [] Values;
	}

	virtual void RemoveAtIterator(Iterator *LI)
	{
		ArrayIterator *ai = (ArrayIterator *)LI;
		Delete(ai->i,1);
	}
	virtual bool LoadAsText(int i,String &FromStream)
	{
		throw Exception("CryTArray needs LoadAsText");
	}
	virtual bool SaveAsText(int i,String &ToStream) const
	{
		throw Exception("CryTarray needs SaveAsText");
	}

	void SetItem(unsigned int i,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
	{
		SetValue(i,*(T*)Item);
	}

	const T &operator [](int i)
	{
		return Values[i];
	}
	void SetValue(int i,T v)
	{
		if (i>=CurrentCount) {
			if(i>=MaxCount)
				SetSize(i+1);
			CurrentCount = i+1;
		}
		Values[i] = v;
	}
TArray<T> &Delete(int start,int amount)
{
	if (start>=Size()) return *this;
	if (start+amount>=MaxCount) {
		amount = MaxCount - start;
		if (amount<=0) {
			return *this;
		}
	}
	while(start<Size())
	{
		if (start+amount>=Size()) {
			CurrentCount=start;
			return *this;
		}
		Values[start] = Values[start+amount];
		start++;
	}
	return *this;
}

	T GetValue(int i) const
	{
		if ((i<0) || ((unsigned )i>=CurrentCount))
			throw Exception(this,"Range Error");
		return Values[i];
	}
	virtual bool IsEmpty(const Iterator *I) const {  return ((ArrayIterator *)I)->i>CurrentCount; }
#ifdef VALIDATING
	virtual bool Test(bool Verbose,Object &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail))
{
/* need to code tests for the following functions
	StdFunctionsNoDup(CryTArray,CrySimpleArray);
	void SetSize(size_t _Size);
	virtual Object *Dup()const; // creates a duplicate of this object
	CryTArray<T> &Delete(int start,int amount);

	virtual void Clear() { CurrentCount = 0; }

	CryTArray(int _Size=100) : CrySimpleArray(sizeof(T))
	{
		Values = new T[_Size];
		SetMax(_Size);
		SetSize(_Size);
	}
	~CryTArray()
	{
		delete [] Values;
	}

	virtual void RemoveAtIterator(Iterator *LI);
	virtual bool LoadAsText(int i,CryString &FromStream);
	virtual bool SaveAsText(int i,CryString &ToStream) const;
	void SetItem(unsigned int i,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
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
			throw Exception(this,"Range Error");
		return Values[i];
	}
	virtual bool IsEmpty(const Iterator *I) const {  return ((ArrayIterator *)I)->i>CurrentCount; }
*/
	return Container::Test(Verbose,Object,CallBack);
}
#endif
};

/*template<>
int Crystal::CryTArray<int>::GetDefault() const
{
	return 0;
}
template<>
float Crystal::CryTArray<float>::GetDefault() const
{
	return 0.0;
}

template<>
CryString Crystal::CryTArray<CryString>::GetDefault() const
{
	return "";
}
*/
/*template<>
void CryTArray<int>::GetEleType(CryString &Result) const
	{
		Result = "int";
	}
template<>
void CryTArray<float>::GetEleType(CryString &Result) const
	{
		Result = "float";
	}
template<>
void CryTArray<CryString>::GetEleType(CryString &Result) const
	{
		Result = "CryString";
	}
  */
	/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the Parent must be the appropriete container type or a derived class which can create the object (if the default class can't) */
template<>
Object *TArray<int>::Create(const PropertyParser &PropertyName,Object *Parent);
/*{

	if (PropertyName==CTArray)
		return new TArray<int>();
	return SimpleArray::Create(PropertyName,Parent);
}*/
template<>
void TArray<int>::GetEleType(String &Result) const;

template<>
bool TArray<int>::LoadAsText(int i,String &FromStream);
template<>
bool TArray<int>::SaveAsText(int i,String &ToStream) const;
/*{
	ToStream.printf("%d ",Values[i]);
	return true;
}*/
/*const char *TArray<int>::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (PropertyName=="Type")
	{
		Result = "int";
		return Result.AsPChar();
	}
	if (PropertyName=="int")
	{
		Result = "[]";  // if Result != what is returned, it's a special situation, in this case pointer to an array of weights
		return "*";
	}
	if (PropertyName=="*int")
	{
		for(int i=0;i<CurrentCount;i++)
			Result.printf("%d ",Values[i]);
		return Result.AsPChar();
	}
	return SimpleArray::GetProperty(PropertyName,Result);
}
  */
/*template<>
bool TArray<int>::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="int")
	{
	String Pv = PropertyValue;
		Pv.SeekFromStart(0);
		for(int i=0;i<CurrentCount;i++)
		{
		int v;
			Pv.scanf("%d ",&v);
			SetValue(i,v);
		}
		return true;
	}
	return SimpleArray::SetProperty(PropertyName,PropertyValue);
}
*/
	/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the Parent must be the appropriete container type or a derived class which can create the object (if the default class can't) */
template<>
Object *TArray<float>::Create(const PropertyParser &PropertyName,Object *Parent);
template<>
void TArray<float>::GetEleType(String &Result) const;
template<>
bool TArray<float>::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);
template<>
bool TArray<float>::LoadAsText(int i,String &FromStream);
template<>
bool TArray<float>::SaveAsText(int i,String &ToStream) const;

/// An Array class. Array elements are pointed to, and are created in the derived classes
class Array : public SimpleArray
{
	struct ElePtr
	{
		EmptyObject *Item;
		size_t Size;
		bool	IsOwned;
		bool IsObject;
		bool IsAutoCreated;
	};
	/// pointer to array of objects
	ElePtr  *pPtr;
	/// derived class will handle the destruction of objects contained in array
	virtual void DestroyArrayItem(Array *Owner,EmptyObject *Item) = 0;
	/// derived class will handle the creation of objects contained in array
	virtual EmptyObject *CreateArrayItem(Array *Owner,bool *IsObject) = 0;
	// derived class will handle the display in CryStream the objects contained in array (text assumed)
	virtual void SaveItemTo(const Array *Owner,EmptyObject *FromItem,Stream &ToStream) const = 0;
	// derived class will handle the loading of an Object from the stream, objectmust have already been created
	virtual EmptyObject *LoadItemFrom(Array *Owner,EmptyObject *ToItem,Stream &FromStream) = 0;
  void    DeleteItemOffset(unsigned int i);
  virtual void Sort(int SortType=0) {};

protected:
	virtual ~Array();									// derived class's destructor should SetCurrent(0); for proper cleanup
	EmptyObject *Add(EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0);
public:
StdFunctionsNoDup(Array,SimpleArray);
	Array(size_t ElementSize = 1);
	virtual FunctionDefList *GetFunctions(const char *Type=0) const;
	void clear()
	{
		SetSize(0);
	}
	EmptyObject *GetItem(unsigned int i) const;						//will return object[i] indexed from 0
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0);

	const void *operator [](int i)
	{
		return GetItem(i);
	}
	void SetMax(size_t m);			// set the number that can be made
	void SetSize(size_t n); // set the number currently in use (either grow or shrink)
	//  size_t GetSize() const { return CurrentCount; }
    int DeleteItem(unsigned int i);
    virtual void CopyTo(Array &Dest) const;  //copies contents of this to Dest
    void Sort(int (Compare) (const void *ele1,const void *ele2));

    virtual void CopyTo(Object &Dest) const;  //copies contents of this to Dest
    /// logically a virtual function since the elements contained cannot be known in advance (eg  save and load text functions)
    virtual Object *Dup()const = 0;

    virtual const cbyte* GetRaw() const
    {
        return (const cbyte*) &pPtr[0];
    }
#ifdef VALIDATING
    virtual bool Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool Fail));
#endif


	// if this class contains the property name, it will attempt to load it
	// if all is well returns true
	virtual bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);
	virtual bool SetPropertyAsObject(const PropertyParser &PropertyName,Object *Value);
	virtual bool SetPropertyAsObject(Property *Value);
	virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;
	virtual bool HasProperty(const PropertyParser &PropertyName)const;
	virtual int GetPropertyCount() const
	{
		return SimpleArray::GetPropertyCount() + 2;
	}
	virtual PropertyList* PropertyNames() const;
	virtual EmptyObject *GetAtIterator(const Iterator *I) const
	{
		int i = IteratorValue(I);
		return GetItem(i);
	}
	/// will set a value to the container[Iterator]
	virtual void SetItem(unsigned int i,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0);
	virtual bool LoadAsText(int i,String &FromStream) = 0;
	virtual bool SaveAsText(int i,String &ToStream) const = 0;
    virtual bool LoadAsText(Iterator *I,String &FromStream)
    {
        return SimpleArray::LoadAsText(I,FromStream);
    }
    virtual bool SaveAsText(Iterator *I,String &ToStream) const
    {
        return SimpleArray::SaveAsText(I,ToStream);
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
    virtual Object *Add(Object *Item)
    {
        return (Object *)Add(Item,true,false,0);
    }    // returns Item
    virtual Object *AddOwned(Object *Item)
    {
        return (Object *)Add(Item,true,true,0);
    }   // gives ownership to list
    EmptyObject *DupItem(const Array::ElePtr  *Node) const;
    virtual void SetItemOwnerShip(Iterator *I,bool Owned);
    virtual bool GetItemOwnerShip(const Iterator *I) const;
    virtual bool IsObject(const Iterator *I) const;
    virtual bool IsEmpty(const Iterator *I) const;
	virtual void SetIsObject(Iterator *I,bool IsCO);
    size_t GetItemSize(Iterator *I) const;
    virtual void GetEleType(String &Result) const;

	virtual void Clear();

};

/// an array of doubles
class DoubleArray : public SimpleArray
{
	double *Values;
	//int Size;
	// not used
	virtual void SaveItemTo(const Array *Owner,EmptyObject *FromItem,Stream &ToStream) const
		{}
	;
	virtual EmptyObject *LoadItemFrom(Array *Owner,EmptyObject *ToItem,Stream &FromStream)
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
	virtual Object *Add(Object *Item)
	{
		return Item;
	};
	virtual Object *AddOwned(Object *Item)
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
	virtual bool IsObject(const Iterator *I) const
	{
		return false;
	};
	virtual void GetEleType(String &Result) const;

public:
StdFunctionsNoDup(DoubleArray,SimpleArray);
	void SetSize(size_t _Size);
	virtual Object *Dup()const; // creates a duplicate of this object

	virtual FunctionDefList *GetFunctions(const char *Type=0) const;

	DoubleArray(int _Size=100) : SimpleArray(sizeof(double))
	{
		Values = new double[_Size];
		SetMax(_Size);
		SetSize(_Size);
	}
	~DoubleArray()
	{
		delete [] Values;
	}

	// always returns 0 (not creating a pointer to a double)
	virtual void RemoveAtIterator(Iterator *LI);
	/// will set a value to the container[Iterator]
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0);

	virtual void DestroyArrayItem(Array *Owner,EmptyObject *Item)
	{
		delete Item;
	};
	virtual EmptyObject *CreateArrayItem(Array *Owner,bool *IsObject)
	{
		*IsObject = false;
		double *d = new double;
		return (EmptyObject *)d;
	};
	virtual bool LoadAsText(int i,String &FromStream);
	virtual bool SaveAsText(int i,String &ToStream) const;
	virtual bool LoadAsText(Iterator *I,String &FromStream)
	{
		return LoadAsText(((ArrayIterator *)I)->i,FromStream);
	}
	virtual bool SaveAsText(Iterator *I,String &ToStream) const
	{
		return SaveAsText(((ArrayIterator *)I)->i,ToStream);
	}
	void SetItem(unsigned int i,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
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
	virtual bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);
	virtual bool SetPropertyAsObject(const PropertyParser &PropertyName,Object *Value);
	virtual bool SetPropertyAsObject(Property *Value)
	{
		return SimpleArray::SetPropertyAsObject(Value);
	}

	virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;
	virtual bool HasProperty(const PropertyParser &PropertyName)const;
	virtual int GetPropertyCount() const;
	virtual PropertyList* PropertyNames() const;
	virtual bool IsEmpty(const Iterator *I) const {  return ((ArrayIterator *)I)->i>CurrentCount; }
};


class CryIntegerArray : public TArray<int>
{
public:

};
class IntArray : public SimpleArray
{
int *Values;


//Array Length defines


//Local Variables Declaration


public:


//Constructor/Destructor Functions
	IntArray(int _SetSize=10);
	~IntArray();

	void SetSize(size_t _Size);

//Inherited Functions
virtual void DestroyArrayItem(Array *Owner,EmptyObject *Item);
virtual EmptyObject *CreateArrayItem(Array *Owner,bool *IsObject);
//virtual void SaveItemTo(const CryArray *Owner,EmptyObject *FromItem,CryStream &ToStream) const;
//virtual EmptyObject *LoadItemFrom(CryArray *Owner,EmptyObject *ToItem,CryStream &FromStream);
virtual bool LoadAsText(int i,String &FromStream);
virtual bool SaveAsText(int i,String &ToStream) const;
virtual void GetEleType(String &Result) const;
//virtual void DeleteIterator(Iterator *I) const;
//virtual bool GotoFirst(Iterator *I) const;
//virtual bool GotoLast(Iterator *Iterator) const;
virtual EmptyObject *GetAtIterator(const Iterator *I) const;
virtual PropertyList *PropertyNames() const;

virtual void CopyTo(Object &Dest) const;

virtual bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);
virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0);
virtual void SetItem(unsigned int i,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0);
virtual void RemoveAtIterator(Iterator *LI);
virtual void Clear();
virtual const char *GetProperty(const char *PropertyName,String &Result) const;

virtual EmptyObject *Add(EmptyObject *Item,size_t Size);
	virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size);
virtual Object *Add(Object *Item);    // returns Item
virtual Object *AddOwned(Object *Item);   // gives ownership to list
virtual void SetItemOwnerShip(Iterator *I,bool Owned);
virtual bool GetItemOwnerShip(const Iterator *I) const;
virtual bool IsObject(const Iterator *I) const;
	void SetValue(int i,int v)
	{
		Values[i] = v;
	}
	int GetValue(int i) const
	{
		if ((i<0) || ((unsigned )i>=CurrentCount))
			throw Exception(
			this,"Range check");
		return Values[i];
	}

//Common functions Macro
//Creates Dup, ClassName,ChildClassName,IsAbstract and IsA() Functions
	StdFunctions(IntArray,SimpleArray);


//Property Access Functions


//Class Footer
}; // class CryIntArray


} // end namespace Crystal
#endif //ARRAY_DEF
