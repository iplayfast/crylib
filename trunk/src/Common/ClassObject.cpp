/***************************************************************************
 *   Copyright (C) 2006 by Chris Bruner                                    *
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef __BORLANDC__
#include <io.h>
#define strcasecmp stricmp
#endif

#include "ClassXML.h"
#include "ClassArray.h"
#include "CryFuzzy.h"
#include "CryBackProp.h"
#include "BitArray.h"
#include "HugeInt.h"
#include "zlib.h"
#include "CryPattern.h"
#include "ClassFunction.h"
#include "ClassProperty.h"
#include "ClassFileStream.h"
#include "ClassList.h"

int gdebug =0;

using namespace Crystal;
#ifdef DEBUG
int ObjectID =0;
#endif

//-------------------------------------------------------------------
// Object
//-------------------------------------------------------------------


#ifdef VALIDATING

bool Object::Test(bool Verbose,Object &ThisObject, bool (CallBack)(bool Verbose,const
                     char *Result,bool Fail))
{
    char Result[200];
    bool Fail = false;
	String spn,spv,stemp;
	sprintf(Result,"\nObject Testing:\nObject of ClassName %s,ChildClassName %s",
            ThisObject.ClassName(),ThisObject.ChildClassName());

    if (!CallBack(Verbose,Result,Fail))
        return false;




    {

        PropertyList *pn = ThisObject.PropertyNames();

        sprintf(Result,"\n%s has %d properties", ThisObject.ClassName(),pn->Count());

        if (!CallBack(Verbose,Result,Fail))
            return false;

        PropertyList::PropertyIterator *i = pn->CreateIterator();

        {
            int count=0;

            if (i->GotoFirst())
			{
			String Result;
				do
				{
					count++;
					String value;

					const
					String *item =  i->GetName();

					i->GetValue(value);
					Result.printf("\n  Property %d) %s = %s",count,item->AsPChar(),value.AsPChar());
					if (!CallBack(Verbose,Result,Fail))
					{
						pn->DeleteIterator(i);
						return false;
					}
					Result = "";
				} while (i->GotoNext());
			}
		}

		pn->DeleteIterator(i);

        delete(pn);

        try
        {
            String t;
            ThisObject.GetProperty("BADVALUE",t);
            Fail = true;

            if (!CallBack(Verbose,"Exception thrown on \"BADVALUE\"",Fail))
                return false;

        }

        catch (Exception &e)
        {
            if (!CallBack(Verbose,"Exception thrown on \"BADVALUE\"",Fail))
                return false;
        }
    }


    if (ThisObject.CanDup())
    {
        String spn,stemp;
        char Result[200];

        try
        {
            ThisObject.SaveTo(spn);

            if (!CallBack(Verbose,"Object saved to string without problems",Fail))
            {
                return false;
            }
        }

        catch (Exception &e)
        {
            Fail=true;

            if (!CallBack(Verbose,"Object saved to string with problems",Fail))
            {
                return false;
            }
        }

        {
            Object *co=0;
            {
                XML x;

                try
                {
                    x.LoadFrom(ThisObject);

					if (!CallBack(Verbose,"XML Loaded Object without problems",Fail))
					{
						return false;
					}
				}

				catch (Exception &e)
				{
					Fail=true;

					if (!CallBack(Verbose,"XML Loaded Object with problems",Fail))
					{
                        return false;
                    }
                }


				try
				{
					co = x.CreateObjectFromNode();
				}

				catch (Exception &e)
				{
					String s;
					Fail = true;

					s.printf("Exception thrown on CreateObjectFromNode \"%s\" continue to attempt using object as parent",(const
							 char *)e);

					try
					{
					String str;
						x.SaveTo(str);
	// useful for debugging
{
FileStream a;
	a.Open("x.xml","w",false);
	str.CopyTo(a);
}

						Fail = false;	// above failure may be valid (as designed)
						co = x.CreateObjectFromNode(&ThisObject);
					}

					catch (Exception &e)
					{
						String s;
						Fail = true;

						s.printf("Failed to CreateObjectFromNode \"%s\" (including using object as parent)",(const
								 char *)e);

						if (!CallBack(Verbose,s,Fail))
							return false;

					}

				}

			}

			if (co)
			{
				if (!CallBack(Verbose,"CreateObjectFromNode",Fail))
				{
					delete co;
					return false;
				}
				String factorystring;
				co->SaveTo(factorystring);

				if (factorystring!=spn)
				{
  // useful for debugging
	FileStream a,b;
	a.Open("factory.xml","w",false);
	b.Open("spn.xml","w",false);
	factorystring.CopyTo(a);
	spn.CopyTo(b);
	a.Close();
	b.Close();

					sprintf(Result,"\nObject failed XML save rebuild test");
					Fail = true;
				}

				else
				{
					sprintf(Result,"\nObject passed XML save rebuild test");
				}

                delete co;


                if (!CallBack(Verbose,Result,Fail))
                    return false;
            }
        }

    }


    return !Fail;

    /*
    virtual void SaveTo(Stream *ToStream);
    virtual void LoadFrom(Stream *FromStream);
	virtual void CopyTo(Object *Dest) const =0;  //copies contents of this to Dest
    virtual Object *Dup() const =0; // creates a duplicate of this object
    int GetCopyCount() const {  return CopyCount; }
    int IncCopyCount() {  CopyCount++; return CopyCount; }
    int DecCopyCount() {  CopyCount--; return CopyCount; }
    void Destroy(Object *Object);
    virtual Object *CreateItemType(const char *Name);
    virtual const cbyte* GetRaw() const = 0;
    virtual bool Event(EObject EventNumber,Context::IO &Context);
    */
}

