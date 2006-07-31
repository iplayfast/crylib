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
#ifndef CRYOBJECT
#define CRYOBJECT
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

/*! The following defines create the following functions (using for example, CryContainer and CryObject as the class and base)
 
 
const char *ClassName() const { return TCryContainer; }
virtual const char *ChildClassName() const { return TCryContainer; }
virtual bool IsA(const char *Name) const { return ::strcmp(Name,TCryContainer)==0) || CryObject::IsA(Name); }
 
// for non-abstract
virtual CryObject *Dup() const { CryContainer *New = new CryContainer(); CopyTo(New); return New; }
bool IsAbstract() const { return false; }
 
// for abstract
virtual CryObject *Dup() const = 0;
bool IsAbstract() const { return true; }
 
*/


#define StdFunctionsClassName(_ClassName)	const char* ClassName() const { return T##_ClassName; }
#define StdFunctionsChildClassName(_ClassName)	virtual const char* ChildClassName() const { return T##_ClassName; }
#define StdFunctionsIsA(ClassName,BaseClass)	virtual bool IsA(const char *Name) const { return (::strcmp(Name,T##ClassName)==0) || BaseClass::IsA(Name); }
#define StdFunctionsDup(ClassName)	virtual CryObject *Dup() const { ClassName *New = new ClassName(); CopyTo(*New); return New; }
#define StdFunctionsDupA(ClassName)	virtual CryObject *Dup() const = 0;
#define StdFunctionsIsAbstractTrue()	bool IsAbstract() const { return true; }
#define StdFunctionsIsAbstractFalse()	bool IsAbstract() const { return false; }

#define StdFunctionsAbstract(ClassName,BaseClass)	StdFunctionsClassName(ClassName) StdFunctionsChildClassName(ClassName)  StdFunctionsIsA(ClassName,BaseClass) StdFunctionsDupA(ClassName) StdFunctionsIsAbstractTrue()
#define StdFunctions(ClassName,BaseClass)	StdFunctionsClassName(ClassName) StdFunctionsChildClassName(ClassName)  StdFunctionsIsA(ClassName,BaseClass) StdFunctionsDup(ClassName) StdFunctionsIsAbstractFalse()

#define StdFunctionsNoDup(ClassName,BaseClass)	StdFunctionsClassName(ClassName) StdFunctionsChildClassName(ClassName)  StdFunctionsIsA(ClassName,BaseClass) StdFunctionsIsAbstractFalse()

extern int gdebug;

namespace Crystal
{
using namespace std;

//#define VALIDATING

#ifdef VALIDATING
#define EXTRACHECKS
#endif

#define TMaxClassNameSize 100

#define TEmpyObject "EmptyObject"
#define TCryObject "CryObject"

#define ExceptionUnknownProperty 100
#define ExceptionSaveToDifferentType 101
#define ExceptionNotImplemented 102

#ifndef cbool
#define cbool   char
#endif
#ifndef cbyte
#define cbyte unsigned char
#endif
class CryObject;
class CryNamedObject;
class CryStream;
class CryString;
class CryException;
class CryStdStream;
class CryProperty;
class CryFunctionDef;
class CryList;
class CryFunctionDefList;
class CryContainer;
class CryPropertyParser;
class CryPropertyList;

/// this is the base class for any object which will be created or destroyed
struct EmptyObject
    {}
;


/// All classes within this library are derived from CryObject.
/*!	Through the use of virtual functions, CryObject can save/load streams
	Duplicate itself, copy itself and create any of the subclasses in the library
	It can also build a list of properties that the dirived classes use.
*/


class CryObject : public EmptyObject    // functions only, no data (abstract)
{
#ifdef DEBUG
public:
int ObjectID;
private:
#endif
	CryObject (CryObject &nono);	// made private to prevent derived copy constructors, this might not be necessary
public:
	enum BOperation { And,Or,Xor,Not,Nor,Nand};

	/* The context info is used with Events in order to allow interpratation of code. Ie it can be used to create meta data of objects */
	struct Context
	{
		/* The following structures are all possible contexts of the data exchange of internal functions */
		struct _InTo
		{
			CryStream *ToStream;
		};
		struct _InLoad
		{
			CryStream *FromStream;
		};
		struct _CryObject
		{
			CryObject *Object;
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
            struct _CryObject   InCopyTo;
            struct _int         OutSize;
            struct _CryObject   OutDup;
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

    CryObject();
    virtual ~CryObject();
    /// can this function duplicate itself? This is useful for sub-classes which may not be able to duplicate. eg a file stream class.
    virtual bool CanDup() const;
	///copies contents of this to Dest
    virtual void CopyTo(CryObject &Dest) const;
    /// creates a duplicate of this object (no data so it's easy!)
    virtual CryObject *Dup() const;
    /// returns the size of an object (in this case 0)
    virtual size_t Size() const;
    /// will attempt to create an object of the type specified in PropertyName
    virtual CryObject *CreateItemType(const CryPropertyParser &PropertyName);

    /// returns a pointer to a string stating the current class Name, Eg CryObject (not CryString which would be a child class)
    const char* ClassName() const;
    /// returns a pointer to a string stating the ChildClassName, EG CryString (not CryObject which is the base class)
    virtual const char *ChildClassName() const;
    /// attempt to return the class's main data type as a cbyte *
    virtual const cbyte* GetRaw() const;
    /// get the class data in a raw state as a char *
    virtual const char *AsPChar() const;

	/*! returns true if the object can map to the class name.
    for example TCryString is true for CryObject, CryStream,CryMemStream and CryString*/
    virtual bool IsA(const char *ClassName) const;    // can the object map to a ClassName

