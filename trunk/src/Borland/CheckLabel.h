//---------------------------------------------------------------------------

#ifndef CheckLabelH
#define CheckLabelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TCheckedLabel : public TFrame
{
__published:	// IDE-managed Components
	TCheckBox *CheckBox1;
	TEdit *Edit1;
	void __fastcall CheckBox1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TCheckedLabel(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCheckedLabel *CheckedLabel;
//---------------------------------------------------------------------------
#endif
