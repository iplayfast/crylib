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
#include <ctype.h>
#include "ClassBuilder.h"
#include "ClassObject.h"
#include "ClassArray.h"
#include "CryBackProp.h"
#include "CryFuzzy.h"
using namespace Crystal;
using namespace std;
#ifdef OldVersion
extern "C"
{
    bool DefaultWriteClassState(CryStream *Header,CryStream *Body,_ClassState &ClassState,_BuildState &BuildState,ClassBuilder *cb,_Ele *Ele);
    bool DefaultWriteClassState1(CryStream *Header,CryStream *Body,_ClassState &ClassState,_BuildState &BuildState,ClassBuilder *cb,_Ele *Ele);
};
CryString *gBody;
class BuildStateBuilder : public Strategy
{
    static int AddFunctionCount;
    ClassBuilder *cb;
public:
    BuildStateBuilder(ClassBuilder *_cb)
    {
        cb = _cb;
    }

    void AddFunction(const char *function);
    void Clear1()
    {
        cb->ScratchPad1.Clear();
    }
    const char *GetName()
    {
        return cb->GetName();
    }

    void Clear2()
    {
        cb->ScratchPad2.Clear();
    }
    void Clear()
    {
        Clear1();
        Clear2();
    }
    void FunctionsClear()
    {
        cb->Functions.Clear();
        AddFunctionCount = 0;
    }
    const char *Functions()
    {
        return cb->Functions.AsPChar();
    }
    void Iterate(int ID,PropertyType PType);
    virtual bool IterUpdate(_Ele *Ele,int ID)
    {
        return true;
    }
	virtual int DoStrategy(Object *Sender) const
    {
        _ClassState cs = Start;
        if (Sender->IsA(TEle))
        {
            Update(cs,(_Ele *)Sender);
			return -1;
        }
        throw CryException("Passed wrong class to DefaultBuildState DoStrategy");
    }

    bool Update(
        //CryStream *header,CryStream *Body,
        _ClassState &ClassState,_Ele *Ele)  const;
    CryString &ScratchPad1()
    {
        return cb->ScratchPad1;
    }
    CryString &ScratchPad2()
    {
        return cb->ScratchPad2;
    }
    const ClassBuilder *GetCB()
    {
        return cb;
    }
    const char *ClassName()
    {
        return cb->GetName();
    }
    const char *InheritType()
    {
        return cb->GetInheritType();
    }
    const CryString *GetInheritTypeStr() const
    {
        return cb->GetInheritTypeStr();
    }
    bool SetInheritedProperties(//CryStream *Header,const CryStream *Body,
        _Ele *Ele);

};


void BuildStateBuilder::Iterate(int ID,PropertyType PType)
{
    _Ele *Ele;
    CryContainer::Iterator *LocIterator = cb->CreateIterator();
    if (LocIterator->GotoFirst())
    {
        do
        {
            Ele =(_Ele *) LocIterator->Get();
            if ((Ele->CmpProperty("PropertyType",cb->GetPropertyTypeAsText(PType))) ||
                    (PType==Unknown))
            {
                if (IterUpdate(Ele,ID))
                    break;
            }
        }
        while(LocIterator->GotoNext());
        cb->DeleteIterator(LocIterator);
    }
};

int BuildStateBuilder::AddFunctionCount=0;
void BuildStateBuilder::AddFunction(const char *function)
{
    /*    CryString f;
        f.printf("%s;",function);
        f.Replace(";;",";");
        cb->ScratchPad1.Replace(f.AsPChar());*/
    CryFunctionDef fd;
    fd.Parse(function);
    CryString a;
    cb->Functions.printf("\ts +=\"%s\";\n",fd.GetNPDeclaration(a).AsPChar());
}

bool BuildStateBuilder::Update(//CryStream *header,CryStream *Body,
    _ClassState &ClassState,_Ele *Ele)  const
{
    switch (ClassState)
    {
    case Start:
        {
            CryString Blank;
            // for every element remove any previously generated code that isn't a function
            if (Ele->CmpProperty("PropertyType","AbstractFunction")!=0)
            {
                Ele->SetProperty("Declare",Blank);
                Ele->SetProperty("DeclareComment",Blank);
                Ele->SetProperty("Body",Blank);
                Ele->SetProperty("BodyComment",Blank);
            }
            else
            {
                CryString Body,Name,Declare,PrimitiveType,DeclareComment,BodyComment,_Value,IsProperty,Count,BaseClass,PropertyName;
                Ele->GetProperty("Body",Body);
                Ele->GetProperty("Name",Name);
                Ele->GetProperty("Declare",Declare);
                Ele->GetProperty("PrimitiveType",PrimitiveType);
                Ele->GetProperty("DeclareComment",DeclareComment);
                Ele->GetProperty("BodyComment",BodyComment);
                Ele->GetProperty("_Value",_Value);
                Ele->GetProperty("IsProperty",IsProperty);
                Ele->GetProperty("Count",Count);
                Ele->GetProperty("BaseClass",BaseClass);
                Ele->GetProperty("PropertyName",PropertyName);
            }
        }
        // generate code for the footer
        return false;// so every element get's processed
    case MidStart:
        if (Ele->CmpProperty("PropertyType","Section")==0)
        {
            if (Ele->CmpProperty("PropertyName","Header")==0)
            {
                CryString BodyComment;
                /*! ToDo Make this loadable from an external file (for other peoples copyright notices */
                BodyComment.printf("//****************************************************\n");
                BodyComment.printf("//Copyright 2005\n");
                BodyComment.printf("// Crystal Software (Canada) Inc.\n");
                BodyComment.printf("//****************************************************\n");
                Ele->SetProperty("DeclareComment",BodyComment);
                Ele->SetProperty("BodyComment",BodyComment);
                return true;
            }
        }
        return false;
    case MidMid:
    case MidEnd:
    case End:
        return true;
    }
    return true;
}



bool BuildStateBuilder::SetInheritedProperties(
    //CryStream *Header,
    //const CryStream *Body,
    _Ele *Ele)
{

    if (Ele->CmpProperty("PropertyType","AbstractFunction")==0)
    {
        CryString Body,Name;
        Ele->GetValue("Name",Name);
        Ele->GetValue("Body",Body);
        if (cb->ScratchPad1.Pos(Name.AsPChar())==0) // already there
        {
            if (Body=="")
            {
                Body.printf("%s\n{\n// base class has this abstract function you need to create\n}\n",Name.AsPChar());
                Ele->SetProperty("Body",Body);
            }
        }
        return true;
    }
    if (Ele->CmpProperty("PropertyType","StubFunction")==0)
    {
        if (Ele->CmpProperty("Body","")==0)
        {
            CryString Body,Name;
            Ele->GetValue("Name",Name);
            Body.printf("%s\n{\n// Stub\n\t //%s::%s;\n}\n",Name.AsPChar(),cb->GetInheritTypeStr(),
                        Name.AsPChar());
            Ele->SetProperty("Body",Body);
        }
        return true;
    }
    if (Ele->CmpProperty("PropertyType","Inherited")==0)
    {
        CryString Body,Name,Type;
        Ele->GetValue("Name",Name);
        Ele->GetValue("Body",Body);
        Type = GetInheritTypeStr();
        CryString p;
        bool SaveToBody = (Body=="");
        if ((Type==TCryString) || (Type.strstr("char *")))//(*Ele->Type=="char *") || (*Ele->Type=="const char *"))
            p.printf("const char *Get%s(CryString &Result) const",Name.AsPChar());
        else
            p.printf("%s Get%s(CryString &Result) const",Type.AsPChar(),Name.AsPChar());
        {
            CryString n;
            n = p;
            n.printf(";\n");
            n = p;
            n.printf(";\\n");
        }
        if (SaveToBody)
        {
            Body.printf("%s\n{\n\treturn GetProperty(\"%s\",Result);\n}\n",p.AsPChar(),Name.AsPChar());
            Ele->SetProperty("Body",Body);
        }
        p = "";
        p.printf("bool Set%s(const CryString &Value)",Name.AsPChar());
        if (SaveToBody)
        {
            Body.printf("%s\n{\n\treturn SetProperty(\"%s\",Value);\n}\n",p.AsPChar(),
                        Name.AsPChar(),Name.AsPChar());
            Ele->SetProperty("Body",Body);
        }
    }
    return true;
}

class BSB_StartBuildState : public BuildStateBuilder
{
public:
    BSB_StartBuildState(ClassBuilder *cb) : BuildStateBuilder(cb)
    {}
}
;

class BSB_ConstructorDestructor : public BSB_StartBuildState
{
public:
    BSB_ConstructorDestructor(ClassBuilder *cb) : BSB_StartBuildState(cb)
    {}
    bool SetHeaderConstructor(_Ele *Ele);
    bool SetHeaderDestructor(_Ele *Ele);
};

class BSB_Constructor : public BSB_ConstructorDestructor
{
public:
    BSB_Constructor(ClassBuilder *cb) : BSB_ConstructorDestructor(cb)
    {}
    bool SetHeader(_Ele *Ele)
    {
        return SetHeaderConstructor(Ele);
    }
    bool Update(_ClassState &ClassState,_Ele *Ele);
};
class BSB_Destructor : public BSB_ConstructorDestructor
{
public:
    BSB_Destructor(ClassBuilder *cb) : BSB_ConstructorDestructor(cb)
    {}
    bool SetHeader(_Ele *Ele)
    {
        return SetHeaderDestructor(Ele);
    }
    bool Update(_ClassState &ClassState,_Ele *Ele);
};


bool BSB_ConstructorDestructor::SetHeaderDestructor(_Ele *Ele)
{
    if (Ele->CmpProperty("PropertyType","ClassPtr")==0)
    {
        CryString Name,CountDefineName;
        Ele->GetValue("Name",Name);
        Ele->GetValue("CountDefineName",CountDefineName);
        if (Ele->CmpProperty("Count","1")==0)
            ScratchPad1().printf("\tdelete %s;\n",Name.AsPChar());
        else
            ScratchPad1().printf("\tfor(int i=0;i<%s;i++)\n\t\tdelete %s[i];\n",
                                 CountDefineName.AsPChar(),Name.AsPChar());
        return true;
    }
    return false;
}

