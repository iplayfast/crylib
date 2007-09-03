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
#include "ClassXML.h"
#include "HugeDouble.h"

using namespace Dialogs;
using namespace Crystal;
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
		: TForm(Owner)
{}


bool/* __cdecl */FormCallBack(bool Verbose,const char *Result,bool fail)
{
{
	Crystal::String s;
	s.printf("%s %s",Result,fail ? "Fail" : "Pass");
	s.Replace("\n","");
	Form1->Memo1->Lines->Add(s.AsPChar());
}
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
				TArray<float> a;
				for (int i=10;i<40;i++)
					a.SetValue(i,i * 1.5);
				a.SetValue(5,20.2);
				a.SetValue(0,30.2);
				Fail = a.Test(true,a,FormCallBack);
			}
			break;
		case 3:
			{
				Crystal::String a;
				Fail = a.Test(true,a,FormCallBack);
			}
			break;
		case 4:
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
		case 5:
			{
			List ml;
			ml.IsA("");	// initialize ObjectType debug data

			Crystal::String NotOwned = "Item Not Owned";
			/* TODO -cCodegear bug :
This should be valid but CodeGear's compiler can't handle it. 
Uncomment it after the compiler is fixed. 
				ml.AddOwned((EmptyObject *)(new int(42)),sizeof(int *));
				*/
				ml.AddOwned(new Crystal::String("Item 1"));
				ml.AddOwned(new Crystal::String("Item 2"));
				ml.AddOwned(new Crystal::String("Item 3"));
				ml.Add(&NotOwned);
				ml.FirstNode();
				ml.Test(true,ml,FormCallBack);
				List a;
				Fail = a.Test(true,a,FormCallBack);
			}
			break;
		case 6:
			{
				Strategy a;
				Fail = a.Test(true,a,FormCallBack);
			}
			break;
		case 7:
			{
				Observer a;
				Fail = a.Test(true,a,FormCallBack);
			}
			break;
		case 8:
			{
			{
			BackPropagateNetwork b;
				Fail = b.Test(true,b,FormCallBack);
{
XML x;
	x.LoadFrom(b);
Object *o = x.CreateObjectFromNode(&b);      // loosing memory here!
delete o;
}
				
			}

//				BPNetContainer bp;
//				Fail = bp.Test(true,bp,FormCallBack);
			}
			break;
		case 9:
			{
				BitArray a;
				Fail = a.Test(true,a,FormCallBack);
			}
			break;
		case 10:
			{
				Fuzzy f;
				Fail = f.Test(true,f,FormCallBack);
			}
			break;
		case 11:
			{
			HugeInt d;
				Fail = d.Test(true,d,FormCallBack);
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



