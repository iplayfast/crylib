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
		if (n>v) return -1;
	}
	return -1;
}
bool CrySet::IsIn(int v)
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
CrySet Copy(*this);
int CS = Copy.Size();
int SS = s.Size();
	Clear();
	for(int i=0;i<CS;i++)
	{
		for(int j=0;j<SS;j++)
		{
		int v = Copy.GetValue(i);
			if (v==s.GetValue(j)) {
				Add(v);
				break;
			}
		}
	}
}


