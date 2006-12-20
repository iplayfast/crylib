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

#include "CryNN.h"
#include "Utility.h"
using namespace Crystal;

#define MAXANNEAL 1000
double CryNN::CalcErrorLevel(double *ExpectedOutputs)
{
   ErrorLevel = 0;
    for(int i=0;i<OutputSize;i++)
    {
	double Out = Blob[OutputIndexes[i]].Count;
	double Error = ExpectedOutputs[i] - Out;
	ErrorLevel += 0.5 * Error * Error;
    }
    return ErrorLevel;
}
double CryNN::CalcSetErrorLevel(int TrainingSets,double *Inputs,double *ExpectedOutputs)
{
double e,r = 0;
   for(int i=0;i<TrainingSets;i++)
   {
    Reset();
    TraceInputs(&Inputs[i*InputSize]);
    e = CalcErrorLevel(&ExpectedOutputs[i*OutputSize]);
    r += 0.5 * e * e;
   }
   ErrorLevel = r;
   return ErrorLevel;
}

void CryNN::Train(int TrainingSets,double *Inputs,double *ExpectedOutputs)
{
double  OldErrorLevel = CalcSetErrorLevel(TrainingSets,Inputs,ExpectedOutputs);


  if (OldErrorLevel==0.0)
		return;
  SaveToTemp();
// do adjustments and compare to the copy
int Annealing = MAXANNEAL;
int Momentum = 1;
    while(Annealing)
    {
        if (NodesTriggered==0)  // no point in addjusting weigghts and values until some triggering happens
        {
          switch(RandomInt(0,2))
          {
          case 0: AdjustInputIndexes(MAXANNEAL - Annealing); break;
          case 1: AdjustBlobIndexes(MAXANNEAL - Annealing); break;
          case 2: AdjustTriggerPoints((double)Annealing / MAXANNEAL); break;
          }
        }
        else
        {
          switch(RandomInt(2,4))
          {
//          case 0: AdjustInputIndexes(MAXANNEAL - Annealing); break;
//          case 1: AdjustBlobIndexes(MAXANNEAL - Annealing); break;
          case 2: AdjustWeights((double)Annealing / MAXANNEAL); break;
          case 3: AdjustValues((double)Annealing / MAXANNEAL); break;
          case 4: AdjustTriggerPoints((double)Annealing / MAXANNEAL); break;
          }
        }
        TraceInputs(Inputs);
	FindBestOutputs(ExpectedOutputs);
        {
        double  OldErrorLevel = ErrorLevel;
            CalcSetErrorLevel(TrainingSets,Inputs,ExpectedOutputs);
            if (OldErrorLevel > ErrorLevel) // we've found a better solution
            {
        		SaveToTemp();
                Annealing += Momentum;
                if (Momentum>1) Momentum--;
		        if (ErrorLevel==0.0) Annealing = 0;
            }
            else
            {
                ErrorLevel = OldErrorLevel;
                Annealing--;
                Momentum++;
            }

        }
        Reset();
    }
    RestoreFromTemp();
}

void CryNN::Randomize()
{
    for(int i=0;i<BlobSize;i++)
    {
        Blob[i].Index = RandomInt(0,BlobSize-1);
        Blob[i].Count = RandomDouble(0,1.0);
        Blob[i].Value = RandomDouble(0,1.0);
        Blob[i].TriggerPoint = RandomDouble(0,1.0);
	Blob[i].Weight = RandomDouble(0,1.0);
	Blob[i].Used = 0;
    }
    for(int i=0;i<OutputSize;i++)
        OutputIndexes[i] = RandomInt(0,BlobSize-1);
    AdjustInputIndexes(0);
}

void CryNN::FindBestOutputs(double *ExpectedOutputs)
{
    for(int i=0;i<OutputSize;i++)
    {
    int n = FindBestOutput(ExpectedOutputs[i]);
        OutputIndexes[i] = n;
    }
}
int CryNN::FindBestOutput(double ExpectedOutput)
{
int Result = 0;
double Diff = fabs(ExpectedOutput - Blob[0].Count);
    for(int i=1;i<BlobSize;i++)
    {
    double NewDiff = fabs(ExpectedOutput - Blob[i].Count);
        if (NewDiff<Diff)
        {
            Result = i;
            Diff = NewDiff;
        }
    }
    return Result;
}

int CryNN::TraceInputs(double *Inputs)
{
    for(int i=0;i<InputSize;i++)
    {
        Blob[InputIndexes[i]].Count = Inputs[i];
        TempInputIndexes[i] = InputIndexes[i];
    }
bool done = false;
int Result = 0;
    NodesTriggered = 0;
    while(!done)
    {
        done = true;
        Result++;
        for(int i=0;i<InputSize;i++)
        {
            if (TempInputIndexes[i]>-1)  // if still valid
            {
            int j = TempInputIndexes[i];
                if (Blob[j].Count >= Blob[j].TriggerPoint)
                {
                    NodesTriggered++;
                    done = false;
                    Blob[Blob[j].Index].Count += Blob[j].Weight * Blob[j].Value;
                    Blob[j].Used++;
                    if (Blob[j].Used>InputSize*2)
                        TempInputIndexes[i] = -1;    // possible looping detected
                    else
                        TempInputIndexes[i] = Blob[j].Index; // set up for the next node

                }
                else TempInputIndexes[i] = -1;   // we are done with this node
            }
        }
    }
    return Result;
}

void CryNN::AdjustBlobIndexes(int EveryNth)
{
if (EveryNth<0) EveryNth *=-1;
    for(int c = EveryNth,i=0;i<BlobSize;i++)
    {
        if (c==0)
        {
            c = EveryNth;
            int idx;
            do
            {
                idx = RandomInt(0,BlobSize-1);
                Blob[i].Index = idx;
            } while(idx==i);    // a node can't point to itself
        }
        else c--;
    }
}

void CryNN::AdjustInputIndexes(int EveryNth)
{
if (EveryNth<0) EveryNth *=-1;
    for(int c = EveryNth,i=0;i<InputSize;i++)
    {
        if (c==0)
        {
            c = EveryNth;
            InputIndexes[i] = RandomInt(0,BlobSize-1);
        }
        else c--;
    }
}

void CryNN::AdjustWeights(double PlusMinusValue)
{
    for(int i=0;i<BlobSize;i++)
        Blob[i].Weight = Blob[i].Weight + RandomDouble(-PlusMinusValue,PlusMinusValue);
}

void CryNN::AdjustTriggerPoints(double PlusMinusValue)
{
double tp;
    for(int i=0;i<BlobSize;i++)
    {
        tp = Blob[i].TriggerPoint + RandomDouble(-PlusMinusValue,PlusMinusValue);
        if (tp<0.0)
            tp = 0.0;
        Blob[i].TriggerPoint = tp;
    }
}

void CryNN::AdjustValues(double PlusMinusValue)
{
    for(int i=0;i<BlobSize;i++)
        Blob[i].Value = Blob[i].Value + RandomDouble(-PlusMinusValue,PlusMinusValue);
}

