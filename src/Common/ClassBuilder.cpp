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
#include "ClassBuilder.h"
#include "CryXML.h"
#include "CryBackProp.h"
#include "ClassHeader.h"
#include "ClassFooter.h"
#include "ClassContainer.h"
#include "PrimInstance.h"
#include "HeadImp.h"
#include "ClassInstance.h"
#include "ClassFunction.h"
#include "ClassFileStream.h"
#include "HugeInt.h"
#include "HugeDouble.h"
#include "BitArray.h"
#include "CryPattern.h"

//#define DIAGNOSE

using namespace Crystal;
using namespace std;

// the following classes are here only in order to create an instance of the abstract
// classes they are subclassed from. Only functions where are defined in the base classes
// are to be used.

/// Dummy class with stubs for virtual functions
class CryObjectNAbstract : public Object
{
    virtual void CopyTo(Object &Dest) const
        {}
    ;
    virtual Object *Dup() const
    {
        return 0;
    }
	virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const
    {
        return  "";
	}

};

/// Dummy class with stubs for virtual functions
class CryContainerNAbstract : public Container
{
	virtual EmptyObject *Add(EmptyObject *Item,size_t Size)
	{
		return 0;
	}
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0) 
	{
	}

    virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size)
    {
        return 0;
    }
    virtual Object *Dup()const
    {
        return 0;
    }
    virtual Object *Add(Object *Item)
    {
        return  0;
    }
    virtual Object *AddOwned(Object *Item)
    {
        return 0;
    }
    virtual void SetItemOwnerShip(Iterator *I,bool Owned)
    {}
    virtual bool GetItemOwnerShip(const Iterator *I) const
    {
        return false;
    }
    virtual size_t GetItemSize(Iterator *I) const
    {
        return 0;
    }
    virtual bool IsObject(const Iterator *I) const
    {
        return false;
    }
    virtual bool LoadAsText(Iterator *I,String &FromStream)
    {
        return false;
    }
    virtual bool SaveAsText(Iterator *I,String &ToStream) const
    {
        return false;
    }
    virtual void GetEleType(String &Result) const
        {}
    ;
    virtual Iterator *_CreateIterator() const
    {
        return 0;
    };
    virtual void DeleteIterator(Iterator *I) const
		{}
	;
	virtual bool GotoFirst(Iterator *I) const
	{
		return false;
	}
	;   // returns true if success
	virtual bool GotoPrev(Iterator *I) const
	{
		return false;
	}
	;   // returns true if success
	virtual bool GotoNext(Iterator *I) const
	{
		return false;
	}
	;    // returns true if success
	virtual bool GotoLast(Iterator *Iterator) const
	{
		return false;
	}    // returns true if success
	virtual bool IsEmpty(const Iterator *I)const
	{
		return true;
	}

	virtual EmptyObject *GetAtIterator(const Iterator *I) const
	{
		return 0;
	}
	virtual void RemoveAtIterator(Iterator *LI)
	{
		return;
    }

    virtual size_t Count() const
    {
        return 0;
    }
    //virtual bool CanDup() const { return false; }	// Containers might contain things that can't be duped, so we must assume they can't
    virtual void Clear()
    {}
    ;
    virtual void CopyTo(Object &Dest) const
        {}
    ;
    virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const
    {
        return  "";
    }

};

/// Dummy class with stubs for virtual functions
class CryStreamNAbstract : public Stream
{
    virtual void Clear()
    {}
    ;
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0) 
	{
	}

    virtual Iterator *_CreateIterator() const
    {
        return 0;
    };
    virtual void DeleteIterator(Iterator *I) const
        {  }
    virtual bool GotoFirst(Iterator *I) const
    {
        return false;
    }
    virtual bool GotoPrev(Iterator *I) const
    {
        return false;
    }
    ; // returns true if success
    virtual bool GotoNext(Iterator *I) const
    {
        return false;
    }
    ;    // returns true if success
    virtual bool GotoLast(Iterator *Iterator) const
    {
        return false;
    }
    ;    // returns true if success
    virtual bool IsEmpty(const Iterator *I)const
    {
        return true;
    }

    virtual EmptyObject *GetAtIterator(const Iterator *I) const
    {
        return 0;
    }

    /// abstract function used by subclasses to remove the item this iterator points at (iterator is still valid)
    /// if is not owned then is returned, else is deleted and null is returned
	virtual void RemoveAtIterator(Iterator *I)
	{
        return;
    }
    /// abstract function used by subclasses to count the number of items held by the container
    virtual size_t Count() const
    {
        return 0;
    }

    virtual EmptyObject *Add(EmptyObject *Item,size_t Size)
    {
        return 0;
    }
    virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size)
    {
        return 0;
    }
    virtual Object *Add(Object *Item)
    {
        return Item;
    }    // returns Item
    virtual Object *AddOwned(Object *Item)
    {
        return Item;
    }   // gives ownership to list
    virtual void SetItemOwnerShip(Iterator *I,bool Owned)
    {} // ignored we always own, as it's a copy
    virtual bool GetItemOwnerShip(const Iterator *I) const
    {
        return true;
    }  // always
    virtual size_t GetItemSize(Iterator *I) const
    {
        return 1;
    }
    virtual bool IsObject(const Iterator *I) const
    {
        return false;
    }
    virtual bool LoadAsText(Iterator *I,String &FromStream)
	{
        return false;
    }
    ;
    virtual bool SaveAsText(Iterator *I,String &ToStream) const
    {
        return false;
    }
    ;

    virtual int Seek(int offset,int whence) const
    {
        return 0;
    }
    virtual int SeekFromStart(int Offset=0) const
    {
        return 0;
    }
    virtual int SeekFromCurrent(int Offset) const
    {
        return 0;
    }
    virtual int SeekFromEnd(int Offset=0) const
    {
        return 0;
    }
    virtual size_t Read(char *ToBuffer,size_t Size) const
    {
        return 0;
    }
    virtual size_t Write(const char *FromBuffer,size_t Size)
    {
        return 0;
    }
    virtual size_t Read(Stream *ToStream,size_t Size) const
    {
        return 0;
	}
    virtual size_t Read(Stream *ToStream) const
    {
        return 0;
    }
    virtual size_t Write(const Stream *FromStream)
    {
        return 0;
    }
    virtual size_t Write(const Stream *FromStream,size_t Size)
    {
        return 0;
    }
    virtual size_t Size() const
    {
        return 0;
    }
    virtual size_t Tell() const
    {
        return 0;
    }
    virtual bool Eof() const
    {
        return true;
    }
    virtual bool Open(const char *Name,const char *Operation,bool ExceptOnError=true)
    {
        return true;
    }
    virtual void Close(bool ExceptOnError=true)
    {}
    virtual bool IsOpen() const
    {
        return true;
    }
    virtual void Flush()
    {}
	;
    virtual int scanf(const char *format,...) const
    {
        return 0;
    }
    virtual size_t printf(const char *format,...)
    {
        return 0;
    }
    Object *Dup() const
    {
        return 0;
    }
};

