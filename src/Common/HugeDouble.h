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

 
#include "HugeInt.h"
#include "ClassProperty.h"
namespace Crystal {

using namespace Crystal;
#ifndef CHugeDouble
#define CHugeDouble "HugeDouble"
#define CHugeFraction "HugeFraction"


class HugeFraction : public Object
{
HugeInt Num;
HugeInt Den;
	inline void Normalize()
	{
		while(Num.LastDigitZero() && Den.LastDigitZero()) { Num.Shr32(); Den.Shr32(); }
		while(Num.IsEven() && Den.IsEven()) { Num.Shr1(); Den.Shr1(); }
	}
inline	HugeFraction & AddNumerators(HugeFraction &n) { Num.Add(n.Num); return *this; }
	HugeFraction & SubNumerators(HugeFraction &n) { Num.Sub(n.Num);  return *this; }
	void FindLCD(HugeFraction &n)//lowest common denominator
	{
		Normalize(); n.Normalize();
		HugeInt t = n.Den;
				n.Den.Mult(Den);
				n.Num.Mult(Den);
				Den.Mult(t);
				Num.Mult(t);
	}
public:
StdFunctions(HugeFraction,Object);
	HugeFraction() { Num = Den = 1; }
	HugeFraction(const HugeFraction &n) { Num = n.Num; Den = n.Den; }
	HugeFraction(const HugeInt &n,const HugeInt &d=1) { Num = n; Den = d; }
	virtual bool IsZero() const { return Num.IsZero(); }
	virtual bool Shl1() { if ((!Den.IsZero()) &&  Den.IsEven()) Den.Shr1(); else return Num.Shl1(); return Num.IsZero();  }
	virtual bool Shl32() { if ((!Den.IsZero()) && Den.GetBottomDigit()==0) Den.Shr32(); else return Num.Shl32(); return Num.IsZero(); }
	virtual bool Shr1() { if (Num.IsEven()) Num.Shr1(); else Den.Shl1(); return Num.IsZero(); }
	virtual bool Shr32() { if (Num.GetBottomDigit()==0) Num.Shr32(); else Den.Shl32(); return IsZero(); }
	virtual HugeFraction &Addf(const HugeInt &n) { HugeInt temp(n); temp.Mult(Den); Num.Add(temp); return *this; }
	virtual HugeFraction &Subf(const HugeInt &n) { HugeInt temp(n); temp.Mult(Den); Num.Sub(temp); return *this; }
	virtual HugeFraction &Add(unsigned int &n) { HugeInt temp(n); temp.Mult(Den); Num.Add(temp); return *this; }
	virtual HugeFraction &Sub(unsigned int &n) { HugeInt temp(n); temp.Mult(Den); Num.Sub(temp); return *this; }
	virtual HugeFraction &Sub(const unsigned int &n) { HugeInt temp(n); temp.Mult(Den); Num.Sub(temp); return *this; }
	virtual HugeFraction &Add(const HugeInt &n) {  HugeInt temp(n); temp.Mult(Den); Num.Add(temp); return *this; }
	virtual bool Equal(const HugeFraction &n)
	{
		return
			(Den==n.Den) &&
			(Num==n.Num); }

	virtual HugeFraction &Add(HugeFraction &n)
	{
		if (Den!=n.Den)	FindLCD(n);
		return AddNumerators(n);
	}
	virtual HugeFraction &Sub(HugeFraction &n)
	{
		if (Den!=n.Den)	FindLCD(n);
		return SubNumerators(n);
	}
	inline	HugeFraction & Mult(const HugeInt &n)
	{
		Num.Mult(n);
		return *this;
	}

	inline HugeFraction &Mult(const HugeFraction &n)
	{
		Num.Mult(n.Num);
		Den.Mult(n.Den);
		Normalize();
		return *this;
	}

	HugeFraction &Div(const HugeFraction &n)
	{
		Num.Mult(n.Den);
		Den.Mult(n.Num);
		Normalize();
		return *this;
	}

	HugeFraction &HugeFraction::Flip()
	{
		HugeInt t = Den;
		Den = Num;
		Num = t;
		return *this;
	}

	PropertyList *PropertyNames() const
	{
	PropertyList *result = Object::PropertyNames();
		result->AddPropertyByName("Numerator",this);
		result->AddPropertyByName("Denominator",this);
		return result;
	}
	const char *GetProperty(const PropertyParser &PropertyName,String &Result) const
	{
		if (PropertyName=="Numerator")
			return Num.GetProperty("Value",Result);
		if (PropertyName=="Denominator")
			return Den.GetProperty("Value",Result);
		return Object::GetProperty(PropertyName,Result);
	}

