//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
#include "TFuzzy.h"

FuzzyList *gf=0;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	Head = 0;
	Connections = 0;
}



//---------------------------------------------------------------------------
void __fastcall TForm1::Refresh(TObject *Sender)
{
AnsiString In,Out;
int MaxLength;
	if (Head==0)
	{
		Fuzzy1->Visible = false;
		return;
	}
	else
		Fuzzy1->Visible = true;
	ImageInfo *l = Head;
	TRect r;
	r.Left = 1;
	r.Top = 1;
	r.Right = Image1->Width;
	r.Bottom = Image1->Height;
//	Image1->Canvas->FillRect(r);

	Image1->Invalidate();
	if (ComboBox1->Items->Count==0)
		return;
	ImageInfo *ii = (ImageInfo *)ComboBox1->Items->Objects[ComboBox1->ItemIndex];
	vl->Visible = false;
	Output1->Visible = false;
	Label1->Visible = false;
	Button1->Visible = false;
	Button2->Visible = false;
	Button3->Visible = false;

	switch(ii->Type)
	{
	case ImageInfo::Fuzzy:
		{
		FuzzyList *f = (FuzzyList *)ii;
		vl->Strings->Clear();
		for(int i=0;i<f->Fuzzy.Length();i++)
		{
				In = f->Fuzzy.IndexAt(i);
				Out = f->Fuzzy.ValueAt(i);
				vl->InsertRow(In,Out,true);
		}
			vl->Visible = true;
			Output1->Visible = true;
			Label1->Visible = true;
			Button1->Visible = true;
			Button2->Visible = true;
			Button3->Visible = true;

		}
		break;
	case ImageInfo::Not:
		Label2->Caption = "Maximum Value";
		break;

	}
	return;

FuzzyList *f;
AnsiString a;
	if (f->Fuzzy.Length()<MyFuzzy[1].Length())
		MaxLength = MyFuzzy[1].Length();
	else
		MaxLength = f->Fuzzy.Length();
	grid->RowCount = 1+MaxLength;
int Complexity = 0;
	if (TwoFuzzyLogics1->Checked) Complexity =1;
	if (FuzzyLogicOperations1->Checked) Complexity = 2;
	switch(Complexity)
	{
	case 0:
	{
		grid->ColCount = 3;
	}
	break;
	case 1:
	{
		grid->ColCount = 5;
	}
	break;
	case 2:
	{
		grid->ColCount = 5;
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
		if (f->Fuzzy.Length()>i)
		{
			a = f->Fuzzy.IndexAt(i);
			grid->Cells[1][i+1] = a;
			a = f->Fuzzy.ValueAt(i);
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
	for(int i=0;i<f->Fuzzy.Length();i++)
	{
		s->AddXY(f->Fuzzy.IndexAt(i),f->Fuzzy.ValueAt(i));
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
/*	if (FuzzyLogicOperations1->Checked)
	{
		for(int i=0;i<LogicIn->Max;i+=UpDown5->Position)
		{
		VALUE_TYPE x,y;
			x = i;
			y = LogicFunctionResult(x);
			s->AddXY(x,y);
		}
	}
*/	
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
VALUE_TYPE x,y;

FuzzyList *f = (FuzzyList *)ComboBox1->Items->Objects[ComboBox1->ItemIndex];
	x = Input1->Position;
	y = Output1->Position;
	f->Fuzzy.TFuzzyAddPoint(x,y);
	Refresh(Sender);
}
void TForm1::UpdateLogic()
{
	ImageInfo *l = Head;
	AnsiString a;
	// Reset values
/*	while(l)
	{
		if (l->Type!=ImageInfo::In)
		{
			l->DoneIn1 = l->DoneIn2 = false;
			l->SetOutValue(0);
		}
		l = l->Next;
	}*/
	Connection *C = Connections;
	while(C)
	{
		C->Done = false;
		if (C->From->Type==ImageInfo::In)
		{
			if (C->ToIn==1)
			{
				C->To->SetIn1Value(C->From->In1V);
				C->To->DoneIn1 = true;
				if (C->To->In1Y==C->To->In2Y)	// only one input
					C->To->DoneIn2 = true;

			}
			else
			{
				C->To->SetIn2Value(C->From->In1V);
				C->To->DoneIn2 = true;
			}
			C->To->Calculate();
			C->Done = true;
		}
		C = C->Next;
	}
	bool Done = false;
	int InfLoopWatch=200;	// bigger then anything anyone would do
	while(!Done)
	{
		Done = true;
		C = Connections;
		while(C)
		{
			if (!C->Done)	// C In is not from an input (which is already done)
			{
				if (C->From->DoneIn1 && C->From->DoneIn2)
				{
					C->From->Calculate();
					if (C->ToIn==1)
					{
						C->To->SetIn1Value(C->From->OutV);
						C->To->DoneIn1 = true;
						if (C->To->In1Y==C->To->In2Y)	// only one input
							C->To->DoneIn2 = true;
					}
					else
					{
						C->To->SetIn2Value(C->From->OutV);
						C->To->DoneIn2 = true;
					}
					C->To->Calculate();
					C->Done = true;
				}
			}
			else
				Done = false;
			C = C->Next;
		}
		InfLoopWatch--;
		if (InfLoopWatch==0)
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Input1Change(TObject *Sender)
{
ImageInfo *ii = (ImageInfo *)ComboBox1->Items->Objects[ComboBox1->ItemIndex];
	switch(ii->Type)
	{
	case ImageInfo::In:
		ii->SetIn1Value(Input1->Position);
		break;
	case ImageInfo::Not:
		{
			ii->SetIn2Value(Input1->Position);
		}
		break;
	case ImageInfo::Fuzzy:
		{
		FuzzyList *f = (FuzzyList *) ii;
		gf = f;
		AnsiString sender = Sender->ClassName();
			Label2->Caption = AnsiString("Input ") + Input1->Position;
			if (Input1==Active)
			{
				Output1->Position = f->Fuzzy.Value(Input1->Position);
				f->SetIn1Value(Input1->Position);
				f->SetOutValue(Output1->Position);
			}
		}
		break;
	}
	UpdateLogic();
	Image1->Invalidate();
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
FuzzyList *f = (FuzzyList *)ComboBox1->Items->Objects[ComboBox1->ItemIndex];

	f->Fuzzy.Clear();
	GridClear();
	Refresh(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Input2Exit(TObject *Sender)
{
//	Active = Output2;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Input2Change(TObject *Sender)
{
/*	Label4->Caption = AnsiString("Input ") + Input2->Position;
	if (Input2==Active)
		Output2->Position = MyFuzzy[1].Value(Input2->Position);
	UpdateLogic();
*/	
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
/*VALUE_TYPE x,y;
	x = Input2->Position;
	y = Output2->Position;
	MyFuzzy[1].TFuzzyAddPoint(x,y);
	Refresh(Sender);
*/	
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
//Active = Input2;	
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Output2Change(TObject *Sender)
{
//	Label5->Caption = AnsiString("Output ") + Output2->Position;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::LogicSelectionClick(TObject *Sender)
{
	UpdateLogic();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LogicInChange(TObject *Sender)
{
/*	UpdateLogic();
	AnsiString a = LogicIn->Position;
	LogicInText->Caption = a;
*/	
}
//---------------------------------------------------------------------------

void __fastcall TForm1::New1Click(TObject *Sender)
{
FuzzyList *f = (FuzzyList *)ComboBox1->Items->Objects[ComboBox1->ItemIndex];

	f->Fuzzy.Clear();
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
/*		if (F)
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
			VALUE_TYPE x,y;
				fgets(Buffer,199,F);
				sscanf(Buffer+2,"%f,%f",&x,&y);
				f->Fuzzy.TFuzzyAddPoint(x,y);
			}
			fgets(Buffer,199,F);	// Fuzzy2 Name
			Fuzzy2Name->Text = Buffer;
			fgets(Buffer,199,F);	// number of items for second fuzzy
			sscanf(Buffer+2,"%d",&c);
			for(int i=0;i<c;i++)
			{
			VALUE_TYPE x,y;
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
		}*/
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
//		a += Fuzzy1Name->Text;
		a += "\n";
		f->Write(a.c_str(),a.Length());
		a = "//";
FuzzyList *fuzzy = (FuzzyList *) ComboBox1->Items->Objects[ComboBox1->ItemIndex];

		a += fuzzy->Fuzzy.Count();
		a += "  \t\t// Fuzzy 1 number of data values\n";
		f->Write(a.c_str(),a.Length());
		for(int i=0;i<fuzzy->Fuzzy.Count();i++)
		{
		a = "//";
		TFuzzyXY *v = fuzzy->Fuzzy.GetItem(i);
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
//		a += Fuzzy1Name->Text;
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
		for(int i=0;i<fuzzy->Fuzzy.Count();i++)
		{
		TFuzzyXY *v = fuzzy->Fuzzy.GetItem(i);
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


void __fastcall TForm1::Button1Click(TObject *Sender)
{
FuzzyList *f = (FuzzyList *) ComboBox1->Items->Objects[ComboBox1->ItemIndex];
	f->Fuzzy.Optimize(1.0);
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

void __fastcall TForm1::Button7Click(TObject *Sender)
{
static int count=1;
	Form3->Name->Text = "Name" + IntToStr(count);
	count++;
	if (Form3->ShowModal()==mrOk)
	{
	ImageInfo *l = Head;
		while(l)
		{
			if (Form3->Name->Text==l->Name)
			{
				ShowMessage("Fuzzy Logic Block with that Name already created");
				return;
			}
			l = l->Next;
		}
	int min,max;
		min = StrToInt(Form3->Min->Text);
		max = StrToInt(Form3->Max->Text);
		l = new FuzzyList(Form3->Name->Text,Form3->Description->Text,min,max);
		l->Next = Head;
		Head = l;
		UpdateComboBox();
static StartX = 10;
static StartY = 100;
		Head->y = StartX;
		Head->x = StartY;
		StartX += 10;
		StartY += 10;
		Refresh(Sender);
	}
}

void TForm1::UpdateComboBox()
{
ImageInfo *l = Head;
		ComboBox1->Items->Clear();
		while(l)
		{
			switch(l->Type)
			{
			case ImageInfo::Fuzzy:
				ComboBox1->Items->AddObject(l->Name,l);
				break;
			case ImageInfo::Not:
				{
				AnsiString a;
					a = "Not:";
					a += l->Name;
					ComboBox1->Items->AddObject(a,l);
				}
				break;
			case ImageInfo::In:
				{
				AnsiString a;
					a = "In:";
					a += l->Name;
					ComboBox1->Items->AddObject(a,l);
				}
			}
			l = l->Next;
		}
		ComboBox1->ItemIndex = 0;
}
//---------------------------------------------------------------------------

bool ImageInfo::Selected(int X,int Y)
{
		return (x <=X && y<=Y && x+64>=X && y+64>=Y);
}

bool ImageInfo::MouseAtIn(int X,int Y)	// returns true if at left side of symbol
{
		return (x <=X && y<=Y && x+32>=X && y+64>=Y);
}

bool ImageInfo::MouseAtOut(int X,int Y) // returns true if at right side of symbol
{
		return (x+32 <=X && y<=Y && x+64>=X && y+64>=Y);
}
bool ImageInfo::MouseAtIn1(int X,int Y)	// returns true if at top left side of symbol
{
		return (x <=X && y<=Y && x+32>=X && y+32>=Y);
}
bool ImageInfo::MouseAtIn2(int X,int Y)	// returns true if at bottom left side of symbol
{
		return (x <=X && y>=Y+32 && x+32>=X && y+64>=Y);
}



void __fastcall TForm1::Image11MouseDown(TObject *Sender, TMouseButton Button,
	  TShiftState Shift, int X, int Y)
{
ImageInfo *l = Head;
	if (DoubleClick)
	{
		DoubleClick = false;
		return;
	}
	if ((Button ==mbLeft) || From==0)
	while(l)
	{
		if (l->Selected(X,Y))
		{
			if (From==0)
			{
				From = l;
				LastMouseX = X;
				LastMouseY = Y;
				Moving = (Button==mbRight);
				return;
			}
			else
			{
				if (!Moving)
				{
				int ConnectionPoint = 1;	// default
					if (l->MouseAtIn2(X,Y))
						ConnectionPoint = 2;

				if (l->MouseAtIn(X,Y))
				{
				Connection *pc=0,*c = Connections;
				while(c)
				{
					if (c->From==From && c->To ==l)	// connection already present, get rid of it.
					{
						if (pc==0)
							Connections = c->Next;
						else
							pc->Next = c->Next;
						delete c;
						From=0;
						Refresh(Sender);
						return;
					}
					pc = c;
					c = c->Next;
				}
				 	c = new Connection(From,l);
					c->ToIn = ConnectionPoint;
					c->Next = Connections;
					Connections = c;
					From = 0;
				}
				}
			}
		}
		l = l->Next;
	}
	if (From)
	{
		Image1->Canvas->Pen->Width = 1;
		Image1->Canvas->Pen->Mode = pmNotXor;
		Image1->Canvas->MoveTo(From->x+64,From->y+32);
		Image1->Canvas->LineTo(LastMouseX,LastMouseY);
	}
	From = 0;	// user click outside of anything so drop from
	Refresh(Sender);	// incase a drop needs to redraw screen
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image11MouseMove(TObject *Sender, TShiftState Shift,
	  int X, int Y)
{
	if (From)
	{
		if (Moving)
		{
		TRect r;
			r.left = From->x-1;
			r.top = From->y-1;
			r.right = From->x+64+2;
			r.Bottom = From->y + 64+2;
//			Image1->Canvas->Pen->Color = clWhite;
//			Image1->Canvas->Rectangle(r);
//			Image1->Canvas->Refresh();
//			Image1->Canvas->Pen->Color = clBlack;
			From->x = X;
			From->y = Y;
			ImageList1->Draw(Image1->Canvas,From->x,From->y,From->ImageID,true);
//			Image1->Canvas->Refresh();
this->Refresh(Sender);
		}
		else
		{
			Image1->Canvas->Pen->Width = 1;
			Image1->Canvas->Pen->Mode = pmNotXor;
	//		Image1->Canvas->Pen->Style =   psClear;
			Image1->Canvas->MoveTo(From->x+64,From->y+32);
			Image1->Canvas->LineTo(LastMouseX,LastMouseY);
	//		Image1->Canvas->Pen->Style =   psSolid;
	//    	Image1->Canvas->Pen->Mode = pmBlack;
			Image1->Canvas->MoveTo(From->x+64,From->y+32);
			Image1->Canvas->LineTo(X,Y);
		}
		LastMouseX = X;
		LastMouseY = Y;
	}
	else
	{
	ImageInfo *l = Head;
		while(l)
		{
			if (l->Selected(X,Y))
			{
				StatusBar1->Panels->Items[0]->Text = l->Name;
				StatusBar1->Panels->Items[1]->Text = l->Description;
			}
			l = l->Next;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{

ImageInfo *l;
Connection *C;
LogicList *ll;
				while(Head)
				{
					l = Head;
					Head = l->Next;
					delete l;
				}
				while(Connections)
				{
					C = Connections;
					Connections = C->Next;
					delete C;
				}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
	if (Form2->ShowModal()==mrOk)
	{
	int min,max;
	static StartX = 10;
	static StartY = 10;
		min = StrToInt(Form3->Min->Text);
		max = StrToInt(Form3->Max->Text);
		ImageInfo *l = new LogicList(Form2->LogicSelection->ItemIndex,Form2->Description->Text);
		l->Next = Head;
		Head = l;
		if (((LogicList *)Head)->Type==LogicList::In)
		{
			Head->x = 10;
		}
		else
		if (((LogicList *)Head)->Type==LogicList::Out)
		{
			Head->x = Image1->Width-70;
		}
		else
		{
		Head->x = StartX;
		StartX += 10;
		}
		Head->y = StartY;
		StartY += 10;
		UpdateComboBox();
		Refresh(Sender);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{
	for(int x=0;x<grid->ColCount;x++)
		for(int y=0;y<grid->RowCount;y++)
			grid->Cells[x][y] = "";
	Refresh(Sender);
//FuzzyList *f = (FuzzyList *)ComboBox1->Items[ComboBox1->ItemIndex].Objects[0];

}
//---------------------------------------------------------------------------



void __fastcall TForm1::Image1Paint(TObject *Sender)
{
	ImageInfo *l = Head;
	TRect r;
	r.Left = 1;
	r.Top = 1;
	r.Right = Image1->Width;
	r.Bottom = Image1->Height;
	Image1->Canvas->FillRect(r);
	AnsiString a;
	l = Head;
	while(l)
	{
		ImageList1->Draw(Image1->Canvas,l->x,l->y,l->ImageID,true);
			r.left = l->x+5;
			r.top = l->y+35;
			r.right = l->x + 58;
			r.Bottom = l->y + 58;
		Image1->Canvas->TextOutA(r.Left+10,r.top+1,l->Name);
		r.top = l->In2Y+16 + l->y;
		r.left = l->x+58;
		if (l->Type==ImageInfo::In)
		{
			l->GetIn1Value(a);
		}
		else
		{
			l->GetOutValue(a);
		}
		Image1->Canvas->TextOutA(r.left,r.top,a);

//		TextRect(r,r.left+1,r.top+2,l->Name);
		l = l->Next;
	}
	Connection *C = Connections;
	while(C)
	{
		Image1->Canvas->MoveTo(C->From->x+64,C->From->y+32);
		if (C->ToIn==1)
			Image1->Canvas->LineTo(C->To->x,C->To->y+C->To->In1Y);
		else
			Image1->Canvas->LineTo(C->To->x,C->To->y + C->To->In2Y);
		C = C->Next;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	DoubleBuffered = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1DblClick(TObject *Sender)
{
ImageInfo *l = Head;
	while(l)
	{
		if (l->Selected(LastMouseX,LastMouseY))
		{
			if (l->Type==ImageInfo::Fuzzy)
			{
			FuzzyList *fl = (FuzzyList *)l;
				Form3->Description->Text = fl->Description;
				Form3->Name->Text = fl->Name;
				Form3->Min->Text = fl->Min;
				Form3->Max->Text = fl->Max;
				if (Form3->ShowModal()!=mrCancel)
				{
					fl->Name = Form3->Name->Text;
					fl->Description = Form3->Description->Text;
					fl->Min = StrToInt(Form3->Min->Text);
					fl->Max = StrToInt(Form3->Max->Text);
					fl->SetImageID();
				}
				From = 0;
				Refresh(Sender);
				From = 0;
			}
			else
			{
				Form2->Description->Text = l->Description;
				Form2->LogicSelection->ItemIndex = l->Type;
				if (Form2->ShowModal()!=mrCancel)
				{
					l->Type = Form2->LogicSelection->ItemIndex;
					l->Description = Form2->Description->Text;
					l->SetImageID();
				}
				From = 0;
				Refresh(Sender);
				From = 0;
			}

		}
		l = l->Next;
	}
	DoubleClick = true;
}
//---------------------------------------------------------------------------

