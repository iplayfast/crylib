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
#include "ClassException.h"
#include "ClassFunction.h"
#include "ClassProperty.h"
#include "zlib.h"
using namespace Crystal;

//-------------------------------------------------------------------
// CryStream
//-------------------------------------------------------------------

#ifdef VALIDATING
bool CryStream::Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose, const char *Result,bool fail))
{
    char Result[200];
    bool Fail = false;
//    CryMemStream *DebugViewMemStream = (CryMemStream *)&Object;
    bool IsString=Object.IsA(TCryString);	// some tests have different results for strings
    size_t t;
    if (Object.IsA(TCryStream))
    {
        CryStream *s = (CryStream *)&Object;
        sprintf(Result,"\nCryStream Testing:\nObject UnNamed of ClassName %s,ChildClassName %s",
                s->ClassName(),s->ChildClassName());
        if (!CallBack(Verbose,Result,Fail))
            return false;
        try
        {
            if (Verbose)
                CallBack(Verbose,"\nOpening Test.dat in w+b mode",false);
            if (!s->Open("test.dat","w+b",true))
            {
                Fail = true;
                sprintf(Result,"\nCould not open test.dat");
                if (!CallBack(Verbose,Result,Fail))
                    return false;
                return false;
            }
        }
        catch(CryException &E)
        {
            Fail = true;
            sprintf(Result,"\nException on opening Could not open test.dat");
            if (!CallBack(Verbose,Result,Fail))
                return false;
            return false;
        }
        try
        {
            if (!CallBack(Verbose,"\nOpen \"w+b\" succeeded",Fail))
                return false;

            if (Verbose)
                CallBack(Verbose,"\nSeek tests",false);
            s->SeekFromStart(100);
            if (s->Tell()!=100)
            {
                Fail = true;
                sprintf(Result,"\nSeek From Start to location 100 returned %d",s->Tell());
                if (!CallBack(Verbose,Result,Fail))
                {
                    s->Close();
                    return false;
                }
            }
            s->SeekFromEnd(0);
            if (s->Tell()!=0)
            {
                Fail = true;
                sprintf(Result,"\nSeek From End to location 0 failed returned %d should be 0",s->Tell());
                if (!CallBack(Verbose,Result,Fail))
                {
                    s->Close();
                    return false;
                }
            }
            s->SeekFromCurrent(50);
            if (s->Tell()!=50)
            {
                Fail = true;
                sprintf(Result,"\nSeek From Current to location 50 failed returned %d should be 50",s->Tell());
                if (!CallBack(Verbose,Result,Fail))
                {
                    s->Close();
                    return false;
                }
            }
            if (Verbose)
                CallBack(Verbose,"\nRead/Write tests",false);
            char Buffer[50];
            unsigned int r = s->Read(Result,10);
            if (r!=0)
            {
                Fail = true;
                sprintf(Result,"\nRead from empty stream returned %d results!",r);
                if (!CallBack(Verbose,Result,Fail))
                {
                    s->Close();
                    return false;
                }
            }
            if (s->Tell()!=50)
            {
                Fail = true;
                sprintf(Result,"\nEmpty Read Moved location from 50 to %d should be 50",s->Tell());
                if (!CallBack(Verbose,Result,Fail))
                {
                    s->Close();
                    return false;
                }
            }
            const char *test = "The quick brown fox";
            memset(Buffer,1,50);
            if (IsString)
                t = 22;
            else
                t = (unsigned)(strlen(test)+sizeof(size_t));
            if ((r=s->WriteNStr(test))!= t)
            {
                Fail = true;
                sprintf(Result,"\nWrong WriteNStr length of %d returned should be %d",r,strlen(test)+sizeof(size_t));
                if (!CallBack(Verbose,Result,Fail))
                {
                    s->Close();
                    return false;
                }
            }
            s->SeekFromStart(50);
            if (s->Tell()!=50)
            {
                Fail = true;
                sprintf(Result,"\nSeekFromStart after write to %d should be 50",s->Tell());
                if (!CallBack(Verbose,Result,Fail))
                {
                    s->Close();
                    return false;
                }
            }
            s->SeekFromEnd(0);
            {
                unsigned int start = 50 + strlen(test) + sizeof(size_t);	// for files and memory
                if (IsA(TCryString))
                    start = 22; // strings see the 0 and mark as end of str
                if ((r=s->Tell())!=(unsigned)(start ))
                {
                    Fail = true;
                    sprintf(Result,"\nFilesize after write is %d should be %d ",r,start + strlen(test) + sizeof(size_t));
                    if (!CallBack(Verbose,Result,Fail))
                    {
                        s->Close();
                        return false;
                    }
                }
            }

            s->SeekFromStart(50);
            r = s->ReadNStr(Buffer,50);
            if ((r!=(unsigned)strlen(test)))
            {
                Fail = true;
                sprintf(Result,"\nReadNStr returned \"%s\" read length %d,\n should be \"%s\" length %d",Buffer,r,test,strlen(test));
                if (!CallBack(Verbose,Result,Fail))
                {
                    s->Close();
                    return false;
                }
            }

            if (Verbose)
                CallBack(Verbose,"\nCopyTo (memstream) tests",false);
            CryMemStream ms;
            s->CopyTo(ms);
            int extra = 0;
            if (IsString)
                extra = 1;	// 0 at end will be copied to memstrem so sizes will differ by 1, since string doesn't include it
            if (ms.Size()!= s->Size()+extra)
            {
                Fail = true;
                sprintf(Result,"\nCopyTo CryMemStream size is %d, original size is %d",ms.Size(),s->Size());
                if (!CallBack(Verbose,Result,Fail))
                {
                    s->Close();
                    return false;
                }
            }
            ms.SeekFromStart(0);
            s->SeekFromStart(0);
            for (unsigned int i=0;i<ms.Size();i++)
            {
                char mb[2],sb[2];
                ms.Read(mb,1);
                s->Read(sb,1);
                if (mb[0]!=sb[0])
                {
                    Fail = true;
                    sprintf(Result,
                            "\nDifference at position %d original %c CopyTo %c ",
                            i,sb[0],mb[0]);
                    if (!CallBack(Verbose,Result,Fail))
                    {
                        s->Close();
                        return false;
                    }
                }
            }

            s->Close();

        }
        catch(CryException &E)
        {
            s->Close();
            throw E;
        }
    }

    return CryObject::Test(Verbose,Object,CallBack);

    /*
        virtual void CopyTo(CryObject *Object) const;
        virtual CryObject *Dup() const =0; // creates a duplicate of this object

        virtual void SetTerminator(char Terminator_) {  Terminator[0] = Terminator_; Terminator[1] = '\0'; }
        virtual char GetTerminator() const {  return Terminator[0]; }
        // read until terminator or Size (inclusive)
        virtual size_t ReadTI(char *ToBuffer,size_t Size);
        // write until terminator or size (inclusive)
        virtual size_t WriteTI(const char *FromBuffer,size_t Size);
        // read until terminator or Size (inclusive)
        virtual size_t ReadTI(CryStream *ToBuffer,size_t Size);
        // write until terminator or size (inclusive)
        virtual size_t WriteTI(CryStream *FromBuffer,size_t Size);
     
        // read until terminator or Size (exclusive)
        virtual size_t ReadTE(char *ToBuffer,size_t Size);
        // write until terminator or size (exculusive)
        virtual size_t WriteTE(const char *FromBuffer,size_t Size);
        // read until terminator or Size (exclusive)
        virtual size_t ReadTE(CryStream *ToBuffer,size_t Size);
        // write until terminator or size (Exclusive)
        virtual size_t WriteTE(CryStream *FromBuffer,size_t Size);
     
        virtual char *StreamedClass(char ClassNameBuffer[TMaxClassNameSize]) const; // will LoadFrom the Buffer with the name of the next class in the stream to LoadFrom
        virtual size_t Read(CryStream *ToStream,size_t Size)=0;
        virtual size_t Write(CryStream *FromStream,size_t Size)=0;
        virtual size_t Read(CryStream *ToStream) {  return Read(ToStream,Size()); }
        virtual size_t Write(CryStream *FromStream) {  return Write(FromStream,FromStream->Size()); }
        virtual void SaveTo(CryStream *ToStream);
        virtual void LoadFrom(CryStream *FromStream);
        virtual size_t WriteNStr(const char *Buffer);
        virtual size_t ReadNStr(char *Buffer,size_t MaxLength);
        virtual size_t Size() const = 0;
        virtual size_t Tell() const =0;
        virtual bool Eof() const =0;
        virtual bool Open(const char *Name,const char *Operation,bool ExceptOnError=true)=0;
        virtual void Close(bool ExceptOnError=true)=0;
        virtual void Flush() =0;
        virtual bool IsA(const char *ClassName) const;    // can the object map to a ClassName
        virtual bool Event(CryObject::EObject EventNumber,CryObject::Context::IO &Context);
        virtual bool Event(EObject EventNumber,Context::UIO &Context);
        virtual int scanf(const char *format,...)=0;
        virtual size_t printf(const char *format,...)=0;
     
    // if this class contains the property name, it will attempt to load it
    // if all is well returns true
        virtual bool SetProperty(CryString *PropertyName,CryString *PropertyValue);
        virtual bool HasProperty(const char *PropertyName);
        virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString *Result) const;
        virtual int GetPropertyCount() {  return CryObject::GetPropertyCount() + 2; }
        virtual CryList *PropertyNames();
        */
}
#endif //VALIDATING



