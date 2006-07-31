//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "labelEditfrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLabelEdit *LabelEdit;
//---------------------------------------------------------------------------
__fastcall TLabelEdit::TLabelEdit(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
