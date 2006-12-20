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
#include <errno.h>

#include "ClassFileStream.h"
#include "ClassString.h"
#include "ClassFunction.h"
#include "ClassException.h"

using namespace Crystal;
using namespace std;
//-------------------------------------------------------------------
// CryFileStream
//-------------------------------------------------------------------


FileStream::FileStreamIterator::FileStreamIterator(const FileStream *Container) : StreamIterator(Container)
{
    Size = 1;
    Offset = Container->Tell();
}
FileStream::FileStreamIterator::~FileStreamIterator()
{}
;
void FileStream::FileStreamIterator::SetSize(size_t _Size)
{
    Size = _Size;
}
size_t FileStream::FileStreamIterator::GetSize()
{
    return Size;
}

const cbyte* FileStream::GetRaw() const
{
    return (const cbyte *) File;
}

size_t FileStream::Read(Stream *ToStream) const
{
    return Read(ToStream,Size());
}

size_t FileStream::Write(const Stream *FromStream)
{
    return Write(FromStream,FromStream->Size());
}

bool FileStream::IsOpen() const
{
    return File!= 0;
}

bool FileStream::IsEmpty(const Iterator *I) const
{
    return Size()==0;
}

Object *FileStream::Add(Object *Item)
{
    return Stream::Add(Item);
}    // returns Item

Object *FileStream::AddOwned(Object *Item)
{
    return Stream::AddOwned(Item);
}   // gives ownership to list

size_t FileStream::Count() const
{
    return Size();
}

void FileStream::DeleteIterator(Iterator *I) const
{
    delete (FileStreamIterator *) I;
}

size_t FileStream::Tell() const
{
    if (File)
    {
        return (size_t)ftell(File);
    }
    else
        throw Exception(this,"Stream Not Open");
}

Object *FileStream::Dup() const // creates a duplicate of this object
{
    if (!IsOpen())
        throw Exception(this,"Stream Not Open");
    FileStream *Result = new FileStream;
    CopyTo(*Result);
    return Result;
}
FunctionDefList *FileStream::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return Stream::GetFunctions(Type);
	// otherwise get any functions in subclasses
	FunctionDefList *l = Stream::GetFunctions();
    String s;
    s += "// Class CryFilestream;";
    s += "virtual const cbyte* GetRaw() const;";
    s += "virtual CryString *GetFunctions() const;";
    s += "const char* ClassName() const;";
    s += "virtual const char *ChildClassName() const;";
    s += "virtual size_t Size() const;";
    s += "virtual int Seek(int offset,int whence) const;";
    s += "virtual int SeekFromStart(int Offset=0) const;";
    s += "virtual int SeekFromCurrent(int Offset) const;";
    s += "virtual int SeekFromEnd(int Offset=0) const;";
    s += "virtual Object *Dup() const;";
    s += "virtual size_t ReadTI(char *ToBuffer,size_t Size) const;";
    s += "virtual size_t WriteTI(const char *FromBuffer,size_t Size);";
    s += "virtual size_t ReadTI(CryStream *ToBuffer,size_t Size) const;";
    s += "virtual size_t WriteTI(CryStream *FromBuffer,size_t Size);";
    s += "virtual size_t Read(char *ToBuffer,size_t Size) const;";
    s += "virtual size_t Write(const char *FromBuffer,size_t Size);";
    s += "virtual bool Eof() const;";
    s += "virtual size_t Read(CryStream *ToStream,size_t Size) const;";
    s += "virtual size_t Write(const CryStream *FromStream,size_t Size);";
    s += "virtual size_t Read(CryStream *ToStream) const;";
    s += "virtual size_t Write(const CryStream *FromStream);";
    s += "virtual size_t Tell() const;";
    s += "virtual bool IsOpen() const;";
    s += "virtual bool Open(const char *Name,const char *Operation,bool ExceptOnError=true);";
    s += "virtual void Close(bool ExceptOnError=true);";
    s += "virtual void Flush();";
    s += "virtual bool ReOpen(const char *FileName,const char *Mode,bool ExceptOnError=true);";
    s += "virtual bool IsA(const char *ClassName) const;    // can the object map to a ClassName;";
    s += "virtual int scanf(const char *format,...) const;";
    s += "virtual size_t printf(const char *format,...);";
    l->LoadFromString(s,";");
    return l;
}

