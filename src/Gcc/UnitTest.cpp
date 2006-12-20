#include <stdio.h>

#include "ClassString.h"
#include "CryBackProp.h"
#include "CryFuzzy.h"
#include "HugeInt.h"
#include "CrySet.h"

using namespace Crystal;
//---------------------------------------------------------------------------
bool/* __cdecl */FormCallBack(bool Verbose,const char *Result,bool fail)
{
String s;
	s.printf("%s %s",Result,fail ? "Fail" : "Pass");
  	printf("%s",s.AsPChar());
	if (fail)
  {
	return false;
  }
  return true;
}

void StringTest()
{
try
{
String a;
	a.Test(true,a,FormCallBack);
}
catch(Exception &E)
{
String s;
	s ="Exception Caught: ";
	s += E;
	printf("%s",s.AsPChar());
}
}
//---------------------------------------------------------------------------

void BackPropTest()
{
try
{
BPNetContainer bp;
#ifdef VALIDATING
	bp.Test(true,bp,FormCallBack);
#endif

}
catch(Exception &E)
{
String s;
	s ="Exception Caught: ";
	s += E;
	printf("%s",s.AsPChar());
}

}
//---------------------------------------------------------------------------
void TemplateTest()
{
try
{
TArray<int> a;
#ifdef VALIDATING
	a.Test(true,a,FormCallBack);
#endif

}
catch(Exception &E)
{
String s;
	s ="Exception Caught: ";
	s += E;
	printf("%s",s.AsPChar());
}

}
//---------------------------------------------------------------------------
void SetTest()
{
try
{
CrySet a;
	a.Test(true,a,FormCallBack);
}
catch(Exception &E)
{
String s;
	s ="Exception Caught: ";
	s += E;
	printf("%s",s.AsPChar());
}
}
//---------------------------------------------------------------------------
int main(int argc,char *argv[])
{
	SetTest();
	TemplateTest();
	StringTest();
}