	bool HasProperty(const PropertyParser &PropertyName) const
	{
		return  PropertyName=="Numerator" ||
				PropertyName=="Denominator" ||
				Object::HasProperty(PropertyName);
	}
	bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
	{
		if (PropertyName=="Numerator")
			return Num.SetProperty("Value",PropertyValue);
		if (PropertyName=="Denominator")
			return Den.SetProperty("Value",PropertyValue);
		return Object::SetProperty(PropertyName,PropertyValue);
	}
};

class HugeDouble : public HugeInt
{
// the number of digits below the decimal
int Exp;
void Normalize(const HugeDouble &ExpToMatch);
HugeInt &Mult(const HugeInt &m); // not implemented, this class replaces with double equivalent
virtual bool Div(HugeInt &Dividend,HugeInt &Divisor,HugeInt &Quotient,HugeInt &Remainder) const;// not implemented, this class replaces with double equivalent
virtual bool Div(HugeInt &Divisor,HugeInt &Quotient,HugeInt &Remainder);// not implemented, this class replaces with double equivalent
virtual bool Div(HugeInt &Divisor,HugeInt &Result);// not implemented, this class replaces with double equivalent
virtual bool Mod(HugeInt &Divisor,HugeInt &Result);// not implemented, this class replaces with double equivalent
virtual HugeInt &operator=(const HugeInt &n); // not implemented, this class replaces with double equivalent
virtual HugeInt &SetValue(const HugeInt &n); // not implemented, this class replaces with double equivalent
virtual bool operator>(const HugeInt &n) const; // not implemented, this class replaces with double equivalent
virtual bool operator<(const HugeInt &n); // not implemented, this class replaces with double equivalent
virtual bool operator==(const HugeInt &n) const;// not implemented, this class replaces with double equivalent
virtual bool operator<(const HugeInt &n) const;// not implemented, this class replaces with double equivalent
virtual bool operator<=(const HugeInt &n) const;// not implemented, this class replaces with double equivalent
virtual bool operator>=(const HugeInt &n) const;// not implemented, this class replaces with double equivalent
virtual bool operator!=(const HugeInt &n) const;// not implemented, this class replaces with double equivalent
virtual HugeInt &Add(const HugeInt &n);// not implemented, this class replaces with double equivalent
virtual HugeInt &Sub(const HugeInt &n);// not implemented, this class replaces with double equivalent


public:
StdFunctionsNoDup(HugeDouble,HugeInt);
// CryObject virtuals
virtual void CopyTo(Object &Dest) const;  //copies contents of this to Dest
virtual Object *Dup() const; // creates a duplicate of this object
virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;

virtual bool HasProperty(const PropertyParser &PropertyName)const;
virtual int GetPropertyCount() const;
virtual PropertyList* PropertyNames() const;

virtual bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);

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
HugeDouble() { Exp = 0;}
HugeDouble(unsigned int n)  {Exp = 0; }
HugeDouble(const HugeInt &n) { Exp = 0; }
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
bool operator==(const HugeDouble &n) const
{
const HugeInt *t = this;
	if (Exp==n.Exp) return *t==n;
	return false;
}
bool operator>(unsigned int n)const { HugeDouble a(n); return *this>a; }
bool operator<(unsigned int n) const { HugeDouble a(n); return *this<a; }
bool operator==(unsigned int n) const { HugeDouble a(n); return *this==a; }

bool operator<=(unsigned int n) const { return !(*this>n); }
bool operator>=(unsigned int n) const { return !(*this<n); }
bool operator!=(unsigned int n) const { return !(*this==n); }
bool operator<=(const HugeDouble &n) const { return !(*this>n); }
bool operator>=(const HugeDouble &n) const { return !(*this<n); }
bool operator!=(const HugeDouble &n) const { return !(*this==n); }
HugeDouble &Shl(unsigned int n=1);
HugeDouble &Shr(unsigned int n=1);
HugeDouble &Add(const HugeDouble &n);
HugeDouble &Sub(const HugeDouble &n);
HugeDouble &Add(const unsigned int &n) { HugeInt::Add(n); return *this; }
HugeDouble &Sub(const unsigned int &n) { HugeInt::Sub(n); return *this; }

void Huge2Str(char *b);

#ifdef VALIDATING
virtual bool Test(bool Verbose,Object &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

};

};//namespace Crystal

#endif //THugeDouble
