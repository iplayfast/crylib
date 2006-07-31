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

#include "CryPattern.h"
using namespace Crystal;

//-------------------------------------------------------------------
// Strategy
//-------------------------------------------------------------------
int Strategy::DoStrategy() const
{
    return -1;
}
int Strategy::DoStrategy(CryObject *Sender) const
{
    return -1;
}

//-------------------------------------------------------------------
// StrategyHolder
//-------------------------------------------------------------------
int StrategyHolder::GetMaxCount() const
{
    return MaxCount;
}
StrategyHolder::StrategyHolder(unsigned int NumStrategies)
{
    MaxCount = NumStrategies;
    OwnsStrategies = false;
	Array = new const Strategy*[NumStrategies];
    for(unsigned int i=0;i<NumStrategies;i++)
        Array[i] = this;// default do nothing values;

};

StrategyHolder::~StrategyHolder()
{
    if (OwnsStrategies)
        for(unsigned int i = 0;i<MaxCount;i++)
        {
            const Strategy *s = GetStrategy(i);
            if (s!=this)
            {
                delete(s);
                SetStrategy(i,this);
            }
        }
    delete []Array;
}
void StrategyHolder::SetOwnsStrategies(bool Owns)
{
    OwnsStrategies = Owns;
}
bool StrategyHolder::GetOwnsStrategies() const
{
    return OwnsStrategies;
}
void StrategyHolder::SetStrategy(unsigned int StrategyIndex,const Strategy *s)
{
#ifdef RangeChecking
    if (/*(StrategyIndex<0) || */(StrategyIndex>=MaxCount))
        throw CryException("Out of Range of StrategyHolder");
#endif

    if (s)
        Array[StrategyIndex] = s;
    else
        Array[StrategyIndex] = this;	// do nothing
}


const Strategy *StrategyHolder::GetStrategy(unsigned int StrategyIndex) const
{
#ifdef RangeChecking
    if (/*(StrategyIndex<0) || */(StrategyIndex>=MaxCount))
		throw CryException("Out of Range of StrategyHolder");
#endif

    return Array[StrategyIndex];
}

int StrategyHolder::DoStrategy() const
{
    return Strategy::DoStrategy();
}

int StrategyHolder::DoStrategy(CryObject *o) const
{
    return Strategy::DoStrategy(o);
}

void StrategyHolder::DoStrategy(int StrategyIndex) const
{
#ifdef RangeChecking
    if ((StrategyIndex<0) || ((unsigned int)StrategyIndex>=MaxCount))
        throw CryException("Out of Range of StrategyHolder");
#endif

    do
    {
        StrategyIndex = Array[StrategyIndex]->DoStrategy();
#ifdef RangeChecking

        if ((unsigned int)StrategyIndex>=MaxCount)
            throw CryException("Out of Range of StrategyHolder");
#endif

    }
    while(StrategyIndex > -1);
}

//-------------------------------------------------------------------
// StrategyHolderSender
//-------------------------------------------------------------------

StrategyHolderSender::StrategyHolderSender(unsigned int NumStrategies) : StrategyHolder(NumStrategies)
{}
int StrategyHolderSender::DoStrategy() const
{
    return StrategyHolder::DoStrategy();
}
int StrategyHolderSender::DoStrategy(CryObject *o) const
{
    return StrategyHolder::DoStrategy(o);
}

void StrategyHolderSender::DoStrategy(int StrategyIndex,CryObject *Sender) const
{
#ifdef RangeChecking
    if ((StrategyIndex<0) || (StrategyIndex>=GetMaxCount()))
        throw CryException("Out of Range of StrategyHolderSender");
#endif

    do
    {
        StrategyIndex = GetStrategy(StrategyIndex)->DoStrategy(Sender);
#ifdef RangeChecking

        if (StrategyIndex>=GetMaxCount())
            throw CryException("Out of Range of StrategyHolderSender");
#endif

    }
    while(StrategyIndex > -1);
}

#ifdef VALIDATING

#define _FLY	0
#define _MOVE	1

TestStrategy::Ducks::Ducks()
{
    Sitting = true;
}
bool TestStrategy::Ducks::GetSitting()
{
    return Sitting;
}
bool TestStrategy::Ducks::SetSitting(bool V)
{
    return Sitting = V;
}
bool TestStrategy::Ducks::IsA(const char *ClassName) const
{
    if (strcmp(ClassName,"Duck")==0)
        return true;
    else
        return false;
}
int TestStrategy::DucksFly::DoStrategy() const
{
    printf("I'm Flying!\n");
    return -1;
}
int TestStrategy::DucksFly::DoStrategy(CryObject *d) const
{
    printf("I'm Flying!\n");
    return -1;
}