    virtual bool IsAbstract() const;
    // not used?  fixme
    //CryObject *LoadItem(CryStream &FromStream);
    /// returns a list of public functions for a class (including the abstract ones)
    virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;

    /*! returns a list of the abstract functions for a class
    	as this function is virtual it will go to any decendants first, and each of them
    	calls it's base GetFunctions After all the functions are retrieved, 
    	all the virtual fuctions are collected, which have not been overrode by non-virtual
    	functions  */
    virtual CryFunctionDefList *GetAbstractFunctions(const char *Type) const;

    /*! will return a value showing a comparison result using CompareType. Used by derived classes
    	in order to have comparisons which makes sense to the class.  CompareType is used to allow different types of comparisons within a class
    */
    virtual int CompareLogical(int CompareType,const CryObject *Test) const;
    /// returns bool value of LessThen as determined by CompareLogical
    virtual bool LessThen(int CompareType,CryObject *Test) const;
    /// returns bool value of GreaterThen as determined by CompareLogical
    virtual bool GreaterThen(int CompareType,CryObject *Test) const;
    /// returns bool value of EqualTo as determined by CompareLogical
    virtual bool EqualTo(int CompareType,CryObject *Test) const;

	/*! IsContainer is true when the object in question can contain
     accessable instances of data or objects
     ie streams are not containers, but lists and arrays are
     */
    virtual bool IsContainer() const;
	/// will return the value of a property, or if the property is an array, a text string representing the array
	virtual CryProperty *GetPropertyAsCryProperty(const CryPropertyParser &PropertyName) const;
	/// will return a property represented as an object, useful for classes which contain properties that are dynamically allocated, as a property that is dynamic is a CryObject and therefore callable
	virtual CryObject *GetCopyOfPropertyAsObject(const CryPropertyParser &PropertyName) const;
	/// will return a pointer to the property if the property is an CryObject (or decendent)
	virtual CryObject *_GetPropertyAsObject(const CryPropertyParser &PropertyName) const;
	/// will return whether or not the property named in PropertyName is a container
    virtual bool GetIsPropertyContainer(const CryPropertyParser &PropertyName) const;

    /*! will return a const char * and a Result showing the property's value, usually the const char * will point to the first character of the Result, however in the case of a property that is an array, the const char * will point to a "*", and the result will return a "[]"
    in this case you will need to use GetPropertyAsCryProperty
    */
    const char *GetProperty(const char *PropertyName,CryString &Result) const;

    /*! will return a const char * and a Result showing the property's value, usually the const char * will point to the first character of the Result, however in the case of a property that is an array, the const char * will point to a "*", and the result will return a "[]"
    in this case you will need to use GetPropertyAsCryProperty
    */
    virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;

    /// returns true if the class in question has the property
    virtual bool HasProperty(const CryPropertyParser &PropertyName) const;
    /*! returns true if the class in question can have the property. Useful for determining if a class can accept dynamic properties */
    virtual bool CanHaveProperty(const CryPropertyParser &PropertyName) const;
    /// The count of the properties a class has
    virtual int GetPropertyCount() const;

    /*! Make a list of all property names, the function is called from the parent class through each inheritance until it reaches this class, at which point a list is created and filled with any properties on the way back through the inheritance */
	virtual CryPropertyList* PropertyNames() const;

    /// set the value of a property
    virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
    virtual bool SetPropertyAsObject(CryProperty *Value);
    /*! save (in xml format) to a stream, stream pays attention to it's mode and will compress the data if mode is SObject, if it's SText, it saves as text.*/
    virtual void SaveTo(CryStream &ToStream) const; // xml save
    /// load a previously saved (in xml format) stream
    virtual void LoadFrom(const CryStream &FromStream); // xml load
	/// create an object (or container of objects) from the stream
	virtual CryObject *Create(CryStream &FromStream);

	/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the Parent must be the appropriete container type or a derived class which can create the object (if the default class can't) */
	virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent=0);
	/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the Parent must be the appropriete container type or a derived class which can create the object (if the default class can't) */
    static CryObject *ClassCreate(const CryPropertyParser &PropertyName,CryObject *Parent=0);

    virtual bool CanCreate(const CryPropertyParser &PropertyName) const;
    static bool ClassCanCreate(const CryPropertyParser &PropertyName);

    //virtual size_t printf(const char *format,...)= 0;
    //virtual CryStream * sprintf(CryStream *s,const char *format,...)= 0;

    /*!  Will call "IteratedFunction" for each item in the container class, passing Control
    to each item. Returns false if the iteration was aborted early
    */
    bool IterateThroughAll(CryContainer *Container,EmptyObject *Control);

    /*! IteratedFunction is called for each item in the container (from IteratedThroughAll)
     returns false if iteration should stop
     */
    virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item);

	CryObject &operator =(const CryObject &From);
	CryObject &operator =(const CryObject *From);

#ifdef VALIDATING

	virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));
#endif
}
;  // CryObject

class CryOwnedObject : public CryObject
{
CryObject *Owner;
	CryOwnedObject(CryOwnedObject &nono);
public:
	CryOwnedObject() { Owner =0; }
	void SetOwner(CryObject *_Owner) { Owner = _Owner; }
	CryObject *GetOwner() const { return Owner; }
/*! will create an object of the Type named in Type. In container classes where the Type is the contained object, the
	Parent must be the appropriete container type or a derived class which can create the object (if the default class can't) */
	virtual CryObject *Create(const CryPropertyParser &PropertyName,CryObject *Parent=0);
	virtual CryObject *Create(CryStream &e) { return CryObject::Create(e); }
};


}	//namespace Crystal


#endif  // CRYOBJECT

