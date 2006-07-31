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

CryList::ListIterator::ListIterator(const CryList *container) : Iterator(container)
{
    GotoFirst();
}

/// creates a duplicate of this object
CryObject *CryList::ListIterator::Dup() const
{
	ListIterator *LI = (ListIterator *)GetOrigContainer()->_CreateIterator();
    LI->p = p;
    return LI;
}

CryList::CryList()
{
    Items = 0;
}

void CryList::GetEleType(CryString &Result) const
{
    Result = "CryList::ListNode";
}

const cbyte* CryList::GetRaw() const
{
    return (const cbyte *)Items;
}

CryContainer::Iterator *CryList::_CreateIterator() const
{
    ListIterator *LI = new ListIterator(this);
    LI->p = Items;
    return LI;
}

void CryList::DeleteIterator(Iterator *LI) const
{
    delete (ListIterator *)LI;
}

bool CryList::GotoFirst(Iterator *LI) const
{
	if (Items)
	{
		((ListIterator *)LI) ->p = Items->Next;
		return true;
	}
    return false;
}
bool CryList::GotoNext(Iterator *LI) const
{
    ListIterator *pLI = (ListIterator *)LI;
	if (Items && pLI->p && pLI->p!=Items)// if list is deleted this might be a problem
    {
        pLI->p = pLI->p->Next;
        return pLI->p != 0;
    }
    return false;
}

EmptyObject* CryList::GetAtIterator(const Iterator *I) const
{
	ListIterator *pLI = (ListIterator *)I;
	if (pLI->p==0)	// nothing there!
		throw CryException("Attempt to GetAtIterator where Iterator is pointing to nothing");
	return pLI->p->Item;
}
void CryList::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size) 
{
	ListIterator *pLI = (ListIterator *)I;
	if (pLI->p==0)	// nothing there!
		throw CryException("Attempt to SetAtIterator where Iterator is pointing to nothing");
	DeleteItem(pLI->p); // only deletes if owned
	pLI->p->IsCryObject = IsCryObject;
	pLI->p->IsOwned = IsOwned;
	pLI->p->Size = Size;
	pLI->p->Item = Item;
}

bool CryList::IsEmpty(const Iterator *I) const
{
    ListIterator *pLI = (ListIterator *)I;
    return (pLI->p==0);// nothing there!
}

bool CryList::HasItems() const
{
    return (Items!=0);
}

const CryList::ListNode *CryList::FirstNode() const
{
	if (Items)
		return Items->Next;
	else
		return 0;
}

CryList::ListNode *CryList::_FirstNode() const
{
	if (Items)
		return Items->Next;
	else
		return 0;
}


const CryList::ListNode *CryList::LastNode() const
{
	return Items;
}

CryList::ListNode *CryList::_LastNode() const
{
	return Items;
}

const CryList::ListNode *CryList::NextNode(const ListNode *n) const
{
	if (n->Next!=Items)	// not at end of list
		return n->Next;
	else
		return 0;
}
CryList::ListNode *CryList::_NextNode(const ListNode *n) const
{
	if (n->Next!=Items)	// not at end of list
		return n->Next;
	else
		return 0;
}

bool CryList::Sortable()const
{
	return true;
}
bool CryList::IsContainer() const
{
	return true;
}

EmptyObject *CryList::DupItem(const ListNode *Node) const
{
	if (Node->IsCryObject)
	{
		return (EmptyObject *)((CryObject *) Node->Item)->Dup();
	}
	else
	{
		char *temp = new char[Node->Size];
		EmptyObject *e = (EmptyObject *)temp;
		memcpy(temp,Node->Item,Node->Size);
		return e;
	}
}
bool CryList::IsCryObject(const Iterator *I) const
{
	ListIterator *li = (ListIterator *)I;
	if (Items)
	{
		if (li->p==0)
			li->p = Items;
		return li->p->IsCryObject;
	}
	else
		return false;
}
size_t CryList::GetItemSize(Iterator *I) const
{
	ListIterator *li = (ListIterator *)I;
	if (Items)
	{
		if (li->p==0)
			li->p = Items;
		return li->p->Size;
	}
	else
		return 0;
}

CryFunctionDefList *CryList::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return CryContainer::GetFunctions(Type);
	// otherwise get any functions in subclasses
	CryFunctionDefList *l = CryContainer::GetFunctions();
	CryString s;
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
	s += "bool IsCryObject(const Iterator *I) const;";
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