int TestStrategy::DucksNoFly::DoStrategy() const
{
    printf("I can't fly!\n");
    return -1;
}
int TestStrategy::DucksNoFly::DoStrategy(CryObject *) const
{
    return DoStrategy();
}
int TestStrategy::DucksMove::DoStrategy() const
{
    printf("I'm moving in some fashion\n");
    return -1;
}
int TestStrategy::DucksMove::DoStrategy(CryObject *d) const
{
    if (d->IsA("Duck"))
    {
        if (((Ducks *)d)->GetSitting())
            printf("I'm Waddling\n");
        else
            return _FLY;	// tell the strategy holder to do the _FLY strategy
        return -1;
    }
    return -1;
}

TestStrategy::TestStrategy()
{
    StrategyHolder h(2);
    StrategyHolderSender hs(2);
    DucksFly df;
    DucksNoFly dnf;
    DucksMove dm;
    Ducks d;		//  normally it would make sense for these strategies to be contained inside
    //the duck but for testing purposes this will do.
    h.DoStrategy(_MOVE);	// nothing should happen as no strategies have been set
    h.SetStrategy(_FLY,&df);	// set dsome strategies
    h.SetStrategy(_MOVE,&dm);
    h.DoStrategy(_MOVE);	// shouldn't work since our move strategy requires the object passed to it to see the state
    h.DoStrategy(_FLY);	// should work "I'm Flying!"
    h.SetStrategy(_FLY,&dnf);
    h.DoStrategy(_MOVE);	// nothing should happen for same reason as above
    h.DoStrategy(_FLY);	// should work "I can't fly!"

    hs.SetStrategy(_FLY,&df);	// set dsome strategies
    hs.SetStrategy(_MOVE,&dm);
    d.SetSitting(true);		// a sitting duck if moved will waddle
    hs.DoStrategy(_MOVE,&d);	// should waddle
    d.SetSitting(false);
    hs.DoStrategy(_MOVE,&d);	// should fly
};
#endif




//-------------------------------------------------------------------
// Observer
//-------------------------------------------------------------------

Observer::Observer()
{}


Observer::~Observer()
{}

void Observer::NotifyObservers(Observable *Observed,int ObserverID)
{
    // do something
};

void Observer::NotifyObservers(Observable *Observed)
{
    // do something
};

//-------------------------------------------------------------------
// Observable
//-------------------------------------------------------------------


Observable::Observable()
{
    Name = "Unknown";
    Changed = false;
}
void Observable::SetName(const char *_Name)
{
    Name = _Name;
}

Observable::~Observable()
{
    Iterator *I = _CreateIterator();
    if (I->GotoFirst())
    {
        do
        {
            Observer *b = (Observer *)I->Get();
            UnRegisterObserver(b);
        }
        while(I->GotoFirst());// always delete from first of list so iterator is kept in sync
    }
    DeleteIterator(I);
}
/*bool Observable::IsA(const char *_Name) const
{
    return Name == _Name;
}
*/
void Observable::NotifyObservers(int ObserverID)
{
    if (!Changed)
        return;

    Iterator *I = _CreateIterator();
    if (I->GotoFirst())
    {
        do
        {
            Observer *b = (Observer *)I->Get();
            b->NotifyObservers(this,ObserverID);
        }
        while(I->GotoNext());
    }
    DeleteIterator(I);
    Changed = false;
}

void Observable::NotifyObservers()
{
    if (!Changed)
        return;
    Iterator *I = _CreateIterator();
    if (I->GotoFirst())
    {
        do
        {
            Observer *b = (Observer *)I->Get();
            b->NotifyObservers(this);
        }
        while(I->GotoNext());
    }
    DeleteIterator(I);
    Changed = false;
}

void Observable::RegisterObserver(Observer *_Observer,bool Owned)
{
    if (Owned)
        AddOwned(_Observer);
    else
        Add(_Observer);
}
void Observable::UnRegisterObserver(Observer *_Observer)
{
    Remove(_Observer);
}

void Observable::SetChanged()
{
    Changed = true;
}