int FileStream::scanf(const char *format,...) const
{
    va_list argptr;
    va_start(argptr,format);
    return vfscanf(File,format,argptr);
}
size_t FileStream::printf(const char *format,...)
{
    va_list argptr;
    va_start(argptr,format);
    return vfprintf(File,format,argptr);
}

// read until terminator or Size (inclusive)
size_t FileStream::ReadTI(char *ToBuffer,size_t Size) const
{
    char T = GetTerminator();
    for(size_t i=0;i<Size;i++)
    {
        if (Read(ToBuffer+i,1)!=1)
            return i;
        if (ToBuffer[i]==T)
            return i+1;
    }
    return Size;
}
// read until terminator or Size (inclusive)
size_t FileStream::ReadTI(Stream *ToBuffer,size_t Size) const
{
    char *buffer = new char[Size+1];
    size_t s = ReadTI(buffer,Size);
    s = ToBuffer->Write(buffer,s);
    delete []buffer;
    return s;
}
// write until terminator or size (inclusive)
size_t FileStream::WriteTI(Stream *FromBuffer,size_t Size)
{
    char t = FromBuffer->GetTerminator();
    FromBuffer->SetTerminator(GetTerminator());
    size_t s =  FromBuffer->ReadTI(this,Size);
    FromBuffer->SetTerminator(t);
    return s;
}

// write until terminator or size (inclusive)
size_t FileStream::WriteTI(const char *FromBuffer,size_t Size)
{
    char T = GetTerminator();
    const char *Dest = (char *)memchr(FromBuffer,T,Size);
    if (Dest)
        Size = Dest - FromBuffer+1;
    return Write(FromBuffer,Size);
}


size_t FileStream::Read(char *ToBuffer,size_t Size) const
{
    if (File)
        return fread(ToBuffer,1,Size,File);
    else
        throw Exception(this,"Read on unopened Stream");
}
size_t FileStream::Write(const char *FromBuffer,size_t Size)
{
    if (File)
        return fwrite(FromBuffer,1,Size,File);
    else
        throw Exception(this,"Write on unopened Stream");
}

bool FileStream::Eof() const
{
    if (File)
        return feof(File);
    else
        throw Exception(this,"Eof on unopened Stream");
}

size_t FileStream::Read(Stream *ToStream,size_t Size) const
{
    char Buffer[1025];
    size_t l,t,r,w;
    t = 0;
    while(Size!=0)
    {
        size_t Start = Tell();
        if (Size>1024)
            r = 1024;
        else
            r = Size;
        l = Read(Buffer,r);
        w = ToStream->Write(Buffer,l);
        if (w!=l)   // stream write terminated early
        {
            SeekFromStart(Start + w);
            return w;
        }
        t+=l;
        if ((l==0) || Eof())
            return t;
        Size -= r;
    }
    return t;
}
size_t FileStream::Write(const Stream *FromStream,size_t Size)
{
    return FromStream->Read(this,Size);
}


size_t FileStream::Size() const
{
    if (StdStream)
        return 0;
    FileStream *f = (FileStream *) this;
    if (!f->IsOpen())
        return 0;
    size_t CurrentLocation = f->Tell();
    f->SeekFromEnd();
    size_t Result = f->Tell();
    f->SeekFromStart(CurrentLocation);
    return Result;
}


bool FileStream::GotoFirst(Iterator *I) const
{
    if (!IsOpen())
        throw Exception("Cannot use Iterator on Closed Filestream");
    SeekFromStart(0);
    return true;
}
bool FileStream::GotoPrev(Iterator *I) const // returns true if success
{
    if (!IsOpen())
        throw Exception("Cannot use Iterator on Closed Filestream");
    FileStreamIterator *i = (FileStreamIterator *)I;
    unsigned int iSize = i->GetSize();
    if (i->Offset > iSize)
        i->Offset -= iSize;
    else
        return false;
    if (SeekFromStart(i->Offset)==(int)i->Offset)
        return true;
    return false;
}
bool FileStream::GotoNext(Iterator *I) const    // returns true if success
{
    if (!IsOpen())
        throw Exception("Cannot use Iterator on Closed Filestream");
    FileStreamIterator *i = (FileStreamIterator *)I;
    unsigned int iSize = i->GetSize();
    if (i->Offset + iSize < Size() )
        i->Offset += iSize;
    else
        return false;
    if (SeekFromStart(i->Offset)==(int)i->Offset)
        return true;
    return false;
}

Container::Iterator *FileStream::_CreateIterator() const
{
    if (IsOpen())
    {
        return new FileStreamIterator(this);
    }
    else
        throw Exception("Cannot create Iterator on Closed FileStream");
}

