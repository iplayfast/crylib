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
#include <stdio.h>
#include <string.h>
#include "HugeInt.h"
#include "HugeDouble.h"
#include "ClassProperty.h"
#include "ClassException.h"

using namespace Crystal;


void HugeDouble::CopyTo(Object &Dest) const  //copies contents of this to Dest
{
    if (Dest.IsA(CHugeDouble))
    {
    HugeDouble *h = (HugeDouble *)&Dest;
        *h = *this;
        return;
    }
    if (Dest.IsA(CHugeInt))
    {
    HugeInt *h = (HugeInt *) &Dest;
    	*h = *this;
	return;
    }
    if (Dest.IsA(CStream))
    {
		SaveTo(*(Stream *)&Dest);
        return;
    }
    throw Exception(this,"Can't copy HugeDouble to %s",Dest.ChildClassName());
}
#ifdef VALIDATING
bool HugeDouble::Test(bool Verbose,Object &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail))
{
bool fail = false;
    if (Object::Test(Verbose,Object,CallBack))
        return true;
  HugeDouble m,n;
  	m.Add(0xffffffff);
	m.Inc(1);
	m.Inc(1);
	m.Dec(2);
	n = m;
	m.Add(m);
	m.Sub(n);
    if (m!=0xffffffff)
        fail = true;
    if (!CallBack(Verbose,"Carry Tests",fail))
        return true;
//    m.Set
    return true;
}
#endif

Object *HugeDouble::Dup() const // creates a duplicate of this object
{
HugeDouble *h = new HugeDouble();
    *h = *this;
    return h;
}

const char *HugeDouble::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (PropertyName=="Value")
	{
		HugeInt::GetProperty(PropertyName,Result);
		Result.printf("E%d",Exp);
		return Result.AsPChar();
	}
    return Object::GetProperty(PropertyName,Result);
}
bool HugeDouble::HasProperty(const PropertyParser &PropertyName)const
{
    return (PropertyName=="Value") || Object::HasProperty(PropertyName);
}

PropertyList *HugeDouble::PropertyNames() const
{
	PropertyList *n = Object::PropertyNames();
	n->AddPropertyByName("Value",this);
	return n;
}
int HugeDouble::GetPropertyCount() const
{
	return 1 + Object::GetPropertyCount();
}
bool HugeDouble::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Value")
	{
	String v(PropertyValue);
		HugeInt::SetProperty(PropertyName,PropertyValue);
		v.Delete(0,v.Pos("E"));
		v.scanf("%d",&Exp);
		return true;
    }
    return Object::SetProperty(PropertyName,PropertyValue);
}


HugeDouble &HugeDouble::SetValue(const char *str)
{
	char ch;
	HugeDouble M = 10;
	ZeroOut();
	bool ExpFound = false;
	const char *orgstr = str;
	while(*str)
	{
		if (*str=='E')
		{
			 ExpFound = true;
			 str++;
			 continue;
		}
		if (ExpFound)
		{
			Exp*=10;
			if (*str<'0' || *str>'9')
				throw Exception(this,"Bad format for HugeDouble ",orgstr);
			Exp += (*str - '0');
		}
		else
		{
			if (*str<'0' || *str>'9')
				throw Exception(this,"Bad format for HugeDouble ",orgstr);
			ch = *str - '0';
			M = *this;
					// Mult by 10 = (M*2*2+M)*2
			Shl1();	// *2
			Shl1(); // *2
			Add(M); // add original value
			Shl1(); // *2

			Add(ch);	// Add new digit
		}
		str++;
	}
	return *this;
}

HugeDouble &HugeDouble::Mult(const HugeDouble &m)
{
int NewExp = Exp + m.Exp;
	HugeInt::Mult(m);
	Exp = NewExp;
	return *this;
}


HugeDouble &HugeDouble::Shl(unsigned int n)
{
    while(n>32)
    {
        Shl32();
        n-=32;
    }
    while(n)
    {
        Shl1();
        n--;
    }
    return *this;
}


HugeDouble &HugeDouble::Shr(unsigned int n)
{
    while(n>32)
    {
        Shr32();
        n-=32;
    }
    while(n)
    {
        Shr1();
        n--;
    }
    return *this;
}

void HugeDouble::Normalize(const HugeDouble &ExpToMatch)
{
	if (Exp==ExpToMatch.Exp) return;
	if (IsZero()) 
	{
		Exp = ExpToMatch.Exp;
		return;
	}

// this is for changing the const ExpToMatch. This is only done when the value doesn't change, but the internal representation does
HugeDouble *t = (HugeDouble *) &ExpToMatch;	
	if (ExpToMatch.IsZero())
	{//		ExpToMatch.Exp = Exp;	// Setting the exponent of a zero number doesn't change the value of the number
		t->Exp = Exp;
		return;
	}
	if (Exp>ExpToMatch.Exp)		// Number is Big
	{
		while((Exp>ExpToMatch.Exp) && LastDigitZero())
		{
			HugeInt::Shr32();
			Exp--;
		}	
	}
	if (Exp==ExpToMatch.Exp) return;
	while((Exp<ExpToMatch.Exp) && ExpToMatch.LastDigitZero())
	{
	HugeInt *i = (HugeInt *) &ExpToMatch;
		i->Shr32();
		t->Exp--;
	}
	while (Exp<ExpToMatch.Exp)	// number is small
	{
		HugeInt::Shl32();
		Exp++;
	}	
}
HugeDouble &HugeDouble::Add(const HugeDouble &n)
{
#ifdef Verify
    HugeDouble Org = *this;
#endif
	 if (Exp==n.Exp)
	{
		HugeInt::Add(n);
		return *this;
	}
	Normalize(n);
	HugeInt::Add(n);
	return *this;
}
HugeDouble &HugeDouble::Sub(const HugeDouble &n)
{
	 if (Exp==n.Exp)
	{
		HugeInt::Sub(n);
		return *this;
	}
	Normalize(n);
	HugeInt::Sub(n);
	return *this;
}