#ifdef VALIDATING
#include <iostream>
#include <cstdlib>
TestObserver::TestObserver()
{
    Weather w;
    w.SetName("Weather");
    w.RegisterObserver(new Forcast(),true);
    w.RegisterObserver(new CurrentConditions(),true);
    w.setMeasurements(4.4,24.5,22.5);
    w.setMeasurements(1.4,24.5,32.5);

}
void TestObserver::CurrentConditions::NotifyObservers(Observable *Observed)
{
    if (Observed->IsA("Weather"))
    {
        Weather *w = (Weather *)Observed;
        temp = w->getTemp();
        humidity = w->getHumidity();
        pressure = w->getPressure();
        Display();
    }
}
void TestObserver::CurrentConditions::NotifyObservers(Observable *Observed,int status)
{}

void TestObserver::CurrentConditions::Display()
{
    cout << "Current conditions: " << temp << "F degrees and " << humidity << "% humidity";
    cout << "Hello, world!" << endl;
}
void TestObserver::Forcast::Display()
{
    cout << "Forcasting.... " << temp << "F degrees and " << humidity << "% humidity";
    cout << "Hello, world!" << endl;
}
TestObserver::Weather::Weather() : Observable()
{}
float TestObserver::Weather::getTemp()
{
    return temp;
}
float TestObserver::Weather::getHumidity()
{
    return humidity;
}
float TestObserver::Weather::getPressure()
{
    return pressure;
}
void TestObserver::Weather::measurementsChanged()
{
    SetChanged();
    NotifyObservers();
}
void TestObserver::Weather::setMeasurements(float _temp,float hum,float press)
{
    temp = _temp;
    humidity = hum;
    pressure = press;
    measurementsChanged();
}

#endif

//-------------------------------------------------------------------
// Decorator
//-------------------------------------------------------------------

Decorator::Decorator(Decorator *n)
{
    Next = n;
}
Decorator::~Decorator()
{
    if (Next)
        delete Next;
    Next = 0;
}
void Decorator::Decorate()
{
    if (Next)
        Next->Decorate();
}

#ifdef VALIDATING
TestDecorator::Animal::Animal(char *_Name) : Decorator()
{
    Name = _Name;
}
void TestDecorator::Animal::Decorate()
{
    Decorator::Decorate();
    printf("My name is %s\n",Name.AsPChar());
}
TestDecorator::Sleeping::Sleeping(Decorator *n) : Decorator(n)
{ }
void TestDecorator::Sleeping::Decorate()
{
    Decorator::Decorate();
    printf("I'm sleeping\n");
}
TestDecorator::Running::Running(Decorator *n) : Decorator(n)
{}
void TestDecorator::Running::Decorate()
{
    printf("I'm running\n");
    Decorator::Decorate();
}
TestDecorator::Panting::Panting(Decorator *n) : Running(n)
{}
void TestDecorator::Panting::Decorate()
{
    printf("I'm Panting\n");
    Decorator::Decorate();
}
TestDecorator::Eating::Eating(char *_What,Decorator *n) : Decorator(n)
{
    What = new CryString(_What);
}
TestDecorator::Eating::~Eating()
{
    delete What;
}
void TestDecorator::Eating::Decorate()
{
    printf("I'm eating %s\n",What->AsPChar());
    Decorator::Decorate();
}
TestDecorator::TestDecorator()
{
    Decorator *Puma = new Eating("Elephant",new Panting(new Running(new Animal("Puma"))));

    Puma->Decorate();
    delete Puma;
};

#endif



/*CryObject *Factory::Create(const CryPropertyParser &PropertyName,CryObject *Parent)
{
    if (PropertyName==0)
        return 0;  // Create nothing
    // basic Cryobject creation
    if (CryObject::CanCreate(PropertyName))
        return CryObject::Create(PropertyName,Parent);
    if (Parent && Parent->CanCreate(PropertyName))
        return Parent->Create(PropertyName,Parent);
    return 0;
}
*/
void CryFactory::DeleteFactoryDescribed(const char *Description)
{
    CryFactory **OldArray = Array;
    int _MaxCount = MaxCount;
    Array = new CryFactory*[0];
    MaxCount = 0;
    for(int i=_MaxCount-1;i>-1;i--)// do in reverse order so that when added is in correct order
    {
        if (strcmp(OldArray[i]->Describe(),Description)==0)
            delete OldArray[i];
        else
            AddFactory(OldArray[i]);
    }
    delete []OldArray;
};
void CryFactory::DeleteHeldFactories()
{
    for(int i=0;i<MaxCount;i++)
        delete Array[i];
    MaxCount = 0;
    delete []Array;
	Array = new CryFactory*[0];
};
CryFactory::~CryFactory()
{
	DeleteHeldFactories();
	delete []Array;
}

