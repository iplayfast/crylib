/***************************************************************************
 *   Copyright (C) 2003 by Chris Bruner                                    *
 *   chris@Martha.crystal.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

 
 #include "hugeint.h"

namespace Crystal {

using namespace Crystal;
#ifndef THugeDouble
#define THugeDouble "HugeDouble"
#define THugeFraction "HugeFraction"


class HugeFraction : private HugeInt
{
HugeInt Den;
	void Normalize() { while(IsEven() && Den.IsEven()) { HugeInt::Shr1(); Den.Shr1(); } }
	HugeFraction & AddNumerators(HugeFraction &n) { HugeInt::Add(n); return *this; }
	HugeFraction & SubNumerators(HugeFraction &n) { HugeInt::Sub(n);  return *this; }
	void MakeSameDen(HugeFraction &n)
	{
		Normalize(); n.Normalize();
		HugeInt t = n.Den;
				n.Den.Mult(Den);
				n.MultNum(Den);
				Den.Mult(t);
				HugeInt::Mult(t);
	}
public:
StdFunctions(HugeFraction,HugeInt);
	HugeFraction() { Den = 1; }
	HugeFraction(const HugeFraction &n) { Den = n.Den; }
        HugeFraction(const HugeInt &n,const HugeInt &d) :HugeInt(n) { Den = d; }
	virtual bool Shl1() { if ((!Den.IsZero()) &&  Den.IsEven()) Den.Shr1(); else return HugeInt::Shl1(); return IsZero();  }
	virtual bool Shl32() { if ((!Den.IsZero()) && Den.GetBottomDigit()==0) Den.Shr32(); else return HugeInt::Shl32(); return IsZero(); }
	virtual bool Shr1() { if (IsEven()) HugeInt::Shr1(); else Den.Shl1(); return IsZero(); }
	virtual bool Shr32() { if (GetBottomDigit()==0) HugeInt::Shr32(); else Den.Shl32(); return IsZero(); }
	virtual HugeFraction &Addf(const HugeInt &n) { HugeInt temp(n); temp.Mult(Den); HugeInt::Add(temp); return *this; }
	virtual HugeFraction &Subf(const HugeInt &n) { HugeInt temp(n); temp.Mult(Den); HugeInt::Sub(temp); return *this; }
  virtual HugeFraction &Add(unsigned int &n) { HugeInt temp(n); temp.Mult(Den); HugeInt::Add(temp); return *this; }
	virtual HugeFraction &Sub(unsigned int &n) { HugeInt temp(n); temp.Mult(Den); HugeInt::Sub(temp); return *this; }

	virtual HugeFraction &Add(HugeFraction &n)
	{
		if (Den==n.Den) return AddNumerators(n);
		MakeSameDen(n);
		HugeInt::Add(n);
		return *this;
	}
	virtual HugeFraction &Sub(HugeFraction &n)
	{
		if (Den==n.Den) return SubNumerators(n);
		MakeSameDen(n);
		HugeInt::Sub(n);
		return *this;
	}
	HugeFraction & MultNum(const HugeInt &n);// { HugeInt::Mult(n); return *this; }
	HugeFraction &Mult(const HugeFraction &n);// { HugeInt::Mult(n); Den.Mult(n.Den); Normalize(); return *this; }
	HugeFraction &Div(const HugeFraction &n);// { HugeInt::Mult(n.Den); Den.Mult(n); Normalize(); return *this; }
	HugeFraction &Flip();// { HugeInt t = Den; Den.SetValue(*((HugeInt *)this));  SetValue(t); return *this; }
	const char *GetAsStr(HugeFraction Base) const;
	const char *GetAsStr() const;// { HugeFraction ten; ten.SetValue("10"); return GetAsStr(ten); }
};

class HugeDouble : public HugeInt
{
// the number of digits below the decimal
int Exp;
void Normalize(const HugeDouble &ExpToMatch);
public:
StdFunctionsNoDup(HugeDouble,HugeInt);
// CryObject virtuals
virtual void CopyTo(CryObject &Dest) const;  //copies contents of this to Dest
virtual CryObject *Dup() const; // creates a duplicate of this object
virtual const char *GetProperty(CryPropertyParser &PropertyName,CryString &Result) const;
virtual bool HasProperty(CryPropertyParser &PropertyName)const;
virtual int GetPropertyCount() const;
virtual CryPropertyList* PropertyNames() const;

virtual bool SetProperty(CryPropertyParser &PropertyName,const char *PropertyValue);

// HugeDouble functions
bool ZeroOut() { HugeInt::ZeroOut(); Exp = 0; return true;}
bool Shl32() { Exp--; return IsZero();}
bool  Shr32() { Exp++; return IsZero(); }
bool Shr1() {
    if (IsOdd())
    {
    	SetNumDigits(GetNumDigits()+1);
	Shl32();
    }
    HugeInt::Shr1();
    return IsZero();
   }
   
    

HugeDouble &Mult(const HugeDouble &m);
unsigned int GetExp() const { return Exp; }
void SetExp(unsigned int m) { Exp = m; }
HugeDouble GetDigit(unsigned int Digit,const HugeDouble &Base) const;
unsigned int GetDigit(unsigned int Digit,unsigned int Base) const;
virtual unsigned int GetDigit(unsigned int Digit) const  { return HugeInt::GetDigit(Digit); }

void GetDigits(unsigned int start,unsigned int end,HugeDouble Base,unsigned int *Digits) const;
HugeDouble() { Exp = 0;}
HugeDouble(unsigned int n)  {Exp = 0; }
//HugeDouble(unsigned int n) { ZeroOut(); Number[NumDigits-1] = n; SetFlags(n); }
HugeDouble(const HugeDouble &n) {*this=n; }
// return true if div by 0
bool Div(HugeDouble &Dividend,HugeDouble &Divisor,HugeDouble &Quotient,HugeDouble &Remainder) const;
bool Div(HugeDouble &Divisor,HugeDouble &Quotient,HugeDouble &Remainder);
bool Div(HugeDouble &Divisor,HugeDouble &Result) { HugeDouble Q; return Div(Divisor,Result,Q); }
bool Mod(HugeDouble &Divisor,HugeDouble &Result) { HugeDouble R; return Div(Divisor,R,Result); }

HugeDouble &Inc(unsigned int Amount=1) { HugeInt::Add(Amount); return *this; }
HugeDouble &Dec(unsigned int Amount=1) { HugeInt::Sub(Amount); return *this; }

HugeDouble &IncDigit(unsigned int d) { HugeInt::IncDigit(d); return *this; }
HugeDouble &DecDigit(unsigned int d) { HugeInt::DecDigit(d); return *this; }
HugeDouble &operator=(unsigned int n) { 
	ZeroOut();
	HugeInt::Add(n);
	return *this;
	}
	
HugeDouble &operator=(const HugeDouble &n) { HugeInt *h = this; *h = n; Exp = 0; return *this;}
HugeDouble & SetValue(const char *str);
bool operator>(HugeDouble &n) const
{
const HugeInt *t = this;
	if (Exp==n.Exp) return *t>n;
	unsigned a = GetFirstDigit() + Exp;
	unsigned b = n.GetFirstDigit() + n.GetExp();
	if (a==b) return HugeInt::GetDigit(a) > ((HugeInt *)&n)->GetDigit(b);
	else return a>b;
}
bool operator<(HugeDouble &n) const
{
const HugeInt *t = this;
	if (Exp==n.Exp) return *t<n;
	unsigned a = GetFirstDigit() + Exp;
	unsigned b = n.GetFirstDigit() + n.GetExp();
	if (a==b) return HugeInt::GetDigit(a) < ((HugeInt *)&n)->GetDigit(b);
	else return a<b;
}
bool operator==(HugeDouble &n) const
{
const HugeInt *t = this;
	if (Exp==n.Exp) return *t==n;
	return false;
/*	unsigned a = GetFirstDigit() + Exp;
	unsigned b = n.GetFirstDigit() + n.GetExp();
	if (a!=b) 	return false;
	return HugeInt::GetDigit(a)==((HugeInt *)&n)->GetDigit(b);*/
}
bool operator>(unsigned int n)const { HugeDouble a(n); return *this>a; }
bool operator<(unsigned int n) const { HugeDouble a(n); return *this<a; }
bool operator==(unsigned int n) const { HugeDouble a(n); return *this==a; }

bool operator<=(unsigned int n) const { return !(*this>n); }
bool operator>=(unsigned int n) const { return !(*this<n); }
bool operator!=(unsigned int n) const { return !(*this==n); }
bool operator<=(HugeDouble &n) const { return !(*this>n); }
bool operator>=(HugeDouble &n) const { return !(*this<n); }
bool operator!=(HugeDouble &n) const { return !(*this==n); }
HugeDouble &Shl(unsigned int n=1);
HugeDouble &Shr(unsigned int n=1);
HugeDouble &Add(const HugeDouble &n);
HugeDouble &Sub(const HugeDouble &n);
HugeDouble &Add(unsigned int n) { HugeInt::Add(n); return *this; }
HugeDouble &Sub(unsigned int n) { HugeInt::Sub(n); return *this; }
const char *GetAsStr(unsigned int Base=10)const;
const char *GetAsStr(const HugeDouble &base)const;
void Huge2Str(char *b);
#ifdef VALIDATING
virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

};

};//namespace Crystal

#endif //THugeDouble
