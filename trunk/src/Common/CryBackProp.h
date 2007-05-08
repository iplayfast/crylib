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
//#include "ClassXML.h"	// temporary

#include "ClassArray.h"
#include "ClassString.h"
#include "Utility.h"
#include <vector>
#ifndef BACKPROP_DEF
#define BACKPROP_DEF

#define BIAS          1
#define MIN_DOUBLE      -HUGE_VAL
#define MAX_DOUBLE      +HUGE_VAL


namespace Crystal   {
using namespace std;

#define CBackPropagateNetwork "BackPropagateNetwork"
// we are going to use the cry library strictly as an interface to a _BackPropagateNetwork
class _BackPropagateNetwork
{
friend class BackPropagateNetwork;
struct Layer
{
	int     LayerSize;     /* - number of units in this Layer       */
	vector <double> Weights;
	vector <double> WeightsSave;
	vector <double> dWeights;	// Delta Weights for momentum
	vector <double> Output;
	vector <double> Error;
public:
typedef vector <double>::iterator WeightsI;
typedef vector <double>::const_iterator cWeightsI;

	Layer(int Size,int PreviousSize)
				   { LayerSize = Size;
						int SizePS = Size * PreviousSize;
						for(int i=0;i<Size;i++)
						{
							Output.push_back(0.0);
							Error.push_back(0.0);
						}
						for(int i=0;i<SizePS;i++)
						{
/*							Weights.push_back(Size);		// useful for setting values to debug with
							WeightsSave.push_back(Size);
							dWeights.push_back(Size);
*/
							Weights.push_back(RandomDouble(-1,1));
							WeightsSave.push_back(RandomDouble(-1,1));
							dWeights.push_back(RandomDouble(-1,1));
						}
					}
};

vector <Layer> Layers;
typedef vector <Layer>::iterator LayerI;
typedef vector <Layer>::const_iterator cLayerI;
int LockLevel;
double TestError;
double TrainError;
double          Alpha;         // - momentum factor
double          Eta;           // - learning rate
double          Gain;          // - gain of sigmoid function
double          Error;         // - total net error
String Status;
public:
	Object *Parent;	// not used by this class, used by the callback, or other classes
	void (*CallBack)(Object *Parent,String &Status);	// must be set by an external class (as well as parent) in order to get a status when training
	_BackPropagateNetwork()
	{
		Alpha       = 0.9;
		Eta         = 0.25;
		Gain        = 1;
		TestError = TrainError = MAX_DOUBLE;
		LockLevel = 1;
		CallBack = 0;
	}
	int Size() { return Layers.size(); }
  void SetAlpha(double a) { Alpha = a; }
  void SetEta(double e) { Eta = e; }
  void SetGain(double g) { Gain = g; }
  void AddLayer(int Size)
  {
  int PreviousSize = 0;
  LayerI it = Layers.end();
	if (it!=Layers.begin())
	{
		it--;
		PreviousSize = it->LayerSize;
	}
	Layers.push_back(Layer(Size,PreviousSize));
  }
  ~_BackPropagateNetwork()
  {
  }
  void Propagate(int _From,int _To,double Gain)
  {
	LayerI From = Layers.begin();
	LayerI To = Layers.begin();
		for(int i=0;i<_From;i++)
			From++;
		for(int i=0;i<_To;i++)
			To++;
		Layer::WeightsI w = To->Weights.begin();
		Layer::WeightsI o = To->Output.begin();
		for (int i=0; i<To->LayerSize; i++)
		{
			double Sum = 0.0;
			Layer::WeightsI f = From->Output.begin();
			for (int j=0; j<From->LayerSize; j++)
			{
				Sum += *w++ * *f++;
			}
			Sum = 1.0 / (1.0 + (exp(-Gain * Sum)));
			if (errno==ERANGE)
				Sum = MAX_DOUBLE;
			*o++ = Sum;
		}
  }