EmptyObject *CryFactory::GetAtIterator(const Iterator *I) const
{
	FactoryIterator *fI = (FactoryIterator *)I;
	CryFactory *cf = (CryFactory *) fI->GetOrigContainer();
	return cf->Array[fI->Index];
};

// IsCryObject, IsOwned and Size are not used
void CryFactory::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size) 
{
	FactoryIterator *fI = (FactoryIterator *)I;
	CryFactory *cf = (CryFactory *) fI->GetOrigContainer();
	cf->Array[fI->Index] =(CryFactory *) Item;
}


CryFactory *CryFactory::FindFactory(const char *FactoryName) const
{
    for(int i=0;i<MaxCount;i++)
    {
        if (Array[i]->IsA(FactoryName))
            return Array[i];
    }
    return 0;
}

/// Newest factory is always added as the first
void CryFactory::AddFactory(CryFactory *f)
{
    MaxCount++;
    CryFactory **NewArray = new CryFactory*[MaxCount];
    for(int i=0;i<MaxCount-1;i++)
        NewArray[i+1] = Array[i];
    NewArray[0] = f;
    delete []Array;
    Array = NewArray;
    //    for(int i=0;i<MaxCount;i++)
    //      Array[i]->Describe();
}

CryObject *CryFactory::Create(const CryPropertyParser &PropertyName,CryObject *Parent)
{
    // first Factory that can create what the thing matches returns thing
    for(int i=0;i<MaxCount;i++)
    {
        //    const char *describe = Array[i]->Describe();
        CryObject *Return = Array[i]->Create(PropertyName,Parent);
        if (Return)
            return Return;
    }
    return 0;
};
CryPropertyList *CryFactory::PropertyNames() const
{
    CryPropertyList *pn = CryContainer::PropertyNames();
    return pn;
    //  pn->AddOwned(new CryString("Values"));
};

int CryFactory::GetPropertyCount() const
{
    return CryContainer::GetPropertyCount();
};

CryObject *CryFactory::GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const
{
    return CryContainer::GetCopyOfPropertyAsObject(PropertyName);
}


const char * CryFactory::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
    Result.Clear();
    if (PropertyName=="Values")   // intercept crycontainer's property for our own
    {
        Result = "[]";  // if Result != what is returned, it's a special situation
        return "*";
    }
    else
        return CryContainer::GetProperty(PropertyName,Result);
};
bool  CryFactory::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
	return CryContainer::SetProperty(PropertyName,PropertyValue);
};

CryObject *CryOFactory::Create(const CryPropertyParser &PropertyName,CryObject *Parent)
{
    CryObject *Return = CryFactory::Create(PropertyName,Parent);
    if (!Return)
        return CryObject::Create(PropertyName,Parent);
    else
        return Return;
};

CryList *CryFactory::GetProducts() const
{
    CryString s;
    CryList *l = new CryList();
    for(int i=0;i<MaxCount;i++)
    {
        s += Array[i]->Describe();
        s += "|";

        if (Array[i]->IsA(TCryFactory))
            l->AddOwned(Array[i]->GetProducts());
    }
    s.LoadListFromString("|",l);
    return l;
};

CryObject *CryOFactory::Create(const char *FactoryName,const CryPropertyParser &PropertyName,CryObject *Parent)
{
    return CryFactory::Create(FactoryName,PropertyName,Parent);
}


/// returns a CryObject or 0
CryObject *CryFactory::Create(const char *FactoryName,const CryPropertyParser &PropertyName,CryObject *Parent)
{
    CryObject *Return;
    if (PropertyName=="")
        return 0;  // Create nothing
    /// first try the different held factories to see if their description matches the Factory Name we are looking for
    for(int i=0;i<MaxCount;i++)
        if ((strcmp(Array[i]->Describe(),FactoryName)==0) && (Array[i]->CanCreate(PropertyName)))
            return Array[i]->Create(PropertyName,Parent);

    /// if can not match exact factory so ask each factory to make it, see which one succeeds (if any)
    return Create(PropertyName,Parent);
}
bool CryFactory::CanCreate(const CryPropertyParser &PropertyName) const
{
    CompositeIterator a(this);
    if (a.GotoFirst())
    {
        do
        {
            if (GetFactory(a.GetIterator())->CanCreate(PropertyName))
                return true;
        }
        while(a.GotoNext());
    }
    return false;
}