#endif

Object::~Object()
{}

Object &Object::operator =(const

                                 Object &From)
{
    From.CopyTo(*this);
    return *this;
}

Object &Object::operator =(const

                                 Object *From)
{
    From->CopyTo(*this);
    return *this;
}

void Object::CopyTo(Object &Dest) const

{
    return;
}

Object *Object::Dup() const

{
    return new Object();
} // creates a duplicate of this object (no data so it's easy!)

size_t Object::Size() const
{
    return 0;
}

const
char *Object::AsPChar() const
{

    return (const
            char *) GetRaw();
}

bool Object::IsAbstract() const

{
    return false;
}    // is the class abstract?

int Object::Compare(int CompareType,const
                       Object *Test1,const
                       Object *Test2) const
{
    return Test1->CompareLogical(CompareType,Test2);
}

int Object::CompareLogical(int CompareType,const

                              Object *Test) const
{
    return 0;
}

bool Object::LessThen(int CompareType,const

                         Object *Test) const
{
    return false;
}

bool Object::GreaterThen(int CompareType,const

                            Object *Test) const
{
    return false;
}

bool Object::EqualTo(int CompareType,const

                        Object *Test) const
{
    return true;
}

/*! IsContainer is true when the object in question can contain
 accessable instances of data or objects
 ie streams are not containers, but lists and arrays are
 */
bool Object::IsContainer() const
{
    return false;
}

/*! returns true if the class in question has the property    */
bool Object::HasProperty(const
                            PropertyParser &PropertyName) const
{
#ifdef DEBUG
    return PropertyName=="ObjectID";
#else
    return false;
#endif
}

/*! returns true if the class in question can have the property    */
bool Object::CanHaveProperty(const
                                PropertyParser &PropertyName) const
{
    return HasProperty(PropertyName);
}

/*! The count of the properties a class has */
int Object::GetPropertyCount() const
{
#ifdef DEBUG
    return 1;
#else
    return 0;
#endif
}

bool Object::SetProperty(const

                            PropertyParser &PropertyName,const
                            char *PropertyValue)
{
#ifdef DEBUG

    if (PropertyName=="ObjectID")
    {
        String s;
        s = PropertyValue;
        s.scanf("%d",&ObjectID);
        return true;
    }

#endif
    return false;
}


bool Object::CanCreate(const

                          PropertyParser &PropertyName) const
{
    return ClassCanCreate(PropertyName);
}

/*! IteratedFunction is called for each item in the container (from IteratedThroughAll)
 returns true if iteration should stop
 */
bool Object::IteratedFunction(EmptyObject *Control,EmptyObject *Item)
{
    return true;
}


