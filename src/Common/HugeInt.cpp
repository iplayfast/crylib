#pragma inline
#include <stdio.h>
#include <string.h>
#include "HugeInt.h"

#include "ClassString.h"
#include "ClassProperty.h"
#include "ClassException.h"

using namespace Crystal;

#define SBS 1024
#ifdef VALIDATING
bool HugeInt::Test(bool Verbose,Object &ThisObject,bool  (CallBack)(bool Verbose,const char *Result,bool fail))
{
	bool fail = false;
	if (!Object::Test(Verbose,ThisObject,CallBack))
        return true;
    HugeInt m,n;
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

void HugeInt::SetNumDigits(unsigned int n)
{
  if (NumDigits==n) return;
  if (NumDigits>n)  // we are reducing the size of a number so zero out anything that is above the size
  {
    for(unsigned int i=0;i<(NumDigits-n);i++)
      Number[i] = 0;
    return;
  }
    unsigned *nn = new unsigned [n];
    if (!nn)
        throw Exception(this,"Out of memory Creating HugeInt");
    for (unsigned int i=0;i<n-NumDigits;i++)
        nn[i] = 0;
    for(unsigned int j=0,i=n-NumDigits;i<n;i++,j++)
    {
        nn[i] = Number[j];
    }
    if (NumDigits)
        delete []Number;
    Number = nn;
    NumDigits = n;
    SetFlags();
}

void HugeInt::CopyTo(Object &Dest) const  //copies contents of this to Dest
{
    if (Dest.IsA(CHugeInt))
    {
        HugeInt *h = (HugeInt *)&Dest;
        *h = *this;
        return;
    }
    if (Dest.IsA(CStream))
    {
        SaveTo(*(Stream *)&Dest);
        return;
    }
    throw Exception(this,"Can't copy HugeInt to %s",Dest.ChildClassName());
}

Object *HugeInt::Dup() const // creates a duplicate of this object
{
    HugeInt *h = new HugeInt();
    *h = *this;
    return h;
}

const char *HugeInt::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (PropertyName=="Value")
	{
		return GetValue(Result);
	}
	return Object::GetProperty(PropertyName,Result);
}
bool HugeInt::HasProperty(const PropertyParser &PropertyName)const
{
	return (PropertyName=="Value") || Object::HasProperty(PropertyName);
}

PropertyList *HugeInt::PropertyNames() const
{
	PropertyList *n = Object::PropertyNames();
	n->AddPropertyByName("Value",this);
	return n;
}
int HugeInt::GetPropertyCount() const
{
    return 1 + Object::GetPropertyCount();
}
bool HugeInt::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Value")
	{
		this->SetValue(PropertyValue);
		return true;
	}
	return Object::SetProperty(PropertyName,PropertyValue);
}
void FindFactor::Step()
{
	HugeInt n;
	CalcTargetDigits();
	LowerNums.PutPool(CurrentResult);
	FindMults();
    exp++;
}

void FindFactor::CalcTargetDigits()
{
    if (TargetDigitsBase!=HIBase)
    {
        Target.GetDigits(0,Target.GetNumDigits() * 32,HIBase,TargetDigits);
        TargetDigitsBase = HIBase;
    }
}

