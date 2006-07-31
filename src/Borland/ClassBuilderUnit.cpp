//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ClassBuilderUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <io.h>
#pragma link "CheckLabel"
#pragma link "labelEditfrm"
#pragma resource "*.dfm"
#include "CryObject.h"
#include "ClassString.h"
#include "ClassException.h"
#include "ClassBuilder.h"
#include "ClassFileStream.h"
#include "BitArray.h"
#include "CryXML.h"
#include "ClassFunction.h"
#include "HugeInt.h"

using namespace Crystal;

TClassBuilderFrm *ClassBuilderFrm;
//---------------------------------------------------------------------------
__fastcall TClassBuilderFrm::TClassBuilderFrm(TComponent* Owner)
	: TForm(Owner)
{
	ANewClass = new ClassBuilder();
	PropertyCount = 0;
	RefreshView();
	AddVariables->Enabled = false;
	RemoveVariable->Enabled = false;

}
//---------------------------------------------------------------------------
void __fastcall TClassBuilderFrm::SetBaseClassBTClick(TObject *Sender)
{
	try
	{
		CryString a(ClassTypeCB->Items->Strings[ClassTypeCB->ItemIndex].c_str());
		delete ANewClass;
		ANewClass = new ClassBuilder();
		ANewClass->SetBaseClass(a,IncludeStubs->Checked,true,NameED->Edit1->Text.c_str());
		//        ANewClass->SetName(NameField->getText().text());

		AddVariables->Enabled = true;
		RemoveVariable->Enabled = true;
		ClassBuilderFrm->Caption = "ClassBuilder" + NameED->Edit1->Text;
		AddVariables->Enabled = true;
		VariableNameEditedYet = false;
		RefreshView();
		UpdateStatus(Sender);
		PageControl1->ActivePage = AddVariables;
	}
	catch(CryException &e)
	{
		const char *cp = e;
		AnsiString a;
		a.printf("Setting Base Class Error: %s",cp);
		MessageDlg(a.c_str(),mtInformation,TMsgDlgButtons() << mbOK,0);
	}
}
//---------------------------------------------------------------------------

void __fastcall TClassBuilderFrm::ArrayCountChange(TObject *Sender)
{
AnsiString a;
	a.sprintf("%d",ArrayCount->Position);
	ArrayAmountLb->Caption = a;
	if (ArrayCount->Position > 1)
	{
		PointerToArray->Enabled = true;
	}
	else
	{
		PointerToArray->Checked = false;
		PointerToArray->Enabled = false;
	}
	this->UpdateStatus(Sender);
	return;
}
//---------------------------------------------------------------------------

