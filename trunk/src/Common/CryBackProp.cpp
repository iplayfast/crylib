/***************************************************************************
 *   Copyright (C) 2006 by Chris Bruner                                    *
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

#include <stdlib.h>
#include <errno.h>
#include "CryBackProp.h"
#include "ClassFunction.h"
#include "ClassProperty.h"
#include "Utility.h"
#include "ClassException.h"
#include "ClassFileStream.h"
#include "ClassXML.h"

using namespace Crystal;
using namespace std;

BackPropagateLayer::BackPropagateLayer(BPNet *_Owner)
{
    SetID(-1);
    LayerSize = 0;
    Owner = _Owner;
	//id = -1;
}
FunctionDefList *BackPropagateLayer::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return Object::GetFunctions(Type);
	// otherwise get any functions in subclasses
	FunctionDefList *l = Object::GetFunctions();
String s;
	s += "\n\n//BackPropagateLayer;";
	s +="String *GetFunctions() const;";
	s +="BackPropagateLayer(BPNet *_Owner);";
	s +="void SetID(int i);";
	s +="int  GetID() const;";
	s +="virtual void CopyTo(Object &Object) const;";
	s +="virtual Object *Dup() const;";
	s +="const char* ChildClassName() const;";
	s +="virtual bool HasProperty(const PropertyParser &PropertyName) const;";
	s +="virtual int GetPropertyCount() const;";
	s +="virtual List *PropertyNames() const;";
	s +="virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;";
	s +="virtual bool SetProperty(const PropertyParser &PropertyName,const String &PropertyValue);";
	s +="virtual bool IsA(const char *_ClassName) const;";
	l->LoadFromString(s,";");
	return l;
}
PropertyList *BackPropagateLayer::PropertyNames() const
{
	PropertyList *n = Object::PropertyNames();
	n->AddPropertyByName("Size",this);
	if (GetID()!=0)
		n->AddPropertyByName("Weights",this);
	return n;
}

bool BackPropagateLayer::HasProperty(const PropertyParser &PropertyName) const
{
	return (PropertyName=="Size") ||
			((GetID()!=0) && (PropertyName=="Weights")) ||
			Object::HasProperty(PropertyName);
}
int BackPropagateLayer::GetPropertyCount() const
{
	int i = 2;	// Size,Weights
	if (GetID()==0)
		i = 1;	// except if LayerNumber 0
	return Object::GetPropertyCount()+i;
}
const char *BackPropagateLayer::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (PropertyName=="Size")
	{
		Result.printf("%d",LayerSize);
		return Result.AsPChar();
	}
	if (PropertyName=="Weights")
	{
		Result = "[]";  // if Result != what is returned, it's a special situation, in this case pointer to an array of weights
		return "*";
	}
	if ((PropertyName=="*Weights") && (GetID()!=0))
	{
		double *w = Owner->GetAllWeights() +WeightStart;
		Result.Clear();
		int NumOfWeights = LayerSize * PreviousLayerSize;
		for(int i=0;i<NumOfWeights;i++)
			Result.printf("%lf ",*w++);
		return Result.AsPChar();
	}
	return Object::GetProperty(PropertyName,Result);
}

// if this class contains the property name, it will attempt to load it
// if all is well returns true
bool BackPropagateLayer::SetProperty(const PropertyParser &PropertyName,const char *_PropertyValue)
{
String PropertyValue;
	PropertyValue = _PropertyValue;
	if (PropertyName=="Weights")
	{
		Owner->SetAllWeights();	// make sure there is enough memory
		if (GetID()==0)
			throw Exception(this,"Assigning Weights to input layer");
		double *w = Owner->GetAllWeights() +WeightStart;

		PropertyValue.SeekFromStart(0);
		int NumOfWeights = LayerSize * PreviousLayerSize;
		for(int i=0;i<NumOfWeights;i++)
		{
			PropertyValue.scanf("%lf ",w);
			PropertyValue.SeekTextFromCurrent(" ");
			PropertyValue.SeekFromCurrent(1);
			w++;
		}
		return true;
	}
	if (PropertyName=="Size")
	{
	int Size;
		PropertyValue.scanf("%d",&Size);
		Owner->SetLayerSize(id,Size);
		Owner->SetAllWeights();
		return true;
	}
	else
		return Object::SetProperty(PropertyName,PropertyValue);
}


void BackPropagateLayer::SetID(int i)
{
    id = i;
}
void BackPropagateLayer::CopyTo(Object &Object) const
{
    if (Object.IsA(CBackPropagateLayer))
    {
        BackPropagateLayer *b = (BackPropagateLayer *)&Object;
		b->id = id;
        b->LayerSize = LayerSize;
        b->PreviousLayerSize = PreviousLayerSize;
        b->OutputStart = OutputStart;
        b->ErrorStart = ErrorStart;
        b->WeightStart = WeightStart;
        b->WeightSaveStart = WeightSaveStart;
        b->dWeightStart = dWeightStart;
    }
    else
        throw Exception(this,"Cannot CopyTo %s from %s",Object.ClassName(),ClassName());
}

BackPropagateLayer * BPNet::AddLayer(int Size)
{
BackPropagateLayer *b = 0;
	int c = Count();
		if (c<0) c=0;
	b = SetLayerSize(c,Size);
	SetAllWeights();
	return b;
}
void BPNet::GetEleType(String &Result) const
{
	Result = CBackPropagateLayer;
}

FunctionDefList *BPNet::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return Array::GetFunctions(Type);
	// otherwise get any functions in subclasses
	FunctionDefList *l = Array::GetFunctions();
String s;
	s += "\n\n//BPNet;";
	s +="String *GetFunctions() const;";
	s +="void SetAllWeights();";
	s +="double *GetAllWeights() const;";
	s +="virtual void DestroyArrayItem(Array *Owner,EmptyObject *Layer);";
	s +="virtual EmptyObject *CreateArrayItem(Array *Owner,bool *IsObject);";
	s +="virtual void SaveItemTo(const Array *Owner,EmptyObject *FromItem,Stream &ToStream) const;";
	s +="virtual EmptyObject *LoadItemFrom(Array *Owner,EmptyObject *ToItem,Stream &FromStream);";
	s +="virtual bool LoadAsText(int i,String &FromStream) ;";
	s +="virtual bool LoadAsText(Iterator *I,String &FromStream);";
	s +="virtual bool SaveAsText(int i,String &ToStream) const;";
	s +="virtual bool SaveAsText(Iterator *I,String &ToStream) const;";
	s +="void SetLayerSize(int i,int Size);";
	s +="int GetLayerSize(int i) const;";
	s +="void Propagate(int From,int To,double Gain);";
	s +="void BackPropagate(int From,int To,double Gain);";
	s +="void RandomWeights(int LayerNumber);";
	s +="void RandomWeights();";
    s +="virtual bool IsA(const char *ClassName) const;";
    s +="BackPropagateLayer * AddLayer(int Size);";
    s +="void SetLockLevel(int n);";
    s +="int GetLockLevel();";
    s +="virtual void CopyTo(Array &Dest) const;";
    s +="virtual void CopyTo(Object &Dest) const;";
    s +="virtual void GetEleType(String &Result) const;";
	l->LoadFromString(s,";");
    return l;
}
	/// will return whether or not the property named in PropertyName is a container
bool BPNet::GetIsPropertyContainer(const PropertyParser &PropertyName) const
{
	if (PropertyName=="Layer")
		return true;
	return Array::GetIsPropertyContainer(PropertyName);
}

void BPNet::SetAllWeights()	// allocate the actual array of doubles
{
    int n = 0,ps = 0;
	for(unsigned int i=0;i<Count();i++)// for each layer
	{
		BackPropagateLayer *L =(BackPropagateLayer *)GetItem(i);
		L->SetID(i);    // set layer id
		int s = L->LayerSize;
        L->PreviousLayerSize = ps;
        L->OutputStart = n;
        n += s;
        L->ErrorStart = n;
        n += s;
        if (i)
        {
			L->WeightStart = n;
            n += s*ps;
            L->WeightSaveStart = n;
			n+=s*ps;
            L->dWeightStart = n;
            n+=s*ps;
        }
        ps = s;
    }
    // n is the number of doubles we need

    if (AllWeights && (n == AllWeightsSize))
        return;// already configured

    double *NewWeights = new double[n];
    if (AllWeights)	// a new layer was added!
    {
        for(int i=0;i<n;i++)
		{
            if (i<AllWeightsSize)
                NewWeights[i] = AllWeights[i];
            else
                NewWeights[i] = RandomDouble(-1,1);
        }
        delete []AllWeights;
    }
    else
        for(int i=0;i<n;i++)
            NewWeights[i] = RandomDouble(-1,1);
	AllWeights = NewWeights;
    AllWeightsSize = n;
}
/// derived class will handle the destruction of objects contained in array
void BPNet::DestroyArrayItem(Array *Owner,EmptyObject *Layer)
{
    BackPropagateLayer *L = (BackPropagateLayer *)Layer;
    delete L;
}

/// derived class will handle the creation of objects contained in array
EmptyObject *BPNet::CreateArrayItem(Array *Owner,bool *IsObject)
{
    *IsObject = true;
    BackPropagateLayer *l = new BackPropagateLayer(this);
    return l;
}

// derived class will handle the display in Stream the objects contained in array (text assumed)
void BPNet::SaveItemTo(const Array *Owner,EmptyObject *FromItem,Stream &ToStream) const
{
	BackPropagateLayer *l =(BackPropagateLayer *)FromItem;
	XMLNode n;
		n.LoadFrom(*l);
		n.SaveTo(ToStream);
		return;
  //  MemStream *ms = (MemStream *)ToStream;

	ToStream.printf("%d %d ",l->GetID(),l->LayerSize);
	if (l->GetID()<0)
		ToStream.printf("Uninitialized");
}
// derived class will handle the loading of an Object from the stream, objectmust have already been created
EmptyObject *BPNet::LoadItemFrom(Array *Owner,EmptyObject *ToItem,Stream &FromStream)
{
	BackPropagateLayer  *l = (BackPropagateLayer *)ToItem;
	int LayerNumber=-1,LayerSize;
	FromStream.scanf("%d %d ",&LayerNumber,&LayerSize);
	if (LayerNumber==-1)
		return ToItem;	// uninitialized
	l->LayerSize = LayerSize;
	return l;
}

bool BPNet::LoadAsText(int i,String &FromStream)
{
	LoadItemFrom(this,GetItem(i),FromStream);
	return true;
}

bool BPNet::SaveAsText(int i,String &ToStream) const
{
	MemStream e;
	SaveItemTo(this,GetItem(i),e);
	e.CopyToStream(ToStream);
	return true;
}

void BPNet::Propagate(int _From,int _To,double Gain)
{
	BackPropagateLayer *From,*To;
	From = (BackPropagateLayer *)GetItem(_From);
	To = (BackPropagateLayer *)GetItem(_To);
	double *w = AllWeights +To->WeightStart;
	double *_f = AllWeights + From->OutputStart;
	double *o = AllWeights + To->OutputStart;
	for (int i=0; i<To->LayerSize; i++)
	{
		double Sum = 0.0;
		double *f = _f;
		for (int j=0; j<From->LayerSize; j++)
		{
			Sum += *w * *f;
			w++;
			f++;
		}
		//    if ((-Gain * Sum)>500.0)
		//        gint++;
		Sum = 1.0 / (1.0 + (exp(-Gain * Sum)));
		if (errno==ERANGE)
			Sum = MAX_DOUBLE;
		*o++ = Sum;
	}
}

FunctionDefList *BPNetContainer::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return BPNetContainer::GetFunctions(Type);
	// otherwise get any functions in subclasses
	FunctionDefList *l = BPNetContainer::GetFunctions();
String s;
	s = "\n\n//BPNetContainer;";
	s +="void SetAlpha(double a);";
	s +="void SetEta(double e);";
	s +="void SetGain(double g);";
	s +="String *GetFunctions() const;";
	s +="virtual void CopyTo(Object &Dest) const;";
	s +="virtual Object *Dup() const;";
	s +="virtual const char* ChildClassName() const;";
	s +="virtual bool HasProperty(const PropertyParser &PropertyName) const;";
	s +="virtual int GetPropertyCount() const;";
	s +="virtual List *PropertyNames() const;";
	s +="virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;";
	s +="virtual bool SetProperty(const PropertyParser &PropertyName,const String &PropertyValue);";
	s +="const char *GetStatus() const;";
	s +="virtual void ShowStatus();";
	s +="void TrainNet(int Epochs,int LengthIn,double *SampleIn,int LengthOut,double *SampleOut);";
	s +="void STTrainNet(int Epochs,int LengthIn,double *SampleIn,int LengthOut,double *SampleOut);";
	s +="void SimulateNet(const double* Input, double* Output,const double* Target, bool Training);";
	s +="void SetInput(const double* Input);";
	s +="void PropagateNet();";
	s +="void BackPropagateNet();";
	s +="void GetOutput(double* Output);";
	s +="void ComputeOutputError(const double* Target);";
	s +="void AdjustWeights();";
	s +="void RandomWeights();";
	s +="double GetError() const;";
	s +="void printWeights();";
	s +="virtual bool IsA(const char *_ClassName) const;";
	s +="virtual void CopyTo(Array &Dest) const;";
	s +="virtual void CopyTo(BPNetContainer &Dest) const;";
	s +="void SetSize(size_t n);";
#ifdef VALIDATING
	s +="virtual bool Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));";
#endif
	l->LoadFromString(s,";");
	return l;
}

void BPNetContainer::SetSize(size_t n) // set the number currently in use (either grow or shrink)
{
	Array::SetSize(n);
}

void BPNet::BackPropagate(int _From,int _To,double Gain)
{
	int  i,j;
	double Out, Err;
	BackPropagateLayer *From,*To;
	if (_From <1)
		return;	// can't do this
	From = (BackPropagateLayer *)GetItem(_From);
	To = (BackPropagateLayer *)GetItem(_To);
	double *f = AllWeights + From->WeightStart;
	double *_fe = AllWeights + From->ErrorStart;
	double *o = AllWeights + To->OutputStart;
	double *te = AllWeights + To->ErrorStart;
	for (i=0; i<To->LayerSize; i++)
	{
		Out = *o++;
		Err = 0.0;
		double *fe = _fe;
		for (j=0; j<From->LayerSize; j++)
		{
			Err += *f++ * *fe++;
		}
		*te++ = Gain * Out * (1 - Out) * Err;
	}
}

int BPNet::GetLayerSize(int LayerNumber) const
{
	BackPropagateLayer *l = (BackPropagateLayer *)GetItem(LayerNumber);
	return l->LayerSize;
}

BackPropagateLayer *BPNet::SetLayerSize(int LayerNumber,int LayerSize)
{
	BackPropagateLayer *l;
	if (this->Count()>LayerNumber)
	   l = (BackPropagateLayer *)GetItem(LayerNumber);
	else
	{
		l = new BackPropagateLayer(this);
		AddOwned(l);
	}
	l->SetID(LayerNumber);
	l->LayerSize = LayerSize;
	if (LayerNumber != 0)
	{
		const BackPropagateLayer *lp = (BackPropagateLayer *)GetItem(LayerNumber -1);
		l->PreviousLayerSize =  lp->LayerSize;
	}
	else
		l->PreviousLayerSize =0;
	return l;
}

void BPNet::RandomWeights()
{
	SetAllWeights();
	double *w = AllWeights;
	for(int i=0;i<AllWeightsSize;i++)
		*w++ = RandomDouble(-1,1);
}

void BPNet::CopyFromWeights(const double *Source,int length)
{
	if (length>AllWeightsSize) length = AllWeightsSize;
	double *w = AllWeights;
	for(int i=0;i<length;i++)
		*w++ = *Source++;
}

void BPNet::CopyToWeights(double *Dest,int length) const
{
	if (length>AllWeightsSize) length = AllWeightsSize;
	double *w = AllWeights;
	for(int i=0;i<length;i++)
		*Dest++ = *w++;
}


void BPNet::RandomWeights(int LayerNumber)
{
	int i,j;
	{
		BackPropagateLayer *pLayer = (BackPropagateLayer *) GetItem(LayerNumber);
		int PreviousLayerSize = ((BackPropagateLayer *)GetItem(LayerNumber-1))->LayerSize;
		double *w = GetAllWeights() + pLayer->WeightStart;
		for (i=0; i<pLayer->LayerSize; i++)
			for (j=0; j < PreviousLayerSize; j++)
				*w++ = RandomDouble(-0.5,0.5);
	}
}

//
// BPNetContainer : This class contains an array of back propagation layers
//
// functions needed by Object
void BPNetContainer::CopyTo(Object &Dest) const  //copies contents of this to Dest
{
	if (Dest.IsA(CBPNetContainer))
	{
	BPNetContainer *d = (BPNetContainer *)&Dest;
		d->TestError = TestError;
		d->TrainError = TrainError;
		d->Alpha = Alpha;
		d->Eta = Eta;
		d->Gain = Gain;
		d->Error = Error;
		d->Status = Status;
		BPNet::CopyTo(Dest);
		d->SetAllWeights();
		d->CopyFromWeights(GetAllWeights(),GetAllWeightsSize());
	}
	else
		BPNet::CopyTo(Dest);
}
void BPNetContainer::RandomWeights()
{
	BPNet::RandomWeights();
	for (size_t l=1; l<Size(); l++)
		BPNet::RandomWeights(l);
}

/// will return a property represented as an object, useful for classes which contain properties that are dynamically allocated, as a property that is dynamic is a Object and therefore callable
Object *BPNetContainer::GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const
{
	if (PropertyName=="Layer")
	{
	String Result;
	const char *c = Container::GetProperty("*Values",Result);
	XMLNode *x = new XMLNode(PropertyName);
		x->LoadFrom(Result);
		return x;
	}
	return BPNet::GetCopyOfPropertyAsObject(PropertyName);
}


bool BPNetContainer::HasProperty(const PropertyParser &PropertyName) const
{
	if (PropertyName=="Layer_Count")
		return true;
	if (PropertyName=="TestError")
		return true;
	if (PropertyName=="TrainError")
		return true;
	if (PropertyName=="Layer")
		return true;
	return BPNet::HasProperty(PropertyName);
}
int BPNetContainer::GetPropertyCount() const
{
	return BPNet::GetPropertyCount() +
		   4; 	//dWeigths,Weights, TestError and TrainError (Layer Count is same as base class Size property)
}

PropertyList *BPNetContainer::PropertyNames() const
{
	PropertyList *n = BPNet::PropertyNames();
	n->AddPropertyByName("TestError",this);
	n->AddPropertyByName("TrainError",this);
	// we call Size == Layer Count
	n->RenameProperty("Size","Layer_Count");
	// we call Values == Layers
	n->RenameProperty("Values","Layer");

	return n;
}
const char *BPNetContainer::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (PropertyName=="Layer_Count")
	{
		return GetProperty("Size",Result);
	}
	if (PropertyName=="TestError")
	{
		Result.printf("%f ",TestError);
		return Result.AsPChar();
	}
	if (PropertyName=="TrainError")
	{
		Result.printf("%f ",TrainError);
		return Result.AsPChar();
	}
	if ((PropertyName=="Layer") || (PropertyName=="Values"))
	{
		PropertyParser NewPropertyName("Values");
		return BPNet::GetProperty(NewPropertyName,Result);
	}
	if (PropertyName=="*Layer")
	{
	PropertyParser NewPropertyName("*Values");
		return BPNet::GetProperty(NewPropertyName,Result);
	}
	return BPNet::GetProperty(PropertyName,Result);
}

// if this class contains the property name, it will attempt to load it
// if all is well returns true
bool BPNetContainer::SetProperty(const PropertyParser &PropertyName,const char *_PropertyValue)
{
String PropertyValue;
	PropertyValue = _PropertyValue;
	if (PropertyName=="Layer_Count")
	{
		return false;	// read only property
	}
	if (PropertyName=="TestError")
	{
		double v=0;
		PropertyValue.scanf("%lf ",&v);
		TestError = v;
		return true;
	}
	if (PropertyName=="TrainError")
	{
		double v=0;
		PropertyValue.scanf("%lf ",&v);
		TrainError = v;
		return true;
	}
	if (PropertyName=="*Layer")
	{
	PropertyParser NewPropertyName("*Values");
		return BPNet::SetProperty(NewPropertyName,PropertyValue);
	}
	if (PropertyName=="Layer")
	{
	XML x;
	String s = _PropertyValue;
		x.LoadFrom(s);
		BackPropagateLayer *l = (BackPropagateLayer *) x.CreateObjectFromNode(this);
		return true;

	PropertyParser NewPropertyName("Values");
		return BPNet::SetProperty(NewPropertyName,PropertyValue);
	}

	return BPNet::SetProperty(PropertyName,PropertyValue);
}


BPNetContainer::BPNetContainer()
{
	Alpha       = 0.9;
	Eta         = 0.25;
	Gain        = 1;
	TestError = TrainError = MAX_DOUBLE;
}
	/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the Parent must be the appropriete container type or a derived class which can create the object (if the default class can't) */