void FindFactor::FindMults()
{
    HugeInt i,k,*n,*d,t;
    HugeInt exp10 = 1;
    HugeInt c;
    HugeInt AddVal,TopVal;
    HugeInt LocalTarget;
    for(unsigned int I=0;I<exp;I++)
    {
        AddVal = exp10;
        if (HIBase!=0)
            exp10.Mult(HIBase);
        else
            exp10.Shl32();
    }
    TopVal = AddVal;
    if (HIBase!=0)
        TopVal.Mult(HIBase);
    else
        TopVal.Shl32();
    HugeInt *FirstMult,*SecondMult;
    n = Scrap.Get();
    while(LowerNums.Count())
    {
        FirstMult = LowerNums.Get();
        SecondMult = LowerNums.Get();
        i.ZeroOut();
        for(unsigned int I=0;HIBase >= I; I++,i.Add(AddVal))
        {
            *n = i;
            n->Add(*FirstMult);
            k.ZeroOut();
            d = Scrap.Get();
            for(HugeInt K=0;HIBase>=K;K.Inc(),k.Add(AddVal))
            {
                *d = k;
                d->Add(*SecondMult);
                t = *n;
                t.Mult(*d);
                if (t>Target)
                    break;
                if (t.GetDigit(exp,HIBase) == TargetDigits[exp-1])//Target.GetDigit(exp,Base))
                {
                    if (t==Target)
                    {
                        HugeInt *f = Scrap.Get();
                        *f = *d;
                        FinalResults.Put(f);
                        f = Scrap.Get();
                        *f = *n;
                        FinalResults.Put(f);
                    }
                    if (!CurrentResult.InPool(*d,*n))
                    {
                        HugeInt *Temp = Scrap.Get();
                        *Temp = *d;
                        CurrentResult.Put(Temp);
                        Temp = Scrap.Get();
                        *Temp = *n;
                        CurrentResult.Put(Temp);
                    }
                }
            }
        }
    }
    Scrap.Put(n);
}

//=======================================================================================================

static char StrBuffer[SBS];

HugeInt *Pool::Get()
{
    if (pPool == 0)
    {
        pPool = new HugeInt();
        pPool->SetPointer(0);
        _Count=1;
    }
    HugeInt *r = pPool;
    pPool = r->GetPointer();
    r->SetPointer(0);
    _Count--;
    if (_Count==0)
        memset(HashArray,0,HASHSIZE);
    return r;
}

void Pool::Put(HugeInt *p)
{
    if (p)
    {
        SetHash(p);
        p->SetPointer(pPool);
        pPool = p;
        _Count++;
    }
}

bool Pool::CheckHash(HugeInt *p)
{
    unsigned int Value = p->GetBottomDigit();
    unsigned int HashValue = (Value / 32) % HASHSIZE;
    return HashArray[HashValue] & BitArray[Value & 31];
}
void Pool::SetHash(HugeInt *p)
{
    unsigned int Value = p->GetBottomDigit();
    unsigned int HashValue = (Value / 32) % HASHSIZE;
    HashArray[HashValue] |= BitArray[Value & 31];
}

void Pool::PutTail(HugeInt *p)
{
    HugeInt *pp = pPool;
    if (p)
    {
        SetHash(p);
        p->SetPointer(0L);
        while(pp->GetPointer())
            pp = pp->GetPointer();
        pp->SetPointer(p);
        _Count++;
    }
}

bool Pool::InPool(HugeInt &Value)
{
    HugeInt *p1 = pPool;
    if (CheckHash(&Value))
        while(p1)
        {
            if (*p1==Value)
                return true;
            p1 = p1->GetPointer();
        }
    return false;
}

bool Pool::InPool(HugeInt &Value1,HugeInt &Value2)
{
    HugeInt *p1 = pPool;
    if (CheckHash(&Value1))
        while(p1 && p1->GetPointer())
        {
            if ((*p1==Value1) && (*(p1->GetPointer())==Value2))
                return true;
            p1 = p1->GetPointer();
        }
    return false;
}

bool Pool::InPool(HugeInt &Value1,HugeInt &Value2,HugeInt &Value3)
{
    HugeInt *p1 = pPool;
    if (CheckHash(&Value1))
        while(p1 && p1->GetPointer() && p1->GetPointer()->GetPointer())
        {
            if ((*p1==Value1) && (*(p1->GetPointer())==Value2) && *(p1->GetPointer()->GetPointer())==Value3)
                return true;
            p1 = p1->GetPointer();
        }
    return false;
}


//================================================================================