void __fastcall TClassBuilderFrm::FormCreate(TObject *Sender)
{
	if (ANewClass)
    	delete ANewClass;
	ANewClass = new ClassBuilder();
	AddVariables->Enabled = false;
	RemoveVariable->Enabled = false;

	ClassTypeCB->Clear();
	ClassTypeCB->AddItem(TCryObject,0);
	ClassTypeCB->Text = TCryObject;                                        	

	ClassTypeCB->AddItem(TCryContainer,0);
	ClassTypeCB->AddItem(TCryStream,0);
	ClassTypeCB->AddItem(TCryMemStream,0);
	ClassTypeCB->AddItem(TCryFileStream,0);
	ClassTypeCB->AddItem(TCryString,0);
	ClassTypeCB->AddItem(TCryProperty,0);
	ClassTypeCB->AddItem(TCryList,0);
	ClassTypeCB->AddItem(TCryPropertyList,0);
	// From CryArray
	ClassTypeCB->AddItem(TCrySimpleArray,0);
	ClassTypeCB->AddItem(TCryArray,0);
	ClassTypeCB->AddItem(TCryDoubleArray,0);
	// From BitArray
	ClassTypeCB->AddItem(TBitArray,0);

	// from CryXml
	ClassTypeCB->AddItem(TCryXMLNode,0);
	ClassTypeCB->AddItem(TCryXML,0);
	// From CryFuzzy
	ClassTypeCB->AddItem(TCryFuzzy,0);
	// From CryBackProp
//	ClassTypeCB->AddItem("BackPropagateLayer",0);
	ClassTypeCB->AddItem(TCryBPNetContainer,0);

	ClassTypeCB->AddItem(TCryFunctionDef,0);
	ClassTypeCB->AddItem(TCryFunctionDefList,0);
	// from HugeINt
	ClassTypeCB->AddItem(THugeInt,0);
	// from CryPattern
	ClassTypeCB->AddItem(TStrategy,0);
	ClassTypeCB->AddItem(TStrategyHolder,0);
	ClassTypeCB->AddItem(TStrategyHolderSender,0);
	ClassTypeCB->AddItem(TObserver,0);
	ClassTypeCB->AddItem(TObservable,0);
	ClassTypeCB->AddItem(TDecorator,0);
	ClassTypeCB->AddItem(TCryFactory,0);
	ClassTypeCB->AddItem(TCryOFactory,0);
	ClassTypeCB->AddItem(TCommandHolder,0);
	ClassTypeCB->AddItem(TCommandHolderSender,0);
	ClassTypeCB->AddItem(TCompositeIterator,0);
	ClassTypeCB->AddItem(TState,0);

	ClassTypeCB->ItemIndex = 0;

	ClassType2CB->Items = ClassTypeCB->Items;
	ClassType2CB->ItemIndex = 0;

	VariableTypeCB->AddItem("bool",0);
	VariableTypeCB->AddItem("char",0);
	VariableTypeCB->AddItem("int",0);
	VariableTypeCB->AddItem("float",0);
	VariableTypeCB->AddItem("double",0);

	VariableTypeCB->ItemIndex = 0;

}
//---------------------------------------------------------------------------
void __fastcall TClassBuilderFrm::RefreshView(void)
{
	CryString SourceHeader,SourceBody;
	SourceHeader.SetLength(10000);
	ANewClass->SaveHeaderBody(SourceHeader,SourceBody);

	TTreeNode *pt=0,*body,*name,*type,*value,*count,*bc,*pn,*MainTree,*SourceTree;
	//    const CryString *Body,*Name,*Type,*BaseClass,*pPropertyName,*Value;
	CryString InheriteName;
	CryString CountDefineName;
	//    int *iPtype,Count;
	//    ClassBuilder::PropertyType _PropertyType,LastPT = ClassBuilder::Unknown;

	TreeView1->Items->Clear();
	MainTree = name = TreeView1->Items->AddFirst(0,ANewClass->GetName());
	{
		SourceTree = TreeView1->Items->AddChild(name,"Complete Source Code:");
		body = TreeView1->Items->AddChild(SourceTree,"Header:");
		CryString s;
		s = SourceHeader;
		s.Replace("\t","   ");
		s.SetTerminator('\n');
		while(s.Size())
		{
			CryString t;
			t.SetTerminator('\n');
			t.WriteTE((CryString *)s.GetAddress(),500);
			s.Delete(0,t.Size()+1);
			s.SeekFromStart(0);
			t.Trim();
			if (t=="")
				t = " ";
			//            else
			//              t.Insert(0,"|");
			TreeView1->Items->AddChild(body,t.AsPChar());
		}
		s = SourceBody;
		body = TreeView1->Items->AddChild(SourceTree,"Body:");
		s.Replace("\t","   ");
		s.SetTerminator('\n');
		while(s.Size())
		{
			CryString t;
			t.SetTerminator('\n');
			t.WriteTE((CryString *)s.GetAddress(),500);
			s.Delete(0,t.Size()+1);
			s.SeekFromStart(0);
			t.Trim();
			if (t=="")
				t = " ";
			TreeView1->Items->AddChild(body,t.AsPChar());
		}
	}
/*
CryContainer::Iterator *I = ANewClass->_CreateIterator();
	if (ANewClass->GotoFirst(I))
		do
		{
			CryString PropertyType;
			CryString PrimitiveType;
			CryString EleName;
			CryString Declare;
			CryString DeclareComment;
			CryString Body;
			CryString BodyComment;
			CryString Name;
			CryString _Value;
			CryString IsProperty;
			CryString Count;
			CryString CountDefineName;
			CryString BaseClass;
			CryString PropertyName;
			int iCount;

			_Ele *Ele = (_Ele *)ANewClass->Get(I);

			if (Ele)
			{
				const CryString *sPropertyType;
				// Inherited,Local,ClassPtr,ClassLocal,ClassPtrProp,ClassLocalProp
				InheriteName.Clear();
				/// When building a class these aspects of it are analyised
				Ele->GetProperty("PropertyType",PropertyType);
				Ele->GetProperty("PrimitiveType",PrimitiveType);
				Ele->GetProperty("Name",EleName);
				Ele->GetProperty("Declare",Declare);
				Ele->GetProperty("DeclareComment",DeclareComment);
				Ele->GetProperty("Body",Body);
				Ele->GetProperty("BodyComment",BodyComment);
				Ele->GetProperty("Name",Name);
				Ele->GetProperty("_Value",_Value);
				Ele->GetProperty("IsProperty",IsProperty);
				Ele->GetProperty("Count",Count);
				Count.scanf("%d",&iCount);
				Ele->GetProperty("CountDefineName",CountDefineName);
				Ele->GetProperty("BaseClass",BaseClass);
				Ele->GetProperty("PropertyName",PropertyName);


				if (Ele->CmpProperty("PropertyType","Inherited")==0)
					InheriteName.printf("%s %s",PropertyType.AsPChar(),ANewClass->GetInheritType());
				else
					InheriteName.printf("%s::%s (Name)",PropertyType.AsPChar(),EleName.AsPChar());
				if (iCount>1)
					InheriteName.printf("[%s]",CountDefineName.AsPChar());
				InheriteName.printf("Primitive Type %s",PrimitiveType.AsPChar());
				name = TreeView1->findItem(PropertyType.AsPChar());//ANewClass->GetName());//InheriteName.AsPChar());
				if (name == 0)
					name = TreeView1->Items->appendItem(MainTree,InheriteName.AsPChar());

				name = TreeView1->Items->appendItem(name,CryString("Property:%s",EleName.AsPChar()).AsPChar());
				//if ((LastPT != _PropertyType) || (_PropertyType!=ClassBuilder::Inherited))
				pt = TreeView1->Items->appendItem(name,CryString("PropertyType %s", PropertyType.AsPChar()).AsPChar());

				if ((PropertyType == "Property") ||
						(PropertyType == "Primitive"))
					value = TreeView1->Items->appendItem(name,CryString("Value %s",_Value.AsPChar()).AsPChar());
				if (PropertyType != "ClassPtr")
				{
					body = AddStrings(name,DeclareComment,"Header Comment:");
					body = AddStrings(name,Declare,"Header:");
					body = AddStrings(name,BodyComment,"Body Comment:");
					body = AddStrings(name,Body,"Body:");
					CryString s;
					s.printf("Name = %s, Default value = %s",EleName.AsPChar(), _Value.AsPChar());
					body = AddStrings(name,s,"Misc:");
				}
				if (iCount>1)
				{
					CountDefineName.printf("(defined as %d)",iCount);
					TreeView1->Items->appendItem(name,CountDefineName.AsPChar());
				}

			}
			else
			{
				return false;
			}
		}
		while(ANewClass->I_GotoNext());
									  */
	return;
	}