void CryStream::GetEleType(CryString &Result) const
{
    Result = "CryStream::ListNode";
}



void CryStream::SetMode(StreamMode NewMode)
{
    _Mode.ExtraData =0;
    _Mode.Mode = NewMode;
}

CryFunctionDefList *CryStream::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return CryContainer::GetFunctions(Type);
	// otherwise get any functions in subclasses
	CryFunctionDefList *l = CryContainer::GetFunctions();
    
    CryString s;
    s += "// Class CryStream;";
    s += "StreamMode GetMode() const;";
    s += "void SetMode(StreamMode NewMode);";
    s += "const char* ClassName() const;";
    s += "virtual const char *ChildClassName() const;";
    s += "virtual int Seek(int offset,int whence) const = 0;";
    s += "virtual int SeekToStart() const = 0;";
    s += "virtual int SeekFromStart(int Offset=0) const = 0;";
    s += "virtual int SeekFromCurrent(int Offset) const = 0;";
    s += "virtual int SeekFromEnd(int Offset=0) const = 0;";
    s += "virtual CryString *GetFunctions() const;";
    s += "virtual size_t Read(char *ToBuffer,size_t Size) const = 0;";
    s += "virtual size_t Write(const char *FromBuffer,size_t Size) = 0;";
    s += "virtual void CopyTo(CryObject &Dest) const;";
    s += "virtual void CopyToStream(CryStream &Dest) const;";
    s += "virtual bool CanDup() const;";
    s += "virtual CryObject *Dup() const = 0;";
    s += "virtual void SetTerminator(char Terminator_);";
    s += "virtual char GetTerminator() const;";
    s += "virtual size_t ReadTI(char *ToBuffer,size_t Size) const;";
    s += "virtual size_t WriteTI(const char *FromBuffer,size_t Size);";
    s += "virtual size_t ReadTI(CryStream *ToBuffer,size_t Size) const;";
    s += "virtual size_t WriteTI(CryStream *FromBuffer,size_t Size);";
    s += "virtual size_t ReadTE(char *ToBuffer,size_t Size) const ;";
    s += "virtual size_t WriteTE(const char *FromBuffer,size_t Size);";
    s += "virtual size_t ReadTE(CryStream *ToBuffer,size_t Size) const;";
    s += "virtual size_t WriteTE(CryStream *FromBuffer,size_t Size);";
    s += "virtual char *StreamedClass(char ClassNameBuffer[TMaxClassNameSize]) const;";
    s += "virtual size_t Read(CryStream *ToStream,size_t Size) const = 0;";
    s += "virtual size_t Write(const CryStream *FromStream,size_t Size) = 0;";
    s += "virtual size_t Read(CryStream *ToStream) const;";
    s += "virtual size_t Write(const CryStream *FromStream);";
    s += "virtual size_t WriteNStr(const char *Buffer);";
    s += "virtual size_t ReadNStr(char *Buffer,size_t MaxLength) const;";
    s += "virtual size_t WriteStr(const char *Buffer);";
    s += "virtual size_t ReadStr(char *Buffer,size_t MaxLength) const;";
    s += "virtual size_t Size() const = 0;";
    s += "virtual size_t Tell() const = 0;";
    s += "virtual bool Eof() const = 0;";
    s += "virtual bool Open(const char *Name,const char *Operation,bool ExceptOnError=true) = 0;";
    s += "virtual void Close(bool ExceptOnError=true) = 0;";
    s += "virtual bool IsOpen() const = 0;";
    s += "virtual void Flush() = 0;";
    s += "virtual bool IsA(const char *ClassName) const;";
    s += "virtual bool Event(CryObject::EObject EventNumber,CryObject::Context::IO &Context);";
    s += "virtual bool Event(EObject EventNumber,Context::UIO &Context);";
    s += "virtual int scanf(const char *format,...) const = 0;";
    s += "virtual size_t printf(const char *format,...) = 0;";
    s += "virtual bool SetProperty(const char *PropertyName,const char *PropertyValue);";
    s += "virtual bool HasProperty(const char *PropertyName) const;";
    s += "virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;";
    s += "virtual int GetPropertyCount() const;";
    s += "virtual CryList *PropertyNames() const;";
