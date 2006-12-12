//#ifdef HAVE_CONFIG_H
//#include <config.h>
//#endif

#include <iostream>
#include <cstdlib>
#include "CryBackProp.h"
#include "CryFuzzy.h"
#include "CryXML.h"
#include "CryNN.h"
#include "HugeInt.h"
#include "HugeDouble.h"
#include "ClassBuilder.h"
#include "CryPattern.h"
#include "ClassFileStream.h"

using namespace std;
using namespace Crystal;
#pragma hdrstop
//---------------------------------------------------------------------------

/// on return true, testing should continue
bool/* __cdecl */CallBack(bool Verbose,const char *Result,bool fail)
{
  printf("%s %s\n",Result,fail ? "Fail" : "Pass");
  fflush(stdout);
  if (fail)
  {
      printf("\n\ncontinue? (y/n)\n ");
      fflush(stdout);
      char ch[30];
      scanf("%s",ch);
      if (toupper(ch[0])=='Y') return true;
      else return false;
  }
  return true;
}
#pragma argsused
int TestFuzzyMain(int argc, char* argv[])
{
try
{
CryBPNetContainer bp;
#ifdef VALIDATING
String a;
//	a.Test(true,&a,CallBack);
	a.Test(true,a,CallBack);
	bp.Test(true,bp,CallBack);
#endif
//CryArray A;
	//A.AddOwned(new CryString("Hello");
HugeInt h;

	
}
catch(Exception &E)
{
	printf("\nException Caught: %s\n",(const char *)E);
}
/*{
int LayerSizes[3];
    LayerSizes[0] = 5;
    LayerSizes[1] = 50;
    LayerSizes[2] = 5;
CryBackProp bp(3,LayerSizes,1.0,2.0,3.0);

CryXML x;
    x.LoadFrom(&bp);
CryFileStream fs;
    fs.Open("backprop.xml","w");
    x.SaveTo(&fs);
}
*/
return 0;	// bp done
CryFuzzy f;
    f.AddPoint(4.0,-5.0);
    f.AddPoint(1.3,-6.0);
    f.AddPoint(4.0,-5.0);
    f.AddPoint(40.0,-50.0);
    f.AddPoint(400.0,-500.0);

{
List list;
List *l1 = new List;
    list.AddOwned(l1);
    l1->Add(&f);
CryFileStream fs;
    fs.Open("fuzzy.xml","w");
CryXML x("obj");
    x.LoadFrom(list);
    x.SaveTo(fs);
    fs.Close();
}

try
{
List l;
CryFileStream fs;
#ifdef VALIDATING
fs.Test(false,fs,CallBack);
l.Test(true,l,CallBack);
f.Test(true,f,CallBack);
#endif
}
catch(Exception &E)
{
    printf("\nException Caught: %s\n",(const char *)E);
    return 0;
}
//    f.SetName("Test Fuzzy");
    f.AddPoint(4.0,-5.0);
    f.AddPoint(1.3,-6.0);
    f.AddPoint(4.0,-5.0);
    f.AddPoint(40.0,-50.0);
    f.AddPoint(400.0,-500.0);
{
CryFileStream fs;
    fs.Open("fuzzy.xml","w");
CryXML x("obj");
    x.LoadFrom(f);
    x.SaveTo(fs);
	fs.Close();
}       // Test Fuzzy saved to fuzzy.xml
{
CryFileStream fs;
    fs.Open("fuzzy.xml","r");
CryXML x("obj");
    x.LoadFrom(fs);
   fs.Close();      // x has xml representation of Test Fuzzy
{
    CryFuzzy *nf  = (CryFuzzy *)x.CreateObjectFromNode();
    CryFileStream fs1;
    fs1.Open("testfuzzy.xml","w");
    CryXML nx("obj");
    nx.LoadFrom(*nf);
    delete nf;
    nx.SaveTo(fs1);
    fs1.Close();    // saved to testfuzzy.xml should be the same as fuzzy.xml
}
}
#ifdef __BORLANDC__
    cout << "Fuzzy Value are " << f << endl;
    f.Normalize(0.01);
    cout << "Fuzzy Value are " << f << endl;
    f.IncreaseSamples();
    cout << "Fuzzy Value are " << f << endl;
    f.IncreaseSamples();
    cout << "Fuzzy Value are " << f << endl;
    f.IncreaseSamples();
    cout << "Fuzzy Value are " << f << endl;
    f.Normalize(0.01);
    cout << "Fuzzy Value are " << f << endl;
CryFuzzy g(f);
    g.AddPoint(4.0,-4.0);
    cout << "Fuzzy Value are:" << endl <<
        "f:" << f << endl <<
        "g:" << g << endl;
    g.Range(0.0,1.0);
    f.Range(0.0,1.0);
	cout << "Fuzzy Value are :"<< endl << f << endl <<g<<endl;
    cout << g.Or(f,4.0);
    g.SwapXY();
    g.Range(0.0,1.0);
    g.SwapXY();
    f.SwapXY();
    f.Range(0.0,1.0);
    f.SwapXY();
    cout << "Fuzzy Value are :" << endl << f << endl <<g<<endl;
    cout << g.Or(f,4.0);
#endif //__BORLANDC__
    return 0;
}
//---------------------------------------------------------------------------
/*
                                           dcba
                                           hgfe
------------------------------------------
    111  1111  1111 1111 0111  0011
XWVU  TSRQ PONM LKJI HGFE DCBA



*/