String &HugeInt::GetValue(String &Result) const
{
    Result.Clear();
    HugeInt Divisor,Quotient,Remainder,Temp = *this;
    char Buff[2];
    Buff[1] = '\0';
    Divisor = 10;
    if (Temp.IsZero())
        Result = "0";
    while(!Temp.IsZero())
    {

        Temp.Div(Divisor,Quotient,Remainder);
        int i = Remainder.GetBottomDigit();
        Buff[0] = '0'+i;
        Result += Buff;
        Temp = Remainder;
    }
    return Result;
}
HugeInt &HugeInt::SetValue(const char *str)
{
	char ch;
	HugeInt M = 10;
	ZeroOut();
	const char *Orgstr = str;
	while(*str)
	{
		if (*str<'0' || *str>'9')
			throw Exception(this,"Bad format for HugeInt ",Orgstr);

		ch = *str - '0';
		M = *this;
		// Mult by 10 = (M*2*2+M)*2
		Shl1();	// *2
		Shl1(); // *2
		Add(M); // add original value
		Shl1(); // *2

		Add(ch);	// Add new digit
		str++;
	}
	return *this;
}

HugeInt &HugeInt::Mult(const HugeInt &m)
{
    HugeInt t = *this;
    unsigned int i;
//    bool first = false;
    if (&m == this)
        return Mult(t);
    if (m.IsOne() || IsZero())
        return *this;  // if I'm 0 return me cause 0 * N = 0
    if (m.IsZero() || IsOne())                // if I'm 1 return N cause 1 * N = N
    {
        *this = m;
        return *this;
    }
    if (m.IsTwo())
    {
        Shl1();
        return *this;
    }
    if (IsTwo())
    {
        *this = m;
        Shl1();
        return *this;
    }
    ZeroOut();
HugeInt nm = m;
    while(m.Number[m.NumDigits-1]==0)
    {
      nm.Shr32();
      t.Shl32();
    }
//    for(i=(m.FirstDigit)*32 ;i<NumBits;i++) // the first bit of the the multipliers first digit,
    for(i=nm.FirstBit();i<NumBits;i++) // the first bit of the the multipliers first digit,
    {
      if (nm.IsOdd())   // 1 * N = N
          Add(t);     // flags are set here as well

      if (!nm.Shr1())    // is m == 0
          return *this; // yes
      t.Shl1();
    }
    SetFlags(); // should never get here
    return *this;
}
// returns true if non-zero
bool HugeInt::Shl1()
{
    if (IsZero())
        return false;
    unsigned int *Digit,*EndDigit;
    //bool cy;
    if ((FirstDigit==0) && ((Number[FirstDigit] & HighDigitBit)!=0))
    {
        SetNumDigits(NumDigits+1);
        //FirstDigit++;
    }
    EndDigit = &Number[NumDigits-1];
    Digit = &Number[FirstDigit];
    if (Digit==EndDigit)
    {
        if ((*Digit & HighDigitBit)!=0)
        {
            FirstDigit--;
            Number[FirstDigit] = 1;
        }
    }
    while(Digit < EndDigit)
    {
        *Digit = (*Digit<<1);
        if ((Digit[1] & HighDigitBit))	// if the next one has the high bit set
            *Digit|=1;			//  add 1
        Digit++;
    }
    *Digit = (*Digit<<1);
    return SetFlags();
}

// returns true if non zero
bool HugeInt::Shl32()
{
    unsigned int *Digit;
    unsigned int t = 0;
    if ((FirstDigit==0) && (Number[FirstDigit]!=0))
    {
        SetNumDigits(NumDigits+1);
        FirstDigit++;
    }
    if (FirstDigit)
        FirstDigit--;	// setup for final result
    Digit = &Number[FirstDigit];
    while(Digit<&Number[NumDigits-1])
    {
        t |= *Digit = Digit[1];
        Digit++;
    }
    *Digit = 0L;
    if (t==0)
        Flags = ISZERO | ISMOSTLYZERO;
    else
        Flags = 0;
    return Flags != ISZERO;
}


