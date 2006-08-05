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
class CryObjectNAbstract : public CryObject
{
    virtual void CopyTo(CryObject &Dest) const
        {}
    ;
    virtual CryObject *Dup() const
    {
        return 0;
    }
    virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
    {
        return  "";
    }

};
/*/// Dummy class with stubs for virtual functions
class CryNamedObjectNAbstract : public CryNamedObject
{
void CopyTo(CryObject &Object) const
{}
;
CryObject *Dup() const
{
return 0;
}
};*/
/// Dummy class with stubs for virtual functions
class CryContainerNAbstract : public CryContainer
{
	virtual EmptyObject *Add(EmptyObject *Item,size_t Size)
	{
		return 0;
	}
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0) 
	{
	}

    virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size)
    {
        return 0;
    }
    virtual CryObject *Dup()const
    {
        return 0;
    }
    virtual CryObject *Add(CryObject *Item)
    {
        return  0;
    }
    virtual CryObject *AddOwned(CryObject *Item)
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
    virtual bool IsCryObject(const Iterator *I) const
    {
        return false;
    }
    virtual bool LoadAsText(Iterator *I,CryString &FromStream)
    {
        return false;
    }
    virtual bool SaveAsText(Iterator *I,CryString &ToStream) const
    {
        return false;
    }
    virtual void GetEleType(CryString &Result) const
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
    virtual void CopyTo(CryObject &Dest) const
        {}
    ;
    virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
    {
        return  "";
    }

};

/// Dummy class with stubs for virtual functions
class CryStreamNAbstract : public CryStream
{
    virtual void Clear()
    {}
    ;
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0) 
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
    virtual CryObject *Add(CryObject *Item)
    {
        return Item;
    }    // returns Item
    virtual CryObject *AddOwned(CryObject *Item)
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
    virtual bool IsCryObject(const Iterator *I) const
    {
        return false;
    }
    virtual bool LoadAsText(Iterator *I,CryString &FromStream)
	{
        return false;
    }
    ;
    virtual bool SaveAsText(Iterator *I,CryString &ToStream) const
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
    virtual size_t Read(CryStream *ToStream,size_t Size) const
    {
        return 0;
	}
    virtual size_t Read(CryStream *ToStream) const
    {
        return 0;
    }
    virtual size_t Write(const CryStream *FromStream)
    {
        return 0;
    }
    virtual size_t Write(const CryStream *FromStream,size_t Size)
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
    CryObject *Dup() const
    {
        return 0;
    }
};

/// Dummy class with stubs for virtual functions
class CrySimpleArrayNAbstract : public CrySimpleArray
{
    virtual void CopyTo(CryObject &Dest) const
        {}
    ;
    virtual CryObject *Dup() const
    {
        return 0;
    }
	virtual void RemoveAtIterator(Iterator *LI)
	{
        return;
    }
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0) 
	{
	}
	virtual void SetItem(unsigned int i,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size)
	{

	}

	virtual bool IsCryObject(const Iterator *I) const
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
    virtual CryObject *Add(CryObject *Item)
    {
        return  0;
    }
    virtual CryObject *AddOwned(CryObject *Item)
    {
        return 0;
    }

    virtual bool LoadAsText(int i,CryString &FromStream)
    {
        return false;
    };
    virtual bool SaveAsText(int i,CryString &ToStream) const
    {
        return false;
    };
    virtual void GetEleType(CryString &Result) const
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
    virtual bool LoadAsText(Iterator *I,CryString &FromStream)
    {
        return true;
    }
    virtual bool SaveAsText(Iterator *I,CryString &ToStream) const
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
class CryArrayNAbstract : public CryArray
{
    virtual EmptyObject *Add(EmptyObject *Item,size_t Size)
    {
        return 0;
    }
    virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size)
    {
        return 0;
    }
    virtual CryObject *Add(CryObject *Item)
    {
        return  0;
    }
    virtual CryObject *AddOwned(CryObject *Item)
    {
        return 0;
    }
    virtual CryObject *Dup()const
    {
        return 0;
    }
    virtual void DestroyArrayItem(CryArray *Owner,EmptyObject *Item)
    {}
    ;
    virtual EmptyObject *CreateArrayItem(CryArray *Owner,bool *IsCryObject)
    {
        return 0;
    };
    virtual void SaveItemTo(const CryArray *Owner,EmptyObject *FromItem,CryStream &ToStream) const
        {}
    ;
    virtual EmptyObject *LoadItemFrom(CryArray *Owner,EmptyObject *ToItem,CryStream &FromStream)
    {
        return 0;
    };
    virtual bool LoadAsText(int i,CryString &FromStream)
    {
        return false;
    };
    virtual bool SaveAsText(int i,CryString &ToStream) const
    {
        return false;
    };
    virtual void GetEleType(CryString &Result) const
        {}
    ;
    virtual bool LoadAsText(Iterator *I,CryString &FromStream)
    {
        return true;
    }
    virtual bool SaveAsText(Iterator *I,CryString &ToStream) const
    {
        return true;
    }
};

