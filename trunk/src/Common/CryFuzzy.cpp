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


#include "CryFuzzy.h"
#include "ClassProperty.h"
#include "ClassFunction.h"

using namespace Crystal;
using namespace std;
/*void CryFuzzy::SetActualLength(int NewLength)
{
    if (NewLength!=fActualLength)
    {
    std::vector<TFuzzyXY> temp = fxy;

        fActualLength = NewLength;
        fxy.resize(NewLength);
    }
} */

/*void CryFuzzy::SetUsedLength(int UsedLength)
{
int i,t;
    if (fxy.size() < UsedLength)
        SetActualLength(UsedLength*2);
    if (fUsedLength<UsedLength)
    {
        t = fxy.size()-1;
        if (t>UsedLength)
          t = UsedLength;
        for(i = fUsedLength;i<=t;i++)
        {
            fxy[i].x = 0.0;
            fxy[i].y = 0.0;
        }
    }
    fUsedLength = UsedLength;
} */


Fuzzy::Fuzzy(const Fuzzy &c)
{
TFuzzyXY *t;
    if (&c==this)
        return;
    SetMax(c.GetMax());
    SetAllowResize(c.GetAllowResize());
    for(unsigned int i =0;i<c.Count();i++)
    {
        t = c.Index(i);
        AddPoint(t->x,t->y);
    }
}

Fuzzy::Fuzzy()
{
}

Fuzzy::~Fuzzy()
{
    while(Count())
        DeleteItem(0);
}

Fuzzy::Fuzzy(const Fuzzy *p)
{
TFuzzyXY *t;
    if (p==this)
        return;

    SetMax(p->GetMax());
    SetAllowResize(p->GetAllowResize());
    for(unsigned int i =0;i<p->Count();i++)
    {
        t = p->Index(i);
        AddPoint(t->x,t->y);
    }
}

// derived class will handle the display in CryStream the objects contained in array (text assumed)
void Fuzzy::SaveItemTo(const Array *Owner,EmptyObject *FromItem,Stream &ToStream) const
{
TFuzzyXY *t = (TFuzzyXY *)FromItem;
    ToStream.printf("%f,%f",t->x,t->y);
}

// derived class will handle the Creation of an Object from the stream
EmptyObject *Fuzzy::LoadItemFrom(Array *Owner,EmptyObject *Item,Stream &FromStream)
{
TFuzzyXY *t = (TFuzzyXY *)Item;
    t->x = t->y = 0.0;
    FromStream.scanf("%f,%f",&t->x,&t->y);
    return t;
}

void Fuzzy::DestroyArrayItem(Array *Owner,EmptyObject *Item)
{
    delete Item;
}
EmptyObject *Fuzzy::CreateArrayItem(Array *Owner,bool *IsObject)
{
TFuzzyXY *t = new TFuzzyXY;
    t->x = t->y = 0.0;
    *IsObject = false;
    return t;
}

	// adds a point to the fuzzy table, returns index of point added
int Fuzzy::AddPoint(float x,float y)
{
unsigned int i;
TFuzzyXY *t,*n;
    for( i = 0;i< Count();i++)
    {
        t = Index(i);
        if (t->x== x)
        {
    		t->y = (t->y + y) / (2.0);//average original point and new point
            return i;
        };
    };
    SetSize(Count()+1);
//    {
//    TFuzzyXY tmp;
//        tmp.x = 0; tmp.y = 0;
//    insert(end(),tmp);
//    }

	for(i = Count()-1,t = Index(i);;i--)	// insertion sort
	{
      	if ((i == 0) || (Index(i-1)->x < x))
        {
            t->x = x;
            t->y = y;
            return i;
        }
        else
		{
            n = Index(i-1);
        	t->x = n->x;
            t->y = n->y;
            t = n;
        };
     };
//     return i; // never gets here
}