Object *BPNetContainer::Create(const PropertyParser &PropertyName,Object *Parent)
{

	if (PropertyName==CBPNetContainer)
		return new BPNetContainer();
	if (PropertyName==CBackPropagateLayer)
	{
		if (Parent && (Parent->IsA(CBPNet)))
		{
			return ((BPNet *)Parent)->AddLayer(0);
		}
		else
			throw Exception("Cannot Create BackPropagateLayer, Must have a parent of %s (or a decendent)",CBPNet);
	}
	return BPNet::ClassCreate(PropertyName,Parent);
}

void BPNetContainer::ComputeOutputError(const double *Target)
{
	int  i;
	double Out, Err;

	double Error = 0;
	BackPropagateLayer  *pOutputLayer = (BackPropagateLayer  *)GetItem(Size()-1);
	double *e = GetAllWeights() + pOutputLayer->ErrorStart;
	double *o = GetAllWeights() + pOutputLayer->OutputStart;
	Out = Err = 0.0;

	for (i=0; i<pOutputLayer->LayerSize; i++)
	{
		Out = *o++;
		Err = Target[i]-Out;
		*e++ = Gain * Out * (1-Out) * Err;
		Error += 0.5 * Err * Err;
	}
	TestError += Error;
	//printf("ComputeOutputError %3.3f\n",Net->Error);
}

