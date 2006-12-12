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
#include "ClassException.h"
#include "ClassString.h"
#include "ClassFunction.h"
#include "ClassProperty.h"

using namespace Crystal;
							/* TODO : Implement End-of-Link type link list */
///----------------------------------------------------------------------------
// CryList
///----------------------------------------------------------------------------

List::ListIterator::ListIterator(const List *container) : Iterator(container)
{
    GotoFirst();
}

/// creates a duplicate of this object
Object *List::ListIterator::Dup() const
{
	ListIterator *LI = (ListIterator *)GetOrigContainer()->_CreateIterator();
    LI->p = p;
    return LI;
}

List::List()
{
	Head = Tail = 0;
}

void List::GetEleType(String &Result) const
{
	Result = "CryList::ListNode";
}

const cbyte* List::GetRaw() const
{
	return (const cbyte *)Head;
}

Container::Iterator *List::_CreateIterator() const
{
	ListIterator *LI = new ListIterator(this);
	LI->p = Head;
	return LI;
}

void List::DeleteIterator(Iterator *LI) const
{
	delete (ListIterator *)LI;
}

bool List::GotoFirst(Iterator *LI) const
{
	if (Head)
	{
		((ListIterator *)LI) ->p = Head;
		return true;
	}
	return false;
}
bool List::GotoNext(Iterator *LI) const
{
	ListIterator *pLI = (ListIterator *)LI;
	if (Head && pLI->p)
	{
		pLI->p = pLI->p->Next;
		return pLI->p != 0;
	}
	return false;
}

EmptyObject* List::GetAtIterator(const Iterator *I) const
{
	ListIterator *pLI = (ListIterator *)I;
	if (pLI->p==0)	// nothing there!
		throw Exception("Attempt to GetAtIterator where Iterator is pointing to nothing");
	return pLI->p->Item;
}
void List::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
{
	ListIterator *pLI = (ListIterator *)I;
	if (pLI->p==0)	// nothing there!
		throw Exception("Attempt to SetAtIterator where Iterator is pointing to nothing");
	DeleteItem(pLI->p); // only deletes if owned
	pLI->p->IsObject = IsObject;
	pLI->p->IsOwned = IsOwned;
	pLI->p->Size = Size;
	pLI->p->Item = Item;
}

bool List::IsEmpty(const Iterator *I) const
{
    ListIterator *pLI = (ListIterator *)I;
    return (pLI->p==0);// nothing there!
}

bool List::HasItems() const
{
    return (Head!=0);
}

const List::ListNode *List::FirstNode() const
{
	return Head;
}

List::ListNode *List::_FirstNode() const
{
	return Head;
}


const List::ListNode *List::LastNode() const
{
	return Tail;
}

List::ListNode *List::_LastNode() const
{
	return Tail;
}

const List::ListNode *List::NextNode(const ListNode *n) const
{
	return n->Next;
}
List::ListNode *List::_NextNode(const ListNode *n) const
{
	return n->Next;
}

bool List::Sortable()const
{
	return true;
}
bool List::IsContainer() const
{
	return true;
}

EmptyObject *List::DupItem(const ListNode *Node) const
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
bool List::IsObject(const Iterator *I) const
{
	ListIterator *li = (ListIterator *)I;
	if (Head)
	{
		if (li->p==0)
			li->p = Head;
		return li->p->IsObject;
	}
	else
		return false;
}
size_t List::GetItemSize(Iterator *I) const
{
	ListIterator *li = (ListIterator *)I;
	if (Head)
	{
		if (li->p==0)
			li->p = Head;
		return li->p->Size;
	}
	else
		return 0;
}