#ifdef VALIDATING

    s += "virtual bool Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));";
#endif

    l->LoadFromString(s,";");
    return l;
}

CryStream::~CryStream()
{}


void CryStream::CopyToStream(CryStream &Dest,CopyStyle Style) const
{
    if (!IsOpen())
        throw CryException(this,"Stream Not Open in CopyTo Source");
    if (!Dest.IsOpen())
        throw CryException(this,"Stream Not Open in CopyTo Dest");
    Dest.Terminator[0] = Terminator[0];   //copies contents of this to Dest    SeekFromStart(0);
    size_t OrigPosition = Tell();
    CryStream *s = (CryStream *) this;
    s->SeekFromStart(0);
    /*    // old working slow code
        for(unsigned int i=0;i<s->Size();i++)
        {
            char Buffer[2];
            s->Read(Buffer,1);
            Dest.Write(Buffer,1);
        }
        */
    unsigned long InBuffSize = Size();
    size_t OrigSize = InBuffSize;
    char *InBuffer = 0;
    char *OutBuffer = 0;
    unsigned long OutBuffSize = 16384;
    if (Style==NORMAL)
        OutBuffSize = 0;
    else
    {
        if (Style==ZIP)
        {
            if (Dest.IsA(TCryString))
                CryException(this,"Cannot compress into a CryString as there may be zeros in compressed data");
        }
        else
            if (Style==UNZIP)
            {
                if (IsA(TCryString))
                    CryException(this,"Cannot decompress from a CryString as there may be zeros in compressed data");
            }
            else
                throw CryException(this,"CopyToStream unknown copy style");
        while(OutBuffer==0)
        {
            OutBuffer = new char[OutBuffSize];
            if (OutBuffer==0)
                OutBuffSize /=2;
        }
    }
    while(InBuffer==0)
    {
        InBuffer = new char[InBuffSize];
        if (InBuffer==0)
            InBuffSize /=2;
    }
    // at this point we have appropriate buffers allocated
    CryStream *pDest = &Dest;

    switch(Style)
    {
    case NORMAL:
        while(OrigSize)
        {
            if (InBuffSize>OrigSize)
                InBuffSize = OrigSize;
            size_t rs = s->Read(InBuffer,InBuffSize);
            OrigSize -= rs;
            Dest.Write(InBuffer,rs);
        }
        delete [] InBuffer;
        s->SeekFromStart(OrigPosition);
        return;
    case ZIP:
        while(OrigSize)
        {
            if (InBuffSize>OrigSize)
                InBuffSize = OrigSize;
            size_t rs = s->Read(InBuffer,InBuffSize);
            OrigSize -= rs;

            int err = compress((Bytef *)OutBuffer,&OutBuffSize,(Bytef *)InBuffer,rs);
            if (err!= Z_OK)
            {
                delete [] InBuffer;
                delete [] OutBuffer;
                throw CryException(this,"ZipCopy compress Error %d",err);
            }
            if (pDest->Write(OutBuffer,OutBuffSize)!=OutBuffSize)
            {
                delete [] InBuffer;
                delete [] OutBuffer;
                throw CryException(this,"ZipCopy write not equal to write requested");
            }
        }
        delete [] InBuffer;
        delete [] OutBuffer;
        s->SeekFromStart(OrigPosition);
        return;
    case UNZIP:
        while(OrigSize)
        {
            if (InBuffSize>OrigSize)
                InBuffSize = OrigSize;
            size_t rs = s->Read(InBuffer,InBuffSize);
            OrigSize -= rs;
            OutBuffSize /=2;
            int err = uncompress((Bytef *)OutBuffer,&OutBuffSize,(Bytef *)InBuffer,rs);
            if (err!= Z_OK)
            {
                delete [] InBuffer;
                delete [] OutBuffer;
                throw CryException(this,"ZipCopy uncompress Error %d",err);
            }
            if (pDest->Write(OutBuffer,OutBuffSize)!=OutBuffSize)
            {
                delete [] InBuffer;
                delete [] OutBuffer;
                throw CryException(this,"ZipCopy write not equal to write requested");
            }
        }
        delete [] InBuffer;
        delete [] OutBuffer;
        s->SeekFromStart(OrigPosition);
        return;
    }
    return; // should never get here
}

