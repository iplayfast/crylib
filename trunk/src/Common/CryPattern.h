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

#ifndef Patterns
#define Patterns

#ifdef __WIN32__
//windows
    #include <dos.h>  // for sleep
#endif
#include <time.h>


#include "CryArray.h"
#include "ClassList.h"
#include "ClassProperty.h"
#include "ClassException.h"
namespace Crystal
{
//#define RangeChecking
/*#define TCryFactory "CryFactory"
//#define TFactory "Factory"
#define TFactoryHolder "FactoryHolder"
#define TStrategy "Strategy"
#define TStrategyHolder "StrategyHolderSender"
#define TObserver "Observer"
#define TObservable "Observable"
#define TDecorator "Decorator"
#define TSingleton "Singleton"
#define TCommandHolder "CommandHolderSender"
#define TCompositeIterator "CompositeIterator"
#define TState "State"
*/
/*! The strategy pattern defines a family of algorithms, encapsulates each one,
and makes them interchangable. Strategy lets the algorithm vary independentyly
from clients that use it. (From Head First Design Patterns O'reilly 2004)
 
To implement this in C++, we will have two main classes. The StrategyHolder
and the Strategy. As we don't know the number of Strategies the StrategyHolder
will need, that number will be passed in on the creation. StrategyHolder will
access the strategies through an index. StrategyHolder never owns the strategy
so care must be taken to delete any created stratgies once they are finished
with.

StrategyHolderSender is a variation of StrategyHolder in that all it's
strategy's Send pointers to a CryObject (originally from the calling function).
It was kept separate from StrategyHolder so that there was no possiblity of mixing
Strategy which need a sender with strategies without. The same strategy can be used for
StrategyHolder and StrategyHolderSender, but only one type within each holder.

The pattern has been extended so that strategies return a value.
If the value >=0 then the value represents the next strategy to be called.
This way one strategy can invoke another.

*/
#define TStrategy "Strategy"
	class Strategy : public CryObject {
	public:
		StdFunctions(Strategy,CryObject);
		virtual int DoStrategy() const;
		virtual int DoStrategy(CryObject *Sender) const;
#ifdef VALIDATING
	virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif		
	};

#define TStrategyHolder "StrategyHolder"
	class StrategyHolder : public Strategy {
		const Strategy **Array;
		unsigned int MaxCount;
		bool OwnsStrategies;
	public:
		StdFunctions(StrategyHolder,Strategy);
		int GetMaxCount() const;
		StrategyHolder(unsigned int NumStrategies=1);
		~StrategyHolder();
		void SetOwnsStrategies(bool Owns);
		bool GetOwnsStrategies() const;
		void SetStrategy(unsigned int StrategyIndex,const Strategy *s);
		const Strategy *GetStrategy(unsigned int StrategyIndex) const;
		virtual int DoStrategy() const;
		virtual int DoStrategy(CryObject *o) const;
		void DoStrategy(int StrategyIndex) const;
	};

#define TStrategyHolderSender "StrategyHolderSender"
	class StrategyHolderSender : public StrategyHolder {
	public:
		StdFunctions(StrategyHolderSender,StrategyHolder);
		StrategyHolderSender(unsigned int NumStrategies=1);
		virtual int DoStrategy() const;
		virtual int DoStrategy(CryObject *o) const;
		void DoStrategy(int StrategyIndex,CryObject *Sender) const;
	};

#ifdef VALIDATING
	class TestStrategy {
	public:
	char *Result; // array of 100 bytes showing the text of result
#define _FLY	0
#define _MOVE	1
		class Ducks : public CryObject {
			bool Sitting;
		public:
        	Ducks();
			bool GetSitting();
			bool SetSitting(bool V);
			virtual bool IsA(const char *ClassName) const;
		};
		// test case for Strategy pattern
		class DucksFly : public Strategy {
		char *Result;
		public:
			DucksFly(char *_Result) { Result = _Result; }
			int DoStrategy() const;
			int DoStrategy(CryObject *d) const;
		};
		class DucksNoFly : public Strategy {
		char *Result;
		public:
			DucksNoFly(char *_Result) { Result = _Result; }
			int DoStrategy() const;
			int DoStrategy(CryObject *) const;
		};
		class DucksMove : public Strategy {
		char *Result;
		public:
			DucksMove(char *_Result) { Result = _Result; }
			int DoStrategy() const;
			int DoStrategy(CryObject *d) const;
		};
	public:
        TestStrategy(char *_Result,bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
    };
#endif

/*! The Observer pattern defines a one to many dependancy between objects so that when one object changes state, all it's dependents are notified ad updated automatically.(From Head First Design Patterns O'reilly 2004)
*/

#define TObserver "Observer"
	class Observable;
	class Observer: public CryObject {
	public:
		Observer();
		~Observer();
		/// multiple behaviours are possible for one class
		virtual void NotifyObservers(Observable *Observed,int ObserverID);
		virtual void NotifyObservers(Observable *Observed);
		StdFunctions(Observer,CryObject);
		/*virtual bool IsA(const char *ClassName) const    // can the object map to a ClassName
		{
			return (strcmp(ClassName,TObserver)==0) ||
				   CryObject::IsA(ClassName);
		}
		/// returns a pointer to a string stating the current class Name, EG CryObject (not CryString)
		const char* ClassName() const
		{
			return TObserver;
		}

		/// returns a pointer to a string stating the ChildClassName, EG CryString (not CryObject)
		virtual const char *ChildClassName() const
		{
			return TObserver;
		}
		*/
#ifdef VALIDATING
		virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

	};
#define TObservable "Observable"
	class Observable : public CryList {
		bool Changed;
		CryString Name;
	public:
		Observable();
		void SetName(const char *_Name);

