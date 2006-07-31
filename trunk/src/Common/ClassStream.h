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

#include "ClassContainer.h"

namespace Crystal
{
using namespace std;

#ifndef TCryStream
#define TCryStream "CryStream"
/*! CryStream is the base for all streaming within the library
  All functions are const, as even though internal (to the class)data is modified, the
  the data that the stream is dealing with is not modified.
*/
class CryStream : public CryContainer //abstract
{
protected:
    ////////////////
struct _StreamMode : public EmptyObject
    {
        StreamMode Mode;
        EmptyObject *ExtraData;
    };
    ///////////////
class StreamIterator : public Iterator
    {
    public:
        unsigned int Offset;
        StreamIterator(const CryContainer *Container);
        virtual CryObject *Dup() const;
    };
private:
    char Terminator[2];
    _StreamMode _Mode;
    CryStream(CryStream &nono);
public:
    int Tag;	// for other classes to use, set to 0 on contruction
    CryStream();
    virtual ~CryStream();
    StreamMode GetMode() const;
    void SetMode(StreamMode NewMode);
    enum CopyStyle { NORMAL,ZIP,UNZIP };

struct Context : public CryObject::Context
    {
        struct  _BuffInt
        {
            char *Buffer;
            int Size;
        };
        struct _CryFromStream
        {
            CryStream * FromStream;
        };
        struct _CryToStream
        {
            CryStream * ToStream;
        };
        struct _StreamSize
        {
            CryStream *Stream;
            size_t Size;
        };
        struct _char
        {
            char ch;
        };
        struct _size_t
        {
            size_t    Size;
        };
        struct _BuffClassName
        {
            char ClassNameBuffer[TMaxClassNameSize];
        };
        struct _ccb
        {
            const char *FileName;
            const char *OpenMode;
            bool ExceptOnError;
        };
        union   UContext
        {
            CryObject::Context::UContext _CryObject;
            _intint Seek;
            _int    SeekFromStart;
            _int    SeekFromCurrent;
            _int    SeekFromEnd;
            _int    OutRead;
            _BuffInt    InRead;
            _int    OutWrite;
            _BuffInt InWrite;
            _CryFromStream InLoadItem;
            _CryFromStream OutLoadItem;
            CryObject::Context::UContext InLoadItemType;
            _char   InSetTerminator;
            _char   OutGetTerminator;
            _size_t OutReadT;
            _BuffInt    InReadT;
            _size_t OutWriteT;
            _BuffInt    InWriteT;
            _int    OutStreamedClass;
            _BuffClassName InStreamedClass;
            _size_t OutReadStream;
            _StreamSize InReadStream;
            _size_t OutWriteStream;
            _StreamSize InWriteStream;
            _size_t OutWriteStr;
            _cCharp InWriteStr;
            _size_t OutReadStr;
            _BuffInt    InReadStr;
            _size_t OutSize;
            _size_t OutTell;
            _bool   OutEof;
            _bool   OutOpen;
            _ccb    InOpen;
            _bool   InClose;
        };
        struct IO
        {
            UContext In;
            UContext Out;
        };
        union UIO
        {
            struct IO   StreamContext;
            CryObject::Context::IO ObjectContext;
        };
    }
    ;  // Context

    enum EObject {EFirst=CryObject::ELast+1,
                  ESeek,ESeekFromStart,ESeekFromCurrent,ESeekFromEnd,ERead,EWrite,
                  ELoadItem,ELoadItemType,
                  ESetTerminator,EGetTerminator,EReadT,EWriteT,
                  EReadStream,EWriteStream,
                  EWriteNStr,EReadNStr,
                  EWriteStr,EReadStr,
                  ETell,EEof,EOpen,EClose,
                  ELast};
    StdFunctionsAbstract(CryStream,CryContainer);
    //    const char* ClassName() const;
    //virtual const char *ChildClassName() const;
    virtual int Seek(int offset,int whence) const= 0;
    virtual int SeekToStart() const;
    virtual int SeekFromStart(int Offset=0) const= 0;
    virtual int SeekFromCurrent(int Offset) const= 0;
    virtual int SeekFromEnd(int Offset=0) const= 0;
    virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;
    virtual void SetTag(int i) const;
    virtual size_t Read(char *ToBuffer,size_t Size) const = 0;
    virtual size_t Write(const char *FromBuffer,size_t Size)= 0;
    virtual void CopyTo(CryObject &Object) const;
    virtual void CopyToStream(CryStream &Dest,CopyStyle Style = NORMAL) const;
    virtual bool CanDup() const;
    //    virtual CryObject *Dup() const = 0; // creates a duplicate of this object

