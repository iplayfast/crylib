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
// fuzzy

#include <stdio.h>
#include <iostream>
#include <math.h>
#include "ClassObject.h"
#include "ClassArray.h"
#include "ClassString.h"

#ifndef FUZZY_DEF
#define FUZZY_DEF

namespace Crystal   {
using namespace std;

#define CFuzzy    "Fuzzy"


struct VC
{
  float Value;
  float Certainty;
};

/// A Fuzzy Logic Implimenation
class Fuzzy : public Array
{
struct TFuzzyXY : public EmptyObject
{
    float x;
    float y;
};

 void SetValueXY(int i,float x, float y ); //Sets a point in a fuzzy table at index i
protected:
    TFuzzyXY *Index(int i) const { return (TFuzzyXY *) this->GetItem(i);}
    virtual void DestroyArrayItem(Array *Owner,EmptyObject *Item);	//derived class object destructor
    virtual EmptyObject *CreateArrayItem(Array *Owner,bool *IsObject);			//derived class object creator

public:
StdFunctionsNoDup(Fuzzy,Array);
virtual Object *Dup()const;
    Fuzzy();
	Fuzzy(const Fuzzy &c);
    Fuzzy(const Fuzzy *p);
    virtual ~Fuzzy();
/*! AddPoint will add a point to the Fuzzy Table, if the point already exists, it will average the old point with
   The new point
   Returns the index into the table of the point just added (or updated)
   */
	int AddPoint(float x,float y);
	float Value(float x) const;  // returns a y value for an x into the table
	/*! SetValue will set the value of a point in a table, or add it if it isn't currently in the table
	returns the index into the table of the point just set or added */
	virtual int SetValue(float x,float y); 
    float ValueAt(unsigned int idx) const; // returns a y value for the index into the table
    float IndexAt(unsigned int idx )const; // returns a x value for the index into the table
	float LowestRange()const; // returns the lowest x
    float HighestRange()const; // returns the highest x
    void SwapXY();
    float Equality(Fuzzy &t)const;
    void Normalize(float factor);    // remove values that would normally fall within the range of factor
    void IncreaseSamples(); // add additional values (of the calculated value to data)
    void SaveFuzzToStream(int i,Stream *ToStream) const; // write 1 datapoint to the stream

    void AppendFromStream(Stream &Stream);
    int AppendFuzzFromStream(Stream &Stream);
	Object *Create(const PropertyParser &PropertyName,Object *Parent)
	{
		if (PropertyName==CFuzzy)
			return new Fuzzy();
		return Array::Create(PropertyName,Parent);
	}
	Object *Create(Stream &FromStream)
	{
		return Array::Create(FromStream);
	}

	void SimpleInhibit(float InhibitPercent);
	void CopyAndInhibit(Fuzzy &Fuzzy);
	void CopyAndInhibitTemp(Fuzzy &Fuzzy,Fuzzy &Temp);

	 // will reduce all values so that v is the maximum (useful for converting raw data into value between the range 0.0 to 1.0
	void Range(float Floor,float Ceil);

	// these function operate on fuzzy values in the range of 0.0 to 1.0 (0==false, 1==true)
	float Or(const Fuzzy &f, float v)const;
	float And(Fuzzy f, float v)const;
	float XOr(Fuzzy f, float v)const;
	float Not(float v)const;

   virtual bool LoadAsText(int i,String &FromStream);
   virtual bool SaveAsText(int i,String &ToStream) const;
   virtual bool LoadAsText(Iterator *i,String &FromStream)
   {
	return Array::LoadAsText(i,FromStream);
   }
   virtual bool SaveAsText(Iterator *i,String &ToStream) const
   {
	return Array::SaveAsText(i,ToStream);
   }
	void print(ostream &os);
//    friend ostream &operator<<( ostream &os, CryFuzzy& f );

  virtual Object *CreateItemType(const PropertyParser &PropertyName);
// derived class will handle the display in CryStream the objects contained in array (text assumed)
    virtual void SaveItemTo(const Array *Owner,EmptyObject *FromItem,Stream &ToStream) const;
// derived class will handle the Creation of an Object from the stream
	virtual EmptyObject *LoadItemFrom(Array *Owner,EmptyObject *ToItem,Stream &FromStream);
	bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);

	virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;
	virtual bool HasProperty(const PropertyParser &PropertyName)const;
	virtual int GetPropertyCount() const;
	virtual PropertyList* PropertyNames() const;
   virtual void CopyTo(Array &Dest) const { Array::CopyTo(Dest);  } //copies contents of this to Dest
	virtual void CopyTo(Object &Dest) const { Array::CopyTo(Dest);  }  //copies contents of this to Dest
	virtual void GetEleType(String &Result) const { Result = "TFuzzyXY"; }
#ifdef VALIDATING
virtual bool Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));
#endif
	virtual FunctionDefList *GetFunctions(const char *Type=0) const;
};
} // end namespace Crystal

#ifdef __BORLANDC__
using namespace std;
	ostream & operator<<( std::ostream &os, Crystal::Fuzzy& f );
#endif

#endif //FUZZY_DEF
