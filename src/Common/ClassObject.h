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
#ifndef OBJECT
#define OBJECT
#include <stdio.h>
#include <string.h>
#ifdef __BORLANDC__
#include <stdarg.h>
#define FILEDEL	"\\"
#else
#define FILEDEL	"/"
#endif

#define DEBUG

extern "C" bool CallBack(const char *Result,bool fail);


//ToDo
// add a variable of type <baseclass> *BaseClass;
// IsA(const char *name)
// { return strcmp(name,ChildClassName()) || BaseClass->IsA(name); }

/*! The following defines create the following functions (using for example, CryContainer and Object as the class and base)
 
 
const char *ClassName() const { return TCryContainer; }
virtual const char *ChildClassName() const { return TCryContainer; }
virtual bool IsA(const char *Name) const { return ::strcmp(Name,TCryContainer)==0) || Object::IsA(Name); }
 
// for non-abstract
virtual Object *Dup() const { CryContainer *New = new CryContainer(); CopyTo(New); return New; }
bool IsAbstract() const { return false; }
 
// for abstract
virtual Object *Dup() const = 0;
bool IsAbstract() const { return true; }
 
*/


#define StdFunctionsClassName(_ClassName)	const char* ClassName() const { return C##_ClassName; }
#define StdFunctionsChildClassName(_ClassName)	virtual const char* ChildClassName() const { return C##_ClassName; }
#define StdFunctionsIsA(ClassName,BaseClass)	virtual bool IsA(const char *Name) const { return (::strcmp(Name,C##ClassName)==0) || BaseClass::IsA(Name); }
#define StdFunctionsDup(ClassName)	virtual Object *Dup() const { ClassName *New = new ClassName(); CopyTo(*New); return New; }
#define StdFunctionsDupA(ClassName)	virtual Object *Dup() const = 0;
#define StdFunctionsIsAbstractTrue()	bool IsAbstract() const { return true; }
#define StdFunctionsIsAbstractFalse()	bool IsAbstract() const { return false; }

#define StdFunctionsAbstract(ClassName,BaseClass)	StdFunctionsClassName(ClassName) StdFunctionsChildClassName(ClassName)  StdFunctionsIsA(ClassName,BaseClass) StdFunctionsDupA(ClassName) StdFunctionsIsAbstractTrue()
#define StdFunctions(ClassName,BaseClass)	StdFunctionsClassName(ClassName) StdFunctionsChildClassName(ClassName)  StdFunctionsIsA(ClassName,BaseClass) StdFunctionsDup(ClassName) StdFunctionsIsAbstractFalse()

#define StdFunctionsNoDup(ClassName,BaseClass)	StdFunctionsClassName(ClassName) StdFunctionsChildClassName(ClassName)  StdFunctionsIsA(ClassName,BaseClass) StdFunctionsIsAbstractFalse()

extern int gdebug;
namespace Crystal
{
#ifndef MAXINT
#define MAXINT 0X7FFFFFFF
#endif

using namespace std;

//#define VALIDATING

#ifdef VALIDATING
#define EXTRACHECKS
#endif

#define TMaxClassNameSize 100

#define TEmpyObject "EmptyObject"
#define CObject "Object"

#define ExceptionUnknownProperty 100
#define ExceptionSaveToDifferentType 101
#define ExceptionNotImplemented 102

#ifndef cbool
#define cbool   char
#endif
#ifndef cbyte
#define cbyte unsigned char
#endif
class Object;
class Stream;
class String;
class Exception;
class Property;
class FunctionDef;
class Property;
class FunctionDefList;
class Container;
class PropertyParser;
class PropertyList;
class HugeInt;
class XML;

/// this is the base class for any object which will be created or destroyed
struct EmptyObject
	{}
;

template <class T>
class _Tupple
{
T  First;
T  Second;
public:
	_Tupple(T &f,T &s) { First = f; Second = s; }
	_Tupple(_Tupple &t) { First = t.First; Second = t.Second; }
	bool Set(T &f,T &s) { if (f==First) { Second = s; return true; } return false; }
	T GetFirst() { return First; }
	T GetSecond() { return Second; }
	void SetFirst(T &f) { First =f; }
	void SetSecond(T &s) { Second = s; }

};



/// All classes within this library are derived from Object.
/*!	Through the use of virtual functions, Object can save/load streams
	Duplicate itself, copy itself and create any of the subclasses in the library
	It can also build a list of properties that the dirived classes use.
*/
class Object : public EmptyObject    // functions only, no data (abstract)
{
#ifdef DEBUG
public:
int ObjectID;		// exception to the rule only for debugging. ObjectID get's inc'd every time an object is created, so you can use it to track objects in the debugger
char ObjectType[30];	// again only for debugging will have the Text version of the object type
private:
#endif
	Object (Object &nono);	// made private to prevent derived copy constructors, this might not be necessary
public:
	enum BOperation { And,Or,Xor,Not,Nor,Nand};