float Fuzzy::Value(float x)const  // returns a y value for an x into the table
{
int i,l,Previous,Next;
float	m;
TFuzzyXY *t;
	l = Count();
    Previous = 0;
    Next = 0;
    if (l==0) return -1.0;
	for(i = 0;i< l;i++)
    {
        t = Index(i);
		if (t->x < x) // is already sorted so no need for "and ((x - xy[i].x) < (x - xy[Previous].x))"
    	    Previous = i;
		if (t->x == x) // is already sorted so no need for "and ((x - xy[i].x) < (x - xy[Previous].x))"
        {
    	    Previous = i;
            Next = i;
            break;
        };
		if (t->x > x) break;
        Next = i+1;
    };
    if (Next>l-1) Next = l-1;
    if (Next==Previous)
  	    return Index(Next)->y;
	else
    {
    TFuzzyXY *p,*n;
        p = Index(Previous);
        n = Index(Next);
          // result = mx + b;
          m = (n->y - p->y) / (n->x - p->x);
          return m * (x - p->x) + p->y;
    };
}
int Fuzzy::SetValue(float x,float y) // Sets a point to the fuzzy table, returns index of point added
{
unsigned int i;
TFuzzyXY *t;

	// if already in list then just set it
	for (i = 0;i<Count();i++)
	{
		t = Index(i);
		if (t->x > x)
			break;
		if (t->x==x)
		{
			t->y = y;
			return i;
		};
	};
	// not in list so add it
	return AddPoint(x,y);
};

 // returns a y value for the index into the table
float Fuzzy::ValueAt(unsigned int idx)const
{
	return Index(idx)->y;
}
 // returns a x value for the index into the table
float Fuzzy::IndexAt(unsigned int idx )const
{
    return Index(idx)->x;
}
float Fuzzy::LowestRange() const// returns the lowest x
{
    return Index(0)->x;
}
float Fuzzy::HighestRange() const // returns the highest x
{
  return Index(Count()-1)->x;
}
void Fuzzy::SwapXY()
{
  for (unsigned int i=0;i<Count();i++)
    SetValueXY(i,ValueAt(i),IndexAt(i));
}

float Fuzzy::Equality(Fuzzy &t)const
{
unsigned int i;
float	v,v1,result;
  	result = 0.0;
    if (t.Count()<Count())
    {
    	for(i = 0;i<t.Count();i++)
        {
      	    v = Value(t.IndexAt(i));
            v1 = t.ValueAt(i);
            if (v==0)  v = 0.00001;
            if (v1==0)  v1 = 0.00001;
            v = 1 / v;
            v1 = 1 / v1;
      	    result = result + v - v1;
        };
		if (t.Count()>1)
     		result = result / (t.Count()-1);
    }
    else
    {
    	for(i = 0;i<Count();i++)
        {
          	v1 = t.Value(IndexAt(i));
            v = ValueAt(i);
            if (v==0)  v = 0.00001;
            if (v1==0)  v1 = 0.00001;
            if (v1<0.00001)  v1 = 0.00001;
            if (v<0.00001)  v = 0.00001;
            v = 1 / v;
            v1 = 1 / v1;
          	result = result + v - v1;
          };
      	    if (Count()>1)
	      	    result = result / (Count()-1);
    };
    if (result < 0.0)  result = -result;
    result = (1.0 - result) * 100;
    return result;
};

// remove values that would normally fall within the range of factor
void Fuzzy::Normalize(float factor)
{
unsigned int i,j;
float	r,va,v,s1,s2,factor2;

  i = 1;
  factor2 = factor * 2;
  while(i<Count()-1)
  {
  	s1 = (ValueAt(i) - ValueAt(i-1));
    s2 = (ValueAt(i+1) - ValueAt(i));
    if ((fabs(s1)<=factor) && (fabs(s2)<=factor))// does it fall within limit?
    {   // yes we can remove it
        DeleteItem(i);
    }
    else
  	i++;
  };
  i = 1;
TFuzzyXY *n,*p;
  while(i<Count()-1)
  {
  	va = ValueAt(i) - factor;
    n = Index(i+1);
    p = Index(i-1);
    s1 = (n->y - p->y);
    s2 = (n->x - p->x);
    if (s2<0.0001)
         s2 = 0.0001;
    if (s2!=0)
    	v = Index(i)->x * s1 / s2;
    else
    	v = Index(i)->x * s1 / 0.0001;
    v = v - factor;
    if ((s1==0) || ((v<va) && (v+factor2>=va)) || ((v>=va) && (v-factor2<=va)))
    {
        DeleteItem(i);
        i = 1;
    }
    else
        i++;
  };
};