	void BackPropagate(int _From,int _To,double Gain)
	{
		int  i,j;
		double Out, Err;
		if (_From <1)
			return;	// can't do this
		LayerI From = Layers.begin();
		LayerI To = Layers.begin();
		for(int i=0;i<_From;i++)
			From++;
		for(int i=0;i<_To;i++)
			To++;
		Layer::WeightsI f = From->Weights.begin();
		Layer::WeightsI _fe = From->Error.begin();
		Layer::WeightsI o = To->Output.begin();
		Layer::WeightsI te = To->Error.begin();
		for(int i=0;i<To->LayerSize;i++)
		{
			Out = *o++;
			Err = 0.0;
			Layer::WeightsI fe = From->Error.begin();
			for(int j=0;j<From->LayerSize;j++)
			{
				Err += *f++ * *fe++;
			}
			*te++ = Gain * Out * (1- Out) * Err;
		}
	}
	void ComputeOutputError(const double *Target)
	{
		int  i;
		double Out, Err;

		double Error = 0;
		LayerI OutputLayer = Layers.end();
		OutputLayer--;	// at last layer now
		Layer::WeightsI e = OutputLayer->Error.begin();
		Layer::WeightsI o = OutputLayer->Output.begin();
		Out = Err = 0.0;
		for (i=0; i<OutputLayer->LayerSize; i++)
		{
			Out = *o++;
			Err = Target[i]-Out;
			*e++ = Gain * Out * (1-Out) * Err;
			Error += 0.5 * Err * Err;
		}
		TestError += Error;
		//printf("ComputeOutputError %3.3f\n",Net->Error);
	}
	void AdjustWeights()
	{
		LayerI Layer2 = Layers.begin();
		LayerI Layer1 = Layers.begin();
			Layer1++;
		for(unsigned int l = 1;l<Layers.size();l++)
		{
			if (l>=(unsigned) LockLevel)
			{
				Layer::WeightsI dw = Layer1->dWeights.begin();
				Layer::WeightsI w = Layer1->Weights.begin();
				Layer::WeightsI e = Layer1->Error.begin();
				for(int i=0;i<Layer1->LayerSize;i++)
				{
				Layer::WeightsI o = Layer2->Output.begin();
					for(int j=0;j<Layer2->LayerSize;j++)
					{
					double Out = *o++;
					double Err = *e;
					*w = *w + (Eta *Err *Out + Alpha * *dw);
					*dw = Eta * Err * Out;
					w++;
					dw++;
					}
					e++;
				}
			}
			Layer1++;
			Layer2++;
		}
	}
void PropagateNet()
{
	unsigned int l,s = Layers.size()-1;
	for (l=0; l<s; l++)
		Propagate(l,l+1,Gain);
}

void BackPropagateNet()
{
	for (int l=Layers.size()-1; l>1; l--)
		BackPropagate(l,l-1,Gain);
}
void GetOutput(double *Output)
{
	LayerI Layer = Layers.end();
	Layer--;
	Layer::WeightsI out = Layer->Output.begin();
	for (int i=0; i<Layer->LayerSize; i++)
		*Output++ = *out++;
}

void SetInput(const double *Input)
{
	LayerI Layer = Layers.begin();
	Layer::WeightsI in = Layer->Output.begin();
	for (int i=0; i<Layer->LayerSize; i++)
		*in++ = *Input++;
}

void RandomWeights()
{
	for(LayerI start = Layers.begin();start<Layers.end();start++)
	{
		for(Layer::WeightsI w = start->Weights.begin();w<start->Weights.end();w++)
		{
//        	*w = start->LayerSize;	// useful for setting values to debug with
			*w =RandomDouble(-1,1);
		}
	}
}

void SaveWeights()
{
	int l,i,j;
	LayerI start = Layers.begin();
	int PreviousLayerSize = start->LayerSize;
		start++;
	LayerI end = Layers.end();

	double *w,*ws;
	while(start < end)
	{
		Layer::WeightsI w = start->Weights.begin();
		Layer::WeightsI ws = start->WeightsSave.begin();
		for (i=1; i<start->LayerSize; i++)
			for (j=0; j < PreviousLayerSize; j++)
				*ws++ = *w++;
		PreviousLayerSize = start->LayerSize;
		start++;
	}
}

void RestoreWeights()
{
	int l,i,j;
	LayerI start = Layers.begin();
	int PreviousLayerSize = start->LayerSize;
		start++;
	LayerI end = Layers.end();

	double *w,*ws;
	while(start < end)
	{
		Layer::WeightsI w = start->Weights.begin();
		Layer::WeightsI ws = start->WeightsSave.begin();
		for (i=1; i<start->LayerSize; i++)
			for (j=0; j < PreviousLayerSize; j++)
				*w++ = *ws++;
		PreviousLayerSize = start->LayerSize;
		start++;
	}
}
void SimulateNet(const double *Input, double *Output,const double *Target, bool Training)
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

void TrainNet(int Epochs,int LengthIn,double *SampleIn,int LengthOut,double *SampleOut)
{
	int  j, i;
	double *Output;
	LayerI it = Layers.end();
		it--;
	int OutputSize = it->LayerSize;
		it = Layers.begin();
	int InputSize = it->LayerSize;

	Output = new double[OutputSize];
	TestError = 0.0;
	for (i=0; i<Epochs; i++)
	{
		int Sample = RandomInt(0,LengthIn-1);
		j =Sample * InputSize;	// adjust for input width
		int k = Sample * OutputSize;
		SimulateNet(&SampleIn[j], Output, &SampleOut[k], true);
	}
	delete []Output;
}

void STTrainNet(int EPochs,int LengthIn,double *SampleIn,int LengthOut,double *SampleOut)
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
		if (CallBack)
			CallBack(Parent,Status);
			Status.printf(" - saving Weights ...\n");
		if (CallBack)
			CallBack(Parent,Status);
			MinTestError = TestError;
			SaveWeights();
		}
		else
		{
			if ((TestError < 1) || (TestError > 1.2 * MinTestError))
			{
				Status.printf(" - stopping Training and restoring Weights ...\n");
				if (CallBack)
					CallBack(Parent,Status);
				Stop = true;
				RestoreWeights();
			}
		}
	}
	while (!Stop);
}
};