	/* The context info is used with Events in order to allow interpratation of code. Ie it can be used to create meta data of objects */
	struct Context
	{
		/* The following structures are all possible contexts of the data exchange of internal functions */
		struct _InTo
		{
			Stream *ToStream;
		};
		struct _InLoad
		{
			Stream *FromStream;
		};
		struct _Object
		{
			Object *_Object;
		};
		struct _int
		{
			int Result;
		};
		struct _intint
		{
            int i1;
            int i2;
        };
        struct _bool
        {
            bool Result;
        };
        struct _cCharp
        {
            const char *Text;
        };
		struct  _charp
        {
            char *Text;
        };
        /* UContext is any possible context */
        union UContext
		{
            struct _InTo      InSave;
            struct _InLoad      InLoad;
            struct _Object   InCopyTo;
			struct _int         OutSize;
            struct _Object   OutDup;
            struct _cCharp      OutClassName;
            struct _cCharp      InIsa;
            struct _bool        OutIsa;
        };
		/* IO has an input and an output context */
        struct IO
        {
            UContext In;
            UContext Out;
        };
    };
    /// used to define events, which can be interpreted by the Event function
    enum EObject {EFirst=0,ESaveObject,ELoadObject,ESaveStream,ELoadStream,ECopyTo,EDup,ESize,EClassName,EIsA,EEvent,ELast};
    /// used to define how objects save themselves, ie as text or binary info
    enum StreamMode { SText,SObject};
    /// will interpert an EObject and act upon the Context
    virtual bool Event(EObject EventNumber,Context::IO &Context);

	Object();
    virtual ~Object();
	/// can this function duplicate itself? This is useful for sub-classes which may not be able to duplicate. eg a file stream class.
	virtual inline bool CanDup() const { return true; }
	///copies contents of this to Dest
	virtual void CopyTo(Object &Dest) const;
	/// creates a duplicate of this object (no data so it's easy!)
	virtual Object *Dup() const;
	/// returns the size of an object (in this case 0)
	virtual size_t Size() const;
	/// will attempt to create an object of the type specified in PropertyName
	virtual Object *CreateItemType(const PropertyParser &PropertyName);

	/// returns a pointer to a string stating the current class Name, Eg Object (not CryString which would be a child class)
	const char* ClassName() const;
	/// returns a pointer to a string stating the ChildClassName, EG CryString (not Object which is the base class)
	virtual const char *ChildClassName() const;
	/// attempt to return the class's main data type as a cbyte *
	virtual const cbyte* GetRaw() const;
	/// get the class data in a raw state as a char *
	virtual const char *AsPChar() const;

	/*! returns true if the object can map to the class name.
	for example TCryString is true for Object, CryStream,CryMemStream and CryString*/
	virtual bool IsA(const char *ClassName) const;    // can the object map to a ClassName

	virtual bool IsAbstract() const;
	// not used?  fixme
	//Object *LoadItem(CryStream &FromStream);
	/// returns a list of public functions for a class (including the abstract ones)
	virtual FunctionDefList *GetFunctions(const char *Type=0) const;

	/*! returns a list of the abstract functions for a class
		as this function is virtual it will go to any decendants first, and each of them
		calls it's base GetFunctions After all the functions are retrieved,
		all the virtual fuctions are collected, which have not been overrode by non-virtual
		functions  */
	virtual FunctionDefList *GetAbstractFunctions(const char *Type) const;

	virtual int Compare(int CompareType,const Object *Test1,const Object *Test2) const;
	/*! will return a value showing a comparison result using CompareType. Used by derived classes
		in order to have comparisons which makes sense to the class.  CompareType is used to allow different types of comparisons within a class
	*/
	virtual int CompareLogical(int CompareType,const Object *Test) const;
	/// returns bool value of LessThen as determined by CompareLogical
	virtual bool LessThen(int CompareType,const Object *Test) const;
	/// returns bool value of GreaterThen as determined by CompareLogical
	virtual bool GreaterThen(int CompareType,const Object *Test) const;
	/// returns bool value of EqualTo as determined by CompareLogical
	virtual bool EqualTo(int CompareType,const Object *Test) const;

	virtual XML *SaveAsXML() const;
	/*! IsContainer is true when the object in question can contain
	 accessable instances of data or objects
	 ie streams are not containers, but lists and arrays are
	 */
	virtual bool IsContainer() const;
	/// will return the value of a property, or if the property is an array, a text string representing the array
	virtual Property *GetPropertyAsCryProperty(const PropertyParser &PropertyName) const;
	/// will return a property represented as an object, useful for classes which contain properties that are dynamically allocated, as a property that is dynamic is a Object and therefore callable
	virtual Object *GetCopyOfPropertyAsObject(const PropertyParser &PropertyName) const;
	/// will return a pointer to the property if the property is an Object (or decendent)
	virtual Object *_GetPropertyAsObject(const PropertyParser &PropertyName) const;
	/// will return whether or not the property named in PropertyName is a container
	virtual bool GetIsPropertyContainer(const PropertyParser &PropertyName) const;