		virtual bool IsName(const char *_Name) const { return Name==_Name; }
		~Observable();
		void RegisterObserver(Observer *_Observer,bool Owned = false);
		void UnRegisterObserver(Observer *_Observer);
		void SetChanged();
		void NotifyObservers(int ObserverID);
		void NotifyObservers();
		StdFunctions(Observable,CryList);
		/*/// returns a pointer to a string stating the current class Name, EG CryObject (not CryString)
		const char* ClassName() const
		{
			return TObservable;
		}

		/// returns a pointer to a string stating the ChildClassName, EG CryString (not CryObject)
		virtual const char *ChildClassName() const
		{
			return TObservable;
		}
		*/
	};


#ifdef VALIDATING

// test case Observer Pattern based on Head First Design Patterns example
	class TestObserver {
		class CurrentConditions : public Observer {
		protected:
			float temp;
			float humidity;
			float pressure;
		public:
			virtual void NotifyObservers(Observable *Observed,int status);
			virtual void NotifyObservers(Observable *Observed);
			virtual void Display();
		};

		class Forcast : public CurrentConditions {
		public:
			virtual void Display();
		};

		class Weather : public Observable {
			float temp;
			float humidity;
			float pressure;
		public:
			Weather();
			float getTemp();
			float getHumidity();
			float getPressure();
			void measurementsChanged();
			void setMeasurements(float _temp,float hum,float press);
		};
	public:
		TestObserver(char *_Result,bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
	};
#endif

/*! The Decorator pattern attaches additional responibilities to an object dynamically. Decorators provide a flexible alternative to subclassing for extending functionality.
 
Decorator patterns are normally implemented as a base class and then many virtual classes which all have a common function.
They can be created by initialization code. See example below
*/


#define TDecorator	"Decorator"
    class Decorator : public CryObject {
        Decorator *Next;
    public:
        Decorator()
        {
            Next = 0;
        }
        Decorator(Decorator *n);
        ~Decorator();
        virtual void Decorate();
        StdFunctions(Decorator,CryObject);
        /*virtual bool IsA(const char *ClassName) const    // can the object map to a ClassName
        {
            return (strcmp(ClassName,TDecorator)==0) ||
                   CryObject::IsA(ClassName);
        }
        /// returns a pointer to a string stating the current class Name, EG CryObject (not CryString)
        const char* ClassName() const
        {
            return TDecorator;
        }
    
        /// returns a pointer to a string stating the ChildClassName, EG CryString (not CryObject)
        virtual const char *ChildClassName() const
        {
            return TDecorator;
        }
        */
    };


#ifdef VALIDATING
    class TestDecorator {
        class Animal : public Decorator {
            CryString Name;
        public:
            Animal(char *_Name);
            virtual void Decorate();
        };
        class Sleeping : public Decorator {
        public:
            Sleeping(Decorator *n);
            virtual void Decorate();
        };
        class Running : public Decorator {
        public:
            Running(Decorator *n);
            virtual void Decorate();
        };
        class Panting : public Running {
        public:
            Panting(Decorator *n);
            virtual void Decorate();
        };
        class Eating : public Decorator {
            CryString *What;
        public:
            Eating(char *_What,Decorator *n);
            ~Eating();
            virtual void Decorate();
        };
    public:
        TestDecorator();
    };
#endif

/*! The Factory Method Pattern defines an interface for creating an object, but lets subclasses decide which class to
instantiate. Factory Method lets a class defer instantiation to subclasses.
 
class thingsNeededInClass
{
    virtual thing* CreateThing1();
    virtual thing* CreateThing2();
};
class specializedThingsNeededInClass : public thingsNeededInClass
{
    virtual thing* CreateThing1();
    virtual thing* CreateThing2();
};
class differentSpecializedThingsNeededInClass : public thingsNeededInClass
{
	virtual thing* CreateThing1();
	virtual thing* CreateThing2();
};

class Factory
{
thing *Thing1;
thing *Thing2;
public:
	Factory(thingsNeededInClass *class)
	{
		Thing1 = class->CreateThing1();
		Thing2 = class->CreateThing2();
	}
};

our things will always be CryObjects

*/

#define TCryFactory	"CryFactory"
	class CryFactory : public CryContainer {
		CryFactory **Array;
		int MaxCount;
		class FactoryIterator : public Iterator {
			friend class CryFactory;
			int Index;
		public:
			FactoryIterator(const CryContainer *oc) : Iterator(oc)
			{
				Index = 0;
			}
			virtual CryObject *Dup() const
			{
				FactoryIterator *it = (FactoryIterator *)GetOrigContainer()->_CreateIterator();
				it->Index = Index;
				return it;

			}

		};
	public:
		StdFunctions(CryFactory,CryContainer);
		virtual EmptyObject *GetAtIterator(const Iterator *I) const;
		virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0);
		virtual CryPropertyList* PropertyNames() const;
		virtual int GetPropertyCount() const;
		virtual CryObject *GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const;

		virtual const char * GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
		virtual bool  SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);