void BPNetContainer::AdjustWeights()
{
	for (unsigned int l=GetLockLevel(); l<Size(); l++)
	{
		BackPropagateLayer *pLayer1 = (BackPropagateLayer *)GetItem(l);
		BackPropagateLayer *pLayer2 = (BackPropagateLayer *)GetItem(l-1);
		double *dw,*w,*o,*e;
		dw = GetAllWeights() + pLayer1->dWeightStart;
		w = GetAllWeights() + pLayer1->WeightStart;
		e = GetAllWeights() + pLayer1->ErrorStart;
		for (int i=0; i<pLayer1->LayerSize; i++)
		{
			o = GetAllWeights() + pLayer2->OutputStart;
			for (int j=0; j<pLayer2->LayerSize; j++)
			{
				double Out = *o++;
				double Err = *e;
				*w = *w + (Eta *Err *Out + Alpha * *dw);
				*dw = Eta * Err * Out;
				w++;
				dw++;
				//Total += pLayer1->Weight[i][j] + pLayer1->dWeight[i][j];
			}
			e++;
		}
	}
	//printf("AW Total = %3.3f\n",Total);
}

void BPNetContainer::PropagateNet()
{
	unsigned int l,s = Size()-1;
	for (l=0; l<s; l++)
		Propagate(l,l+1,Gain);
}