FunctionDefList *Object::GetFunctions(const

        char *Type) const
{
    if (Type && !IsA(Type))
        throw Exception("Cannot GetFunctions for type %s",Type);

    String s;

    s+= "//Class Object;";

    s+= "virtual bool CanDup() const;";

    s+= "virtual void CopyTo(Object &Dest) const = 0;";

    s+= "virtual Object *Dup() const = 0;";

    s+= "virtual size_t Size() const;";

    s+= "virtual Object *CreateItemType(const char *Name);";

    s+= "const char* ClassName() const;";

    s+= "virtual const char *ChildClassName() const;";

    s+= "virtual const cbyte* GetRaw() const;";

    s+= "virtual const char *AsPChar() const;";

    s+= "virtual bool IsA(const char *ClassName) const;";

    s+= "virtual bool Event(EObject EventNumber,Context::IO &Context);";

    //    s+= "Object *LoadItem(Stream &FromStream);";
    s+= "virtual String *GetFunctions() const;";

    s+= "virtual bool IsContainer() const;";

    s+= "virtual Property *GetPropertyAsProperty(const char *PropertyName) const;";

    s+= "virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const = 0;";

    s+= "virtual bool HasProperty(const char *PropertyName) const;";

    s+= "virtual int GetPropertyCount() const;";

    s+= "virtual List *PropertyNames() const;";

    s+= "virtual bool SetProperty(const char *PropertyName,const char *PropertyValue) = 0;";

	s+= "virtual bool SetPropertyAsObject(const Property *Value);";

    s+= "virtual void SaveTo(Stream &ToStream) const;";

    s+= "virtual void LoadFrom(Stream &FromStream);";

    s+= "virtual Object *Create(Stream &FromStream);";

    s+= "virtual Object *Create(const char *Type,Object *Parent);";

    s+= "bool IterateThroughAll(Container *Container,EmptyObject *Control);";

    s+= "virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item);";

#ifdef VALIDATING

    s+= "virtual bool Test(bool Verbose,Object &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));";

#endif

    FunctionDefList *l = new FunctionDefList();

    l->LoadFromString(s,";");

    return l;
}

FunctionDefList *Object::GetAbstractFunctions(const

        char *Type) const
{
    // pure and abstract mean the same thing
    // first get all the functions (from the derived childclass)
    FunctionDefList *l = GetFunctions(Type);
    FunctionDefList::ListIterator	*it =(FunctionDefList::ListIterator *) l->_CreateIterator();
    FunctionDefList *Return = new FunctionDefList();
    FunctionDef *s,*s1;

    if (it->GotoFirst())
        do
        {
            s = (FunctionDef *)it->Get();

            if (!s->IsPure)	// found one that isn't pure, change any pure ones following to non-pure
            {
                FunctionDefList::ListIterator *it1 =
                    (FunctionDefList::ListIterator *)it->Dup();

                if (it1->GotoNext()) // check all functions following
                    do
                    {
                        s1 = (FunctionDef *)it1->Get();

                        if (s1->GetNPDeclaration()==s->GetNPDeclaration())	// same function def
                            s1->IsPure = false;
                    }

                    while (it1->GotoNext());

                l->DeleteIterator(it1);
            }
        }

        while (it->GotoNext());

    // now any pure functions are unique
    if (it->GotoFirst())
        do
        {

            s = (FunctionDef *)it->Get();

            if (s->IsPure)
            {
                FunctionDef *n = new FunctionDef(*s);
                Return->AddOwned(n);
            }
        }

        while (it->GotoNext());

    l->DeleteIterator(it);

    delete l;

    return Return;
}



const
cbyte* Object::GetRaw() const
{
    throw Exception(this,"Cannot GetRaw from Object");
}

bool Object::GetIsPropertyContainer(const PropertyParser &PropertyName) const
{
	if (HasProperty(PropertyName))
	{
		return false; // generally properties are strings containing values, special case needs to be handled specially
	}
    throw Exception(this,ExceptionUnknownProperty,"Unknown Property \"%s\"",PropertyName.AsPChar());
}

/// Get a property
Object *Object::GetCopyOfPropertyAsObject(const
        PropertyParser &PropertyName) const
{
#ifdef DEBUG

    if (PropertyName=="ObjectID")
    {
        String *s = new String();
        s->printf("%d",ObjectID);
        return s;
    }

#endif
    if (HasProperty(PropertyName))
    {
        throw Exception(this,ExceptionNotImplemented,"\"%s\" has not implemented GetCopyOfPropertyAsObject for Property \"%s\"",
                           ChildClassName(),PropertyName.AsPChar());
    }

    else
        throw Exception(this,ExceptionUnknownProperty,"Unknown Property \"%s\"",PropertyName.AsPChar());
}

