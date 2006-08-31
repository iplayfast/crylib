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
#include "CryObject.h"
#include "CryArray.h"
#include "ClassString.h"

#ifndef FUZZY_DEF
#define FUZZY_DEF

namespace Crystal   {
using namespace std;

#define TCryFuzzy    "CryFuzzy"


struct VC
{
  float Value;
  float Certainty;
};

/// A Fuzzy Logic Implimenation
class CryFuzzy : public CryArray
{
struct TFuzzyXY : public EmptyObject
{
    float x;
    float y;
};

 void SetValueXY(int i,float x, float y ); //Sets a point in a fuzzy table at index i
protected:
    TFuzzyXY *Index(int i) const { return (TFuzzyXY *) this->GetItem(i);}
    virtual void DestroyArrayItem(CryArray *Owner,EmptyObject *Item);	//derived class object destructor
    virtual EmptyObject *CreateArrayItem(CryArray *Owner,bool *IsCryObject);			//derived class object creator

public:
StdFunctionsNoDup(CryFuzzy,CryArray);
virtual CryObject *Dup()const;
    CryFuzzy();
	CryFuzzy(const CryFuzzy &c);
    CryFuzzy(const CryFuzzy *p);
    virtual ~CryFuzzy();

    int AddPoint(float x,float y);	// adds a point to the fuzzy table, returns index of point added
    float Value(float x) const;  // returns a y value for an x into the table
    virtual int SetValueAt(float x,float y); // Sets a point to the fuzzy table, returns index of point added
    float ValueAt(unsigned int idx) const; // returns a y value for the index into the table
    float IndexAt(unsigned int idx )const; // returns a x value for the index into the table
	float LowestRange()const; // returns the lowest x
    float HighestRange()const; // returns the highest x
    void SwapXY();
    float Equality(CryFuzzy &t)const;
    void Normalize(float factor);    // remove values that would normally fall within the range of factor
    void IncreaseSamples(); // add additional values (of the calculated value to data)
    void SaveFuzzToStream(int i,CryStream *ToStream) const; // write 1 datapoint to the stream

    void AppendFromStream(CryStream &Stream);
    int AppendFuzzFromStream(CryStream &Stream);

	void SimpleInhibit(float InhibitPercent);
	void CopyAndInhibit(CryFuzzy &Fuzzy);
	void CopyAndInhibitTemp(CryFuzzy &Fuzzy,CryFuzzy &Temp);

	 // will reduce all values so that v is the maximum (useful for converting raw data into value between the range 0.0 to 1.0
	void Range(float Floor,float Ceil);

	// these function operate on fuzzy values in the range of 0.0 to 1.0 (0==false, 1==true)
	float Or(const CryFuzzy &f, float v)const;
	float And(CryFuzzy f, float v)const;
	float XOr(CryFuzzy f, float v)const;
	float Not(float v)const;

   virtual bool LoadAsText(int i,CryString &FromStream);
   virtual bool SaveAsText(int i,CryString &ToStream) const;
   virtual bool LoadAsText(Iterator *i,CryString &FromStream)
   {
	return CryArray::LoadAsText(i,FromStream);
   }
   virtual bool SaveAsText(Iterator *i,CryString &ToStream) const
   {
	return CryArray::SaveAsText(i,ToStream);
   }
	void print(ostream &os);
//    friend ostream &operator<<( ostream &os, CryFuzzy& f );

  virtual CryObject *CreateItemType(const CryPropertyParser &PropertyName);
// derived class will handle the display in CryStream the objects contained in array (text assumed)
    virtual void SaveItemTo(const CryArray *Owner,EmptyObject *FromItem,CryStream &ToStream) const;
// derived class will handle the Creation of an Object from the stream
    virtual EmptyObject *LoadItemFrom(CryArray *Owner,EmptyObject *ToItem,CryStream &FromStream);

    bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
    virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
    virtual bool HasProperty(const CryPropertyParser &PropertyName)const;
	virtual int GetPropertyCount() const;
	virtual CryPropertyList* PropertyNames() const;
   virtual void CopyTo(CryArray &Dest) const { CryArray::CopyTo(Dest);  } //copies contents of this to Dest
    virtual void CopyTo(CryObject &Dest) const { CryArray::CopyTo(Dest);  }  //copies contents of this to Dest
    virtual void GetEleType(CryString &Result) const { Result = "TFuzzyXY"; }

#ifdef VALIDATING
virtual bool Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));
#endif
	virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;

};
} // end namespace Crystal

#ifdef __BORLANDC__
using namespace std;
	ostream & operator<<( std::ostream &os, Crystal::CryFuzzy& f );
#endif

#endif //FUZZY_DEF
