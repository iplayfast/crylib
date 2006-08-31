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

#ifndef THugeInt
#include "CryObject.h"


namespace Crystal {
using namespace Crystal;

#define THugeInt "HugeInt"

#ifdef __BORLANDC__
#include <mem.h>
#else
#include <string.h>
#endif

// HugeInt's are in base 0xffffffff +1 (so 32 bits per digit)
#define DefNumDigits   7

#define HighDigitBit 0x80000000
//#define Verify      // defined if calculations are to be verified

// flag bits
#define ISZERO  1
#define ISONE   2
#define ISTWO   4
#define ISNEG   8   //not used
#define ISMOSTLYZERO 16


class HugeInt : public CryObject
{
HugeInt *p;
unsigned int NumDigits;
unsigned int *Number;	// Number[NumDigits]
int Flags;
unsigned int FirstDigit;
HugeInt &DivMod10(unsigned int *ModResult);
protected:
// returns true if non-zero
bool SetFlags(int N)
{
    switch(N)
    {
        case 0: Flags = ISZERO; break;
        case 1: Flags = ISONE; break;
        case 2 : Flags = ISTWO; break;
        default : Flags = 0; break;
    }
    Flags |= ISMOSTLYZERO;  // won't get into this routine otherwise
    return (N!=0);
}
// returns true if non-zero
bool SetFlags()
	{
		Flags = 0L;
		for(FirstDigit=0;FirstDigit<(NumDigits-1);FirstDigit++)
			if (Number[FirstDigit]!=0L) return true;
		if (FirstDigit==NumDigits-1)
			return SetFlags(Number[NumDigits-1]);
    return false;
	}
void SetLastDigitFlags() { SetFlags(Number[NumDigits-1]); }
void SetNumDigits(unsigned int n);
public:
StdFunctionsNoDup(HugeInt,CryObject);
~HugeInt() { if (NumDigits) delete[]Number; }
// CryObject virtuals
virtual void CopyTo(CryObject &Dest) const;  //copies contents of this to Dest
virtual CryObject *Dup() const; // creates a duplicate of this object
virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
virtual bool HasProperty(const CryPropertyParser &PropertyName)const;
virtual int GetPropertyCount() const;
virtual CryPropertyList* PropertyNames() const;
virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
// HugeInt functions
      // returns true if non zero
virtual bool ZeroOut() { for(unsigned int i=0;i<NumDigits;i++) Number[i] = 0; Flags = ISZERO; FirstDigit = NumDigits-1; return false;}
virtual bool Shl1();
virtual bool Shl32();
virtual bool Shr1();
virtual bool Shr32();
virtual bool Mult10();
virtual bool IMZ() const { return (Flags & ISMOSTLYZERO)!=0; }
virtual unsigned int FirstBit() const;
virtual HugeInt &Mult(const HugeInt &m);
virtual unsigned int GetNumDigits() const { return NumDigits; }
virtual unsigned int GetBottomDigit() const { return Number[NumDigits-1]; }
virtual unsigned int GetDigit(unsigned int Digit) const  { return Number[NumDigits-Digit]; }
virtual unsigned int GetDigit(unsigned int Digit,unsigned int Base)  const;
virtual HugeInt GetDigit(unsigned int Digit,const HugeInt &Base) const;
virtual unsigned int GetFirstDigit()  const { return FirstDigit; }
virtual void SetDigit(unsigned int Digit,unsigned int Value) { Number[NumDigits-Digit] = Value; SetFlags();}
virtual void GetDigits(unsigned int start,unsigned int end,HugeInt Base,unsigned int *Digits) const;
virtual HugeInt *GetPointer() const  { return (HugeInt *)p; }
void SetPointer(HugeInt *_p) { p =_p; }
virtual bool Bit(unsigned int n) const  { return ((Number[n >>5] & (0x80000000 >> (n & 0x1f))) != 0);}
HugeInt() { p = 0L; NumDigits = 0; Number = 0;  SetNumDigits(DefNumDigits);ZeroOut(); }
HugeInt(unsigned int n)  { NumDigits = 0; Number = 0; SetNumDigits(DefNumDigits); Number[NumDigits-1] = n;  SetFlags(n);}
//HugeInt(unsigned int n) { ZeroOut(); Number[NumDigits-1] = n; SetFlags(n); }
HugeInt(const HugeInt &n) {NumDigits = 0; Number = 0; *this=n; }
// return true if div by 0
virtual bool Div(HugeInt &Dividend,HugeInt &Divisor,HugeInt &Quotient,HugeInt &Remainder) const;
virtual bool Div(HugeInt &Divisor,HugeInt &Quotient,HugeInt &Remainder);
virtual bool Div(HugeInt &Divisor,HugeInt &Result) { HugeInt Q; return Div(Divisor,Result,Q); }
virtual bool Mod(HugeInt &Divisor,HugeInt &Result) { HugeInt R; return Div(Divisor,R,Result); }

virtual HugeInt &Inc(unsigned int Amount=1);
virtual HugeInt &Dec(unsigned int Amount=1);

virtual HugeInt &IncDigit(unsigned int d);
virtual HugeInt &DecDigit(unsigned int d);
virtual HugeInt &operator=(unsigned int n) { ZeroOut(); Number[NumDigits-1] = n; SetFlags(n); return *this;}
//HugeInt &operator=(unsigned int n) { ZeroOut(); Number[NumDigits-1] = n; SetFlags(n); return *this; }
//HugeInt &operator=(unsigned int n) { ZeroOut(); Number[NumDigits-1] = n; SetFlags(n); return *this; }
virtual HugeInt &operator=(const HugeInt &n) { return SetValue(n); }
 virtual HugeInt &SetValue(const HugeInt &n)
 {
	if (this==&n) return *this;
	SetNumDigits(n.GetNumDigits());
	memcpy(Number,n.Number,sizeof(unsigned int) * n.NumDigits);
	Flags = n.Flags; FirstDigit = n.FirstDigit; return *this;}
virtual HugeInt & SetValue(const char *str);
virtual CryString &GetValue(CryString &Result) const;
virtual HugeInt &Or(HugeInt &n) { for(unsigned int i=0;i<NumDigits;i++) Number[i] |=n.Number[i]; SetFlags(); return *this; }
virtual HugeInt &And(HugeInt &n) { for(unsigned int i=0;i<NumDigits;i++) Number[i] &=n.Number[i]; SetFlags(); return *this; }
virtual bool Mask(HugeInt &v,unsigned int Digits) const
    {
        for(unsigned int i=NumDigits-1;Digits;Digits--,i--)
            if (Number[i] != v.Number[i])
                return false;
            return true;
        }
virtual void SetMask(HugeInt &v,unsigned int Digits)
    {
        for(unsigned int i=NumDigits-1;Digits;Digits--,i--)
            v.Number[i] = Number[i];
        v.SetFlags();
    }
virtual bool LastDigitZero() const { return Number[NumDigits-1]!=0; }
virtual bool IsEven() const { return (Number[NumDigits-1] & 1)==0; }
virtual bool IsOdd() const { return !IsEven(); }
virtual bool IsZero() const { return Flags & ISZERO; }
virtual bool IsOne() const { return Flags & ISONE; }
virtual bool IsTwo() const { return Flags & ISTWO; }
virtual bool operator>(HugeInt &n) const { return ((FirstDigit<n.FirstDigit) ||
                                        ((FirstDigit==n.FirstDigit) && Number[FirstDigit]>n.Number[n.FirstDigit]));  }
virtual bool operator>(unsigned int n)const { for(unsigned int i=0;i<NumDigits-1;i++)
                                            if (Number[i]>0L) return true;
                                       return (Number[NumDigits-1]>n);
                                      }
virtual bool operator<(unsigned int n) const { for(unsigned int i=0;i<NumDigits-1;i++)
                                            if (Number[i]>0L) return false;
                                         return (Number[NumDigits-1]<n);
                                       }
virtual bool operator==(unsigned int n) const { for(unsigned int i=0;i<NumDigits-1;i++)
                                            if (Number[i]!=0L) return false;
                                         return (Number[NumDigits-1]==n);
                                        }

virtual bool operator<=(unsigned int n) const { return !(*this>n); }
virtual bool operator>=(unsigned int n) const { return !(*this<n); }
virtual bool operator!=(unsigned int n) const { return !(*this==n); }
virtual bool operator==(HugeInt &n) const {
                 for(int i=(int)NumDigits-1;i>=0;i--)
                        if (Number[i]!=n.Number[i])
                            return false;
                 return true;
                                  }
virtual bool operator<(HugeInt &n) const {
                             if (IMZ())
                             {
                                if (n.IMZ())
                                    return GetBottomDigit() < n.GetBottomDigit();
                                return true;
                             }
                             if (n.IMZ())
                                return false;
                             unsigned int start = FirstDigit;
                             if (n.FirstDigit<start)
                                start = n.FirstDigit;
                             for(unsigned int i=start;i<NumDigits;i++)
                             {
                                if (Number[i]<n.Number[i]) return true;
                                if (Number[i]>n.Number[i]) return false;
                             }
                             return false;
                            }
virtual bool operator<=(HugeInt &n) const { return !(*this>n); }
virtual bool operator>=(HugeInt &n) const { return !(*this<n); }
virtual bool operator!=(HugeInt &n) const { return !(*this==n); }
virtual HugeInt &Shl(unsigned int n=1);
virtual HugeInt &Shr(unsigned int n=1);
virtual HugeInt &Add(const HugeInt &n);
virtual HugeInt &Sub(const HugeInt &n);
virtual HugeInt &Add(const unsigned int &n);
virtual HugeInt &Sub(const unsigned int &n);
const char *GetAsStr(unsigned int Base=10)const;
unsigned int GetAsLong()const { return Number[NumDigits-1];}
void Huge2Str(char *b);
#ifdef VALIDATING
virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif
};



HugeInt Power(HugeInt x,HugeInt n);
bool TestForPrime(HugeInt x,HugeInt Base);
#define HASHSIZE 4096
class Pool
{
HugeInt *pPool;
unsigned int HashArray[HASHSIZE];
unsigned int BitArray[32];

unsigned int _Count;
public:
    Pool() { pPool = 0; _Count = 0; memset(HashArray,0,HASHSIZE);
                for(unsigned int j=1, i=0;i<32;i++)
                {
                    BitArray[i] = j;
                    j*=2;
                }
            }
    ~Pool() { while(pPool) { delete Get(); } }
    HugeInt *Get(); // will alway return 1 (will create if needed)

