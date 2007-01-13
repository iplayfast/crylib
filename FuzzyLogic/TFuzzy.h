
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef FUZZY_DEF
#define FUZZY_DEF

#define FUZZY_DATA_LENGTH 40
#define FUZZY_NAME_LENGTH 40
#ifdef __cplusplus
namespace Crystal   {
using namespace std;
namespace Fuzzy	{
extern "C" {
#endif
struct VC
{
  float Value;
  float Certainty;
};
struct TFuzzyXY
{
	float x;
	float y;
};


//class TFuzzy
struct TFuzzy
{
struct TFuzzyXY Data[FUZZY_DATA_LENGTH];
int DataLength;
//char Name[FUZZY_NAME_LENGTH+1];
};

void TFuzzyInit(struct TFuzzy *data);
int TFuzzyAdd(struct TFuzzy *data,float X,float Y);	// returns the index of the TFuzzyXY added
void TFuzzyAddFuzzy(struct TFuzzy *data,struct TFuzzyXY f);
void TFuzzyAddFuzzyp(struct TFuzzy *data,const struct TFuzzyXY *f);
void TFuzzySetValueAt(struct TFuzzy *data,int i,float x,float y);
const struct TFuzzyXY *GetItemc(const struct TFuzzy *data,int i);
struct TFuzzyXY *GetItem(struct TFuzzy *data,int i);
const char *TFuzzyGetName(const struct TFuzzy *data);
void TFuzzySetName(struct TFuzzy *data,const char *name);
int TFuzzySetValueXY(struct TFuzzy *data,float x,float y);
void DeleteItem(struct TFuzzy *data,int i);
/// A Fuzzy Logic Implimenation
int TFuzzyAddPoint(struct TFuzzy *data,float x,float y);

	float ValueAt(const struct TFuzzy *data,unsigned int idx);
	float IndexAt(const struct TFuzzy *data,unsigned int idx );
	float Value(const struct TFuzzy *Data,float InValue);
	float LowestRange(const struct TFuzzy *data);
	float HighestRange(const struct TFuzzy *data);
	void SwapXY(struct TFuzzy *data);
	// show how equal two fuzzy sets are
	float Equality(const struct TFuzzy *data,struct TFuzzy *compare);
	// will attempt to delete datapoints which would normally fall on the calcuated path by factor amount
	void Optimize(struct TFuzzy *data,float factor);
	// will increase the number of samples, by adding points between current points, but still along the data path
	void IncreaseSamples(const struct TFuzzy *data);
	float InhibitValue(const struct TFuzzy *data,float In,float InhibitPercent);
	void SimpleInhibit(const struct TFuzzy *data,float InhibitPercent);

	void CopyAndInhibit(struct TFuzzy *data,const struct TFuzzy *Fuzzy);
	// helper routine for CopyAndInhibit
	void CopyAndInhibitTemp(struct TFuzzy *data,const struct TFuzzy *Fuzzy,struct TFuzzy *Temp);

	// restrict the range to within Floor and Ceil
	void Range(struct TFuzzy *data,float Floor,float Ceil);

	// returns the higher of two results
	float Or(const struct TFuzzy *data,const struct TFuzzy *f, float In);
	/*
	 This is to be used when more then 2 data's must be or'd,
		eg.
		OrResult(Fuzzy1,ValueIn,Or(Fuzzy2,Fuzzy3,ValueIn);
	 */
	float OrResult(const struct TFuzzy *data,float In,float CurrentResult);
	// returns the lower of two results
	float And(const struct TFuzzy *data,const struct TFuzzy *f, float In);
	// used when more then two fuzzys must be anded (similar to OrResult)
	float AndResult(const struct TFuzzy *data,float In,float CurrentResult);
	// returns the differnce between two results
	float XOr(const struct TFuzzy *data,const struct TFuzzy *f, float In);
	// returns the TrueValue - the result, TrueValue is whatever value in the Fuzzy that would represent true (1.0 if classical fuzzy logic)
	float Not(const struct TFuzzy *data,float In,float TrueValue);

#ifdef VALIDATING
virtual bool Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

#ifdef __cplusplus
}	//extern "C"
}	//namespace Fuzzy
using namespace Fuzzy;

class ClassFuzzy
{
TFuzzy f;
public:
	ClassFuzzy() { ::TFuzzyInit(&f); }
int TFuzzyAddPoint(float x,float y)
{
	return ::TFuzzyAddPoint(&f,x,y);

}

int TFuzzyAdd(float X,float Y)	// returns the index of the TFuzzyXY added
{
	return ::TFuzzyAdd(&f,X,Y);
}
void TFuzzyAddFuzzy(const struct TFuzzyXY &xy)
{
	return ::TFuzzyAddFuzzy(&f,xy);
}

void TFuzzyAddFuzzyp(const struct TFuzzyXY *F)
{
	::TFuzzyAddFuzzyp(&f,F);
}
void TFuzzySetValueAt(int i,float x,float y)
{
	::TFuzzySetValueAt(&f,i,x,y);
}
const struct TFuzzyXY *GetItemc(int i)
{
	return ::GetItemc(&f,i);
}
struct TFuzzyXY *GetItem(int i)
{
	return ::GetItem(&f,i);
}
const char *TFuzzyGetName()
{
	return ::TFuzzyGetName(&f);
}
void TFuzzySetName(const char *name)
{
	return ::TFuzzySetName(&f,name);
}
int Count() const
{
	return f.DataLength;
}
void Clear()
{
	f.DataLength = 0;
}
int TFuzzySetValueXY(float x,float y)
{
	return ::TFuzzySetValueXY(&f,x,y);
}
void DeleteItem(int i)
{
	return ::DeleteItem(&f,i);
}
/// A Fuzzy Logic Implimenation

float ValueAt(unsigned int idx)
{
	return ::ValueAt(&f,idx);
}
float IndexAt(unsigned int idx )
{
	return ::IndexAt(&f,idx );
}
float Value(float InValue)
{
	return ::Value(&f,InValue);
}
float LowestRange()
{
	return ::LowestRange(&f);
}
float HighestRange()
{
	return ::HighestRange(&f);
}
void SwapXY()
{
	return ::SwapXY(&f);
}
float Equality(struct TFuzzy *compare)
{
	return ::Equality(&f,compare);
}
void Optimize(float factor)
{
	::Optimize(&f,factor);
}
void IncreaseSamples()
{
	::IncreaseSamples(&f);
}
int Length()const  { return f.DataLength; }
float InhibitValue(float In,float InhibitPercent)
{
	return ::InhibitValue(&f,In,InhibitPercent);
}
void SimpleInhibit(float InhibitPercent)
{
	::SimpleInhibit(&f,InhibitPercent);
}
void CopyAndInhibit(const struct TFuzzy *Fuzzy)
{
	::CopyAndInhibit(&f,Fuzzy);
}
void CopyAndInhibitTemp(const struct TFuzzy *Fuzzy,struct TFuzzy *Temp)
{
	::CopyAndInhibitTemp(&f,Fuzzy,Temp);
}

	// restrict the range to within Floor and Ceil
void Range(float Floor,float Ceil)
{
	return ::Range(&f,Floor,Ceil);
}


float Or(const ClassFuzzy *F, float v)
{
	return ::Or(&f,&F->f, v);
}
float And(const ClassFuzzy *F, float v)
{
	return ::And(&f,&F->f, v);
}
float XOr(const ClassFuzzy *F, float v)
{
	return ::XOr(&f,&F->f, v);
}
float Not(float v,float TrueValue)
{
	return ::Not(&f,v,TrueValue);
}
};
}	//namespace Crystal
#endif


#endif //FUZZY_DEF
