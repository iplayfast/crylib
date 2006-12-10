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

#include <math.h>

#include "CryArray.h"
#include "ClassString.h"

#ifndef BACKPROP_DEF
#define BACKPROP_DEF

#define BIAS          1
#define MIN_DOUBLE      -HUGE_VAL
#define MAX_DOUBLE      +HUGE_VAL


namespace Crystal   {
using namespace std;

#define CCryBPNetContainer "CryBPNetContainer"
#define CBackPropagateLayer "BackPropagateLayer"
#define CCryBPNet	"CryBPNet"

class CryBPNet;
/// a layer class for use within the CryBackpropagation Class
/*! The layer of the backpropagation net Weights are stored elsewhere, this class just stores the offsets into the weights array*/
class BackPropagateLayer : public Object
{
                     /* A Layer OF A NET:                     */
	int id;
	BackPropagateLayer();	// can't access
public:

		int           LayerSize;     /* - number of units in this Layer       */
	int		PreviousLayerSize;// handy to know
	int		OutputStart;	// output of Nth unit
	int		ErrorStart;		// error term of Nth unit
	int		WeightStart;	// connection weights to Nth unit
	int		WeightSaveStart;	// saved weights for intermediate results
	int		dWeightStart;	// delta weights for momentum
	CryBPNet *Owner;
public:
StdFunctionsNoDup(BackPropagateLayer,Object);

virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;

BackPropagateLayer(CryBPNet *_Owner);
void SetID(int i);
int  GetID() const { return id; }
virtual void CopyTo(Object &Object) const;
virtual bool HasProperty(const CryPropertyParser &PropertyName)const;
virtual int GetPropertyCount() const;
virtual CryPropertyList* PropertyNames() const;
virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
// if this class contains the property name, it will attempt to load it
// if all is well returns true
virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
};

/// low level backpropagation array class
class CryBPNet : public Array
{
public:// shit remove atfter testing
double *AllWeights;
int	AllWeightsSize;
int LockLevel;
protected:
    void CopyFromWeights(const double *Source,int length);
    void CopyToWeights(double *Dest,int length) const;
    int GetAllWeightsSize() const { return AllWeightsSize; }
public:
StdFunctions(CryBPNet,Array);
    virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;

	void SetAllWeights();	/// gets' called just before the network is trained, or as it is loaded
	double *GetAllWeights() const { return AllWeights; }
/// derived class will handle the destruction of objects contained in array
	virtual void DestroyArrayItem(Array *Owner,EmptyObject *Layer) ;
/// derived class will handle the creation of objects contained in array
	virtual EmptyObject *CreateArrayItem(Array *Owner,bool *IsCryObject) ;
/// derived class will handle the display in CryStream the objects contained in array (text assumed)
	virtual void SaveItemTo(const Array *Owner,EmptyObject *FromItem,CryStream &ToStream) const;
/// derived class will handle the loading of an Object from the stream, objectmust have already been created
	virtual EmptyObject *LoadItemFrom(Array *Owner,EmptyObject *ToItem,CryStream &FromStream);
	virtual bool LoadAsText(int i,CryString &FromStream) ;

    virtual bool LoadAsText(Iterator *I,CryString &FromStream)
    {
        return Array::LoadAsText(I,FromStream);
    }
   	virtual bool SaveAsText(int i,CryString &ToStream) const;
    virtual bool SaveAsText(Iterator *I,CryString &ToStream) const
    {
        return Array::SaveAsText(I,ToStream);
	}
	BackPropagateLayer *SetLayerSize(int i,int Size);
	int GetLayerSize(int i) const;
	void Propagate(int From,int To,double Gain);
    void BackPropagate(int From,int To,double Gain);
    void RandomWeights(int LayerNumber);
    void RandomWeights();