bool FileStream::GotoLast(Iterator *Iterator) const    // returns true if success
{
    if (!IsOpen())
        throw Exception("Cannot use Iterator on Closed Filestream");
    FileStreamIterator *i = (FileStreamIterator *)Iterator;
    int S = Size(),iSize = i->GetSize();


    if (S < iSize)
        return false; // no items?
    i->Offset = S - iSize;
    if (SeekFromStart(i->Offset)==(int)i->Offset)
        return true;
    return false;
}

void FileStream::RemoveAtIterator(Iterator *I)
{
	Exception("Cannot use RemoveAtIterator with FileStreams.");
}

EmptyObject *FileStream::GetAtIterator(const Iterator *I) const
{
	if (!IsOpen())
		throw Exception("Cannot use Iterator on Closed Filestream");
	Exception("Cannot use GetAtIterator with FileStreams. Use Read or Write instead");
	return 0;
}

void FileStream::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
{
	if (!IsOpen())
		throw Exception("Cannot use Iterator on Closed Filestream");
	Exception("Cannot use SetAtIterator with FileStreams. Use Read or Write instead");
}

EmptyObject *FileStream::Add(EmptyObject *Item,size_t Size)
{
    Exception("Cannot use Add with FileStreams.");
    return 0;
}
EmptyObject *FileStream::AddOwned(EmptyObject *Item,size_t Size)
{
    Exception("Cannot use AddOwned with FileStreams.");
    return 0;
}
void FileStream::Clear()
{
    Exception("Cannot use Clear with FileStreams.");
}

FileStream::FileStream()
{
    StdStream = false;
    StdType = 0;
    File = 0;
}
FileStream::~FileStream()
{
    Close(false);
}
bool FileStream::Open(const char *Name,const char *Operation,bool ExceptOnError)
{
    Tag = 0;
    if (!StdStream && File)
        throw Exception(this,"Open of already Opened Stream");
    if (StdStream)
        Close();
    if (strcmp(Name,"stdout")==0)
    {
        StdType = 'O';
        StdStream = true;
        File = stdout;
        return true;
    }
    else if (strcmp(Name,"stderr")==0)
    {
        StdType = 'E';
        StdStream = true;
        File = stderr;
        return true;
    }
    else if (strcmp(Name,"stdin")==0)
    {
        StdType = 'I';
        StdStream = true;
        File = stdin;
        return true;
    }

    File = fopen(Name,Operation);
    if (!File)
    {
        if (ExceptOnError)

            throw Exception(this,errno,"Could not Open ",Name);
        else
            return false;
    }
    return true;
}
void FileStream::Close(bool ExceptOnError)
{
    if (File)
    {
        if ((!StdStream) && (fclose(File) && ExceptOnError))
            throw Exception(this,"Error closing File");
        File = 0;
        StdStream = 0;
    }
    else
        if (ExceptOnError)
            throw Exception(this,"Closing unopened File");
}
void FileStream::Flush()
{
    if (File)
        fflush(File);
}
int FileStream::Seek(int Offset,int whence) const
{
    if (File)
    {
        if (fseek(File,Offset,whence))
            throw Exception(this,errno,"Seek");
        else
        {
            return ftell(File);
            // if dos verify location
        }
    }
    else
        throw Exception(this,"Stream Not Open");
}
int FileStream::SeekFromStart(int Offset) const
{
    try
    {
        return Seek(Offset,SEEK_SET);
    }
    catch (Exception E)
    {
        E.SetErrorStr("SeekFromStart");
        throw(E);
    }
}

int FileStream::SeekFromEnd(int Offset) const
{
    try
    {
        return Seek(Offset,SEEK_END);
    }
    catch (Exception E)
    {
        E.SetErrorStr("SeekFromEnd");
        throw(E);
    }
}
int FileStream::SeekFromCurrent(int Offset) const
{
    try
    {
        return Seek(Offset,SEEK_CUR);
    }
    catch (Exception E)
    {
        E.SetErrorStr("SeekFromCurrent");
        throw(E);
    }
}
bool FileStream::ReOpen(const char *FileName,const char *Mode,bool ExceptOnError)
{
    if (!File)
        throw Exception(this,"ReOpen of file that wasn't open");
    if (freopen(FileName, Mode, File)== NULL)
    {
        if (ExceptOnError)
            throw Exception(this,errno,"error redirecting Stream to ",FileName);
        else
            return false;
    }
    return true;
}