		virtual void RemoveAtIterator(Iterator *I)
		{
			return;
		};
		virtual size_t Count() const
		{
			return 0;
		};
		virtual void Clear()
		{}
		;
		virtual EmptyObject *Add(EmptyObject *Item,size_t Size)
		{
			return 0;
		};
		virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size)
		{
			return 0;
		};
		virtual CryObject *Add(CryObject *Item)
		{
			return 0;
		}
		;    // returns Item
		virtual CryObject *AddOwned(CryObject *Item)
		{
			return 0;
		}
		;   // gives ownership to list
		virtual void SetItemOwnerShip(Iterator *I,bool Owned)
		{}
		;
		virtual bool GetItemOwnerShip(const Iterator *I) const
		{
			return false;
		};
		virtual size_t GetItemSize(Iterator *I) const
		{
			return 0;
		};
		virtual bool LoadAsText(Iterator *I,CryString &FromStream)
		{
			return false;
		};
		virtual bool SaveAsText(Iterator *I,CryString &ToStream) const
		{
			return false;
		};
	protected:
		CryFactory *GetFactory(int n) const
		{
			if (n<MaxCount)
				return Array[n];
			else
				throw CryException("Out of range");
		}
		void SetFactory(int n,CryFactory *f) const
		{
			if (n<MaxCount)
				Array[n]=f;
		}
	public:
		// The factory part
		virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent);
		virtual CryList *GetProducts() const;
		//void AddFactory(Factory *f);
		virtual const char *Describe() const
		{
			return TCryFactory;
		}
		virtual bool CanCreate(const CryPropertyParser &PropertyName) const;
		virtual void GetEleType(CryString &Result) const
		{
			Result = TCryFactory;
		}
		// The Factory Holder part
		int GetMaxCount() const
		{
			return MaxCount;
		}
		CryFactory()
		{
			MaxCount = 0;
			Array = new CryFactory*[0];
			//Array[0] = new Factory();// default No Factories;
		};
		void DeleteFactoryDescribed(const char *Description);
		void DeleteHeldFactories();
		~CryFactory();
		/// Newest factory is always added as the first
		void AddFactory(CryFactory *f);
		virtual CryObject *Create(const char *FactoryName,const CryPropertyParser &PropertyName,CryObject *Parent=0);
		virtual CryObject *Create(CryStream &s)
		{
			return CryObject::Create(s);
		}
		virtual const char *Describe(const char *FactoryName) const
		{
			return TCryFactory;
		}
		// Crystal Stuff
		virtual Iterator *_CreateIterator() const
		{
			return new FactoryIterator(this);
		}
		FactoryIterator *CreateIterator() const
		{
			return(FactoryIterator *)_CreateIterator();
		}
		virtual void DeleteIterator(Iterator *I) const
		{
			delete I;
		}
		virtual CryFactory *FindFactory(const char *FactoryName) const;
		virtual CryFactory *GetFactory(const Iterator *I) const
		{
			FactoryIterator *fi = (FactoryIterator *)I;
			if (fi->Index<0 || fi->Index>=MaxCount)
				throw CryException("Range Error");
			return Array[fi->Index];
		};

		virtual bool GotoFirst(Iterator *I) const
		{
			FactoryIterator *fi = (FactoryIterator *)I;
			fi->Index = 0;
			return MaxCount > 0;
		}   // returns true if success
		/// abstract function used by subclasses to go to the Prev element of a container (list or array doesn't matter)
		virtual bool GotoPrev(Iterator *I) const
		{
			FactoryIterator *fi = (FactoryIterator *)I;
			if (fi->Index>0)
				fi->Index--;
			else
				return false;
			return MaxCount > fi->Index;
		}   // returns true if success
		/// abstract function used by subclasses to go to the Next element of a container (list or array doesn't matter)
		virtual bool GotoNext(Iterator *I) const
		{
			FactoryIterator *fi = (FactoryIterator *)I;
			if (fi->Index<MaxCount-1)
				fi->Index++;
			else
				return false;
			return true;
		}   // returns true if success
		/// abstract function used by subclasses to go to the last element of a container (list or array doesn't matter)
		virtual bool GotoLast(Iterator *Iterator) const     // returns true if success
		{
			FactoryIterator *fi = (FactoryIterator *)Iterator;
			fi->Index = MaxCount-1;
			return MaxCount > 0;
		}   // returns true if success
		/// abstract function used by subclasses to get a pointer to the first element of a container (list or array doesn't matter)
		virtual bool IsEmpty(const Iterator *I) const
		{
			return MaxCount < 1;
		}
		virtual bool IsCryObject(const Iterator *I) const
		{
			return MaxCount!=0;
		}
	virtual bool Sortable() const { return true;}
	virtual void Sort(int CompareType=0);

	};

/*! CryOFactory will attempt to create CryObject classes if other classes aren't able to fit the bill*/

#define TCryOFactory "CryOFactory"
	class CryOFactory : public CryFactory {
	public:
		StdFunctions(CryOFactory,CryFactory);

		virtual CryObject *Create(const char *FactoryName,const CryPropertyParser &PropertyName,CryObject *Parent=0);
		virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent);
        virtual CryObject *Create(CryStream &s)
        {
            return CryFactory::Create(s);
        }

    };

    class FactoryHolder;
