//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitTestForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#include "Dialogs.hpp"
#include "ClassString.h"
#include "CryBackProp.h"
#include "CryFuzzy.h"
#include "HugeInt.h"
#include "CrySet.h"

using namespace Dialogs;
using namespace Crystal;
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}


bool/* __cdecl */FormCallBack(bool Verbose,const char *Result,bool fail)
{
CryString s;
	s.printf("%s %s",Result,fail ? "Fail" : "Pass");
	s.Replace("\n","");
	Form1->Memo1->Lines->Add(s.AsPChar());
	Application->ProcessMessages();
  if (fail)
  {
  AnsiString s = "Continue?";
	return MessageDlg(s,mtConfirmation,mbYesNo,0)==mrYes;
  }
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
Memo1->Clear();
try
{
CryString a;
	a.Test(true,a,FormCallBack);
}
catch(CryException &E)
{
CryString s;
	s ="Exception Caught: ";
	s += E;
	ShowMessage(s.AsPChar());
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
Memo1->Clear();
try
{
CryBPNetContainer bp;
#ifdef VALIDATING
	bp.Test(true,bp,FormCallBack);
#endif

}
catch(CryException &E)
{
CryString s;
	s ="Exception Caught: ";
	s += E;
	ShowMessage(s.AsPChar());
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
Memo1->Clear();
try
{
CryObject o;
#ifdef VALIDATING
	o.Test(true,o,FormCallBack);
#endif

}
catch(CryException &E)
{
CryString s;
	s ="Exception Caught: ";
	s += E;
	ShowMessage(s.AsPChar());
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button4Click(TObject *Sender)
{
Memo1->Clear();
try
{
CryTArray<int> a;
#ifdef VALIDATING
	for(int i=10;i<40;i++)
		a.SetValue(i,i);
	a.SetValue(5,20);
	a.SetValue(0,30);
	a.Test(true,a,FormCallBack);
#endif

}
catch(CryException &E)
{
CryString s;
	s ="Exception Caught: ";
	s += E;
	ShowMessage(s.AsPChar());
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button5Click(TObject *Sender)
{
Memo1->Clear();
try
{
CrySet a;
#ifdef VALIDATING
	for(int i=10;i<40;i++)
		a.SetValue(i,i);
	a.SetValue(5,20);
	a.SetValue(0,30);
	a.Test(true,a,FormCallBack);
#endif

}
catch(CryException &E)
{
CryString s;
	s ="Exception Caught: ";
	s += E;
	ShowMessage(s.AsPChar());
}
}
//---------------------------------------------------------------------------

