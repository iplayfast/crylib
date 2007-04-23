//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include "TFuzzy.h"
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <Chart.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Series.hpp>
using namespace Crystal;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TPanel *Fuzzy1;
	TLabel *Label2;
	TLabel *Label1;
	TTrackBar *Input1;
	TTrackBar *Output1;
	TButton *Button3;
	TButton *Button2;
	TPanel *Logic;
	TPanel *Panel2;
	TStringGrid *grid;
	TLabel *Label3;
	TPanel *Fuzzy2;
	TLabel *Label4;
	TLabel *Label5;
	TTrackBar *Input2;
	TTrackBar *Output2;
	TButton *Button4;
	TButton *Button5;
	TLabel *Label6;
	TLabel *Label7;
	TPanel *Panel1;
	TProgressBar *FuzzyOut2;
	TProgressBar *LogicOut;
	TProgressBar *FuzzyOut1;
	TLabel *Label8;
	TLabel *Label9;
	TTrackBar *LogicIn;
	TMenuItem *File1;
	TMenuItem *Exit1;
	TMenuItem *N2;
	TMenuItem *SaveAs1;
	TMenuItem *Save1;
	TMenuItem *Open1;
	TMenuItem *New1;
	TOpenTextFileDialog *OpenTextFileDialog1;
	TSaveTextFileDialog *SaveTextFileDialog1;
	TUpDown *UpDown1;
	TLabel *MaxValue;
	TUpDown *UpDown2;
	TUpDown *UpDown3;
	TUpDown *UpDown4;
	TButton *Button1;
	TButton *Button6;
	TChart *Chart1;
	TRadioGroup *RadioGroup1;
	TLineSeries *Series1;
	TLineSeries *Series2;
	TLineSeries *Series3;
	TCheckBox *CheckBox3d;
	TUpDown *UpDown5;
	TBevel *Bevel1;
	TLabel *Label10;
	TMenuItem *Complexity1;
	TMenuItem *OneFuzzyLogic1;
	TMenuItem *TwoFuzzyLogics1;
	TMenuItem *FuzzyLogicOperations1;
	TComboBox *LogicSelection;
	TLabel *LogicInText;
	TEdit *Fuzzy1Name;
	TEdit *Fuzzy2Name;
	TMenuItem *Import1;
	TOpenTextFileDialog *ImportOpenTextFileDialog2;
	void __fastcall Refresh(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Input1Change(TObject *Sender);
	void __fastcall Output1Change(TObject *Sender);
	void __fastcall Input1Enter(TObject *Sender);
	void __fastcall Output1Enter(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Input2Exit(TObject *Sender);
	void __fastcall Input2Change(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Input1Exit(TObject *Sender);
	void __fastcall Input2Enter(TObject *Sender);
	void __fastcall Output2Change(TObject *Sender);
	void __fastcall LogicSelectionClick(TObject *Sender);
	void __fastcall LogicInChange(TObject *Sender);
	void __fastcall New1Click(TObject *Sender);
	void __fastcall Open1Click(TObject *Sender);
	void __fastcall Save1Click(TObject *Sender);
	void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall SaveAs1Click(TObject *Sender);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall RadioGroup1Click(TObject *Sender);
	void __fastcall CheckBox3dClick(TObject *Sender);
	void __fastcall UpDown5Click(TObject *Sender, TUDBtnType Button);
	void __fastcall OneFuzzyLogic1Click(TObject *Sender);
	void __fastcall TwoFuzzyLogics1Click(TObject *Sender);
	void __fastcall FuzzyLogicOperations1Click(TObject *Sender);
	void __fastcall Import1Click(TObject *Sender);
private:	// User declarations
	void GridClear();
	void UpdateLogic();
	float LogicFunctionResult(float In);
	void ParseLine(int WhichFuzzy,TEdit *Name,char *Buffer);
public:		// User declarations
ClassFuzzy MyFuzzy[2];
TTrackBar *Active;	// used to determine which the user is currently moving
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