/*class Factory : public CryObject
{
public:
    virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent);
    virtual CryObject *Create(CryStream &s)
    {
        return CryObject::Create(s);
    }
    virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *o)
    {
        return CryObject::Create(s,o);
    }
    virtual const char *Describe() const
    {
        return TCryObject;
    }
    virtual bool CanCreate(const CryPropertyParser &PropertyName) const
    {
        return CryObject::CanCreate(PropertyName);
    };
    virtual void GetEleType(CryString &Result) const
    {
        Result = TFactory;
    }
 
};
*/
/*#define TFactoryHolder "FactoryHolder"
/// once factories are added, the factoryholder owns them (until deleted)
class FactoryHolder : public Factory
{
    Factory **Array;
    int MaxCount;
public:
    int GetMaxCount() const
    {
        return MaxCount;
    }
    FactoryHolder()
    {
        MaxCount = 0;
        Array = new Factory*[0];
        //Array[0] = new Factory();// default No Factories;
    };
    void DeleteFactoryDescribed(const char *Description);
    void DeleteHeldFactories();
    ~FactoryHolder();
    /// Newest factory is always added as the first
    void AddFactory(Factory *f);
//    virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent=0);
    /// returns a CryObject or 0
    virtual CryObject *Create(const char *FactoryName,const CryPropertyParser &PropertyName,CryObject *Parent=0);
    virtual CryObject *Create(CryStream &s)
    {
        return Factory::Create(s);
    }
    virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *o)
    {
        return Factory::Create(PropertyName,o);
    }
    virtual const char *Describe() const
    {
        return Factory::Describe();
    }
    virtual const char *Describe(const char *FactoryName) const
    {
        return TFactoryHolder;
    }
//    virtual bool CanCreate(const CryPropertyParser &PropertyName) const;
};
*/
#ifdef VALIDATING
    class Ingredients : public CryObject {
    public:
        virtual int Cost()
        {
            return 0;
        }
        virtual const char * Describe() const
        {
            return "Ingredients";
        }
    };
    class FrozenClams : public Ingredients {
    public:
        virtual int Cost()
        {
            return 20;
        }
        virtual const char *Describe() const
        {
            return "frozen Clams";
        }
    };
    class FreshClams: public Ingredients {
    public:
        virtual int Cost()
        {
            return 10;
        }
        virtual const char *Describe() const
        {
            return "fresh Clams";
        }
    };
    class FetaCheese : public Ingredients {
    public:
        virtual int Cost()
        {
            return 40;
        }
        virtual const char *Describe() const
        {
            return "feta Cheese";
        }
    };
    class MotCheese : public Ingredients {
    public:
        virtual int Cost()
        {
            return 35;
        }
        virtual const char *Describe() const
        {
            return "Mot Cheese";
        }
    };

    class Sause : public Ingredients {
    public:
        virtual int Cost()
        {
            return 15;
        }
        virtual const char *Describe() const
        {
            return "Sause";
        }
    };

    class ThinCrust: public Ingredients {
    public:
        virtual int Cost()
        {
            return 25;
        }
        virtual const char *Describe() const
        {
            return "Thin Crust";
        }
    };
    class ThickCrust: public Ingredients {
    public:
        virtual int Cost()
        {
            return 30;
        }
        virtual const char *Describe() const
        {
            return "Thick Crust";
        }
    };

    class ThickCrustFactory: public CryFactory {
    public:
        virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent=0)
        {
            return new ThickCrust;
        }

    };

    class ThinCrustFactory: public CryFactory {
    public:
        virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent=0)
        {
            return new ThinCrust;
        }
    };

    class CrustFactory : public CryFactory {
    public:
        CrustFactory()
        {
            AddFactory(new ThinCrustFactory);
            AddFactory(new ThickCrustFactory);
        }
        ~CrustFactory()
        {}

    }
    ;

    class PizzaIngredientFactory : public CryFactory {
    public:
        virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent)=0;
    };

#define NEWYORK "New York Style\n"
#define CHICAGO "Chicago Style\n"
    class NYIngredientFactory : public PizzaIngredientFactory {
    public:
        virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent)
        {
            if (strcmp(PropertyName,"Cheese")==0)
                return new MotCheese;
            if (strcmp(PropertyName,"Clams")==0)
                return new FreshClams;
            if (strcmp(PropertyName,"Sause")==0)
                return new Sause;
            return 0;
        };
        virtual const char *Describe() const
        {
            return NEWYORK;
        }
        virtual bool CanCreate(const CryPropertyParser &PropertyName) const
        {
            if (strcmp(PropertyName,"Cheese")==0)
                return true;
            if (strcmp(PropertyName,"Clams")==0)
                return true;
            if (strcmp(PropertyName,"Sause")==0)
                return true;
            return false;
        }
    };


    class ChicagoIngredientFactory : public PizzaIngredientFactory {
    public:
        virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent)
        {
            if (strcmp(PropertyName,"Cheese")==0)
                return new FetaCheese;
            if (strcmp(PropertyName,"Clams")==0)
                return new FrozenClams;
            if (strcmp(PropertyName,"Sause")==0)
                return new Sause;
            return 0;
        };
        virtual const char *Describe() const
        {
            return CHICAGO;
        }
        virtual bool CanCreate(const CryPropertyParser &PropertyName) const
        {
            if (strcmp(PropertyName,"Cheese")==0)
                return true;
            if (strcmp(PropertyName,"Clams")==0)
                return true;
            if (strcmp(PropertyName,"Sause")==0)
                return true;
            return false;
        }

    };

    class Pizzaria : public CryFactory {
    public:
        Pizzaria()
        {
            AddFactory(new NYIngredientFactory);
            AddFactory(new ChicagoIngredientFactory);
            AddFactory(new CrustFactory);
        }
        ~Pizzaria()
        {}
    }
    ;

    class TestFactory {
        void Print(const char *City,const char *I1,const char *I2,const char *I3,const char *I4)
        {

            Pizzaria f;
            Ingredients *thing1= (Ingredients *)    f.Create(City,I1);
            Ingredients *thing2= (Ingredients *)    f.Create(City,I2);
            Ingredients *thing3= (Ingredients *)    f.Create(City,I3);
            Ingredients *thing4= (Ingredients *)    f.Create(City,I4);
            printf("Making a %s",City);
            printf(" pizza with:\n");
            int cost = 0;
            if (thing1) {
                printf("%s\t%d\n",
                       thing1->Describe(),
                       thing1->Cost());
                cost += thing1->Cost();
            }
            if (thing2) {
                printf("%s\t%d\n",
                       thing2->Describe(),
                       thing2->Cost());
                cost += thing2->Cost();
            }
            if (thing3) {
                printf("%s\t%d\n",thing3->Describe(),thing3->Cost());
                cost += thing3->Cost();
            }
            if (thing4) {
                printf("%s\t%d\n",thing4->Describe(),thing4->Cost());
                cost += thing4->Cost();
            }
            int i = 0;

            printf("\nOur cost is %d\n",cost);
            delete thing1;
            delete thing2;
            delete thing3;
            delete thing4;
        }
    public:
        TestFactory()
        {
            Print(NEWYORK,"Cheese","Thick Crust","Clams","Sause");
            Print(CHICAGO,"Thin Crust","Cheese","Sause",0);
            Print(NEWYORK,"Cheese","Thin Crust","Clams","Sause");
        }
    };