bool CryList::LoadAsText(Iterator *I,CryString &FromStream)
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
	li->p->IsCryObject = false;
	li->p->IsOwned = true;
	li->p->Item = (EmptyObject *) Buffer;
	li->p->Size = Size;
	return true;
}
bool CryList::SaveAsText(Iterator *I,CryString &ToStream) const
{
	ToStream.Clear();
	ListIterator *li = (ListIterator *)I;
	if (Items)
	{
		if (li->p==0)
			li->p = Items;
    }
    else
        return false;

    unsigned char *c = (unsigned char *)li->p->Item;
    ToStream.printf("%d ",li->p->Size);
    for(size_t i=0;i<li->p->Size;i++)
        ToStream.printf("%d ",c[i]);
    return true;

}

bool CryList::GotoLast(Iterator *LI) const
{
	ListIterator *li = (ListIterator *)LI;
	li->p = Items;
	return Items!=0;
}
bool CryList::GotoPrev(Iterator *LI) const
{
	ListIterator *li = (ListIterator *)LI;
	if (Items)						// something there?
	{
		if (li->p == 0)				// not pointing at anything
			return false;
		ListNode *n = Items;		//Point to last in list
		do
		{
			if (n->Next == li->p)	// Check next pointer
			{
				li->p = n;
				return true;
			}
			n = n->Next;
		}
		while(n!=Items);
		return false;
	}
	else
		return false;
}
void CryList::SetItemOwnerShip(Iterator *I,bool IsOwned)
{
	ListIterator *li = (ListIterator *)I;
	li->p->IsOwned = IsOwned;
}
bool CryList::GetItemOwnerShip(const Iterator *I) const
{
	ListIterator *li = (ListIterator *)I;
	return  li->p->IsOwned;
}