bool CompositeIterator::PushIfContainer()
{
	if (Stack->I->IsContainer())
	{
		CryContainer *c = (CryContainer *) Stack->I->Get();
		CryContainer::Iterator *I = c->_CreateIterator();
		LittleStack *s = new LittleStack;
		s->I = I;
		s->Next = Stack;
		Stack = s;
		return true;
	}
	return false;
}

bool CompositeIterator::PopIfContainer()
{
    if (Stack->Next)
    {
        LittleStack *s = Stack;
        Stack = Stack->Next;
        s->I->GetOrigContainer()->DeleteIterator(s->I);
        delete s;
        return true;
    }
    return false;
}

CompositeIterator::CompositeIterator(const CryContainer *oc ) : CryContainer::Iterator(oc)
{
	VisitingContainer = true;
	VisitContainer = true;
	CryContainer::Iterator *I = oc->_CreateIterator();
	LittleStack *s = new LittleStack;
	s->I = I;
	s->Next = 0;
	Stack = s;
}
CompositeIterator::~CompositeIterator()
{
	while(Stack)
	{
		LittleStack *s = Stack;
		Stack = Stack->Next;
		if (s->I)
		{
			s->I->GetOrigContainer()->DeleteIterator(s->I);
			s->I = 0;
		}

		delete(s);
	}
};
bool CompositeIterator::GotoPrev()
{
	if (VisitingContainer)
		return GotoLast();
	if (Stack->I->GotoPrev())
	{
		if (PushIfContainer())
			return GotoLast();
		else
			return true;
	}
	else
	{
		if (PopIfContainer())
			return GotoPrev();
		else
			return false;
	}
}
bool CompositeIterator::GotoLast()             /* TODO : GotoFirst and GotoNext are now working, Need to correct/verify GotoLast and GotoPrev */
{
	if (!VisitingContainer && VisitContainer && Stack->I->IsContainer())
	{
			VisitingContainer = true;
			return true;
	}
	else VisitingContainer = false;

	if (Stack->I->GotoLast())
	{
		if (PushIfContainer())
			return GotoLast();
		else
			return true;
	}
	else
		return GotoPrev();
}

/// We want to go to the next item through containers
bool CompositeIterator::GotoNext()
{
	if (VisitingContainer)
	{
		VisitingContainer = false;
		if (PushIfContainer())
		{
			if (!GotoFirst())
			{
				PopIfContainer();
				return GotoNext();
			}
			return true;
		}
	}
	if  (!(Stack->I->GotoNext()))
	{
		if (PopIfContainer())
		{
        	return GotoNext();
		}
		return false;
	}
	return true;


	if (VisitingContainer)
	{
		VisitingContainer = false;
		VisitContainer = false;
		bool result = GotoFirst();
		VisitContainer = true;
		return result;
	}
	if (Stack->I->GotoNext())
	{
		if (PushIfContainer())
			return GotoFirst();
		else
			return true;
	}
	else
	{
		if (PopIfContainer())
			return GotoNext();
		else
			return false;
	}
}

/// After GotoFirst the iterator will be pointing to a non-container that is the first possible item, (wether contained[subcontained] or not)
bool CompositeIterator::GotoFirst()
{
	return (VisitingContainer = true);//Stack->I->GotoFirst());

	if (VisitContainer && Stack->I->IsContainer())
	{
		VisitingContainer = true;
		return true;
	}

	if (Stack->I->GotoFirst())
	{
		if (PushIfContainer())
			return GotoFirst();
		else
			return true;
	}
	else
		return GotoNext();
}

EmptyObject *CompositeIterator::GetAtIterator() const
{
	const CryContainer *oc = Stack->I->GetOrigContainer();
	if (VisitingContainer) return (EmptyObject *)oc;
	return oc->GetAtIterator(Stack->I);
}
/*void CompositeIterator::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size) const
{
	const CryContainer *oc = Stack->I->GetOrigContainer();
	if (VisitingContainer) oc = Item;
	oc->SetAtIterator(Stack->I,Item,IsCryObject,IsOwned,Size);
} */