/// Dummy class with stubs for virtual functions
class CrySimpleArrayNAbstract : public SimpleArray
{
    virtual void CopyTo(Object &Dest) const
        {}
    ;
    virtual Object *Dup() const
    {
        return 0;
    }
	virtual void RemoveAtIterator(Iterator *LI)
	{
        return;
    }
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0) 
	{
	}
	virtual void SetItem(unsigned int i,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
	{

	}

	virtual bool IsObject(const Iterator *I) const
    {
        return false;
    }
    virtual EmptyObject *Add(EmptyObject *Item,size_t Size)
    {
        return 0;
    }
    virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size)
    {
        return 0;
    }
    virtual Object *Add(Object *Item)
    {
        return  0;
    }
    virtual Object *AddOwned(Object *Item)
    {
        return 0;
    }
	void SetSize(size_t _Size)
	{
	}
    virtual bool LoadAsText(int i,String &FromStream)
    {
        return false;
    };
    virtual bool SaveAsText(int i,String &ToStream) const
    {
        return false;
    };
    virtual void GetEleType(String &Result) const
        {}
    ;
    virtual bool IsEmpty(const Iterator *I)const
    {
        return true;
    }

    virtual EmptyObject *GetAtIterator(const Iterator *I) const
    {
        return 0;
    };
    virtual void Clear()
    {}
    ;
    virtual bool LoadAsText(Iterator *I,String &FromStream)
    {
        return true;
    }
    virtual bool SaveAsText(Iterator *I,String &ToStream) const
    {
        return true;
    }
    virtual void SetItemOwnerShip(Iterator *I,bool Owned)
    {}
    ;
    virtual bool GetItemOwnerShip(const Iterator *I) const
    {
        return false;
    };
    virtual size_t GetItemSize(Iterator *I) const
    {
        return 0;
    };

};



/// Dummy class with stubs for virtual functions
class CryArrayNAbstract : public Array
{
    virtual EmptyObject *Add(EmptyObject *Item,size_t Size)
    {
        return 0;
    }
    virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size)
    {
        return 0;
    }
    virtual Object *Add(Object *Item)
    {
        return  0;
    }
    virtual Object *AddOwned(Object *Item)
    {
        return 0;
    }
    virtual Object *Dup()const
    {
        return 0;
    }
    virtual void DestroyArrayItem(Array *Owner,EmptyObject *Item)
    {}
    ;
    virtual EmptyObject *CreateArrayItem(Array *Owner,bool *IsObject)
    {
        return 0;
    };
    virtual void SaveItemTo(const Array *Owner,EmptyObject *FromItem,Stream &ToStream) const
        {}
    ;
    virtual EmptyObject *LoadItemFrom(Array *Owner,EmptyObject *ToItem,Stream &FromStream)
    {
        return 0;
    };
    virtual bool LoadAsText(int i,String &FromStream)
    {
        return false;
    };
    virtual bool SaveAsText(int i,String &ToStream) const
    {
        return false;
    };
    virtual void GetEleType(String &Result) const
        {}
    ;
    virtual bool LoadAsText(Iterator *I,String &FromStream)
    {
        return true;
    }
    virtual bool SaveAsText(Iterator *I,String &ToStream) const
    {
        return true;
    }
};

void ClassBuilder::SetDefaultBodies(FunctionDefList *AbstractFunctions) // will setup default bodies of functions
{
    HeaderFactory *hf = (HeaderFactory *) Create(CHeaderFactory,this);
	ClassHeaderFactory *cf = (ClassHeaderFactory *)Create(CClassHeaderFactory,hf);
	if (hf && AbstractFunctions)
	{
		List::Iterator *i = AbstractFunctions->_CreateIterator();
		if (i->GotoFirst())
			do
			{
				//                AbstractFunctions->SetItemOwnerShip(i,false);     /// I'll take that!
				FunctionDef *s = (FunctionDef *) i->Get();
				try
				{
					String Def;
					s->GetDeclaration(Def);
					if (/*s->IsPure && */(!s->IsComment) && !cf->Present(s))
					{
						String Declare = s->GetNPDeclaration();
						InheritedFactory *If = (InheritedFactory *)Create(CInheritedFactory,hf);
						If->SetProperty("Function",Declare);
					}
				}
				catch(Exception &e)
				{
					// If function already added, then don worry bout it
				}

			}
			while(i->GotoNext());
		AbstractFunctions->DeleteIterator(i);
	}
	//    AddFactory(new HeaderFactory(this,AbstractFunctions));

	{
		// Common functions that are always needed so we fill them in to make life easier

		/*        _Ele *e;
				CryFunctionDef *s;
				try
				{
					CryFunctionDef fd;
					fd.Parse("virtual CryFunctionDefList *GetFunctions(const char *Type=0) const");
					e = AddProperty(AbstractFunction,"Inherited",fd.GetDeclaration().AsPChar() ,"",0,"","",false);
					e->SetFunction(fd);
				}
				catch(CryException &e)
				{
					// If function already added, then don worry bout it
				}

				try
				{
					CryFunctionDef fd("virtual size_t Size() const");
					e = AddProperty(AbstractFunction,"Inherited",fd.GetDeclaration().AsPChar(),"",0,"","",false);
					e->SetFunction(fd);
				}
				catch(CryException &e)
				{
					// If function already added, then don worry bout it
				}

				try
				{
					CryFunctionDef fd("virtual CryObject *Dup() const");
					e = AddProperty(AbstractFunction,"Inherited",
									fd.GetDeclaration().AsPChar(),"",0,"","",false);
					e->SetFunction(fd);
				}
				catch(CryException &e)
				{
					// If function already added, then don worry bout it
				}
				try
				{
					CryFunctionDef fd("virtual void CopyTo(CryObject &Dest) const");
					e = AddProperty(AbstractFunction,"Inherited",
									fd.GetDeclaration().AsPChar(),"",0,"","",false);
					e->SetFunction(fd);
				}
				catch(CryException &e)
				{
					// If function already added, then don worry bout it
				}*/
	}
}


/*===========================================================================
// ClassBuilder
============================================================================*/
ClassBuilder::ClassBuilder() : CodeFactory(0,"ClassBuilder")
{
	p = 0;
	AddProduct("Source");
}

ClassBuilder::~ClassBuilder()
{
	delete p;
}
const Object *ClassBuilder::Getp() const
{
    return p;
}