HugeInt &HugeInt::Shl(unsigned int n)
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
// returns true if non-zero
bool HugeInt::Shr1()
{
    if (IsZero())
        return false;
    unsigned int F = Flags;
    unsigned int *Digit = &Number[NumDigits-1];
    if (F & (ISONE | ISTWO))
    {
        *Digit = *Digit >> 1;
        return SetFlags(*Digit);
    }
    while(Digit>&Number[FirstDigit])
    {
        *Digit = *Digit >> 1;
        Digit--;
        if (*Digit & 0x00000001)
            Digit[1] |= HighDigitBit;
    }
    *Digit = *Digit >> 1;
    return SetFlags();
}
// returns true if non-zero
bool HugeInt::Shr32()
{
    unsigned int *Digit = &Number[NumDigits-1];
    unsigned int *First = &Number[FirstDigit];
    unsigned int F = 0L;
    if (Flags & (ISZERO | ISMOSTLYZERO)==0)
    {
        *Digit = 0;
        Flags = ISZERO;
        return false;
    }
    while(Digit >First)
    {
        F |= Digit[0] = Digit[-1];
        Digit--;
    }
    Digit[0] = 0L;
    if (FirstDigit<NumDigits-1)
      FirstDigit++;
    if (F)
    {
        Flags = 0;   // one of the digits had a value reset flags
        return true;
    }
    else
    {
        Flags = ISZERO | ISMOSTLYZERO;
        return false;
    }
}

HugeInt &HugeInt::Shr(unsigned int n)
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

bool HugeInt::Mult10()
{
    HugeInt t = *this;
    Shl1();
    Shl1();
    Add(t);
    Shl1();
    return SetFlags();
}
HugeInt &HugeInt::Inc(unsigned int Amount)	// need to test
{
    unsigned int cy=Amount,*d1 = &Number[NumDigits-1],temp;
    while(d1>=Number)
    {
        temp =  *d1;
        *d1 += cy;
        if (temp <= *d1)
            break;
        d1--;
        cy = 1;
        //        *d1 = *d1 + cy;
        //asm jc CarryLable:
        //        SetFlags();
        //        return *this;
        //        CarryLable:
        //        d1--;
    }
    SetFlags();
    return *this;
}
HugeInt &HugeInt::Dec(unsigned int Amount)// Need Testing
{
    unsigned int cy=Amount,*d1 = &Number[NumDigits-1],temp;
    while(d1>=Number)
    {
        temp = *d1;
        *d1 -= cy;
        if (temp >= cy)
            break;
        d1--;
        cy = 1;
        //	 *d1 = *d1 - cy;
        //asm jc CarryLable:
        //SetFlags();
        //                return *this;
        //        CarryLable:
        //                d1--;
    }
    SetFlags();
    return *this;
}
HugeInt &HugeInt::IncDigit(unsigned int d)
{
    d--;
    do
    {
        d++;
        Number[NumDigits - d]++;
    }
    while (Number[NumDigits -d]==0);
    SetFlags();
    return *this;
}
HugeInt &HugeInt::DecDigit(unsigned int d)
{
    if (d>NumDigits)
        return *this;
    bool cy;
    do
    {
        cy = Number[NumDigits - d]==0;
        Number[NumDigits - d]--;
        d++;
    }
    while(cy);
    SetFlags();
    return *this;
}

