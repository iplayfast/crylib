//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("CheckLabel.cpp", CheckedLabel); /* TFrame: File Type */
USEFORM("ClassBuilderUnit.cpp", ClassBuilderFrm);
USEFORM("labelEditfrm.cpp", LabelEdit); /* TFrame: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TClassBuilderFrm), &ClassBuilderFrm);
		Application->CreateForm(__classid(TCheckedLabel), &CheckedLabel);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
