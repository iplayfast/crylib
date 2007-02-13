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
#include <ImgList.hpp>
#include <ValEdit.hpp>
using namespace Crystal;
//---------------------------------------------------------------------------
#define MAXVALUE 1000
class ImageInfo : public TObject
{
public:
ImageInfo *Next;
enum LogicType { In,Out,Not,Or,Xor,And,Fuzzy };
LogicType Type;	// type = Not,Or,XOR,And
bool DoneIn1;
bool DoneIn2;

int x;
int y;
int ImageID;
int In1Y;	// graphical positions that lines connect to only,
int In2Y;

int In1V;
int In2V;
int OutV;
AnsiString Name;
AnsiString Description;
	void SetImageID()
	{
		switch(Type)
		{
			case In: ImageID = 6; break;
			case Out: ImageID = 5;  break;
			case Not: ImageID = 0; break;
			case Or: ImageID = 1; break;
			case Xor: ImageID = 2; break;
			case And: ImageID = 3; break;
			case Fuzzy: ImageID = 4; break;
		};
	};
	bool Selected(int X,int Y);
	bool MouseAtIn(int X,int Y);
	bool MouseAtOut(int X,int Y);
	bool MouseAtIn1(int X,int Y);
	bool MouseAtIn2(int X,int Y);
	virtual void GetIn1Value(AnsiString &Result) { Result = In1V; }
	virtual void GetIn2Value(AnsiString &Result) { Result = In2V; }
	virtual void GetOutValue(AnsiString &Result) { Result = OutV; }
	virtual void SetIn1Value(int i) { In1V = i; };
	virtual void SetIn2Value(int i) { In2V = i; };
	virtual void SetOutValue(int i) { OutV = i; };
	virtual void Calculate() {};
};

class Connection
{
Connection() {} // don't allow
public:
	Connection(ImageInfo *F,ImageInfo *T) { Next = 0; From = F; To = T; ToIn = 0; }
Connection *Next;
ImageInfo *From;
ImageInfo *To;
bool Done;
int ToIn;	// either 0 1 or 2	0 is center, 1 it top, 2 is bottom
};
class LogicList : public ImageInfo
{
LogicList() {}
public:
	void Calculate()
	{
		switch(Type)
		{
		case In: OutV = In2V = In1V; break;
		case Out: OutV = In2V = In1V;  break;
		case Not: OutV = In2V - In1V; break;
		case Or: ImageID = 1; break;
		case Xor: ImageID = 2; break;
		case And: ImageID = 3; break;
		}
	};
	LogicList(int t,AnsiString _Description)
	{
		Type = (ImageInfo::LogicType) t;
		Name = _Description;
		switch(t)
		{
		case In: ImageID = 6; break;
		case Out: ImageID = 5;  break;
		case Not: ImageID = 0; break;
		case Or: ImageID = 1; break;
		case Xor: ImageID = 2; break;
		case And: ImageID = 3; break;
		}
		x = -1;
		y = 10;
		if (t<3)
		{
		In1Y = 32;
		In2Y = 32;
		}
		else
		{
			In1Y = 21;
			In2Y = 42;
		}
	}
};
class FuzzyList : public ImageInfo
{
FuzzyList() {} // can't do this
public:
ClassFuzzy Fuzzy;
int Min;
int Max;
	virtual void Calculate() {OutV = Fuzzy.Value(In1V); };
	FuzzyList(AnsiString _Name,AnsiString _Description,int _Min,int _Max)
	{
		Type = ImageInfo::Fuzzy;
		Name = _Name;
		Description = _Description;
		Next = 0;
		Min = _Min;
		Max = _Max;
		x = 10;
		y = 10;
		In1Y = 32;
		In2Y = 32;
		ImageID = 4;
	}
};
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
	TPanel *Panel2;
	TStringGrid *grid;
	TPanel *t;
	TMenuItem *File1;
	TMenuItem *Exit1;
	TMenuItem *N2;
	TMenuItem *SaveAs1;
	TMenuItem *Save1;
	TMenuItem *Open1;
	TMenuItem *New1;
	TOpenTextFileDialog *OpenTextFileDialog1;
	TSaveTextFileDialog *SaveTextFileDialog1;
	TButton *Button1;
	TChart *Chart1;
	TRadioGroup *RadioGroup1;
	TLineSeries *Series1;
	TLineSeries *Series2;
	TLineSeries *Series3;
	TCheckBox *CheckBox3d;
	TUpDown *UpDown5;
	TMenuItem *Complexity1;
	TMenuItem *OneFuzzyLogic1;
	TMenuItem *TwoFuzzyLogics1;
	TMenuItem *FuzzyLogicOperations1;
	TButton *Button7;
	TButton *Button8;
	TImageList *ImageList1;
	TComboBox *ComboBox1;
	TScrollBox *ScrollBox1;
	TStatusBar *StatusBar1;
	TValueListEditor *vl;
	TPaintBox *Image1;
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
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Image11MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Image11MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall Image1Paint(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Image1DblClick(TObject *Sender);
private:	// User declarations
	void GridClear();
	void UpdateLogic();
	void UpdateComboBox();
ImageInfo *From;
Connection *Connections;

int LastMouseX;
int LastMouseY;
bool Moving;
bool DoubleClick;
public:		// User declarations
ImageInfo *Head;

ClassFuzzy MyFuzzy[];
TTrackBar *Active;	// used to determine which the user is currently moving
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