// add additional values (of the calculated value to data)
void Fuzzy::IncreaseSamples()
{
unsigned int i;
float	x1,x2,y;
	for(i=0;i<Count()-1;i+=2)
    {
        x1 = IndexAt(i);
        x2 = IndexAt(i+1);
        x1 = (x1+x2) /2;
        SetValue(x1,Value(x1));
    }
}
// write 1 datapoint to the stream
void Fuzzy::SaveFuzzToStream(int i,Stream *ToStream) const
{
int l;
float x,y;
   	x = Index(i)->x;
   	y = Index(i)->y;
    switch(ToStream->GetMode())
    {
      case SObject:
        ToStream->Write((const char *)&x,sizeof(x));
        ToStream->Write((const char *)&y,sizeof(y));
        break;
      case SText:
        ToStream->printf("%f %f ",x,y);
        break;
    }
}
bool Fuzzy::LoadAsText(int i,String &FromStream)
{
float x,y;	// i is ignored as Fuzzy sorts input values and figures out the index itself
	FromStream.scanf("%f %f ",&x,&y);
	return SetValue(x,y);
}
bool Fuzzy::SaveAsText(int i,String &ToStream) const
{
float x,y;
   	x = Index(i)->x;
   	y = Index(i)->y;
        ToStream.printf("%f %f ",x,y);
    return true;
}


void Fuzzy::AppendFromStream(Stream &FromStream)
{
int l,i,c;
    switch(FromStream.GetMode())
    {
      case SObject:
        FromStream.Read((char *)&l,sizeof(l));
        break;
      case SText:
        FromStream.scanf("%d ",&l);
        break;
    }
    for(i=0;i<l;i++)
	    AppendFuzzFromStream(FromStream);
}

int Fuzzy::AppendFuzzFromStream(Stream &FromStream)
{
float x,y;
    switch(FromStream.GetMode())
    {
      case SObject:
        FromStream.Read((char *)&x,sizeof(x));
        FromStream.Read((char *)&y,sizeof(y));
        break;
      case SText:
        FromStream.scanf("%f %f ",&x,&y);
        break;
    }
    return SetValue(x,y);
}

void Fuzzy::SetValueXY(int i,float x, float y ) //Sets a point in a fuzzy table at index i
{
    Index(i)->x = x;  Index(i)->y = y;
}

void Fuzzy::SimpleInhibit(float InhibitPercent)
{
unsigned int i;
float	v1,v2,v3,v4;
	v1 = ValueAt(0);
  v2 = ValueAt(1);
  v3 = ValueAt(2);
  v4 = ValueAt(3);
//  InhibitPercent = 100.0 / InhibitPercent;
  SetValueXY(0,IndexAt(0),0);
  for(i = 1;i<Count();i++)
  {
  	v1 = (v1 * InhibitPercent) + (v3 * InhibitPercent);
    SetValueXY(i,IndexAt(i),v2- v1);
    v1 = v2; v2 = v3; v3 = v4;
    if (i<(Count()-4))
	    v4 = ValueAt(i+3);
  }
}
void Fuzzy::CopyAndInhibit(Fuzzy &_Fuzzy)
{
Fuzzy temp;
	CopyAndInhibitTemp(_Fuzzy,temp);
 	Normalize(1);
}
void Fuzzy::CopyAndInhibitTemp(Fuzzy &_Fuzzy,Fuzzy &Temp)
{
float l,h,r,d;
	Clear();
	l = _Fuzzy.LowestRange();
	h = _Fuzzy.HighestRange();
	if ((l<0) || (h<0)) return;
	d = (h - l) / (_Fuzzy.Count() * 10);
	r = l;

//    Temp.SetUsedLength(ceil(h - l));	// make space needed
	Temp.Clear();// set the space used right now
//    SetUsedLength(ceil(h-l)*10);
//    SetUsedLength(0);
	Clear();
	while(r<h)
	{
		AddPoint(r,_Fuzzy.Value(r));
		r = r + d;
	};
	r = l + d;
	while(r<(h-d))		// do inhibit from previous and next (if all 3 are equal  value=0)
	{
		Temp.AddPoint(r,Value(r) - (Value(r-d)/2.0) - (Value(r+d)/2.0));
		r = r + d;
	};
	Clear();
	r = l + d;
	AddPoint(r,Temp.Value(r));
	AddPoint(h-d,Temp.Value(h-d));
	while(r<(h-d))
	{
		AddPoint(r,Temp.Value(r-d)-Temp.Value(r) + Temp.Value(r) - Temp.Value(r+d));
		r = r + d;
	};
}