void ClassBuilder::SetDefaultBodies(CryFunctionDefList *AbstractFunctions) // will setup default bodies of functions
{
    HeaderFactory *hf = (HeaderFactory *) Create(THeaderFactory,this);
	ClassHeaderFactory *cf = (ClassHeaderFactory *)Create(TClassHeaderFactory,hf);
	if (hf && AbstractFunctions)
	{
		CryList::Iterator *i = AbstractFunctions->_CreateIterator();
		if (i->GotoFirst())
			do
			{
				//                AbstractFunctions->SetItemOwnerShip(i,false);     /// I'll take that!
				CryFunctionDef *s = (CryFunctionDef *) i->Get();
				try
				{
					CryString Def;
					s->GetDeclaration(Def);
					if (s->IsPure && (!s->IsComment) && !cf->Present(s))
					{
						CryString Declare = s->GetNPDeclaration();
						InheritedFactory *If = (InheritedFactory *)Create(TInheritedFactory,hf);
						If->SetProperty("Function",Declare);
					}
				}
				catch(CryException &e)
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
const CryObject *ClassBuilder::Getp() const
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
    CryObject *p1;
    SetName(_ClassName);
    CryFunctionDefList *AbstractFunctions = 0;
    try
    {
        p1 = CryObject::Create(Type);
        _IsAbstract = false;
    }
    catch(CryException &e)
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
            throw CryException("Could not create abstract function");
        }
    };
    if (p)
        delete p;
    p = p1;
    if (p->IsContainer())
        this->AddFactory(new ContainerFactory( this,(CryContainer *)p));
    if (AddStubs)
        AbstractFunctions = p->GetFunctions(Type);
    else
        if (_IsAbstract) // if the base class wasn't able to be created because it was abstract, then we need to add the functions that are missing
            AbstractFunctions = p->GetAbstractFunctions(Type);
    /* We add these functions just to make things easier */
    if (!AbstractFunctions)
        AbstractFunctions = new CryFunctionDefList();
	AbstractFunctions->AddOwned(new CryFunctionDef(
							   "virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const = 0;"));
	AbstractFunctions->AddOwned(new CryFunctionDef(
							   "virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue) = 0;"));
	AbstractFunctions->AddOwned(new CryFunctionDef(
							   "virtual void CopyTo(CryObject &Dest) const = 0;"));
    AbstractFunctions->AddOwned(new CryFunctionDef(
                               "virtual CryList *PropertyNames() const = 0;"));
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
            if (o->IsA(TClassHeaderFactory))
            {
				ClassHeaderFactory *chf = (ClassHeaderFactory *)o;
                CryString n;
                n = ClassName;
                if (chf->Present(&n))
					throw CryException("Component name \"%s\" is already being used");
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
				catch(CryException &s)
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
			if (o->IsA(TClassHeaderFactory))
            {
                ClassHeaderFactory *chf = (ClassHeaderFactory *)o;
                CryString n;
                n = GetName();
                if (chf->Present(&n))
                    //		if (chf->Present(ClassName))
                    throw CryException("Component name \"%s\" is already being used");
                chf->AddFactory(new PrimInstance(chf,PrimType,PrimName,DefaultValue,Count,IsProperty,IsPointer,IsArrayPointer));
//                break;
            }
        }
        while(a.GotoNext());
}

CryObject *ClassBuilder::Create(CryStream &FromStream)
{
    throw CryException("Not Implemented");
}


CryObject *ClassBuilder::Create(const CryPropertyParser &PropertyName,CryObject *Parent)
{
    return CodeFactory::Create(PropertyName,(CryObject *)Parent);
}