void BPNetContainer::BackPropagateNet()
{
	for (int l=Size()-1; l>1; l--)
		BackPropagate(l,l-1,Gain);
}

void BPNetContainer::GetOutput(double *Output)
{
	BackPropagateLayer  *pLayer = (BackPropagateLayer  *) GetItem(Size()-1);    // Output layer
	double *out = GetAllWeights() + pLayer->OutputStart;
	for (int i=0; i<pLayer->LayerSize; i++)
		*Output++ = *out++;
}

void BPNetContainer::SetInput(const double *Input)
{
	BackPropagateLayer  *L = (BackPropagateLayer  *)GetItem(0);    // Input layer
	double *in = GetAllWeights() + L->OutputStart;
	for (int i=0; i<L->LayerSize; i++)
		*in++ = *Input++;
}

void BPNetContainer::SaveWeights()
{
	int l,i,j;
	int NumLayers = Size();
	double *w,*ws;
	for (l=1; l<NumLayers; l++)
	{
		BackPropagateLayer *pLayer = (BackPropagateLayer *) GetItem(l);
		int PreviousLayerSize = GetLayerSize(l-1);
		w = GetAllWeights() + pLayer->WeightStart;
		ws = GetAllWeights() + pLayer->WeightSaveStart;
		for (i=1; i<pLayer->LayerSize; i++)
			for (j=0; j < PreviousLayerSize; j++)
				*ws++ = *w++;
	}
}