void CryStream::CopyTo(CryObject &Dest) const
{
    if (Dest.IsA(TCryStream))
        CopyToStream(*(CryStream *)&Dest);
    else
        throw CryException(this,"Copying from stream to object that is not streamable");
}

CryPropertyList *CryStream::PropertyNames() const
{
	CryPropertyList *n = CryObject::PropertyNames();
	n->AddPropertyByName("Terminator",this);
	return n;
}

bool CryStream::HasProperty(const CryPropertyParser &PropertyName) const
{
    if (PropertyName=="Terminator")
        return true;
    return CryObject::HasProperty(PropertyName);
}

const char *CryStream::GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const
{
    if (PropertyName=="Terminator")
    {
        Result.printf("0x%02x",Terminator[0]);
        return Result;
    }
    return CryObject::GetProperty(PropertyName,Result);
}

bool CryStream::SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue)
{
    if (PropertyName=="Terminator")
    {
        CryString Value;
        Value = PropertyValue;
        char ch;

        if (Value.strstr("0x"))
            Value.scanf("%x",&ch);
        else
            Value.scanf("%d",&ch);
        SetTerminator(ch);
        return true;
    }
    if (CryObject::SetProperty(PropertyName,PropertyValue))
        return true;

    return false;
}
//
bool CryStream::Event(CryObject::EObject EventNumber,CryObject::Context::IO &Context)
{
    return CryObject::Event(EventNumber,Context);
}

