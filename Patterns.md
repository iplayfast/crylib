# Design Patterns included in the CryLib

# Introduction #

The book **Head First Design Patterns** by Freeman & Freeman inspired me to add a set of classes to my library. These classes cover all the design patterns in their book in C++ form. (The book was Java). It also includes Unit test code for all the patterns to not only test the code, but to give an example of how the pattern could be used.


# Details #

**Strategy Pattern**
The strategy pattern defines a family of algorithms, encapsulates each one,
and makes them interchangable. Strategy lets the algorithm vary independentyly
from clients that use it. (From Head First Design Patterns O'reilly 2004)

```
	class Strategy : public Object 
and
	class StrategyHolder : public Strategy 
```



To implement this in C++, we will have two main classes. The StrategyHolder
and the Strategy. As we don't know the number of Strategies the StrategyHolder
will need, that number will be passed in on the creation. StrategyHolder will
access the strategies through an index. StrategyHolder never owns the strategy
so care must be taken to delete any created stratgies once they are finished
with.

```
	class StrategyHolderSender : public StrategyHolder 
```
StrategyHolderSender is a variation of StrategyHolder in that all it's
strategy's Send pointers to a Object (originally from the calling function).
It was kept separate from StrategyHolder so that there was no possiblity of mixing
Strategy which need a sender with strategies without. The same strategy can be used for
StrategyHolder and StrategyHolderSender, but only one type within each holder.

The pattern has been extended so that strategies return a value.
If the value >=0 then the value represents the next strategy to be called.
This way one strategy can invoke another.

**Observer Pattern**
The Observer pattern defines a one to many dependancy between objects so that when one object changes state, all it's dependents are notified ad updated automatically.(From Head First Design Patterns O'reilly 2004)

```
	class Observer: public Object 
and
	class Observable : public List 
```


**Decorator Pattern**

The Decorator pattern attaches additional responibilities to an object dynamically. Decorators provide a flexible alternative to subclassing for extending functionality.

Decorator patterns are normally implemented as a base class and then many virtual classes which all have a common function.
They can be created by initialization code. See example in code.
```
	class Decorator : public Object 
```

**Factory Pattern**
The Factory Method Pattern defines an interface for creating an object, but lets subclasses decide which class to instantiate. Factory Method lets a class defer instantiation to subclasses.
```
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
```
our things will always be Objects

```
	class Factory : public Container 
```

Which will create locally defined object

```
	class OFactory : public Factory 
```
Which will call the Object Factory functions if the locally defined objects aren't what is being asked to be created.

**Singleton**
> This class is a template class in order for different singletons to be defined
> > A singleton class is a class where one (and only one) instance of the class is created

```
	template <typename T>
	class Singleton 
```

**Command pattern**
Encapsulates a request as an object thereby letting you parameterize clients with different requests, queue or log requests, and support undoable operations

Implemented in the same manner as StrategyHolderSender with CommandHolder for none ObjectPassing, and CommandHolderSender for object passing.
```
	class CommandHolder : public StrategyHolder 
```

```
        class CommandHolderSender : public StrategyHolderSender 
```


**Iterator Pattern**
The Iterator Pattern provids a way to access the elements of an aggregate object  sequentially without exposing its underlying representation. (From Head First Design Patterns O'reilly 2004)

Our Iterator is defined as a class contained inside the [Container](Container.md) class.

**Composite Iterator Patter**
The Composite Pattern allows you to compose objects into Tree Structions to represent part-whole hierarchies. Composite lets clients treat individual objects and compositions of objects uniformly. (From Head First Design Patterns O'reilly 2004)

Our Composite is defined as a Container Class's Iterator

The CompositeIterator will Iterate through a container which hold containers or Objects.
An empty container will be iterated through (but of course will not be holding anything so
will return false for IsObject()) Visited Containers will be the first item found when going in either direction (Next or Prev)

```
	class CompositeIterator : public Container::Iterator 
```

**State Pattern**
The state pattern allows an object to alter its behaviour when its internal state changes.
The object will appeer to change it's class.

This has been implemented by allowing different strategys to represent the states. The State class can own the Strategies.
```
	class State : public StrategyHolderSender 
```