#define CBackPropagateNetwork "BackPropagateNetwork"

// interface class
class BackPropagateNetwork	: public Object
{

_BackPropagateNetwork *nn;
typedef _BackPropagateNetwork::LayerI BPLayerI;
typedef _BackPropagateNetwork::Layer::WeightsI	BPWeightsI;

public:
	BackPropagateNetwork() { nn = new _BackPropagateNetwork(); }
	~BackPropagateNetwork() { delete nn; }
	StdFunctionsNoDup(BackPropagateNetwork,Object);
	PropertyList *PropertyNames() const
	{
	PropertyList *result = Object::PropertyNames();
		result->AddPropertyByName("LockLevel",this);
		result->AddPropertyByName("TestError",this);
		result->AddPropertyByName("TrainError",this);
		result->AddPropertyByName("Alpha",this);         // momentum factor
		result->AddPropertyByName("Eta",this);		   //  learning rate
		result->AddPropertyByName("Gain",this);		  //  gain of sigmoid function
		result->AddPropertyByName("Error",this);   		 //  total net error
		result->AddPropertyByName("Layers",this);
		return result;
	}
const char *GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	if (PropertyName=="LockLevel")
	{
		Result.printf("%d",nn->LockLevel);
		return (const char *) Result;
	}
	if (PropertyName=="TestError")
	{
		Result.printf("%f",nn->TestError);
		return (const char *) Result;
	}
	if (PropertyName=="TrainError")
	{
		Result.printf("%f",nn->TrainError);
		return (const char *) Result;
	}
	if (PropertyName=="Alpha")
	{
		Result.printf("%f",nn->Alpha);
		return (const char *) Result;
	}
	if (PropertyName=="Eta")
	{
		Result.printf("%f",nn->Eta);
		return (const char *) Result;
	}
	if (PropertyName=="Gain")
	{
		Result.printf("%f",nn->Gain);
		return (const char *) Result;
	}
	if (PropertyName=="Error")
	{
		Result.printf("%f",nn->Error);
		return (const char *) Result;
	}
	if (PropertyName=="Layers")
	{
		Result = "[]";  // if Result != what is returned, it's a special situation
		return "*";
	}
	return Object::GetProperty(PropertyName,Result);
}

bool HasProperty(const PropertyParser &PropertyName) const
{
	return  PropertyName=="LockLevel" ||
			PropertyName=="TestError" ||
			PropertyName=="TrainError" ||
			PropertyName=="Alpha"	||
			PropertyName=="Eta"	||
			PropertyName=="Gain"	||
			PropertyName=="Error"	||
			PropertyName=="Layers" ||
			Object::HasProperty(PropertyName);
}

bool GetIsPropertyContainer(const PropertyParser &PropertyName) const
{
	if (PropertyName=="Layers") return true;
	return Object::GetIsPropertyContainer(PropertyName);
}
Object *GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const
{
	if (PropertyName!="Layers") return Object::GetCopyOfPropertyAsObject(PropertyName);
	List *l = new List();
	{    // saves the size of each layer
	IntArray *ia = new IntArray(nn->Layers.size());
		int i=0;
		for(BPLayerI From = nn->Layers.begin();From < nn->Layers.end();From++,i++)
			ia->SetValue(i,From->LayerSize);
		l->AddOwned(ia);
	}
	// for each layer
	for(BPLayerI From = nn->Layers.begin();From <nn->Layers.end();From++)
	{
		List *nl = new List();	// save the weights
		for(BPWeightsI dw = From->dWeights.begin(),w = From->Weights.begin();w<From->Weights.end();w++,dw++)
		{
		String *v = new String();	// save weight[i] first
			v->printf("%lf ",*w);
			nl->AddOwned(v);
			v = new String();
			v->printf("%lf ",*dw);	// save dweight[i] next
			nl->AddOwned(v);
		}
		l->AddOwned(nl);
	}
	Property *p = new Property();
	p->SetName("Layers");
	p->SetValueOwned(l);	// must wait until l is fully loaded before setting the property
	return p;
}