/// Get a property
Object *Object::_GetPropertyAsObject(const
        PropertyParser &PropertyName) const
{
    if (HasProperty(PropertyName))
    {
        throw Exception(this,ExceptionNotImplemented,"\"%s\" has not implemented _GetPropertyAsObject for Property \"%s\"",
                           ChildClassName(),PropertyName.AsPChar());
    }

    else
        throw Exception(this,ExceptionUnknownProperty,"Unknown Property \"%s\"",PropertyName.AsPChar());
}

const
char *Object::GetProperty(const
                             char *PropertyName,String &Result) const
{
    PropertyParser pp(PropertyName);
    return GetProperty(pp,Result);

}

const
char *Object::GetProperty(const
                             PropertyParser &PropertyName,String &Result) const
{
#ifdef DEBUG

    if (PropertyName=="ObjectID")
    {
        Result.printf("%d",ObjectID);

        return (const
                char *)Result;
    }

#endif
    throw Exception(this,ExceptionUnknownProperty,"Unknown Property \"%s\"",PropertyName.AsPChar());
}

bool Object::SetPropertyAsObject(const Property *Value)

{
    String Result;
    return SetProperty(Value->GetName()->AsPChar(),Value->GetValue(Result));
}

/*Object *Object::Dup() const
{
    throw Exception(this,"Cannot Dup a Object");
} */


Object::Object()
{
#ifdef DEBUG
    ObjectID = ::ObjectID;
	::ObjectID++;
	strncpy(ObjectType,"Unknown",29);
#endif
}

const
char* Object::ClassName() const
{
	return CObject;
}

const
char* Object::ChildClassName() const
{
	return CObject;
}

bool Object::IsA(const

					char *_ClassName) const // can the object map to a ClassName
{
#ifdef DEBUG
// this is done here instead of the constructor because it requires a virtual function, and the constructor doesn't have it set up yet.
// consequently if you are debugging and need to see the Object type, you will have needed to call IsA before hand. (IsA("") works fine).
	strncpy((char *)this->ObjectType,ChildClassName(),29);
#endif
	return strcmp(_ClassName,CObject)==0;
}

// returns true if handled
bool Object::Event(EObject EventNumber,Context::IO &Context)
{
    switch (EventNumber)
    {

EFirst:
            return false;

ESaveObject:
            Context.In.InSave.ToStream->SetMode(SObject);
            SaveTo(*Context.In.InSave.ToStream);    // ToStream as Object
            break;

ELoadObject:
            Context.In.InLoad.FromStream->SetMode(SObject);
            LoadFrom(*Context.In.InLoad.FromStream);    // FromStream as Object
            break;

ESaveStream:
            Context.In.InSave.ToStream->SetMode(SText);
            SaveTo(*Context.In.InSave.ToStream);    // ToStream as Object
            break;

ELoadStream:
            Context.In.InLoad.FromStream->SetMode(SText);
            LoadFrom(*Context.In.InLoad.FromStream);    // FromStream as Object
            break;

ECopyTo:
            CopyTo(*Context.In.InCopyTo._Object);  //copies contents of this to Dest
            break;

EDup:
            Context.Out.OutDup._Object = Dup();
            break;

ESize:
            Context.Out.OutSize.Result = Size();
            break;

EClassName:
            Context.Out.OutClassName.Text = ChildClassName();
            break;

EIsA:
            Context.Out.OutIsa.Result = IsA(Context.In.InIsa.Text);
            break;

EEvent:
            return false;

ELast:
            return false;
    }

    return true;
}

bool Object::ClassCanCreate(const

                               PropertyParser &PropertyName)
{
	return (PropertyName==CMemStream) ||
		   (PropertyName==CXML) ||
		   (PropertyName==CXMLNode) ||
		   (PropertyName==CFileStream) ||
		   (PropertyName==CString) ||
		   (PropertyName==CProperty) ||
		   (PropertyName==CList) ||
		   (PropertyName==CPropertyList) ||
		   (PropertyName==CDoubleArray) ||
		   (PropertyName==CFunctionDef) ||
		   (PropertyName==CFunctionDefList) ||
		   (PropertyName==CFuzzy) ||
		   (PropertyName==CBitArray) ||
		   (PropertyName==CBackPropagateNetwork) ||
		   (PropertyName==CHugeInt);
}