bool HugeDouble::Div(HugeDouble &Divisor,HugeDouble &Quotient,HugeDouble &Remainder)
{
    return Div(*this,Divisor,Quotient,Remainder);
}


bool HugeDouble::Div(HugeDouble &Dividend, HugeDouble &Divisor,HugeDouble &Quotient,HugeDouble &Remainder) const
{
bool Result = HugeInt::Div(Dividend,Divisor,Quotient,Remainder);
return Result;
/*    unsigned int num_bits,q,bit;
	unsigned int i;
	HugeDouble d;
	Quotient.ZeroOut();
	Remainder.ZeroOut();
	if (Divisor.IsZero())
		return true;    // divide by zero error

	if (Divisor > Dividend)
	{
		Remainder = Dividend;
		return false;
	}
	if (Divisor == Dividend)
	{
		Quotient = 1L;
		return false;
	}
	if (Divisor.IsOne())
		return false;

	if (Divisor.IsTwo())
	{
		if (Dividend.IsOdd())
			Remainder = 1;
		else
			Remainder = 0;
		Quotient = Dividend;
		Quotient.Shr1();
		return false;
	}

	//-----------------------------------

	//    #define SLOW_BUT_WORKING
#ifndef SLOW_BUT_WORKING
//    while(IsEven() && Divisor.IsEven())
//    {
//        Shr1();
//        Divisor.Shr1();
//    }
	HugeDouble OrgDivisor = Divisor;
	HugeDouble ShiftDivisor = Divisor;
	unsigned int DividendFirstBit = Dividend.FirstBit();
	unsigned int DivisorFirstBit=  Divisor.FirstBit();
	while(DivisorFirstBit>DividendFirstBit)
	{
		if ((DivisorFirstBit-32)>DividendFirstBit)
		{
			DivisorFirstBit -= 32;
			ShiftDivisor.Shl32();
		}
		else
		{
			while(DivisorFirstBit>DividendFirstBit) // avoid the above if, we know the answer
			{
				DivisorFirstBit--;
				ShiftDivisor.Shl1();
			}
			break;  // we're done
		}
	}
	Remainder = Dividend;
	while(ShiftDivisor >=OrgDivisor)
	{
		if (!Quotient.IsZero())
			Quotient.Shl1();
		if (Remainder >= ShiftDivisor)
		{
			Remainder.Sub(ShiftDivisor);
			Quotient.Inc();
		}
		ShiftDivisor.Shr1();
	}
	return false;
	//-----------------------------------
#endif
#ifdef SLOW_BUT_WORKING

	num_bits = NumBits;
	bool First = false;
	while(Remainder < Divisor)
	{
		bit = (Dividend.Number[0] & 0x80000000) >> 31;
		if (First)
			Remainder.Shl1();
		First |= bit;
		Remainder.Number[GetNumDigits()-1] |= bit;
		d = Dividend;
		Dividend.Shl1();
		num_bits--;
	}
	Dividend = d;
	Remainder.Shr1();
	num_bits++;

	HugeDouble t;
	for(unsigned int i = 0;i<num_bits;i++)
	{
		bit = (Dividend.Number[0] & 0x80000000) >> 31;
		Remainder.Shl1();
		Remainder.Number[GetNumDigits()-1] |= bit;
		t = Remainder;
		t.Sub(Divisor);
		q = !((t.Number[0] & 0x80000000) >> 31);
		Dividend.Shl1();
		Quotient.Shl1();
		Quotient.Number[GetNumDigits()-1] |= q;
		if (q)
			Remainder = t;
	}
	return false;
#endif
*/
}



//==========================================================

HugeDouble Power(HugeDouble x,HugeDouble n)
{
	HugeDouble r(1);
	HugeDouble y(x);
	while(n > 1)
	{
		if (n.IsOdd())
			r.Mult(y);
		n.Shr1();
        y.Mult(x);
        x = y;
    }
    r.Mult(y);
    return r;
}

// Return (x*y)%z
HugeDouble MulMod(HugeDouble x,HugeDouble y,HugeDouble z)
{
    HugeDouble d1,d2;
    x.Mult(y);
    x.Div(z,d1,d2);
    return d2;
}
// a^b % m
HugeDouble ModExp(HugeDouble a ,HugeDouble b,HugeDouble m)
{
    HugeDouble n = 1;
    HugeDouble d1,d2;//dummy
    while(!b.IsZero())
    {
        if (b.IsOdd())
        {
            n.Mult(a);
            n.Div(m,d1,d2);
            n = d2;
        }
        b.Shr1();
        {
            a.Mult(a);
            a.Div(m,d1,d2);
            a = d2;
        }
    }
    return n;
}

bool TestForPrime(HugeDouble n,HugeDouble Base)
{
    HugeDouble t = n;
    t.Dec();
    if (t.IsZero())
        return false;
    HugeDouble nMinus1 = t;
    unsigned int a = 0;
    //Find t and a satisfying: n-1=2^a * t, t odd
    while( t.IsEven())
    {
        t.Shr1();
        a++;
    }
    // We check the congruence class of b^((2^i)t) % n for each i
    // from 0 to a - 1. If any one is correct, then n passes.
    // Otherwise, n fails.

    HugeDouble test = ModExp(Base, t, n);
    if( test.IsOne() || test == nMinus1 )
        return true;
    HugeDouble d1,d2;
    while( --a )
    {
        test.Mult(test);
        test.Div(n,d1,d2);
        test = d2;
        if( test == nMinus1 )
            return true;
    }
    return false;
}