    void SetHash(HugeInt *p);
    bool CheckHash(HugeInt *p);

    void Put(HugeInt *);
    void Put(HugeInt *p1,HugeInt *p2) { Put(p2); Put(p1); } // preserve order
    void Put(HugeInt *p1,HugeInt *p2,HugeInt *p3) { Put(p3); Put(p2); Put(p1); } // preserve order
    void PutPool(Pool &p) {   while(p.Count()) Put(p.Get()); }
    bool InPool(HugeInt &Value);
    bool InPool(HugeInt &Value1,HugeInt &Value2);
    bool InPool(HugeInt &Value1,HugeInt &Value2,HugeInt &Value3);
    void PutTail(HugeInt *p);
    unsigned int Count() const { return _Count; }
    HugeInt * operator[](unsigned int i) { HugeInt *r = pPool; while(i--) r = r->GetPointer(); return r; }
};

class Primes
{
Pool _Primes;
  bool  Div(HugeInt &i);
public:
    Primes(char *Number);
    HugeInt *operator[](unsigned int i) { return _Primes[i]; }
};

#define NumBits ((unsigned int) (NumDigits * 32))

class FindFactor
{
HugeInt *p,*pTemp;
public:
Pool CurrentResult,LowerNums;
HugeInt HIBase,Target;
//int Base;
unsigned int exp;
private:
unsigned int l;
Pool Scrap;
HugeInt TargetDigitsBase;
unsigned int TargetDigits[7];//NumBits];
    void CalcTargetDigits();
public:
Pool FinalResults;
    FindFactor()
    {
        exp = 1;
//        Base = 2;
        HIBase = 2;
        Target.SetValue("188198812920607963838697239461650439807163563379417382700763356422988859715234665485319060606504743045317388011303396716199692321205734031879550656996221305168759307650257059");
        LowerNums.Put(Scrap.Get(),Scrap.Get());// put two 0's in to prime the pump
        TargetDigitsBase = 0;
    }
    void FindMults();
    void Step();
    void SetBase(HugeInt n) { /*Base = n;*/ HIBase = n; }
};

};
#endif  //THugeInt