/*!
	Create can create
	MemStream,
	FileStream,
	String,
	Property,
	List,
	PropertyList,
	DoubleArray,
	Fuzzy,
	BPNetContainer
	BackPropagateLayer
	HugeInt
*/
Object *Object::ClassCreate(const PropertyParser &PropertyName,Object *Parent)
{
// first create the object

	if (PropertyName==CObject)
		return new Object();
	if (PropertyName==CMemStream)
		return (Object *) new MemStream();
	if (PropertyName==CXML)
		return (Object *)new XML();
	if (PropertyName==CXMLNode)
		return (Object *)new XMLNode();
	if (PropertyName==CFileStream)
		return (Object *)new FileStream();
	if (PropertyName==CString)
		return (Object *)new String();
	if (PropertyName==CProperty)
		return (Object *)new Property("NoName");
	if (PropertyName==CList)
		return (Object *)new List();
	if (PropertyName==CMyList)
    	return (Object *)new MyList();
	if (PropertyName==CPropertyList)
		return (Object *)new PropertyList();
	if (PropertyName==CDoubleArray)
		return (Object *)new DoubleArray();
	if (PropertyName==CIntArray)
		return (Object *)new IntArray();
	if (PropertyName==CFunctionDef)
		return (FunctionDef *)new FunctionDef();
	if (PropertyName==CFunctionDefList)
		return (FunctionDefList *) new FunctionDefList();
	if (PropertyName==CFuzzy)
		return (Object *)new Fuzzy();
	if (PropertyName==CBitArray)
		return (BitArray *) new BitArray();
	if (PropertyName==CStrategy)
		return new Strategy();
	if (PropertyName==CStrategyHolder)
		return new StrategyHolder();
	if (PropertyName==CStrategyHolderSender)
		return new StrategyHolderSender();
	if (PropertyName==CObserver)
		return new Observer();
	if (PropertyName==CObservable)
		return new Observable();
	if (PropertyName==CDecorator)
	{
		if (Parent==0 || Parent->IsA(CDecorator))
			return  new Decorator((Decorator *)Parent);
	}
	if (PropertyName==CFactory)
		return new Factory();
	if (PropertyName==COFactory)
		return new OFactory();
	if (PropertyName==CCommandHolder)
		return new CommandHolder();
	if (PropertyName==CCommandHolderSender)
		return new CommandHolderSender();
	if (PropertyName==CCompositeIterator)
		return new CompositeIterator(0);
	if (PropertyName==CState)
		return new State(1);
		// Uncomment this to make backpropagation part of the base class creation abilities
/*
	 if (PropertyName==CCryBPNetContainer)
		return (Object *)new CryBPNetContainer();
	if (PropertyName==CBackPropagateLayer)
	{
		if (Parent && (Parent->IsA(CCryBPNet)))
		{
			return ((CryBPNet *)Parent)->AddLayer(0);
		}
		else
			throw Exception("Cannot Create BackPropagateLayer, Must have a parent of %s (or a decendent)",TCryBPNet);
	}
*/
	if (PropertyName==CHugeInt)
		return (Object *)new HugeInt();
	if (Parent)
		return Parent->Create(PropertyName,0);
	throw Exception("Could not Create Class %s (maybe abstract?)",(const char *)PropertyName);
}



void Object::SaveTo(Stream &ToStream) const

{
	XML xml;
	xml.LoadFrom(*this);
	switch (ToStream.GetMode())
	{
		case
				SObject:
			// zlib compression of xml class
			{
				String w;
				xml.SaveTo(w);
				MemStream c;
				int wl = w.GetLength();
				c.SetLength(w.GetLength()+ 100);    // safty range of size
				//        zlibcompress(w,c);
				int l = 0;
				ToStream.Write((const char *) &l,sizeof(l));    // marker for asciiz if opened incorrectly and also indicator that this is compressed
				ToStream.Write((const char *)&wl,sizeof(wl));  // original length
				l = c.GetLength();
				ToStream.Write((const char *)&l,sizeof(l));   // compressed length
				ToStream.Write((const char *)c.GetRaw(),l);   //data
			}
			break;
		case
				SText:
			xml.SaveTo(ToStream);
			break;
	}
}

