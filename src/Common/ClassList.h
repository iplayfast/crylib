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
#ifndef _ClassList_
#define _ClassList_

#include "ClassMemStream.h"
#include "ClassException.h"
#include "ClassString.h"
#include <list>

/*namespace Crystal
{
class stlList : public OwnedObject
{
struct ListNode : public EmptyObject
{
	EmptyObject *Item;
	size_t Size;	// only used if Item is not an Object
	bool IsOwned;
	bool IsObject;
};
list <ListNode> TheList;
	void Test()
	{
	TheList.
	}

*/

namespace Crystal
{
using namespace std;
#ifndef CList
#define CList "List"
#define CList "List"

class PropertyList;


class List : public Container
{
//public:
struct ListNode : public EmptyObject
	{
		EmptyObject *Item;
		size_t Size;		// only used if Item is not a CryObject
		bool IsOwned;
		bool IsObject;
		~ListNode() {}
	};
public:
list <ListNode *> *Head;
typedef list<ListNode *>::iterator ListNodeI;
typedef list<ListNode *>::const_iterator cListNodeI;

struct ListIterator : public Iterator
	{
		cListNodeI p;
		ListIterator(const List *container) : Iterator(container) { p = container->Head->begin(); GotoFirst(); }
		virtual Object *Dup() const
		{
			ListIterator *LI = (ListIterator *)GetOrigContainer()->_CreateIterator();
			LI->p = p;
			return LI;
		}
	};
private:
	EmptyObject *Add(EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0)
	{
		ListNode *New = new ListNode;
		if (New==0)
			throw Exception(this,"Memory error\n");
		New->Item = Item;
		New->Size = Size;
		New->IsOwned = IsOwned;
		New->IsObject = IsObject;
		Head->push_back(New);
		return Item;
	}
	EmptyObject *DupItem(const ListNode *Node) const
	{
		if (Node->IsObject)
		{
			return (EmptyObject *)((Object *) Node->Item)->Dup();
		}
		else
		{
			char *temp = new char[Node->Size];
			EmptyObject *e = (EmptyObject *)temp;
			memcpy(temp,Node->Item,Node->Size);
			return e;
		}
	}
	void DeleteItem(struct ListNode *n)
	{
		if (n->IsOwned)
		{
			if (n->IsObject)
				delete((Object *)n->Item);
			else
			{
				char *e = (char *)n->Item;
				delete []e;
			}
		}
	}

