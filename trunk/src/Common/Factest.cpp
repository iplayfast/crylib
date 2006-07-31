

//
//
//Includes
//
//

#include <stdio.h>
#include <string.h>



//
//
//Target
//
//

char *Target = "1591";



//
//
//Global Variables
//
//

//Numerators
int Numerators[3];


//Denominators
int Denominators[3];


//Results
int Results[6];
// Lookup instead of multiply
int lookup[10][10];// Array of digit test functions
bool (*TestArray[6])();
// Array of Carry calculate functions
int (*CarryArray[6])();

int NDigitFactors[10][10];
int DDigitFactors[10][10];
int LowestDFactor[10];


//
//
//Row Col functions for each numerator and denominator
//
//


int RowColn1d1(int Numerators1,int Denominators1)
{
	return Numerators1 * Denominators1;
}
int RowColn2d1(int Numerators2,int Denominators1)
{
	return Numerators2 * Denominators1;
}
int RowColn3d1(int Numerators3,int Denominators1)
{
	return Numerators3 * Denominators1;
}
int RowColn1d2(int Numerators1,int Denominators2)
{
	return Numerators1 * Denominators2;
}
int RowColn2d2(int Numerators2,int Denominators2)
{
	return Numerators2 * Denominators2;
}
int RowColn3d2(int Numerators3,int Denominators2)
{
	return Numerators3 * Denominators2;
}
int RowColn1d3(int Numerators1,int Denominators3)
{
	return Numerators1 * Denominators3;
}
int RowColn2d3(int Numerators2,int Denominators3)
{
	return Numerators2 * Denominators3;
}
int RowColn3d3(int Numerators3,int Denominators3)
{
	return Numerators3 * Denominators3;
}

//
//
//Sum functions for each Col
//
//


int Sum1()
{

	return RowColn1d1(Numerators[0],Denominators[0]);
}

int Sum2()
{

	return RowColn2d1(Numerators[1],Denominators[0])+RowColn1d2(Numerators[0],Denominators[1]);
}

int Sum3()
{

	return RowColn3d1(Numerators[2],Denominators[0])+RowColn2d2(Numerators[1],Denominators[1])+
		RowColn1d3(Numerators[0],Denominators[2]);
}

int Sum4()
{

	return RowColn3d2(Numerators[2],Denominators[1])+
		RowColn2d3(Numerators[1],Denominators[2]);
}

int Sum5()
{

	return 
		RowColn3d3(Numerators[2],Denominators[2]);
}

int Sum6()
{

	return 0;
}


//
//
//Carry Functions for each col
//
//


int Carry1()
{
	return 0;
}

int Carry2()
{
	return Sum1() / 10;
}

int Carry3()
{
	return Sum2() / 10;
}

int Carry4()
{
	return Sum3() / 10;
}

int Carry5()
{
	return Sum4() / 10;
}

int Carry6()
{
	return Sum5() / 10;
}

int Carry7()
{
	return Sum6() / 10;
}


//
//
//Digit Functions for each col
//
//


int Digit1()
{
return Sum1() % 10;
}

int Digit2()
{
return (Carry2() + Sum2()) % 10;
}

int Digit3()
{
return (Carry3() + Sum3()) % 10;
}

int Digit4()
{
return (Carry4() + Sum4()) % 10;
}

int Digit5()
{
return (Carry5() + Sum5()) % 10;
}

int Digit6()
{
return (Carry6() + Sum6()) % 10;
}


//
//
//Test Functions for each col
//
//

bool Test1()
{
 return Digit1()==Results[0];
}
bool Test2()
{
 return Digit2()==Results[1];
}
bool Test3()
{
 return Digit3()==Results[2];
}
bool Test4()
{
 return Digit4()==Results[3];
}
bool Test5()
{
 return Digit5()==Results[4];
}
bool Test6()
{
 return Digit6()==Results[5];
}


//
//
//Setup
//
//

void Setup()
{
for(size_t c=0,i=0;i<6;i++)
{
	Results[i] = 0;
	if (!(strlen(Target)<6-i))
		Results[c++] = Target[5-i]-'0';
}
	TestArray[0] = Test1;
	CarryArray[0] = Carry1;
	TestArray[1] = Test2;
	CarryArray[1] = Carry2;
	TestArray[2] = Test3;
	CarryArray[2] = Carry3;
	TestArray[3] = Test4;
	CarryArray[3] = Carry4;
	TestArray[4] = Test5;
	CarryArray[4] = Carry5;
	TestArray[5] = Test6;
	CarryArray[5] = Carry6;

int i,j,k;
    for(i=0;i<10;i++)
        for(j=0;j<10;j++)
            NDigitFactors[i][j] = DDigitFactors[i][j] = -1;
    for(i=0;i<10;i++)
    {
        LowestDFactor[i] = -1;
        for(j=0;j<10;j++)
            for(k=0;k<10;k++)
                if (k * j % 10 == i)
                {
                    if (LowestDFactor[i]==-1)
                        LowestDFactor[i] = k;
                    NDigitFactors[i][k] = j;
                    DDigitFactors[i][j] = k;
                }
    }
    for(i=1;i<3;i++)
        Denominators[i] = Numerators[i] = -1;
    Denominators[0] = LowestDFactor[Results[0]];
	Numerators[0] = NDigitFactors[Results[0]][Denominators[0]];

}

            void ShowPrint(int rindex)
            {
                if (rindex==0) return;
                printf("\n\t");
                for(int i=0;i<rindex;i++)
                    printf("%d",Numerators[rindex -1  - i]);
                printf(" X ");
                for(int i=0;i<rindex;i++)
                    printf("%d",Denominators[rindex -1  - i]);
                printf(" = ");
                for(int i=0;i<6;i++)
                    printf("%d",Results[5 - i]);
	            }


//
//
//Main
//
//

int main(int argv,char *argc[])
{
	Setup();
bool (*Digit)();
int (*Carry)();
int rindex;
bool recalc = false,done = false;
int n,d,r;
	rindex = 0;
	while(!done)
	{
	Digit = TestArray[rindex];
	Carry = CarryArray[rindex];
		r = (*Digit)();
		if (recalc || (r!=Results[rindex]))
		{
            recalc = false;
            r = Results[rindex] - (*Carry)();
            Numerators[rindex]++;
            if (Numerators[rindex]>9)
            {
                Numerators[rindex]=0;
                Denominators[rindex]++;
                if (Denominators[rindex]>9)
                {
                    Denominators[rindex] = 0;
                    rindex--;
                    ShowPrint(rindex);
                    recalc = true;
                }
            }
		}
        else
        {
            rindex++;
            ShowPrint(rindex);
            Denominators[rindex] =
	            Numerators[rindex] = 0;
            if (rindex>5)
                     ShowPrint(rindex);
	}
}
}