HugeInt &HugeInt::Add(const HugeInt &n)
{
#ifdef Verify
    HugeInt Org = *this;
#endif

    if (&n==this)
    {
        HugeInt m = n;
        return Add(m);
    }

    // Insure there is space for the result
    {
    unsigned int MaxDigitNeeded = n.NumDigits;
      if (n.Number[0]!=0) MaxDigitNeeded++;
      if (NumDigits >= MaxDigitNeeded)
      {
         MaxDigitNeeded = NumDigits;
        if (Number[0]!=0) MaxDigitNeeded++;
      }
      if (NumDigits<MaxDigitNeeded)
        SetNumDigits(MaxDigitNeeded);
    }

    unsigned int cy=0,
                *Stop,
                *d1 = &Number[NumDigits-1],*d2 = &n.Number[n.NumDigits-1],temp;
    unsigned int FirstDigitM1; // last digit to add (first from the left)

    FirstDigitM1 = FirstDigit-1;  // first digit must be at least one, (because otherwise it would have been expanded above)
    if (n.FirstDigit<FirstDigit)
      FirstDigitM1 = n.FirstDigit-1;
    
    Stop = &Number[FirstDigitM1];
    while(1)
    {
        if (cy)
        {
          temp = *d1;
          *d1 +=  cy;
          if (temp > *d1)
              cy = 1;
          else
            cy = 0;
        }
        temp = *d1;
        *d1 += *d2;
        if (temp > *d1)
            cy = 1;
        if (d1==Stop)  // at first digit
          break;
        d1--;
        if (d1==Stop)
        {
          if (cy)
            *d1 += cy;
          break;
        }
        d2--;
    }
#ifdef Verify
    HugeInt v = *this;
    v.Sub(n);
    if (v!=Org)
        throw;
#endif

    SetFlags();
    return *this;
}
HugeInt &HugeInt::Sub(const HugeInt &n)
{
    if (&n==this)
    {
        ZeroOut();
        return *this;
    }
    if (NumDigits<n.NumDigits)
      SetNumDigits(n.NumDigits);
    unsigned int cy=0,*d1 = &Number[NumDigits-1],*d2 = &n.Number[n.NumDigits-1],temp;
    unsigned int *stop = &n.Number[n.FirstDigit];
    while(d1>=Number)
    {
        temp = *d1;
        *d1 -= cy;
        if (temp < *d1)
            cy = 1;
        else
            cy = 0;
        temp = *d1;
        *d1 -= *d2;
        if (temp < *d1)
            cy = 1;
        if (d1>Number) // avoid codeguard pointer underrun warning
        {
          d1--;
          if (d2==stop)
          {
            if (cy) *d1 -= cy;
            break;
          }
          d2--;
        }
        else break;
    }
    SetFlags();
    return *this;
}

HugeInt &HugeInt::Add(const unsigned int &n)
{
    unsigned int cy=0,*d1 = &Number[NumDigits-1],d2,temp;
    d2 = n;
    do
    {
        temp = *d1;
        *d1 += cy;
        if (temp > *d1)
            cy = 1;
        else
            cy = 0;
        temp = *d1;
        *d1 += d2;
        if (temp > *d1)
            cy = 1;
        d1--;
        d2 = 0L;
    }
    while(cy!=0);
    SetFlags();
    return *this;
}
HugeInt &HugeInt::Sub(const unsigned int &n)
{
    unsigned int cy=0,*d1 = &Number[NumDigits-1],d2,temp;
    d2 = n;
    do
    {
        temp = *d1;
        *d1 -= cy;
        if (temp < *d1)
            cy = 1;
        else
            cy = 0;
        temp = *d1;
        *d1 -= d2;
        if (temp < *d1)
            cy = 1;
        d1--;
        d2 = 0L;
    }
    while(cy!=0);
    SetFlags();
    return *this;
}
bool HugeInt::Div(HugeInt &Divisor,HugeInt &Quotient,HugeInt &Remainder)
{
    return Div(*this,Divisor,Quotient,Remainder);
}

unsigned int HugeInt::FirstBit() const
{
    unsigned int FirstBit = 0;
    const unsigned int *pNumber = &Number[0];
    if (IsZero())
        return 0;
    unsigned int i;
    i = FirstDigit;
    FirstBit = i * 32;
    pNumber = &Number[i];
    for(;i<NumDigits;i++)
    {
        if (*pNumber!=0L)
        {
        unsigned int ByteMask = 0xff000000;
        unsigned int BitMask =  0x80000000;
          while(1)
          {
              if ((*pNumber & ByteMask)!=0)
              {
                  while ((*pNumber & BitMask)==0)
                  {
                      FirstBit++;
                      BitMask >>=1;
                  }
                  return FirstBit;
              }
              FirstBit +=8;
              ByteMask >>=8;
              BitMask >>=8;
          }
        }
        else  // shouldn't ever get here
            FirstBit += 32;
        pNumber++;
    }
    return 0;   // shouldn't get here
}