	inline ListNode *_FirstNode() const
	{
	cListNodeI I;
		I = Head->begin();
		return *I;
	}
	ListNode *_LastNode() const
	{
		return *Head->end();
	}
	ListNode *_NextNode(const ListNode *n) const;
	bool _Remove(EmptyObject *Item)	// returns true if Item removed
	{
	ListNodeI p = Head->begin();
	cListNodeI e = Head->end();
	ListNode *ln;
		while(p!=e)
		{
			ln = *p;
				if (ln->Item==Item)
				{
					DeleteItem(ln);
					Head->erase(p);
					delete(ln);
					return true;
				}
				p++;
		}
		return false;

/*	ListNode *ln = _FindNode(Item);
			if (ln)
			{
				DeleteItem(ln);
				Head->remove(ln);
			}
			else return false;
			return true;
*/
	}
	ListNode *_FindNode(const EmptyObject *Needle) const
	{
	cListNodeI p = Head->begin();
	cListNodeI e = Head->end();
	ListNode *ln;
		while(p!=e)
		{
			ln = *p;
				if (ln->Item==Needle) return ln;
				p++;
		}
		return 0;
	}
	bool _IsEmpty() const { return Head->begin()==Head->end(); }
protected:
	void AddListNode (ListNode *Node);
public:
	StdFunctions(List,Container);
	List() { Head = new list <ListNode *>;}
	List(List &List);
	List(List *List);
	/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the Parent must be the appropriete container type or a derived class which can create the object (if the default class can't) */
	static Object *ClassCreate(const PropertyParser &PropertyName,Object *Parent);
	virtual Object *Create(const PropertyParser &PropertyName,Object *Parent)
	{
		return ClassCreate(PropertyName,Parent);
	}
	virtual Object *Create(Stream &e) { return Container::Create(e); }

	virtual void GetEleType(String &Result) const { Result = "List::ListNode"; }
	void SaveItemsTo(Stream &ToStream) const;
	virtual FunctionDefList *GetFunctions(const char *Type=0) const
	{

		if (Type && !IsA(Type))
			   return Container::GetFunctions(Type);
		FunctionDefList *l = Container::GetFunctions();
		return l;
	}
	virtual ~List()
	{
		Clear();
		delete Head;
	}
	virtual const cbyte* GetRaw() const
	{
		return (const cbyte *)(void *)&Head;
	}
	virtual Iterator *_CreateIterator() const { return new ListIterator(this); }
	ListIterator *CreateIterator() const { return (ListIterator *)_CreateIterator(); }
	virtual void DeleteIterator(Iterator *LI) const { delete LI; }
	void RemoveAtIterator(auto_ptr <ListIterator> li)
	{
		ListNodeI p = Head->begin();
		while(p != Head->end())
		{
			if (*li->p == *p)
			{
			ListNode *ln = *p;
				DeleteItem(ln);
				li->p = Head->erase(p);
				delete ln;
				return;
			}
		}
	}

	void RemoveAtIterator(Iterator *LI)
	{
		ListNodeI p = Head->begin();
		ListIterator *li = (ListIterator *)LI;
		while(p != Head->end())
		{
			if (*li->p == *p)
			{
			ListNode *ln = *p;
				DeleteItem(ln);
				li->p = Head->erase(p);
				delete ln;
				return;
			}
		}
	}
	virtual bool Sortable()const { return true; }
	virtual void Sort(int CompareType=0);
	virtual int Compare2(int CompareType,const EmptyObject *First,const EmptyObject *Second)const
	{
		ListNode *f,*s;
		f = (ListNode *)First;
		s = (ListNode *)Second;
		if (f->IsObject)
		{
			Object *F = (Object *)f->Item;
			if (s->IsObject)
			{
				Object *S = (Object *)s->Item;
				return Compare(CompareType,F,S);
			}
			return -1;  // EmptyObjects are always less then Objects
		}
		else
		{
			if (s->IsObject)
			{
				return 1; // EmptyObjects are alway less then Objects
			}
			if (f->Size==s->Size)
				return memcmp(f->Item,s->Item,f->Size);
			return f->Size - s->Size; // both empty objects
		}
	}
	virtual EmptyObject* GetAtIterator(const Iterator *I) const
	{
		ListIterator *pLI = (ListIterator *)I;
		ListNode *n = *pLI->p;
		return n->Item;
	}
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0)
	{
		ListIterator *pLI = (ListIterator *)I;
		if (pLI->p==0)	// nothing there!
			throw Exception("Attempt to SetAtIterator where Iterator is pointing to nothing");
		DeleteItem(*(pLI->p)); // only deletes if owned
		(*pLI->p)->IsObject = IsObject;
		(*pLI->p)->IsOwned = IsOwned;
		(*pLI->p)->Size = Size;
		(*pLI->p)->Item = Item;
	}

	virtual bool IsEmpty(const Iterator *I) const { return _IsEmpty(); };
	virtual bool GotoFirst(Iterator *LI) const
	{
		if (_IsEmpty()) return false;
		((ListIterator *)LI)->p = Head->begin();
		return true;
	}
	virtual bool GotoNext(Iterator *LI) const
	{
		if (_IsEmpty() || ((ListIterator *)LI)->p==Head->end()) return false;
		((ListIterator *)LI)->p++;
		if (((ListIterator *)LI)->p==Head->end()) return false;
		return true;
	}

	virtual bool GotoPrev(Iterator *LI) const
	{
		if (_IsEmpty() || ((ListIterator *)LI)->p==Head->begin()) return false;
		((ListIterator *)LI)->p--;
		return true;
	}

	virtual bool GotoLast(Iterator *LI) const
	{
		if (_IsEmpty()) return false;
		((ListIterator *)LI)->p = Head->end();
		return GotoPrev(LI);
	}

	virtual bool HasFirst(const Iterator *LI) const { return !_IsEmpty(); }
	virtual bool HasPrev(const Iterator *LI) const {  if (_IsEmpty() || ((ListIterator *)LI)->p==Head->begin()) return false;
												else return true; }

