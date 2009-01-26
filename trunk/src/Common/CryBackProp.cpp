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

bool BackPropagateNetwork::SetPropertyAsObject(const Property *Value)
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
	return SetProperty(PropertyParser(Value->GetName()->AsPChar()),Value->GetValue(Result));
}

bool BackPropagateNetwork::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
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