void BPNetContainer::RestoreWeights()
{
	int l,i,j;
	int NumLayers = Size();
	double *w,*ws;
	for (l=1; l<NumLayers; l++)
	{
		BackPropagateLayer *pLayer = (BackPropagateLayer *) GetItem(l);
		int PreviousLayerSize = GetLayerSize(l-1);
		w = GetAllWeights() + pLayer->WeightStart;
		ws = GetAllWeights() + pLayer->WeightSaveStart;
		for (i=1; i<pLayer->LayerSize; i++)
			for (j=0; j < PreviousLayerSize; j++)
				*w++ = *ws++;
	}
}

void BPNetContainer::printWeights()
{
	for(unsigned int i=0;i<Size();i++)
	{
		BackPropagateLayer *pLayer = (BackPropagateLayer *) GetItem(i);
		String s;
		s.printf("Layer %d Size is %d ",i,pLayer->LayerSize);
		if (i)
		{
			s.printf("(");
		double *w = GetAllWeights() + pLayer->WeightStart;
			for(int j=0;j<pLayer->LayerSize;j++)
				s.printf("%f, ",*w++);
			s.printf(")\n");
		}
		else
			s.printf(" (no weights)\n");
		//cout << s;         a derived class could use this
	}
}

void BPNetContainer::SimulateNet(const double *Input, double *Output,const double *Target, bool Training)
{
	SetInput(Input);
	PropagateNet();
	GetOutput(Output);

	ComputeOutputError(Target);
	if (Training)
	{
		BackPropagateNet();
		AdjustWeights();
	}
}

