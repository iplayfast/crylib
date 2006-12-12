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
//---------------------------------------------------------------------------

#include "ClassList.h"
#include "ClassString.h"
#include "ClassProperty.h"
namespace Crystal
{
using namespace std;

#ifndef CXML
#define CXML "XML"
#define CXMLNode "XMLNode"




/*!

In order to be used by XML classes must have at least

// functions needed by CryObject
   virtual void CopyTo(CryObject *Dest) const;  //copies contents of this to Dest
   virtual CryObject *Dup() const; // creates a duplicate of this object

// functions needed by CryNamedObject
    virtual const char* ChildClassName() const {     return TCryBPNetContainer; }
    virtual bool HasProperty(const char *PropertyName)const;
    virtual int GetPropertyCount() const { return 1; }
    virtual CryList *PropertyNames() const;
    virtual const char *GetProperty(const char *PropertyName,CryString *Result) const;
    // if this class contains the property name, it will attempt to load it
    // if all is well returns true
    virtual bool SetProperty(const char *PropertyName,const char *PropertyValue);


// if the class is a container class it must also have
    // functions needed by CryContainer
   virtual Iterator *_CreateIterator() const;
   virtual void DeleteIterator(Iterator *I) const;
   virtual bool GotoFirst(Iterator *I) const;   // returns true if success
   virtual bool GotoPrev(Iterator *I) const;   // returns true if success
   virtual bool GotoNext(Iterator *I) const;    // returns true if success
   virtual bool GotoLast(Iterator *Iterator) const;    // returns true if success
   virtual EmptyObject *GetAtIterator(const Iterator *I) const;
   virtual size_t Count() const;
   virtual void Clear();
   virtual EmptyObject *Add(EmptyObject *Item,size_t Size);
   virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size);
   virtual CryObject *Add(CryObject *Item);    // returns Item
   virtual CryObject *AddOwned(CryObject *Item);   // gives ownership to list
   virtual void SetItemOwnerShip(Iterator *I,bool Owned);
   virtual bool GetItemOwnerShip(Iterator *I) const;
   virtual size_t GetItemSize(Iterator *I) const;
   virtual bool IsCryObject(const Iterator *I) const;
   virtual bool LoadAsText(Iterator *I,CryString *FromStream);
   virtual bool SaveAsText(Iterator *I,CryString *ToStream) const;


*/


/// The internal nodes of the xml system
class XMLNode : public List
{
String Name;
/*! The type of object represented by this node */
String Type;
/*! the list of attributes/values that this object has */
CryPropertyList *_Attributes;
/*! the list of XML nodes that this node contains */
List SubNodes;
/*! an internal flag allowing the same code to be used for getting attribute names, as well as class names */
bool GettingName;

public:
StdFunctionsNoDup(XMLNode,List);
    XMLNode(const char *_Name="")
    {
        Name = _Name;
        GettingName = false;
        _Attributes = new CryPropertyList();
        Type="Unknown";
    }
    ~XMLNode()
    {
        //SubNodes = 0;
        delete _Attributes;
    }
    void SetName(const char *N)
    {
      Name = N;
    }
    const char *GetName() const { return Name.AsPChar(); }

    void AddAttribute(const char *Name,const char *Value);
    /*! Save this node to a stream (xml format) */
    virtual void SaveTo(Stream &ToStream) const;
    /*! save this node to an object setting the object's attributes */
    virtual void SaveTo(Object &ToObject) const;
    /*! Load this node from an xml stream */
    virtual void LoadFrom(const Stream &FromStream);
    /*! Load this node from an object */
    virtual void LoadFrom(const Object &FromObject);
    /*! Create the Object defined by this XML Node*/
    virtual Object *CreateObjectFromNode(Object *Parent=0);

    // not used
    virtual void CopyTo(Object &Dest) const {}  //copies contents of this to Dest
    virtual Object *Dup() const { return 0; } // creates a duplicate of this object /* TODO : dup isn't implemented */
    virtual size_t Size() const {return Type.Size() + _Attributes->Size() + /*Next->Size() + */SubNodes.Size();}
    virtual const cbyte* GetRaw() const {return Type.GetRaw(); }
};

/// Interface class for XML 
class CryXML : protected List
{
public:
StdFunctionsNoDup(XML,List);
    CryXML(const char *Name = ""){ /*head = */ AddOwned(new XMLNode(Name));}
    virtual ~CryXML() { /*delete head;*/ }
        
	/*! Save this xml tree to a stream (xml format) */
    virtual void SaveTo(Stream &ToStream) const { XMLNode *head = (XMLNode *) FirstNode()->Item; head->SaveTo(ToStream); }
    /*! save this xml stream to an object setting the object's attributes */   
    virtual void SaveTo(Object &ToObject) const { XMLNode *head = (XMLNode *) FirstNode()->Item; head->SaveTo(ToObject); }
    
     /*! Load this xml tree from a xml stream */
    virtual void LoadFrom(const Stream &FromStream) { XMLNode *head = (XMLNode *) FirstNode()->Item; head->LoadFrom(FromStream); }
    virtual void CopyTo(Object &Dest) const {};  //copies contents of this to Dest
    /*! Load this xml tree from an object */
    virtual void LoadFrom(const Object &FromObject) { XMLNode *head = (XMLNode *) FirstNode()->Item; head->LoadFrom(FromObject); }
    /*! Create the Object defined by this XML class */
    virtual Object *CreateObjectFromNode(Object *Parent=0);
    virtual Object *Dup() const { return 0; } /* TODO : dup isn't implemented */
    virtual size_t Size() const { return List::Size(); } //return head->Size(); }
    virtual const cbyte* GetRaw() const { return List::GetRaw(); }



};


}//namespace Crystal

#endif
