//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CheckLabel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCheckedLabel *CheckedLabel;
//---------------------------------------------------------------------------
__fastcall TCheckedLabel::TCheckedLabel(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCheckedLabel::CheckBox1Click(TObject *Sender)
{
	Edit1->Enabled = CheckBox1->Checked;	
}
//---------------------------------------------------------------------------