#endif

/*! This class is a template class in order for different singletons to be defined
  A singleton class is a class where one (and only one) instance of the class is created
 
*/

    template <typename T>
    class Singleton {
        static T* Instance;
        static int Busy;
        static int References;
        Singleton(Singleton &t)
        {}  // can't have copy constructor
    protected:
        Singleton()
        {}  // derived classes can use this
        virtual ~Singleton()
        {}
    public:
        static T* GetInstance()
        {
            Busy++;
            while (Busy>1) {
#ifdef __WIN32__
                _sleep(50);
#else

                struct timespec timeout,remains;
                timeout.tv_sec = 0;
                timeout.tv_nsec =500000000; /* 50 milliseconds */
                nanosleep(&timeout,&remains);
#endif

            }
            // we are sure we are the only thread here now
            if (Instance == 0)
                Instance = new T;
            Busy--;
            if (Instance==0)
                throw CryException("Initial Singleton Instance could not be created");
            References++;
            return Instance;
        }
        void ClearReferences()
        {
            References = 0;
        };
        void DeleteInstance()
        {
            if (References>0)
                References--;
            if (References==0) {
                delete Instance;
                Instance = 0;
            }
        }
    };

// default initialization
    template <typename T>
    T* Singleton<T>::Instance = 0;
    template <typename T>
    int Singleton<T>::Busy= 0;
    template <typename T>
    int Singleton<T>::References= 0;

#ifdef VALIDATING
// derived singleton class
    class MySingleton : public Singleton<MySingleton> {
        friend class Singleton<MySingleton>;
    private:
        MySingleton()
        {}
        ;
        ~MySingleton()
        {}
        ;
    };

    class TestSingleton {
        MySingleton *First;
        MySingleton *Second;
    public:
        TestSingleton()
        {
            First = MySingleton::GetInstance();
            for (int i=0;i<1000;i++) {
                Second = MySingleton::GetInstance();
                if (Second != First)
                    printf("Error--- two singleton's present");
            }
            Second->ClearReferences();
            First = MySingleton::GetInstance();
            Second = MySingleton::GetInstance();
        }
        ~TestSingleton()
        {
            First->DeleteInstance();
            Second->DeleteInstance();
        };
    };

#endif

/*!Command pattern
Encapsulates a request as an object thereby letting you parameterize clients with different requests, 
queue or log requests, and support undoable operations
 
Implemented in the same manner as StrategyHolderSender with CommandHolder for none ObjectPassing, and CommandHolderSender 
for object passing.
*/
#define TCommandHolder	"CommandHolder"
    class CommandHolder : public StrategyHolder {
        StrategyHolder *Undo;
        int *UndoList;
        int UndoLength;
        int UndoIndex;
        int UsedLength;
        void SetStrategy(int StrategyIndex,const Strategy *s)
        {} // not public

    public:
        StdFunctions(CommandHolder,StrategyHolder);

        CommandHolder(unsigned int NumStrategies=1,unsigned int _UndoLength= 1) : StrategyHolder(NumStrategies)
        {
            Undo = new StrategyHolder(NumStrategies);
            if (!Undo)
                throw CryException("Error creating CommandHolder");
            UndoLength = _UndoLength;
            UndoList = new int[UndoLength];
            if (!UndoList) {
                delete Undo;
                Undo = 0;
                throw CryException("Error creating CommandHolder");
            }
            UndoIndex = 0;
            UsedLength = 0;
        };
        ~CommandHolder()
        {
            delete Undo;
            delete []UndoList;
        };
        void SetStrategy(int StrategyIndex,const Strategy *Command,const Strategy *_Undo=0)
        {
            StrategyHolder::SetStrategy(StrategyIndex,Command);
            if (_Undo)
                Undo->SetStrategy(StrategyIndex,_Undo);
            else
                Undo->SetStrategy(StrategyIndex,this);// do nothing

        }
        void DoMacro(int MacroLength,int *MacroList)
        {
            for (int i=0;i<MacroLength;i++)
                DoStrategy(MacroList[i]);
        }

        virtual int DoStrategy() const
        {
            return StrategyHolder::DoStrategy();
        }
        virtual int DoStrategy(CryObject *Sender) const
        {
            return StrategyHolder::DoStrategy(Sender);
        }

        void DoStrategy(int StrategyIndex)
        {
            StrategyHolder::DoStrategy(StrategyIndex);
            if (UndoIndex<UndoLength) {
                UndoList[UndoIndex] = StrategyIndex;
                UndoIndex++;
                UsedLength = UndoIndex;
            } else {
                for (int i=0;i<UndoLength-1;i++)
                    UndoList[i] = UndoList[i+1];
                UndoList[UndoIndex-1] = StrategyIndex;
            }
        }
        void UndoStrategy()
        {
            if (UndoIndex) {
                UndoIndex--;
                Undo->DoStrategy(UndoList[UndoIndex]);
            } else
                throw CryException("At end of Undo list");
        }
        void RedoStrategy()
        {
            if (UndoIndex<UsedLength) {
                StrategyHolder::DoStrategy(UndoList[UndoIndex]);
                UndoIndex++;
            }
        }
    };