/*void ClassBuilder::WriteHeader(const char *text)
{
    HeadImp.SetHead(text);
}
void ClassBuilder::WriteImp(const char *text)
{
    HeadImp.SetImp(text);
}*
void ClassBuilder::WriteHI(const char *text)
{
    WriteHeader(text);
    WriteImp(text);
}*/
void ClassBuilder::SetBaseClass(const char *Type,bool AddStubs,bool AddVirtuals,const char *_ClassName)
{
    Object *p1;
    SetName(_ClassName);
    FunctionDefList *AbstractFunctions = 0;
    try
    {
        p1 = Object::Create(Type);
        _IsAbstract = false;
    }
    catch(Exception &e)
    {
        _IsAbstract = true;
        p1 = new CryObjectNAbstract();
        if (!p1->IsA(Type))
        {
            delete p1;
            p1 = 0;
        }
        if (!p1)
        {
			p1 = new CryContainerNAbstract();
            if (!p1->IsA(Type))
            {
                delete p1;
                p1 = 0;
            }
        }
        if (!p1)
        {
            p1 = new CryStreamNAbstract();
            if (!p1->IsA(Type))
            {
                delete p1;
                p1 = 0;
            }
        }
        if (!p1)
        {
            p1 = new CryArrayNAbstract();
            if (!p1->IsA(Type))
            {
                delete p1;
                p1 = 0;
            }
        }
        if (!p1)
        {
			p1 = new CrySimpleArrayNAbstract();
            if (!p1->IsA(Type))
            {
                delete p1;
                p1 = 0;
            }
        }
        if (!p1)
        {
            throw Exception("Could not create abstract function");
        }
    };
    if (p)
        delete p;
    p = p1;
    if (p->IsContainer())
        this->AddFactory(new ContainerFactory( this,(Container *)p));
    if (AddStubs)
        AbstractFunctions = p->GetFunctions(Type);
    else
        if (_IsAbstract) // if the base class wasn't able to be created because it was abstract, then we need to add the functions that are missing
            AbstractFunctions = p->GetAbstractFunctions(Type);
    /* We add these functions just to make things easier */
    if (!AbstractFunctions)
        AbstractFunctions = new FunctionDefList();
	AbstractFunctions->AddOwned(new FunctionDef(
								"virtual CryPropertyList *PropertyNames() const;"));
	AbstractFunctions->AddOwned(new FunctionDef(
							   "virtual void CopyTo(CryObject &Dest) const;"));
	AbstractFunctions->AddOwned(new FunctionDef(
								"virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);"));
	AbstractFunctions->AddOwned(new FunctionDef(
							"virtual const char *GetProperty(const char *PropertyName,CryString &Result) const;"));
	//AbstractFunctions->Add(new CryFunctionDef(
    //                         "virtual CryObject *Dup() const = 0;"));

    InheritType = Type;
    AssignInheritedElements();
    if (AddVirtuals)
        SetDefaultBodies(AbstractFunctions);
    else
        SetDefaultBodies(0);
    delete AbstractFunctions;
}

void ClassBuilder::AddClassInstance(const char *ClassType,const char *ClassName,int count,bool IsProperty,bool IsPointer,bool IsArrayPointer,bool HasDefault,const char *DefaultValue)
{
    /* TODO : Check for Property Name already present */
    //AddFactory(new LocalVariablesFactory());
    CompositeIterator a(this);
    if (a.GotoFirst())
        do
        {
            CryFactory *o = (CryFactory *)a.Get();
			if (o->IsA(CClassHeaderFactory))
            {
				ClassHeaderFactory *chf = (ClassHeaderFactory *)o;
                String n;
                n = ClassName;
                if (chf->Present(&n))
					throw Exception("Component name \"%s\" is already being used");
				ClassInstance *i = new ClassInstance(chf);
				try
				{
				i->SetProperty("Type",ClassType);
				i->SetProperty("Name",ClassName);
				n.Clear();
				n.printf("%d",count);
				i->SetProperty("Count",n);
				i->SetProperty("IsProperty",IsProperty ? "Yes" : "No");
				i->SetProperty("IsPointer",IsPointer ? "Yes" : "No");
				i->SetProperty("IsArrayPointer",IsArrayPointer ? "Yes" : "No");
				if (HasDefault)
				{
					i->SetProperty("HasDefault","Yes");
					i->SetProperty("DefaultValue",DefaultValue);
				}
				chf->AddFactory(i);
				}
				catch(Exception &s)
				{
					delete i;
					throw s;
				}
				break;
			}
		}
		while(a.GotoNext());

}
void ClassBuilder::AddPrimInstance(const char *PrimType,
								   const char *PrimName,const char *DefaultValue, int Count,bool IsProperty,bool IsPointer,bool IsArrayPointer)
{           /* TODO : Check for Property Name already present */
	//AddFactory(new LocalVariablesFactory());
	CompositeIterator a(this);
	if (a.GotoFirst())
		do
		{
			CryFactory *o = (CryFactory *)a.Get();
			if (o->IsA(CClassHeaderFactory))
            {
                ClassHeaderFactory *chf = (ClassHeaderFactory *)o;
                String n;
                n = GetName();
                if (chf->Present(&n))
                    //		if (chf->Present(ClassName))
					throw Exception("Component name \"%s\" is already being used");
				chf->AddFactory(new PrimInstance(chf,PrimType,PrimName,DefaultValue,Count,IsProperty,IsPointer,IsArrayPointer));
			}
//                break;
		}
        while(a.GotoNext());
}

Object *ClassBuilder::Create(Stream &FromStream)
{
    throw Exception("Not Implemented");
}


Object *ClassBuilder::Create(const PropertyParser &PropertyName,Object *Parent)
{
    return CodeFactory::Create(PropertyName,(Object *)Parent);
}

Object *ClassBuilder::Create(const PropertyParser &PropertyName,CodeFactory *Parent)
{
	if (PropertyName==CContainerFactory && p)
    {
        ContainerFactory *cf = new ContainerFactory( this,(Container *)p);
        AddFactory(cf);
        return cf;
    }
	if (PropertyName==CPropertyFactory)
    {
        PropertyFactory *pf = (PropertyFactory *) FindFactory(PropertyName);
        if (!pf)
        {
            String Name,Value;
            pf = new PropertyFactory(this,&Name,&Value);
            AddFactory(pf);
        }
        return pf;
    }

	if (PropertyName==CHeaderFactory)
    {
		HeaderFactory *hf = (HeaderFactory *) FindFactory(CHeaderFactory);
        if (!hf)
        {
            hf = new HeaderFactory(this,0);
            AddFactory(hf);
        }
        return hf;
    }
    return CodeFactory::Create(PropertyName,Parent);
}