bool BSB_ConstructorDestructor::SetHeaderConstructor(_Ele *Ele)
{
    if (Ele->CmpProperty("PropertyType","ClassPtr")==0)
    {
        CryString Name,Type,CountDefineName;
        Ele->GetValue("Name",Name);
        Type = GetInheritTypeStr();
        Ele->GetValue("CountDefineName",CountDefineName);
        if (Ele->CmpProperty("Count","1")==0)
            ScratchPad1().printf("\n\t%s = new %s();\n",Name.AsPChar(),Type.AsPChar());
        else
            ScratchPad1().printf("\n\tfor(int i=0;i<%s;i++)\n\t\t%s[i] = new %s();\n",
                                 CountDefineName.AsPChar(),Name.AsPChar(),Type.AsPChar());
        return true;
    }

    if (Ele->CmpProperty("PropertyType","Primitive")==0)
    {
        CryString Name,Type,CountDefineName,_Value;
        Ele->GetValue("Name",Name);
        Type = Ele->GetValue("PrimitiveType",Type);
        Ele->GetValue("_Value",_Value);
        Ele->GetValue("CountDefineName",CountDefineName);
        if (Type=="char *")
        {
            if (Ele->CmpProperty("Count","1")==0)
                ScratchPad1().printf("\n\t%s = \"%s\";\n",
                                     Name.AsPChar(),_Value.AsPChar());
            else
                ScratchPad1().printf("\n\tstrncpy(%s,\"%s\",%s);\n",
                                     Name.AsPChar(),_Value.AsPChar(),
                                     CountDefineName.AsPChar());
            return true;
        }
        if (Type=="char")
        {
            if (Ele->CmpProperty("Count","1")==0)
                ScratchPad1().printf("\n\t%s = '%c';\n",
                                     Name.AsPChar(),_Value.AsPChar());
            else
                ScratchPad1().printf("\n\tstrncpy(%s[i],\"%s\",%s);\n",
                                     Name.AsPChar(),_Value.AsPChar(),
                                     CountDefineName.AsPChar());
            return true;
        }


        {
            CryString rValue;
            Ele->GetValue("_Value",rValue);
            if (Type=="int")
            {
                rValue = "0";
                rValue.scanf("%d",_Value.AsPChar());
            }
            if (Type=="float")
            {
                rValue = "0.0";
                rValue.scanf("%f",_Value.AsPChar());
            }
            if (Type=="double")
            {
                rValue = "0.0";
                rValue.scanf("%f",_Value.AsPChar());
            }
            if (Ele->CmpProperty("Count","1")==0)
                ScratchPad1().printf("\n\t%s = %s;\n",
                                     Name.AsPChar(),
                                     rValue.AsPChar());
            else
                ScratchPad1().printf("\n\tfor(int i=0;i<%s;i++)\n\t\t%s[i] = %s;\n",
                                     CountDefineName.AsPChar(),
                                     Name.AsPChar(),
                                     rValue.AsPChar());
        }
    }
    return true;
}
const char *gchar;
bool BSB_Constructor::Update(_ClassState &ClassState,_Ele *Ele)
{
    switch(ClassState)
    {
    case Start:
        {
            if ((Ele->CmpProperty("PropertyType","Section") != 0) ||
                    (Ele->CmpProperty("PropertyName","Constructor") != 0))
                return false;
            Clear();
            CryString DeclareComment;
            Ele->GetValue("DeclareComment",DeclareComment);
            DeclareComment.printf("\n// ***********************************************************************\n");
            DeclareComment.printf("// Public Functions\n");
            DeclareComment.printf("// ***********************************************************************\n");
            DeclareComment.printf("public:\n");
            Ele->SetProperty("DeclareComment",DeclareComment);

            CryString Declare,Body;
            Ele->GetValue("Declare",Declare);
            Declare.printf("\t%s();\n",GetName());
            Ele->SetProperty("Declare",Declare);

            Ele->GetValue("Body",Body);
            Body.printf("\n//constructor\n%s::%s()\n{",GetName(),GetName());
            if (GetCB()->Getp()->IsContainer())
                Body.printf("\n\tLocIterator = CreateIterator();\n");
            Ele->SetProperty("Body",Body);
            return true;
        }
    case MidStart:
        if ((Ele->CmpProperty("PropertyType","Section") == 0) &&
                (Ele->CmpProperty("PropertyName","Constructor") == 0))
        {
            return false;
        }
        SetHeaderConstructor(Ele);
        return false;
    case MidMid:
        {
            if ((Ele->CmpProperty("PropertyType","Section") != 0) ||
                    (Ele->CmpProperty("PropertyName","Constructor") != 0))
                return false;
            CryString Body,BodyComment,DeclareComment;
            Ele->GetValue("Body",Body);
            Ele->GetValue("BodyComment",BodyComment);
            Ele->GetValue("DeclareComment",DeclareComment);
            Body.printf("%s}// %s\n\n",ScratchPad1().AsPChar(),GetName()); // end of constructor code
            BodyComment.printf(" /*! Default %s constructor */\n",GetName());
            DeclareComment.printf(" /*! Default %s constructor */\n",GetName());
            Ele->SetProperty("Body",Body);
            Ele->SetProperty("BodyComment",BodyComment);
            Ele->SetProperty("DeclareComment",DeclareComment);
        }
        return true;
    case MidEnd:
    case End:
        return true;
    }
    return true;
}
bool BSB_Destructor::Update(_ClassState &ClassState,_Ele *Ele)
{
    switch(ClassState)
    {
    case Start:
        {
            if ((Ele->CmpProperty("PropertyType","Section") != 0) ||
                    (Ele->CmpProperty("PropertyName","Destructor") != 0))
                return false;
            CryString Declare,Body;
            Ele->GetValue("Declare",Declare);
            Ele->GetValue("Body",Body);
            Clear();
            Declare.printf("\t~%s();\n",GetName());
            Body.printf("%s::~%s()\n{",GetName(),GetName());
            if (GetCB()->Getp()->IsContainer())
                Body.printf("\n\t\tDeleteIterator(LocIterator);\n");
            Ele->SetProperty("Declare",Declare);
            Ele->SetProperty("Body",Body);
        }
        return true;
    case MidStart:
        SetHeaderDestructor(Ele);
        return false;
    case MidMid:
        {
            if ((Ele->CmpProperty("PropertyType","Section") != 0) ||
                    (Ele->CmpProperty("PropertyName","Destructor") != 0))
                return false;
            CryString Body,BodyComment,DeclareComment;
            Ele->GetValue("Body",Body);
            Ele->GetValue("BodyComment",BodyComment);
            Ele->GetValue("DeclareComment",DeclareComment);

            Body.printf("%s}// ~%s\n\n",ScratchPad1().AsPChar(),GetName()); // end of constructor code
            BodyComment.printf(" /*!  %s destructor */\n",GetName());
            DeclareComment = BodyComment;
            Ele->SetProperty("Body",Body);
            Ele->SetProperty("BodyComment",BodyComment);
            Ele->SetProperty("DeclareComment",DeclareComment);
        }
        return true;
    case MidEnd:
    case End:
        return true;
    }
    return true;
}

class BSB_Initial : public BSB_StartBuildState
{
public:
    BSB_Initial(ClassBuilder *cb) : BSB_StartBuildState(cb)
    {}
    bool Update(_ClassState &ClassState,_Ele *Ele);
};

/*! This function finds all include files that will be needed.
It does this by adding the includes to the ScratchPad1 for any instance of  a type that needs an include
at the end will add the formated includes along with comments to the declaration section
*/
bool BSB_Initial::Update(_ClassState &ClassState,_Ele *Ele)
{
    switch (ClassState)
    {
    case Start:
        if ((Ele->CmpProperty("PropertyType","Section") != 0) ||
                (Ele->CmpProperty("PropertyName","Header") != 0))
            return false;


        Clear1();
        FunctionsClear();
        ScratchPad1().printf("#ifndef T%s\n",GetName());
        ScratchPad1().printf("#define T%s\t\"%s\"\n\n",GetName(),GetName());
        ScratchPad1().printf("//standard files\n#include\t<string.h>\n#include\t<stdlib.h>\n\n");
        ScratchPad1().printf("//Crystal files\n");
        {// get the base class's include(s)
            CryString s;
            GetCB()->GetInheritTypeInclude(s);
            ScratchPad1() += s;
        }
        return true;
    case MidStart:	// if any element is a class, get it's include(s)
        if ((Ele->CmpProperty("PropertyType","Section") != 0) ||
                (Ele->CmpProperty("PropertyName","Footer") != 0))
            return false;
        {
            CryString Declare,BodyComment;
            Ele->GetValue("Declare",Declare);
            Ele->GetValue("BodyComment",BodyComment);
            Declare.printf("}; // class %s\n\n}; // namespace Crystal \n\n#endif //T%s\n// End of file\n",
                           GetName(),GetName());
            BodyComment.printf("\n/* %s End of file */\n",GetName());
            Ele->SetProperty("Declare",Declare);
            Ele->SetProperty("BodyComment",BodyComment);
        }
        return true;
    case MidMid:
        if (*GetInheritTypeStr()=="TCryDoubleArray")
			ScratchPad1().ExclusiveAppend("#include\t\"ClassArray.h\"\n");
        if (*GetInheritTypeStr()=="TCryBPNetContainer")
            ScratchPad1().ExclusiveAppend("#include\t\"CryBackProp.h\"\n");
        if (*GetInheritTypeStr()=="TCryFuzzy")
            ScratchPad1().ExclusiveAppend("#include\t\"CryFuzzy.h\"\n");
        return false;
    case MidEnd:
        return true;
    case End:
        if ((Ele->CmpProperty("PropertyType","Section")!=0) ||
                Ele->CmpProperty("PropertyName","Header")!=0)
            return false;
        {
            CryString Declare,Body;
            Ele->GetValue("Declare",Declare);
            Ele->GetValue("Body",Body);

            Declare.printf("%s\n",ScratchPad1().AsPChar());
            {
                CryString s = GetCB()->GetFilename();
                if (s=="")
                    s = GetName();
                Body.printf("\n#include \"%s.h\"\n",s.AsPChar());
            }
			Body.printf("#include \"ClassXML.h\"\t//Needed for SetDefaultValues\n");
            Body.printf("\n\nusing namespace Crystal;\n");
            Declare.printf("namespace Crystal {\nusing namespace Crystal;\n\n");//}
            Declare.printf("class %s : public %s\n",GetName(),InheritType());
            Declare.printf("{\n");//}
            Ele->SetProperty("Declare",Declare);
            Ele->SetProperty("Body",Body);
        }
        return true;
    }
    return true;
}