bool HugeInt::Div(HugeInt &Dividend, HugeInt &Divisor,HugeInt &Quotient,HugeInt &Remainder) const
{
    unsigned int num_bits,q,bit;
    unsigned int i;
    HugeInt d;
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
    //    while(IsEven() && Divisor.IsEven())
    //    {
    //        Shr1();
    //        Divisor.Shr1();
    //    }
    HugeInt OrgDivisor = Divisor;
    HugeInt ShiftDivisor = Divisor;
    unsigned int DividendFirstBit = Dividend.FirstBit();
    unsigned int DivisorFirstBit=  Divisor.FirstBit();
    if (DivisorFirstBit>DividendFirstBit)
    {
      ShiftDivisor.Shl(DivisorFirstBit-DividendFirstBit);
      DivisorFirstBit = DividendFirstBit;
    }
    Remainder = Dividend;
    while(ShiftDivisor >=OrgDivisor)
    {
        if (!Quotient.IsZero())
            Quotient.Shl1();
        if (Remainder >= ShiftDivisor)
        {
            Remainder.Sub(ShiftDivisor);
            Quotient.Number[Quotient.NumDigits-1] |= 1;
            if (Quotient.IMZ())
                Quotient.SetFlags(Quotient.Number[Quotient.NumDigits-1]);
            else
                Quotient.Flags = 0;
        }
        ShiftDivisor.Shr1();
    }
    return false;
    //-----------------------------------
}

const char *HugeInt::GetAsStr(unsigned int Base) const
{
    char *s = &StrBuffer[SBS-1];
    HugeInt Dividend,Divisor,Remainder,Quotient;
    memset(StrBuffer,' ',SBS-1);
    Dividend = *this;
    *s-- = '\0';
    if (Base==10)
    {
        while(!Dividend.IsZero())
        {
            Divisor = Base;
            Div(Dividend,Divisor,Quotient,Remainder);
            *s = Remainder.Number[NumDigits-1] + '0';
            s--;
            Dividend = Quotient;
        }
        //    Huge2Str(StrBuffer);
        s++;
        if (*s=='\0')
        {
            s--;
            *s = '0';
        }
        return s;
    }
    char FinalOut[1024];
    FinalOut[0] = '\0';
    while(!Dividend.IsZero())
    {
        char buff[80];
        Divisor = Base;
        Div(Dividend,Divisor,Quotient,Remainder);
        strcat(FinalOut,Remainder.GetAsStr());
        strcat(FinalOut,",");
        Dividend = Quotient;
    }
    {
        StrBuffer[0] = '\0';
        char *ch = &FinalOut[strlen(FinalOut)-1];
        while(ch>FinalOut)
        {
            *ch = '\0';
            while((ch>FinalOut) && (*ch!=','))
                ch--;
            if (ch==FinalOut)
                strcat(StrBuffer,",");
            strcat(StrBuffer,ch);
        }
    }
    return StrBuffer+1;
}
void HugeInt::Huge2Str(char *b)
{
    char Buffer[20];
    bool dd =false;

    strcpy(StrBuffer,"0x");
    for(unsigned int i=0;i<NumDigits;i++)
    {
        if (Number[i]!=0L)
        {
            sprintf(Buffer,"%lX",Number[i]);
            strcat(StrBuffer,Buffer);
            dd = true;
        }
    }
    if (!dd)
        strcat(StrBuffer,"0");
    strcpy(b,StrBuffer);
}
unsigned int HugeInt::GetDigit(unsigned int Digit,unsigned int Base) const
{
    if (Base==0)
        return GetDigit(Digit);

    if (IMZ())
    {
        unsigned int Value = GetBottomDigit();
        while(--Digit)
        {
            Value /= Base;
        }
        return Value % Base;
    }

    HugeInt t = *this;
    HugeInt q,r,HIBase = Base;

    while(Digit--)
    {
        Div(t,HIBase,q,r);
        HIBase = Base;
        t = q;
    }
    return r.GetDigit(1);
}