int ClassBuilder::GetPropertyCount() const
{
    return CodeFactory::GetPropertyCount()+1;
}
/// will return a pointer to a dup of the property
Object *ClassBuilder::GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const
{
	return CodeFactory::GetCopyOfPropertyAsObject(PropertyName);
}
Object *ClassBuilder::_GetPropertyAsObject(const PropertyParser &PropertyName) const
{
	return CodeFactory::_GetPropertyAsObject(PropertyName);
}

bool ClassBuilder::GetIsPropertyContainer(const PropertyParser &PropertyName) const
{
    if (PropertyName=="ClassType")
        return false;
    else
        return CodeFactory::GetIsPropertyContainer(PropertyName);
}
const char * ClassBuilder::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
    if (PropertyName=="ClassType")
    {
        Result = p->ChildClassName();
        return Result.AsPChar();
    }
    return CodeFactory::GetProperty(PropertyName,Result);
}

bool  ClassBuilder::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
    if (PropertyName=="ClassType")
    { // we don't add stubs here because this is coming from a saved version, which will have already defined them
        String Name;
        Object::GetProperty("Name",Name);
        SetBaseClass(PropertyValue,false,false,Name);
        return true;
    }
    return CodeFactory::SetProperty(PropertyName,PropertyValue);

}

CryPropertyList *ClassBuilder::PropertyNames() const
{
	CryPropertyList *pn = CodeFactory::PropertyNames();
	pn->AddPropertyByName("ClassType",this);
	return pn;
};
bool ClassBuilder::HasProperty(const PropertyParser &PropertyName) const
{
	return (PropertyName=="ClassType") ||
           CodeFactory::HasProperty(PropertyName);
}

void ClassBuilder::Build()
{
    Create("Header",this);
}
void ClassBuilder::AssignInheritedElements()
{
CryPropertyList *Properties = p->PropertyNames();

	CryPropertyList::PropertyIterator *pi =
		(CryPropertyList::PropertyIterator *)Properties->_CreateIterator();

	if (pi->GotoFirst())
		do
		{
			const String *name = pi->GetName();
			String r;
//			const char *value = pi->GetValue(r);
			this->AddFactory(new PropertyFactory(this,name,&r));
		}
		while(pi->GotoNext());
	Properties->DeleteIterator(pi);
	delete Properties;
}


void ClassBuilder::Remove(const char *Description)
{
    DeleteFactoryDescribed(Description);
}
void ClassBuilder::SetFilename(const char *name)
{
    Filename = name;
    Filename.Replace(".xml");
}//no extention on filename, eg stdio not stdio.h
const String &ClassBuilder::GetFilename() const
{
    return Filename;
}
/*const char *ClassBuilder::GetName()
{
    return ClassName.AsPChar();
}
void ClassBuilder::SetName(const char *NewClassName)
{
    if (NewClassName && *NewClassName)
        ClassName = NewClassName;
}*/
const char *ClassBuilder::GetInheritType()
{
    return InheritType.AsPChar();
}
void ClassBuilder::SaveHeaderBody(Stream &ToStreamHeader,Stream &ToStreamBody)
{
    Clear("Source");
    Build();
    ToStreamHeader.Clear();
    ToStreamBody.Clear();
    ToStreamHeader.Write(GetHead("Source"));
    ToStreamBody.Write(GetImp("Source"));
}

void ClassBuilder::SaveSource()	// will save the source under Filename.h and Filename.cpp and Filename.xml
{
/*{
CompositeIterator  ci(this);
	if (ci.GotoFirst()) {
		do
		{
		CryObject *o;
			if (ci.IsObject())
			{
				o = (CryObject *)ci.Get();
					if (o-Sortable()) {
						o->Sort(0);
					}
				
			}
		}   while(ci.GotoNext());
	}

} */
String fn = Filename;
    if (Filename =="")
        throw Exception("filename must be assigned before saving source (body must include header, so we need to know header's name)");
    fn += ".h";
    CryFileStream header;
	//fn = "stdout";
    header.Open(fn,"w",true);

    fn = Filename;
    fn += ".cpp";
    CryFileStream body;
    body.Open(fn,"w",true);

    Clear("Source");
    Build();
    header.Write(GetHead("Source"));
    body.Write(GetImp("Source"));

    header.Close(true);
    body.Close(true);
    fn = Filename;
    fn+=".xml";
    body.Open(fn,"w",true);
    CryXML x("obj");
    x.LoadFrom(*this);
	x.SaveTo(body);
    body.Close();

}
ClassBuilder *gptr;
void ClassBuilder::LoadSource()
{
    String fn = Filename;
    if (Filename =="")
        throw Exception("filename must be assigned before loading");
    fn = Filename;
    fn+=".xml";
    CryFileStream body;
    body.Open(fn,"r",true);
    CryXML x("obj");
    x.LoadFrom(body);
    gptr = this;
    x.SaveTo(*this);
    body.Close();

}

/*===========================================================================
// ContainerFactory
============================================================================*/
ContainerFactory::ContainerFactory(CodeFactory *Parent,Container *c) : CodeFactory(Parent,"ContainerFactory")
{
    c->GetEleType(ElementType);
    this->AddProduct("Container");
}


/*===========================================================================
// PropertyFactory
============================================================================*/
PropertyFactory::PropertyFactory(CodeFactory *Parent,const String *n,const String *v) : CodeFactory(Parent,CPropertyFactory)
{
	SetSortValue(0X00090000);
    SetName(*n);
    Value = *v;
    AddProduct("Property");
}
bool PropertyFactory::IsA(const char *ClassName) const    // can the object map to a ClassName
{
	return (strcmp(ClassName,CPropertyFactory)==0) || CodeFactory::IsA(ClassName);
}
const char *PropertyFactory::GetPropertyName()
{
    return GetName();
}
const String *PropertyFactory::GetPropertyValue()
{
    return &Value;
}

/*===========================================================================
// HeaderFactory
============================================================================*/

HeaderFactory::HeaderFactory(CodeFactory *Parent,FunctionDefList *AbstractFunctions) : CodeFactory(Parent,"HeaderFactory")
{
	SetSortValue(0);
    AddProduct("Header");
    AddFactory(new IncludesFactory(this));
    AddFactory(new ClassHeaderFactory(this));
    AddFactory(new FooterFactory(this));
}


