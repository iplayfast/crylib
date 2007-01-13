//---------------------------------------------------------------------------

#ifndef ClassBuilderUnitH
#define ClassBuilderUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include "CheckLabel.h"
#include "labelEditfrm.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "ClassBuilder.h"
#include <Dialogs.hpp>
//class ClassBuilder;
class TClassBuilderFrm : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *New1;
	TMenuItem *Open1;
	TMenuItem *Save1;
	TMenuItem *SaveAs1;
	TMenuItem *N2;
	TMenuItem *Print1;
	TMenuItem *PrintSetup1;
	TMenuItem *N1;
	TMenuItem *Exit1;
	TMenuItem *testUseDebugtoverifyresults1;
	TMenuItem *testcode11;
	TMenuItem *testcode21;
	TPageControl *PageControl1;
	TTabSheet *SetBaseClass;
	TButton *SetBaseClassBT;
	TCheckBox *IncludeStubs;
	TLabelEdit *NameED;
	TComboBox *ClassTypeCB;
	TTabSheet *AddVariables;
	TLabel *ArrayAmountLb;
	TLabelEdit *VariableName;
	TRadioGroup *RadioGroup1;
	TCheckBox *PropertyCB;
	TCheckBox *TypePointer;
	TCheckBox *PointerToArray;
	TComboBox *ClassType2CB;
	TTrackBar *ArrayCount;
	TTabSheet *RemoveVariable;
	TLabel *Label1;
	TComboBox *RemoveVariableCB;
	TTreeView *TreeView1;
	TButton *AddVariable;
	TStatusBar *StatusBar1;
	TComboBox *VariableTypeCB;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TCheckedLabel *DefaultValue;
	TButton *Button1;
	TComboBox *TemplateType;
	void __fastcall SetBaseClassBTClick(TObject *Sender);
	void __fastcall ArrayCountChange(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall AddVariableClick(TObject *Sender);
	void __fastcall UpdateStatus(TObject *Sender);
	void __fastcall VariableNameEdit1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall MenuItem_NewClick(TObject *Sender);
	void __fastcall MenuItem_OpenClick(TObject *Sender);
	void __fastcall MenuItem_SaveClick(TObject *Sender);
	void __fastcall MenuItem_SaveAsClick(TObject *Sender);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall ClassTypeCBChange(TObject *Sender);
private:	// User declarations
Crystal::ClassBuilder *ANewClass;
int PropertyCount;
bool VariableNameEditedYet;
	void __fastcall RefreshView(void);
public:		// User declarations
	__fastcall TClassBuilderFrm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TClassBuilderFrm *ClassBuilderFrm;
//---------------------------------------------------------------------------
#endif