void CreateAINETXML()
{
  CryBPNetContainer ai;
  ai.AddLayer(2); 	// 2 inputs
  ai.AddLayer(4);     // 2 calcs
  ai.AddLayer(1);     // 1 output
  ai.SetAlpha(0.9);
  ai.SetEta(0.25);
  ai.SetGain(1);
  ai.RandomWeights();
  {
  double _InData [4][2] = { {0,0},{0,1},{1,0},{1,1}};	//2 bit inputs
  double *InData = &_InData[0][0];
  double OutData[4];
  double TargetData[4] = {0,1,1,0};	// xor
    ai.STTrainNet(5000,4,InData,4,TargetData);
    CryFileStream fs;
        fs.Open("ainet.xml","w");
    CryXML x;
        x.LoadFrom(ai);
        x.SaveTo(fs);
        fs.Close();
  }
}


void Char2Bit(char *In,char *Out)
{
int i = 0x80;
  for(int j=0;j<8;j++)
  {
    if (*In & i) *Out++='1';
    else *Out++='0';
    i>>=1;
  }
}
char *CharStr2BitStr(char *In,int len)
{
char *Out = new char[len * 8+1];
char *Ret = Out;
  for(int i=0;i<len;i++)
  {
    Char2Bit(In++,Out);
    Out +=8;
  }
  Ret[len*8] = '\0';
  return Ret;
}
struct _Every
{
int N;
int Index;
int Use;
};

int sortfunction( const void *a, const void *b)
{
_Every *pa,*pb;
  pa = (_Every *)a;
  pb = (_Every *)b;
  return (pa->N - pb->N);
}

int Diff(char *s1,char *s2)
{
  int d = 0;
    while(*s1)
    {
      if (*s1!=*s2) d++;
      s1++;
      s2++;
    }
    return d;
}

int Apply(char *s1,char *s2,int n,bool flipfirst,bool testing)
{
int i=1,d = 0;
char *os1 = s1;
  while(*s1)
  {
    if ((i & n) || (flipfirst && (s1 ==os1)))
    {
    char ch = *s1;
      if (ch=='1') ch = '0';
      else ch = '1';
      if (*s2!=ch) d++;
      if (! testing)
        *s1 = ch;
	}
    else
    if (*s1!=*s2) d++;
    if (s1==os1 && d!=0) d+=10000;
    s1++; s2++;
    i++;
  }
  if (d<0) d = 0;
  return d;
}

void SetEvery(struct _Every *Every,char *cHash,int len)
{
int i;
  for(i=0;i<256;i++)
  {
    Every[i].N = 0;
    Every[i].Use = 0;
    Every[i].Index = i;
  }

  for(i=0;i<len*8;i++)
  {
    if (cHash[i]=='1')
    {
      for(int j=1;j<256;j++)
        Every[i & j].N++;
    }
  }
}

void calc(float *xValue,float *yValue,float *intercept,float *slope,int number)
{
	/* local variables */
	int count;
	float sumX = 0;
	float sumXSquared = 0;
	float sumY = 0;
	float sumXY = 0;

	/* perform calculations */
	for(count = 0; count <= number - 1; count ++)
	{
		sumX = sumX + xValue[count];
		sumXSquared = sumXSquared + xValue[count]*xValue[count];
		sumY = sumY + yValue[count];
		sumXY = sumXY + xValue[count]*yValue[count];
	}

	*intercept = sumXSquared * sumY - sumX * sumXY;
	*intercept = *intercept/(number * sumXSquared - sumX * sumX);

	*slope = sumXY - *intercept * sumX;
	*slope = *slope/sumXSquared;
}

void MakeHash(char *In,int len)
{
char *cHash = CharStr2BitStr(In,len);
char *nHash = new char[len*8+1];
char *dHash = new char[len*8+1];
int i;
float *xValue = new float[len*8+1];
float *yValue = new float[len*8+1];
float intercept;
float slope;
char *cH;
  cH = cHash;
  for(i=0;i<len*8;i++)
  {
    if (*cH=='1')
      yValue[i] = 1;
    else
      yValue[i] = 0;
    xValue[i] = i;
    cH++;
  }
   calc(xValue,yValue,&intercept,&slope,len*8);



struct _Every Every[256];
  for(i=0;i<len*8;i++)
    nHash[i] = '0';

  nHash[len*8] = '\0';
  dHash[len*8] = '\0';
  SetEvery(&Every[0],cHash,len);
  qsort(Every,256,sizeof(struct _Every),sortfunction);

int diff = Diff(nHash,cHash);
int bestindex=0,worst = 100000;
int lastbest = -1;
char *nH,*lch;
bool FlipFirst = false;
  cH = cHash;
  lch = cH;

  nH = nHash;
  while(worst>0)
  {
  worst = 1000000;
    for(i=0;i<255;i++)
    {
//      if (Every[i].Index ==-1) continue;
    int t = Apply(nH,cH,Every[i].Index,FlipFirst,true);
      if (t<=worst)
      {
        worst = t;
        bestindex = i;
      }
    }
    worst = Apply(nH,cH,Every[bestindex].Index,FlipFirst,false);
    while(*cH)
	{
      if (*cH!=*nH) break;
      cH++;nH++;
    }
    FlipFirst = true;
    printf("differece of %d when xor every %dnth for %d bytes\n",worst,Every[bestindex].Index,cH - lch);
    lch = cH;
    for(i=0;i<len*8;i++)
    {
      if (cHash[i]==nHash[i]) dHash[i] = '0';
      else dHash[i] = '1';
    }
    SetEvery(&Every[0],dHash,len);

//    Every[bestindex].Index = -1;
    lastbest = bestindex;
  }

  delete []nHash;
  delete []cHash;
}