Object *HeaderFactory::Create(const PropertyParser &PropertyName,CodeFactory *Parent)
{
	if (!Parent->IsA(CClassBuilder))
    {

		if (PropertyName==CHeaderFactory)
            return this;
        CryFactory *f = FindFactory(PropertyName);
        if (f)
            return f;
		if (PropertyName==CIncludesFactory)
        {
            CodeFactory *f;
            f = new IncludesFactory(this);
            AddFactory(f);
            return f;
        }
		if (PropertyName==CFooter)
        {
            CodeFactory *f;
            f = new FooterFactory(this);
            AddFactory(f);
            return f;
        }

    }
    if (PropertyName=="Header")
    {
        String s;
        //        s.printf("/*! ToDo Make this loadable from an external file (for other peoples copyright notices */\n");
        s.printf("//****************************************************\n");
        s.printf("//Copyright 2004\n");
        s.printf("// Crystal Software (Canada) Inc.\n");
        s.printf("//****************************************************\n");
        SetHeadImp(PropertyName,s);
        Parent->AppendHead(*GetHead(PropertyName));
        Parent->AppendImp(*GetImp(PropertyName));
        Parent->AppendHeadImp("\n//Includes\n");

        Create("Includes",Parent);
        Parent->AppendHeadImp("\n//Class Header\n");

		Create(CClassHeaderFactory,Parent);

        Parent->AppendHeadImp("\n//Footer\n");

        Create("Footer",Parent);
        return this;
    }
    return CodeFactory::Create(PropertyName,Parent);
}



/*===========================================================================
// FooterFactory
============================================================================*/
FooterFactory::FooterFactory(CodeFactory *Parent) : CodeFactory(Parent,"FooterFactory")
{
	SetSortValue(MAXINT-100);
	AddProduct(CFooter);
}

Object *FooterFactory::Create(const PropertyParser &PropertyName,CodeFactory *Parent)
{
	if (PropertyName==CFooter)
	{
        if (Parent->IsA(CClassBuilder))
        {
            String s;
            const char *Name = Parent->GetName();
            s.printf("\n#endif // %s\n// End of file\n",Name);
            SetHead(PropertyName,s.AsPChar());
            s.Clear();
            s.printf("\n// %s\n",Name);
            SetImp(PropertyName,s.AsPChar());
            Parent->AppendHead(*GetHead(PropertyName));
            Parent->AppendImp(*GetImp(PropertyName));
        }
        return this;
    }
    return CodeFactory::Create(PropertyName,Parent);
}
/*===========================================================================
// IncludesFactory
============================================================================*/
IncludesFactory::IncludesFactory(CodeFactory *Parent) : CodeFactory(Parent,"IncludesFactory")
{
	SetSortValue(0X0001000);
    AddProduct("Includes");
}

void IncludesFactory::AddInclude(const Object *p,const char *Type,String &CurrentIncludes,const char *File)
{
	if (p->IsA(Type) && CurrentIncludes.Pos(File)==-1)
	{
		CurrentIncludes.printf("#include\t\"%s\"\n",File);
		return;
	}
}

Object *IncludesFactory::Create(const Object *p,const PropertyParser &PropertyName,String &CurrentIncludes)
{
    if (PropertyName=="CryIncludes")
    {
	AddInclude(p,CObject,CurrentIncludes,"Object.h");
	AddInclude(p,CObject,CurrentIncludes,"ClassException.h");	// always need this one
	AddInclude(p,CContainer,CurrentIncludes,"ClassContainer.h");
	AddInclude(p,CProperty,CurrentIncludes,"ClassProperty.h");
	AddInclude(p,CCryPropertyList,CurrentIncludes,"ClassProperty.h");
	AddInclude(p,CList,CurrentIncludes,"ClassList.h");
	AddInclude(p,CStream,CurrentIncludes,"ClassStream.h");
	AddInclude(p,CCryFileStream,CurrentIncludes,"ClassFileStream.h");
	AddInclude(p,CMemStream,CurrentIncludes,"ClassMemStream.h");
	AddInclude(p,CString,CurrentIncludes,"ClassString.h");
	AddInclude(p,CHugeDouble,CurrentIncludes,"hugedouble.h");
	AddInclude(p,CHugeFraction,CurrentIncludes,"hugedouble.h");
	AddInclude(p,CBitArray,CurrentIncludes,"BitArray.h");
	AddInclude(p,CStrategy,CurrentIncludes,"CryPattern.h");
	AddInclude(p,CStrategyHolder,CurrentIncludes,"CryPattern.h");
	AddInclude(p,CStrategyHolderSender,CurrentIncludes,"CryPattern.h");

	AddInclude(p,CObserver,CurrentIncludes,"CryPattern.h");
	AddInclude(p,CObservable,CurrentIncludes,"CryPattern.h");
	AddInclude(p,CDecorator,CurrentIncludes,"CryPattern.h");
//	AddInclude(p,CSingleton,CurrentIncludes,"CryPattern.h");
	AddInclude(p,CCommandHolder,CurrentIncludes,"CryPattern.h");
	AddInclude(p,CCompositeIterator,CurrentIncludes,"CryPattern.h");
	AddInclude(p,CState,CurrentIncludes,"CryPattern.h");
	AddInclude(p,CCryFactory,CurrentIncludes,"CryPattern.h");
	AddInclude(p,CCryOFactory,CurrentIncludes,"CryPattern.h");

	AddInclude(p,CCryFuzzy,CurrentIncludes,"CryFuzzy.h");
	AddInclude(p,CFunctionDef,CurrentIncludes,"CryFunctionDef.h");
	AddInclude(p,CFunctionDefList,CurrentIncludes,"CryFunctionDef.h");

	AddInclude(p,CArray,CurrentIncludes,"CryArray.h");
	AddInclude(p,CSimpleArray,CurrentIncludes,"CryArray.h");
	AddInclude(p,CDoubleArray,CurrentIncludes,"CryArray.h");

	AddInclude(p,CCryBPNetContainer,CurrentIncludes,"CryBackProp.h");
	AddInclude(p,CBackPropagateLayer,CurrentIncludes,"CryBackProp.h");
	AddInclude(p,CCryBPNet,CurrentIncludes,"CryBackProp.h");

	AddInclude(p,CHugeInt,CurrentIncludes,"hugeint.h");

	AddInclude(p,CXML,CurrentIncludes,"CryXML.h");
	AddInclude(p,CXMLNode,CurrentIncludes,"CryXML.h");

		return 0;	// always return 0 so other classes can add theirs
	}
	return 0;
}


