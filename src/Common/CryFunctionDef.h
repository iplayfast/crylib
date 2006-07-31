#include "ClassString.h"
#include "ClassProperty.h"

namespace Crystal {

using namespace std;
#ifndef TFunctionDef
#define TFunctionDef	"FunctionDef"
#define TListFunctionDef	"ListFunctionDef"

class FunctionDef : public CryString
{
CryString Return;
CryString Param;
public:
StdFunctions(FunctionDef,CryString);
	FunctionDef()
	{	}
	~FunctionDef()
	{	}

// --- Functions for setting and retrieving property values ---
// if a Get function returns a char * that is different
// then the Result, it implies a special situation
// (usually a container class)


// --- Inhertied properties ---
	const char * GetValue(CryString &Result) const
		{ return GetProperty("Value",Result);}
	void SetValue(const char * _Value)
		{ SetProperty("Value", _Value);}


// --- Local properties ---
	const char *GetReturn(CryString &Result) const
		{ return GetProperty("Return",Result);}
	void SetReturn(CryString _Return)
		{ SetProperty("Return", _Return);}
	const char *GetParam(CryString &Result) const
		{ return GetProperty("Param",Result);}
	void SetParam(CryString _Param)
		{ SetProperty("Param", _Param);}

// Inherited Functions


//CryObject

//virtual CryObject *CreateItemType(const char *Name);
//virtual bool Event(EObject EventNumber,Context::IO &Context);
//CryObject *LoadItem(CryStream &FromStream);
//virtual bool IsContainer() const;
//virtual CryProperty *GetCopyOfPropertyAsObject(const char *PropertyName) const;
//virtual bool SetPropertyAsObject(CryProperty *Value);
//virtual void SaveTo(CryStream &ToStream) const;
//virtual void LoadFrom(CryStream &FromStream);
CryObject *FactoryCreate(CryStream &FromStream);
CryObject *FactoryCreate(const CryString &Type,CryObject *Parent=0);
bool IterateThroughAll(CryContainer *Container,EmptyObject *Control);
//virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item);
//virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));


//CryStream

StreamMode GetMode() const;
void SetMode(StreamMode NewMode);
//virtual //virtual bool CanDup() const;
//virtual void SetTerminator(char Terminator_);
//virtual char GetTerminator() const;
//virtual size_t ReadTE(char *ToBuffer,size_t Size) const ;
//virtual size_t WriteTE(const char *FromBuffer,size_t Size);
//virtual size_t ReadTE(CryStream *ToBuffer,size_t Size) const;
//virtual size_t WriteTE(CryStream *FromBuffer,size_t Size);
//virtual char *StreamedClass(char ClassNameBuffer[TMaxClassNameSize]) const;
//virtual size_t WriteStr(const char *Buffer);
//virtual size_t ReadStr(char *Buffer,size_t MaxLength) const;
//virtual bool Event(CryObject::EObject EventNumber,CryObject::Context::IO &Context);
//virtual bool Event(EObject EventNumber,Context::UIO &Context);
//virtual bool SetProperty(const char *PropertyName,const CryString &PropertyValue);


//CryMemStream

size_t Resize(size_t s);
//void CopyToStream(CryStream &Dest) const;
//virtual const cbyte* GetRaw() const;
//virtual const char *AsPChar() const;
//virtual void SetRaw(unsigned int i,cbyte v);
//virtual void SetRaw(unsigned int Start,const cbyte *v,size_t Length);
//virtual size_t GetLength() const;
void Compress();
void Decompress();
bool operator ==(const CryMemStream &s);
bool operator !=(const CryMemStream &s);
bool operator ==(const CryMemStream *s);
bool operator !=(const CryMemStream *s);
bool operator ==(const char *s);
bool operator !=(const char *s);
//virtual int Seek(int offset,int whence) const;
//virtual int SeekToStart() const;
//virtual int SeekFromStart(int Offset=0) const;
//virtual int SeekFromCurrent(int Offset) const;
//virtual int SeekFromEnd(int Offset=0) const;
//virtual CryMemStream &MemDelete(unsigned int start,int amount);
//virtual void CopyTo(CryObject &Dest) const;
//virtual size_t ReadTI(char *ToBuffer,size_t Size) const;
//virtual size_t WriteTI(const char *FromBuffer,size_t Size);
//virtual size_t ReadTI(CryStream *ToBuffer,size_t Size) const;
//virtual size_t WriteTI(CryStream *FromBuffer,size_t Size);
//virtual size_t Read(char *ToBuffer,size_t Size) const;
//virtual size_t Write(const char *FromBuffer,size_t Size);
//virtual size_t Tell() const;
//virtual bool Eof() const;
int Compare(CryMemStream *MemStream);
//virtual bool Open(const char *Name,const char *Operation,bool ExceptOnError=true);
//virtual void Flush();
int vsscanf(const char *format, va_list ap) const;
int vsprintf(const char *format, va_list ap);


//CryString

//virtual void Clear();
//virtual //virtual void Clear(int amount);
//virtual size_t Length() const;
//virtual void SetLength(int i);
//virtual CryList *PropertyNames() const;
//virtual int GetPropertyCount() const;
CryString &Delete(unsigned int start,int amount);
//virtual const char *ChildClassName() const;
//virtual operator const char *() const;
CryString operator &();
char operator [](unsigned int Offset) const;
//virtual CryObject *Dup() const;
//virtual size_t Read(CryStream *ToStream,size_t Size) const;
//virtual size_t Write(const CryStream *FromStream,size_t Size);
//virtual size_t Read(CryStream *ToStream) const;
//virtual size_t Write(const CryStream *FromStream);
//virtual size_t Read(char *ToBuffer,size_t t) const;
//virtual size_t Write(const char *FromBuffer,size_t t);
//virtual size_t WriteNStr(const char *Buffer);
//virtual size_t ReadNStr(char *Buffer,size_t MaxLength) const;
const char *strstr(CryString &s) const;
const char *strstr(const char *s) const;
int strncmp(CryString &s,size_t n) const;
int strncmp(const char *s,size_t n) const;
int strcmp(CryString &s) const;
int strcmp(const char *s) const;
const char *strcat(CryString &s);
const char *strcat(const char *s);
const char *FormatHex(cbyte *data,size_t length);
EmptyObject *GetAddress();
bool operator ==(const char *s) const;
bool operator !=(const char *s) const;
CryString & operator +=(const char *s);
//virtual bool IsOpen() const;
//virtual int scanf(const char *format,...) const;
//virtual size_t printf(const char *format,...);
//virtual CryString &TrimLeft();
//virtual CryString &TrimRight();
//virtual CryString &Trim();
//virtual CryString &Replace(const char *_Search,const char *_Replace);
//virtual CryString &ReplaceAppend(const char *_Search,const char *_Replace,const char *_Append=0);
int SeekTextFromStart(const char *SearchString) const;
int SeekTextFromCurrent(const char *SearchString) const;
//virtual void Close(bool ExceptOnError=true);
//virtual bool Convert(int *i) const;
//virtual bool Convert(char *b,int len) const;
//virtual bool IsA(const char *ClassName) const;
CryString & operator =(const char *text);
CryString & operator = (const CryString *v);
CryString & operator = (const CryString &v);
//virtual size_t Size() const;
//virtual bool HasProperty(const char *PropertyName) const;
//virtual const char *GetProperty(const char *PropertyName,CryString &Result) const;
//virtual bool Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));


//FunctionDef
virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;
void SetDefaultValues();
}; // class FunctionDef

//////////////////////

class ListFunctionDef : public CryList
{
CryContainer::Iterator *LocIterator;
public:
StdFunctions(ListFunctionDef,CryList);
	ListFunctionDef()
	{
		LocIterator = _CreateIterator();
	}
	~ListFunctionDef()
	{
		DeleteIterator(LocIterator);
	}

// --- Functions for setting and retrieving property values ---
// if a Get function returns a char * that is different
// then the Result, it implies a special situation
// (usually a container class)


// --- Inhertied properties ---
	const char * GetItems(CryString &Result) const
		{ return GetProperty("Items",Result);}
	void SetItems(const char * _Items)
		{ SetProperty("Items", _Items);}


// --- Local properties ---
/*! Get a string delimited by \n containing all functions this class can inherite */
CryFunctionDefList *GetFunctions(const char *Type=0) const;
/*! This function set's default values that were present when class was built.
If default values aren't needed it can safely be deleted */
void SetDefaultValues();
/*!  IControl is only set up as an example of an easy way to have the Iterated function called for multiple reasons
	  Use the "State" variable to select which reason it's being called.*/
struct IControl
{
//int State;
};

/*! The virtual function called from CryContainer for each iteration of the container */
virtual bool IteratedFunction(EmptyObject *Control,EmptyObject *Item)
{
	return IteratedFunction((IControl *)Control,(FunctionDef *)Item);
}
/*! Our own iterated function using a contol set up internally */
virtual bool IteratedFunction(ListFunctionDef::IControl *Control,FunctionDef *Item);
/*! This function will add an object of type FunctionDef to
the container. The Owned part means that the container is responsible for deleting it*/
virtual FunctionDef* ListFunctionDef::AddOwned(FunctionDef * Item);
/*! This function will add an object of type FunctionDef to
the container. It is not "Owned" so the container is NOT responsible for deleting it*/
virtual FunctionDef* ListFunctionDef::Add(FunctionDef * Item);

// Iterator functions available because this is a container class

/*! If thing at current location is in CryObject */
	bool I_IsCryObject() { return LocIterator->IsCryObject(); }

/*! If thing at current location is owned by the Container (ie the container delete's it */
	bool I_IsOwned() { return LocIterator->IsOwned(); }

/*! This container is specialized to handle FunctionDef's. Get the current FunctionDef the Iterator is pointing to */
	FunctionDef *I_Get() { return (FunctionDef *)LocIterator->Get(); }
/*! Position the Iterator at the first item in the container (return true if items there)*/
	bool I_GotoFirst() { return LocIterator->GotoFirst(); }
/*! Position the Iterator at the previous item in the container (could be slow for list type containers*/
	bool I_GotoPrev() { return LocIterator->GotoPrev(); }
/*! Position the Iterator at the next item in the container */
	bool I_GotoNext() { return LocIterator->GotoNext(); }
/*! Position the Iterator at the last item in the container */
	bool I_GotoLast() { return LocIterator->GotoLast(); }
	size_t I_GetItemSize() { return LocIterator->GetItemSize(); }
	bool I_SaveAsText(CryString &ToBody) { return  LocIterator->SaveAsText(ToBody); }
	const CryContainer *I_GetOrigContainer() const  { return LocIterator->GetOrigContainer(); }

/*!*************************************************
// Inherited Functions


//CryObject

virtual bool CanDup() const;
virtual virtual CryObject *CreateItemType(const char *Name);
virtual const char *AsPChar() const;
virtual bool Event(EObject EventNumber,Context::IO &Context);
//CryObject *LoadItem(CryStream &FromStream);

virtual CryProperty *GetCopyOfPropertyAsObject(const char *PropertyName) const;
virtual bool SetPropertyAsObject(CryProperty *Value);
virtual void SaveTo(CryStream &ToStream) const;
virtual void LoadFrom(CryStream &FromStream);
CryObject *FactoryCreate(CryStream &FromStream);
CryObject *FactoryCreate(const CryString &Type,CryObject *Parent=0);
bool IterateThroughAll(CryContainer *Container,EmptyObject *Control);


//CryNamedObject

const char *GetName() const;
void SetName(const char *N);
void SetName(const char *N,int n);
int GetCopyCount() const;
int IncCopyCount();
int DecCopyCount();
virtual CryString *GetFunctions() const;void Destroy(CryNamedObject *Object);


//CryContainer

const CryContainer *GetOwner(Iterator *I) const;
virtual void DeleteIterator(Iterator *I) const;
virtual bool GotoFirst(Iterator *I) const;
virtual bool GotoPrev(Iterator *I) const;
virtual bool GotoNext(Iterator *I) const;
virtual bool GotoLast(Iterator *Iterator) const;
virtual EmptyObject *GetAtIterator(Iterator *I) const;
virtual CryString *GetFunctions();
virtual virtual virtual virtual virtual void SetItemOwnerShip(Iterator *I,bool Owned);
virtual virtual virtual virtual virtual bool IterateThroughAll(EmptyObject *Control);

virtual bool SetProperty(const char *PropertyName,const CryString &PropertyValue);
virtual bool Test(bool Verbose,CryObject &Object,bool  (CallBack)(bool Verbose,const char *Result,bool fail));


//CryList

virtual void GetEleType(CryString &Result) const;
void SaveItemsTo(CryStream &ToStream) const;
virtual virtual ~CryList();
virtual const cbyte* GetRaw() const;
virtual Iterator *_CreateIterator() const;
virtual void DeleteIterator(Iterator *LI) const;
virtual bool GotoFirst(Iterator *LI) const;
virtual bool GotoPrev(Iterator *LI) const;
virtual bool GotoNext(Iterator *LI) const;
virtual EmptyObject* GetAtIterator(Iterator *I) const;
virtual bool GotoLast(Iterator *LI) const;
virtual size_t Count() const;
virtual bool HasItems() const;
virtual void Clear();
const char* ClassName() const;
virtual const char *ChildClassName() const;
virtual bool IsA(const char *ClassName) const;
ListNode *FindNode(EmptyObject *Needle) const;
ListNode *FindNodeValue(CryMemStream &Needle) const;
const ListNode *FirstNode() const;
const ListNode *NextNode(const ListNode *n) const;
EmptyObject *Add(EmptyObject *Item,size_t Size);
EmptyObject *AddOwned(EmptyObject *Item,size_t Size);
CryObject *Add(CryObject *Item);
CryObject *AddOwned(CryObject *Item);
void SetItemOwnerShip(EmptyObject  *Item,bool Owned);
bool GetItemOwnerShip(EmptyObject *Item) const;
bool IsCryObject(Iterator *I) const;
size_t GetItemSize(Iterator *I) const;
bool LoadAsText(Iterator *I,CryString &FromStream);
bool SaveAsText(Iterator *I,CryString &ToStream) const;
void SetItemOwnerShip(Iterator  *I,bool Owned);
bool GetItemOwnerShip(Iterator *I) const;
bool InList(CryObject *Needle) const;
EmptyObject *GetItem(int i) const;
virtual bool IsContainer() const;
void Remove(CryObject *Item);
CryObject *Dup() const;
void Copy(CryList *List);
virtual const char *GetProperty(const char *PropertyName,CryString &Result) const;
virtual bool HasProperty(const char *PropertyName) const;
virtual int GetPropertyCount() const;
virtual CryList *PropertyNames() const;
virtual void CopyTo(CryObject &Dest) const;
virtual size_t Size() const;

***************************************************/
}; // class ListFunctionDef


}; //namespace Crystal

#endif	// TFunctionDef