const CryContainer::Iterator *CompositeIterator::GetIterator() const
{
	return Stack->I;
}
bool CompositeIterator::IsCryObject() const
{
	return Stack->I->IsCryObject();
}
bool CompositeIterator::IsEmpty() const
{
	return Stack->I->IsEmpty();
}
bool CompositeIterator::IsOwned() const
{
	return Stack->I->IsOwned();
}
EmptyObject *CompositeIterator::Get()
{
	if (VisitingContainer) return (EmptyObject *) Stack->I->GetOrigContainer();

    return Stack->I->Get();
}
size_t CompositeIterator::GetItemSize()
{
    return Stack->I->GetItemSize();
}
//bool LoadAsText(CryString *FromStream) { return OrigContainer->LoadAsText(this,FromStream); }
bool CompositeIterator::SaveAsText(CryString &ToStream)
{
    return Stack->I->SaveAsText(ToStream);
}
const CryContainer *CompositeIterator::GetOrigContainer() const
{
    return Stack->I->GetOrigContainer();
}
/// return the type of structure used to control this list (ie listnode eleptr, etc)
void CompositeIterator::GetEleType(CryString &Result) const
{
    Stack->I->GetEleType(Result);
}

int CompositeIterator::GetCurrentLevel() const
{
    LittleStack *p = Stack;
    int Result = 0;
    while(p->Next)
    {
        Result++;
        p = p->Next;
    }
    return Result;
}

const CryContainer *CompositeIterator::GetLevelContainer(int i) const
{
    LittleStack *p = Stack;
    int j = i;
	i = i - GetCurrentLevel();
	if (i<0)
	{
            CryString s;
            s.printf("Level %d is out of range",j);
            throw CryException(s.AsPChar());
	}
    while(i)
    {
        if (p->Next)
            p = p->Next;
	i--;
    }
    return p->I->GetOrigContainer();
}

#ifdef VALIDATING
void TestCompositeIterator::AddMenuItems(TestCompositeIterator::Menu *l,char *text,int n)
{
    for(int i=0;i<n;i++)
    {
        CryString *s = new CryString();
        s->printf("%s %d of %d",text,i+1,n);
        l->AddOwned(s);
    }
}
TestCompositeIterator::Menu *TestCompositeIterator::AddSubMenu(TestCompositeIterator::Menu *Parent,char *text,int n)
{
    Menu *l = new Menu(text);
    AddMenuItems(l,text,n);
    Parent->AddOwned(l);
    return l;
}

TestCompositeIterator::TestCompositeIterator()
{}
void TestCompositeIterator::DoTest()
{
    TestCompositeIterator::Menu l("File");
    AddSubMenu(&l,"Nothing",0);
    AddMenuItems(&l,"Main",3);
    AddSubMenu(AddSubMenu(&l,"Sub Main",4),"SubSub Main",3);
    AddMenuItems(&l,"More Main",2);
    int Indent = 0;
    CompositeIterator ci(&l);
    printf("\nforward\n");
    if (ci.GotoFirst())
    {
        do
        {
            if (!ci.IsEmpty())
            {
                if (ci.IsCryObject())
                {
                    CryObject *o  = (CryObject *) ci.Get();
                    if (o->IsA(TCryString))
                    {
			int i = ci.GetCurrentLevel();
			Menu *m = (TestCompositeIterator::Menu *) ci.GetLevelContainer(i);
			while(i--)
			{
				printf("____");
			}
			printf("%s_",m->Name.AsPChar());
                        const char *c = ((CryString *)o)->AsPChar();
                        printf("%s\n",c);
                    }
                    else
                    {
                        throw CryException("Shouldn't get here!");
                    }
                }
                else
                {// should never get here in this example
                    EmptyObject *e = ci.Get();
                    throw CryException("Shouldn't get here!");
                }
            }
        }
        while(ci.GotoNext());
    }
    printf("\nReverse\n");
    if (ci.GotoLast())
    {
        do
        {
            if (ci.IsCryObject())
            {
                CryObject *o  = (CryObject *) ci.Get();
                if (o->IsA(TCryString))
                {
                    const char *c = ((CryString *)o)->AsPChar();
                    printf("%s\n",c);
                }
                else
                    throw CryException("Shouldn't get here!");
            }
        }
        while(ci.GotoPrev());
    }
}


#endif
#ifdef VALIDATING
int TestCommand::TestValue;
#endif