FunctionDefList *List::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return Container::GetFunctions(Type);
	// otherwise get any functions in subclasses
	FunctionDefList *l = Container::GetFunctions();
	String s;
	s += "// Class CryList;";
	s += "virtual void GetEleType(CryString &Result) const;";
	s += "void SaveItemsTo(CryStream &ToStream) const;";
	s += "virtual CryString *GetFunctions() const;";
	s += "virtual const cbyte* GetRaw() const;";
	s += "virtual Iterator *_CreateIterator() const;";
	s += "virtual void DeleteIterator(Iterator *LI) const;";
	s += "virtual bool GotoFirst(Iterator *LI) const;";
	s += "virtual bool GotoPrev(Iterator *LI) const;";
	s += "virtual bool GotoNext(Iterator *LI) const;";
	s += "virtual EmptyObject* GetAtIterator(const Iterator *I) const;";
	s += "virtual bool GotoLast(Iterator *LI) const;";
	s += "virtual size_t Count() const;";
	s += "virtual bool HasItems() const;";
	s += "virtual void Clear();";
	s += "const char* ClassName() const;";
	s += "virtual const char *ChildClassName() const;";
	s += "virtual bool IsA(const char *ClassName) const;";
	s += "ListNode *FindNode(const EmptyObject *Needle) const;";
	s += "int FindNodeValue(CryMemStream &Needle) const;";
	s += "const ListNode *FirstNode() const;";
	s += "const ListNode *NextNode(const ListNode *n) const;";
	s += "EmptyObject *Add(EmptyObject *Item,size_t Size);";
	s += "EmptyObject *AddOwned(EmptyObject *Item,size_t Size);";
	s += "CryObject *Add(CryObject *Item);";
	s += "CryObject *AddOwned(CryObject *Item);";
	s += "void SetItemOwnerShip(EmptyObject  *Item,bool Owned);";
	s += "bool GetItemOwnerShip(const EmptyObject *Item) const;";
	s += "bool IsObject(const Iterator *I) const;";
	s += "size_t GetItemSize(Iterator *I) const;";
	s += "bool LoadAsText(Iterator *I,CryString &FromStream);";
	s += "bool SaveAsText(Iterator *I,CryString &ToStream) const;";
	s += "void SetItemOwnerShip(Iterator  *I,bool Owned);";
	s += "bool GetItemOwnerShip(const Iterator *I) const;";
	s += "bool InList(CryObject *Needle) const;";
	s += "EmptyObject *GetItem(int i) const;";
	s += "virtual bool IsContainer() const;";
	s += "void Remove(CryObject *Item);";
	s += "void RemoveNodeValue(CryMemStream &Needle);";
	s += "CryObject *Dup() const;";
	s += "void Copy(CryList *List);";
	s += "virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;";
	s += "virtual bool HasProperty(const char *PropertyName) const;";
	s += "virtual int GetPropertyCount() const;";
	s += "virtual CryList *PropertyNames() const;";
	s += "virtual void CopyTo(CryObject &Dest) const;";
	s += "virtual size_t Size() const;";
	l->LoadFromString(s,";");
	return l;
}

bool List::LoadAsText(Iterator *I,String &FromStream)
{
	size_t Size;
	FromStream.scanf("%d ",&Size);
	char *Buffer = new char[Size];		// This might cause problems due to delete []Item needed for arrays.
	for(size_t i =0;i<Size;i++)
	{
		FromStream.scanf("%d ",&Buffer[i]);
	}
	ListIterator *li = (ListIterator *)I;
	if (li->p)
	{
		DeleteItem(li->p);
		li->p->Item = 0;
	}
	else
	{
		li->p = new ListNode();
		AddListNode(li->p);
	}
	li->p->IsObject = false;
	li->p->IsOwned = true;
	li->p->Item = (EmptyObject *) Buffer;
	li->p->Size = Size;
	return true;
}
bool List::SaveAsText(Iterator *I,String &ToStream) const
{
	ToStream.Clear();
	ListIterator *li = (ListIterator *)I;
	if (Head)
	{
		if (li->p==0)
			li->p = Head;
	}
	else
		return false;
	if (li->p->IsObject) {
	Object *t = (Object *)li->p->Item;
		t->SaveTo(ToStream);
	}
	else
	{
		unsigned char *c = (unsigned char *)li->p->Item;
		ToStream.printf("%d ",li->p->Size);
		for(size_t i=0;i<li->p->Size;i++)
			ToStream.printf("%d ",c[i]);
	}
	return true;

}