	virtual bool HasNext(const Iterator *LI) const {  if (_IsEmpty() || ((ListIterator *)LI)->p==Head->end()) return false;
												else return true; }
	virtual bool HasLast(const Iterator *I) const { return !_IsEmpty(); }
	virtual bool HasN(const Iterator *LI,int n) const { return Count()>n; }


	virtual inline size_t Count() const { return Head->size(); }
	virtual inline bool HasItems() const { return Count()>0; }
///copies contents of this to Dest
	virtual void CopyTo(Object &Dest) const
	{
		if (Dest.IsA(CList))
		{
		List *_Dest = (List *) &Dest;
		cListNodeI B = Head->begin();
		cListNodeI E = Head->end();
		while(B!=E)
		{
		const ListNode *p = *B++;
					if (p->IsOwned)
					{
						if (p->IsObject)
							_Dest->AddOwned((Object *)(DupItem(p)));
						else
							_Dest->AddOwned(DupItem(p),p->Size);
					}
					else
					{
						if (p->IsObject)
							_Dest->Add((Object *)p->Item );
						else
							_Dest->Add(p->Item,p->Size);
					}
		}
		return;
	}
	else
		throw Exception(this,"Copying from List to object that is not Listable");
	}
#ifdef VALIDATING
	bool Test(bool Verbose,Object &Object, bool (CallBack)(bool Verbose,const char *Result,bool Fail))
	{
	List l;

		l.AddOwned(new String("First"));
		l.AddOwned(new String("Second"));
		l.AddOwned(new String("Third"));
	ListIterator *li = l.CreateIterator();
	String Result;
	bool fail;

	try
	{
	String *s;
		li->GotoFirst();
		s = (String *)li->Get();
		fail = (*s!="Third");
		li->GotoNext();
		s = (String *)li->Get();
		fail |= (*s!="Second");
		li->GotoNext();
		s = (String *)li->Get();
		fail |= (*s!="First");
		l.Sort(0);
		li->GotoFirst();
		s = (String *)li->Get();
		fail |= (*s!="Third");
		li->GotoNext();
		s = (String *)li->Get();
		fail |= (*s!="Second");
		li->GotoNext();
		s = (String *)li->Get();
		fail |= (*s!="First");
		Result = "List sort test ";
		if (!CallBack(Verbose,Result,fail))
		{
			l.DeleteIterator(li);
			return false;
		}
		l.Sort(2);
		li->GotoFirst();
		s = (String *)li->Get();
		fail |= (*s!="First");
		li->GotoNext();
		s = (String *)li->Get();
		fail |= (*s!="Second");
		li->GotoNext();
		s = (String *)li->Get();
		fail = (*s!="Third");
		Result = "List reverse sort test ";
		l.DeleteIterator(li);
		}
		catch(Exception &e)
		{
			l.DeleteIterator(li);
		}
		if (!CallBack(Verbose,Result,fail))
		{
			return false;
		}
		return Container::Test(Verbose,Object,CallBack);
	}
#endif
	virtual void Clear()
	{
		ListNodeI I = Head->begin();
		ListNodeI E = Head->end();
		while(I!=E)
		{
		ListNode *p = *I;
//			Head->pop_front();
//			I = Head->begin();
			if (p->IsOwned)
			{
				if (p->IsObject)
				{
					Object *I = (Object *)p->Item;
					delete I;
				}
				else
					delete p->Item;
			}
			delete p;
			I++;
		}
		Head->clear();
	}
	const ListNode *FindNode(const EmptyObject *Needle) const;
	/// find a node who's item has the same "value" property
	int FindNodeValue(const MemStream &Needle) const;
	void RemoveNodeValue(const MemStream &Needle);
	const ListNode *FirstNode() const
	{
		return _FirstNode();
	}
	const ListNode *LastNode() const
	{
    	return _LastNode();
	}
	const ListNode *NextNode(const ListNode *n) const;
	EmptyObject *Add(EmptyObject *Item,size_t Size)
	{
		return Add(Item,false,false,Size);
	}
	void AddOwned(EmptyObject *Item,size_t Size)
	{
		Add(Item,false,true,Size);
	}
	Object *Add(Object *Item)    // returns Item
	{
		return (Object *) Add(Item,true,false);
	}
	void AddOwned(Object *Item)   // gives ownership to list
	{
		Add(Item,true,true);
	}
	void SetItemOwnerShip(EmptyObject  *Item,bool Owned)
	{
		ListNode *n = _FindNode(Item);
		if (n)
			n->IsOwned = Owned;
	}
	bool GetItemOwnerShip(const EmptyObject *Item) const
	{
		ListNode *n = _FindNode(Item);
		if (n)
			return n->IsOwned;
		return false;
	}
	bool IsObject(const Iterator *I) const
	{
		ListIterator *li = (ListIterator *)I;
		return (*li->p)->IsObject;
	}
	size_t GetItemSize(Iterator *I) const
	{
		ListIterator *li = (ListIterator *)I;
		return (*li->p)->Size;
	}
	bool SaveAsText(Iterator *I,String &ToStream) const
	{
		ToStream.Clear();
		ListIterator *li = (ListIterator *)I;
		if (_IsEmpty()) return false;
		if ((*li->p)->IsObject) {
		Object *t = (Object *)(*li->p)->Item;
			t->SaveTo(ToStream);
		}
		else
		{
			unsigned char *c = (unsigned char *)(*li->p)->Item;
			ToStream.printf("%d ",(*li->p)->Size);
			for(size_t i=0;i<(*li->p)->Size;i++)
				ToStream.printf("%d ",c[i]);
		}
		return true;
	}
	bool LoadAsText(Iterator *I,String &FromStream)
	{
		size_t Size;
		FromStream.scanf("%d ",&Size);
		char *Buffer = new char[Size];		// This might cause problems due to delete []Item needed for arrays.
		for(size_t i =0;i<Size;i++)
		{
			FromStream.scanf("%d ",&Buffer[i]);
		}
		this->DeleteItem((ListNode *)I);
		this->AddOwned((EmptyObject *)Buffer,Size);
		return true;
	}