class BSB_IteratedFunctions : public BSB_StartBuildState
{
public:
    BSB_IteratedFunctions(ClassBuilder *cb) : BSB_StartBuildState(cb)
    {}
    bool Update(_ClassState &ClassState,_Ele *Ele);
};

bool BSB_IteratedFunctions::Update(_ClassState &ClassState,_Ele *Ele)
{
    if (ClassState != Start)
        return true;
    if (!(Ele->CmpProperty("PropertyType","Section")==0 &&
            Ele->CmpProperty("PropertyName","Iterator")==0))
        return false;

    const char *EleType = GetCB()->GetEleType();

    CryString Body,BodyComment,Declare,DeclareComment;
    Ele->GetValue("Body",Body);
    Ele->GetValue("BodyComment",BodyComment);
    Ele->GetValue("DeclareComment",DeclareComment);
    Ele->GetValue("Declare",Declare);
    DeclareComment.printf("\n// **************************************************************\n");
    DeclareComment.printf("// Iterator functions available because this is a container class\n");
    DeclareComment.printf("// **************************************************************\n");
	Declare.printf("Container::Iterator *LocIterator;\n");
    if (EleType[0]!='\0')// we are using a fixed type of _Ele
    {
        Declare.printf("\n/*!  IControl is only set up as an example of an easy way to have the Iterated function called for multiple reasons\n");
        Declare.printf("      Use the \"State\" variable to select which reason it's being called.*/\nstruct IControl\n{\n//int State;\n};\n\n");
        CryString source;
        source.printf("virtual bool IteratedFunction(%s::IControl *Control,%s *Item)",GetName(),EleType);
        AddFunction(source);
        Declare.printf("/*! Our own iterated function using a contol set up internally */\n");
        Declare.printf("%s;\n",source.AsPChar());
        Body.printf("%s","\n/*! IteratedFunction is called for each item in the container (from IteratedThroughAll)");
        Body.printf("%s","returns true if iteration should stop */");

        Body.printf("\nbool %s::IteratedFunction(%s::IControl *Control,%s *Item)\n{\n//\tswitch(Control->State) ...\n\treturn false;\n}\n", GetName(), GetName(), EleType);
        source = "virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item)";
        AddFunction(source);
        Declare.printf("\n/*! The virtual function called from CryContainer for each iteration of the container */\n");
        Declare.printf("%s\n{\n\treturn IteratedFunction((IControl *)Control,(%s *)Item);\n}\n",source.AsPChar(),EleType);
        {
            CryString source,s1;
            {
                s1.printf("%s*%s::AddOwned(%s * Item,size_t Size)", EleType,GetName(),EleType);
                Body.printf("%s\n{\n\t%s::AddOwned(Item,Size);\n\treturn Item;\n}\n", s1.AsPChar(),InheritType());
                Declare.printf("\n/*! This function will add an item to the container.\n   The Owned part means that the container is responsible for deleting it and will use delete on whatever is added*/\n");
                source.printf("virtual void AddOwned(%s *Item,size_t Size)",EleType);
                AddFunction(source);
            }
            ScratchPad1().Replace(source.AsPChar());
            Declare.printf("%s;\n",source.AsPChar());
            {
                CryString source,s1;
                {
                    s1.printf("%s* %s::Add(%s * Item,size_t Size)",
                              EleType,GetName(),EleType);
                    Body.printf("%s\n{\n\t%s::Add(Item,Size);\n\treturn Item;\n}\n", s1.AsPChar(),InheritType());
                    Declare.printf("\n/*! This function will add an item to the container.\n  It is not owned so the container is NOT responsible for deleting it*/\n");
                    source.printf("virtual %s;\n",s1.AsPChar());
                }
                ScratchPad1().Replace(source.AsPChar());
                Declare.printf("%s",source.AsPChar());
            }

        }
        Declare.printf("\n/*! If thing at current location is in Object */\n");
        Declare.printf("\tbool I_IsObject() { return LocIterator->IsObject(); }\n");
        Declare.printf("\n/*! If thing at current location is owned by the Container (ie the container delete's it */\n");
        Declare.printf("\tbool I_IsOwned() { return LocIterator->IsOwned(); }\n");
        if (EleType[0]!='\0')
        {
            Declare.printf("\n/*! This container is specialized to handle %s's. Get the current %s the Iterator is pointing to */\n", EleType,EleType);
            Declare.printf("\t%s *I_Get() { return (%s *)LocIterator->Get(); }\n",EleType,EleType);
        }
        else
        {
            Declare.printf(
                "\n/*! This container is not specialized so it will return whatever the iterator is pointing to,\nthe calling function will need to cast it to the appropriate value */\n");
            Declare.printf("\tEmptyObject *I_Get() { return LocIterator->Get(); }\n");
        }
        Declare.printf("/*! Position the Iterator at the first item in the container (return true if items there)*/\n");
        Declare.printf("\tbool I_GotoFirst() { return LocIterator->GotoFirst(); }\n");
        AddFunction("bool I_GotoFirst()");
        Declare.printf("/*! Position the Iterator at the previous item in the container (could be slow for list type containers*/\n");
        Declare.printf("\tbool I_GotoPrev() { return LocIterator->GotoPrev(); }\n");
        AddFunction("bool I_GotoPrev()");
        Declare.printf("/*! Position the Iterator at the next item in the container */\n");
        Declare.printf("\tbool I_GotoNext() { return LocIterator->GotoNext(); }\n");
        AddFunction("bool I_GotoNext()");
        Declare.printf("/* remove the item at the iterator */\n");
        Declare.printf("\tvirtual EmptyObject *RemoveAtIterator(Iterator *I) {};\n");
        AddFunction("EmptyObject *RemoveAtIterator(Iterator *I)");
        Declare.printf("/*! Position the Iterator at the last item in the container */\n");
        Declare.printf("\tbool I_GotoLast() { return LocIterator->GotoLast(); }\n");
        AddFunction("bool I_GotoLast()");
        Declare.printf("\tsize_t I_GetItemSize() { return LocIterator->GetItemSize(); }\n");
        AddFunction("bool I_GetItemSize()");
        //bool LoadAsText(CryString *FromStream) { return OrigContainer->LoadAsText(this,FromStream); }
        Declare.printf("\tbool I_SaveAsText(CryString &ToBody) { return  LocIterator->SaveAsText(ToBody); }\n");
        AddFunction("bool I_SaveAsText(CryString &ToBody)");
        Declare.printf("\tconst CryContainer *I_GetOrigContainer() const  { return LocIterator->GetOrigContainer(); }\n");
        AddFunction("const CryContainer *I_GetOrigContainer() const");

    }
    Ele->SetProperty("Body",Body);
    Ele->SetProperty("BodyComment",BodyComment);
    Ele->SetProperty("DeclareComment",DeclareComment);
    Ele->SetProperty("Declare",Declare);

    return true;
}
class BSB_HeaderInstanceVariables: public BSB_StartBuildState
{
public:
    BSB_HeaderInstanceVariables(ClassBuilder *cb) : BSB_StartBuildState(cb)
    {}
    bool Update(_ClassState &ClassState,_Ele *Ele);
};
bool BSB_HeaderInstanceVariables::Update(_ClassState &ClassState,_Ele *Ele)
{
    CryString ScratchPad;
    bool didComment = false;
    switch(ClassState)
    {
    case Start:
        {
            if ((Ele->CmpProperty("PropertyType","Section")!=0) ||
                    Ele->CmpProperty("PropertyName","Local Variables Declaration")!=0)
                return false;
            _Ele *iEle;// filling in the local variables
            CryContainer::Iterator *LocIterator = GetCB()->CreateIterator();
            if (LocIterator->GotoFirst())
            {
                do
                {
                    const char *cdn,*n,*t,*c;
                    CryString PropertyName,Count,PrimitiveType,CountDefineName;
		    iEle = (_Ele *)LocIterator->Get();
                    iEle->GetProperty("PrimitiveType",PrimitiveType);
                    iEle->GetProperty("PropertyName",PropertyName);
                    iEle->GetProperty("Count",Count);
                    iEle->GetValue("CountDefineName",CountDefineName);
                    cdn = CountDefineName.AsPChar();
                    n = PropertyName.AsPChar();
                    t = PrimitiveType.AsPChar();
                    c = Count.AsPChar();
                    if (iEle->LogChanges)
                        iEle->MarkChange();
                    if (iEle->CmpProperty("PropertyType","Primitive")==0)
                    {
                        CryString s,t1,t2;

                        if (iEle->CmpProperty("Count","1")==0)
                            s.printf("%s %s;\n",PrimitiveType.AsPChar(),PropertyName.AsPChar());
                        else
                        {
                            const char *cdn,*n,*t,*c;
                            cdn = CountDefineName.AsPChar();
                            n = PropertyName.AsPChar();
                            t = PrimitiveType.AsPChar();
                            c = Count.AsPChar();
                            s.printf("#define %s\t%s\n",cdn,c);
                            s.printf("%s %s[%s];\n",t,n,cdn);
                        }
                        ScratchPad.printf("%s",s.AsPChar());
                        if (Count=="1")
                        {
                            CryString h,b;
                            const char *cdn,*n,*t,*c;
                            cdn = CountDefineName.AsPChar();
                            n = PropertyName.AsPChar();
                            t = PrimitiveType.AsPChar();
                            c = Count.AsPChar();
                            h.printf("%s Get%s() const;\n",t,PropertyName.AsPChar());
                            b.printf("%s %s::Get%s() const\n{\n\treturn %s;\n}\n",t,GetName(),n,n);
                            h.printf("void Set%s(%s _%s);\n",n,t,n);
                            b.printf("void %s::Set%s(%s _%s)\n{\n\t%s = _%s;\n}\n",GetName(),n,t,n,n,n);
                            iEle->SetDefaultCode(h,b);
                        }
                        else
                        {
                            CryString h,b;
                            h.printf("%s Get%s(int Index) const;\n",t,n);
                            b.printf("%s %s::Get%s(int Index) const\n{\n",t,GetName(),n);
                            b.printf("#ifdef RangeChecking\n");
                            b.printf("\tif ((Index<0) || (Index>=%s))\n\tthrow CryException(\"Index Out of range in Get%s(Index)\");\n#endif\n\treturn %s[Index];\n}\n",cdn,n,n,n);
                            h.printf("void Set%s(int Index,%s _%s);\n",n,t,n);
                            b.printf("void %s::Set%s(int Index,%s _%s)",GetName(),n,t,n);
                            b.printf("\n{\n#ifdef RangeChecking\n");
                            b.printf("\tif ((Index<0) || (Index>=%s))\n\tthrow CryException(\"Index Out of range in Set%s(Index,_%s)\");\n#endif\n",cdn,n,n);
                            b.printf("\t%s[Index] = _%s;\n}\n",n,n);
                            iEle->SetDefaultCode(h,b);
                        }



                    }
                    if (iEle->CmpProperty("PropertyType","ClassPtr")==0)
                    {
                        CryString ClassType,Count,PropertyName,CountDefineName;
                        iEle->GetProperty("PrimitiveType",ClassType);
                        iEle->GetProperty("PropertyName",PropertyName);
                        iEle->GetProperty("Count",Count);
                        iEle->GetValue("CountDefineName",CountDefineName);
                        CryString s,t1,t2;
                        if (iEle->CmpProperty("Count","1")==0)
                            s.printf("%s *%s;\n",ClassType.AsPChar(),PropertyName.AsPChar());
                        else
                        {
                            const char *cdn,*n,*t,*c;
                            cdn = CountDefineName.AsPChar();
                            n = PropertyName.AsPChar();
                            t = ClassType.AsPChar();
                            c = Count.AsPChar();
                            s.printf("#define %s\t%s\n",cdn,c);
                            s.printf("%s *%s[%s];\n", t,n,cdn);
                        }
                        ScratchPad.printf("%s",s.AsPChar());
                        {
                            const char *c = GetName();      // class name
                            const char *n = PropertyName.AsPChar();
                            const char *t = ClassType.AsPChar();
                            const char *r = CountDefineName.AsPChar();  // range
                            CryString h,b;
                            if (!didComment)
                            {
                                didComment = true;
                                CryString DeclareComment;
                                DeclareComment.printf("\n// ***********************************************************************\n");
                                DeclareComment.printf("//  Local Instance Variables Handling routines\n");
                                DeclareComment.printf("// ***********************************************************************\n");
                                iEle->SetProperty("DeclareComment",DeclareComment);
                                iEle->SetProperty("BodyComment",DeclareComment);
                            }
                            if (Count=="1")
                            {
                                h.printf("%s *Get%s() const;\n",t,n);
                                b.printf("%s *%s::Get%s() const\n{\n\treturn %s;\n}\n",t,GetName(),n,n);
                                /*                        h.printf("void Set%s(%s _%s);\n",n,t,n);
                                                        b.printf("void %s::Set%s(%s _%s)\n{\n\t%s = _%s;\n}\n",c,n,t,n,n,n);
                                */
                                iEle->SetDefaultCode(h,b);
                            }
                            else
                            {
                                h.printf("%s *Get%s(int Index) const;\n",t,n);
                                b.printf("%s *%s::Get%s(int Index) const\n{\n",t,GetName(),n);
                                b.printf("#ifdef RangeChecking\n");
                                b.printf("\tif ((Index<0) || (Index>=%s))\n\tthrow CryException(\"Index Out of range in Get%s(Index)\");\n#endif\n\treturn %s[Index];\n}\n",cdn,n,n,n);
                                /*                        h.printf("void Set%s(int Index,%s *_%s);\n",n,t,n);
                                                        b.printf("void %s::Set%s(int Index,%s *_%s)",c,n,t,n);
                                                        b.printf("\n{\n#ifdef RangeChecking\n");
                                                        b.printf("\tif ((Index<0) || (Index>=%s))\n\tthrow CryException(\"Index Out of range in Set%s(Index,_%s)\");\n#endif\n",r,n,n);
                                                        b.printf("\t*%s[Index] = *_%s;\n}\n",n,n);*/
                                iEle->SetDefaultCode(h,b);
                            }
                        }
                    }
                }
                while(LocIterator->GotoNext());
                GetCB()->DeleteIterator(LocIterator);
            }
            /// if this section is "Local Variables Declarations" then handle it, else keep looking
            /* Local Variables are declared in the Section Element in order to keep room
               for Get/Set Property code in the actual variables section
            */
            {
                CryString DeclareComment,Declare;
                Ele->GetValue("DeclareComment",DeclareComment);
                Ele->GetValue("Declare",Declare);
                if (ScratchPad!="")		// if local variable(s) was found
                {
                    DeclareComment.printf("\n// ***********************************************************************\n");
                    DeclareComment.printf("//  Local Instance Variables\n");
                    DeclareComment.printf("// ***********************************************************************\n");
                    Declare = (const char *)ScratchPad.AsPChar();
                }
                else
                {
                    DeclareComment = "";
                    Declare = "";
                }
                Ele->SetProperty("DeclareComment",DeclareComment);
                Ele->SetProperty("Declare",Declare);
            }
        }
    default:
        return true;
    }
    return true;
}