#define TCommandHolderSender "CommandHolderSender"
    class CommandHolderSender : public StrategyHolderSender {
        StrategyHolderSender *Undo;
        int *UndoList;
        int UndoLength;
        int UndoIndex;
        int UsedLength;
        void SetStrategy(int StrategyIndex,const Strategy *s)
        {} // not public

    public:
        StdFunctions(CommandHolderSender,StrategyHolderSender);

        CommandHolderSender(unsigned int NumStrategies=1,unsigned int _UndoLength= 1) : StrategyHolderSender(NumStrategies)
        {
            Undo = new StrategyHolderSender(NumStrategies);
            if (!Undo)
                throw CryException("Error creating CommandHolderSender");
            UndoLength = _UndoLength;
            UndoList = new int[UndoLength];
            if (!UndoList) {
                delete Undo;
                Undo = 0;
                throw CryException("Error creating CommandHolderSender");
            }
            UndoIndex = 0;
            UsedLength = 0;
        };
        ~CommandHolderSender()
        {
            delete Undo;
            delete []UndoList;
        };
        void SetStrategy(int StrategyIndex,const Strategy *Command,const Strategy *_Undo=0)
        {
            StrategyHolderSender::SetStrategy(StrategyIndex,Command);
            if (_Undo)
                Undo->SetStrategy(StrategyIndex,_Undo);
            else
                Undo->SetStrategy(StrategyIndex,this);// do nothing

        }
        void DoMacro(int MacroLength,int *MacroList,CryObject *o)
        {
            for (int i=0;i<MacroLength;i++)
                DoStrategy(MacroList[i],o);
        }
        virtual int DoStrategy() const
        {
            return StrategyHolderSender::DoStrategy();
        }
        virtual int DoStrategy(CryObject *Sender) const
        {
            return StrategyHolderSender::DoStrategy(Sender);
        }

        void DoStrategy(int StrategyIndex,CryObject *o)
        {
            StrategyHolderSender::DoStrategy(StrategyIndex,o);
            if (UndoIndex<UndoLength) {
                UndoList[UndoIndex] = StrategyIndex;
                UndoIndex++;
                UsedLength = UndoIndex;
            } else {
                for (int i=0;i<UndoLength-1;i++)
                    UndoList[i] = UndoList[i+1];
                UndoList[UndoIndex-1] = StrategyIndex;
            }
        }
        void UndoStrategy(CryObject *o)
        {
            if (UndoIndex) {
                UndoIndex--;
                Undo->DoStrategy(UndoList[UndoIndex],o);
            } else
                throw CryException("At end of Undo list");
        }
        void RedoStrategy(CryObject *o)
        {
            if (UndoIndex<UsedLength) {
                StrategyHolderSender::DoStrategy(UndoList[UndoIndex],o);
                UndoIndex++;
            } else
                throw CryException("At end of Redo list");
        }
    };