Object *IncludesFactory::Create(const PropertyParser &PropertyName,CodeFactory *Parent)
{
	if ((PropertyName==CIncludesFactory) && (!Parent->IsA(CClassBuilder)))
	{
		return this;
	}

	if (PropertyName=="Includes" && Parent->IsA(CClassBuilder))
	{
		ClassBuilder *cbParent = (ClassBuilder *)Parent;
		const Object *p = cbParent->Getp();
		String s;
		Clear(PropertyName);
		s.printf("#ifndef C%s\n#define C%s\t\"%s\"\n",Parent->GetName(),Parent->GetName(),Parent->GetName());
		s.printf("//standard files\n#include\t<string.h>\n#include\t<stdlib.h>\n");
		s.printf("//Crystal files\n");

	Create(p,"CryIncludes",s);

		Parent->AppendHeadImp("\n//Class Instance Includes");

		Parent->Create("CryIncludes",Parent);
		CompositeIterator a(Parent);
		if (a.GotoFirst())
			do
			{
				CryFactory *f = (CryFactory *)a.Get();
				if (f->IsA(CClassInstance))
				{
					ClassInstance *c = (ClassInstance *)f;
					Create(c->Getp(),"CryIncludes",s);
				}
			}
			while(a.GotoNext());
		SetHead(PropertyName,s);
		s.Clear();
		String FileName;
		FileName =cbParent->GetFilename();
		if (FileName=="")
			FileName.printf("?%s?",Parent->GetName());
		else
		{

			FileName.Rev();
			FileName.Delete(FileName.Pos(FILEDEL),FileName.Length());
			FileName.Rev();
		}
		s.printf("#include \"%s.h\"\n#include \"ClassProperty.h\"\n#include \"CryXML.h\"\t//Needed for SetDefaultValues\n\nusing namespace Crystal;\n",FileName.AsPChar());
		SetImp(PropertyName,s);
		Parent->AppendHead(*GetHead(PropertyName));
		Parent->AppendImp(*GetImp(PropertyName));
		return this;
	}
	return CodeFactory::Create(PropertyName,Parent);
}

/*===========================================================================
// InheritedFactory
============================================================================*/
InheritedFactory::InheritedFactory(CodeFactory *Parent,FunctionDef &_Func) : CodeFactory(Parent,CInheritedFactory)
{
    Func = new FunctionDef(_Func);
    SetSortValue(0X00120000);
    AddProduct(CInheritedFactory);
}
	/// will return a property represented as an object, useful for classes which contain properties that are dynamically allocated, as a property that is dynamic is a Object and therefore callable
Object *InheritedFactory::GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const
{  // we can't just dup the result of _GetPropertyAsObject as subclasses may not implement a property in _GetPropertyAsObject
	if (PropertyName=="Function")
	{
		return Func->Dup();
	}
	return CodeFactory::GetCopyOfPropertyAsObject(PropertyName);
}
	/// will return a pointer to the property if the property is an Object (or decendent)
Object *InheritedFactory::_GetPropertyAsObject(const PropertyParser &PropertyName) const
{
	if (PropertyName=="Function")
	{
		return Func;
	}
	return CodeFactory::_GetPropertyAsObject(PropertyName);
}
/*! will return whether or not the property named in PropertyName is a container */
bool InheritedFactory::GetIsPropertyContainer(const PropertyParser &PropertyName) const
{
    if (PropertyName=="Function")
    {
        return false;
    }
    return CodeFactory::GetIsPropertyContainer(PropertyName);
}
/*! will return the property named in PropertyName in a string format */
const char *InheritedFactory::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
    if (PropertyName=="Function")
    {
        Func->GetDeclaration(Result);
        return Result.AsPChar();
    }
    return CodeFactory::GetProperty(PropertyName,Result);
}
/*! returns true if the class in question has the property */
bool InheritedFactory::HasProperty(const PropertyParser &PropertyName) const
{
    if (PropertyName=="Function")
    {
        return true;
    }
    return CodeFactory::HasProperty(PropertyName);
}
/*! The count of the properties a class has */
int InheritedFactory::GetPropertyCount() const
{
    return CodeFactory::GetPropertyCount()+1;
}
/*! Make a list of all property names, the function is called from the parent class through each inheritance until it reaches this class, at which point a list is created and filled with any properties on the way back through the inheritance */
CryPropertyList *InheritedFactory::PropertyNames() const
{
	CryPropertyList *l = CodeFactory::PropertyNames();
	l->AddPropertyByName("Function",this);
	return l;
}
bool InheritedFactory::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Function")
    {
		Func->Parse(PropertyValue);
		this->SetSortValue(0x0012000 + Func->FunctionName.HashValue());
        return true;
    }
    return CodeFactory::HasProperty(PropertyName);
}

InheritedFactory::~InheritedFactory()
{
    delete Func;
}
const FunctionDef *InheritedFactory::GetFunc()
{
    return Func;
}
bool InheritedFactory::IsA(const char *ClassName) const    // can the object map to a ClassName
{
	return (ClassName==CInheritedFactory) || CodeFactory::IsA(ClassName);
}