bool SetPropertyAsObject(const Property *Value)
{
	String Result;
	if (*Value->GetName()=="Layers")
	{
	bool GettingLayerSetup = true;
	const Object *o = Value->GetValue();
		if (o->IsA(CList))
		{
		List *layer = (List *)o;
		List::Iterator *iLayer = layer->CreateIterator();
		try
		{
			if (iLayer->GotoFirstObject(CIntArray))
			{
				// first get the layer definitions
					IntArray *Sizes = (IntArray*) iLayer->Get();
					IntArray::Iterator *iSizes = Sizes->CreateIterator();
					try
					{
						if (iSizes->GotoFirstNonObject())
						do
						{
						int size;
							size = * (int *) iSizes->Get();
							nn->AddLayer(size);
						} while(iSizes->GotoNextNonObject());
						Sizes->DeleteIterator(iSizes);
					}
					catch(Exception &e)
					{
						Sizes->DeleteIterator(iSizes);
						throw e;
					}
			}
			// now get the layer weights
			// for each layer
			for(BPLayerI From = nn->Layers.begin();From <nn->Layers.end();From++)
			{
			int LayerLength = From->LayerSize;
			int LayerSize = From->Weights.end() - From->Weights.begin();

				if (!iLayer->GotoNextObject(CList)) return false;
				List *Weights = (List *) iLayer->Get();
				List::Iterator *iWeights = Weights->CreateIterator();
				try
				{
					for(BPWeightsI dw = From->dWeights.begin(),w = From->Weights.begin();w<From->Weights.end();w++,dw++)
					{
					double Weight;
						String *s = (String *) iWeights->Get();
						s->scanf("%lf ",&Weight);	// set weight[i] first
						*w = Weight;
						if (iWeights->GotoNextObject(CString))
						{
							String *s = (String *) iWeights->Get();
							s->scanf("%lf ",&Weight);	// set dweight[i] next
							*dw = Weight;
						}
						else break;
						if (!iWeights->GotoNextObject(CString))
							break;
					}
					Weights->DeleteIterator(iWeights);
				}
				catch(Exception &e)
				{
					Weights->DeleteIterator(iWeights);
					throw e;
				}

			}
			layer->DeleteIterator(iLayer);
		}
		catch(Exception &e)
		{
			layer->DeleteIterator(iLayer);
			throw e;
		}
		return true;
		}
	}
	return SetProperty(Value->GetName()->AsPChar(),Value->GetValue(Result));
}

bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
String s = PropertyValue;
float v;
	if (PropertyName=="LockLevel")
	{
	int v;
		s.scanf("%d",&v);
		nn->LockLevel=v;
		return true;
	}
	if (PropertyName=="TestError")
	{
		s.scanf("%f",&v);
		nn->TestError = v;
		return true;
	}
	if (PropertyName=="TrainError")
	{
		s.scanf("%f",&v);
		nn->TrainError = v;
		return true;
	}
	if (PropertyName=="Alpha")
	{
		s.scanf("%f",&v);
		nn->Alpha = v;
		return true;
	}
	if (PropertyName=="Eta")
	{
		s.scanf("%f",&v);
		nn->Eta = v;
		return true;
	}
	if (PropertyName=="Gain")
	{
		s.scanf("%f",&v);
		nn->Gain = v;
		return true;
	}
	if (PropertyName=="Error")
	{
		s.scanf("%f",&v);
		nn->Error = v;
		return true;
	}
	if (PropertyName=="Layers")
	{
//		s.scanf("%f",&nn->Layers);
		return true;
	}
	return Object::SetProperty(PropertyName,PropertyValue);
}
virtual Object *Create(const PropertyParser &PropertyName,Object *Parent)
{
	if (PropertyName==CBackPropagateNetwork)
		return new BackPropagateNetwork();
	if (PropertyName==CProperty)	// not a direct desendent but used in the properties so we need to be able to create it
		return new Property();

	return Object::Create(PropertyName,Parent);
}
virtual Object *Create(Stream &FromStream)
{
	return Object::Create(FromStream);
};


