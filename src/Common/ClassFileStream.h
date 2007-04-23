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

#include "ClassStream.h"

namespace Crystal
{
using namespace std;
#ifndef CFileStream
#define CFileStream "CryFileStream"

/// File Handling stream
class FileStream : public Stream
{
class FileStreamIterator : public StreamIterator
    {
        size_t Size;  // size of stream items
    public:
        FileStreamIterator(const FileStream *Container);
        ~FileStreamIterator();
        void SetSize(size_t _Size);
        size_t GetSize();
    };
    FILE *File;

    bool UpdateOpen;    // r+ open needs seek before input -> output or output->input
    bool LastAccessOutput;
    bool LastAccessInput;
    bool StdStream; // if true this class only redirects, It doesn't open or close
    char StdType;
    FileStream(FileStream &nono);
public:
    StdFunctionsNoDup(FileStream,Stream);
    virtual Object *Dup() const; // creates a duplicate of this object
    virtual const cbyte* GetRaw() const;
	virtual FunctionDefList *GetFunctions(const char *Type=0) const;

    //    const char* ClassName() const;

    //    virtual const char *ChildClassName() const;
	virtual size_t Size() const;
	virtual int Seek(int offset,int whence) const;
    virtual int SeekFromStart(int Offset=0) const;
    virtual int SeekFromCurrent(int Offset) const;
    virtual int SeekFromEnd(int Offset=0) const;
    //virtual void CopyTo(Object *Dest) const;
    //virtual void CopyTo(CryStream *Dest) const;
    //virtual void CopyTo(CryFileStream *Dest) const;   //copies contents of this to Dest
    //    virtual Object *Dup() const; // creates a duplicate of this object

    // read until terminator or Size (inclusive)
    virtual size_t ReadTI(char *ToBuffer,size_t Size) const;
    // write until terminator or size (inclusive)
    virtual size_t WriteTI(const char *FromBuffer,size_t Size);
    // read until terminator or Size (inclusive)
    virtual size_t ReadTI(Stream *ToBuffer,size_t Size) const;
    // write until terminator or size (inclusive)
    virtual size_t WriteTI(Stream *FromBuffer,size_t Size);
    //    virtual bool IsAbstract() const;
    virtual size_t Read(char *ToBuffer,size_t Size) const;
    virtual size_t Write(const char *FromBuffer,size_t Size);
    virtual bool Eof() const;
    virtual size_t Read(Stream *ToStream,size_t Size) const;
	virtual size_t Write(const Stream *FromStream,size_t Size);
    virtual size_t Read(Stream *ToStream) const;
    virtual size_t Write(const Stream *FromStream);
    virtual bool IsOpen() const;
    virtual size_t Tell() const;
    virtual bool Open(const char *Name,const char *Operation,bool ExceptOnError=true);
    virtual void Close(bool ExceptOnError=true);
    virtual void Flush();
    virtual bool ReOpen(const char *FileName,const char *Mode,bool ExceptOnError=true);
    virtual int scanf(const char *format,...) const;
    virtual size_t printf(const char *format,...);
	FileStream();
    virtual ~FileStream();
    // Container functions
	virtual Iterator *_CreateIterator() const;
	Iterator *CreateIterator() const { return _CreateIterator(); }
	virtual void DeleteIterator(Iterator *I) const;
	virtual bool IsEmpty(const Iterator *I) const;
    virtual bool GotoFirst(Iterator *I) const;
    virtual bool GotoPrev(Iterator *I) const; // returns true if success
    virtual bool GotoNext(Iterator *I) const;    // returns true if success
    virtual bool GotoLast(Iterator *Iterator) const;    // returns true if success
	virtual EmptyObject *GetAtIterator(const Iterator *I) const;
	virtual void SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size = 0);

    virtual void RemoveAtIterator(Iterator *I);
    /// abstract function used by subclasses to count the number of items held by the container
	virtual size_t Count() const;
	virtual void Clear();

	virtual EmptyObject *Add(EmptyObject *Item,size_t Size);
	virtual EmptyObject *AddOwned(EmptyObject *Item,size_t Size);
	virtual Object *Add(Object *Item);
	virtual Object *AddOwned(Object *Item);
}
;  // FileStream
#endif //CFileStream
}