void __fastcall TClassBuilderFrm::AddVariableClick(TObject *Sender)
{
char Type[200];
char Name[200];

	strncpy(Name,VariableName->Edit1->Text.c_str(),199);

bool AddingClass = (RadioGroup1->ItemIndex==0);
	if (AddingClass)
		strncpy(Type,ClassType2CB->Items->Strings[ClassType2CB->ItemIndex].c_str(),199);
	else
		strncpy(Type,VariableTypeCB->Items->Strings[VariableTypeCB->ItemIndex].c_str(),199);


	try
	{

		if (AddingClass) 
			ANewClass->AddClassInstance(
				Type,
				Name,
				ArrayCount->Position,
				PropertyCB->Checked,
				TypePointer->Checked,
				PointerToArray->Checked,
				DefaultValue->CheckBox1->Checked,
				DefaultValue->CheckBox1->Checked ? DefaultValue->Edit1->Text.c_str() : "");
		else
			ANewClass->AddPrimInstance(Type,Name,
				Name,ArrayCount->Position,
				PropertyCB->Checked,
				TypePointer->Checked,
				PointerToArray->Checked );

			RefreshView();
			RemoveVariable->Enabled = true;
			PropertyCount++;
			VariableNameEditedYet = false;
			UpdateStatus(Sender);
	}
	catch(CryException &e)
	{
		ShowMessage((const char *)CryString("Adding Class/Variable Instance Error %s",(const char *)e));
	}
	return;
}
//---------------------------------------------------------------------------