// returns true if handled
bool CryStream::Event(EObject EventNumber,Context::UIO &Context)
{
    Context::UContext In = Context.StreamContext.In;  // In is parameters heading into this function, ie input parameters
    Context::UContext Out = Context.StreamContext.Out;// out is results from event
    switch(EventNumber)
    {
    case EFirst:
        return CryObject::Event(CryObject::EFirst,Context.ObjectContext);
    case ESeek:
        Seek(In.Seek.i1,In.Seek.i2);
        return true;
    case ESeekFromStart:
        SeekFromStart(In.SeekFromStart.Result);
        return true;
    case ESeekFromCurrent:
        SeekFromCurrent(In.SeekFromCurrent.Result);
        return true;
    case ESeekFromEnd:
        SeekFromEnd(In.SeekFromEnd.Result);
        return true;
    case ERead:
        Out.OutRead.Result = Read(In.InRead.Buffer,In.InRead.Size);
        return true;
    case EWrite:
        Out.OutWrite.Result = Write(In.InWrite.Buffer,In.InWrite.Size);
        return true;
    case ESetTerminator:
        SetTerminator(In.InSetTerminator.ch);
        return true;
    case EGetTerminator:
        Out.OutGetTerminator.ch = GetTerminator();
        return true;
    case EReadT:
        Out.OutReadT.Size = ReadTI(In.InReadT.Buffer,In.InReadT.Size);
        return true;
    case EWriteT:
        Out.OutWriteT.Size = WriteTI(In.InWriteT.Buffer,In.InWriteT.Size);
        return true;
    case EReadStream:
        Out.OutReadStream.Size = Read(In.InReadStream.Stream,In.InReadStream.Size);
        return true;
    case EWriteStream:
        Out.OutWriteStream.Size = Write(In.InWriteStream.Stream,In.InWriteStream.Size);
        return true;
    case EWriteNStr:
        Out.OutWriteStr.Size = WriteNStr(In.InWriteStr.Text);
        return true;
    case EReadNStr:
        Out.OutReadStr.Size = ReadNStr(In.InReadStr.Buffer,In.InReadStr.Size);
        return true;
    case EWriteStr:
        Out.OutWriteStr.Size = WriteStr(In.InWriteStr.Text);
        return true;
    case EReadStr:
        Out.OutReadStr.Size = ReadStr(In.InReadStr.Buffer,In.InReadStr.Size);
        return true;
    case ETell:
        Out.OutTell.Size = Tell();
        return true;
    case EEof:
        Out.OutEof.Result = Eof();
        return true;
    case EOpen:
        Out.OutOpen.Result = Open(In.InOpen.FileName,In.InOpen.OpenMode,In.InOpen.ExceptOnError);
        return true;
    case EClose:
        Close(In.InClose.Result);
        return true;
    case ELast:
        return CryObject::Event(CryObject::ELast,Context.ObjectContext);
    }
    if (EventNumber < EFirst)
    {
        CryObject::EObject coEventNumber = (CryObject::EObject ) EventNumber;
        return CryObject::Event(coEventNumber,Context.ObjectContext);
    }
    return false;
}