	virtual void SetItemOwnerShip(Iterator  *I,bool Owned)
	{
		ListIterator *li = (ListIterator *)I;
		(*li->p)->IsOwned = Owned;
	}

	virtual bool GetItemOwnerShip(const Iterator *I) const
	{
		ListIterator *li = (ListIterator *)I;
		return (*li->p)->IsOwned;
	}
	bool InList(Object *Needle) const
	{
		return _FindNode(Needle)!=0;
	}
	EmptyObject *GetItem(int i) const
	{
	cListNodeI li = Head->begin();
		while(i && li!=Head->end())
		{
			i--;
			li++;
		}
		if (i==0)
		{
			return (*li)->Item;
		}
		return 0;
	}
	virtual bool IsContainer() const { return true; }
/// throws exception if Item not found
	void Remove(EmptyObject *Item)
	{
	if (! _Remove(Item))
		throw Exception(this,0,(const char *)"Item Not found in list");
	}
	void Remove(Object*Item)
	{
	if (! _Remove(Item))
		throw Exception(this,0,Item->ChildClassName(),(const char *)"Not found in list");
	}

	void Copy(List *List);
	// if this class contains the property name, it will attempt to load it
	// if all is well returns true
	//virtual bool SetProperty(CryString *PropertyName,CryString *PropertyValue);
	virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;