CryObject *ClassBuilder::Create(const CryPropertyParser &PropertyName,CodeFactory *Parent)
{
    if (PropertyName==TContainerFactory && p)
    {
        ContainerFactory *cf = new ContainerFactory( this,(CryContainer *)p);
        AddFactory(cf);
        return cf;
    }
    if (PropertyName==TPropertyFactory)
    {
        PropertyFactory *pf = (PropertyFactory *) FindFactory(PropertyName);
        if (!pf)
        {
            CryString Name,Value;
            pf = new PropertyFactory(this,&Name,&Value);
            AddFactory(pf);
        }
        return pf;
    }

    if (PropertyName==THeaderFactory)
    {
        HeaderFactory *hf = (HeaderFactory *) FindFactory(THeaderFactory);
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
CryObject *ClassBuilder::GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const
{
	return CodeFactory::GetCopyOfPropertyAsObject(PropertyName);
}
CryObject *ClassBuilder::_GetPropertyAsObject(const CryPropertyParser &PropertyName) const
{
	return CodeFactory::_GetPropertyAsObject(PropertyName);
}

bool ClassBuilder::GetIsPropertyContainer(const CryPropertyParser &PropertyName) const
{
    if (PropertyName=="ClassType")
        return false;
    else
        return CodeFactory::GetIsPropertyContainer(PropertyName);
}
const char * ClassBuilder::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
    if (PropertyName=="ClassType")
    {
        Result = p->ChildClassName();
        return Result.AsPChar();
    }
    return CodeFactory::GetProperty(PropertyName,Result);
}

bool  ClassBuilder::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
    if (PropertyName=="ClassType")
    { // we don't add stubs here because this is coming from a saved version, which will have already defined them
        CryString Name;
        CryObject::GetProperty("Name",Name);
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
bool ClassBuilder::HasProperty(const CryPropertyParser &PropertyName) const
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
			const CryString *name = pi->GetName();
			CryString r;
			const char *value = pi->GetValue(r);
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
const CryString &ClassBuilder::GetFilename() const
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
void ClassBuilder::SaveHeaderBody(CryStream &ToStreamHeader,CryStream &ToStreamBody)
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
    CryString fn = Filename;
    if (Filename =="")
        throw CryException("filename must be assigned before saving source (body must include header, so we need to know header's name)");
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
    CryString fn = Filename;
    if (Filename =="")
        throw CryException("filename must be assigned before loading");
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
ContainerFactory::ContainerFactory(CodeFactory *Parent,CryContainer *c) : CodeFactory(Parent,"ContainerFactory")
{
    c->GetEleType(ElementType);
    this->AddProduct("Container");
}


/*===========================================================================
// PropertyFactory
============================================================================*/
PropertyFactory::PropertyFactory(CodeFactory *Parent,const CryString *n,const CryString *v) : CodeFactory(Parent,TPropertyFactory)
{
    SetSortValue(9);
    SetName(*n);
    Value = *v;
    AddProduct("Property");
}
bool PropertyFactory::IsA(const char *ClassName) const    // can the object map to a ClassName
{
    return (strcmp(ClassName,TPropertyFactory)==0) || CodeFactory::IsA(ClassName);
}
const char *PropertyFactory::GetPropertyName()
{
    return GetName();
}
const CryString *PropertyFactory::GetPropertyValue()
{
    return &Value;
}

/*===========================================================================
// HeaderFactory
============================================================================*/

HeaderFactory::HeaderFactory(CodeFactory *Parent,CryFunctionDefList *AbstractFunctions) : CodeFactory(Parent,"HeaderFactory")
{
    SetSortValue(0);
    AddProduct("Header");
    AddFactory(new IncludesFactory(this));
    AddFactory(new ClassHeaderFactory(this));
    AddFactory(new FooterFactory(this));
}


CryObject *HeaderFactory::Create(const CryPropertyParser &PropertyName,CodeFactory *Parent)
{
    if (!Parent->IsA(TClassBuilder))
    {

        if (PropertyName==THeaderFactory)
            return this;
        CryFactory *f = FindFactory(PropertyName);
        if (f)
            return f;
        if (PropertyName==TIncludesFactory)
        {
            CodeFactory *f;
            f = new IncludesFactory(this);
            AddFactory(f);
            return f;
        }
        if (PropertyName==TFooter)
        {
            CodeFactory *f;
            f = new FooterFactory(this);
            AddFactory(f);
            return f;
        }

    }
    if (PropertyName=="Header")
    {
        CryString s;
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

        Create(TClassHeaderFactory,Parent);

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
    SetSortValue(100);
    AddProduct(TFooter);
}

CryObject *FooterFactory::Create(const CryPropertyParser &PropertyName,CodeFactory *Parent)
{
    if (PropertyName==TFooter)
    {
        if (Parent->IsA(TClassBuilder))
        {
            CryString s;
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
    SetSortValue(1);
    AddProduct("Includes");
}

void IncludesFactory::AddInclude(const CryObject *p,const char *Type,CryString &CurrentIncludes,const char *File)
{
	if (p->IsA(Type) && CurrentIncludes.Pos(File)==-1)
	{
		CurrentIncludes.printf("#include\t\"%s\"\n",File);
		return;
	}
}

CryObject *IncludesFactory::Create(const CryObject *p,const CryPropertyParser &PropertyName,CryString &CurrentIncludes)
{
    if (PropertyName=="CryIncludes")
    {
	AddInclude(p,TCryObject,CurrentIncludes,"CryObject.h");
	AddInclude(p,TCryObject,CurrentIncludes,"ClassException.h");	// always need this one
	AddInclude(p,TCryContainer,CurrentIncludes,"ClassContainer.h");
	AddInclude(p,TCryProperty,CurrentIncludes,"ClassProperty.h");
	AddInclude(p,TCryPropertyList,CurrentIncludes,"ClassProperty.h");
	AddInclude(p,TCryList,CurrentIncludes,"ClassList.h");
	AddInclude(p,TCryStream,CurrentIncludes,"ClassStream.h");
	AddInclude(p,TCryFileStream,CurrentIncludes,"ClassFileStream.h");
	AddInclude(p,TCryMemStream,CurrentIncludes,"ClassMemStream.h");
	AddInclude(p,TCryString,CurrentIncludes,"ClassString.h");
	AddInclude(p,THugeDouble,CurrentIncludes,"hugedouble.h");
	AddInclude(p,THugeFraction,CurrentIncludes,"hugedouble.h");
	AddInclude(p,TBitArray,CurrentIncludes,"BitArray.h");
	AddInclude(p,TStrategy,CurrentIncludes,"CryPattern.h");
	AddInclude(p,TStrategyHolder,CurrentIncludes,"CryPattern.h");
	AddInclude(p,TStrategyHolderSender,CurrentIncludes,"CryPattern.h");

	AddInclude(p,TObserver,CurrentIncludes,"CryPattern.h");
	AddInclude(p,TObservable,CurrentIncludes,"CryPattern.h");
	AddInclude(p,TDecorator,CurrentIncludes,"CryPattern.h");
//	AddInclude(p,TSingleton,CurrentIncludes,"CryPattern.h");
	AddInclude(p,TCommandHolder,CurrentIncludes,"CryPattern.h");
	AddInclude(p,TCompositeIterator,CurrentIncludes,"CryPattern.h");
	AddInclude(p,TState,CurrentIncludes,"CryPattern.h");
	AddInclude(p,TCryFactory,CurrentIncludes,"CryPattern.h");
	AddInclude(p,TCryOFactory,CurrentIncludes,"CryPattern.h");

	AddInclude(p,TCryFuzzy,CurrentIncludes,"CryFuzzy.h");
	AddInclude(p,TCryFunctionDef,CurrentIncludes,"CryFunctionDef.h");
	AddInclude(p,TCryFunctionDefList,CurrentIncludes,"CryFunctionDef.h");

	AddInclude(p,TCryArray,CurrentIncludes,"CryArray.h");
	AddInclude(p,TCrySimpleArray,CurrentIncludes,"CryArray.h");
	AddInclude(p,TCryDoubleArray,CurrentIncludes,"CryArray.h");

	AddInclude(p,TCryBPNetContainer,CurrentIncludes,"CryBackProp.h");
	AddInclude(p,TBackPropagateLayer,CurrentIncludes,"CryBackProp.h");
	AddInclude(p,TCryBPNet,CurrentIncludes,"CryBackProp.h");

	AddInclude(p,THugeInt,CurrentIncludes,"hugeint.h");

	AddInclude(p,TCryXML,CurrentIncludes,"CryXML.h");
	AddInclude(p,TCryXMLNode,CurrentIncludes,"CryXML.h");

        return 0;	// always return 0 so other classes can add theirs
    }
    return 0;
}


CryObject *IncludesFactory::Create(const CryPropertyParser &PropertyName,CodeFactory *Parent)
{
    if ((PropertyName==TIncludesFactory) && (!Parent->IsA(TClassBuilder)))
    {
        return this;
    }

    if (PropertyName=="Includes" && Parent->IsA(TClassBuilder))
    {
        ClassBuilder *cbParent = (ClassBuilder *)Parent;
        const CryObject *p = cbParent->Getp();
        CryString s;
        Clear(PropertyName);
        s.printf("#ifndef T%s\n#define T%s\t\"%s\"\n",Parent->GetName(),Parent->GetName(),Parent->GetName());
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
				if (f->IsA(TClassInstance))
				{
					ClassInstance *c = (ClassInstance *)f;
					Create(c->Getp(),"CryIncludes",s);
				}
			}
			while(a.GotoNext());
		SetHead(PropertyName,s);
		s.Clear();
		CryString FileName;
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
InheritedFactory::InheritedFactory(CodeFactory *Parent,CryFunctionDef &_Func) : CodeFactory(Parent,TInheritedFactory)
{
    Func = new CryFunctionDef(_Func);
    SetSortValue(12);
    AddProduct(TInheritedFactory);
}
	/// will return a property represented as an object, useful for classes which contain properties that are dynamically allocated, as a property that is dynamic is a CryObject and therefore callable
CryObject *InheritedFactory::GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const
{  // we can't just dup the result of _GetPropertyAsObject as subclasses may not implement a property in _GetPropertyAsObject
	if (PropertyName=="Function")
	{
		return Func->Dup();
	}
	return CodeFactory::GetCopyOfPropertyAsObject(PropertyName);
}
	/// will return a pointer to the property if the property is an CryObject (or decendent)
CryObject *InheritedFactory::_GetPropertyAsObject(const CryPropertyParser &PropertyName) const
{
	if (PropertyName=="Function")
	{
		return Func;
	}
	return CodeFactory::_GetPropertyAsObject(PropertyName);
}
/*! will return whether or not the property named in PropertyName is a container */
bool InheritedFactory::GetIsPropertyContainer(const CryPropertyParser &PropertyName) const
{
    if (PropertyName=="Function")
    {
        return false;
    }
    return CodeFactory::GetIsPropertyContainer(PropertyName);
}
/*! will return the property named in PropertyName in a string format */
const char *InheritedFactory::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
    if (PropertyName=="Function")
    {
        Func->GetDeclaration(Result);
        return Result.AsPChar();
    }
    return CodeFactory::GetProperty(PropertyName,Result);
}
/*! returns true if the class in question has the property */
bool InheritedFactory::HasProperty(const CryPropertyParser &PropertyName) const
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
bool InheritedFactory::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Function")
    {
        Func->Parse(PropertyValue);
        return true;
    }
    return CodeFactory::HasProperty(PropertyName);
}

InheritedFactory::~InheritedFactory()
{
    delete Func;
}
const CryFunctionDef *InheritedFactory::GetFunc()
{
    return Func;
}
bool InheritedFactory::IsA(const char *ClassName) const    // can the object map to a ClassName
{
    return (ClassName==TInheritedFactory) || CodeFactory::IsA(ClassName);
}


CryObject *InheritedFactory::Create(const CryPropertyParser &PropertyName,CodeFactory *Parent)
{
    if ((PropertyName==TInheritedFactory) && (!Parent->IsA(TClassBuilder)))
    {
        return this;
    }

    if (CanBuildProduct(TCopyToEnd) && (PropertyName==TCopyToEnd))
    {
        if (!Parent->IsA(TClassBuilder))
            throw CryException("Bad Parent at end of copyto function");
        ClassBuilder *p = (ClassBuilder *)Parent;
        CryString Implementation;
        Implementation.Clear();
        Implementation.printf("\n\t}\n//Now copy the base class\n\t%s::CopyTo(Dest);\n\n}//%s\n\n",p->GetInheritType(), Parent->GetName());
        SetImp(PropertyName,Implementation);
        Parent->AppendImp(*GetImp(PropertyName));
        return this;
    }

    if (PropertyName==TInheritedFactory && Parent->IsA(TClassBuilder))
    {
        ClassBuilder *cbParent = (ClassBuilder *)Parent;
        CryString s,h,t,Tester,Header,Implementation;
        CryFunctionDef fd;
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
                        CryObject *o = (CryObject *)ci->Get();
                        {
                            CodeFactory *cf;
                            const char *N;
                            if (o->IsA(TCodeFactory))
                            {
                                cf = (CodeFactory *)o;
                                if (cf->GetIsProperty())
                                {
				CryString PropertyStart;
                                    const char *N = cf->GetName();
				if (cf->IsA(TCryProperty))
					PropertyStart.printf("PropertyName==%s.GetName()",N);
				else
				if (cf->IsA(TCryPropertyList))
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
                                if (o->IsA(TPrimInstance))
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
                                    if (o->IsA(TClassInstance))
                                    {
                                        ClassInstance *AClassInstance = (ClassInstance *)o;
                                        const char *N;
                                        if (AClassInstance->GetIsProperty())
                                        {
N = AClassInstance->GetName();
CryString PropertyStart;
if (AClassInstance->IsA(TCryProperty))
	PropertyStart.printf("PropertyName==%s.GetName()",N);
else
	if (AClassInstance->IsA(TCryPropertyList))
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
                        CryObject *o = (CryObject *)ci->Get();
                        {
                            CodeFactory *cf;
                            if (o->IsA(TCodeFactory))
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
                                if (o->IsA(TPrimInstance))
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
	fd.Parse("virtual CryList *PropertyNames() const");
	fd.GetNPDeclaration(Tester);
	if (Header==Tester)
	{
        if (!Parent->IsA(TClassBuilder))
            throw CryException("Bad Parent at PropertyNames function");
	    ClassBuilder *p = (ClassBuilder *)Parent;
            Clear(PropertyName);
            Header.printf("\n");
		Implementation = "\n\n/*!Get the names of all properties of this class*/\n";
            Implementation += Func->GetImplementedDeclaration(Parent->GetName(),true);
            Implementation.printf("\n{\n// get base class's properties");
			Implementation.printf("\nCryPropertyList *Names = %s::PropertyNames();",p->GetInheritType());
		Implementation.printf("\n// now add our own properties (if any)");
		{
		CryPropertyList *Names = p->Getp()->PropertyNames();
		CryPropertyList::PropertyIterator *i = Names->CreateIterator();
		Implementation.printf("\n\n// first, inherited properites which can be exposed by uncommenting");
		if(i->GotoFirst())
		{
			do
			{
			CryString r;
				Implementation.printf("\n//\tNames->AddProperty(\"%s\",\"%s\");",i->GetName(),i->GetValue(r));
			} while(i->GotoNext());
		}
		Names->DeleteIterator(i);
		delete Names;
		}
		Implementation.printf("\n\n// next properties that are local to this class");

				CompositeIterator *ci = new CompositeIterator(Parent);
				if (ci->GotoFirst())
				{
					do
					{
						CryObject *o = (CryObject *)ci->Get();
						{
							CodeFactory *cf;
							if (o->IsA(TCodeFactory))
							{
								cf = (CodeFactory *)o;
								if (cf->GetIsProperty())
								{
									const char *N = cf->GetName();
					Implementation.printf("\n\tNames->AddOwned(new CryString(\"%s\"));",N);
								}
							}
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
		fd.Parse("virtual void CopyTo(CryObject &Dest) const");
		fd.GetNPDeclaration(Tester);
		if (Header == Tester)
		{
			if (!CanBuildProduct(TCopyToEnd))
            {
                AddProduct(TCopyToEnd);
            }

            Clear(PropertyName);
            Header.printf("\n");
		Implementation = "\n/*!Copy this class and any parts of it to Dest, if possible*/";
            Implementation = Func->GetImplementedDeclaration(Parent->GetName(),true);
            Implementation.printf("\n{\n");
            Implementation.printf("\n\tif (Dest.IsA(T%s))\n\t{",Parent->GetName());
		Implementation.printf("\n\t\t// Copy this classes variables first");
            Implementation.printf("\n\t\t%s *CastDest = (%s *)&Dest;",Parent->GetName(),Parent->GetName());

            //            Implementation.printf("\t\t*CastDest = *this;");
            /*            Iterator *ci = Parent->_CreateIterator();
            if (ci->GotoFirst())
            {
            do
            {
            CryObject *o = (CryObject *)ci->Get();
            {
            CodeFactory *cf;
            CryString N;
            if (o->IsA(TCodeFactory))
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
            GetParent()->Create(TCopyTo,Parent);
            GetParent()->Create(TCopyToEnd,Parent);
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

