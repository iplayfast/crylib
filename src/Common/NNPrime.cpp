//---------------------------------------------------------------------------

#pragma hdrstop
#include <stdio.h>
#include "CryBackProp.h"
#include "ClassFileStream.h"
#include "CryFuzzy.h"
#include "CryXML.h"
#include "CryNN.h"
#include "HugeInt.h"
#include "PrimeTab.h"
#include "Utility.h"
//---------------------------------------------------------------------------
using namespace std;
using namespace Crystal;


// NumDigits is the max number of hugeint digits

// WorkingBase is the base of the input digits
#define WorkingBase 10
// assuming a long is 32 bits, working base is 2, then InputsPerDigit is 32
#define InputsPerDigit 4
#define MaxHugeIntBits (DefNumDigits * InputsPerDigit)

double dResults[100][64*MaxHugeIntBits];
double dTotal[100][32*MaxHugeIntBits];

HugeInt SomePrimes[2095];

int SomePrimesCount;
void LoadSomePrimes()
{
CryFileStream f;
String s;
    f.Open("primes.txt","r");
    f.SetTerminator('\n');
int i=0;
    for(i=0;i<1000;i++)
        SomePrimes[i] =(long) primeTab[i];
    while(!f.Eof())
    {
        f.ReadTI(&s,(size_t)1000);
        if (s[s.Length()-1]=='\n')
            s.Delete(s.Length()-1,1);
        const char *ch = s;
        SomePrimes[i++].SetValue(ch);
    }
    SomePrimesCount = i;
}

#pragma argsused
int main(int argc, char* argv[])
{
{
HugeInt n,d;
  n.SetValue("123456789123456789");
  printf("%s * \n",n.GetAsStr());
//d.SetValue("100000000000000000");
  d.SetValue("281474976710656");
  printf("%s\n==========\n",d.GetAsStr());

  n.Mult(d);
  printf("\n%s ",n.GetAsStr());
}

    LoadSomePrimes();
CryBPNetContainer bp;
    bp.AddLayer(MaxHugeIntBits);   // input is digits
    bp.AddLayer(MaxHugeIntBits);
    bp.AddLayer(MaxHugeIntBits * 2);
    bp.SetAlpha(0.9);
    bp.SetEta(0.25);
    bp.SetGain(1);
    bp.RandomWeights();
    for(int i=0;i<SomePrimesCount;i++)
        for(int j=0;j<SomePrimesCount;j++)
        {
          for(int p=0;p<100;p++)
          {
          int n,d;
            n = RandomInt(0,SomePrimesCount);
            d = RandomInt(0,SomePrimesCount);
            HugeInt Num;  Num.Add(SomePrimes[n]);
            HugeInt Den;  Den.Add(SomePrimes[d]);
            HugeInt Total;
            Total = Num;
            Total.Mult(Den);
            {
            char Buff1[1200];
            char Buff2[1200];
            char Buff3[1200];
                strcpy(Buff1,Num.GetAsStr());
                strcpy(Buff2,Den.GetAsStr());
                strcpy(Buff3,Total.GetAsStr());
            printf("%d %s \n%d %s\n==\n%d %s\n",strlen(Buff1),Buff1,strlen(Buff2),Buff2,strlen(Buff3),Buff3);
            }
            for(int k=0;k<MaxHugeIntBits;k++)
            {
                dTotal[p][k]= Total.GetDigit(k+1,WorkingBase);
                dResults[p][k] = Num.GetDigit(k+1,WorkingBase);
                dResults[p][k *2 ] = Den.GetDigit(k+1,WorkingBase);
            }
          }
          bp.STTrainNet(5000,100,(double *)dTotal,100,(double *)dResults);
          bp.printWeights();
        }
    return 0;
}
//---------------------------------------------------------------------------