void __fastcall TClassBuilderFrm::UpdateStatus(TObject *Sender)
{
	CryString s;
	if (RadioGroup1->ItemIndex==0)// adding a class
		s.printf("My%s%d",ClassType2CB->Items->Strings[ClassType2CB->ItemIndex].c_str(),PropertyCount);
	else
		s.printf("My%s%d",VariableTypeCB->Items->Strings[VariableTypeCB->ItemIndex].c_str(),PropertyCount);
	if (!VariableNameEditedYet) {
		VariableName->Edit1->Text = (const char *)s;
	}

	CryString Comment = "// ";
	CryString Type = VariableTypeCB->Items->Strings[VariableTypeCB->ItemIndex].c_str();
	CryString Name = VariableName->Edit1->Text.c_str();
	if (RadioGroup1->ItemIndex==0)
		Type = 	ClassType2CB->Items->Strings[ClassType2CB->ItemIndex].c_str();
	CryString Array = "";
	if (ArrayCount->Position>1)
		Array.printf("[%s_LEN]",(const char *) Name);
	CryString Pointers = (TypePointer->Checked ? "*" : "");
	Pointers += PointerToArray->Checked ? "*" : "";
	if (TypePointer->Checked)
		Comment += " Pointer to ";
	if (Array!="")
		Comment += " an array of ";
	if (PointerToArray->Checked)
		Comment += "pointer of ";
	Comment += Type;
	if (PropertyCB->Checked)
	{
    	Comment+= " coded as a property";    
	}
	CryString r;
	r.printf("%s %s%s%s;   %s",(const char *)Type,
										(const char *) Pointers,
										(const char *)Name,
										(const char *) Array,
										(const char *) Comment);
	StatusBar1->Panels->Items[0]->Text =  (const char *)r;
}
//---------------------------------------------------------------------------


void __fastcall TClassBuilderFrm::VariableNameEdit1KeyDown(TObject *Sender,
	  WORD &Key, TShiftState Shift)
{
	VariableNameEditedYet = true;
}
//---------------------------------------------------------------------------

void __fastcall TClassBuilderFrm::MenuItem_NewClick(TObject *Sender)
{
	delete ANewClass;
	ANewClass = new ClassBuilder();
	PropertyCount = 0;
	RefreshView();
	UpdateStatus(Sender);
	AddVariables->Enabled = false;
	RemoveVariable->Enabled = false;
	return;
}
//---------------------------------------------------------------------------

void __fastcall TClassBuilderFrm::MenuItem_OpenClick(TObject *Sender)
{
	try
	{
		OpenDialog1->Title = "Open ClassBuilder file";
		CryString filename = ANewClass->GetFilename();
		filename += ".xml";
		OpenDialog1->FileName =(const char *) filename;
		if(OpenDialog1->Execute())
		{
			CryString file;
			file = OpenDialog1->FileName.c_str();
			CryString cfile(file);
			cfile.Replace(".xml","");
			delete ANewClass;
			ANewClass = new ClassBuilder();

			ANewClass->SetFilename(cfile.AsPChar());
			ANewClass->LoadSource();
			AddVariables->Enabled = true;
			RemoveVariable->Enabled = true;
			RefreshView();
			UpdateStatus(Sender);
		}
	}
	catch(CryException &e)
	{
	CryString s = "Error on open: ";
		s += e;
		ShowMessage((const char *)s);
	}
}
//---------------------------------------------------------------------------



void __fastcall TClassBuilderFrm::MenuItem_SaveClick(TObject *Sender)
{
	if (ANewClass->GetName()=="")
	{
		ShowMessage("Error baseclass has no name");
		return;
	}
	if (ANewClass->GetFilename()=="")
	{
		MenuItem_SaveAsClick(Sender);
		return;
	}
	ANewClass->SaveSource();
	RefreshView();
	UpdateStatus(Sender);
	return;
}
//---------------------------------------------------------------------------

void __fastcall TClassBuilderFrm::MenuItem_SaveAsClick(TObject *Sender)
{
   try
	{
		SaveDialog1->Title = "Save source,header and xml";
		SaveDialog1->FileName = (const char *)ANewClass->GetFilename();

		if(!SaveDialog1->Execute())
			return;
		//setCurrentPattern(savedialog.getCurrentPattern());
		CryString file; file = SaveDialog1->FileName.c_str();
		if (access(file,0)==0) {
			if (Application->MessageBox("Overwrite Document?","File Exists",MB_YESNO)==IDNO)
				return;
		}
		if (file[-3]=='.')
			file.Delete(-3,4);
		ANewClass->SetFilename((const char *)file);
		ANewClass->SaveSource();
		RefreshView();
	}
	catch(CryException &e)
	{
	CryString a;
		a = "Error Saving"; a += e;
		ShowMessage((const char *) a);
	}
	return;
}
//---------------------------------------------------------------------------



void __fastcall TClassBuilderFrm::Exit1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TClassBuilderFrm::FormDestroy(TObject *Sender)
{
	delete ANewClass;
}
//---------------------------------------------------------------------------

void __fastcall TClassBuilderFrm::Button1Click(TObject *Sender)
{
	SetBaseClassBTClick(Sender);
		ANewClass->SetFilename("C:\\Documents and Settings\\Chris\\My Documents\\test.xml");
		ANewClass->SaveSource();
		RefreshView();

}
//---------------------------------------------------------------------------