Object *InheritedFactory::Create(const PropertyParser &PropertyName,CodeFactory *Parent)
{
	if ((PropertyName==CInheritedFactory) && (!Parent->IsA(CClassBuilder)))
	{
		return this;
	}

	if (CanBuildProduct(CCopyToEnd) && (PropertyName==CCopyToEnd))
	{
		if (!Parent->IsA(CClassBuilder))
			throw Exception("Bad Parent at end of copyto function");
		ClassBuilder *p = (ClassBuilder *)Parent;
		String Implementation;
		Implementation.Clear();
		Implementation.printf("\n\t}\n//Now copy the base class\n\t%s::CopyTo(Dest);\n\n}//%s\n\n",p->GetInheritType(), Parent->GetName());
		SetImp(PropertyName,Implementation);
		Parent->AppendImp(*GetImp(PropertyName));
		return this;
	}

	if (PropertyName==CInheritedFactory && Parent->IsA(CClassBuilder))
	{
		ClassBuilder *cbParent = (ClassBuilder *)Parent;
		String s,h,t,Tester,Header,Implementation;
		FunctionDef fd;
		Func->GetNPDeclaration(s);
		Header = s;
		fd.Parse("virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;");
		fd.GetNPDeclaration(Tester);
		if (Header==Tester)
		{
			Header.printf("\n");
			Func->GetImplementedDeclaration(Parent->GetName(),s,true);
			Implementation = s;
			Implementation.printf("\n{");
			{
				CompositeIterator *ci = new CompositeIterator(Parent);
				if (ci->GotoFirst())
				{
					do
					{
						Object *o = (Object *)ci->Get();
						{
							CodeFactory *cf;
							const char *N;
							if (o->IsA(CCodeFactory))
							{
								cf = (CodeFactory *)o;
								if (cf->GetIsProperty())
								{
				String PropertyStart;
									const char *N = cf->GetName();
				if (cf->IsA(CProperty))
					PropertyStart.printf("PropertyName==%s.GetName()",N);
				else
				if (cf->IsA(CCryPropertyList))
					PropertyStart.printf("%s.HasProperty(PropertyName)",N);
				else
					PropertyStart.printf("PropertyName==\"%s\"",N);

									if (cf->GetCount()>1)
										Implementation.printf(
											"\n\tif (%s)\n\t{\n\t\tif (PropertyName.GetIndex()>-1)\n\t\t Get%s(PropertyName.GetIndex(),Result);\n\telse\n\t\tthrow CryException(\"%s needs an index\");\n\t}",PropertyStart.AsPChar(),N,N);
									else
										Implementation.printf("\n\tif (%s)\n\t{\n#ifdef RangeChecking\n\t\tif (PropertyName.GetIndex()!=-1)\n\t\t\tthrow CryException(\"%s should not have an index\");\n#endif\n\t\tGet%s(Result);\n\t\treturn Result.AsPChar();\n\t}",PropertyStart.AsPChar(),N,N);

								}
							}
							else
								if (o->IsA(CPrimInstance))
								{
									PrimInstance *pi = (PrimInstance *)o;
									const char *N;
									if (pi->GetIsProperty())
									{
										N = pi->GetName();
										if (pi->GetCount()>1)
											Implementation.printf("\n\tif (PropertyName==T%s)==0)\n\t{\n\t\tResult = \"[]\"; \n\t\treturn \"*\";// indicate that it's an indexed property\n\t}",N);
										else
											Implementation.printf("\n\tif (PropertyName==T%s)\n\t\t{\n\t\t\tGet%s(&Result);\n\t\t\treturn Result.AsPChar();\n\t\t}",N,N);
									}
								}
								else
								{
									if (o->IsA(CClassInstance))
									{
										ClassInstance *AClassInstance = (ClassInstance *)o;
										const char *N;
										if (AClassInstance->GetIsProperty())
										{
N = AClassInstance->GetName();
String PropertyStart;
if (AClassInstance->IsA(CProperty))
	PropertyStart.printf("PropertyName==%s.GetName()",N);
else
	if (AClassInstance->IsA(CCryPropertyList))
		PropertyStart.printf("%s.HasProperty(PropertyName)",N);
	else
		PropertyStart.printf("PropertyName==T%s",N);

											if (AClassInstance->GetCount()>1)
											{
												int l = strlen(N)+1;
												Implementation.printf("\n\tif (%s)\n\t{\n\t\tResult = \"[]\"; \n\t\treturn \"*\";// indicate that it's an indexed property\n\t}",PropertyStart.AsPChar());
												Implementation.printf("\nint i;\n\tif (::strncmp(PropertyName,T%s,%d)==0 && sscanf(PropertyName+%d,\"%d\",&i);)\n\t{\n\t\tResult = \"[]\"; \n\t\treturn \"*\";// indicate that it's an indexed property\n\t}",N,l,l+1);

											}
											else
												Implementation.printf("\n\tif (%s)\n\t\tGet%s(&Result);\n\t\t\treturn Result.AsPChar();\n\t\t}",PropertyStart.AsPChar(),N);
										}
									}
								}
                        }

                    }
                    while(ci->GotoNext());
                }
                delete(ci);
                Implementation.printf("\n\treturn %s::GetProperty(PropertyName,Result);\n}\n\n",
                                      cbParent->GetInheritType());
                Clear(PropertyName);
                SetHead(PropertyName,Header);
                SetImp(PropertyName,Implementation);
                Parent->AppendHead(*GetHead(PropertyName));
                Parent->AppendImp(*GetImp(PropertyName));
                return 0;
            }

        }
        fd.Parse("virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);" );
        fd.GetNPDeclaration(Tester);
        if (Header == Tester)
        {
            Header.printf("\n");
            Func->GetImplementedDeclaration(Parent->GetName(),s,true);
			Implementation = s;
			Implementation.printf("\n{\n");
			{
				CompositeIterator *ci = new CompositeIterator(Parent);
				if (ci->GotoFirst())
				{
					do
					{
						Object *o = (Object *)ci->Get();
						if (o->IsA(CClassHeaderFactory))
						{
							ClassHeaderFactory *chf = (ClassHeaderFactory *)o;
							CompositeIterator *ci = new CompositeIterator(chf);
							if (ci->GotoFirst()) {
								do
								{
								Object *o = (Object *)ci->Get();



							CodeFactory *cf;
							if (o->IsA(CCodeFactory))
							{
								cf = (CodeFactory *)o;
								if (cf->GetIsProperty())
								{
									const char *N = cf->GetName();
									if (cf->GetCount()>1)
										Implementation.printf(
											"\tif (PropertyName==\"%s\")\n\t{\n\t\tif (PropertyName.GetIndex()>-1)\n\t\t Set%s(PropertyName.GetIndex(),PropertyValue);\n\telse\n\t\tthrow CryException(\"%s needs an index\");\n\t}",N,N,N);
									else
										Implementation.printf( "\tif (PropertyName==\"%s\")\n\t{\n#ifdef RangeChecking\n\t\tif (PropertyName.GetIndex()!=-1)\n\t\t\tthrow CryException(\"%s should not have an index\");\n#endif\n\t\tSet%s(PropertyValue);\n\t\treturn true;\n\t}",N,N,N);
								}
							}
							else
								if (o->IsA(CPrimInstance))
								{
									PrimInstance *pi = (PrimInstance *)o;
									const char *N;
									if (pi->GetIsProperty())
									{
										N = pi->GetName();
										if (pi->GetCount()>1)
											Implementation.printf("\n\tif (PropertyName==\"%s\")\n\t{\n\t\treturn false;// can't set an indexed property this way\n\t}",N);
										else
											Implementation.printf("\n\tif (PropertyName==\"%s\")\n\t\t{\n\t\t\tSet%s(&Result);\n\t\t\treturn true \n\t\t}",N,N);
									}
								}
								}   while(ci->GotoNext());

							}
							delete(ci);
						}

					}
					while(ci->GotoNext());
				}



/*					do
					{
						Object *o = (Object *)ci->Get();
						{
							CodeFactory *cf;
							if (o->IsA(CCodeFactory))
							{
								cf = (CodeFactory *)o;
								if (cf->GetIsProperty())
								{
									const char *N = cf->GetName();
									if (cf->GetCount()>1)
										Implementation.printf(
											"\tif (PropertyName==\"%s\")\n\t{\n\t\tif (PropertyName.GetIndex()>-1)\n\t\t Set%s(PropertyName.GetIndex(),PropertyValue);\n\telse\n\t\tthrow CryException(\"%s needs an index\");\n\t}",N,N,N);
									else
										Implementation.printf( "\tif (PropertyName==\"%s\")\n\t{\n#ifdef RangeChecking\n\t\tif (PropertyName.GetIndex()!=-1)\n\t\t\tthrow CryException(\"%s should not have an index\");\n#endif\n\t\tSet%s(PropertyValue);\n\t\treturn true;\n\t}",N,N,N);
								}
							}
							else
								if (o->IsA(CPrimInstance))
								{
									PrimInstance *pi = (PrimInstance *)o;
									const char *N;
									if (pi->GetIsProperty())
									{
										N = pi->GetName();
										if (pi->GetCount()>1)
											Implementation.printf("\n\tif (PropertyName==\"%s\")\n\t{\n\t\treturn false;// can't set an indexed property this way\n\t}",N);
										else
											Implementation.printf("\n\tif (PropertyName==\"%s\")\n\t\t{\n\t\t\tSet%s(&Result);\n\t\t\treturn true \n\t\t}",N,N);
									}
								}

						}

					}
					while(ci->GotoNext());
  				}
  */
				delete(ci);
				Implementation.printf(
					"\n\treturn %s::SetProperty(PropertyName,PropertyValue);\n}\n\n",
					cbParent->GetInheritType());
				Clear(PropertyName);
				SetHead(PropertyName,Header);
				SetImp(PropertyName,Implementation);
                Parent->AppendHead(*GetHead(PropertyName));
                Parent->AppendImp(*GetImp(PropertyName));
                return 0;
            }

        }
	fd.Parse("virtual CryPropertyList *PropertyNames() const");
	fd.GetNPDeclaration(Tester);
	if (Header==Tester)
	{
		if (!Parent->IsA(CClassBuilder))
			throw Exception("Bad Parent at PropertyNames function");
		ClassBuilder *p = (ClassBuilder *)Parent;
			Clear(PropertyName);
			Header.printf("\n");
		Implementation = "\n\n/*!Get the names of all properties of this class*/\n";
			Implementation += Func->GetImplementedDeclaration(Parent->GetName(),true);
			Implementation.printf("\n{\n// get base class's properties");
			Implementation.printf("\nCryPropertyList *Names = %s::PropertyNames();",p->GetInheritType());
		Implementation.printf("\n// now add our own properties (if any)");
		{
		bool Found = false;
		CryPropertyList *Names = p->Getp()->PropertyNames();
		CryPropertyList::PropertyIterator *i = Names->CreateIterator();
		if(i->GotoFirst())
		{
			do
			{
			if (!Found) {
				Implementation.printf("\n\n// inherited properites which can be exposed by uncommenting");
				Found = true;
			}
			String r;
				Implementation.printf("\n//\tNames->AddProperty(\"%s\",\"%s\");",i->GetName(),i->GetValue(r));
			} while(i->GotoNext());
		}
		Names->DeleteIterator(i);
		delete Names;
		}
		Implementation.printf("\n\n// properties that are local to this class");

				CompositeIterator *ci = new CompositeIterator(Parent);
				if (ci->GotoFirst())
				{
					do
					{
						Object *o = (Object *)ci->Get();
						if (o->IsA(CClassHeaderFactory))
						{
							ClassHeaderFactory *chf = (ClassHeaderFactory *)o;
							CompositeIterator *ci = new CompositeIterator(chf);
							if (ci->GotoFirst()) {
								do
								{
								Object *o = (Object *)ci->Get();
									if (o->IsA(CPrimInstance)) {
									PrimInstance *pi = (PrimInstance *) o;
										if (pi->GetIsProperty()) {

									const char *N = pi->GetName();
					Implementation.printf("\n\t\\\\If there is interference with parent classes then get the value manually and add.\n\tNames->AddPropertyByName(\"%s\",this);",N);
										}
									}
								}   while(ci->GotoNext());

							}
							delete(ci);
						}

					}
					while(ci->GotoNext());
				}
				delete(ci);
			Implementation.printf("\n\treturn Names;\n}\n\n");
				Clear(PropertyName);
				SetHead(PropertyName,Header);
				SetImp(PropertyName,Implementation);
				Parent->AppendHead(*GetHead(PropertyName));
				Parent->AppendImp(*GetImp(PropertyName));
				return 0;

	}
		fd.Parse("virtual void CopyTo(Object &Dest) const");
		fd.GetNPDeclaration(Tester);
		if (Header == Tester)
		{
			if (!CanBuildProduct(CCopyToEnd))
			{
				AddProduct(CCopyToEnd);
			}

			Clear(PropertyName);
			Header.printf("\n");
		Implementation = "\n/*!Copy this class and any parts of it to Dest, if possible*/";
			Implementation = Func->GetImplementedDeclaration(Parent->GetName(),true);
			Implementation.printf("\n{\n");
			Implementation.printf("\n\tif (Dest.IsA(C%s))\n\t{",Parent->GetName());
		Implementation.printf("\n\t\t// Copy this classes variables first");
			Implementation.printf("\n\t\t%s *CastDest = (%s *)&Dest;",Parent->GetName(),Parent->GetName());

			//            Implementation.printf("\t\t*CastDest = *this;");
			/*            Iterator *ci = Parent->_CreateIterator();
			if (ci->GotoFirst())
			{
			do
			{
			Object *o = (Object *)ci->Get();
			{
			CodeFactory *cf;
			CryString N;
			if (o->IsA(CCodeFactory))
			{
			cf = (CodeFactory *)o;
			N = cf->GetName();
			N = (CryString *)cf->Create("VarName",Parent);
			if (N!="")
			Implementation.printf("\t\tCastDest.%s = %s;\n",N.AsPChar(),N.AsPChar());
			}
			}
			}
			while(ci->GotoNext());
			}*/

			// ToDo: Ask Factories to give implementation of variables here
			//            Implementation.printf("\t}\n\n}\\\\%s\n\n",Func->GetFunctionName());
			Clear(PropertyName);
			SetHead(PropertyName,Header);
			SetImp(PropertyName,Implementation);
			Parent->AppendHead(*GetHead(PropertyName));
			Parent->AppendImp(*GetImp(PropertyName));
			GetParent()->Create(CCopyTo,Parent);
			GetParent()->Create(CCopyToEnd,Parent);
			return 0;
		}
		SetHead(PropertyName,Func->GetNPDeclaration());
		Implementation.printf("%s\n{\n}\n",Func->GetImplementedDeclaration(Parent->GetName(),true).AsPChar());
		SetImp(PropertyName,Implementation);
		Parent->AppendHead(*GetHead(PropertyName));
		Parent->AppendImp(*GetImp(PropertyName));
		return 0;
	}// don't return result because there might be more then one Inherited type function
	return CodeFactory::Create(PropertyName,Parent);
}