#ifdef VALIDATING
bool (*pCallBack)(bool Verbose,const char *Result,bool fail);
static void SimpleCallBack(Object *Parent,String &Status)
{
BackPropagateNetwork *p = (BackPropagateNetwork*)Parent;
	p->pCallBack(true,Status.AsPChar(),false);
	Status = "";
};

bool Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail))
{
	pCallBack = CallBack;
	nn->Parent = this;
	nn->CallBack = SimpleCallBack;

_BackPropagateNetwork::LayerI it = nn->Layers.end();

bool Fail = false;
char Result[400];
	try
	{
		nn->AddLayer(2); 	// 2 inputs
		nn->AddLayer(4);     // 2 calcs
		nn->AddLayer(1);     // 1 output
		nn->TestError = 0.1;
		nn->TrainError = 0.2;
		nn->Alpha = 0.3;
		nn->Eta = 0.4;
		nn->Gain =0.5;
		nn->Error = 0.6;
		if (nn->Size()!=3)
			{
				Fail = true;
				sprintf(Result,"\n3 layers added but size is %d",nn->Size());
				if (!CallBack(Verbose,Result,Fail))
					return false;
			}
			else if (Verbose)
				if (!CallBack(Verbose,"\n 3 Layers added, size == 3",Fail))
					return false;

		 nn->SetAlpha(0.9);
		 nn->SetEta(0.25);
		 nn->SetGain(1);
		 nn->RandomWeights();

		double _InData [4][2] = { {0,0},{0,1},{1,0},{1,1}};	//2 bit inputs
		double *InData = &_InData[0][0];
		double OutData[4];
		double TargetData[4] = {0,1,1,0};	// xor
			//double TargetData[4] = { 0,0,0,1};	// and
			//double TargetData[4] = {0,1,1,1};		// or
			//double TargetData[4] = {0,0,0,0};
		if (!Object::Test(Verbose,*this,CallBack))
			return false;
// comment out this return to do some training and verifying
//		return true;
		if ((Verbose) && (!CallBack(Verbose,"\nInitial Training (will take some time)\n",Fail)))
			return false;
		nn->STTrainNet(5000,4,InData,4,TargetData);
//		nn->printWeights();
		for(int i=0;i<4;i++)
		{
			OutData[i] = 4500;
			nn->SimulateNet(&InData[i*2],&OutData[i],&TargetData[i],0);
			int v = (OutData[i]+0.5)>1 ? 1 : 0;
			Fail = v!=TargetData[i];
			sprintf(Result,"In %f,%f Expected %f Out %f\n",_InData[i][0],_InData[i][1],TargetData[i],OutData[i]);
			if (!CallBack(Verbose,Result,Fail))
				return false;
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
#endif // VALIDATING
// pass through functions
  virtual size_t Size() const { return nn->Size() + Object::Size(); }
  void SetAlpha(double a) { nn->SetAlpha(a); }
  void SetEta(double e) { nn->SetEta(e); }
  void SetGain(double g) { nn->SetGain(g); }
  void AddLayer(int Size) { nn->AddLayer(Size); }
  void Propagate(int _From,int _To,double Gain) { nn->Propagate(_From,_To,Gain); }
  void BackPropagate(int _From,int _To,double Gain) { nn->BackPropagate(_From,_To,Gain); }
  void ComputeOutputError(const double *Target) { nn->ComputeOutputError(Target); }
  void AdjustWeights() { nn->AdjustWeights(); }
  void PropagateNet() { nn->PropagateNet(); }
  void BackPropagateNet() { nn->BackPropagateNet(); }
  void GetOutput(double *Output) { nn->GetOutput(Output); }
  void SetInput(const double *Input) { nn->SetInput(Input); }
  void RandomWeights() { nn->RandomWeights(); }
  void SaveWeights() { nn->SaveWeights(); }
  void RestoreWeights() { nn->RestoreWeights(); }
  void SimulateNet(const double *Input, double *Output,const double *Target, bool Training)
  {
	nn->SimulateNet(Input,Output,Target,Training);
  }
  void TrainNet(int Epochs,int LengthIn,double *SampleIn,int LengthOut,double *SampleOut)
  {
	nn->TrainNet(Epochs,LengthIn,SampleIn,LengthOut,SampleOut);
  }

  void STTrainNet(int EPochs,int LengthIn,double *SampleIn,int LengthOut,double *SampleOut)
  {
	nn->STTrainNet(EPochs,LengthIn,SampleIn,LengthOut,SampleOut);
  }

};

}; //namespace Crystal

#endif  // BACKPROP_DEF