bool List::GotoLast(Iterator *LI) const
{
	ListIterator *li = (ListIterator *)LI;
	li->p = Tail;
	return Tail!=0;
}
bool List::GotoPrev(Iterator *LI) const
{
	ListIterator *li = (ListIterator *)LI;
	if (Head)						// something there?
	{
		if ((li->p == 0) ||				// not pointing at anything
			(li->p==Head)) 				// already at start of list
			return false;
		ListNode *n = Head;		//Point to first in list
		do
		{
			if (n->Next == li->p)	// Check next pointer
			{
				li->p = n;
				return true;
			}
			n = n->Next;
		}
		while(n);
		throw Exception(this,"Iterator not from this list");
	}
	return false;
}
void List::SetItemOwnerShip(Iterator *I,bool IsOwned)
{
	ListIterator *li = (ListIterator *)I;
	li->p->IsOwned = IsOwned;
}
bool List::GetItemOwnerShip(const Iterator *I) const
{
	ListIterator *li = (ListIterator *)I;
	return  li->p->IsOwned;
}

void List::RemoveAtIterator(Iterator *LI)
{
	GotoPrev(LI);
	ListIterator *li = (ListIterator *)LI;
	ListNode *ToDelete,*PrevNode = li->p;
	ToDelete = PrevNode->Next;
	PrevNode->Next = ToDelete->Next;
	DeleteItem(ToDelete); // only deletes Items if owned
	delete ToDelete;
	return;
}
List::List(List &_List) : Container()
{
	const ListNode *p = _List.FirstNode();
	const ListNode *pl = _List.LastNode();
	Head = Tail = 0;
	while(p)
	{
		if (p->IsOwned)
			AddOwned(DupItem(p),p->Size);
		else
			Add(DupItem(p),p->Size);
		if (p==pl) break;
		p = p->Next;
	}
}

List::~List()
{
	const ListNode *n,*p = Head;
	Head = Tail = 0;
	while(p)
	{
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
		n = p->Next;
		delete p;
		p = n;
	}
};

List::List(List *_List) : Container()
{
	const ListNode *p = _List->FirstNode();

	Head = Tail = 0;
	while(p)
	{
		if (p->IsOwned)
			AddOwned(DupItem(p),p->Size);
		else
			Add(DupItem(p),p->Size);
		p = p->Next;
    }
}

void List::AddListNode(ListNode *Node)
{
	if (Tail==0) {
		Tail = Head = Node;
		Node->Next = 0;
		return;
	}
	else
	{
	// Fifo version
	Node->Next = 0;
	ListNode *p = Head;	// add on to the tail of the list
		while(p->Next)
			p = p->Next;
		p->Next = Node;
		Tail = Node;
	// LiFo version
	//	Node->Next = Head;
	//	Head = Node;
	}
}

void List::DeleteItem(struct ListNode *p)
{
	if (p->IsOwned)
	{
		if (p->IsObject)
			delete((Object *)p->Item);
		else
		{
			char *e = (char *)p->Item;
			delete []e;
		}
	}
}

EmptyObject *List::Add(EmptyObject *_Item,bool IsObject,bool IsOwned,size_t _Size)
{
	ListNode *New = new ListNode;
	if (New==0)
		throw Exception(this,"Memory error\n");
	New->Item = _Item;
	New->Size = _Size;
	New->Next = 0;
	New->IsOwned = IsOwned;
	New->IsObject = IsObject;
	AddListNode(New);
	return _Item;
}
Object *List::Add(Object *_Item)
{
	Add(_Item,true,false);
	return _Item;
}
EmptyObject *List::Add(EmptyObject *_Item,size_t _Size)
{
	return Add(_Item,false,false,_Size);
};
Object *List::AddOwned(Object *_Item)   // gives ownership to list
{
	Add(_Item,true,true);
	return _Item;
}
EmptyObject *List::AddOwned(EmptyObject *_Item,size_t _Size)   // gives ownership to list
{
	return Add(_Item,false,true,_Size);
}

bool List::_Remove(EmptyObject *_Item)
{
ListNode *Prev,*p = Head;
	if (Head)
	{
		Prev = p;
		if (Head->Item == _Item) {
			Head = Head->Next;
			p->Next = 0;
			DeleteItem(p);
			delete p;
			return true;
		}
		p = p->Next;
		if (p)
		{
			do
			{
				if (p->Item == _Item)
				{
					Prev->Next = p->Next;
					p->Next = 0;
					DeleteItem(p);
					delete p;
					return true;
				}
				Prev = p;
				p=p->Next;
			} while(p);
		}
	}
	return false;
}
void List::Remove(EmptyObject *_Item)
{
	if (! _Remove(_Item))
		throw Exception(this,0,(const char *)"Item Not found in list");
}
void List::Remove(Object*_Item)
{
	if (! _Remove(_Item))
		throw Exception(this,0,_Item->ChildClassName(),(const char *)"Not found in list");
}