	/*! will return a const char * and a Result showing the property's value, usually the const char * will point to the first character of the Result, however in the case of a property that is an array, the const char * will point to a "*", and the result will return a "[]"
	in this case you will need to use GetPropertyAsCryProperty
	*/
	virtual const char *GetProperty(const char *PropertyName,String &Result) const;


	/*! will return a const char * and a Result showing the property's value, usually the const char * will point to the first character of the Result, however in the case of a property that is an array, the const char * will point to a "*", and the result will return a "[]"
	in this case you will need to use GetPropertyAsCryProperty
	*/
	virtual const char *GetProperty(const PropertyParser &PropertyName,String &Result) const;

	/// returns true if the class in question has the property
	virtual bool HasProperty(const PropertyParser &PropertyName) const;
	/*! returns true if the class in question can have the property. Useful for determining if a class can accept dynamic properties */
	virtual bool CanHaveProperty(const PropertyParser &PropertyName) const;
	/// The count of the properties a class has
	virtual int GetPropertyCount() const;

	/*! Make a list of all property names, the function is called from the parent class through each inheritance until it reaches this class, at which point a list is created and filled with any properties on the way back through the inheritance */
	virtual PropertyList* PropertyNames() const;

	/// set the value of a property
	virtual bool SetProperty(const char *pn,const char *PropertyValue);
	virtual bool SetProperty(const PropertyParser &PropertyName,const char *PropertyValue);
	virtual bool SetProperty(const Property *p);
	/// set the value of a property from an Property Value, Value is NOT  given to the object and can be deleted after the call.
	virtual bool SetPropertyAsObject(const Property *Value);
	/*! save (in xml format) to a stream, stream pays attention to it's mode and will compress the data if mode is SObject, if it's SText, it saves as text.*/
	virtual void SaveTo(Stream &ToStream) const; // xml save
	/// load a previously saved (in xml format) stream
	virtual void LoadFrom(const Stream &FromStream); // xml load
	/// create an object (or container of objects) from the stream
	virtual Object *Create(Stream &FromStream);

	/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the Parent must be the appropriete container type or a derived class which can create the object (if the default class can't) */
	static Object *ClassCreate(const PropertyParser &PropertyName,Object *Parent);
	/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the Parent must be the appropriete container type or a derived class which can create the object (if the default class can't) */
	virtual Object *Create(const PropertyParser &PropertyName,Object *Parent)
	{
		return ClassCreate(PropertyName,Parent);
	}

	virtual bool CanCreate(const PropertyParser &PropertyName) const;
	static bool ClassCanCreate(const PropertyParser &PropertyName);

	//virtual size_t printf(const char *format,...)= 0;
	//virtual CryStream * sprintf(CryStream *s,const char *format,...)= 0;

	/*!  Will call "IteratedFunction" for each item in the container class, passing Control
	to each item. Returns false if the iteration was aborted early
	*/
	bool IterateThroughAll(Container *Container,EmptyObject *Control);

	/*! IteratedFunction is called for each item in the container (from IteratedThroughAll)
	 returns false if iteration should stop
	 */
	virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item);

	Object &operator =(const Object &From);
	Object &operator =(const Object *From);
	virtual bool Sortable() const { return false;}
	virtual void Sort(int CompareType=0) { };
// Visiter design pattern implementation
	void Accept(Object *Visitor) { Visitor->Visit(this); }
	void Accept(const Object *Visitor) const { Visitor->Visit(this); }
	void Accept(Object *Visitor,int GoalID) { Visitor->Visit(this,GoalID); }
	void Accept(const Object *Visitor,int GoalID)const  { Visitor->Visit(this,GoalID); }
	void Visit(Object *Sender) { };	// default do nothing
	void Visit(const Object *Sender) const { }; // default do nothing
	void Visit(Object *Sender,int GoalID) { };	// default do nothing
	void Visit(const Object *Sender,int GoalID) const { }; // default do nothing
#ifdef VALIDATING

	virtual bool Test(bool Verbose,Object &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

};  // Object



/*!	OwnedObject is Owned by another object, and knows it.
	Container classes use this object as a base
*/
class OwnedObject : public Object
{
Object *Owner;
	OwnedObject(OwnedObject &nono);     //not allowed
public:
	OwnedObject() { Owner =0; }
	void SetOwner(Object *_Owner) { Owner = _Owner; }
	Object *GetOwner() const { return Owner; }
	virtual XML *SaveAsXML() const { return Object::SaveAsXML(); }

/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the
	Parent must be the appropriete container type or a derived class which can create the object (if the default class can't) */
	virtual Object *Create(const PropertyParser &PropertyName,Object *Parent);
	virtual Object *Create(Stream &e) { return Object::Create(e); }
};

}	//namespace Crystal


#endif  // OBJECT

