# Introduction #

Crylib is an object oriented [library](http://www.crystalsoftware.org/docs/html/index.html) based on the following ideas:

  * that all classes eventually have as their base class Object.
  * all classes have [Properties](Properties.md) which have enough information to recreate the class.  (for example, think of button which has the property of a caption and colour, and possibly other things).
  * all [Properties](Properties.md) can be serialized.

It contains many features also seen in stl, like containers and iterators, strings, streams and so on.

# Details #

Object doesn't normally contain any data. (If DEBUG is defined an exception is made in that an integer named ObjectID is added).  Object instead defines all the interfaces of the classes that inherit from it.

Object itself inherits from EmptyObject which is simply
```
struct EmptyObject {};
```
No data, no functions just a name to hang your code on.

Each class also has a #define of it's class name. The name is the letter C plus the name of the class, so for instance Object is CObject, String is CString.
```
#define CObject "Object"
#define CString "String"
```
These defines are useful in the classes in order for a class to know about itself and it's parent classes.

Some of the functions which Object defines are

```
   const char* ClassName() const;
   virtual const char *ChildClassName() const;
```

This function a pointer to a string stating the current class Name or the child class name.
> For example

```
void foo(Object *o)
{
  cout << "ClassName is " << o->ClassName();            // Object
  cout << "ChildClassName is " << o->ChildClassName();   // String
}

void bar()
{
String A;
   foo(&A);
}

```


Often a class is passed using the least common denomintator (ie Object) so these functions are useful to find out what type of class you are dealing with. If you know the class is a String, cast the Object to a String and start using the String functions.

```
    virtual bool IsA(const char *ClassName) const;    // can the object map to a ClassName
```
IsA is another useful function for finding out if a class inherits from another.

```
String a;
   a.IsA(CObject); // returns true
   a.IsA(CXML);  // returns false
```

```
     virtual bool CanDup() const;
```
Which asks can this function duplicate itself? This is useful for sub-classes which may not be able to duplicate. eg a file stream class.

```
    virtual void CopyTo(Object &Dest) const;
```
Which copies the data of the current object (or class derived from it) to the Object Dest.  This obviously must make sense for the object, but for the most part it works as you would expect. You couldn't for example, copy a string to a neural net and expect the classes to know what to do with it, but you could copy a neural net to an XML class, and copy the XML class to the string or file, (and do the reverse as well).
```
    const char *GetProperty(const char *PropertyName,String &Result) const;
```
GetProperty will return a
```
const char * 
```
and a Result showing the property's value, usually the returned char pointer will point to the first character of the Result, however in the case of a property that is an array, the returned char pointer will point to a `"*"`, and the result will return a "[.md](.md)"
String for example has:
  * property "Value" which has the value of whatever the string contains.
  * property "Terminator" which has the value of the terminating character (usually '\0')
  * When DEBUG is defined property "ObjectID" which has the number representing this object.


```
    virtual const cbyte* GetRaw() const;
```
This is the backdoor to get at data. For string it returns the characters in the string. Sometimes it's just easier to use the backdoor then the front.

```
   virtual int Compare(int CompareType,const Object *Test1,const Object *Test2) const;
```
Will return a value showing a comparison result using CompareType. Used by derived classes
in order to have comparisons which makes sense to the class.  CompareType is used to allow different types of comparisons within a class.
Other compare functions abound
```
	virtual int CompareLogical(int CompareType,const Object *Test) const;

    /// returns bool value of LessThen as determined by CompareLogical
    virtual bool LessThen(int CompareType,const Object *Test) const;

	/// returns bool value of GreaterThen as determined by CompareLogical
	virtual bool GreaterThen(int CompareType,const Object *Test) const;

	/// returns bool value of EqualTo as determined by CompareLogical
	virtual bool EqualTo(int CompareType,const Object *Test) const;
```

```
    virtual bool IsContainer() const;
```
IsContainer is true when the object in question can contain accessable instances of data or objects.IE Streams are not containers, but lists and arrays are.


This is just a small taste of the functionality available with this class system. It is by no means complete, but is already useful.