void CryList::RemoveAtIterator(Iterator *LI)
{
	GotoPrev(LI);
	ListIterator *li = (ListIterator *)LI;
	ListNode *ToDelete,*p = li->p;
	EmptyObject *e;
	ToDelete = p->Next;
	p->Next = ToDelete->Next;
	e = ToDelete->Item;
	if (ToDelete->IsOwned)
	{
		DeleteItem(ToDelete); // only deletes if owned
	}
	delete ToDelete;
	return;
}
CryList::CryList(CryList &_List) : CryContainer()
{
	const ListNode *p = _List.FirstNode();
	const ListNode *pl = _List.LastNode();
	Items = 0;
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

CryList::~CryList()
{
	const ListNode *n,*p = FirstNode();
	if (Items)
		Items->Next = 0;		// break the chain
	Items = 0;
	while(p)
	{
		if (p->IsOwned)
		{
			if (p->IsCryObject)
			{
				CryObject *I = (CryObject *)p->Item;
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

CryList::CryList(CryList *_List) : CryContainer()
{
	const ListNode *p = _List->FirstNode();
	const ListNode *pl = _List->LastNode();

	Items = 0;
	while(p)
	{
		if (p->IsOwned)
			AddOwned(DupItem(p),p->Size);
		else
			Add(DupItem(p),p->Size);
		if (p == pl) break;
		p = p->Next;
    }
}

void CryList::AddListNode(ListNode *Node)
{
	if (Items==0)
	{
		Items = Node;
		Items->Next = Items;
	}
	else
	{
	ListNode *last =  Items;
		Node->Next = last->Next;	// pointer to first node
		last->Next = Node;			// Node is new last
		Items = Node;
	}
}

void CryList::DeleteItem(struct ListNode *p)
{
	if (p->IsOwned)
	{
		if (p->IsCryObject)
			delete((CryObject *)p->Item);
		else
		{
			char *e = (char *)p->Item;
			delete []e;
		}
	}
}

EmptyObject *CryList::Add(EmptyObject *_Item,bool IsCryObject,bool IsOwned,size_t _Size)
{
	ListNode *New = new ListNode;
	if (New==0)
		throw CryException(this,"Memory error\n");
	New->Item = _Item;
	New->Size = _Size;
	New->Next = 0;
	New->IsOwned = IsOwned;
	New->IsCryObject = IsCryObject;
	AddListNode(New);
	return _Item;
}
CryObject *CryList::Add(CryObject *_Item)
{
	Add(_Item,true,false);
	return _Item;
}
EmptyObject *CryList::Add(EmptyObject *_Item,size_t _Size)
{
	return Add(_Item,false,false,_Size);
};
CryObject *CryList::AddOwned(CryObject *_Item)   // gives ownership to list
{
	Add(_Item,true,true);
	return _Item;
}
EmptyObject *CryList::AddOwned(EmptyObject *_Item,size_t _Size)   // gives ownership to list
{
	return Add(_Item,false,true,_Size);
}

bool CryList::_Remove(EmptyObject *_Item)
{
ListNode *Prev,*p,*f = _FirstNode();
	if (f)
	{
		Prev = p = f;
		do
		{
			if (Items->Item == _Item)
			{
				if (p==Items)	// is last node?
				{
					if (Items==Prev)	// single item on list
						Items=0;
					else
						Items = Prev;	// update Items to next to last
				}
				Prev->Next = p->Next;
				p->Next = 0;
				DeleteItem(p);
				delete p;
				return true;
			}
			Prev = p;
			p=p->Next;
		} while(p!=f);
	}
	return false;
}
void CryList::Remove(EmptyObject *_Item)
{
	if (! _Remove(_Item))
		throw CryException(this,0,(const char *)"Item Not found in list");
}
void CryList::Remove(CryObject*_Item)
{
	if (! _Remove(_Item))
		throw CryException(this,0,_Item->ChildClassName(),(const char *)"Not found in list");
}


void CryList::SetItemOwnerShip(EmptyObject *Item,bool Owned)
{
	ListNode *n = _FindNode(Item);
	n->IsOwned = Owned;
}
bool CryList::GetItemOwnerShip(const EmptyObject *Item) const
{
	ListNode *n = _FindNode(Item);
    return n->IsOwned;
}

void CryList::Clear()
{
    while(Items)
    {
        ListNode *p = Items;
        Items = p->Next;
        DeleteItem(p);
        delete p;
    }
}

size_t CryList::Count() const
{
	size_t i = 0;
	if (Items)
	{
	const ListNode *f = FirstNode();
	const ListNode *l = LastNode();
		i++;
		while(f != l)
		{
			f = f->Next;
			i++;
		}
	}
	return i;
}

void CryList::SaveItemsTo(CryStream &ToStream) const
{
	const ListNode *p,*f = FirstNode();
	if (f)
	{
		p = f;
		StreamMode Mode = ToStream.GetMode();
		do
		{
			if (p->IsCryObject)
				((CryObject *)p->Item)->SaveTo(ToStream);
			else
			{
				ToStream.printf("%d ",p->Size);
				char *e = (char *)p->Item;
				for(size_t i=0;i<p->Size;i++)
					ToStream.printf("%d ",e[i]);
			}
			p = p->Next;
			if (Mode==SText)
				ToStream.printf("|");
		} while(p!=f);
	}
}

bool CryList::InList(CryObject *Needle) const
{
	if (FindNode(Needle))
		return true;
	return false;
}

const CryList::ListNode *CryList::FindNode(const EmptyObject *Needle) const
{
	const ListNode *p,*f = FirstNode();
	if (f)
	{
		p = f;
		do
		{
			if (p->Item==Needle)
				return p;
			p=p->Next;
		} while(p!=f);
	}
	return 0;
}

CryList::ListNode *CryList::_FindNode(const EmptyObject *Needle) const
{
	return (CryList::ListNode*) FindNode(Needle);
}

void CryList::RemoveNodeValue(const CryMemStream &Needle)   // find a node who's item has the same "value" property
{

	ListNode *Prev,*p,*f = _FirstNode();
	if (f)
	{
		Prev = p = f;
		size_t nl = Needle.GetLength();
		CryMemStream *t;
		do
		{
			if ((p->IsCryObject) && (((CryObject *)p->Item)->IsA(TCryMemStream)))
			{
				t = (CryMemStream *) p->Item;
				if ((t->GetLength()==nl) && (*t==Needle))
				{
					if (p==Items)	// is last node?
					{
						if (Items==Prev)	// single item on list
							Items=0;
						else
							Items = Prev;	// update Items to next to last
					}
					Prev->Next = p->Next;
					p->Next = 0;
					DeleteItem(p);
					delete p;
				}
			}
			Prev = p;
			p=p->Next;
		} while(p!=f);
	}
	return;
}

/// return the index into the list of a value
int CryList::FindNodeValue(const CryMemStream &Needle) const   // find a node who's item has the same "value" property
{
int i=0;
	const ListNode *p,*f = FirstNode();
	if (f)
	{
		p = f;
		size_t nl = Needle.GetLength();
		CryMemStream *t;
		do
		{
			if ((p->IsCryObject) && (((CryObject *)p->Item)->IsA(TCryMemStream)))
			{
				t = (CryMemStream *) p->Item;
				if ((t->GetLength()==nl) && (*t==Needle))
					return i;
			}
			i++;
			p=p->Next;
		} while(p!=f);
	}
	return -1;
}

EmptyObject *CryList::GetItem(int i) const
{
	const ListNode *p,*f = FirstNode();
	if (f)
	{
		p = f;
		do
		{
			p = p->Next;
			i--;
		} while(i && (p!=f));
	}
    if (p)
        return p->Item;
    else
        throw CryException(this,"Item Not found");
}

void CryList::CopyTo(CryObject &Dest) const //copies contents of this to Dest
{
	if (Dest.IsA(TCryList))
	{
		CryList *_Dest = (CryList *) &Dest;
		const ListNode *p,*f = FirstNode();
		if (f)
		{
			p = f;
			do
			{
				if (p->IsOwned)
				{
					if (p->IsCryObject)
						_Dest->AddOwned((CryObject *)(DupItem(p)));
					else
						_Dest->AddOwned(DupItem(p),p->Size);
				}
				else
				{
					if (p->IsCryObject)
						_Dest->Add((CryObject *)p->Item );
					else
						_Dest->Add(p->Item,p->Size);
				}
				p = p->Next;
			} while(p!=f);
		}
		return;
	}
	else
		throw CryException(this,"Copying from List to object that is not Listable");
}

size_t CryList::Size() const
{
	size_t Result = sizeof(int);// for storing count
	const ListNode *p,*f = FirstNode();
	if (f)
	{
		p = f;
		do
		{
			if (p->IsCryObject)
				Result += ((CryObject *)p->Item)->Size();
			else
				Result += p->Size;
			p = p->Next;
		} while(p!=f);
	}
	return Result;
}

int CryList::GetPropertyCount() const
{
	return CryContainer::GetPropertyCount() + 1;
}

bool CryList::HasProperty(const CryPropertyParser &PropertyName) const
{
	return CryContainer::HasProperty(PropertyName);
}

const char *CryList::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
	Result.Clear();
	if (PropertyName=="Values")   // intercept crycontainer's property for our own
	{
		Result = "[]";  // if Result != what is returned, it's a special situation
		return "*";
	}
	else
		return CryContainer::GetProperty(PropertyName,Result);
}

CryPropertyList *CryList::PropertyNames() const
{
    CryPropertyList *n = CryContainer::PropertyNames();
    return n;
}

void CryList::Sort(int CompareType)
{
	if (Items==0)	// nothing in list
		return;
	if (Items->Next==Items)
		return; // only one item in list

	ListNode *LocItems = _FirstNode();
	Items->Next =0;	// make a normal linked list
	Items = 0;
	ListNode *Current,*Prev,*LastAdded,*Test,*PrevTest;
	LastAdded = 0;
	while(LocItems)
	{
		Current = LocItems;
		Prev = PrevTest = 0;
		if (Current->Next == 0) // on last one
		{
			LastAdded->Next = Current;  // done
			Current->Next = Items;	// make back to our looped list
			Items = Current;
            return;
        }
        // find the lowest one in list
        Test = Current->Next;
        while(Test)
        {
            int c = Compare2(CompareType,Current,Test);
            if (c<0)
            {
                Prev = PrevTest;
                Current = Test;
            }
            PrevTest = Test;
            Test = Test->Next;
        }
        // Current is lowest in list
        if (Prev)       // Item before one being unlinked
            Prev->Next = Current->Next; // Update Next to skip the Current
        else
        {
            if (Current==LocItems)
                LocItems = LocItems->Next;  // The first in the list is the Current None-before
            else
                LocItems->Next = Current->Next;
        }
        if (LastAdded)
            LastAdded->Next = Current;
        else
            Items = Current;
        LastAdded = Current;
        Current->Next = 0;// not needed?
    }
}

int CryList::Compare2(int CompareType,const EmptyObject *First,const EmptyObject *Second) const
{
    ListNode *f,*s;
    f = (ListNode *)First;
    s = (ListNode *)Second;
    if (f->IsCryObject)
    {
		CryObject *F = (CryObject *)f->Item;
        if (s->IsCryObject)
        {
            CryObject *S = (CryObject *)s->Item;
            return F->CompareLogical(CompareType,S);
        }
        return -1;  // EmptyObjects are always less then CryObjects
    }
    else
    {
        if (s->IsCryObject)
        {
            return 1; // EmptyObjects are alway less then CryObjects
		}
		if (f->Size==s->Size)
			return memcmp(f->Item,s->Item,f->Size);
		return f->Size - s->Size; // both empty objects
    }
}
