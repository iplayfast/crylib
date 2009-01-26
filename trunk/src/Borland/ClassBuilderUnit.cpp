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
#include "ClassObject.h"
#include "ClassString.h"
#include "ClassException.h"
#include "ClassBuilder.h"
#include "ClassFileStream.h"
#include "BitArray.h"
#include "ClassXML.h"
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

		AnsiString as = ClassTypeCB->Items->Strings[ClassTypeCB->ItemIndex];
		Crystal::String a(as.c_str());

		delete ANewClass;
		ANewClass = new ClassBuilder();
		as = TemplateType->Text;
		const PropertyParser pp("TemplateType");
		ANewClass->SetProperty(pp,as.c_str());
		as = NameED->Edit1->Text;
		ANewClass->SetBaseClass(a,IncludeStubs->Checked,true,as.c_str());
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
	catch(Crystal::Exception &e)
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
	ClassTypeCB->AddItem(CObject,0);
	ClassTypeCB->Text = CObject;

	ClassTypeCB->AddItem(CContainer,0);
	ClassTypeCB->AddItem(CStream,0);
	ClassTypeCB->AddItem(CMemStream,0);
	ClassTypeCB->AddItem(CFileStream,0);
	ClassTypeCB->AddItem(CString,0);
	ClassTypeCB->AddItem(CProperty,0);
	ClassTypeCB->AddItem(CList,0);
	ClassTypeCB->AddItem(CPropertyList,0);
	// From Array
	ClassTypeCB->AddItem(CSimpleArray,0);
	ClassTypeCB->AddItem(CArray,0);
	ClassTypeCB->AddItem(CTArray,0);
	ClassTypeCB->AddItem(CDoubleArray,0);
	// From BitArray
	ClassTypeCB->AddItem(CBitArray,0);

	// from Xml
	ClassTypeCB->AddItem(CXMLNode,0);
	ClassTypeCB->AddItem(CXML,0);
	// From Fuzzy
	ClassTypeCB->AddItem(CFuzzy,0);
	// From BackProp
