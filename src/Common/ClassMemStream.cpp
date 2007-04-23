/***************************************************************************
 *   Copyright (C) 2006 by Chris Bruner                                    *
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
#include "ClassMemStream.h"
#include "ClassString.h"
#include "ClassFunction.h"
#include "ClassException.h"

using namespace Crystal;
char *MemStream::gBuffer=NULL; // scrap memory, (volitale)
int MemStream::gBuffSize=0;
int MemStream::gBuffCount=0; // number of strings using it



//-------------------------------------------------------------------
// CryMemStream
//-------------------------------------------------------------------


MemStream::MemStream(const MemStream *E)
{
    gBuffCount++;
    if (E!=this)
    {
        Buffer = new char[E->Length];
        Length = E->Length;
        DataLength = E->DataLength;
        Position = E->Position;
    }
    if (Buffer==0)
        throw Exception("Out of memory creating Memory Stream");
}
MemStream::MemStream(const MemStream &E)
{
    gBuffCount++;
    if (E!=*this)
    {
        Buffer = new char[E.Length];
        Length = E.Length;
        DataLength = E.DataLength;
        Position = E.Position;
    }
    if (Buffer==0)
        throw Exception("Out of memory creating Memory Stream");
}
bool MemStream::GotoFirst(Iterator *I) const
{
    MemStreamIterator *i = (MemStreamIterator *)I;
    if (GetLength())
    {
        i->Offset = 0;
        return true;
    }
    return false;
}

bool MemStream::GotoPrev(Iterator *I) const
{
    MemStreamIterator *i = (MemStreamIterator *)I;
    if (i->Offset && GetLength())
    {
        i->Offset--;
        return true;
    }
    return false;
}
bool MemStream::GotoNext(Iterator *I) const    // returns true if success
{
    MemStreamIterator *i = (MemStreamIterator *)I;
    if (i->Offset < (GetLength()-1))
    {
        i->Offset++;
        return true;
    }
    return false;
}

bool MemStream::GotoLast(Iterator *Iterator) const    // returns true if success
{
    MemStreamIterator *i = (MemStreamIterator *)Iterator;
    int e = GetLength();
    if (e)
    {
        i->Offset = e-1;
        return true;
    }
    return false;
}

void MemStream::RemoveAtIterator(Iterator *I)
{
    throw Exception("Cannot remove at iterator in CryMemoryStream");
}

MemStream::MemStreamIterator::MemStreamIterator(const Container *container): StreamIterator(container)
{
    GotoFirst();
}

void MemStream::Pos2DataLength()
{
    Position = DataLength;
}

void MemStream::Pos2Asciiz()
{
    if (DataLength)
        Position = DataLength-1;
    else
        Position = 0;
}

char *MemStream::GetBuffer()
{
    return Buffer;
}

void MemStream::CheckMem(size_t RequestedSize)
{
    if (Length<RequestedSize)
        ReMem(RequestedSize);
}
void MemStream::Clear()
{
    Position = DataLength = 0;
}
const cbyte* MemStream::GetRaw() const
{
    return (cbyte *)Buffer;
}
/// Return a non-const reference to the ith character
char& MemStream::operator[](unsigned int i)
{
    return Buffer[i];
}

/// Return a const reference to the ith character
const char& MemStream::operator[](unsigned int i) const
{
    return Buffer[i];
}

const char *MemStream::AsPChar() const
{
    return (const char *) Buffer;
}
void MemStream::SetRaw(unsigned int i,cbyte v)
{
    Buffer[i]=v;
}
void MemStream::SetRaw(unsigned int Start,const cbyte *v,size_t Length)
{
    memcpy(&Buffer[Start],v,Length);
}
size_t MemStream::GetLength() const
{
	return DataLength;
}
void MemStream::SetLength(int i)
{
    CheckMem(i);
    DataLength = i;
}
MemStream::operator const char *() const
{
    return Buffer;
}

void MemStream::SetPosition(size_t p)
{
    Position = p;
}

size_t MemStream::GetPosition() const
{
    return Position;
}

bool MemStream::operator ==(const MemStream *s)
{
    return *this==*s;
}

bool MemStream::operator !=(const MemStream *s)
{
    return *this!=*s;
}

size_t MemStream::Size() const
{
    return DataLength;
}

size_t MemStream::Read(Stream *ToStream) const
{
    return Read(ToStream,Size());
}

size_t MemStream::Write(const Stream *FromStream)
{
    return Write(FromStream,FromStream->Size());
}

int MemStream::SeekToStart() const
{
    ((MemStream *)this)->Position = 0;
    return 0;
}


bool MemStream::IsOpen()  const
{
    return true;
}

bool MemStream::Open(const char *Name,const char *Operation,bool ExceptOnError)
{
    DataLength = 0;
    return true;
}

void MemStream::Close(bool ExceptOnError)
{
    DataLength = 0;
};

void MemStream::Flush()
{}
;

Container::Iterator *MemStream::_CreateIterator() const
{
    return new MemStreamIterator(this);
};

void MemStream::DeleteIterator(Iterator *I) const
{
    delete (MemStreamIterator *) I;
}

EmptyObject *MemStream::GetAtIterator(const Iterator *I) const
{
	const MemStreamIterator *i = (const MemStreamIterator *)I;
	return (EmptyObject *) &Buffer[i->Offset];
}

void MemStream::SetAtIterator(const Iterator *I,EmptyObject *Item,bool IsObject,bool IsOwned,size_t Size)
{
const MemStreamIterator *i = (const MemStreamIterator *)I;
	memcpy(&Buffer[i->Offset],Item,Size);
}

size_t MemStream::Count() const
{
	return GetLength();
}

EmptyObject *MemStream::Add(EmptyObject *Item,size_t Size)
{
	Write((const char *)Item,Size);
	return Item;
}

EmptyObject *MemStream::AddOwned(EmptyObject *Item,size_t Size)
{
	Write((const char *)Item,Size);
	return Item;
}

Object *MemStream::Add(Object *Item)
{
	return Stream::Add(Item);
}    // returns Item

Object *MemStream::AddOwned(Object *Item)
{
    return Stream::AddOwned(Item);
}   // gives ownership to list




FunctionDefList *MemStream::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return Stream::GetFunctions(Type);
	// otherwise get any functions in subclasses
	FunctionDefList *l = Stream::GetFunctions();
    String s;
	s += "// Class MemStream;";
    s += "size_t Resize(size_t s);";
    s += "virtual void Clear();";
    s += "virtual void Clear(int amount);";
    s += "void CopyToStream(CryStream &Dest) const;";
    s += "const char* ClassName() const;";
    s += "virtual CryString *GetFunctions() const;";
    s += "virtual const char *ChildClassName() const;";
    s += "virtual const cbyte* GetRaw() const;";
    s += "char& operator[](unsigned int i);";
    s += "const char& operator[](unsigned int i) const";
    s += "virtual const char *AsPChar() const;";
    s += "virtual void SetRaw(unsigned int i,cbyte v);";
    s += "virtual void SetRaw(unsigned int Start,const cbyte *v,size_t Length);";
    s += "virtual size_t GetLength() const;";
    s += "virtual void SetLength(int i);";
    s += "virtual operator const char *() const;";
    s += "void Compress();";
    s += "void Decompress();";
    s += "bool operator ==(const CryMemStream &s);";
    s += "bool operator !=(const CryMemStream &s);";
    s += "bool operator ==(const CryMemStream *s);";
    s += "bool operator !=(const CryMemStream *s);";
    s += "bool operator ==(const char *s);";
    s += "bool operator !=(const char *s);";
    s += "virtual int Seek(int offset,int whence) const;";
    s += "virtual int SeekToStart() const;";
    s += "virtual int SeekFromStart(int Offset=0) const;";
    s += "virtual int SeekFromCurrent(int Offset) const;";
    s += "virtual int SeekFromEnd(int Offset=0) const;";
    s += "virtual CryMemStream &MemDelete(unsigned int start,int amount);";
    s += "virtual size_t Size() const;";
    s += "virtual void CopyTo(Object &Dest) const;";
	s += "virtual Object *Dup() const;";
    s += "virtual size_t ReadTI(char *ToBuffer,size_t Size) const;";
    s += "virtual size_t WriteTI(const char *FromBuffer,size_t Size);";
    s += "virtual size_t ReadTI(CryStream *ToBuffer,size_t Size) const;";
    s += "virtual size_t WriteTI(CryStream *FromBuffer,size_t Size);";
    s += "virtual size_t Read(char *ToBuffer,size_t Size) const;";
    s += "virtual size_t Write(const char *FromBuffer,size_t Size);";
    s += "virtual size_t Read(CryStream *ToStream,size_t Size) const;";
    s += "virtual size_t Write(const CryStream *FromStream,size_t Size);";
    s += "virtual size_t Read(CryStream *ToStream) const;";
    s += "virtual size_t Write(const CryStream *FromStream);";
    s += "virtual size_t Tell() const;";
    s += "virtual bool Eof() const;";
    s += "int Compare(CryMemStream *MemStream);";
    s += "virtual bool IsOpen() const;";
    s += "virtual bool Open(const char *Name,const char *Operation,bool ExceptOnError=true);";
    s += "virtual void Close(bool ExceptOnError=true);";
    s += "virtual void Flush();";
    s += "virtual bool IsA(const char *ClassName) const;";
    s += "int vsscanf(const char *format, va_list ap) const;";
    s += "int vsprintf(const char *format, va_list ap);";
    s += "virtual int scanf(const char *format,...) const;";
    s += "virtual size_t printf(const char *format,...);";
    l->LoadFromString(s,";");
    return l;
}
MemStream::MemStream()
{
	gBuffCount++;

	Buffer = new char[101];
	Length = 100;
	DataLength = 0;
    Position = 0;
    if (Buffer==0)
        throw Exception("Out of memory creating Memory Stream");

}

MemStream::~MemStream()
{
Buffer[0]='a';
    delete [] Buffer;
    Length = 0;
    DataLength = 0;
    Position = 0;
    gBuffCount--;
    if (gBuffCount==0)
    {
        delete [] gBuffer;
        gBuffSize =0;
        gBuffer =0;
    }
}
void MemStream::ReMem(size_t Value)
{
    char *OldBuffer = Buffer;
    Buffer = new char[Value + 1];
    if (Buffer==0)
        throw Exception(this,"out of memory");
    memcpy(Buffer,OldBuffer,DataLength);
    Buffer[DataLength] = '\0';    // just to make it easier for strings
    delete [] OldBuffer;
    Length = Value;
}
// make the memory stream have an 0 at the end.
void MemStream::LookLikeString() const
{
    MemStream *v = (MemStream *) this;    // making this into non const
    if (DataLength>=Length)
        v->ReMem(DataLength+1);

    v->Buffer[DataLength] = '\0';

    v->Position = 0;

}

void MemStream::Compress()
{}
void MemStream::Decompress()
{}

MemStream &MemStream::MemDelete(unsigned int start,int amount)
{
    if (start>=DataLength)
        return *this;
    if (start+amount>DataLength)
        amount = DataLength - start;
    memmove(Buffer+start,Buffer+start+amount,DataLength +1 - (start+amount));
    DataLength -= amount;
    return *this;
}
int MemStream::vsscanf(const char *format, va_list ap) const
{
    return ::vsscanf(Buffer+Position,format,ap);
}


int MemStream::scanf(const char *format,...) const
{
    va_list argptr;
    va_start(argptr,format);
    return ::vsscanf(Buffer,format,argptr);
}

size_t MemStream::printf(const char *format,...)
{ // this is normally a string function so we will assume that this class is being used as a string
    LookLikeString();
    va_list argptr;
    va_start(argptr,format);
    return vsprintf(format,argptr);
}
int MemStream::vsprintf(const char *format, va_list ap)
{
    int size = ::vsnprintf(gBuffer,gBuffSize,format,ap);
    while(size>=gBuffSize)
    {
        delete []gBuffer;
        if (size<1000)
            size = 1000;
        else
            size *=2;
        gBuffer = new char[size+1];
        gBuffSize = size+1;
        size = ::vsnprintf(gBuffer,gBuffSize,format,ap);
    }
    unsigned int newsize = strlen(Buffer)+strlen(gBuffer)+1;
    if (Length<newsize)
        this->ReMem(newsize);
    strcat(Buffer,gBuffer);
    return DataLength = strlen(Buffer) + 1;	// Memory includes the asciiz in length
}

void MemStream::CopyTo(Object &Dest) const
{
    if (Dest.IsA(CStream))
        CopyToStream(*(MemStream *)&Dest);
    else
        Stream::CopyTo(Dest);    // see if any base class can handle it
}


bool MemStream::operator ==(const MemStream &s)
{
    size_t i = s.DataLength;
    if (i>DataLength)
        i= DataLength;
    return memcmp(Buffer,s.Buffer,i)==0;
}
bool MemStream::operator !=(const MemStream &s)
{
    size_t i = s.DataLength;
    if (i>DataLength)
        i= DataLength;
    i = memcmp(Buffer,s.Buffer,i);
    if (i==0)   // same
        return false;
    else
        return true;
    //    return !memcmp(Buffer,s.Buffer,i)==0;
}
bool MemStream::operator ==(const char *s)
{
    int i = DataLength;
    return strncmp(Buffer,s,i)==0;
}
bool MemStream::operator !=(const char *s)
{
    int i = DataLength;
    i = strncmp(Buffer,s,i);
    if (i==0)
        return false;
    else
        return true;
}
int MemStream::Compare(int CompareType,const Object *Test1,const Object *Test2) const
{
    return Test1->CompareLogical(CompareType,Test2);
}
int MemStream::CompareLogical(int CompareType,const Object *Test) const
{
    if (Test->IsA(CMemStream))
    {
        switch(CompareType)
        {
        case 1:
            return ((const MemStream *)Test)->Compare(this);
        default:
            return Compare((MemStream *)Test);
        }
    }
    return 0;
}

int MemStream::Compare(const MemStream *MemStream) const
{
    if (DataLength<MemStream->DataLength)
        return -1;
    if (DataLength>MemStream->DataLength)
        return 1;
    return memcmp(Buffer,MemStream->Buffer,DataLength);
}


int MemStream::Seek(int offset,int whence) const
{
    MemStream *t = (MemStream *) this;// allowing non const change to const object
    switch (whence)
    {
    case SEEK_SET:
        {
            if (offset<0)
                throw Exception(this,"Seek less then 0");
            else if ((size_t) offset >= Length)
                t->ReMem(offset + 1);
            t->Position = offset;
        }
        break;
    case SEEK_CUR:
        {
            if (offset + Position > Length)
                t->ReMem(offset + Position + 1);
            else if ((offset<0) && ((size_t)(-offset) > Position))
                throw Exception(this,"Seek less then 0");
            t->Position = offset + Position;
        }
        break;
    case SEEK_END:
        {
            if (offset + Length> Length)
                t->ReMem(offset + Length + 1);
            else if ((offset<0) && ((size_t)(-offset) > Length))
                throw Exception(this,"Seek less then 0");
            t->Position = offset + DataLength;
        }
    }
    return Position;
}

int MemStream::SeekFromStart(int Offset) const
{
    return Seek(Offset,SEEK_SET);
}
int MemStream::SeekFromCurrent(int Offset) const
{
    return Seek(Offset,SEEK_CUR);
}
int MemStream::SeekFromEnd(int Offset) const
{
    return Seek(Offset,SEEK_END);
}

size_t MemStream::ReadTI(char *ToBuffer,size_t Size) const
{
    if ((Size + Position)> DataLength)
    {
        //        throw CryException(this,"Read past end of stream");
        Size = DataLength - Position;
    }
    const char *Dest = (char *)memchr(Buffer+Position, GetTerminator(), Size);
    if (Dest)
        Size = Dest - (Buffer + Position)+1;
    if (Size)
        memcpy(ToBuffer,Buffer+Position,Size);
    MemStream *t = (MemStream *) this;    // making change to const function
    t->Position += Size;
    return Size;
}
// read until terminator or Size (inclusive)
size_t MemStream::ReadTI(Stream *ToBuffer,size_t Size) const
{
    if (ToBuffer->IsA(CMemStream))
    {
        MemStream *mToBuffer = (MemStream *)ToBuffer;
        if (mToBuffer->Length < Size)
            mToBuffer->ReMem(Size+1);
        mToBuffer->SetLength(ReadTI(mToBuffer->Buffer,Size));
        return mToBuffer->GetLength();
    }
    else
    {
        return Stream::ReadTI(ToBuffer,Size);
    }
}
// write until terminator or size (inclusive)
size_t MemStream::WriteTI(Stream *FromBuffer,size_t Size)
{
    if (FromBuffer->IsA(CMemStream))
    {
        MemStream *mFromBuffer = (MemStream *)FromBuffer;
        if (mFromBuffer->Length < Size)
            mFromBuffer->ReMem(Size+1);
        return WriteTI(mFromBuffer->Buffer,Size);
    }
    else
        return Stream::WriteTI(FromBuffer,Size);
}
void MemStream::Zero()
{
	memset(Buffer,0,Length);
}
void MemStream::Clear(int amount) // 0 == all, 1.. from start, -1..-N from end
{
    if (amount==0)
        Clear();
    else
    {
        if (amount>0)
        {
            if ((unsigned int) amount>DataLength)
                Clear();
            else
            {
                memmove(Buffer,Buffer + amount,DataLength - amount+1);
                DataLength -= amount;
            }
        }
        else    // clear from end
        {
            unsigned int uamount = -amount;
            if (uamount>DataLength)
                Clear();
            else
                DataLength -= uamount;
        }
    }
}
size_t MemStream::WriteTI(const char *FromBuffer,size_t Size)
{
    const char *Dest = (char *)memchr(FromBuffer, GetTerminator(), Size);
    if (Dest)
        Size = Dest - FromBuffer+1;
    if ((Size + Position)>Length)
        ReMem(Size+Length+1);
    memcpy(Buffer + Position,FromBuffer,Size);
    Position += Size;
    if (DataLength<Position)
        DataLength = Position;
    if (Dest)
        return Size+1;
    else
        return Size;
}

size_t MemStream::Read(char *ToBuffer,size_t Size) const
{
    if ((Size + Position)> DataLength)
    {
        if (DataLength>Position)
            Size = DataLength - Position;
        else
            Size = 0;
    }
    if (Size)
        memcpy(ToBuffer,Buffer+Position,Size);
    MemStream *t = (MemStream *) this;// allowing non const change to const object
    t->Position += Size;
    return Size;
}

size_t MemStream::Write(const char *FromBuffer,size_t Size)
{
    if (Size)
    {
        if ((Size + Position)>Length)
            ReMem(Size+Length+1);
        memcpy(Buffer + Position,FromBuffer,Size);
        Position += Size;
        if (DataLength<Position)
            DataLength = Position;
    }
    return Size;
}

size_t MemStream::Read(Stream *ToStream,size_t Size) const
{
    if (Size + Position>DataLength)
        Size = DataLength - Position;
    if (Size)
    {
        size_t w = ToStream->Write(Buffer + Position,Size);
        if (w!=Size)
        {
            MemStream *t = (MemStream *) this;// allowing non const change to const object
            t->Position+=w;
            return w;
        }
        MemStream *t = (MemStream *) this;// allowing non const change to const object
        t->Position += Size;
    }
    return Size;
}
size_t MemStream::Write(const Stream *FromStream,size_t Size)
{
    if (Size)
    {
        if (Position + Size > Length)
            ReMem(Position + Size + 1);
        Size = FromStream->Read(Buffer + Position,Size);
        Position += Size;
        if (DataLength<Position)
            DataLength = Position;
    }
    return Size;
}


size_t MemStream::Resize(size_t s)
{
    if (s<DataLength)
        DataLength = s;
    else
        throw Exception(this,"Resizing beyond current data");
    return DataLength;
}


size_t MemStream::Tell() const
{
    return Position;
}
bool MemStream::Eof() const
{
    return Position == DataLength;
}