void CryStream::SetTag(int i) const
{
    CryStream *p = (CryStream *) this;
    p->Tag = i;
}

// will LoadFrom the Buffer with the name of the next class in the stream to LoadFrom
char *CryStream::StreamedClass(char ClassNameBuffer[TMaxClassNameSize]) const
{
    CryStream *p = (CryStream *) this;
    int CurrentLocation = p->Tell();
    p->ReadNStr(ClassNameBuffer,TMaxClassNameSize);
    p->SeekFromStart(CurrentLocation);
    return &ClassNameBuffer[0];
}





// read until terminator or Size (exclusive)
size_t CryStream::ReadTE(char *ToBuffer,size_t Size) const
{
    Size = ReadTI(ToBuffer,Size);
    if (Size>0)
    {
        Size--;
        if (ToBuffer[Size]==Terminator[0])
            SeekFromStart(Tell()-1);
        ToBuffer[Size] = '\0';
    }
    return Size;
}
// write until terminator or size (exculusive)
size_t CryStream::WriteTE(const char *FromBuffer,size_t Size)
{
    const char *Dest = (char *)memchr(FromBuffer, GetTerminator(), Size);
    if (Dest)
        Size = Dest - FromBuffer+1;
    if (Size)
        this->Write(FromBuffer,Size-1);
    return Size;
}
// write until terminator or size (Inclusive)
size_t CryStream::WriteTI(const char *FromBuffer,size_t Size)
{
    const char *Dest = (char *)memchr(FromBuffer, GetTerminator(), Size);
    if (Dest)
        Size = Dest - FromBuffer+1;
    if (Size)
        this->Write(FromBuffer,Size);
    return Size;
}

// read until terminator or Size (exclusive)
size_t CryStream::ReadTE(CryStream *ToBuffer,size_t Size) const
{
    char *Buffer = new char[Size+1];
    size_t s = ReadTE(Buffer,Size);
    ToBuffer->Write(Buffer,s);
    delete []Buffer;
    return s;
}

/// write until terminator or size (Exclusive)
/// returns the number of chars written
size_t CryStream::WriteTE(CryStream *FromBuffer,size_t Size)
{
    char buff[2];
    char t = FromBuffer->GetTerminator();
    unsigned int i=0;
    while(i<Size)
    {
        FromBuffer->Read(buff,1);
        if (buff[0]==t)
        {
            return i;
        }
        Write(buff,1);
        i++;
    }
    return i;
}

size_t CryStream::WriteStr(const char *StrBuffer)
{
    size_t s = strlen(StrBuffer);
    return Write(StrBuffer,s+1);    // include the terminator
}

