//****************************************************
//Copyright 2004
// Crystal Software (Canada) Inc.
//****************************************************


//Includes


//Class Instance Includes
#include "CrySet.h"
#include "ClassProperty.h"
#include "CryXML.h"	//Needed for SetDefaultValues
#include "CryArray.h"

using namespace Crystal;
using namespace std;
/// Add a value to the set
void CrySet::Add(int v)
{
int S = Size();
	for(int i=0;i<S;i++)
	{
	int n = GetValue(i);
		if (n==v) return;	// set already contains value
		if (n<v) {	// insertion sort
			SetValue(i,v);
			v = n;
		}
	}
	SetSize(S+1);
	SetValue(S,v);
}

/// remove a value from the set
void CrySet::Sub(int v)
{
int p = Pos(v);
	if (p>-1)
		Delete(p,1);
	return;
}
int CrySet::Pos(int v)
{
int s = Size();
	for(int i=0;i<s;i++)
	{
	int n = GetValue(i);
		if (n==v) return i;
		if (n<v) return -1;
	}
	return -1;
}
bool CrySet::Has(int v)
{
	return Pos(v)!=-1;
}
int CrySet::Matches(CrySet &s)
{
int count =0;
	for(unsigned int i=0;i<s.Size();i++)
	{
	int v = s[i];
	bool found = false;
		for(unsigned int j=0;j<Size();j++)
		{
			found = GetValue(j)==v;
			if (found) break;
		}
		if (found) count++;
	}
	return count;
}


void CrySet::Union(CrySet &s)
{
unsigned int SS = s.Size();
	for(unsigned int i=0;i<SS;i++)
		Add(s.GetValue(i));
}
void CrySet::Intersect(CrySet &s)
{
int SS = s.Size();
	for(int i=0,j=Size();i<j;i++)
	{
		int v = GetValue(i);
		if (! s.Has(v)) {
		   Delete(i,1);
		   i--;
		   j--;
		}
	}
}

bool CrySet::operator==(CrySet &s)
{
	if (Count()==s.Count()) {
		for(int i=0,j=Count();i<j;i++)
		{
			if (GetValue(i)!=s.GetValue(i)) {
				return false;
			}
		}
		return true;
	}
	return false;
}
#ifdef VALIDATING
bool CrySet::Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail))
{
char Result[200];
bool Fail = false;
CryString spn,spv,stemp;

CrySet Copy;
CrySet *OrgObject = 0;
	Object.CopyTo(Copy);
	if (Object.IsA(TCrySet)) {
		OrgObject=(CrySet *)&Object;        
	}
	Fail = Copy!=*OrgObject;
	sprintf(Result,"Copy Constructor");
	if (!CallBack(Verbose,Result,Fail))
		return false;
CrySet Prime;
	Prime.Add(2);
	Prime.Add(3);
	Prime.Add(5);
	Prime.Add(7);
	Prime.Add(11);
	Prime.Add(13);
CrySet Odd;
	for(int i=1;i<15;i+=2)
		Odd.Add(i);
	for(int i=0;i<15;i++)
	{
	bool isodd =((i & 1)==1);
	bool oddin = Odd.Has(i);
		Fail |= oddin != isodd;
	}
	sprintf(Result,"Validating Set values");
	if (!CallBack(Verbose,Result,Fail))
		return false;
CrySet Test(Odd);
	Test.Union(Prime);
	for(int i=0;i<15;i++)
	{
		Fail |= Test.Has(i)!= ((i & 1) || (i==2));
	}
	sprintf(Result,"Union");
	if (!CallBack(Verbose,Result,Fail))
		return false;
	Test.Intersect(Prime);
	Fail |= Test != Prime;
	sprintf(Result,"Intersect");
	if (!CallBack(Verbose,Result,Fail))
		return false;
	Test = Odd;
	Test.Add(2);
	Test.Sub(9);
	Test.Sub(1);
	Fail |= Test != Prime;
	sprintf(Result,"Add and Sub");
	if (!CallBack(Verbose,Result,Fail))
		return false;
	return CryTArray<int>::Test(Verbose,Object,CallBack);
}
#endif

