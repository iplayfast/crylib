#pragma once

//---------------------------------------------------------------------------
#pragma hdrstop
#include "MainForm.h"
#include "MainForm.Designer.h"
//---------------------------------------------------------------------------

#ifdef NO_WX
#pragma package(smart_init)
#pragma link "wxBoxSzr"
#pragma link "wxBtn"
#pragma link "wxCoBox"
#pragma link "wxGridSzr"
#pragma link "wxOpenFileDlg"
#pragma resource "*.dfm"
#endif
#include "SimplePlotLine.h"

//---------------------------------------------------------------------------

TMainFrm::TMainFrm(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: TWxDialog(parent, id, title, position, size, style)
{
	//Function defined in MainForm.Designer.h
	InitializeComponent();
	Emotions->SetValue("Pleasure");
	Emotions->Append("Pleasure");
	Emotions->Append("Contentment");
	Emotions->Append("Sorrow");
	Emotions->Append("Concern");
	Emotions->Append("Love");
	Emotions->Append("Hate");
	Emotions->Append("Security");
	Emotions->Append("Fear");
	Emotions->Append("Seriousness");
	Emotions->Append("Frolic");
	Emotions->Append("Belonging");
	Emotions->Append("Solitude");
	Emotions->Append("Volition");
	Emotions->Append("Surrender");
	Emotions->Append("Rigor");
	Emotions->Append("Flimsiness");
	Emotions->Append("Anger");
	Emotions->Append("Leniency");
	Emotions->Append("Interest");
	Emotions->Append("Boredom");
	Emotions->Append("Pride");
	Emotions->Append("Shame");
	Emotions->Append("Superiourity");
	Emotions->Append("Inferiority");
	Emotions->Append("Adoration");
	Emotions->Append("Scorn");
	Emotions->Append("Wariness");
	Emotions->Append("Dreaminess");
	Emotions->Append("Surprise");
	Emotions->Append("Disgust");
	Emotions->Append("Desire");
}
//---------------------------------------------------------------------------

TMainFrm::~TMainFrm()
{
	//Function defined in MainForm.Designer.h
	DestroyComponent();
//	delete SimplePlotLine;
}
//---------------------------------------------------------------------------

void __fastcall TMainFrm::WxDialogClose(wxCloseEvent &Event)
{
	Destroy();
}
//---------------------------------------------------------------------------
void __fastcall TMainFrm::wxButton1Click(wxCommandEvent &Event)
{
TSimplePlotLineForm *SimplePlotLine = new TSimplePlotLineForm(this);
	SimplePlotLine->LoadEmotions(Emotions->GetStrings());
	SimplePlotLine->ShowModal();
	delete SimplePlotLine;
}
//---------------------------------------------------------------------------

void __fastcall TMainFrm::wxButton4Click(wxCommandEvent &Event)
{
 auto_ptr<TSimplePlotLineForm> SimplePlotLine( new TSimplePlotLineForm(this) );
try
{
//TSimplePlotLineForm *SimplePlotLine = new TSimplePlotLineForm(this);
			SimplePlotLine->ImportPlot();
//			wxArrayString Emotions;
//			Emotions.Add("Pleasure");
			SimplePlotLine->LoadEmotions(Emotions->GetStrings());
	SimplePlotLine->ShowModal();
//	delete SimplePlotLine;
}
catch(Exception &e)
{
const char *ch = e;
	wxMessageDialog dialog(this,_T(ch),_T("Error"),wxOK);
	dialog.ShowModal(); // commented out because of wxWidgets bug
}
}
//---------------------------------------------------------------------------