void Fuzzy::Range(float Floor,float Ceil)
{
float v;
unsigned int Size = Count();
    if (Size==0) return;
float min = Index(0)->y;
unsigned int i;
    for(i=1;i<Size;i++)
    {
        v = Index(i)->y;
        if (v<min) min = v;
    }
float max = Index(0)->y - min + Floor;
TFuzzyXY *t;
    for(i = 0;i<Size;i++)
    {
        t = Index(i);
        v = t->y - min + Floor;
        if (v>max) max = v;
        t->y = v;
    }
    max = Ceil / max;
    for(i = 0;i<Size;i++)
    {
        t = Index(i);
        t->y = t->y * max;
    }
}

float Fuzzy::Or(const Fuzzy &f, float v) const
{
float r1,r2;
    r1 = f.Value(v);
    r2 = Value(v);
    if (r1>r2) return r1;
    else return r2;
}

float Fuzzy::And(Fuzzy f, float v) const
{
float r1,r2;
    r1 = f.Value(v);
    r2 = Value(v);
    if (r1<r2) return r1;
    else return r2;
}

float Fuzzy::XOr(Fuzzy f, float v) const
{
float r1,r2;
    r1 = f.Value(v);
    r2 = Value(v);
    if (r1<r2) return r2 - r1;
    else return r1 - r2;
}

float Fuzzy::Not(float v) const
{
    return 1.0 - Value(v);
}

Object *Fuzzy::Dup()const // creates a duplicate of this object
{
Fuzzy *n = new Fuzzy(this);
	return (Object *)n;
}

Object *Fuzzy::CreateItemType(const PropertyParser &PropertyName)
{
	if (PropertyName==CFuzzy)
		return new Fuzzy();
	else return Array::CreateItemType(PropertyName);

}
PropertyList *Fuzzy::PropertyNames() const
{
	PropertyList *n = Container::PropertyNames();// Skip Array class to avoid Size (Fuzzy takes care of that)
			n->AddPropertyByName("Size",this);
	return n;
}

bool Fuzzy::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
	return Array::SetProperty(PropertyName,PropertyValue);
}
const char *Fuzzy::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
	return Array::GetProperty(PropertyName,Result);
}
bool Fuzzy::HasProperty(const PropertyParser &PropertyName)const
{
	return Array::HasProperty(PropertyName); // pass it to base class, see if it knows anything about it
}
int Fuzzy::GetPropertyCount() const { return Container::GetPropertyCount()  + 1; }    // Value is a property