	virtual bool HasProperty(const PropertyParser &PropertyName) const
	{
		return Container::HasProperty(PropertyName);
	}
	virtual int GetPropertyCount() const
	{
		return Container::GetPropertyCount() + 1;
	}
	virtual PropertyList* PropertyNames() const
	{
		PropertyList *n = Container::PropertyNames();
		return n;
	}
	virtual size_t Size() const
	{
		size_t Result = sizeof(int);// for storing count
		cListNodeI B = Head->begin();
		cListNodeI E = Head->end();
		while(B!=E)
		{
		const ListNode *p = *B++;
				if (p->IsObject)
					Result += ((Object *)p->Item)->Size();
				else
					Result += p->Size;
		}
		return Result;
	}
	void SwapListElements(List *ToSwap);

}
;  // CryList
/*class List : public Container
{

//public:
struct ListNode : public EmptyObject
	{
		ListNode *Next;
		EmptyObject *Item;
		size_t Size;		// only used if Item is not a CryObject
		bool IsOwned;
		bool IsObject;
	};
public:

struct ListIterator : public Iterator
	{
		ListNode *p;
		ListIterator(const List *container);
		virtual Object *Dup() const;
	};
private:

	ListNode *Head;
	ListNode *Tail;
	EmptyObject *Add(EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0);
	EmptyObject *DupItem(const ListNode *Node) const;
	void DeleteItem(struct ListNode *n);
	ListNode *_FirstNode() const;
	ListNode *_LastNode() const;
	ListNode *_NextNode(const ListNode *n) const;
	bool _Remove(EmptyObject *Item);	// returns true if Item removed
	ListNode *_FindNode(const EmptyObject *Needle) const;

protected:
	void AddListNode (ListNode *Node);
public:
	StdFunctions(List,Container);
	List();
	List(List &List);
	List(List *List);

	virtual void GetEleType(String &Result) const;
	void SaveItemsTo(Stream &ToStream) const;
	virtual FunctionDefList *GetFunctions(const char *Type=0) const;
	virtual ~List();
	virtual const cbyte* GetRaw() const;
	virtual Iterator *_CreateIterator() const;
	ListIterator *CreateIterator() const { return (ListIterator *)_CreateIterator(); }
	virtual void DeleteIterator(Iterator *LI) const;
	virtual bool GotoFirst(Iterator *LI) const;
	/// avoid this function it's slower since it must search from the beginning each time
	virtual bool GotoPrev(Iterator *LI) const;
	void RemoveAtIterator(Iterator *LI);
	virtual bool GotoNext(Iterator *LI) const;
	virtual bool Sortable()const { return true; }
	virtual void Sort(int CompareType=0);
	virtual int Compare2(int CompareType,const EmptyObject *First,const EmptyObject *Second)const;
	virtual EmptyObject* GetAtIterator(const Iterator *I) const;
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0);

	virtual bool IsEmpty(const Iterator *I) const;
	virtual bool GotoLast(Iterator *LI) const;
	virtual size_t Count() const;
	virtual bool HasItems() const;
	virtual void Clear();
	const ListNode *FindNode(const EmptyObject *Needle) const;
	/// find a node who's item has the same "value" property
	int FindNodeValue(const MemStream &Needle) const;
	void RemoveNodeValue(const MemStream &Needle);
	const ListNode *FirstNode() const;
	const ListNode *LastNode() const;
	const ListNode *NextNode(const ListNode *n) const;
	EmptyObject *Add(EmptyObject *Item,size_t Size);
	void AddOwned(EmptyObject *Item,size_t Size);
	Object *Add(Object *Item);    // returns Item
	void AddOwned(Object *Item);   // gives ownership to list
	void SetItemOwnerShip(EmptyObject  *Item,bool Owned);
	bool GetItemOwnerShip(const EmptyObject *Item) const;
	bool IsObject(const Iterator *I) const;
	size_t GetItemSize(Iterator *I) const;
	bool LoadAsText(Iterator *I,String &FromStream) ;
	bool SaveAsText(Iterator *I,String &ToStream) const;

	virtual void SetItemOwnerShip(Iterator  *I,bool Owned);
	virtual bool GetItemOwnerShip(const Iterator *I) const;
	bool InList(Object *Needle) const;
	EmptyObject *GetItem(int i) const;
	virtual bool IsContainer() const { return true; }
/// throws exception if Item not found
	void Remove(EmptyObject *Item);
	void Remove(Object*Item);

	void Copy(List *List);
	// if this class contains the property name, it will attempt to load it
	// if all is well returns true
	//virtual bool SetProperty(CryString *PropertyName,CryString *PropertyValue);
	virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;
	virtual bool HasProperty(const PropertyParser &PropertyName) const;
	virtual int GetPropertyCount() const;
	virtual PropertyList* PropertyNames() const;
///copies contents of this to Dest
	virtual void CopyTo(Object &Dest) const;
	virtual size_t Size() const;
	void SwapListElements(List *ToSwap);

}*/
;  // CryList
#endif // TCryList
}
#endif // _ClassList_