/*class DuckFly : public Observer
{
public:
	DuckFly() {}
	DoBehaviour() { cout << "Ducks can fly"; }
	DoBehaviour(int i) { cout << i << " Ducks can fly"; }
};

class Ducks : public Observer::Client
{
Ducks() {RegisterBehaviour(new DuckFly(),true); };
Play() { DoBehavior(5); DoBehaviour(); }

};
*/


int main(int argc, char *argv[])
{

#ifdef VALIDATING
{
// Patterns test
//TestObserver to;
StateTest s;
TestCompositeIterator tci;
tci.DoTest();
TestCommand tc;
TestSingleton tsingleton;
TestFactory tf;
TestDecorator td;
//TestStrategy ts;

}
#endif

return 0;
char *teststr = "Hello, Hello world. This is a test of the emerging broad corping castration. If this had been an actual castration you would have been incorporated";
//  MakeHash("A",1);
  MakeHash(teststr,strlen(teststr)+1);

  cout << "Hello, world!" << endl;
  try
  {

  String s;
  MemStream d;
    s = "Hello, Hello world. This is a test of the emerging broad corping castration. If this had been an actual castration you would have been incorporated";
    s.CopyToStream(d,Stream::ZIP);
    
  String u;
    d.CopyToStream(u,Stream::UNZIP);
    printf("s length = %d, zipped length = %d, unzipped length = %d",s.Size(),d.Size(),u.Size());
  return 0;
  //CreateAINETXML();
  ClassBuilder cb;
  cb.SetBaseClass(CCryFuzzy,true,true,"MyFuzzy");
//  cb.SetBaseClass(TCryBPNetContainer);
//  cb.SetName("ListFunctionDef");
  cb.SetFilename("CryTestDef");
  cb.AddClassInstance(CCryFuzzy,"MyFuzzy",10,true,true,true,true,"0");
  cb.SaveSource();
  {
  CryFileStream std;
  	std.Open("stdout","w");
//  CryObject::Context context;
  	//context.IO.In.InSave.ToStream = std;
  //cb.Event(CryObject::ESaveStream,context);
  }
  }
  catch(Exception &e)
  {
  	cout << e << endl;
  }
  return 0;

  /*HugeFraction d,b;
  	d.SetValue("4194163");
  	b.SetValue("3");
  for(int i=1;i<20;i++)
  {
  	printf("base %s\t\t%s",b.GetAsStr(),d.GetAsStr(b));
	b.Shr1();
  }*/
  /*
  HugeInt m;
  m.SetValue("4194163");
  m.Shl32();
  printf("base 10 %s\n",m.GetAsStr(10));
    m.SetValue("4194163");
    for(int i=2;i<4000;i++)
    	printf("base %d\t%s\n",i,  m.GetAsStr(i));
return EXIT_SUCCESS;
//    m.SetValue("1234567890");
    m.Test(true,m,CallBack);
*/    
/*
 {
CryNN nn(2,1,500);
double InData [4][2] = { {0,0},{0,1},{1,0},{1,1}};	//xor inputs
double OutData[4];
double TargetData[4][1] = {{0},{1},{1},{0}};
double LastErrorLevel = 10000;        
	for(int i=0;i<100000;i++)
    	do
		{
                	nn.Train(4,&InData[0][0],&TargetData[0][0]);
			LastErrorLevel = nn.GetErrorLevel();
		} while(LastErrorLevel>0.0);
//        nn.Train(InData[1],TargetData[1]);
	for(int i=0;i<4;i++)
	{
	    nn.Run(TargetData[i],OutData);
	    printf("%d) Inputs %f %f resulted in %f\n",i,InData[i][0],InData[i][1],OutData[i]);
	}
  }
return EXIT_SUCCESS;
*/
  {
/*      CryString s;
      CryDoubleArray da(19);
      for(int i=0;i<19;i++)
      {
	  da.SetValue(i,i * 1.001);
	  da.SaveAsText(i,&s);
	  cout << s;
      }
  */    
  }
  TestFuzzyMain(argc,argv);
  return EXIT_SUCCESS;
  }