FunctionDefList *Fuzzy::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return Array::GetFunctions(Type);
	// otherwise get any functions in subclasses
	FunctionDefList *l = Array::GetFunctions();

    String s;
    s += "// Class CryFuzzy;";
    s += "virtual bool IsA(const char *ClassName) const;";
    s += "int AddPoint(float x,float y);";
    s += "float Value(float x) const;";
    s += "virtual int SetValueAt(float x,float y);";
    s += "float ValueAt(unsigned int idx) const;";
    s += "float IndexAt(unsigned int idx )const;";
  	s += "float LowestRange()const;";
    s += "float HighestRange()const;";
    s += "void SwapXY();";
    s += "float Equality(CryFuzzy &t)const;";;
    s += "void Normalize(float factor);";
    s += "void IncreaseSamples();";
    s += "void SaveFuzzToStream(int i,CryStream *ToStream) const;";
    s += "void AppendFromStream(CryStream &Stream);";
    s += "int AppendFuzzFromStream(CryStream &Stream);";
    s += "void SimpleInhibit(float InhibitPercent);";
    s += "void CopyAndInhibit(CryFuzzy &Fuzzy);";
  	s += "void CopyAndInhibitTemp(CryFuzzy &Fuzzy,CryFuzzy &Temp);";
    s += "void Range(float Floor,float Ceil);";
    s += "float Or(const CryFuzzy &f, float v)const;";
    s += "float And(CryFuzzy f, float v)const;";
    s += "float XOr(CryFuzzy f, float v)const;";
    s += "float Not(float v)const;";
    s += "virtual bool LoadAsText(int i,CryString &FromStream);";
    s += "virtual bool SaveAsText(int i,CryString &ToStream) const;";
    s += "virtual bool LoadAsText(Iterator *i,CryString &FromStream);";
    s += "virtual bool SaveAsText(Iterator *i,CryString &ToStream) const;";
	s += "void print(ostream &os);";
    s += "virtual Object *Dup()const;";
    s += "const char* ClassName() const;";
    s += "virtual const char* ChildClassName() const;";
    s += "virtual Object *CreateItemType(const char *Name);";
	s += "virtual void SaveItemTo(const CryArray *Owner,EmptyObject *FromItem,CryStream &ToStream) const;";
    s += "virtual EmptyObject *LoadItemFrom(CryArray *Owner,EmptyObject *ToItem,CryStream &FromStream);";
    s += "bool SetProperty(const CryPropertyParser &PropertyName,const CryString &PropertyValue);";
    s += "virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;";
    s += "virtual bool HasProperty(const CryPropertyParser &PropertyName)const;";
    s += "virtual int GetPropertyCount() const;";
    s += "virtual CryList *PropertyNames() const;";
    s += "virtual void CopyTo(CryArray &Dest) const;";
    s += "virtual void CopyTo(Object &Dest) const;";
    s += "virtual void GetEleType(CryString &Result) const;";
    s += "virtual bool IsAbstract() const;";
#ifdef VALIDATING
    s += "virtual bool Test(bool Verbose,Object &_Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));";
#endif
    s += "virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;";
    l->LoadFromString(s,";");
    return l;
};

#ifdef VALIDATING
bool Fuzzy::Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail))
{
Fuzzy f;
String Result;
bool fail = false;
float v1,v2;
	f.AddPoint(1,10);
	f.AddPoint(100,1000);
	fail = f.Value(10)!=100;
	Result = "Estimate value correct";
	if (!CallBack(Verbose,Result,fail))
		return false;
	v1 = f.ValueAt(0);
	v2 = f.ValueAt(1);
	fail = !(v1==10 && v2==1000);
	Result = "AddPoint and ValueAt values correct";
	if (!CallBack(Verbose,Result,fail))
		return false;
	v1 = f.IndexAt(0);
	v2 = f.IndexAt(1);
	fail = !(v1==1 && v2==100);
	Result = "IndexAt values correct";
	if (!CallBack(Verbose,Result,fail))
		return false;
	f.SetValue(1,5);
	fail = f.ValueAt(0)!=5;
	Result = "SetValue values correct";
	if (!CallBack(Verbose,Result,fail))
		return false;
	f.AddPoint(1,15);       	// should average the old value (5) with the new one (15) and get 10
	fail = f.ValueAt(0)!=10;
	Result = "AddPoint average of old values correct";
	if (!CallBack(Verbose,Result,fail))
		return false;

	v1 = f.LowestRange();
	fail = v1 !=1;
	Result = "LowestRange correct";
	if (!CallBack(Verbose,Result,fail))
		return false;
	v2 = f.HighestRange();
	fail = v2 !=100;
	Result = "HighestRange correct";
	if (!CallBack(Verbose,Result,fail))
		return false;
	f.SwapXY();
	fail = !((f.ValueAt(0)==1) && (f.ValueAt(1)==100) && (f.IndexAt(0)==10) && f.IndexAt(1)==1000);
	Result = "SwapXY correct";
	if (!CallBack(Verbose,Result,fail))
		return false;

	f.Clear();
	for(int i=0;i<10;i++)
    	f.AddPoint(i*10,i+10+i*2);
	return Array::Test(Verbose,f,CallBack) && Array::Test(Verbose,Object,CallBack) ;
}
#endif

void Fuzzy::print(ostream &os)
{
TFuzzyXY *t;
      for(unsigned int i=0;i<Count();i++)
      {
        t = Index(i);
        os << "[" << t->x <<"," << t->y << "]" <<
            (i < (Count()-1) ? ", " : " ");
      }
}
using namespace std;
ostream &operator<<( ostream &os, Crystal::Fuzzy& ds )
{
        ds.print ( os );
   return os;
}