void Object::LoadFrom(const Stream &FromStream)
{
	XML xml;
	switch (FromStream.GetMode())
	{
		case
				SObject:
			// zlib compression of xml class
			{
				int l;
				FromStream.Read((char *)&l,sizeof(l));
				if (l!=0)   // get marker
					throw Exception(this,"Compressed Read from non-compressed format");
				FromStream.Read((char *)&l,sizeof(l));  // original length
				String w;
				w.SetLength(l);
				MemStream c;
				FromStream.Read((char *)&l,sizeof(l));
				c.SetLength(l);
				c.Write(&FromStream,l);
				//            zlibuncompress(c,w);
				xml.LoadFrom(c);
			}
			break;
		case
				SText:
            xml.LoadFrom(FromStream);
    }

    return xml.SaveTo(*this);
}

Object *Object::Create(Stream &FromStream)

{
    XML xml;

    switch (FromStream.GetMode())
    {
        case

                SObject:
            // zlib compression of xml class
			{
				int l;
				FromStream.Read((char *)&l,sizeof(l));
				if (l!=0)   // get marker
					throw Exception(this,"Compressed Read from non-compressed format");
				FromStream.Read((char *)&l,sizeof(l));  // original length
				String w;
				w.SetLength(l);
				MemStream c;
				FromStream.Read((char *)&l,sizeof(l));
				c.SetLength(l);
				c.Write(&FromStream,l);
				//            zlibuncompress(c,w);
				xml.LoadFrom(c);
			}
			break;
		case
			SText:
			xml.LoadFrom(FromStream);
	}
	return  xml.CreateObjectFromNode(this);
}

PropertyList *Object::PropertyNames() const
{
	PropertyList *n = new PropertyList();
#ifdef DEBUG
	n->AddPropertyByName("ObjectID",this);
#endif
	return n;
}

Property *Object::GetPropertyAsCryProperty(const

		PropertyParser &PropertyName) const
{
	if (HasProperty(PropertyName))
	{
		Property *o = new Property(PropertyName);
		String s;

		const
		char *c =GetProperty(PropertyName,s);
		String Result;
		if ((*c=='*' ) && (s=="[]"))
		{
			// usually s and *c will be the same, but in this case it indicates an array
			s.Clear();
			s.printf("*%s",PropertyName.AsPChar());   // this get's overridden, but is used to show the property name
			GetProperty(s.AsPChar(),Result);
		}

		o->SetValue(Result.AsPChar());

		return o;
	}

	throw Exception(this,ExceptionUnknownProperty,"Unknown Property \"%s\"",PropertyName.AsPChar());
}


bool Object::IterateThroughAll(Container *Container,EmptyObject *Control) // for each item, will call bool (*FunctionToCall)(EmptyObject *); (unless 0)

{
	Container::Iterator *i = Container->_CreateIterator();
	bool result = true;

	if (i->GotoFirst())
	{
		do
		{
			EmptyObject *eo = i->Get();
			if (IteratedFunction(Control,eo))
			{
				result = true;
				break;
			}
		}
		while (i->GotoNext());
	}
	Container->DeleteIterator(i);
	return result;
}

Object *Object::CreateItemType(const
									 PropertyParser &PropertyName)
{
	if (PropertyName==CList)
		return new List();
	if (PropertyName==CFileStream)
		return new FileStream();
	if (PropertyName==CMemStream)
		return new MemStream();
	if (PropertyName==CString)
		return new String();
	if (PropertyName==CStrategy)
		return new Strategy();
	if (PropertyName==CStrategyHolder)
		return new StrategyHolder();
	if (PropertyName==CStrategyHolderSender)
		return new StrategyHolderSender();
	if (PropertyName==CObserver)
		return new Observer();
	if (PropertyName==CObservable)
		return new Observable();
	if (PropertyName==CDecorator)
		return new Decorator(0);
	if (PropertyName==CFactory)
		return new Factory();
	if (PropertyName==COFactory)
		return new OFactory();
	if (PropertyName==CCommandHolder)
		return new CommandHolder();
	if (PropertyName==CCommandHolderSender)
		return new CommandHolderSender();
	if (PropertyName==CCompositeIterator)
		return new CompositeIterator(0);
	if (PropertyName==CState)
		return new State(1);
	throw Exception(this,"Trying to create instance of Unknown or Abstract Class %s",PropertyName.AsPChar());
}



Object *OwnedObject::Create(const

                                  PropertyParser &PropertyName,Object *Parent)
{
    try
    {
        Object *o = Object::Create(PropertyName,Parent);
        return o;
    }

    catch (Exception &e)
    {
        if (Owner)
        {
            return Owner->Create(PropertyName,Parent);
        }

        throw e;
    }
}