class BSB_InheritedProperties: public BSB_StartBuildState
{
public:
    BSB_InheritedProperties(ClassBuilder *cb) : BSB_StartBuildState(cb)
    {}
    bool Update(_ClassState &ClassState,_Ele *Ele);
};

bool BSB_InheritedProperties::Update(_ClassState &ClassState,_Ele *Ele)
{
    switch(ClassState)
    {
    case Start:
        if ((Ele->CmpProperty("PropertyType","Section")!=0) ||
                (Ele->CmpProperty("PropertyName","Inherited Properties")!=0))
            return false;
        {
            CryString DeclareComment,Declare,Body;
            Ele->GetValue("DeclareComment",DeclareComment);
            Ele->GetValue("Declare",Declare);
            /// added to header because this function should always be present
            DeclareComment.printf("\n// ***********************************************************************\n");
            DeclareComment.printf("// Inherited Properties\n");
            DeclareComment.printf("// ***********************************************************************\n");
            Ele->SetProperty("BodyComment",DeclareComment);
            DeclareComment.printf("\n\n// --- Functions for setting and retrieving property values ---\n");
            DeclareComment.printf("// if a Get function returns a char * that is different\n");
            DeclareComment.printf("// then the Result, it implies a special situation\n");
            DeclareComment.printf("// (usually a container class)\n");
            {
                CryString a;
                CryFunctionDef f("virtual const char *GetProperty(const char *PropertyName,CryString &Result) const");
                AddFunction(f.GetDeclaration(a).AsPChar());
                Declare.printf("\n%s\n",f.GetDeclaration().AsPChar());
                Body.printf("%s\n{\n",f.GetImplementedDeclaration(GetName()).AsPChar());//}
            }
            Ele->SetProperty("DeclareComment",DeclareComment);
            Ele->SetProperty("Declare",Declare);
            Ele->SetProperty("Body",Body);
        }
        return true;  // so goes to next state (goes through all elements again)
    case MidStart:
        if (Ele->CmpProperty("IsProperty","Y")==0)
        {
            CryString Name;
            Ele->GetValue("Name",Name);
            ScratchPad2().printf("\tif (strncmp(PropertyName,\"%s\",%d)==0)\n\t{\n", Name.AsPChar(),::strlen(Name.AsPChar()));//}
            if (Ele->CmpProperty("PropertyType","ClassPtr")==0)
            {
                if (Ele->CmpProperty("Count","1")!=0)
                {
                    ScratchPad2().printf("\t\tint i = atoi(PropertyName + %d);\n",::strlen(Name.AsPChar()+2));
                    ScratchPad2().printf("\t\t %s[i]->SaveTo(Result);\n",Name.AsPChar());
                    ScratchPad2().printf(/*{*/"\t\treturn Result.AsPChar();\n\t}\n");
                }
                else
                {
                    ScratchPad2().printf("\t\t%s->SaveTo(Result);\n",Name.AsPChar());
                    ScratchPad2().printf(/*{*/"\t\treturn Result.AsPChar();\n\t}\n");
                }
            }
            if (Ele->CmpProperty("PropertyType","Primitive")==0)
            {
                CryString n;
                if (Ele->CmpProperty("Count","1")!=0)
                {
                    ScratchPad2().printf("\tint i = atoi(PropertyName + %d);\n",::strlen(Name.AsPChar()+1));
                    n.printf("[i]");
                }
                if (Ele->CmpProperty("PrimitiveType","Bool")==0)
                    ScratchPad2().printf("\t\t{char ch;\n\t\t\tResult.scanf(\"%%c\",&ch);\n\t\t\t%s%s = (c=='1');\n\t\t} ",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","pBool")==0)
                    ScratchPad2().printf("\t\t{char ch;\n\t\t\tResult.scanf(\"%%c\",&ch);\n\t\t\t*%s%s = (c=='1');\n\t\t} ",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","Char")==0)
                    ScratchPad2().printf("\t\tResult.scanf(\"%%c\",&%s%s);\n",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","pChar")==0)
                    ScratchPad2().printf("\t\tResult.scanf(\"%%s\",%s%s);\n",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","Int")==0)
                    ScratchPad2().printf("\t\tResult.scanf(\"%%d\",&%s%s);\n",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","pInt")==0)
                    ScratchPad2().printf("\t\tResult.scanf(\"%%d\",%s%s);\n",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","Float")==0)
                    ScratchPad2().printf("\t\tResult.scanf(\"%%f\",&%s%s);\n",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","pFloat")==0)
                    ScratchPad2().printf("\t\tResult.scanf(\"%%f\",%s%s);\n",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","Double")==0)
                    ScratchPad2().printf("\t\tResult.scanf(\"%%lf\",&%s%s);\n",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","pDouble")==0)
                    ScratchPad2().printf("\t\tResult.scanf(\"%%lf\",%s%s);\n",Name.AsPChar(),n.AsPChar());
                //{
                ScratchPad2().printf("\t\treturn Result.AsPChar();\n\t}\n");
            }
            // any other types of properties possible?
        }
        return false; // so every _Ele is checked
    case MidMid:
        {
            if ((Ele->CmpProperty("PropertyType","Section")!=0) ||
                    (Ele->CmpProperty("PropertyName","Inherited Properties")!=0))
                return false;
            CryString Declare,Body;
            Ele->GetValue("Body",Body);
            Ele->GetValue("Declare",Declare);//{
            ScratchPad2().printf("\treturn %s::GetProperty(PropertyName,Result);\n} // GetProperty\n\n",InheritType());
            Body.printf("%s",ScratchPad2().AsPChar());
            {
                CryFunctionDef f("virtual bool SetProperty(const char *PropertyName,CryString &PropertyValue)");
                AddFunction(f.GetDeclaration().AsPChar());
                Declare.printf("%s\n",f.GetDeclaration().AsPChar());
                Body.printf("%s\n{\n",f.GetImplementedDeclaration(GetName()).AsPChar());//}
            }
            Ele->SetProperty("Body",Body);
            Ele->SetProperty("Declare",Declare);
            Clear();
        }
        return true;
    case MidEnd:
        if (Ele->CmpProperty("IsProperty","Y")==0)
        {
            CryString Name;
            Ele->GetValue("Name",Name);
            ScratchPad2().printf("\tif (strncmp(PropertyName,\"%s\",%d)==0)\n\t{\n", Name.AsPChar(),::strlen(Name.AsPChar()));
            if (Ele->CmpProperty("PropertyType","ClassPtr")==0)
            {
                if (Ele->CmpProperty("Count","1")!=0)
                {
                    ScratchPad2().printf("\tint i = atoi(PropertyName + %d);\n",::strlen(Name.AsPChar()+1));
                    ScratchPad2().printf("\t\t %s[i]->LoadFrom(PropertyValue);\n",Name.AsPChar());
                    ScratchPad2().printf("\t\treturn true;\n}\n");
                }
                else
                {
                    ScratchPad2().printf("\t\t%s->LoadFrom(PropertyValue);\n",Name.AsPChar());
                    ScratchPad2().printf("\t\treturn true;\n\t}\n");
                }
            }
            if (Ele->CmpProperty("PropertyType","Primitive")==0)
            {
                CryString n;
                if (Ele->CmpProperty("Count","1")!=0)
                {
                    ScratchPad2().printf("int i = atoi(PropertyName + %d);\n",::strlen(Name.AsPChar()+1));
                    n.printf("[i]");
                }
                if (Ele->CmpProperty("PrimitiveType","Bool")==0)
                    ScratchPad2().printf("\t\t&%s%s = (PropertyValue->AsPChar()[0]=='1');\n",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","pBool")==0)
                    ScratchPad2().printf("\t\t%s%s = (PropertyValue->AsPChar()[0]=='1');\n",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","Char")==0)
                    ScratchPad2().printf("\t\t&%s%s = PropertyValue->AsPChar()[0];\n",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","pChar")==0)
                    ScratchPad2().printf("\t\t%s%s = *PropertyValue.AsPChar();\n",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","Int")==0)
                    ScratchPad2().printf("\t\t%s%s = atoi(PropertyValue.AsPChar());\n",Name.AsPChar(),n.AsPChar());
                else if (Ele->CmpProperty("PrimitiveType","pInt")==0)
                    ScratchPad2().printf("\t\t*%s%s = atoi(PropertyValue.AsPChar());\n",Name.AsPChar(),n.AsPChar());
                else if ((Ele->CmpProperty("PrimitiveType","Double")==0) ||
                         (Ele->CmpProperty("PrimitiveType","Float")==0))
                    ScratchPad2().printf("\t\t*%s%s = atod(PropertyValue.AsPChar());\n",Name.AsPChar(),n.AsPChar());
                //{
                ScratchPad2().printf("\t\treturn true;\n\t}\n");
            }

            // any other types of properties possible?
        }
        return false;
    case End:
        if ((Ele->CmpProperty("PropertyType","Section")!=0) ||
                (Ele->CmpProperty("PropertyName","Inherited Properties")!=0))
            return false;
        {
            CryString Declare,Body;
            Ele->GetValue("Declare",Declare);
            Ele->GetValue("Body",Body);
            Declare += ScratchPad1();
            Body += ScratchPad2();//{
            Body.printf("\treturn %s::SetProperty(PropertyName,PropertyValue);\n} // SetProperty\n\n",InheritType());
            Ele->SetProperty("Declare",Declare);
            Ele->SetProperty("Body",Body);
            return true;
        }
    }
    return true;
}

class BSB_InheritedFunctions: public BSB_StartBuildState
{
public:
    BSB_InheritedFunctions(ClassBuilder *cb) : BSB_StartBuildState(cb)
    {}
    bool Update(_ClassState &ClassState,_Ele *Ele);
    virtual bool IterUpdate(_Ele *Ele,int ID);

};
bool BSB_InheritedFunctions::IterUpdate(_Ele *Ele,int ID)
{
    if (ID==0)  // doing CopyTo function
    {

    }
    return true;
}

bool BSB_InheritedFunctions::Update(_ClassState &ClassState,_Ele *Ele)
{
    switch(ClassState)
    {
    case Start:
        if (Ele->CmpProperty("PropertyType","Section")==0 &&
                Ele->CmpProperty("PropertyName","Inherited Functions")==0)
        {
            CryString DeclareComment,Declare;
            Ele->GetValue("DeclareComment",DeclareComment);
            Ele->GetValue("Declare",Declare);
            DeclareComment.printf("\n// ***********************************************************************\n");
            DeclareComment.printf("// Inherited Functions\n");
            DeclareComment.printf("// ***********************************************************************\n");

            CryString s;
            s = "virtual const char *ChildClassName() const";
            AddFunction(s);
            Declare.printf("\t%s\n\t{\n\t\treturn T%s;\n\t}\n\n",s.AsPChar(), GetName());
            s = "virtual bool IsA(const char *GetName) const";
            AddFunction(s);
            Declare.printf("\t%s\n\t{\n\t\treturn (::strcmp(GetName,T%s)==0) ||\n\t\t %s::IsA(GetName);\n\t}\n\n", s.AsPChar(),
                           GetName(),InheritType());
            Ele->SetProperty("DeclareComment",DeclareComment);
            Ele->SetProperty("Declare",Declare);
            return true;
        }
        return false;
    case MidStart:
        if (Ele->CmpProperty("PropertyType","AbstractFunction")==0)
        {
            CryFunctionDef fd;
            CryString Declare,declare,Body;
            Ele->GetValue("Declare",Declare);
            fd.Parse(Declare);
            Declare = fd.GetDeclaration();
            fd.Parse("virtual Object *Dup() const");
            declare = fd.GetDeclaration();
            if (Declare != declare)
                return false;
            AddFunction(Declare);
            Ele->SetProperty("Declare",Declare);

            Body = fd.GetImplementedDeclaration(GetName());
            Body.printf("\n{\n\t%s *New = new %s();\n",GetName(),GetName());
            Body.printf("\tCopyTo(*New);\n\treturn New;\n}\n");
            Ele->SetProperty("Body",Body);
            return true;
        }
    case MidMid:
    case MidEnd:
        return true;
        /*        {
                CryFunctionDef fd;
                CryString Declare,declare,Body;
                   Ele->GetValue("Declare",Declare);
                   fd.Parse(Declare);
                   Declare = fd.GetDeclaration();
                  fd.Parse("virtual void CopyTo(Object &Dest) const");
                  declare = fd.GetDeclaration();
         
                  if (Declare != declare)
                    return false;
                }
                Iterate(0,Property);
                if (Ele->PType==Inherited)
                {
                    ScratchPad1().printf("%s",Ele->DeclareComment.AsPChar());
                    ScratchPad1().printf("%s;\n\n",Ele->Name.AsPChar());
                    CryString s = Ele->Name;
                    s.Replace("virtual ","");
                    if (Ele->Body=="")
                    {
                        Ele->Body.printf("///Function could not be calculated add class name \n%s\n{\n\t//stub function\n}\n",s.AsPChar());
                    }
                }
                return false;
          */
    case End:
        if (Ele->CmpProperty("PropertyType","Section")==0 &&
                Ele->CmpProperty("PropertyName","Inherited Functions")==0)
        {
            CryString Declare,Body;
            Ele->GetValue("Declare",Declare);
            Ele->GetValue("Body",Body);
            Declare += ScratchPad1();
            Body += ScratchPad2();
            Ele->SetProperty("Declare",Declare);
            Ele->SetProperty("Body",Body);
            return true;
        }
        return false;
    }
    return true;
}

class BSB_CopyToParts: public BSB_StartBuildState
{
public:
    BSB_CopyToParts(ClassBuilder *cb) : BSB_StartBuildState(cb)
    {}
    bool Update(_ClassState &ClassState,_Ele *Ele);
};
bool BSB_CopyToParts::Update(_ClassState &ClassState,_Ele *Ele)
{
    switch(ClassState)
    {
    case Start:
        {
            if (Ele->CmpProperty("PropertyType","AbstractFunction")==0)
            {
                CryFunctionDef fd;
                CryString Declare,declare,Body;
                Ele->GetValue("Declare",Declare);
                fd.Parse(Declare);
                Declare = fd.GetDeclaration();
                fd.Parse("virtual void CopyTo(Object &Dest) const");
                declare = fd.GetDeclaration();

                if (Declare != declare)
                    return false;
                AddFunction(Declare);
                Body = fd.GetImplementedDeclaration(GetName());
                Clear();
                CryString Comment;
                Comment.printf("\t/*!Copy this class and any parts of it to Dest*/\n");
                Ele->SetProperty("DeclareComment",Comment);
                Ele->SetProperty("BodyComment",Comment);
                Ele->SetProperty("Declare",Declare);
                ScratchPad2().printf("\n\tif (Dest.IsA(T%s))\n\t{\n",GetName());
                ScratchPad2().printf("\t%s *CastDest = (%s *)&Dest;\n",GetName(),GetName());
                return true;
            }
        }
    case MidStart:
        {
            CryString Name;
            Ele->GetValue("Name",Name);
            CryString brackets;
            if (Ele->CmpProperty("Count","1")!=0)
                brackets = "[i]";
            else
                brackets = "";

            CryString tBody;	// build up tBody so if it's empty we aren't writing stuff we don't need to
            if (Ele->CmpProperty("PropertyType","Primitive")==0)
                tBody.printf("\t\tCastDest->%s%s = %s%s;\n", Name.AsPChar(),brackets.AsPChar(),Name.AsPChar(),brackets.AsPChar());
            else if (Ele->CmpProperty("PropertyType","LocalProperty")==0)
            {
                tBody.printf("\t\tCastDest->%s.Ele->Name%s = %s.Ele->Name%s;\n", Name.AsPChar(),brackets.AsPChar() ,Name.AsPChar(),brackets.AsPChar());
                tBody.printf("\t\tCastDest->%s.Value%s = %s.Value%s;\n", Name.AsPChar(),brackets.AsPChar() ,Name.AsPChar(),brackets.AsPChar());
            }
            else if (Ele->CmpProperty("PropertyType","ClassPtr")==0)
                tBody.printf("\t\t%s%s->CopyTo(*CastDest->%s%s);\n",Name.AsPChar(),brackets.AsPChar(), Name.AsPChar(),brackets.AsPChar());

            if (tBody != "")
            {
                CryString CountDefineName;

                if (Ele->CmpProperty("Count","1")!=0)
                {
                    Ele->GetValue("CountDefineName",CountDefineName);
                    ScratchPad1().printf("\t\tfor(int i=0;i<%s;i++)\n\t\t{\n",//}
                                         CountDefineName.AsPChar());
                    ScratchPad1().printf("%s",tBody.AsPChar());
                    if (Ele->CmpProperty("Count","1")!=0)//{
                        ScratchPad1().printf("\t\t}\n");
                }
            }
        }
        return false;
    case MidMid:
        if (Ele->CmpProperty("PropertyType","AbstractFunction")==0)
        {
            CryFunctionDef fd;
            CryString Declare,declare,Body,Comment;
            Ele->GetValue("Declare",Declare);
            fd.Parse(Declare);
            Declare = fd.GetDeclaration();
            fd.Parse("virtual void CopyTo(Object &Dest) const");
            declare = fd.GetDeclaration();

            if (Declare != declare)
                return false;
            Body.printf("\n%s\n{",(const char *)fd.GetImplementedDeclaration(GetName()));
            if (ScratchPad1()=="")
            {
                Body.printf("\n/* If any data is added to this class uncomment this and add the data accordingly\n");
                Body.printf("%s",(const char *)ScratchPad2());
                Body.printf("\t}\n*/\n");
            }
            else
            {
                Body.printf("%s",(const char *)ScratchPad2());
                Body.printf("%s",(const char *)ScratchPad1());
                //{
                Body.printf("\t}\n");
            }                                                  //{
            Body.printf("\n\t%s::CopyTo(Dest); //Copy Base Class\n}\n",InheritType());
            Ele->SetProperty("Declare",Declare);
            Ele->SetProperty("Body",Body);
            return true;
        }
        return false;
    case MidEnd:
    case End:
        return true;
    }
    return true;
}

class BSB_GetFunctions: public BSB_StartBuildState
{
public:
    BSB_GetFunctions(ClassBuilder *cb) : BSB_StartBuildState(cb)
    {}
    bool Update(_ClassState &ClassState,_Ele *Ele);
};

bool BSB_GetFunctions::Update(_ClassState &ClassState,_Ele *Ele)
{
    switch(ClassState)
    {
    case Start:
        if (Ele->CmpProperty("PropertyType","AbstractFunction")==0)
        {
            CryFunctionDef fd;
            CryString Declare,declare,Body,Comment;
            Ele->GetValue("Declare",Declare);
            fd.Parse(Declare);
            Declare = fd.GetDeclaration();
            fd.Parse("virtual CryFunctionDefList *GetFunctions(const char *Type=0) const");
            declare = fd.GetDeclaration();
            if (Declare != fd.GetDeclaration())
                return false;
            Clear();
            const char *id = fd.GetImplementedDeclaration(GetName());
            ScratchPad1().printf("%s\n",id);
            ScratchPad1().printf("{\nCryFunctionDefList *l = %s::GetFunctions();\n",InheritType());
            ScratchPad1().printf("\tif (Type && !IsA(Type))\n\t\treturn l;\n");
            ScratchPad1().printf("String s;\n\ts += \"//  %s;\"\n",ClassName());
            _Ele *Ele;
            CryContainer::Iterator *LocIterator = GetCB()->CreateIterator();
            if (LocIterator->GotoFirst())
            {
                do
                {

                    Ele =(_Ele *) LocIterator->Get();
                    if ((*Ele->GetValue("Declare") != "") &&
                            ((*Ele->GetValue("PropertyType")=="StubFunction") ||
                             (*Ele->GetValue("PropertyType")=="AbstractFunction"))
                       )
                    {
                        fd.Parse(Ele->GetValueAsPChar("Declare"));
                        ScratchPad1().printf("\ts += \"%s\";\n",fd.GetDeclaration().AsPChar());
                    }
                }
                while(LocIterator->GotoNext());
                GetCB()->DeleteIterator(LocIterator);
            }
            ScratchPad1().printf("%s",Functions());
            ScratchPad1().printf("\tl->LoadFromString(&s,\";\");\n");
            ScratchPad1().printf("\treturn l;\n}\n");
        }
        Ele->SetProperty("Body",ScratchPad1());
        return true;
    }
}

class BSB_GetSize: public BSB_StartBuildState
{
    void CodeSize(_Ele *Ele);
public:
    BSB_GetSize(ClassBuilder *cb) : BSB_StartBuildState(cb)
    {}
    bool Update(_ClassState &ClassState,_Ele *Ele);
};

void BSB_GetSize::CodeSize(_Ele *Ele)
{
    CryString Name,Count;
    int iCount;
    Ele->GetValue("Name",Name);
    Ele->GetValue("Count",Count);
    Count.scanf("%d",&iCount);

    if (iCount>1)
    {
        CryString CountDefineName;
        Ele->GetValue("CountDefineName",CountDefineName);
        if (Ele->CmpProperty("PropertyType","Primitive")!=0)
        {
            ScratchPad2().printf("\t\tsize += sizeof(%s);\n",Name.AsPChar());
            return;
        }

        if (Ele->CmpProperty("PropertyName","LocalProperty")==0)
        {
            ScratchPad2().printf("\tfor(int i=0;i<%s;i++)\n\t{\n",CountDefineName.AsPChar());
            ScratchPad2().printf("\t\tsize += %s[i].Size();\n",Name.AsPChar());
            ScratchPad2().printf("\t}\n");
            return;
        }

        if (Ele->CmpProperty("PropertyName","ClassPtr")==0)
        {
            ScratchPad2().printf("\tfor(int i=0;i<%s;i++)\n\t{\n",CountDefineName.AsPChar());
            ScratchPad2().printf("\t\tsize += %s[i]->Size();\n",Name.AsPChar());
            ScratchPad2().printf("\t}\n");
            return;
        }
    }
    else
        if (Ele->CmpProperty("PropertyType","Primitive")==0)
        {
            ScratchPad2().printf("\t\tsize += sizeof(%s);\n",Name.AsPChar());
            return;
        }
    if (Ele->CmpProperty("PropertyName","LocalProperty")==0)
    {
        ScratchPad2().printf("\t\tsize += %s.Size();\n",Name.AsPChar());
        return;
    }
    if (Ele->CmpProperty("PropertyName","ClassPtr")==0)
    {
        ScratchPad2().printf("\t\tsize += %s->Size();\n",Name.AsPChar());
        return;
    }
    return;
}
// todo need to handle getting the size for an array

bool BSB_GetSize::Update(_ClassState &ClassState,_Ele *Ele)
{
    CryString Name;
    Ele->GetValue("Name",Name);
    switch(ClassState)
    {
    case Start:
        if (Name.Pos("Size()")==-1)
            return false;
        Clear();
        {
            CryFunctionDef fd;
            fd.Parse(Name);
            CryString b;
            fd.GetDeclaration(Name);
            Ele->SetProperty("Declare",Name);
            fd.GetImplementedDeclaration(GetName(),b);
            AddFunction(b);
            const char *n = Name.AsPChar();
            const char *bp = b.AsPChar();
            const char *i = InheritType();
            ScratchPad1().printf("%s\t%s\n",bp,n);
            ScratchPad2().printf("%s\n{\nsize_t size = %s::Size();\n",bp,i);
        }
        return true;
    case MidStart:
        CodeSize(Ele);
        return false;
    case MidMid:
    case MidEnd:
        return true;
    case End:
        {
            if (Name.Pos("Size()")==-1)
                return false;
        }
        ScratchPad2().printf("\treturn size;\n}\n");
        {
            CryString Body;
            Body = "";
            Ele->SetProperty("Body",Body);
        }
        CryString dummy1;
        CryString  dummy2;
        Ele->SetDefaultCode(
            ScratchPad1().AsPChar(),
            ScratchPad2().AsPChar());
        return true;
    }
    return true;
}
class BSB_Stubs : public BSB_StartBuildState
{
public:
    BSB_Stubs(ClassBuilder *cb) : BSB_StartBuildState(cb)
    {}
    bool Update(_ClassState &ClassState,_Ele *Ele);
};

bool BSB_Stubs::Update(_ClassState &ClassState,_Ele *Ele)
{
    switch(ClassState)
    {
    case Start:
        if (Ele->CmpProperty("PropertyName","StubFunction")==0)
        {
            CryString Comment;
            Comment = "";
            Ele->SetProperty("DeclareComment",Comment);
            Ele->SetProperty("BodyComment",Comment);
        }
        return false;
    case MidStart:
        if (Ele->CmpProperty("PropertyName","StubFunction")==0)
        { // for first one found, add header comment
            CryString Comment;
            Comment.printf("\n// ***********************************************************************\n");
            Comment.printf("// Stub Functions You need to fill these in yourself\n");
            Comment.printf("// ***********************************************************************\n");
            Ele->SetProperty("DeclareComment",Comment);
            Ele->SetProperty("BodyComment",Comment);
            return true;
        }
        return false;
    case MidMid:
        if (Ele->CmpProperty("PropertyName","StubFunction")==0)
        {
            CryString t,Name;
            CryFunctionDef fd;
            Ele->GetValue("Name",Name);
            fd.Parse(Name);
            fd.GetDeclaration(t);
            Ele->SetProperty("Declare",t);
            fd.GetImplementedDeclaration(InheritType(),t);
            t.printf("\n{\n// Stub function needs to be filled in\n");
            t.printf("// or call\n//\t%s::%s\n}\n\n",InheritType(),Name.AsPChar());
            Ele->SetProperty("Body",t);
        }
        return false;
    case End:
        return true;
    }
    return true;
}

class BSB_FinalOutput : public Strategy
{
    ClassBuilder *Parent;
public:
    BSB_FinalOutput(ClassBuilder *_Parent)
	{
        Parent = _Parent;
    }
	virtual int DoStrategy(Object *Sender) const
    {
        Parent->Sort();
        if (Sender->IsA(TTwoStreams))
        {
            CryStream *Header = ((TwoStreams *)Sender)->GetStream1();
            CryStream *Body = ((TwoStreams *)Sender)->GetStream2();
            PrintSection("Header",Header,Body,Unknown,Unknown);
            PrintSection("Local Variables Declaration",Header,Body,Primitive,ClassPtr);
            PrintSection("Constructor",Header,Body,Unknown,Unknown);
            PrintSection("Destructor",Header,Body,ClassPtr,Primitive);
            PrintSection("Iterator",Header,Body,Unknown,Unknown);
            PrintSection("Local Variables Function",Header,Body,Unknown,Unknown);
            PrintSection("Inherited Functions",Header,Body,AbstractFunction,StubFunction);
            PrintSection("Inherited Properties",Header,Body,Unknown,Unknown);
            PrintSection("Footer",Header,Body,Unknown,Unknown);
        }
        return -1;
    }
    void PrintType(PropertyType PType,CryStream *Header,CryStream *Body) const
    {
        CryString ElePropertyType,PropertyType;
        PropertyType = Parent->GetPropertyTypeAsText(PType);
        _Ele *Ele;
        CryContainer::Iterator *LocIterator = Parent->CreateIterator();
        if (LocIterator->GotoFirst())
        {
            do
            {
                Ele =(_Ele *) LocIterator->Get();
                Ele->GetValue("PropertyType",ElePropertyType);
                if (ElePropertyType==PropertyType)
                {
                    Ele->PrintHeader(Header);
                    Ele->PrintBody(Body);
                }
            }
            while(LocIterator->GotoNext());
            Parent->DeleteIterator(LocIterator);
        }
    };

    void PrintSection(const char *SectionName,CryStream *Header,CryStream *Body,PropertyType PType1,PropertyType PType2) const
    {
        _Ele *Ele;
        CryContainer::Iterator *LocIterator = Parent->CreateIterator();
        if (LocIterator->GotoFirst())
        {
            do
            {
                Ele =(_Ele *) LocIterator->Get();
                if ((Ele->CmpProperty("PropertyType","Section")==0) &&
                        (Ele->CmpProperty("PropertyName",SectionName)==0))
                {
                    Ele->PrintHeader(Header);
                    Ele->PrintBody(Body);
                    if (PType1!=Unknown)
                        PrintType(PType1,Header,Body);
                    if (PType2!=Unknown)
                        PrintType(PType2,Header,Body);
                    break;
                }
            }
            while(LocIterator->GotoNext());
            Parent->DeleteIterator(LocIterator);
        }
    }

};
/*    Initial,
    IteratedFunctions,
    HeaderInstanceVariables,
    HeaderConstructor,
    HeaderDestructor,
    InheritedProperties,
    InheritedFunctions,
    HeaderProtoLocal,
    CopyToParts,
    bsPropertyNames,
    bsSetProperty,
    bsGetProperty,
    GetSize,
    _EndBuildState;//HeaderPropInherit,HeaderPropLocal,HeaderProtoInherit,HeaderProtoLocal,BodyProt;
 
*/


extern "C"
{
    /*
        bool SetHeaderProtoLocal(
            CryStream *Header,
            CryString *GetFunctions,
            CryString *Functions,
            _Ele *Ele)
        {
        CryString PropertyType,PropertyName,Name,Body,Type,CountDefineName,BaseClass;
            Ele->GetValue("PropertyType",PropertyType);
            Ele->GetValue("PropertyName",PropertyName);
            Ele->GetValue("Name",Name);
            Ele->GetValue("Body",Body);
     
            Type = GetInheritTypeStr();
            Ele->GetValue("CountDefineName",CountDefineName);
            Ele->GetValue("BaseClass",BaseClass);
            if (PropertyType == "ClassPtr") return true;
            if (PropertyType == "Primitive")
                {
                    CryString p;
                    bool SaveToBody = (Body=="");
                    if (!SaveToBody) // Bodies have already been set and possibly modified by the user
                    {
                        Header->printf("%s",Body.AsPChar());
                        return true;
                    }
                    if (Ele->CmpProperty("Count","1")==0)
                    {
                        CryString s;
                        s.printf("%s Get%s() const",Type.AsPChar(),Name.AsPChar());
                        {
                            CryString n = s;
                            n.printf(";\\n");
                            Functions->printf("\tsource = \"%s\";\n\ts->ReplaceVirtual(source);\n",n.AsPChar());
                        }
                        s.printf("\n{\n\treturn %s;\n}\n",Name.AsPChar());
                        p.printf("%s",s.AsPChar());
                    }
                    else
                    {
                        p.printf("\n%s Get%s(int ItemIndex) const",Type.AsPChar(),Name.AsPChar());
                        {
                            CryString n = p;
                            n.printf(";\\n");
                            Functions->printf("\tsource = \"%s\";\n\ts->ReplaceVirtual(source);\n",n.AsPChar());
                        }
                        p.printf("\n{\n#ifdef _DEBUG\n\tif ((ItemIndex<0) || (ItemIndex>=%s))\n\t\tthrow CryException(\"Out of range\");\n#endif\n",
                                 CountDefineName.AsPChar());
                        p.printf("\treturn %s[ItemIndex];\n}\n",Name.AsPChar());
                    }
                    Body.printf("%s",p.AsPChar());
                    Ele->SetProperty("Body",Body);
                    Header->printf("%s",Body.AsPChar());
                    p.Clear();
                    if (Ele->CmpProperty("Count","1")==0)
                    {
                        p.printf("\nvoid Set%s(%s _%s)",Name.AsPChar(), Type.AsPChar(),Name.AsPChar());
                        {
                            CryString n = p;
                            n.printf(";\\n");
                            Functions->printf("\tsource = \"%s\";\n\ts->ReplaceVirtual(source);\n",n.AsPChar());
                        }
                        p.printf("\n{\n\t %s = _%s;\n}\n",Name.AsPChar(),Name.AsPChar());
                    }
                    else
                    {
                        p.printf("\nvoid Set%s(int ItemIndex,%s _%s)",
                                 Name.AsPChar(),Type.AsPChar(),Name.AsPChar());
                        {
                            CryString n = p;
                            n.printf(";\\n");
                            Functions->printf("\tsource = \"%s\";\n\ts->ReplaceVirtual(source);\n",n.AsPChar());
                        }
                        p.printf("\n{\n#ifdef _DEBUG\n\tif ((ItemIndex<0) || (ItemIndex>=%s))\n\t\tthrow CryException(\"Out of range\");\n#endif\n",
                                 CountDefineName.AsPChar());
                        p.printf("\t%s[ItemIndex] = _%s;\n}\n",Name.AsPChar(),Name.AsPChar());
                    }
                    Body.printf("%s",p.AsPChar());
                    Header->printf("%s",Body.AsPChar());
                }
                return true;
            if (PropertyType == "ClassPtrProp")
                {
                    CryString p;
                    bool SaveToBody = (Body=="");
                    if (Ele->CmpProperty("Count","1")==0)
                    {
                        if ((Type==TCryString) || (Type.strstr("char *")))
                            p.printf("const char *Get%s(CryString &Result) const",Name.AsPChar());
                        else
                            p.printf("%s Get%s(CryString &Result) const",
                                     Type.AsPChar(),Name.AsPChar());
                        {
                            CryString n = p;
                            n.printf(";\\n");
                            Functions->printf("\tsource = \"%s\";\n\ts->ReplaceVirtual(source);\n",n.AsPChar());
                            n = p;
                            n.printf(";\n");
                            GetFunctions->Replace(n.AsPChar(),"");
                        }
                        if (SaveToBody)
                            Body.printf("%s\n{\n\treturn %s->GetProperty(\"%s\",Result);\n}\n",p.AsPChar(),BaseClass.AsPChar(),PropertyName.AsPChar());
                        p = "";
                        p.printf("bool Set%s(const CryString &Value)",Name.AsPChar());
                        if (SaveToBody)
                        {
                            Body.printf("%s\n{\n\treturn %s->SetProperty(\"%s\",Value);\n}\n",
                                                    p.AsPChar(),BaseClass.AsPChar(),PropertyName.AsPChar());
                            Ele->SetProperty("Body",Body);
                        }
                        {
                            CryString n = p;
                            n.printf(";\\n");
                            Functions->printf("\tsource = \"%s\";\n\ts->ReplaceVirtual(source);\n",n.AsPChar());
                        }
                        p.printf(";\n");
                        GetFunctions->Replace(p.AsPChar(),"");
                        Header->printf("%s",Body.AsPChar());
                    }
                    else
                    {
                        if ((Type==TCryString) || (Type.strstr("char *")))
                            p.printf("const char *Get%s(int ItemIndex,CryString &Result) const",Name.AsPChar());
                        else
                            p.printf("%s Get%s(int ItemIndex,CryString &Result) const",
                                     Type.AsPChar(),Name.AsPChar());
                        if (SaveToBody)
                            Body.printf("%s\n{\n#ifdef _DEBUG\n\tif ((ItemIndex<0) || (ItemIndex>=%s))\n\t\tthrow CryException(\"Out of range\");\n#endif\n",
                                                    p.AsPChar(),CountDefineName.AsPChar());
                        {
                            CryString n = p;
                            n.printf(";\\n");
                            Functions->printf("\tsource = \"%s\";\n\ts->ReplaceVirtual(source);\n",n.AsPChar());
                        }
     
                        p.printf(";\n");
                        GetFunctions->Replace(p.AsPChar(),"");
                        if (SaveToBody)
                            Body.printf("\treturn %s[ItemIndex]->GetProperty(\"%s\",Result);\n}\n",
                              BaseClass.AsPChar(),PropertyName.AsPChar());
                        p = "";
                        p.printf("bool Set%s(int ItemIndex,const CryString &Value)",Name.AsPChar());
                        if (SaveToBody)
                            Body.printf("%s\n{\n#ifdef _DEBUG\n\tif ((ItemIndex<0) || (ItemIndex>=%s))\n\t\tthrow CryException(\"Out of range\");\n#endif\n",
                                                    p.AsPChar(),CountDefineName.AsPChar());
                        {
                            CryString n = p;
                            n.printf(";\\n");
                            Functions->printf("\tsource = \"%s\";\n\ts->ReplaceVirtual(source);\n",n.AsPChar());
                        }
                        p.printf(";\n");
                        GetFunctions->Replace(p.AsPChar(),"");
                        if (SaveToBody)
                        {
                            Body.printf("\treturn %s[ItemIndex]->SetProperty(\"%s\",Value);\n}\n",
                                    BaseClass.AsPChar(),BaseClass.AsPChar(),PropertyName.AsPChar());
                            Ele->SetProperty("Body",Body);
                        }
                        Header->printf("%s",Body.AsPChar());
                    }
                    return true;
                }
        }
    */

    /*	Handy reference
    enum PropertyType { Inherited,AbstractFunction,StubFunction,Local,ClassPtr,ClassPtrProp,
                    Property,LocalProperty,Primitive,Unknown };
    enum _PrimitiveType { Bool,pBool,Char,pChar,Int,pInt,Float,pFloat,Double,pDouble };
    enum _ClassState	{ Start,MidStart,MidMid,MidEnd,End };
    enum _BuildState {
    _StartBuildState,
    Initial,
    IteratedFunctions,
    HeaderInstanceVariables,
    HeaderConstructor,
    HeaderDestructor,
    InheritedProperties,
    InheritedFunctions,
    HeaderProtoLocal,
    CopyToParts,
    bsPropertyNames,
    bsSetProperty,
    bsGetProperty,
    GetSize,
    _EndBuildState};//HeaderPropInherit,HeaderPropLocal,HeaderProtoInherit,HeaderProtoLocal,BodyProt};
    */

    /*! Normal flow is:   For each BuildState  go through each class state, for each ClassState go though each element
      This can be shortend by setting the class or build state to the desired one. (it's a state engine). Changine either will lock both to your settings for next loop
      Returning false means show me the next element,  returning true means done (ie this state doesn't need any more elments, speeds things up).
      */
    extern "C"
    {
        void DumpState(CryStream *str,_ClassState &ClassState,_BuildState &BuildState,ClassBuilder *cb,_Ele *Ele,char *CallState)
        {

            char *pBS;
            switch (BuildState)
            {
            case _StartBuildState :
                pBS = "_StartBuildState";
                break;
            case  Initial:
                pBS = "Initial";
                break;
            case IteratedFunctions:
                pBS = "IteratedFunctions";
                break;
            case HeaderInstanceVariables:
                pBS = "HeaderInstanceVariables";
                break;
            case HeaderConstructor:
                pBS = "HeaderConstructor";
                break;
            case HeaderDestructor:
                pBS = "HeaderDestructor";
                break;
            case InheritedProperties:
                pBS = "InheritedProperties";
                break;
            case InheritedFunctions:
                pBS = "InheritedFunctions";
                break;
            case HeaderProtoLocal:
                pBS = "HeaderProtoLocal";
                break;
            case CopyToParts:
                pBS = "CopyToParts";
                break;
            case bsPropertyNames:
                pBS = "bsPropertyNames";
                break;
            case bsSetProperty:
                pBS = "bsSetProperty";
                break;
            case bsGetProperty:
                pBS = "bsGetProperty";
                break;
            case GetSize:
                pBS = "GetSize";
                break;
            case FinalOutput:
                pBS = "FinalOutput";
                break;
            case _EndBuildState:
                pBS = "_EndBuildState";
                break;
            default:
                pBS = "Unknown";
            }

            /// When building a class these aspects of it are analyised
            CryString PropertyType;
            Ele->GetValue("PropertyType",PropertyType);

            /// primitive types that classbuilder can use
            CryString PrimitiveType;
            Ele->GetValue("PrimitiveType",PrimitiveType);

            /// state machine values for class builder used when analizing the PropertyTypes
            char *State;
            switch(ClassState)
            {
            case  Start:
                State = "Start";
                break;
            case MidStart:
                State = "MidStart";
                break;
            case MidMid:
                State = "MidMid";
                break;
            case MidEnd:
                State = "MidEnd";
                break;
            case End :
                State = "End";
                break;
            default:
                State = "Unknown";
                break;
            }
            str->printf("%s BuildState = %s, ClassState = %s,PropertyType = %s, PrimitiveType = %s \n",
                        CallState,pBS,State,PropertyType.AsPChar(),PrimitiveType.AsPChar());
        }

        bool DefaultWriteClassState(CryStream *Header,CryStream *Body,
                                    _ClassState &ClassState,_BuildState &BuildState,ClassBuilder *cb,_Ele *Ele)
        {
            bool result;
            //            DumpState(Header,ClassState,BuildState,cb,Ele,"//Enter");
            //DumpState(Body,ClassState,BuildState,cb,Ele,"//Enter");
            result = DefaultWriteClassState1(Header,Body,ClassState,BuildState,cb,Ele);
            //            DumpState(Header,ClassState,BuildState,cb,Ele,"//Exit   ");
            //            DumpState(Body,ClassState,BuildState,cb,Ele,"//Exit   ");
            //            printf("\n\n\nHeader\n%s",((CryString *)Header)->AsPChar());
            //            printf("\n\n\nBody\n%s",((CryString *)Body)->AsPChar());

            return result;
        }




        bool DefaultWriteClassState1(CryStream *Header,CryStream *Body,
                                     _ClassState &ClassState,_BuildState &BuildState,ClassBuilder *cb,_Ele *Ele)
        {
            gBody = (CryString *)Header;
            const char *h = gBody->AsPChar();
            gBody = (CryString *)Body;
            const char *b = gBody->AsPChar();
            switch (BuildState)
            {
            case _StartBuildState:
                {
                    BSB_StartBuildState sb(cb);
                    return sb.Update(ClassState,Ele);
                }
            case 	Initial:
                {
                    BSB_Initial ib(cb);
                    return ib.Update(ClassState,Ele);
                }
            case IteratedFunctions:
                {
                    BSB_IteratedFunctions IF(cb);
                    if (cb->Getp()->IsContainer())
                        IF.Update(ClassState,Ele);
                    else
                        return true;
                    return false;
                }
            case 	    HeaderInstanceVariables:
                {
                    BSB_HeaderInstanceVariables hi(cb);
                    return hi.Update(ClassState,Ele);
                }
            case HeaderConstructor:
                {
                    BSB_Constructor cd(cb);
                    return cd.Update(ClassState,Ele);
                }
            case HeaderDestructor:
                {
                    BSB_Destructor cd(cb);
                    return cd.Update(ClassState,Ele);
                }
            case     InheritedProperties:
                {
                    BSB_InheritedProperties ip(cb);
                    return ip.Update(ClassState,Ele);
                }
            case InheritedFunctions:
                {
                    BSB_InheritedFunctions IF(cb);
                    return IF.Update(ClassState,Ele);
                }
            case     HeaderProtoLocal:
                return true;
            case     CopyToParts:
                {
                    BSB_CopyToParts CP(cb);
                    return CP.Update(ClassState,Ele);
                }
            case     bsPropertyNames:
                return true;
            case     bsSetProperty:
                return true;
            case     bsGetProperty:
                return true;
            case        bsGetFunctions:
                {
                    BSB_GetFunctions bf(cb);
                    return bf.Update(ClassState,Ele);
                }
            case     GetSize:
                {
                    BSB_GetSize gs(cb);
                    return gs.Update(ClassState,Ele);
                }
            case Stubs:
                {
                    BSB_Stubs st(cb);
                    return st.Update(ClassState,Ele);
                }
            case FinalOutput:

                if (ClassState==Start)
                {
                    BSB_FinalOutput a(cb);
                    TwoStreams o(Header,Body);
                    a.DoStrategy(&o);
                }
                return true;


            }
            return true;  // should never get here
        }
    }
}
Strategy *_LanguageFactory::Create(_BuildState StateName,ClassBuilder *Parent)
{
    switch (StateName)
    {
    case _StartBuildState:
        return new BSB_StartBuildState(Parent);
    case 	Initial:
        return new BSB_Initial(Parent);
    case IteratedFunctions:
        return new BSB_IteratedFunctions(Parent);
    case HeaderInstanceVariables:
        return new BSB_HeaderInstanceVariables(Parent);
    case HeaderConstructor:
        return new BSB_Constructor(Parent);
    case HeaderDestructor:
        return new BSB_Destructor(Parent);
    case     InheritedProperties:
        return new BSB_InheritedProperties(Parent);
    case InheritedFunctions:
        return new BSB_InheritedFunctions(Parent);
    case     HeaderProtoLocal:
        return 0;
    case     CopyToParts:
        return new BSB_CopyToParts(Parent);
    case     bsPropertyNames:
        return 0;
    case     bsSetProperty:
        return 0;
    case     bsGetProperty:
        return 0;
    case   bsGetFunctions:
        return new BSB_GetFunctions(Parent);
    case     GetSize:
        return new BSB_GetSize(Parent);
    case Stubs:
        return new BSB_Stubs(Parent);
    case FinalOutput:
        return new BSB_FinalOutput(Parent);

    };
}
#endif //OldVersion