//	ClassTypeCB->AddItem("BackPropagateLayer",0);
	ClassTypeCB->AddItem(CBackPropagateNetwork,0);

	ClassTypeCB->AddItem(CFunctionDef,0);
	ClassTypeCB->AddItem(CFunctionDefList,0);
	// from HugeINt
	ClassTypeCB->AddItem(CHugeInt,0);
	// from Pattern
	ClassTypeCB->AddItem(CStrategy,0);
	ClassTypeCB->AddItem(CStrategyHolder,0);
	ClassTypeCB->AddItem(CStrategyHolderSender,0);
	ClassTypeCB->AddItem(CObserver,0);
	ClassTypeCB->AddItem(CObservable,0);
	ClassTypeCB->AddItem(CDecorator,0);
	ClassTypeCB->AddItem(CFactory,0);
	ClassTypeCB->AddItem(COFactory,0);
	ClassTypeCB->AddItem(CCommandHolder,0);
	ClassTypeCB->AddItem(CCommandHolderSender,0);
	ClassTypeCB->AddItem(CCompositeIterator,0);
	ClassTypeCB->AddItem(CState,0);

	ClassTypeCB->ItemIndex = 0;

	ClassType2CB->Items = ClassTypeCB->Items;
	ClassType2CB->ItemIndex = 0;

	VariableTypeCB->AddItem("bool",0);
	VariableTypeCB->AddItem("char",0);
	VariableTypeCB->AddItem("int",0);
	VariableTypeCB->AddItem("float",0);
	VariableTypeCB->AddItem("double",0);

	VariableTypeCB->ItemIndex = 0;
	ClassTypeCBChange(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TClassBuilderFrm::RefreshView(void)
{
	Crystal::String SourceHeader,SourceBody;
	SourceHeader.SetLength(10000);
	ANewClass->SaveHeaderBody(SourceHeader,SourceBody);

	TTreeNode *pt=0,*body,*name,*type,*value,*count,*bc,*pn,*MainTree,*SourceTree;
	//    const String *Body,*Name,*Type,*BaseClass,*pPropertyName,*Value;
	Crystal::String InheriteName;
	Crystal::String CountDefineName;
	//    int *iPtype,Count;
	//    ClassBuilder::PropertyType _PropertyType,LastPT = ClassBuilder::Unknown;

	TreeView1->Items->Clear();
	MainTree = name = TreeView1->Items->AddFirst(0,ANewClass->GetName());
	{
		SourceTree = TreeView1->Items->AddChild(name,"Complete Source Code:");
		body = TreeView1->Items->AddChild(SourceTree,"Header:");
		Crystal::String s;
		s = SourceHeader;
		s.Replace("\t","   ");
		s.SetTerminator('\n');
		while(s.Size())
		{
			Crystal::String t;
			t.SetTerminator('\n');
			t.WriteTE((Crystal::String *)s.GetAddress(),500);
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
			Crystal::String t;
			t.SetTerminator('\n');
			t.WriteTE((Crystal::String *)s.GetAddress(),500);
			s.Delete(0,t.Size()+1);
			s.SeekFromStart(0);
			t.Trim();
			if (t=="")
				t = " ";
			TreeView1->Items->AddChild(body,t.AsPChar());
		}
	}
	TTreeNode *n;
		n = TreeView1->Items->GetFirstNode();
		n->Expand(true);
/*
Container::Iterator *I = ANewClass->_CreateIterator();
	if (ANewClass->GotoFirst(I))
		do
		{
			String PropertyType;
			String PrimitiveType;
			String EleName;
			String Declare;
			String DeclareComment;
			String Body;
			String BodyComment;
			String Name;
			String _Value;
			String IsProperty;
			String Count;
			String CountDefineName;
			String BaseClass;
			String PropertyName;
			int iCount;

			_Ele *Ele = (_Ele *)ANewClass->Get(I);

			if (Ele)
			{
				const String *sPropertyType;
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

				name = TreeView1->Items->appendItem(name,String("Property:%s",EleName.AsPChar()).AsPChar());
				//if ((LastPT != _PropertyType) || (_PropertyType!=ClassBuilder::Inherited))
				pt = TreeView1->Items->appendItem(name,String("PropertyType %s", PropertyType.AsPChar()).AsPChar());

				if ((PropertyType == "Property") ||
						(PropertyType == "Primitive"))
					value = TreeView1->Items->appendItem(name,String("Value %s",_Value.AsPChar()).AsPChar());
				if (PropertyType != "ClassPtr")
				{
					body = AddStrings(name,DeclareComment,"Header Comment:");
					body = AddStrings(name,Declare,"Header:");
					body = AddStrings(name,BodyComment,"Body Comment:");
					body = AddStrings(name,Body,"Body:");
					String s;
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
AnsiString as = VariableName->Edit1->Text;
	strncpy(Name,as.c_str(),199);

bool AddingClass = (RadioGroup1->ItemIndex==0);
	if (AddingClass)
		as = ClassType2CB->Items->Strings[ClassType2CB->ItemIndex];
	else
		as = VariableTypeCB->Items->Strings[VariableTypeCB->ItemIndex];
	strncpy(Type,as.c_str(),199);


	try
	{

		if (AddingClass)
		{
			as = DefaultValue->Edit1->Text;

			ANewClass->AddClassInstance(
				Type,
				Name,
				ArrayCount->Position,
				PropertyCB->Checked,
				TypePointer->Checked,
				PointerToArray->Checked,
				DefaultValue->CheckBox1->Checked,
				DefaultValue->CheckBox1->Checked ? as.c_str() : "");
		}
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
	catch(Crystal::Exception &e)
	{
		ShowMessage((const char *)Crystal::String("Adding Class/Variable Instance Error %s",(const char *)e));
	}
	return;
}
//---------------------------------------------------------------------------

void __fastcall TClassBuilderFrm::UpdateStatus(TObject *Sender)
{
	Crystal::String s;
	if (RadioGroup1->ItemIndex==0)// adding a class
		s.printf("My%s%d",ClassType2CB->Items->Strings[ClassType2CB->ItemIndex].c_str(),PropertyCount);
	else
		s.printf("My%s%d",VariableTypeCB->Items->Strings[VariableTypeCB->ItemIndex].c_str(),PropertyCount);
	if (!VariableNameEditedYet) {
		VariableName->Edit1->Text = (const char *)s;
	}

	Crystal::String Comment = "// ";
	AnsiString as;
	as = VariableTypeCB->Items->Strings[VariableTypeCB->ItemIndex];
	Crystal::String Type = as.c_str();
	as = VariableName->Edit1->Text;
	Crystal::String Name = as.c_str();
	if (RadioGroup1->ItemIndex==0)
	{
		as = ClassType2CB->Items->Strings[ClassType2CB->ItemIndex];
		Type = 	as.c_str();
	}
	Crystal::String Array = "";
	if (ArrayCount->Position>1)
		Array.printf("[%s_LEN]",(const char *) Name);
	Crystal::String Pointers = (TypePointer->Checked ? "*" : "");
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
	Crystal::String r;
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
		Crystal::String filename = ANewClass->GetFilename();
		filename += ".xml";
		OpenDialog1->FileName =(const char *) filename;
		if(OpenDialog1->Execute())
		{
			Crystal::String file;
			AnsiString as = OpenDialog1->FileName;
			file = as.c_str();
			Crystal::String cfile(file);
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
	catch(Crystal::Exception &e)
	{
	Crystal::String s = "Error on open: ";
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
		Crystal::String file;
		AnsiString as = SaveDialog1->FileName;
		file = as.c_str();
		if (access(file,0)==0) {
			if (Application->MessageBox(L"Overwrite Document?",L"File Exists",MB_YESNO)==IDNO)
				return;
		}
		if (file[-3]=='.')
			file.Delete(-3,4);
		ANewClass->SetFilename((const char *)file);
		ANewClass->SaveSource();
		RefreshView();
	}
	catch(Crystal::Exception &e)
	{
	Crystal::String a;
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

void __fastcall TClassBuilderFrm::ClassTypeCBChange(TObject *Sender)
{
	TemplateType->Visible = (ClassTypeCB->Text == CTArray);	
}
//---------------------------------------------------------------------------

