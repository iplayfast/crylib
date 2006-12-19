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

#include "ClassMemStream.h"
namespace Crystal
{
using namespace std;

#ifndef CList
#define CList "List"



/// a list class. It tracks whether the object in the list is an CryObject, and if the list "owns" the object, (ie responsible for deleting)
/*!	This class uses the Iterator functions as well as it's own
*/
class List : public Container
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
	virtual bool Sortable()const;
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
	EmptyObject *AddOwned(EmptyObject *Item,size_t Size);
	Object *Add(Object *Item);    // returns Item
	Object *AddOwned(Object *Item);   // gives ownership to list
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
	virtual bool IsContainer() const;
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

}
;  // CryList
#endif // TCryList
}
