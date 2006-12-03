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

#include <stdarg.h>
#include "ClassStream.h"
namespace Crystal
{
using namespace std;

#ifndef TCryMemStream
#define TCryMemStream "CryMemStream"
/// A stream that is contained within memory
class CryMemStream : public CryStream
{
class MemStreamIterator : public StreamIterator
    {
    public:
        MemStreamIterator(const CryContainer *container);
    };

    char *Buffer;   // where the actual data is held
    // current position in the stream
    size_t Position;
    /// total length of the buffer
    size_t Length;
    /// length of the data within the buffer
    size_t DataLength;

    static char * gBuffer;
    static int gBuffSize;
    static int gBuffCount;  // number of references to it
    void ReMem(size_t Value);

protected:
    void CheckMem(size_t RequestedSize);
    void SetPosition(size_t p);
    size_t GetPosition() const;
    void LookLikeString() const;    // this is really non const, but it affects no data and is more useful as const
    void Pos2DataLength();
    void Pos2Asciiz();
	char *GetBuffer();
	void Zero();
public:
    StdFunctions(CryMemStream,CryStream);
    CryMemStream();
    virtual ~CryMemStream();
    size_t Resize(size_t s);
    virtual void Clear();
    virtual void Clear(int amount); // 0 == all, 1.. from start, -1..-N from end
    CryMemStream(const CryMemStream *E);
    CryMemStream(const CryMemStream &E);
    //    const char* ClassName() const;
    virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;
    //    virtual const char *ChildClassName() const;
    virtual const cbyte* GetRaw() const;
    /// Return a non-const reference to the ith character
    char& operator[](unsigned int i);
    /// Return a const reference to the ith character
    const char& operator[](unsigned int i) const;
    virtual const char *AsPChar() const;
    virtual void SetRaw(unsigned int i,cbyte v);
    virtual void SetRaw(unsigned int Start,const cbyte *v,size_t Length);
    virtual size_t GetLength() const;
    virtual void SetLength(int i);
    virtual operator const char *() const;
    //    virtual bool IsAbstract() const;
    void Compress();
    void Decompress();
    bool operator ==(const CryMemStream &s);
    bool operator !=(const CryMemStream &s);
    bool operator ==(const CryMemStream *s);
    bool operator !=(const CryMemStream *s);
    bool operator ==(const char *s);
    bool operator !=(const char *s);
    virtual int Seek(int offset,int whence) const;
    virtual int SeekToStart() const;
    virtual int SeekFromStart(int Offset=0) const;
    virtual int SeekFromCurrent(int Offset) const;
    virtual int SeekFromEnd(int Offset=0) const;
    virtual CryMemStream &MemDelete(unsigned int start,int amount);
    virtual size_t Size() const;
    virtual void CopyTo(CryObject &Dest) const;
    //    virtual void CopyTo(CryStream *Dest) const;
    //    virtual CryObject *Dup() const; // creates a duplicate of this object
    // read until terminator or Size (inclusive)
    virtual size_t ReadTI(char *ToBuffer,size_t Size) const;
    // write until terminator or size (inclusive)
    virtual size_t WriteTI(const char *FromBuffer,size_t Size);
    // read until terminator or Size (inclusive)
    virtual size_t ReadTI(CryStream *ToBuffer,size_t Size) const;
    // write until terminator or size (inclusive)
    virtual size_t WriteTI(CryStream *FromBuffer,size_t Size);

    virtual size_t Read(char *ToBuffer,size_t Size) const;
    virtual size_t Write(const char *FromBuffer,size_t Size);
    virtual size_t Read(CryStream *ToStream,size_t Size) const;
    virtual size_t Write(const CryStream *FromStream,size_t Size);
    virtual size_t Read(CryStream *ToStream) const;
    virtual size_t Write(const CryStream *FromStream);
    virtual size_t Tell() const;
    virtual bool Eof() const;
    virtual int CompareLogical(int CompareType,const CryObject *Test) const;
	virtual int Compare(const CryMemStream *MemStream) const;

    virtual bool IsOpen()  const;
    virtual bool Open(const char *Name,const char *Operation,bool ExceptOnError=true);
    virtual void Close(bool ExceptOnError=true);
    virtual void Flush();
    int vsscanf(const char *format, va_list ap) const;
    int vsprintf(const char *format, va_list ap);

    virtual int scanf(const char *format,...) const;
    virtual size_t printf(const char *format,...);

    // Container functions
    virtual Iterator *_CreateIterator() const;
    virtual void DeleteIterator(Iterator *I) const;
    virtual bool GotoFirst(Iterator *I) const;
    virtual bool GotoPrev(Iterator *I) const; // returns true if success
    virtual bool GotoNext(Iterator *I) const;    // returns true if success
    virtual bool GotoLast(Iterator *Iterator) const;    // returns true if success

    virtual bool IsEmpty(const Iterator *I) const
    {
        return GetLength()==0;
    }
	virtual EmptyObject *GetAtIterator(const Iterator *I) const;
		/// will set a value to the container[Iterator]
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsCryObject,bool IsOwned,size_t Size = 0) ;
    /// abstract function used by subclasses to remove the item this iterator points at (iterator is still valid)
    /// if is not owned then is returned, else is deleted and null is returned
    virtual void RemoveAtIterator(Iterator *I);
    /// abstract function used by subclasses to count the number of items held by the container
    virtual size_t Count() const;
    virtual EmptyObject *Add(EmptyObject *Item,size_t Size);
    virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size);
    virtual CryObject *Add(CryObject *Item);
    virtual CryObject *AddOwned(CryObject *Item);
}
;  // CryMemStream
#endif // TCryMemSteam
}