void List::SetItemOwnerShip(EmptyObject *Item,bool Owned)
{
	ListNode *n = _FindNode(Item);
	n->IsOwned = Owned;
}
bool List::GetItemOwnerShip(const EmptyObject *Item) const
{
	ListNode *n = _FindNode(Item);
    return n->IsOwned;
}

void List::Clear()
{
	while(Head)
	{
		ListNode *p = Head;
		Head= p->Next;
		DeleteItem(p);
		delete p;
	}
	Tail = 0;
}

void List::SwapListElements(List *ToSwap)
{
	ListNode *n = Head;
				Head = ToSwap->Head;
				ToSwap->Head = n;
				n = Tail;
				Tail = ToSwap->Tail;
				ToSwap->Tail = n;
}

size_t List::Count() const
{
	size_t i = 0;
	if (Head)
	{
	const ListNode *f = Head;
		i++;
		while(f)
		{
			f = f->Next;
			i++;
		}
	}
	return i;
}

void List::SaveItemsTo(Stream &ToStream) const
{
	const ListNode *f = FirstNode();
	if (f)
	{
		StreamMode Mode = ToStream.GetMode();
		do
		{
			if (f->IsObject)
				((Object *)f->Item)->SaveTo(ToStream);
			else
			{
				ToStream.printf("%d ",f->Size);
				char *e = (char *)f->Item;
				for(size_t i=0;i<f->Size;i++)
					ToStream.printf("%d ",e[i]);
			}
			f = f->Next;
			if (Mode==SText)
				ToStream.printf("|");
		} while(f);
	}
}

bool List::InList(Object *Needle) const
{
	if (FindNode(Needle))
		return true;
	return false;
}

const List::ListNode *List::FindNode(const EmptyObject *Needle) const
{
	const ListNode *p = FirstNode();
	if (p)
	{
		do
		{
			if (p->Item==Needle)
				return p;
			p=p->Next;
		} while(p);
	}
	return 0;
}

List::ListNode *List::_FindNode(const EmptyObject *Needle) const
{
	return (List::ListNode*) FindNode(Needle);
}

void List::RemoveNodeValue(const MemStream &Needle)   // find a node who's item has the same "value" property
{

	ListNode *Prev,*p = _FirstNode();
	if (p)
	{
		Prev = p;
		size_t nl = Needle.GetLength();
		MemStream *t;
		do
		{
			if ((p->IsObject) && (((Object *)p->Item)->IsA(CMemStream)))
			{
				t = (MemStream *) p->Item;
				if ((t->GetLength()==nl) && (*t==Needle))
				{
					if (p==Head)	// is first node?
					{
						if (Head==Tail)	// single item on list
							Tail = Head=0;
						else
							Head = Head->Next;	// update Items to next to last
					}
					Prev->Next = p->Next;
					p->Next = 0;
					DeleteItem(p);
					delete p;
				}
			}
			Prev = p;
			p=p->Next;
		} while(p);
	}
	return;
}

/// return the index into the list of a value
int List::FindNodeValue(const MemStream &Needle) const   // find a node who's item has the same "value" property
{
int i=0;
	const ListNode *p = FirstNode();
	if (p)
	{
		size_t nl = Needle.GetLength();
		MemStream *t;
		do
		{
			if (p->IsObject)
			{

				if (((Object *)p->Item)->IsA(CMemStream))
				{
					t = (MemStream *) p->Item;
					if ((t->GetLength()==nl) && (*t==Needle))
						return i;
				}
				else
				{
					if (0==Compare(0,(Object *)p->Item,&Needle))
						return i;
				}
			}
			i++;
			p=p->Next;
		} while(p);
	}
	return -1;
}

EmptyObject *List::GetItem(int i) const
{
	const ListNode *p = FirstNode();
	if (p)
	{
		do
		{
			p = p->Next;
			i--;
		} while(i && p);
	}
	if (p)
		return p->Item;
	else
		throw Exception(this,"Item Not found");
}

