//---------------------------------------------------------------------------

#ifndef labelEditfrmH
#define labelEditfrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TLabelEdit : public TFrame
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *Edit1;
private:	// User declarations
public:		// User declarations
	__fastcall TLabelEdit(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLabelEdit *LabelEdit;
//---------------------------------------------------------------------------
#endif
