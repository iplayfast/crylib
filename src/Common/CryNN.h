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

/* The idea:
   The class get's created with a known number of inputs and outputs, and an idea of the complexity of the problem.
   It assigns a number of weights specified by some formula. eg (Inputs + output) * Complexity * 10
   With each weight is a Value. The weights determine how important the value is. Also along with 
   each weight is an index.
   There are no layers, it's a glob instead.
   So each node has the structure of 
   struct node
   {
   double count;	// other nodes weights get added here, compared to trigger
   double triggerpoint;	// point at which value is triggered. 
   double weight;	//the strength of the value
   double value;	// the value output  by this node
   int index1;		// the destination of the output
   int index2;
   int outputindex;	// initially -1
   bool Used;
   };
   

   Initially all values are random, with indexes limited by the upper limit of the number of nodes.
   The inputs go to random nodes. outputindex is set to -1 and Used is set to false.
   
   Output nodes have only a value which is the weight times the value of the input nodes.

   During training an input is presented to the inputnodes. They trigger whatever indexes have been
assigned using only index1.  Whenever a Node is used, the used flag is set to true. If a Used flag is set then the process stops. (That particular input is looping).
The output is tested against various nodes and the closest are then outputindex'd to the output nodes. A total errorlevel is set. Then the same process is repeated using index2 (clearing the used flags first).
If the errorlevel for index2

*/

#include <math.h>
#include <string.h>
#include "CryObject.h"

#define BIAS          1
#define MIN_DOUBLE      -HUGE_VAL
#define MAX_DOUBLE      +HUGE_VAL

class CryNN
{
    struct _blob 
    {
	double Count;	// other nodes (weights * values) get added here, 
	                //compared to triggerpoint, this value is used 
	//		  as the final output as well
	double TriggerPoint;	// point at which value is triggered. 
	double Weight;	//the strength of the value
	double Value;	// the value output  by this node
	int Index;		// the destination of the output of this node (to another node)
	int Used;
    };

_blob *Blob;
_blob *TempBlob;
int *OutputIndexes;
int *InputIndexes;
int *TempInputIndexes;
int BlobSize,InputSize,OutputSize;
int NodesTriggered; // gets set during TraceInputs
double ErrorLevel;
public:
double GetErrorLevel() const { return ErrorLevel; }
CryNN(size_t _Input,size_t _Output,size_t Complexity = 100)
{
    BlobSize = (_Input + _Output) * Complexity;
    InputSize = _Input;
    OutputSize = _Output;
    Blob = new _blob[BlobSize];
    TempBlob = new _blob[BlobSize];
    OutputIndexes = new int[_Output];
    InputIndexes = new int[_Input];
    TempInputIndexes = new int[_Input];
    Randomize();
//    ErrorLevel = MAX_DOUBLE;
};
~CryNN() {
    delete []TempInputIndexes;
    delete []InputIndexes;
    delete []OutputIndexes;
    delete []Blob;
	delete []TempBlob; }
void SaveToTemp() { memcpy(TempBlob,Blob,BlobSize * sizeof(_blob)); }
void RestoreFromTemp() { memcpy(Blob,TempBlob,BlobSize * sizeof(_blob)); }
void Reset() { for(int i=0;i<BlobSize;i++) {Blob[i].Count = 0; Blob[i].Used = 0; } }
void Train(int TrainingSets,double *Inputs,double *ExpectedOutputs);
void Run(double *InputData,double *Results)
     { TraceInputs(InputData); for(int i=0;i<OutputSize;i++) Results[i] = Blob[OutputIndexes[i]].Count; }
private:
void Randomize();
void FindBestOutputs(double *ExpectedOutputs);
int FindBestOutput(double ExpectedOutput);
int TraceInputs(double *Inputs);    // returns the time (in loops) taken to work
void AdjustBlobIndexes(int EveryNth);
void AdjustInputIndexes(int EveryNth);
void AdjustWeights(double PlusMinusValue);
void AdjustTriggerPoints(double PlusMinusValue);
void AdjustValues(double PlusMinusValue);
double CalcErrorLevel(double *ExpectedOutputs);
double CalcSetErrorLevel(int TrainingSets,double *Inputs,double *ExpectedOutputs);
};