    virtual void SetTerminator(char Terminator_);
    virtual char GetTerminator() const;
    // read until terminator or Size (inclusive)
    virtual size_t ReadTI(char *ToBuffer,size_t Size) const;
    // write until terminator or size (inclusive)
    virtual size_t WriteTI(const char *FromBuffer,size_t Size);
    // read until terminator or Size (inclusive)
    virtual size_t ReadTI(CryStream *ToBuffer,size_t Size) const;
    // write until terminator or size (inclusive)
    virtual size_t WriteTI(CryStream *FromBuffer,size_t Size);

    // read until terminator or Size (exclusive)
    virtual size_t ReadTE(char *ToBuffer,size_t Size) const ;
    // write until terminator or size (exculusive)
    virtual size_t WriteTE(const char *FromBuffer,size_t Size);
    // read until terminator or Size (exclusive)
    virtual size_t ReadTE(CryStream *ToBuffer,size_t Size) const;
    // write until terminator or size (Exclusive)
    virtual size_t WriteTE(CryStream *FromBuffer,size_t Size);

    virtual char *StreamedClass(char ClassNameBuffer[TMaxClassNameSize]) const; // will LoadFrom the Buffer with the name of the next class in the stream to LoadFrom
    virtual size_t Read(CryStream *ToStream,size_t Size) const= 0;
    virtual size_t Write(const CryStream *FromStream,size_t Size)= 0;
    virtual size_t Read(CryStream *ToStream) const;
    virtual size_t Write(const CryStream *FromStream);
    virtual size_t WriteNStr(const char *Buffer);
    virtual size_t ReadNStr(char *Buffer,size_t MaxLength) const;
    virtual size_t WriteStr(const char *Buffer);
    virtual size_t ReadStr(char *Buffer,size_t MaxLength) const;
    virtual size_t Size() const = 0;
    virtual size_t Tell() const = 0;
    virtual bool Eof() const = 0;
    virtual bool Open(const char *Name,const char *Operation,bool ExceptOnError=true)= 0;
    virtual void Close(bool ExceptOnError=true)= 0;
    virtual bool IsOpen() const = 0;
    virtual void Flush() = 0;
    virtual bool Event(CryObject::EObject EventNumber,CryObject::Context::IO &Context);
    virtual bool Event(EObject EventNumber,Context::UIO &Context);
    virtual int scanf(const char *format,...) const = 0;
    virtual size_t printf(const char *format,...)= 0;
    virtual int fgetc() const;
    // if this class contains the property name, it will attempt to load it
    // if all is well returns true
    virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);
    virtual bool HasProperty(const CryPropertyParser &PropertyName) const;
    virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
    virtual int GetPropertyCount() const;
	virtual CryPropertyList* PropertyNames() const;

#ifdef VALIDATING

    virtual bool Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

    void GetEleType(CryString &Result) const;
    /// Container fucntions
    virtual CryObject *Add(CryObject *Item);    // returns Item
    virtual CryObject *AddOwned(CryObject *Item);   // gives ownership to list
    virtual void SetItemOwnerShip(Iterator *I,bool Owned);
    virtual bool GetItemOwnerShip(const Iterator *I) const;
    virtual size_t GetItemSize(Iterator *I) const;
    virtual bool IsCryObject(const Iterator *I) const;
    virtual bool LoadAsText(Iterator *I,CryString &FromStream);
    virtual bool SaveAsText(Iterator *I,CryString &ToStream) const;
}
;//CryStream
#endif
}
