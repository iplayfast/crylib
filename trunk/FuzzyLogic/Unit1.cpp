//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
#include "TFuzzy.h"


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}

float TForm1::LogicFunctionResult(float In)
{
	if (LogicSelection->ItemIndex==-1)
    	LogicSelection->ItemIndex = 0;
			switch(LogicSelection->ItemIndex)
			{
			case 0:	// and
				return MyFuzzy[0].And(&MyFuzzy[1],In);
				break;
			case 1: // or
				return MyFuzzy[0].Or(&MyFuzzy[1],In);
				break;
			case 2: // Not Fuzzy 1
				return MyFuzzy[0].Not(In,LogicIn->Max);
				break;
			case 3: // Not Fuzzy 2
				return  MyFuzzy[1].Not(In,LogicIn->Max);
				break;
			case 4: // Fuzzy1 Xor Fuzzy 2
				return MyFuzzy[0].XOr(&MyFuzzy[1],In);
				break;
			case 5: // Inhibit Fuzzy1 based on Fuzzy 2
				{
				float d = MyFuzzy[1].Value(In);
					if (d==0.0) d = 0.000001;	// avoid divide by zero
				return MyFuzzy[0].InhibitValue(In,(LogicIn->Max / d) / 100.0);
				}
			}
}


//---------------------------------------------------------------------------
void __fastcall TForm1::Refresh(TObject *Sender)
{
AnsiString a;
int MaxLength;

	if (MyFuzzy[0].Length()<MyFuzzy[1].Length())
		MaxLength = MyFuzzy[1].Length();
	else
		MaxLength = MyFuzzy[0].Length();
	grid->RowCount = 1+MaxLength;
int Complexity = 0;
	if (TwoFuzzyLogics1->Checked) Complexity =1;
	if (FuzzyLogicOperations1->Checked) Complexity = 2;
	switch(Complexity)
	{
	case 0:
	{
		grid->ColCount = 3;
		Fuzzy2->Visible = false;
		Logic->Visible = false;
	}
	break;
	case 1:
	{
		grid->ColCount = 5;
		Fuzzy2->Visible = true;
		Logic->Visible = false;
	}
	break;
	case 2:
	{
		grid->ColCount = 5;
		Fuzzy2->Visible = true;
		Logic->Visible = true;
	}
	break;
	}
	grid->Cells[1][0] = "Fuzzy 1 Input";
	grid->Cells[2][0] = "Fuzzy 1 Output";
	if (!OneFuzzyLogic1->Checked)
	{
		grid->Cells[3][0] = "Fuzzy 2 Input";
		grid->Cells[4][0] = "Fuzzy 2 Output";
	}
	for(int i=0;i<MaxLength;i++)
	{
		if (MyFuzzy[0].Length()>i)
		{
			a = MyFuzzy[0].IndexAt(i);
			grid->Cells[1][i+1] = a;
			a = MyFuzzy[0].ValueAt(i);
			grid->Cells[2][i+1] = a;
		}
		if (!OneFuzzyLogic1->Checked)
		{
			if (MyFuzzy[1].Length()>i)
			{
				a = MyFuzzy[1].IndexAt(i);
				grid->Cells[3][i+1] = a;
				a = MyFuzzy[1].ValueAt(i);
				grid->Cells[4][i+1] = a;
			}
		}
	}

	TChartSeries *s = (*Chart1->SeriesList)[0];
	s->Clear();
	for(int i=0;i<MyFuzzy[0].Length();i++)
	{
		s->AddXY(MyFuzzy[0].IndexAt(i),MyFuzzy[0].ValueAt(i));
	}
	s = (*Chart1->SeriesList)[1];
	s->Clear();
	if (!OneFuzzyLogic1->Checked)
	{
		for(int i=0;i<MyFuzzy[1].Length();i++)
		{
			s->AddXY(MyFuzzy[1].IndexAt(i),MyFuzzy[1].ValueAt(i));
		}
	}
	s = (*Chart1->SeriesList)[2];
	s->Clear();
	if (FuzzyLogicOperations1->Checked)
	{
		for(int i=0;i<LogicIn->Max;i+=UpDown5->Position)
		{
		float x,y;
			x = i;
			y = LogicFunctionResult(x);
			s->AddXY(x,y);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
float x,y;
	x = Input1->Position;
	y = Output1->Position;
	MyFuzzy[0].TFuzzyAddPoint(x,y);
	Refresh(Sender);
}
void TForm1::UpdateLogic()
{
	FuzzyOut1->Position = MyFuzzy[0].Value(LogicIn->Position);
	FuzzyOut2->Position = MyFuzzy[1].Value(LogicIn->Position);
	LogicOut->Position = LogicFunctionResult(LogicIn->Position);
	Label3->Caption = AnsiString("Fuzzy 1 Out ") + FuzzyOut1->Position;
	Label8->Caption = AnsiString("Fuzzy 2 Out ") + FuzzyOut2->Position;
	Label9->Caption = AnsiString("Logic Out ") + LogicOut->Position;
	Refresh(0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Input1Change(TObject *Sender)
{
	AnsiString sender = Sender->ClassName();
	Label2->Caption = AnsiString("Input ") + Input1->Position;
	if (Input1==Active)
		Output1->Position = MyFuzzy[0].Value(Input1->Position);
	UpdateLogic();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Output1Change(TObject *Sender)
{
	Label1->Caption = AnsiString("Output ") + Output1->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Input1Enter(TObject *Sender)
{
	Active = Input1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Output1Enter(TObject *Sender)
{
	Active = Output1;
}
//---------------------------------------------------------------------------
void TForm1::GridClear()
{
	for(int i=0;i<grid->ColCount;i++)
	{
		grid->Cells[1][i+1] = "";
		grid->Cells[2][i+1] = "";
		grid->Cells[3][i+1] = "";
		grid->Cells[4][i+1] = "";
	}
}
void __fastcall TForm1::Button3Click(TObject *Sender)
{
	MyFuzzy[0].Clear();
	GridClear();
	Refresh(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Input2Exit(TObject *Sender)
{
	Active = Output2;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Input2Change(TObject *Sender)
{
	Label4->Caption = AnsiString("Input ") + Input2->Position;
	if (Input2==Active)
		Output2->Position = MyFuzzy[1].Value(Input2->Position);
	UpdateLogic();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
float x,y;
	x = Input2->Position;
	y = Output2->Position;
	MyFuzzy[1].TFuzzyAddPoint(x,y);
	Refresh(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	MyFuzzy[1].Clear();
	GridClear();
	Refresh(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Input1Exit(TObject *Sender)
{
	Active = Output1;	
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Input2Enter(TObject *Sender)
{
Active = Input2;	
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Output2Change(TObject *Sender)
{
	Label5->Caption = AnsiString("Output ") + Output2->Position;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::LogicSelectionClick(TObject *Sender)
{
	UpdateLogic();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LogicInChange(TObject *Sender)
{
	UpdateLogic();
	AnsiString a = LogicIn->Position;
	LogicInText->Caption = a;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::New1Click(TObject *Sender)
{
	MyFuzzy[0].Clear();
	MyFuzzy[1].Clear();
	OneFuzzyLogic1Click(Sender);
	GridClear();
	Refresh(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Open1Click(TObject *Sender)
{
	if (OpenTextFileDialog1->Execute())
	{
	FILE *F = fopen(OpenTextFileDialog1->FileName.c_str(),"rt");
		if (F)
		{
		int c;
		char ch1,ch2;
		char Buffer[200];
        	SaveTextFileDialog1->FileName = OpenTextFileDialog1->FileName;
			New1Click(Sender);
			OneFuzzyLogic1Click(Sender);
			fgets(Buffer,199,F);	// intro (ignore)
			fgets(Buffer,199,F);	// Fuzzy1 Name
			Fuzzy1Name->Text = Buffer+2;
			fgets(Buffer,199,F);	// number of items
			sscanf(Buffer+2,"%d",&c);
			for(int i=0;i<c;i++)
			{
			float x,y;
				fgets(Buffer,199,F);
				sscanf(Buffer+2,"%f,%f",&x,&y);
				MyFuzzy[0].TFuzzyAddPoint(x,y);
			}
			fgets(Buffer,199,F);	// Fuzzy2 Name
			Fuzzy2Name->Text = Buffer;
			fgets(Buffer,199,F);	// number of items for second fuzzy
			sscanf(Buffer+2,"%d",&c);
			for(int i=0;i<c;i++)
			{
			float x,y;
				fgets(Buffer,199,F);
				sscanf(Buffer+2,"%f,%f",&x,&y);
				MyFuzzy[1].TFuzzyAddPoint(x,y);
				FuzzyLogicOperations1Click(Sender);
			}
			fgets(Buffer,199,F);
			if ((Buffer[0]=='/') && (Buffer[1]=='/'))
			{
			int max;
				sscanf(Buffer+2,"%d",&max);
				UpDown1->Position = (max / 1000) * 1000;
				max %= 1000;
				UpDown2->Position = (max / 100) * 100;
				max %= 100;
				UpDown3->Position = (max / 10) * 10;
				max %= 10;
				UpDown4->Position = max;
				UpDown1Click(Sender, 0);
			}
			fclose(F);
		}
		Refresh(Sender);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Save1Click(TObject *Sender)
{
	if ((SaveTextFileDialog1->FileName!="") || (SaveTextFileDialog1->Execute()))
	{

	AnsiString a;
	a = SaveTextFileDialog1->FileName;
	if (a.Pos(".h")==0)
		a = a + ".h";
	TFileStream *f = new TFileStream(a,fmCreate);
		a = "// The next lines are used by the fuzzy editor to save and load data\n";
		f->Write(a.c_str(),a.Length());
		a = "//";
		a += Fuzzy1Name->Text;
		f->Write(a.c_str(),a.Length());
		a = "//";
		a += MyFuzzy[0].Count();
		a += "  \t\t// Fuzzy 1 number of data values\n";
		f->Write(a.c_str(),a.Length());
		for(int i=0;i<MyFuzzy[0].Count();i++)
		{
		a = "//";
		TFuzzyXY *v = MyFuzzy[0].GetItem(i);
		a += v->x;
		a += ",";
		a += v->y;
		while(a.Length()<10) a += " ";
		a += " \t// data item ";
		a += (i+1);
		a += "\n";
		f->Write(a.c_str(),a.Length());
		}
		a = "//";
		a += Fuzzy1Name->Text;
		f->Write(a.c_str(),a.Length());
		a = "//";
		a += MyFuzzy[1].Count();
		a += "  \t\t// Fuzzy 2 number of data values\n";
		f->Write(a.c_str(),a.Length());
		for(int i=0;i<MyFuzzy[1].Count();i++)
		{
		a = "//";
		TFuzzyXY *v = MyFuzzy[1].GetItem(i);
		a += v->x;
		a += ",";
		a += v->y;
		while(a.Length()<10) a += " ";
		a += " \t// data item ";
		a += (i+1);
		a += "\n";
		f->Write(a.c_str(),a.Length());
		}
		a = "//";
		a += Input1->Max;
		a += "  // Maximum allowed\n";
		f->Write(a.c_str(),a.Length());

		a = "\n\n//The next section is code which can be included to set the values of Fuzzy1 and Fuzzy2\n";
		f->Write(a.c_str(),a.Length());
		a = "\nextern TFuzzy Fuzzy1;\nextern TFuzzy Fuzzy2;\n\nvoid LoadFuzzies()\n{\n";
		f->Write(a.c_str(),a.Length());
		for(int i=0;i<MyFuzzy[0].Count();i++)
		{
		TFuzzyXY *v = MyFuzzy[0].GetItem(i);
		a = "   TFuzzyAddPoint(Fuzzy1,";
		a += v->x;
		a += ",";
		a += v->y;
		a += ");\n";
		f->Write(a.c_str(),a.Length());
		}
		for(int i=0;i<MyFuzzy[1].Count();i++)
		{
		TFuzzyXY *v = MyFuzzy[1].GetItem(i);
		a = "   TFuzzyAddPoint(Fuzzy2,";
		a += v->x;
		a += ",";
		a += v->y;
		a += ");\n";
		f->Write(a.c_str(),a.Length());
		}
		a = "}\n\n";
		f->Write(a.c_str(),a.Length());

		delete f;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
int value = UpDown1->Position + UpDown2->Position + UpDown3->Position + UpDown4->Position;
	MaxValue->Caption = AnsiString("") + value;
	Input1->Max = value;
	Input2->Max = value;
	Output1->Max = value;
	Output2->Max = value;
	LogicIn->Max = value;
	FuzzyOut1->Max = value;
	FuzzyOut2->Max = value;
	LogicOut->Max = value;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	MyFuzzy[0].Optimize(1.0);
	Refresh(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
	MyFuzzy[1].Optimize(1.0);
	Refresh(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveAs1Click(TObject *Sender)
{
	SaveTextFileDialog1->FileName = "";
	Save1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Exit1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioGroup1Click(TObject *Sender)
{
	if (RadioGroup1->ItemIndex==0)
		Chart1->Visible = false;
	else
		Chart1->Visible = true;
	CheckBox3d->Visible = Chart1->Visible;
	UpDown5->Visible = CheckBox3d->Checked && Chart1->Visible;
	Refresh(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox3dClick(TObject *Sender)
{
	Chart1->View3D = CheckBox3d->Checked;
	UpDown5->Visible = CheckBox3d->Checked && Chart1->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpDown5Click(TObject *Sender, TUDBtnType Button)
{
	Refresh(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OneFuzzyLogic1Click(TObject *Sender)
{
	OneFuzzyLogic1->Checked = true;
	TwoFuzzyLogics1->Checked = false;
	FuzzyLogicOperations1->Checked = false;
	Refresh(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TwoFuzzyLogics1Click(TObject *Sender)
{
	OneFuzzyLogic1->Checked = false;
	TwoFuzzyLogics1->Checked = true;
	FuzzyLogicOperations1->Checked = false;
	Refresh(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FuzzyLogicOperations1Click(TObject *Sender)
{
	OneFuzzyLogic1->Checked = false;
	TwoFuzzyLogics1->Checked = false;
	FuzzyLogicOperations1->Checked = true;
	Refresh(Sender);
}
//---------------------------------------------------------------------------