#ifdef VALIDATING

    class TestCommand {
        CommandHolderSender ch;
    public:
        static int TestValue;

        class CryInt : public CryObject {
            int i;
        public:
            CryInt()
            {
                i = 0;
            }
            void Show()
            {
                printf("%d\n");
            }
            void Set(int j)
            {
                i = j;
            }
            int Get()
            {
                return i;
            }
        };

        class Add : public Strategy {
        public:
            virtual int DoStrategy(CryObject *Sender) const
            {
                TestValue += ((CryInt *)Sender)->Get();
                printf(" + %d = %d\n",((CryInt *)Sender)->Get(),TestValue);
                return -1; // done doing strategies
            }
        };

        class Sub : public Strategy {
        public:
            virtual int DoStrategy(CryObject *Sender) const
            {
                TestValue -= ((CryInt *)Sender)->Get();
                printf(" - %d = %d\n",((CryInt *)Sender)->Get(),TestValue);
                return -1; // done doing strategies
            }
        };
        class Mult : public Strategy {
        public:
            virtual int DoStrategy(CryObject *Sender) const
            {
                TestValue *= ((CryInt *)Sender)->Get();
                printf(" * %d = %d\n",((CryInt *)Sender)->Get(),TestValue);
                return -1; // done doing strategies
            }
        };
        class Div : public Strategy {
        public:
            virtual int DoStrategy(CryObject *Sender) const
            {
                TestValue /= ((CryInt *)Sender)->Get();
                printf(" / %d = %d\n",((CryInt *)Sender)->Get(),TestValue);
                return -1; // done doing strategies
            }
        };

    public:
        TestCommand()
        {
            CommandHolderSender ch(4,4);
            Div _Div;
            Add _Add;
            Sub _Sub;
            Mult _Mult;
            try {
                ch.SetStrategy(0,&_Add,&_Sub);
                ch.SetStrategy(1,&_Sub,&_Add);
                ch.SetStrategy(2,&_Mult,&_Div);
                ch.SetStrategy(3,&_Div,&_Mult);

                TestValue = 0;
                CryInt Operand;
                Operand.Set(5);
                printf("%d\n Try to undo with nothing to undo (should cause error)",TestValue);
                try {
                    ch.UndoStrategy(&Operand); // should fail
                } catch (CryException &e) {
                    printf("\n%s\n",(const char *)e);
                }

                ch.DoStrategy(0,&Operand); // add
                printf("Attempting to Undo\n");
                ch.UndoStrategy(&Operand); // add
                printf("Doing some other things\n");
                ch.DoStrategy(1,&Operand);  // sub
                ch.DoStrategy(2,&Operand);  // Mult
                ch.DoStrategy(2,&Operand);  // Mult
                ch.DoStrategy(0,&Operand);  // Add
                ch.DoStrategy(3,&Operand);  // Div
                printf("Undoing now\n");
                ch.UndoStrategy(&Operand);
                ch.UndoStrategy(&Operand);
                ch.UndoStrategy(&Operand);
                ch.UndoStrategy(&Operand);
                printf("redoing now\n");
                ch.RedoStrategy(&Operand);
                ch.RedoStrategy(&Operand);
                ch.RedoStrategy(&Operand);
                ch.RedoStrategy(&Operand);
                try {
                    printf("an extra redo should give an error\n");
                    ch.RedoStrategy(&Operand);
                } catch (CryException &e) {
                    printf("\nexception occured, %s\n",(const char *)e);
                }
                printf("Doing a macro of add,add,mult,mult\n");
                int Macro[4] = { 0,0,2,2};
                ch.DoMacro(4,Macro,&Operand);

            } catch (CryException &e) {
				printf("\nunexpected exception, %s\n",(const char *)e);
            }
        };
    };

#endif
/*!
    The Iterator Pattern provids a way to access the elements of an aggregate object sequentially 
    without exposing its underlying representation. (From Head First Design Patterns O'reilly 2004)
    
    Our Iterator is defined as a class contained inside the CryContainer class
    
    The Composite Pattern allows you to compose objects into Tree Structions to represent 
    part-whole hierarchies. Composite lets clients treat individual objects and compositions of objects 
    uniformly. (From Head First Design Patterns O'reilly 2004)
    
    Our Composite is defined as a CryContainer Class
 
    The CompositeIterator will Iterate through a container which hold containers or Objects.
    An empty container will be iterated through (but of course will not be holding anything so
    will return false for IsObject())
    Visited Containers will be the first item found when going in either direction (Next or Prev)
*/
#define TCompositeIterator "CompositeIterator"
    class CompositeIterator : public CryContainer::Iterator {
        struct LittleStack {
            LittleStack *Next;
            CryContainer::Iterator *I;
        };
        LittleStack *Stack;
        bool VisitContainer;
        bool VisitingContainer;
        bool PushIfContainer();
        bool PopIfContainer();
    public:
        StdFunctionsNoDup(CompositeIterator,CryContainer::Iterator);
        virtual CryObject *Dup() const
        {
			throw CryException("Dup not implemented for CompositeIterator");
        } // creates a duplicate of this object

        CompositeIterator(const CryContainer *oc );
        ~CompositeIterator();
        int GetCurrentLevel() const;
        const CryContainer *GetLevelContainer(int i) const;
        void SetVisitContainer(bool _Visit) { VisitContainer = _Visit;}
        bool GetVisitContainer() const { return VisitContainer;}
        bool GotoPrev();
        bool GotoNext();
        bool GotoFirst();
        bool GotoLast();
        virtual EmptyObject *GetAtIterator() const;
//	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0) const;

        const CryContainer::Iterator *GetIterator() const;
        bool IsCryObject() const;
        bool IsEmpty() const;
        bool IsOwned() const;
        EmptyObject *Get();
        size_t GetItemSize();
        //bool LoadAsText(CryString *FromStream) { return OrigContainer->LoadAsText(this,FromStream); }
        bool SaveAsText(CryString &ToStream);
        const CryContainer *GetOrigContainer() const;
        /// return the type of structure used to control this list (ie listnode eleptr, etc)
        void GetEleType(CryString &Result) const;
    };
#ifdef VALIDATING
    class TestCompositeIterator {
        class Menu : public CryList {
        public:
            CryString Name;
            Menu(const char *_Name) { Name = _Name;}

        };


        void AddMenuItems(Menu *l,char *text,int n);
        Menu* AddSubMenu(Menu *Parent,char *text,int n);

    public:
        TestCompositeIterator();
        void DoTest();
    };