    BackPropagateLayer * AddLayer(int Size);
    void SetLockLevel(int n) { LockLevel = n; } // if retraining a new layer lock the old ones
    int GetLockLevel() { return LockLevel; }
    CryBPNet() : Array()	{	AllWeights = 0; LockLevel = 1; }
    ~CryBPNet()  	{	Clear(); delete []AllWeights;	}
   virtual void CopyTo(Array &Dest) const { Array::CopyTo(Dest);  } //copies contents of this to Dest
    virtual void CopyTo(Object &Dest) const { Array::CopyTo(Dest);  }  //copies contents of this to Dest
    virtual void GetEleType(CryString &Result) const;
};

/// backpropagation interface layer 
class CryBPNetContainer : public CryBPNet
{
double TestError;
double TrainError;
double          Alpha;         /* - momentum factor                     */
double          Eta;           /* - learning rate                       */
double          Gain;          /* - gain of sigmoid function            */
double          Error;         /* - total net error                     */
CryString Status;
void SaveWeights();
void RestoreWeights();
public:
StdFunctions(CryBPNetContainer,CryBPNet);
// DataSetup Functions
  void SetAlpha(double a) { Alpha = a; }
  void SetEta(double e) { Eta = e; }
  void SetGain(double g) { Gain = g; }

// functions needed by CryObject
	virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;

   virtual void CopyTo(Object &Dest) const;  //copies contents of this to Dest
// functions needed by CryNamedObject
	virtual bool HasProperty(const CryPropertyParser &PropertyName)const;
	virtual int GetPropertyCount() const;
	virtual CryPropertyList* PropertyNames() const;
	virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
	// if this class contains the property name, it will attempt to load it
	// if all is well returns true
	virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
	/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the Parent must be the appropriete container type or a derived class which can create the object (if the default class can't) */
	virtual Object *Create(const CryPropertyParser &PropertyName,Object *Parent=0);

// functions if multithreaded
const char *GetStatus() const { return Status.AsPChar(); }
virtual void ShowStatus() {/* printf("%s\n",Status.AsPChar());*/ }	// derived class could get updates with this
// functions needed to run the network
void TrainNet(int Epochs,int LengthIn,double *SampleIn,int LengthOut,double *SampleOut);
void STTrainNet(int Epochs,int LengthIn,double *SampleIn,int LengthOut,double *SampleOut);
void SimulateNet(const double* Input, double* Output,const double* Target, bool Training);
void SetInput(const double* Input);
void PropagateNet();
void BackPropagateNet();
void GetOutput(double* Output);
void ComputeOutputError(const double* Target);
void AdjustWeights();
void RandomWeights();
double GetError() const { return Error; }
void printWeights();

/*
// functions needed by CryContainer
   virtual Iterator *_CreateIterator() const;
   virtual void DeleteIterator(Iterator *I) const;
   virtual bool GotoFirst(Iterator *I) const;   // returns true if success
   virtual bool GotoPrev(Iterator *I) const;   // returns true if success
   virtual bool GotoNext(Iterator *I) const;    // returns true if success
   virtual bool GotoLast(Iterator *Iterator) const;    // returns true if success
   virtual EmptyObject *GetAtIterator(Iterator *I) const;
   virtual size_t Count() const;
   virtual void Clear();
   virtual EmptyObject *Add(EmptyObject *Item,size_t Size);
   virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size);
   virtual CryObject *Add(CryObject *Item);    // returns Item
   virtual CryObject *AddOwned(CryObject *Item);   // gives ownership to list
   virtual void SetItemOwnerShip(Iterator *I,bool Owned);
   virtual bool GetItemOwnerShip(Iterator *I) const;
   virtual size_t GetItemSize(Iterator *I) const;
   virtual bool IsCryObject(Iterator *I) const;
   virtual bool LoadAsText(Iterator *I,CryString *FromStream);
   virtual bool SaveAsText(Iterator *I,CryString *ToStream) const;*/
   virtual void CopyTo(Array &Dest) const { Array::CopyTo(Dest);  } //copies contents of this to Dest
   virtual void CopyTo(CryBPNetContainer &Dest) const { Object *n = &Dest; CopyTo(*n); }
   void SetSize(size_t n); // set the number currently in use (either grow or shrink)
// Actual class stuff
    CryBPNetContainer();
 #ifdef VALIDATING
virtual bool Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

};

}; //namespace Crystal

#endif  // BACKPROP_DEF