void List::CopyTo(Object &Dest) const //copies contents of this to Dest
{
	if (Dest.IsA(CList))
	{
		List *_Dest = (List *) &Dest;
		const ListNode *p = FirstNode();
		if (p)
		{
			do
			{
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
				p = p->Next;
			} while(p);
		}
		return;
	}
	else
		throw Exception(this,"Copying from List to object that is not Listable");
}

size_t List::Size() const
{
	size_t Result = sizeof(int);// for storing count
	const ListNode *p = FirstNode();
	if (p)
	{
		do
		{
			if (p->IsObject)
				Result += ((Object *)p->Item)->Size();
			else
				Result += p->Size;
			p = p->Next;
		} while(p);
	}
	return Result;
}

int List::GetPropertyCount() const
{
	return Container::GetPropertyCount() + 1;
}

bool List::HasProperty(const PropertyParser &PropertyName) const
{
	return Container::HasProperty(PropertyName);
}

const char *List::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	Result.Clear();
	if (PropertyName=="Values")   // intercept crycontainer's property for our own
	{
		Result = "[]";  // if Result != what is returned, it's a special situation
		return "*";
	}
	else
		return Container::GetProperty(PropertyName,Result);
}

CryPropertyList *List::PropertyNames() const
{
	CryPropertyList *n = Container::PropertyNames();
	return n;
}/*
 * This is the actual sort function. Notice that it returns the new
 * head of the list. (It has to, because the head will not
 * generally be the same element after the sort.) So unlike sorting
 * an array, where you can do
 *
 *     sort(myarray);
 *
 * you now have to do
 *
 *     list = listsort(mylist);
 */
void List::Sort(int CompareType)
{
	ListNode *p, *q, *e, *tail, *oldhead;
	int insize, nmerges, psize, qsize, i;

	if (!Head)
		return;

	insize = 1;

	while (1)
	{
		p = Head;
		Head = NULL;
		Tail = NULL;

		nmerges = 0;  /* count number of merges we do in this pass */

		while (p)
		{
			nmerges++;  /* there exists a merge to be done */
			/* step `insize' places along from p */
			q = p;
			psize = 0;
			for (i = 0; (i < insize) && q; i++)
			{
				psize++;
				q = q->Next;
			}

			// if q hasn't fallen off end, we have two lists to merge
			qsize = insize;

			// now we have two lists; merge them
			while ((psize > 0) || (qsize > 0 && q))
			{
				// decide whether next element of merge comes from p or q
				if (psize == 0)
				{
					e = q; q = q->Next; qsize--;// p is empty; e must come from q.
				}
				else
				if (qsize == 0 || !q)
				{
					e = p; p = p->Next; psize--;  // q is empty; e must come from p.

				}
				else
				{
				int cr;
					if (p->IsObject && q->IsObject)
						cr = Compare(CompareType,(Object *)p->Item,(Object *)q->Item);
					else
						cr = Compare2(CompareType,p->Item,q->Item);
					if (cr <= 0)
					{
						e = p; p = p->Next; psize--;// First element of p is lower (or same) e must come from p.
					}
					else
					{
						e = q; q = q->Next; qsize--;// First element of q is lower; e must come from q.
					}
				}

				// add the next element to the merged list
				if (Tail)
					Tail->Next = e;
				else
					Head = e;
				Tail = e;
			}
		// now p has stepped `insize' places along, and q has too
		p = q;
		} // while
		Tail->Next = NULL;

		// If we have done only one merge, we're finished.
		if (nmerges <= 1)   // allow for nmerges==0, the empty list case
		{
		Tail = Head;
			while(Tail->Next)
				Tail = Tail->Next;
			return;
		}
		// Otherwise repeat, merging lists twice the size
		insize *= 2;
	}
}


int List::Compare2(int CompareType,const EmptyObject *First,const EmptyObject *Second) const
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
//            return F->CompareLogical(CompareType,S);
		}
		return -1;  // EmptyObjects are always less then CryObjects
	}
	else
	{
		if (s->IsObject)
		{
			return 1; // EmptyObjects are alway less then CryObjects
		}
		if (f->Size==s->Size)
			return memcmp(f->Item,s->Item,f->Size);
		return f->Size - s->Size; // both empty objects
	}
}