size_t CryStream::WriteNStr(const char *StrBuffer)
{
    size_t s = strlen(StrBuffer);
    size_t c = Write((char *)&s,sizeof(s));
    return c+Write(StrBuffer,s);
}
size_t CryStream::ReadStr(char *Buffer,size_t MaxLength) const
{
    size_t o=0;
    while(MaxLength)
    {
        Read(Buffer,1);
        MaxLength--;
        o++;
        if (*Buffer=='\0')
            return o;
        Buffer++;
    }
    return o;
}
size_t CryStream::ReadNStr(char *Buffer,size_t MaxLength) const
{
    size_t s=0,o=0;
    if (Eof())
        throw CryException(this,"ReadNStr reading past eof");
    if (Read((char *)&s,sizeof(s))<sizeof(s))
        throw CryException(this,"ReadNStr Read Length error on length");
    if (MaxLength<s)
    {
        o = s - MaxLength-1;
        s = MaxLength-1;
    }
    size_t r = Read(Buffer,s);
    if (r!=s)
    {
        if (Eof())
            throw CryException(this,"ReadNStr reading past eof");
        else
            throw CryException(this,"ReadNStr Read length error");

    }
    Buffer[s] = '\0';
    if (o)
        SeekFromCurrent(o);
    return s;
}
// read until terminator or Size (inclusive)
size_t CryStream::ReadTI(char *ToBuffer,size_t Size) const
{
    char t = GetTerminator();
    size_t s =0;
    while(s<Size)
    {
        Read(ToBuffer+s,1);
        if (ToBuffer[s]==t)
        {
            return s;
        }
        s++;
    }
    return s;
}

// read until terminator or Size (inclusive)
size_t CryStream::ReadTI(CryStream *ToBuffer,size_t Size) const
{
    char *Buffer = new char[Size+1];
    size_t s = ReadTI(Buffer,Size);
    ToBuffer->Write(Buffer,s);
    delete []Buffer;
    return s;
}
// write until terminator or size (inclusive)
size_t CryStream::WriteTI(CryStream *FromBuffer,size_t Size)
{
    char *Buffer = new char[Size+1];
    Size = FromBuffer->ReadTI(Buffer,Size);
    this->Write(Buffer,Size);
    return Size;
}



CryStream::CryStream()
{
    Terminator[0] = Terminator[1] = '\0';
    _Mode.Mode = SText;
    Tag = 0;
}


bool CryStream::CanDup() const
{
    return IsOpen();
}

CryObject::StreamMode CryStream::GetMode() const
{
    return _Mode.Mode;
}

void CryStream::SetTerminator(char Terminator_)
{
    Terminator[0] = Terminator_;
    Terminator[1] = '\0';
}
char CryStream::GetTerminator() const
{
    return Terminator[0];
}

size_t CryStream::Read(CryStream *ToStream) const
{
    return Read(ToStream,Size());
}

size_t CryStream::Write(const CryStream *FromStream)
{
    return Write(FromStream,FromStream->Size());
}

int CryStream::fgetc() const
{
    char r[2];
    Read(r,1);
    return r[0];
}

void CryStream::SetItemOwnerShip(Iterator *I,bool Owned)
{} // ignored we always own, as it's a copy
bool CryStream::GetItemOwnerShip(const Iterator *I) const
{
    return true;
}  // always
size_t CryStream::GetItemSize(Iterator *I) const
{
    return 1;
}
bool CryStream::IsCryObject(const Iterator *I) const
{
    return false;
}

int CryStream::GetPropertyCount() const
{
    return CryObject::GetPropertyCount() + 2;
}

int CryStream::SeekToStart() const
{
    return SeekFromStart(0);
}


CryStream::StreamIterator::StreamIterator(const CryContainer *Container) : Iterator(Container)
{}
CryObject *CryStream::StreamIterator::Dup() const
{
StreamIterator *it = (StreamIterator *)GetOrigContainer()->_CreateIterator();
	it->Offset = Offset;
	return it;
}

CryObject *CryStream::Add(CryObject *Item)    // returns Item
{
    CryString s;
    Item->SaveTo(s);
    Write(s.AsPChar(),s.GetLength());
    return Item;
}
CryObject *CryStream::AddOwned(CryObject *Item)   // gives ownership to list
{
    delete Add(Item); // So we add the text equivilent and then delete it
    return 0;
}

bool CryStream::LoadAsText(Iterator *I,CryString &FromStream)
{
    //    StreamIterator *i = (StreamIterator *)I;
    char ToBuff[2];
    if (FromStream.Write(ToBuff,1))
    {
        if (Read(ToBuff,1)==1)
            return true;
    }
    return false;

}
/*! this function does not change the contents of the stream so it is deamed const, even though internal varialbes can be modified
*/
bool CryStream::SaveAsText(Iterator *I,CryString &ToStream) const
{
    StreamIterator *i = (StreamIterator *)I;
    char FromBuff[2];
    if (Size() > i->Offset)
    {
        CryStream *t = (CryStream *)this;
        t->Write(FromBuff,1);
        if (ToStream.Read(FromBuff,1)==1)
            return true;
    }
    return false;
}