#endif
/*! The state pattern allows an object to alter its behaviour when its internal state changes.
The object will appeer to change it's class.
 
This has been implmented by allowing different strategys to represent the states. The State class can own the Strategies.
*/
#define TState	"State"
    class State : public StrategyHolderSender {
#define NOSTATECHANGE -1
        const Strategy *ActiveState;
    public:
        StdFunctions(State,StrategyHolderSender);

        State(unsigned int TotalNumOfStates=10) : StrategyHolderSender(TotalNumOfStates)
        {
            ActiveState = 0;
            SetOwnsStrategies(false);
        }
        ~State()
        {}

        /// returns true if the state changes
        bool SetActiveState(int StateNum)
        {
            if (StateNum>NOSTATECHANGE) {
                ActiveState = GetStrategy(StateNum);
                return true;
            }
            return false;
        }
        const Strategy *GetActiveState()
        {
            return ActiveState;
        }
    };
#ifdef VALIDATING

    class StateTest {

        class _GumBallState : public State {
        public:
            enum GumBallStates {
                SOLD_OUT=0,NO_QUARTER,HAS_QUARTER,GUMBALL_SOLD
            };
            _GumBallState(int NumStates=0) : State(NumStates)
            {}
            virtual int InsertQuarter() const
            {
                printf("Don't want quarter right now\n");
                return NOSTATECHANGE;
            }
            virtual int EjectQuarter() const
            {
                printf("Can't Eject Quarter right now\n");
                return NOSTATECHANGE;
            }
            virtual int TurnCrank() const
            {
                printf("Can't turn crank right now\n");
                return NOSTATECHANGE;
            }
            virtual int Dispense(int *Inventory) const
            {
                printf("Can't dispense gumball right now\n");
                return NOSTATECHANGE;
            }
            virtual int FillMachine() const
            {
                printf("Can't fill gumball machine right now\n");
                return NOSTATECHANGE;
            }
        };

        class GumBallState : public _GumBallState {
        public:
            enum GumBallStates {
                SOLD_OUT=0,NO_QUARTER,HAS_QUARTER,GUMBALL_SOLD
            };
            GumBallState() : _GumBallState(4)
            {
                SetOwnsStrategies(true);
                SetStrategy((int)SOLD_OUT,new SoldOut());
                SetStrategy((int)NO_QUARTER,new NoQuarter());
                SetStrategy((int)HAS_QUARTER,new HasQuarter());
                SetStrategy((int)GUMBALL_SOLD,new GumBallSold());
                SetActiveState((int)NO_QUARTER);
            };
            virtual int InsertQuarter()
            {
                const _GumBallState *s = (_GumBallState *)GetActiveState();
                SetActiveState(s->InsertQuarter());
                return NOSTATECHANGE;
            }
            virtual int EjectQuarter()
            {
                const _GumBallState *s = (_GumBallState *)GetActiveState();
                SetActiveState(s->EjectQuarter());
                return NOSTATECHANGE;
            }
            virtual int TurnCrank()
            {
                const _GumBallState *s = (_GumBallState *)GetActiveState();
                SetActiveState(s->TurnCrank());
                return NOSTATECHANGE;
            }
            virtual int Dispense(int *Inventory)
            {
                const _GumBallState *s = (_GumBallState *)GetActiveState();
                SetActiveState(s->Dispense(Inventory));
                return NOSTATECHANGE;
            }
            virtual int FillMachine()
            {
                const _GumBallState *s = (_GumBallState *)GetActiveState();
                SetActiveState(s->FillMachine());
                return NOSTATECHANGE;
            }

        };


		class SoldOut : public _GumBallState    // The state of being sold out.
		{
        public:
            int FillMachine() const
            {
                printf("Machine filled\n");
                return(int) NO_QUARTER;
            }
        };
		class NoQuarter : public _GumBallState// the state of having gumballs to sell but nobody to sell to
		{
        public:
            int InsertQuarter() const
            {
                printf("Quarter Inserted\n");
                return(int) HAS_QUARTER;
            }
        };
		class HasQuarter : public _GumBallState// have quarter and is ready to dispense
		{
        public:
            int TurnCrank() const
            {
                printf("Turning Crank\n");
                return(int) GUMBALL_SOLD;
            }
            int EjectQuarter() const
            {
                printf("Ejecting Quarter\n");
                return(int) NO_QUARTER;
            }
        };
        class GumBallSold : public _GumBallState {
        public:
            int Dispense(int *Inventory) const
            {
                printf("Despensing Gumball\n");
                *Inventory = *Inventory -1;
                if (*Inventory)
                    return NO_QUARTER;
                else
                    return SOLD_OUT;
            }
        };
    public:
        StateTest()
        {
            int Inventory=4;
            GumBallState state; // Gumball machine states
            state.EjectQuarter();
            state.InsertQuarter();
            state.TurnCrank();
            state.EjectQuarter();
            state.Dispense(&Inventory);
            state.EjectQuarter();
            state.InsertQuarter();
            state.TurnCrank();
            state.EjectQuarter();
            state.Dispense(&Inventory);
            state.EjectQuarter();
            state.InsertQuarter();
            state.TurnCrank();
            state.EjectQuarter();
            state.Dispense(&Inventory);
            state.EjectQuarter();
            state.InsertQuarter();
            state.TurnCrank();
            state.EjectQuarter();
            state.Dispense(&Inventory);
            state.EjectQuarter();
            state.InsertQuarter();
            state.TurnCrank();
            state.EjectQuarter();
            state.Dispense(&Inventory);
            state.EjectQuarter();
            state.InsertQuarter();
            state.TurnCrank();
            state.EjectQuarter();
            state.Dispense(&Inventory);
            state.EjectQuarter();
            state.InsertQuarter();
            state.TurnCrank();
            state.EjectQuarter();
            state.Dispense(&Inventory);
            state.EjectQuarter();
        }
    };
#endif
};
#endif