void BPNetContainer::TrainNet(int Epochs,int LengthIn,double *SampleIn,int LengthOut,double *SampleOut)
{
	int  j, i;
	SetAllWeights();
	double *Output;
	int OutputSize = GetLayerSize(Size()-1);
	Output = new double[OutputSize];
	TestError = 0.0;
	for (i=0; i<Epochs; i++)
	{
		int Sample = RandomInt(0,LengthIn-1);
		j =Sample * GetLayerSize(0);	// adjust for input width
		int k = Sample * OutputSize;
		SimulateNet(&SampleIn[j], Output, &SampleOut[k], true);
	}
	delete []Output;
}

void BPNetContainer::STTrainNet(int EPochs,int LengthIn,double *SampleIn,int LengthOut,double *SampleOut)
{
	bool Stop = false;
	double  MinTestError = MAX_DOUBLE;
	int count = 0;
	do
	{
		TrainNet(EPochs,LengthIn,SampleIn,LengthOut,SampleOut);
		//TestNet();	// uncomment if TestError doesn't get set
		count++;
		Status.printf("Trained %d EPochs, Current Test Error = %3.9f\n",count,TestError);

		if (TestError < MinTestError)
		{
			ShowStatus();
			Status.printf(" - saving Weights ...\n");
			ShowStatus();
			MinTestError = TestError;
			SaveWeights();
		}
		else
		{
			if ((TestError < 1) || (TestError > 1.2 * MinTestError))
			{
				Status.printf(" - stopping Training and restoring Weights ...\n");
				ShowStatus();
				Stop = true;
				RestoreWeights();
			}
		}
	}
	while (!Stop);
}