HugeInt HugeInt::GetDigit(unsigned int Digit,const HugeInt &Base) const
{
    if (Base.IsZero())
        return *this;

    HugeInt t = *this;
    HugeInt q,r,HIBase = Base;

    while(Digit--)
    {
        Div(t,HIBase,q,r);
        HIBase = Base;
        t = q;
    }
    return r;
}

void HugeInt::GetDigits(unsigned int start,unsigned int end,HugeInt Base,unsigned int *Digits) const
{
    while(start)
    {
        start--;
        end--;
        Digits++;
    }
    HugeInt t = *this;
    HugeInt q,r,HiBase = Base;
    while(end)
    {
        Div(t,HiBase,q,r);
        HiBase = Base;
        t = q;
        *Digits++=r.GetDigit(1);
        end--;
    }
}



bool Primes::Div(HugeInt &i)
{
    unsigned int j;
    HugeInt Quot,Rem;
    HugeInt *pp = _Primes[0];
    HugeInt Div2;
    Quot =2; // just temporary
    i.Div(i,Quot,Div2,Rem);
    while(pp)
    {
        if (*pp<=Div2)
        {
            i.Div(i,*pp,Quot,Rem);
            if (Rem==0)
                return true;
        }
        pp = pp->GetPointer();
    }
    return false;
}
Primes::Primes(char *Number)
{
    HugeInt n;
    HugeInt *i = new HugeInt();
    HugeInt Test;
    Test = 5147;
    n.SetValue(Number);
    *i = 2;
    _Primes.Put(i);
    i = new HugeInt();
    *i = 3;
    HugeInt Add;
    Add = 2;
    while(*i < n)
    {
        if (!Div(*i))
        {
            HugeInt *ii = new HugeInt(*i);
            _Primes.PutTail(ii);
        }
        i->Inc(2);
    }
}
//==========================================================

HugeInt Power(HugeInt x,HugeInt n)
{
    HugeInt r(1);
    HugeInt y(x);
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
HugeInt MulMod(HugeInt x,HugeInt y,HugeInt z)
{
    HugeInt d1,d2;
    x.Mult(y);
    x.Div(z,d1,d2);
    return d2;
}
// a^b % m
HugeInt ModExp(HugeInt a ,HugeInt b,HugeInt m)
{
    HugeInt n = 1;
    HugeInt d1,d2;//dummy
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

bool TestForPrime(HugeInt n,HugeInt Base)
{
    HugeInt t = n;
    t.Dec();
    if (t.IsZero())
        return false;
    HugeInt nMinus1 = t;
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

    HugeInt test = ModExp(Base, t, n);
    if( test.IsOne() || test == nMinus1 )
        return true;
    HugeInt d1,d2;
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





/*
Thoughts, if we find a prime contained within the target that will reduce the possibilities
to 2 at that point. We can also test various bases (since we are dealing with digits and not
the whole number).
function IsPrime(ByRef pLngNumber)
    	Dim lLngSquare
    	Dim lLngIndex
    	IsPrime = False
    if pLngNumber < 2 Then Exit function
    if pLngNumber Mod 2 = 0 Then Exit function
    lLngSquare = Sqr(pLngNumber)
    For lLngIndex = 3 To lLngSquare Step 2
    if pLngNumber Mod lLngIndex = 0 Then Exit function
    Next
    	IsPrime = True
    End function
 
 
Finding square roots of of numbers that aren't perfect squares without a calculator
 
1. Estimate - first, get as close as you can by finding two perfect square roots your number is between.
 
2. Divide - divide your number by one of those square roots.
 
3. Average - take the average of the result of step 2 and the root.
 
4. Use the result of step 3 to repeat steps 2 and 3 until you have a number that is accurate enough for you.
 
 
    
*/
