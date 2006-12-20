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
#include "CryPattern.h"
#include "BitArray.h"

using namespace Dialogs;
using namespace Crystal;
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
		: TForm(Owner)
{}


bool/* __cdecl */FormCallBack(bool Verbose,const char *Result,bool fail)
{
	Crystal::String s;
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


void __fastcall TForm1::CheckListBox1Click(TObject *Sender)
{
int i = CheckListBox1->ItemIndex;
bool Fail=true;
		CheckListBox1->Checked[i]= false;

	Memo1->Clear();
	try
	{

		switch(i)
		{
		case 0:
			{
			Object o;
					Fail = o.Test(true,o,FormCallBack);
			}
			break;
		case 1:
			{
				TArray<int> a;
				for (int i=10;i<40;i++)
					a.SetValue(i,i);
				a.SetValue(5,20);
				a.SetValue(0,30);
				Fail = a.Test(true,a,FormCallBack);
			}
			break;
		case 2:
			{
				Crystal::String a;
				Fail = a.Test(true,a,FormCallBack);
			}
			break;
		case 3:
			{
			CrySet a;
			a.Add(5);
			a.Add(10);
			a.Add(8);
			a.Add(7);
			a.Add(7);
			for (int i=10;i<40;i++)
				a.SetValue(i,i);
			a.SetValue(5,20);
			a.SetValue(0,30);
			Fail = a.Test(true,a,FormCallBack);
			}
			break;
		case 4:
			{
				List a;
				Fail = a.Test(true,a,FormCallBack);
			}
			break;
		case 5:
			{
				Strategy a;
				Fail = a.Test(true,a,FormCallBack);
			}
			break;
		case 6:
			{
				Observer a;
				Fail = a.Test(true,a,FormCallBack);
			}
			break;
		case 7:
			{
				BPNetContainer bp;
				Fail = bp.Test(true,bp,FormCallBack);
			}
			break;
		case 8:
			{
				BitArray a;
				Fail = a.Test(true,a,FormCallBack);
			}
		}
		CheckListBox1->Checked[i]= Fail;
	}
	catch (Crystal::Exception &E)
	{
		Crystal::String s;
		s ="Exception Caught: ";
		s += E;
		ShowMessage(s.AsPChar());
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{
	for(int i=0;i<CheckListBox1->Items->Count;i++)
	{
		CheckListBox1->ItemIndex = i;
		CheckListBox1Click(Sender);
	}
}
//---------------------------------------------------------------------------



