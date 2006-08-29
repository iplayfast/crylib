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
#include "CryObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef __BORLANDC__
#include <io.h>
#define strcasecmp stricmp
#endif

#include "CryXML.h"
#include "CryArray.h"
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
// CryObject
//-------------------------------------------------------------------


#ifdef VALIDATING
bool CryObject::Test(bool Verbose,CryObject &Object, bool (CallBack)(bool Verbose,const char *Result,bool Fail))
{
    char Result[200];
    bool Fail = false;
    CryString spn,spv,stemp;
    sprintf(Result,"\nCryObject Testing:\nObject of ClassName %s,ChildClassName %s",
            Object.ClassName(),Object.ChildClassName());
    if (!CallBack(Verbose,Result,Fail))
        return false;




    {

        CryPropertyList *pn = Object.PropertyNames();

        sprintf(Result,"\n%s has %d properties", Object.ClassName(),pn->Count());
        if (!CallBack(Verbose,Result,Fail))
            return false;

		CryPropertyList::PropertyIterator *i = pn->CreateIterator();
		{
		int count=0;
		if (i->GotoFirst()) {
			do
			{
			count++;
			CryString value;
			const CryString *item =  i->GetName();
			i->GetValue(value);
			sprintf(Result,"\n  Property %d) %s = %s",count,item->AsPChar(),value.AsPChar());
			} while(i->GotoNext());

		}
		}
		pn->DeleteIterator(i);
		delete(pn);
        try
        {
            CryString t;
            Object.GetProperty("BADVALUE",t);
            Fail = true;
            if (!CallBack(Verbose,"Exception thrown on \"BADVALUE\"",Fail))
                return false;

        }
        catch (CryException &e)
        {
            if (!CallBack(Verbose,"Exception thrown on \"BADVALUE\"",Fail))
                return false;
        }
    }


    if (Object.CanDup())
    {
        CryString spn,stemp;
        char Result[200];
        Object.SaveTo(spn);
        CryObject *co;
        {
            {
                CryXML x;
                x.LoadFrom(Object);
                try
                {
                    co = x.CreateObjectFromNode();
                }
                catch (CryException &e)
                {
                    CryString s;
                    s.printf("Exception thrown on CreateObjectFromNode \"%s\"",(const char *)e);
                    if (!CallBack(Verbose,s,Fail))
                        return false;

                }

            }

            CryString factorystring;
            co->SaveTo(factorystring);
            if (factorystring!=spn)
            {
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


    return Fail;
    /*
    virtual void SaveTo(CryStream *ToStream);
    virtual void LoadFrom(CryStream *FromStream);
    virtual void CopyTo(CryObject *Dest) const =0;  //copies contents of this to Dest
    virtual CryObject *Dup() const =0; // creates a duplicate of this object
    int GetCopyCount() const {  return CopyCount; }
    int IncCopyCount() {  CopyCount++; return CopyCount; }
    int DecCopyCount() {  CopyCount--; return CopyCount; }
    void Destroy(CryObject *Object);
    virtual CryObject *CreateItemType(const char *Name);
    virtual const cbyte* GetRaw() const = 0;
    virtual bool Event(EObject EventNumber,Context::IO &Context);
    */
}
#endif

CryObject::~CryObject()
{}
bool CryObject::CanDup() const
{
    return true;
}

CryObject &CryObject::operator =(const CryObject &From)
{
	From.CopyTo(*this);
	return *this;
}

CryObject &CryObject::operator =(const CryObject *From)
{
	From->CopyTo(*this);
	return *this;
}

void CryObject::CopyTo(CryObject &Dest) const
{
    return;
}
CryObject *CryObject::Dup() const
{
    return new CryObject();
} // creates a duplicate of this object (no data so it's easy!)

size_t CryObject::Size() const
{
    return 0;
}

const char *CryObject::AsPChar() const
{
    return (const char *) GetRaw();
}

bool CryObject::IsAbstract() const
{
    return false;
}    // is the class abstract?

int CryObject::Compare(int CompareType,const CryObject *Test1,const CryObject *Test2) const
{
    return Test1->CompareLogical(CompareType,Test2);
}
int CryObject::CompareLogical(int CompareType,const CryObject *Test) const
{
	return 0;
}

bool CryObject::LessThen(int CompareType,const CryObject *Test) const
{
	return false;
}
bool CryObject::GreaterThen(int CompareType,const CryObject *Test) const
{
	return false;
}
bool CryObject::EqualTo(int CompareType,const CryObject *Test) const
{
    return true;
}

/*! IsContainer is true when the object in question can contain
 accessable instances of data or objects
 ie streams are not containers, but lists and arrays are
 */
bool CryObject::IsContainer() const
{
    return false;
}

/*! returns true if the class in question has the property    */
bool CryObject::HasProperty(const CryPropertyParser &PropertyName) const
{
#ifdef DEBUG
	return PropertyName=="ObjectID";
#else
	return false;
#endif
}
/*! returns true if the class in question can have the property    */
bool CryObject::CanHaveProperty(const CryPropertyParser &PropertyName) const
{
	return HasProperty(PropertyName);
}

/*! The count of the properties a class has */
int CryObject::GetPropertyCount() const
{
#ifdef DEBUG
	return 1;
#else
	return 0;
#endif
}

bool CryObject::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
#ifdef DEBUG
	if (PropertyName=="ObjectID")
	{
	CryString s;
		s = PropertyValue;
			s.scanf("%d",&ObjectID);
		return true;
	}
#endif
	return false;
}

/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the Parent must be the appropriete container type */
CryObject *CryObject::Create(const CryPropertyParser &PropertyName,CryObject *Parent)
{
	return ClassCreate(PropertyName,Parent);
}

bool CryObject::CanCreate(const CryPropertyParser &PropertyName) const
{
    return ClassCanCreate(PropertyName);
}

/*! IteratedFunction is called for each item in the container (from IteratedThroughAll)
 returns true if iteration should stop
 */
bool CryObject::IteratedFunction(EmptyObject *Control,EmptyObject *Item)
{
    return true;
}


CryFunctionDefList *CryObject::GetFunctions(const char *Type) const
{
	if (Type && !IsA(Type))
		throw CryException("Cannot GetFunctions for type %s",Type);
    CryString s;
    s+= "//Class CryObject;";
    s+= "virtual bool CanDup() const;";
    s+= "virtual void CopyTo(CryObject &Dest) const = 0;";
    s+= "virtual CryObject *Dup() const = 0;";
    s+= "virtual size_t Size() const;";
    s+= "virtual CryObject *CreateItemType(const char *Name);";
    s+= "const char* ClassName() const;";
    s+= "virtual const char *ChildClassName() const;";
    s+= "virtual const cbyte* GetRaw() const;";
    s+= "virtual const char *AsPChar() const;";
    s+= "virtual bool IsA(const char *ClassName) const;";
    s+= "virtual bool Event(EObject EventNumber,Context::IO &Context);";
    //    s+= "CryObject *LoadItem(CryStream &FromStream);";
    s+= "virtual CryString *GetFunctions() const;";
    s+= "virtual bool IsContainer() const;";
    s+= "virtual CryProperty *GetPropertyAsCryProperty(const char *PropertyName) const;";
    s+= "virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const = 0;";
    s+= "virtual bool HasProperty(const char *PropertyName) const;";
    s+= "virtual int GetPropertyCount() const;";
    s+= "virtual CryList *PropertyNames() const;";
    s+= "virtual bool SetProperty(const char *PropertyName,const char *PropertyValue) = 0;";
    s+= "virtual bool SetPropertyAsObject(CryProperty *Value);";
    s+= "virtual void SaveTo(CryStream &ToStream) const;";
    s+= "virtual void LoadFrom(CryStream &FromStream);";
    s+= "virtual CryObject *Create(CryStream &FromStream);";
    s+= "virtual CryObject *Create(const char *Type,CryObject *Parent=0);";
    s+= "bool IterateThroughAll(CryContainer *Container,EmptyObject *Control);";
    s+= "virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item);";
#ifdef VALIDATING

    s+= "virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));";
#endif

    CryFunctionDefList *l = new CryFunctionDefList();
    l->LoadFromString(s,";");
    return l;
}

CryFunctionDefList *CryObject::GetAbstractFunctions(const char *Type) const
{
    // pure and abstract mean the same thing
    // first get all the functions (from the derived childclass)
    CryFunctionDefList *l = GetFunctions(Type);
    CryFunctionDefList::ListIterator	*it =(CryFunctionDefList::ListIterator *) l->_CreateIterator();
    CryFunctionDefList *Return = new CryFunctionDefList();
    CryFunctionDef *s,*s1;
    if (it->GotoFirst())
        do
        {
            s = (CryFunctionDef *)it->Get();
            if (!s->IsPure)	// found one that isn't pure, change any pure ones following to non-pure
            {
                CryFunctionDefList::ListIterator *it1 = 
			(CryFunctionDefList::ListIterator *)it->Dup();
                if (it1->GotoNext()) // check all functions following
                    do
                    {
                        s1 = (CryFunctionDef *)it1->Get();
                        if (s1->GetNPDeclaration()==s->GetNPDeclaration())	// same function def
                            s1->IsPure = false;
                    }
                    while(it1->GotoNext());
                l->DeleteIterator(it1);
            }
        } while(it->GotoNext());
    // now any pure functions are unique
    if (it->GotoFirst())
        do
        {

            s = (CryFunctionDef *)it->Get();
            if (s->IsPure)
            {
                CryFunctionDef *n = new CryFunctionDef(*s);
                Return->AddOwned(n);
            }
        }
        while(it->GotoNext());
    l->DeleteIterator(it);
    delete l;
    return Return;
}



const cbyte* CryObject::GetRaw() const
{
    throw CryException(this,"Cannot GetRaw from CryObject");
}

bool CryObject::GetIsPropertyContainer(const CryPropertyParser &PropertyName) const
{
    if (HasProperty(PropertyName))
        return false; // generally properties are strings containing values, special case needs to be handled specially
    throw CryException(this,ExceptionUnknownProperty,"Unknown Property \"%s\"",PropertyName.AsPChar());
}

/// Get a property
CryObject *CryObject::GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const
{
#ifdef DEBUG
	if (PropertyName=="ObjectID")
	{
	CryString *s = new CryString();
		s->printf("%d",ObjectID);
		return s;
	}
#endif
	if (HasProperty(PropertyName))
	{
		throw CryException(this,ExceptionNotImplemented,"\"%s\" has not implemented GetCopyOfPropertyAsObject for Property \"%s\"",
						   ChildClassName(),PropertyName.AsPChar());
	}
	else
		throw CryException(this,ExceptionUnknownProperty,"Unknown Property \"%s\"",PropertyName.AsPChar());
}
/// Get a property
CryObject *CryObject::_GetPropertyAsObject(const CryPropertyParser &PropertyName) const
{
	if (HasProperty(PropertyName))
	{
		throw CryException(this,ExceptionNotImplemented,"\"%s\" has not implemented _GetPropertyAsObject for Property \"%s\"",
						   ChildClassName(),PropertyName.AsPChar());
	}
	else
		throw CryException(this,ExceptionUnknownProperty,"Unknown Property \"%s\"",PropertyName.AsPChar());
}

const char *CryObject::GetProperty(const char *PropertyName,CryString &Result) const
{
	CryPropertyParser pp(PropertyName);
	return GetProperty(pp,Result);

}

const char *CryObject::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
#ifdef DEBUG
	if (PropertyName=="ObjectID")
	{
		Result.printf("%d",ObjectID);
		return (const char *)Result;
	}
#endif
	throw CryException(this,ExceptionUnknownProperty,"Unknown Property \"%s\"",PropertyName.AsPChar());
}

bool CryObject::SetPropertyAsObject(CryProperty *Value)
{
CryString Result;
	return SetProperty(Value->GetName()->AsPChar(),Value->GetValue(Result));
}
/*CryObject *CryObject::Dup() const
{
    throw CryException(this,"Cannot Dup a CryObject");
} */


CryObject::CryObject()
{
#ifdef DEBUG
	ObjectID = ::ObjectID;
	::ObjectID++;
#endif
}

const char* CryObject::ClassName() const
{
    return TCryObject;
}
const char* CryObject::ChildClassName() const
{
    return TCryObject;
}

bool CryObject::IsA(const char *_ClassName) const // can the object map to a ClassName
{
    return strcmp(_ClassName,TCryObject)==0;
}

// returns true if handled
bool CryObject::Event(EObject EventNumber,Context::IO &Context)
{
    switch(EventNumber)
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
        CopyTo(*Context.In.InCopyTo.Object);  //copies contents of this to Dest
        break;
EDup:
        Context.Out.OutDup.Object = Dup();
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

bool CryObject::ClassCanCreate(const CryPropertyParser &PropertyName)
{
    return (PropertyName==TCryMemStream) ||
           (PropertyName==TCryXML) ||
           (PropertyName==TCryXMLNode) ||
           (PropertyName==TCryFileStream) ||
           (PropertyName==TCryString) ||
           (PropertyName==TCryProperty) ||
           (PropertyName==TCryList) ||
           (PropertyName==TCryPropertyList) ||
           (PropertyName==TCryDoubleArray) ||
           (PropertyName==TCryFunctionDef) ||
           (PropertyName==TCryFunctionDefList) ||
           (PropertyName==TCryFuzzy) ||
           (PropertyName==TBitArray) ||
           (PropertyName==TCryBPNetContainer) ||
           (PropertyName==TBackPropagateLayer) ||
           (PropertyName==THugeInt);
}


/*!
	Create can create 
	CryMemStream,
	CryFileStream,
	CryString,
	CryProperty,
	CryList,
	CryPropertyList,
	CryDoubleArray,
	CryFuzzy,
	CryBPNetContainer
	BackPropagateLayer
	HugeInt
*/
CryObject *CryObject::ClassCreate(const CryPropertyParser &PropertyName,CryObject *Parent)
{
    CryObject *NewObject = 0;
    // first create the object
    if (PropertyName==TCryObject)
        NewObject = new CryObject();
    else
        if (PropertyName==TCryMemStream)
            NewObject =(CryObject *) new CryMemStream();
        else
            if (PropertyName==TCryXML)
                NewObject = (CryObject *)new CryXML();
            else
                if (PropertyName==TCryXMLNode)
                    NewObject = (CryObject *)new CryXMLNode();
                else
                    if (PropertyName==TCryFileStream)
                        NewObject = (CryObject *)new CryFileStream();
                    else
                        if (PropertyName==TCryString)
                            NewObject = (CryObject *)new CryString();
                        else
                            if (PropertyName==TCryProperty)
                                NewObject = (CryObject *)new CryProperty("NoName");
                            else
                                if (PropertyName==TCryList)
                                    NewObject = (CryObject *)new CryList();
                                else
                                    if (PropertyName==TCryPropertyList)
                                        NewObject = (CryObject *)new CryPropertyList();
                                    else
                                        if (PropertyName==TCryDoubleArray)
                                            NewObject = (CryObject *)new CryDoubleArray();
                                        else
                                            if (PropertyName==TCryFunctionDef)
                                                NewObject = (CryFunctionDef *)new CryFunctionDef();
                                            else
                                                if (PropertyName==TCryFunctionDefList)
                                                    NewObject = (CryFunctionDefList *) new CryFunctionDefList();
                                                else
                                                    if (PropertyName==TCryFuzzy)
                                                        NewObject = (CryObject *)new CryFuzzy();
                                                    else
                                                        if (PropertyName==TBitArray)
                                                            NewObject = (BitArray *) new BitArray();
                                                        else
                                                            /*if (PropertyName=="CryBPNet")
                                                               (CryBPNet *) NewObject = new CryBPNet();
                                                            else*/
                                                            if (PropertyName==TStrategy)
                                                                NewObject = new Strategy();
                                                            else
                                                                if (PropertyName==TStrategyHolder)
                                                                    NewObject = new StrategyHolder();
                                                                else
                                                                    if (PropertyName==TStrategyHolderSender)
                                                                        NewObject = new StrategyHolderSender();
                                                                    else
                                                                        if (PropertyName==TObserver)
                                                                            NewObject = new Observer();
                                                                        else
                                                                            if (PropertyName==TObservable)
                                                                                NewObject = new Observable();
                                                                            else
                                                                                if (PropertyName==TDecorator)
                                                                                {
                                                                                    if (Parent==0 || Parent->IsA(TDecorator))
                                                                                        NewObject = new Decorator((Decorator *)Parent);
                                                                                }
                                                                                else
                                                                                    if (PropertyName==TCryFactory)
                                                                                        NewObject = new CryFactory();
                                                                                    else
                                                                                        if (PropertyName==TCryOFactory)
                                                                                            NewObject = new CryOFactory();
                                                                                        else
                                                                                            if (PropertyName==TCommandHolder)
                                                                                                NewObject = new CommandHolder();
                                                                                            else
                                                                                                if (PropertyName==TCommandHolderSender)
                                                                                                    NewObject = new CommandHolderSender();
                                                                                                else
                                                                                                    if (PropertyName==TCompositeIterator)
																										NewObject = new CompositeIterator(0);
                                                                                                    else
                                                                                                        if (PropertyName==TState)
                                                                                                            NewObject = new State(1);

    if (PropertyName==TCryBPNetContainer)
        NewObject = (CryObject *)new CryBPNetContainer();
    else
        if (PropertyName==TBackPropagateLayer)
        {
            if (Parent && (Parent->IsA(TCryBPNet)))
            {
                NewObject = (CryObject *) ((CryBPNet *)Parent)->AddLayer(0);
            }
            else
                throw CryException("Cannot Create BackPropagateLayer, Must have a parent of %s (or a decendent)",TCryBPNet);
        }
        else
            if (PropertyName==THugeInt)
                NewObject = (CryObject *)new HugeInt();
            else
                if (Parent)
                    NewObject = Parent->Create(PropertyName,0);
	if (!NewObject)
	{
		throw CryException("Could not Create Class %s (maybe abstract?)",(const char *)PropertyName);
	}
    return NewObject;
}



void CryObject::SaveTo(CryStream &ToStream) const
{
    CryXML xml;
    xml.LoadFrom(*this);

    switch(ToStream.GetMode())
    {
    case SObject:
        // zlib compression of xml class
        {
            CryString w;
            xml.SaveTo(w);
            CryMemStream c;
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
    case SText:
        xml.SaveTo(ToStream);
        break;
    }
}

void CryObject::LoadFrom(const CryStream &FromStream)
{
    CryXML xml;
    switch(FromStream.GetMode())
    {
    case SObject:
        // zlib compression of xml class
        {
            int l;
            FromStream.Read((char *)&l,sizeof(l));
            if (l!=0)   // get marker
                throw CryException(this,"Compressed Read from non-compressed format");
            FromStream.Read((char *)&l,sizeof(l));  // original length
            CryString w;
            w.SetLength(l);
            CryMemStream c;
            FromStream.Read((char *)&l,sizeof(l));
            c.SetLength(l);
            c.Write(&FromStream,l);
            //            zlibuncompress(c,w);
            xml.LoadFrom(c);
        }
        break;
    case SText:
        xml.LoadFrom(FromStream);
    }
    return xml.SaveTo(*this);
}
CryObject *CryObject::Create(CryStream &FromStream)
{
    CryXML xml;
    switch(FromStream.GetMode())
    {
    case SObject:
        // zlib compression of xml class
        {
            int l;
            FromStream.Read((char *)&l,sizeof(l));
            if (l!=0)   // get marker
                throw CryException(this,"Compressed Read from non-compressed format");
            FromStream.Read((char *)&l,sizeof(l));  // original length
            CryString w;
            w.SetLength(l);
            CryMemStream c;
            FromStream.Read((char *)&l,sizeof(l));
            c.SetLength(l);
            c.Write(&FromStream,l);
            //            zlibuncompress(c,w);
            xml.LoadFrom(c);
        }
        break;
    case SText:
        xml.LoadFrom(FromStream);
    }
    return  xml.CreateObjectFromNode(this);
}

CryPropertyList *CryObject::PropertyNames() const
{
	CryPropertyList *n = new CryPropertyList();
#ifdef DEBUG
	n->AddPropertyByName("ObjectID",this);
#endif
    return n;
}

CryProperty *CryObject::GetPropertyAsCryProperty(const CryPropertyParser &PropertyName) const
{
    if (HasProperty(PropertyName))
    {
        CryProperty *o = new CryProperty(PropertyName);
        CryString s;
        const char *c =GetProperty(PropertyName,s);
        if ((*c=='*' ) && (s=="[]"))
        {
		// usually s and *c will be the same, but in this case it indicates an array
            s.Clear();
            s.printf("*%s",PropertyName.AsPChar());
		// we now get what is at the array
			GetProperty(s.AsPChar(),s);
        }
        o->SetValue(s.AsPChar());
        return o;
    }
    throw CryException(this,ExceptionUnknownProperty,"Unknown Property \"%s\"",PropertyName.AsPChar());
}


bool CryObject::IterateThroughAll(CryContainer *Container,EmptyObject *Control) // for each item, will call bool (*FunctionToCall)(EmptyObject *); (unless 0)
{
	CryContainer::Iterator *i = Container->_CreateIterator();
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
        while(i->GotoNext());
    }
    Container->DeleteIterator(i);
    return result;
}
CryObject *CryObject::CreateItemType(const CryPropertyParser &PropertyName)
{
    CryObject *NewObject;
    if (PropertyName==TCryList)
        NewObject = new CryList();
    else
        if (PropertyName==TCryFileStream)
            NewObject = new CryFileStream();
        else
            if (PropertyName==TCryMemStream)
                NewObject = new CryMemStream();
            else
                if (PropertyName==TCryString)
                    NewObject = new CryString();
                else
                    if (PropertyName==TStrategy)
                        NewObject = new Strategy();
                    else
                        if (PropertyName==TStrategyHolder)
                            NewObject = new StrategyHolder();
                        else
                            if (PropertyName==TStrategyHolderSender)
                                NewObject = new StrategyHolderSender();
                            else
                                if (PropertyName==TObserver)
                                    NewObject = new Observer();
                                else
                                    if (PropertyName==TObservable)
                                        NewObject = new Observable();
                                    else
                                        if (PropertyName==TDecorator)
                                            NewObject = new Decorator(0);
                                        else
                                            if (PropertyName==TCryFactory)
                                                NewObject = new CryFactory();
                                            else
                                                if (PropertyName==TCryOFactory)
                                                    NewObject = new CryOFactory();
                                                else
                                                    if (PropertyName==TCommandHolder)
                                                        NewObject = new CommandHolder();
                                                    else
                                                        if (PropertyName==TCommandHolderSender)
                                                            NewObject = new CommandHolderSender();
                                                        else
                                                            if (PropertyName==TCompositeIterator)
                                                                NewObject = new CompositeIterator(0);
                                                            else
                                                                if (PropertyName==TState)
                                                                    NewObject = new State(1);
                                                                else
                                                                    if (PropertyName==TCryStream)
                                                                        throw CryException(this,"Trying to create instance of Abstract Class CryStream");
                                                                    else
                                                                        throw CryException(this,"Trying to create instance of Unknown Class %s",PropertyName.AsPChar());
    return NewObject;
}



CryObject *CryOwnedObject::Create(const CryPropertyParser &PropertyName,CryObject *Parent)
{
	try
	{
		CryObject *o = CryObject::Create(PropertyName,Parent);
		return o;
	}
	catch(CryException &e)
	{
		if (Owner)
		{
			return Owner->Create(PropertyName,Parent);
		}
		throw e;
	}
}













