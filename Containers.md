# Introduction #

Containers in CryLib have the base class [Container](http://www.crystalsoftware.org/docs/html/classCrystal_1_1Container.html) which includes Iterator. A container is any class which can contain other classes, (such as a hash, array, or list). The CryLib library is similar to but not the same as the stl containers.

It's features are:
  1. knows the ownership of the contained items (individually).
  1. if the container owns the items it will delete them when it is destroyed.
  1. it can serialize the items it contains when it is serialized. (a list of strings can be serialied).
  1. it can contain other containers.

# Details #

Typical code when using a container is shown in the Observeable class which is a subclass of the List Container class.
```
void Observable::NotifyObservers(int ObserverID)
{
    if (!Changed)
        return;

    Iterator *I = _CreateIterator(); // container function
    if (I->GotoFirst())  // container function
    {
        do
        {
            Observer *b = (Observer *)I->Get();  // container function
            b->NotifyObservers(this,ObserverID);
        }
        while(I->GotoNext()); // container function
    }
    DeleteIterator(I);  // container function
    Changed = false;
}
```

In this case the container contains pointers to Observer classes. Each one is being notified. Note the following.
  1. It doesn't matter what type of container is here, they all have the base class container so all will work the same way, even though they may be different types of containers. This allows you to use any type of container (or derived) class with the same pattern.
  1. The iterator is created and destroyed by the container. This allows different types of containers to traverse them using whatever method works best for the container.(list,array etc).
  1. In general the pattern is
```
   if (I->GotoFirst())  // container isn't empty, and you can go to the first item
   do
   {

     // do stuff with the item pointed to by I (using I->Get())

   } while(I->GotoNext()) // returns true if there is another item in the container
```

This can be reversed by using GotoLast and GotoPrev functions.

This pattern insures that you are always dealing with a valid item in the container.

Another enhancement to the Iterators is the GotoFirstObject(Objectname),GotoNextObject and so on. If you have a list of Strings and integers you could deal with them separately by
```
// deal with all the strings first
   if (I->GotoFirstObject(CString))
   do
   {
   } while(I->GotoNextObject(CString));
// deal with all the integers next
   if (I->GotoFirstData())
   do
   {
   } while(I->GotoNextData());
```

**The main Iterator Functions are:**
  * GotoFirst,GotoFirstObject,GotoFirstData
  * GotoNext,GotoNextObject,GotoNextData
  * GotoLast,GotoLastObject,GotoLastData
  * GotoPrev,GotoPrevObject,GotoPrevData
  * [GotoN](GotoN.md)
  * [Get](Get.md),GetObject,IsObject,IsContainer,IsEmpty,
  * [Dup](Dup.md)
  * GetItemSize,GetItemType

**The main Container Functions are:**
  * All the of the Iterator functions with the additional parameter of the Iterator being passed in.
  * [Add](Add.md),AddOwned
  * [Count](Count.md),[Clear](Clear.md)
  * IterateThroughAll, IteratedFunction (actually derived from Object, the continer class to be used is passed in to these functions).
  * SetAtIterator
  * ...