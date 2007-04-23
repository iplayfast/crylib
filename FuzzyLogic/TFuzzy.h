
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef FUZZY_DEF
#define FUZZY_DEF
#ifndef READONLY
#define FUZZY_DATA_LENGTH 40
#else
#define FUZZY_DATA_LENGTH
#endif
#define FUZZY_NAME_LENGTH 40

//#define USE_INT	// not as accurate but will work on simple processors

#ifdef USE_INT
#define VALUE_TYPE int
#define VALUE_SCALE	1000	// if VALUE_TYPE is float then this can be 1
#else
#define VALUE_TYPE float
#endif
#ifdef __cplusplus
namespace Crystal   {
using namespace std;
namespace Fuzzy	{
extern "C" {
#endif
struct VC
{
  VALUE_TYPE Value;
  VALUE_TYPE Certainty;
};
struct TFuzzyXY
{
	VALUE_TYPE x;
	VALUE_TYPE y;
};


//class TFuzzy
struct TFuzzy
{
int DataLength;
struct TFuzzyXY Data[FUZZY_DATA_LENGTH];
};

void TFuzzyInit(struct TFuzzy *data);
int TFuzzyAdd(struct TFuzzy *data,VALUE_TYPE X,VALUE_TYPE Y);	// returns the index of the TFuzzyXY added
void TFuzzyAddFuzzy(struct TFuzzy *data,struct TFuzzyXY f);
void TFuzzyAddFuzzyp(struct TFuzzy *data,const struct TFuzzyXY *f);
void TFuzzySetValueAt(struct TFuzzy *data,int i,VALUE_TYPE x,VALUE_TYPE y);
const struct TFuzzyXY *GetItemc(const struct TFuzzy *data,int i);
struct TFuzzyXY *GetItem(struct TFuzzy *data,int i);
const char *TFuzzyGetName(const struct TFuzzy *data);
void TFuzzySetName(struct TFuzzy *data,const char *name);
int TFuzzySetValueXY(struct TFuzzy *data,VALUE_TYPE x,VALUE_TYPE y);
void DeleteItem(struct TFuzzy *data,int i);
/// A Fuzzy Logic Implimenation
int TFuzzyAddPoint(struct TFuzzy *data,VALUE_TYPE x,VALUE_TYPE y);

	VALUE_TYPE ValueAt(const struct TFuzzy *data,unsigned int idx);
	VALUE_TYPE IndexAt(const struct TFuzzy *data,unsigned int idx );
	VALUE_TYPE Value(const struct TFuzzy *Data,VALUE_TYPE InValue);
	VALUE_TYPE LowestRange(const struct TFuzzy *data);
	VALUE_TYPE HighestRange(const struct TFuzzy *data);
	void SwapXY(struct TFuzzy *data);
	// show how equal two fuzzy sets are
	VALUE_TYPE Equality(const struct TFuzzy *data,struct TFuzzy *compare);
	// will attempt to delete datapoints which would normally fall on the calcuated path by factor amount
	void Optimize(struct TFuzzy *data,VALUE_TYPE factor);
	// will increase the number of samples, by adding points between current points, but still along the data path
	void IncreaseSamples(const struct TFuzzy *data);
	VALUE_TYPE InhibitValue(const struct TFuzzy *data,VALUE_TYPE In,VALUE_TYPE InhibitPercent);
	void SimpleInhibit(const struct TFuzzy *data,VALUE_TYPE InhibitPercent);

	void CopyAndInhibit(struct TFuzzy *data,const struct TFuzzy *Fuzzy);
	// helper routine for CopyAndInhibit
	void CopyAndInhibitTemp(struct TFuzzy *data,const struct TFuzzy *Fuzzy,struct TFuzzy *Temp);

	// restrict the range to within Floor and Ceil
	void Range(struct TFuzzy *data,VALUE_TYPE Floor,VALUE_TYPE Ceil);

	// returns the higher of two results
	VALUE_TYPE Or(const struct TFuzzy *data,const struct TFuzzy *f, VALUE_TYPE In);
	/*
	 This is to be used when more then 2 data's must be or'd,
		eg.
		OrResult(Fuzzy1,ValueIn,Or(Fuzzy2,Fuzzy3,ValueIn);
	 */
	VALUE_TYPE OrResult(const struct TFuzzy *data,VALUE_TYPE In,VALUE_TYPE CurrentResult);
	// returns the lower of two results
	VALUE_TYPE And(const struct TFuzzy *data,const struct TFuzzy *f, VALUE_TYPE In);
	// used when more then two fuzzys must be anded (similar to OrResult)
	VALUE_TYPE AndResult(const struct TFuzzy *data,VALUE_TYPE In,VALUE_TYPE CurrentResult);
	// returns the differnce between two results
	VALUE_TYPE XOr(const struct TFuzzy *data,const struct TFuzzy *f, VALUE_TYPE In);
	// returns the TrueValue - the result, TrueValue is whatever value in the Fuzzy that would represent true (1.0 if classical fuzzy logic)
	VALUE_TYPE Not(const struct TFuzzy *data,VALUE_TYPE In,VALUE_TYPE TrueValue);

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
int TFuzzyAddPoint(VALUE_TYPE x,VALUE_TYPE y)
{
	return ::TFuzzyAddPoint(&f,x,y);
}

int TFuzzyAdd(VALUE_TYPE X,VALUE_TYPE Y)	// returns the index of the TFuzzyXY added
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
void TFuzzySetValueAt(int i,VALUE_TYPE x,VALUE_TYPE y)
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
int TFuzzySetValueXY(VALUE_TYPE x,VALUE_TYPE y)
{
	return ::TFuzzySetValueXY(&f,x,y);
}
void DeleteItem(int i)
{
	return ::DeleteItem(&f,i);
}
/// A Fuzzy Logic Implimenation

VALUE_TYPE ValueAt(unsigned int idx)
{
	return ::ValueAt(&f,idx);
}
VALUE_TYPE IndexAt(unsigned int idx )
{
	return ::IndexAt(&f,idx );
}
VALUE_TYPE Value(VALUE_TYPE InValue)
{
	return ::Value(&f,InValue);
}
VALUE_TYPE LowestRange()
{
	return ::LowestRange(&f);
}
VALUE_TYPE HighestRange()
{
	return ::HighestRange(&f);
}
void SwapXY()
{
	return ::SwapXY(&f);
}
VALUE_TYPE Equality(struct TFuzzy *compare)
{
	return ::Equality(&f,compare);
}
void Optimize(VALUE_TYPE factor)
{
	::Optimize(&f,factor);
}
void IncreaseSamples()
{
	::IncreaseSamples(&f);
}
int Length()const  { return f.DataLength; }
VALUE_TYPE InhibitValue(VALUE_TYPE In,VALUE_TYPE InhibitPercent)
{
	return ::InhibitValue(&f,In,InhibitPercent);
}
void SimpleInhibit(VALUE_TYPE InhibitPercent)
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
void Range(VALUE_TYPE Floor,VALUE_TYPE Ceil)
{
	return ::Range(&f,Floor,Ceil);
}


VALUE_TYPE Or(const ClassFuzzy *F, VALUE_TYPE v)
{
	return ::Or(&f,&F->f, v);
}
VALUE_TYPE And(const ClassFuzzy *F, VALUE_TYPE v)
{
	return ::And(&f,&F->f, v);
}
VALUE_TYPE XOr(const ClassFuzzy *F, VALUE_TYPE v)
{
	return ::XOr(&f,&F->f, v);
}
VALUE_TYPE Not(VALUE_TYPE v,VALUE_TYPE TrueValue)
{
	return ::Not(&f,v,TrueValue);
}
};
}	//namespace Crystal
#endif


#endif //FUZZY_DEF