#ifdef VALIDATING
bool BPNetContainer::Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail))
{
	char Result[200];

	bool Fail = false;

	String spn,spv,stemp;

	sprintf(Result,"\nBPNetContainer Testing:\nObject of ClassName %s,ChildClassName %s",
			Object.ClassName(),Object.ChildClassName());
	if (!CallBack(Verbose,Result,Fail))
		return false;
	try
	{
//		Fail =  !BPNet::Test(Verbose,Object,CallBack);
		if (Fail)
			return true;
		if (Object.IsA(CBPNetContainer))
		{

			BPNetContainer *bp = (BPNetContainer *) &Object;
			bp->Clear();
			if (bp->Size()!=0)
			{
				sprintf(Result,"\nContainer Cleared but size is %d",bp->Size());
				Fail = true;
				if (!CallBack(Verbose,Result,Fail))
					return false;
			}
			else
			{
				if (Verbose)
					if (!CallBack(Verbose,"\nContainer Cleared ok",Fail))
						return false;
			}
			bp->AddLayer(2); 	// 2 inputs
			bp->AddLayer(4);     // 2 calcs
			bp->AddLayer(1);     // 1 output
		{
		bp->TestError = 0.1;
		bp->TrainError = 0.2;
		bp->Alpha = 0.3;
		bp->Eta = 0.4;
		bp->Gain =0.5;
		bp->Error = 0.6;
		for(int i=0;i<bp->GetAllWeightsSize();i++)
		{
			bp->AllWeights[i] = i+0.5;
		}

		XML x,x1;
		String s1,s2;
		BPNetContainer *npb;
			x.LoadFrom(*bp);
			x.SaveTo(s1);
			npb =(BPNetContainer *) x.CreateObjectFromNode(bp);
			x1.LoadFrom(*npb);
			delete npb;
			x1.SaveTo(s2);
			if (s1!=s2)
			{
			const char *c1 = s1;
			const char *c2 = s2;
				while(*c1==*c2)  				{	c1++; c2++; }

				sprintf(Result,"\nSave and Load from xml had error");
				Fail = true;
				if (!CallBack(Verbose,Result,Fail))
					return false;
			}
		}
		Fail =  !BPNet::Test(Verbose,Object,CallBack);
		if (Fail)
			return false;

		if (bp->Size()!=3)
            {
                Fail = true;
                sprintf(Result,"\n3 layers added but size is %d",bp->Size());
                if (!CallBack(Verbose,Result,Fail))
                    return false;
            }
            else if (Verbose)
                if (!CallBack(Verbose,"\n 3 Layers added, size == 3",Fail))
                    return false;

            /*	bp.SetSize(3);
				bp.SetLayerSize(0,2);	// 2 inputs
            	bp.SetLayerSize(1,2);	// 2 calcs
            	bp.SetLayerSize(2,1);	// 1 output
            */
            bp->SetAlpha(0.9);
            bp->SetEta(0.25);
            bp->SetGain(1);
            bp->RandomWeights();

            double _InData [4][2] = { {0,0},{0,1},{1,0},{1,1}};	//2 bit inputs
            double *InData = &_InData[0][0];
            double OutData[4];
            double TargetData[4] = {0,1,1,0};	// xor
            //double TargetData[4] = { 0,0,0,1};	// and
            //double TargetData[4] = {0,1,1,1};		// or
			//double TargetData[4] = {0,0,0,0};
			if ((Verbose) && (!CallBack(Verbose,"\nInitial Training (will take some time)\n",Fail)))
                return false;

			bp->STTrainNet(5000,4,InData,4,TargetData);
            bp->printWeights();
            for(int i=0;i<4;i++)
            {
                OutData[i] = 4500;
				bp->SimulateNet(&InData[i*2],&OutData[i],&TargetData[i],0);
				int v = (OutData[i]+0.5)>1 ? 1 : 0;
				Fail = v!=TargetData[i];
				sprintf(Result,"In %f,%f Expected %f Out %f\n",_InData[i][0],_InData[i][1],TargetData[i],OutData[i]);
				if (!CallBack(Verbose,Result,Fail))
					return false;
			}
			if ((Verbose) && (!CallBack(Verbose,"\nSaving to XML",Fail)))
				return false;
			MemStream fs;
			{
				XML x;
				x.LoadFrom(*bp);
				if ((Verbose) && (!CallBack(Verbose,"\nSaving CryXML to stream",Fail)))
					return false;
				fs.Open("backprop.xml","w");
				x.SaveTo(fs);
			}

			if ((Verbose) && (!CallBack(Verbose,"\nLoading a new CryXML from stream",Fail)))
				return false;

			{
				fs.SeekFromStart(0);
				XML x;
				x.LoadFrom(fs);
				if ((Verbose) && (!CallBack(Verbose,"\nCreating class from XML (factoryCreate",Fail)))
					return false;

				BPNetContainer *bp1 = (BPNetContainer *) x.CreateObjectFromNode(this);
				{
					XML y;
					MemStream fs1;
					y.LoadFrom(*bp1);
					y.SaveTo(fs1);
					if (fs!=fs1)
					{
						Fail = true;
						if (!CallBack(Verbose,"\nStreams differ after saving to xml to stream, loading from stream to xml, creating new instance, saving to xml to stream",Fail))
							return false;
					}
					if ((Verbose) &&
		     (!CallBack(Verbose,"\nStreams equal after saving to xml to stream, loading from stream to xml, creating new instance, saving to xml to stream",Fail)))
                        return false;
                }
                for(int i=0;i<4;i++)
                {
                    OutData[i] = 4500;
					bp1->SimulateNet(&InData[i*2],&OutData[i],&TargetData[i],0);
					int v = (OutData[i]+0.5)>1 ? 1 : 0;
					Fail = v!=TargetData[i];
					sprintf(Result,"In %f,%f Expected %f Out %f\n",_InData[i][0],_InData[i][1],TargetData[i],OutData[i]);
					if (!CallBack(Verbose,Result,Fail))
                        return false;
                }
                delete bp1;
            }
		}
	}
	catch(Exception &E)
	{

        sprintf(Result,"\nException Caught: %s\n",(const char *)E);
        Fail = true;
        if (!CallBack(Verbose,Result,Fail))
			return false;
    }
	return true;
}

#endif


