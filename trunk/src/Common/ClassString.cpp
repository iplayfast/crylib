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
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#ifdef __BORLANDC__
#include <io.h>
#endif

#include "ClassProperty.h"
#include "ClassException.h"
#include "ClassFunction.h"
#include "ClassString.h"

using namespace Crystal;
//-------------------------------------------------------------------
// String
//-------------------------------------------------------------------

#ifdef VALIDATING
bool String::Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail))
{
    bool fail = false;
    // don't do any substream tests since crystring acts differently then other streams. (size doesn't include the asciiz)
    if (Object::Test(Verbose,Object,CallBack))	// if failed
        return true;
    {
        char Result[200];
        sprintf(Result,"\nCryObject Testing:\nObject of ClassName %s,ChildClassName %s",
                Object.ClassName(),Object.ChildClassName());
        if (!CallBack(Verbose,Result,fail))
            return false;
    }
    if (Object.IsA(CString))
    {

        /*
            CryString();
            CryString(CryString &a);
            virtual ~CryString() {}
        //    CryString(const char *Value);
            CryString(const char *FormatStr,...);
            virtual void Clear() { SetRaw(0,0); SetLength(0); SetPosition(0); }
        	 
        	 // 0 == all, 1.. from start, -1..-N from end
            virtual void Clear(int amount) { CryMemStream::Clear(amount); SetRaw(GetLength(),'\0');}
            
            virtual size_t Length() const { return Size(); }
            virtual void SetLength(int i) { CryMemStream::SetLength(i); SetRaw(i,'\0'); }
            virtual CryList *PropertyNames() const;
            virtual int GetPropertyCount() const { return 1; }// strings only have a value (even if they are named!)
         
            const char* ClassName() const;
            virtual const char *ChildClassName() const;
            virtual operator const char *() const { return (const char *)GetRaw(); }
			CryString operator &() { return *this; }
            char operator [](unsigned int Offset) const;
            virtual CryObject *Dup() const{ CryString *Result = new CryString(); CopyTo(*Result); return Result; } // creates a duplicate of this object
            virtual size_t Read(CryStream *ToStream,size_t Size) const;
            virtual size_t Write(const CryStream *FromStream,size_t Size);
            virtual size_t Read(CryStream *ToStream) const { return Read(ToStream,Size()); }
            virtual size_t Write(const CryStream *FromStream) { return Write(FromStream,FromStream->Size()); }
            virtual size_t Read(char *ToBuffer,size_t t) const;
            virtual size_t Write(const char *FromBuffer,size_t t);
            virtual size_t WriteNStr(const char *Buffer);
            virtual size_t ReadNStr(char *Buffer,size_t MaxLength) const;
            
            const char *strstr(CryString &s) const { return  ::strstr((const char *)GetRaw(),(const char *)s.GetRaw()); }
            const char *strstr(const char *s) const { return (const char *) ::strstr((const char *)GetRaw(),s); }
            int strncmp(CryString &s,size_t n) const { size_t l = s.Length(); if (n<l) l = n; if (l<Length()) l = Length(); return ::strncmp((const char *)GetRaw(),(const char *)s.GetRaw(),l); }
            int strncmp(const char *s,size_t n) const { return ::strncmp((const char *)GetRaw(),s,n); }
            int strcmp(CryString &s) const {size_t l = s.Length(); if (l<Length()) l = Length(); return ::strncmp((const char *)GetRaw(),(const char *)s.GetRaw(),l); }
            int strcmp(const char *s) const { return ::strcmp((const char *)GetRaw(),s); }
            const char *strcat(CryString &s) { SeekFromEnd(0); Write((const char *)s.GetRaw(),s.Length()); SeekFromStart(0); return (const char *)GetRaw(); }
            const char *strcat(const char *s) { SeekFromEnd(0); Write((const char *)s,::strlen(s)+1); SeekFromStart(0); return (const char *)GetRaw(); }
            const char *FormatHex(cbyte *data,size_t length);
            EmptyObject *GetAddress() { return this; }
            bool operator ==(const char *s) const ;
            bool operator !=(const char *s) const;
            CryString & operator +=(const char *s);
            virtual bool IsOpen() const { return true;}
            virtual int scanf(const char *format,...) const;
            virtual int scanf(const char *format,...) const;
            virtual size_t printf(const char *format,...);
            virtual size_t printf(const char *format,...);
            virtual CryString &TrimLeft();
            virtual CryString &TrimRight();
            virtual CryString &Trim();
            virtual CryString &Replace(const char *_Search,const char *_Replace);
            int SeekTextFromStart(const char *SearchString) const;
            int SeekTextFromCurrent(const char *SearchString) const;
			virtual void Close(bool ExceptOnError=true) { Clear(0);};
            virtual bool Convert(int *i) const;
            virtual bool Convert(char *b,int len) const;
        //    virtual bool Convert(double *d) const;
            virtual bool IsA(const char *ClassName) const; // can the object map to a ClassName
            CryString & operator =(const char *text);
            CryString & operator = (const CryString *v);
            CryString & operator = (const CryString &v) { return *this = v.AsPChar(); }
            void SetValue(const char *_Value);
            virtual size_t Size() const;
            virtual bool HasProperty(const char *PropertyName) const;
         
            virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
        #ifdef VALIDATING
        virtual bool Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));
        #endif
        */
        String a;
        const char *ErrorStr;
        char *emptyconst = "CryString a; make empty string test";
        char *nonemptyconst = "CryString a(\"test\") contructor";
        char *copyconst = "copy constructor";
        char *cleartest = "Clear test, assignment of const char,Length";
        {
            String a;
            ErrorStr = emptyconst;
            if (a.Length()==0 && (a==""))
            {
                if ((Verbose) && (!CallBack(Verbose,ErrorStr,fail)))
                    return false;
            }
            else
                fail = true;
            if (a!="")
                fail = true;
            else
			{
                if ((Verbose) && (!CallBack(Verbose,ErrorStr,fail)))
                    return false;
            }
        }
        if (!fail)
        {
            {
                String s;
                s = "AaBbCcDd";
                cout << s.AsPChar() << endl;
                ErrorStr = "Sort case sensitive";
                s.Sort(0);	// case sensitive case
                fail = s != "ABCDabcd";
                if (fail)
                {
                    if ((!CallBack(Verbose,ErrorStr,fail)))
                        return false;
                }
                ErrorStr = "Sort case insensitive";
                s.Sort(1);	// case Insensitive
                fail = s != "AaBbCcDd";
                if (fail)
                {
                    if ((!CallBack(Verbose,ErrorStr,fail)))
                        return false;
                }
                ErrorStr = "revSort case sensitive";
                s.Sort(2);
                fail = s != "dcbaDCBA";
                if (fail)
                {
                    if ((!CallBack(Verbose,ErrorStr,fail)))
                        return false;
                }
                ErrorStr = "revSort case insensitive";
				s.Sort(3);
                fail = s != "dDcCbBaA";
                if (fail)
                {
                    if ((!CallBack(Verbose,ErrorStr,fail)))
                        return false;
                }
            }
        }

        if (!fail)
        {
            String Test("this is a testing");
            Test.Replace("is","");  // "th  a testing"
            Test.Replace("ing","ed");       // "th  a tested"
            Test.Replace("e","a");          // "th  a tastad"
            fail = Test != "th  a tastad";
            ErrorStr = "Replace function";
            if (!fail)
            {
                if ((Verbose) && (!CallBack(Verbose,ErrorStr,fail)))
                    return false;
            }
            else
                if ((!CallBack(Verbose,ErrorStr,fail)))
                    return false;
        }
        if (!fail)
        {
            String a("test");
            ;
            ErrorStr = nonemptyconst;
            if ((a.Length()==4) && (a=="test"))
            {
                if ((Verbose) && (!CallBack(Verbose,ErrorStr,fail)))
                    return false;
			}
            else
                fail = true;
            if (a!="test")
                fail = true;
            else
            {
                if ((Verbose) && (!CallBack(Verbose,ErrorStr,fail)))
                    return false;
            }
        }
        if (!fail)
        {
            String a("test");
            String b(a);
            ErrorStr = copyconst;
            if ((b.Length()==4) && (b=="test"))
            {
                if ((Verbose) && (!CallBack(Verbose,ErrorStr,fail)))
                    return false;
            }
            else
                fail = true;
            if (b!="test")
                fail = true;
            else
            {
                if ((Verbose) && (!CallBack(Verbose,ErrorStr,fail)))
                    return false;
            }
        }
        if (!fail)
        {
            ErrorStr = cleartest;
            String a("1234567");
            // 0 == all, 1.. from start, -1..-N from end
			a.Clear(0);
            if (a!="")
                fail = true;
            else
            {
                a = "1234567";		// assignment of const char tested here
                a.Clear(1);	// clear from 1
                if ((a!="234567") || (a.Length()!=6))
                    fail = true;
                else
                {
                    a = "1234567";
                    a.Clear(-1);
                    if (a!="123456")
                        fail = true;
                    else
                    {
                        a.Clear();
                        if (a!="")
                            fail = true;
                    }
                }
            }
            if ((Verbose) && (!CallBack(Verbose,ErrorStr,fail)))
                return false;
        }
        {
            String a("test");
            //		CryList *l = a.PropertyNames();

            // todo, carry on
        }
        if (fail && (!CallBack(Verbose,ErrorStr,fail)))
            return false;
    }
    return true;
}
#endif
size_t String::WriteNStr(const char *StrBuffer)
{
	int l = Length();
	printf("%d %s",strlen(StrBuffer),StrBuffer);
	return Length() - l + 1;
}
bool String::SetProperty(const PropertyParser &PropertyName,const char *PropertyValue)
{
	if (PropertyName=="Value")
	{
		*this = PropertyValue;
		return true;
	}
	return MemStream::SetProperty(PropertyName,PropertyValue);
}
String::operator const char *() const
{
	return (const char *)GetRaw();
}
// creates a hash value of [a..z,A..Z,0..9,_]
int String::HashValue()const
{
const char *c = (const char *)GetRaw();
int e = Length();
int Hash = 0;
	for(int i=0;i<e;i++)
	{
	Hash *=63;
		if (*c>='a' && *c<='z') {
			Hash += *c - 'a';
		}
		else
		{
			if (*c>='A' && *c<='Z') {
				Hash += *c - 'A' +  26;
			}
			else
			{
				if (*c>='0' && *c<='9') {
					Hash += *c - '0' + 52;
				}
				else if (*c=='_') {
						Hash += 63;
					 }
			}
		}
		c++;
	}
	return Hash;
}
size_t String::Read(Stream *ToStream) const
{
    return Read(ToStream,Size());
}
size_t String::Write(const Stream *FromStream)
{
    return Write(FromStream,FromStream->Size());
}
int String::Pos(const char *s,size_t StartIndex)
{
    if (GetLength()<StartIndex)
        return -1;
    const char *c = (const char *) ::strstr(((const char *)GetRaw())+StartIndex,s);
    if (c)
        return c - AsPChar();
    else
        return -1;
}
int String::Pos(const char *s) const
{
    const char *c = strstr(s);
    if (c)
        return c - AsPChar();
    else
        return -1;
}
/// returns NULL if not found
const char *String::strstr(String &s) const
{
    return  ::strstr((const char *)GetRaw(),(const char *)s.GetRaw());
}
const char *String::strstr(const char *s) const
{
    return (const char *) ::strstr((const char *)GetRaw(),s);
}
int String::strncmp(String &s,size_t n) const
{
    size_t l = s.Length();
    if (n<l)
        l = n;
    if (l<Length())
        l = Length();
    return ::strncmp((const char *)GetRaw(),(const char *)s.GetRaw(),l);
}
int String::strncmp(const char *s,size_t n) const
{
    return ::strncmp((const char *)GetRaw(),s,n);
}
int String::strcmp(String &s) const
{
    size_t l = s.Length();
    if (l<Length())
        l = Length();
    return ::strncmp((const char *)GetRaw(),(const char *)s.GetRaw(),l);
}
int String::strcmp(const char *s) const
{
    return ::strcmp((const char *)GetRaw(),s);
}
const char *String::strcat(String &s)
{
    Pos2Asciiz();
    Write((const char *)s.GetRaw(),s.Length());
    SeekFromStart(0);
    return (const char *)GetRaw();
}
const String *String::GetAddress()
{
    return this;
}
String &String::ExclusiveAppend(const char *_Append)
{
    return ReplaceAppend(_Append,"",_Append);
}
void String::Close(bool ExceptOnError)
{
    Clear(0);
};
String & String::operator = (const String &v)
{
    return *this = v.AsPChar();
}

// case sensitive
int CharCompare(const void *p1,const void *p2)
{
    return (*(char *)p1) - (*(char *)p2);
}
// case insenitive (unless they are equal
int ChariCompare(const void *p1,const void *p2)
{
    int r = toupper((*(char *)p1)) - toupper((*(char *)p2));
    if (r==0)
        r = (*(char *)p1) - (*(char *)p2);
    return r;
}
int rCharCompare(const void *p1,const void *p2)
{
    return (*(char *)p2) - (*(char *)p1);
}
int rChariCompare(const void *p1,const void *p2)
{
    int r = toupper((*(char *)p2)) - toupper((*(char *)p1));
    if (r==0)
        r = (*(char *)p2) - (*(char *)p1);
    return r;
}
int RandomCompare(const void *p1,const void *p2)
{
    return rand() - (RAND_MAX / 2);
}
void String::Sort(int CompareType)
{
    char *ch = (char *)GetRaw();
    int l = Length();
    switch(CompareType)
    {
    case 0:	// Normal case sensitive
        qsort(ch,l,1,::CharCompare);
        break;
    case 1:	// Normal case Insensitive
        qsort(ch,l,1,::ChariCompare);
        break;
    case 2:	// Reverse case sensitive
        qsort(ch,l,1,::rCharCompare);
        break;
    case 3:	// Reverse case sensitive
        qsort(ch,l,1,::rChariCompare);
        break;
    default:
        qsort(ch,l,1,::RandomCompare);
        break;
    }
    return;
}
int String::CompareLogical(int CompareType,const Object *Test) const
{
    if ((CompareType>1) || (CompareType<0))
        throw Exception("Unexpected Value for Sorting CryFunctionDef");

    if (Test->IsA(CString))
    {
        String *t = (String *)Test;
        switch(CompareType)
        {
        case 0:
			return ::strcmp(t->AsPChar(),this->AsPChar());
		case 1:
			return ::strcasecmp(t->AsPChar(),this->AsPChar());
        }
    }
    return 0;
}
bool String::LessThen(int CompareType,const Object *Test) const
{
	int i = CompareLogical(CompareType,Test);
	return (i<0);
}
bool String::GreaterThen(int CompareType,const Object *Test)const
{
	int i = CompareLogical(CompareType,Test);
	return (i>0);
}
bool String::EqualTo(int CompareType,const Object *Test)const
{
    int i = CompareLogical(CompareType,Test);
    return (i==0);
}

bool String::IsOpen() const
{
    return true;
}
// 0 == all, 1.. from start, -1..-N from end
void String::Clear(int amount)
{
    MemStream::Clear(amount);
    SetRaw(Size(),'\0');
}

size_t String::Length() const
{
    return Size();
}

// SetLength set's the position of the asciiz, ie 0 = ""
void String::SetLength(int i)
{
    MemStream::SetLength(i+1);
    SetRaw(i,'\0');
}
int String::GetPropertyCount() const
{
    return 1;
}// strings only have a value (even if they are named!)
String &String::Delete(int start,int amount)
{
	if (start<0)
    	start += GetLength() - 2;
	MemStream::MemDelete(start,amount);
    LookLikeString();
    return *this;
}
/// Return a non-const reference to the ith character -ith means from the end back
char& String::operator[](int i)
{
int l = GetLength();
MemStream *m = (MemStream *)this;
	if (i>l)
		throw Exception("Reference beyond end of string");
	if (i>0)
		return  m->operator [](i);
         //(*m)[i];
		
	i = l + i - 2;
	if (i<0)
		throw Exception("Reference before beginning of string");
	return m->operator [](i);
        //(*m)[i];
//	return CryMemStream::operator[i];
}

String& String::simplify()
{
    unsigned int e = GetLength();
    if (e)
    {
        unsigned int s=0;
        unsigned int d=0;
        String &str = *this;
        while(s<e && isspace((char)str[s]))
            s++;
        while(1)
        {
            while(s<e && !isspace((char)str[s]))
                str[d++]=str[s++];
            while(s<e && isspace((char)str[s]))
                s++;
            if(s>=e)
                break;
            str[d++]=' ';
        }
        SetLength(d);

    }
    return *this;
}

size_t String::ReadNStr(char *Buffer,size_t MaxLength) const
{
    int n;
    //    int l = 0;
    char *BufferStart = Buffer;
    bool foundn = false;
    scanf("%d ",&n);
    if ((unsigned int) n>MaxLength)
        n = MaxLength;

    while(n)
    {
        if (Read(Buffer,1)!=1)
            n = 0;
        if (!foundn)
        {
            if (*Buffer==' ')
                foundn = true;
        }
        else
        {
            Buffer++;
            n--;
        }
    }

    *Buffer = '\0';
    return Buffer - BufferStart;
}

String &String::ReplaceAppend(const char *_Search,const char *_Replace,const char *_Append)
{
    Replace(_Search,_Replace);
    if (_Append!=0)
        strcat(_Append);
    return *this;
}

const char *String::strcat(const char *s)
{
    int NewSize = Size() + ::strlen(s) + 1;
    CheckMem(NewSize);
    char *b = GetBuffer();
    ::strcat(b,s);
    MemStream::SetLength(NewSize);
    //LookLikeString();	// not needed
    return (const char *)GetRaw();
}
// returns the offset from start of the string or -1 if not found
int String::SeekTextFromStart(const char *SearchString) const
{
    SeekFromStart(0);
    return SeekTextFromCurrent(SearchString);
}
// Returns the offset from the start of the string or -1 if not found
int String::SeekTextFromCurrent(const char *SearchString) const
{
    const char *p,*e,*t;
    const char *b = (const char *)GetRaw();
    e = b + Length();
    b += GetPosition();
    p = SearchString;
    if (*p=='\0')
        return GetPosition();
    while(b <= e)
    {
        if (*b==*p)
        {
            t = b;
            while (*b==*p)
            {
                b++;
                p++;
                if (*p=='\0')	// end of test string
                {
                    SeekFromStart(b - (const char *)GetRaw()-strlen(SearchString));
                    return GetPosition();
                }
            }
            b = t;	// no match
            p = SearchString;
        }
        b++;
    }
    return -1;	// not found
}


String &String::Insert(int Index,const char *Text)
{
    String Temp;
    Temp = *this;
    Temp.Delete(0,Index);
    this->Delete(Index,GetLength());
    *this += Text;
    *this += Temp;
    return *this;
}
String &String::Replace(const char *_Search,const char *_Replace)
{
    int j,i = SeekTextFromStart(_Search);
    if (i==-1)
        return *this;	// not found
    if (_Replace==0)
        _Replace = "";
    String Build = *this;
    int l = strlen(_Search);
    Build.Delete(i,GetLength());
    Build += _Replace;
    i+=l;
    while(1)
    {
        SeekFromStart(i);
        j = SeekTextFromCurrent(_Search);
        if (j==-1)
        {         // not found
            const char *r = (const char *)GetRaw();
            r = r + i;
            Build.strcat(r);
            *this = Build;
            return *this;
        }
        else
        {
            const char *r = (const char *)GetRaw();
            char Buff[2];
            Buff[1] = '\0';
            r = r + i;
            while(i<j)
            {
                Buff[0] = *r;
                r++;
                Build += Buff;
                i++;
            }
            Build.strcat(_Replace);
            i = j+l;
        }
    }
}

int String::scanf(const char *format,...) const
{
    //    SeekToStart();
    va_list argptr;
    va_start(argptr,format);
    return MemStream::vsscanf(format,argptr);
}

size_t String::printf(const char *format,...)
{ // this is normally a string function so we will assume that this class is being used as a string
    //    Clear();
    va_list argptr;
    va_start(argptr,format);
    return vsprintf(format,argptr);
}
int String::Seek(int offset,int whence) const
{
    int l = MemStream::Seek(offset,whence);
    /*if (whence==SEEK_END && l)
    {
        l--;
        return CryMemStream::Seek(l,SEEK_SET);  // back 1 for ascizz
    }*/
    return l;
}

String::String()
{
	SetIsContainer(false);
    SetTerminator('\0');
    SetLength(0);
    //    Clear();
}
FunctionDefList *String::GetFunctions(const char *Type) const
{
// if a type has been defined and it's not this class, check subclasses for it
	if (Type && !IsA(Type))
	   return MemStream::GetFunctions(Type);
	// otherwise get any functions in subclasses
	FunctionDefList *l = MemStream::GetFunctions();
    String s;
    s += "// Class CryString;";
    s += "virtual void Clear();";
    s += "virtual CryString *GetFunctions() const;";
    s += "virtual void Clear(int amount);";
    s += "virtual size_t Length() const;";
    s += "virtual void SetLength(int i);";
    s += "virtual CryList *PropertyNames() const;";
    s += "virtual int GetPropertyCount() const;";
    s += "CryString &Delete(unsigned int start,int amount);";
    s += "const char* ClassName() const;";
    s += "virtual const char *ChildClassName() const;";
    s += "virtual operator const char *() const;";
    s += "CryString operator &();";
    s += "CryString& simplify();";
    s += "virtual CryObject *Dup() const;";
    s += "virtual size_t Read(CryStream *ToStream,size_t Size) const;";
    s += "virtual size_t Write(const CryStream *FromStream,size_t Size);";
    s += "virtual size_t Read(CryStream *ToStream) const;";
    s += "virtual size_t Write(const CryStream *FromStream);";
    s += "virtual size_t Read(char *ToBuffer,size_t t) const;";
    s += "virtual size_t Write(const char *FromBuffer,size_t t);";
    s += "virtual size_t WriteNStr(const char *Buffer);";
    s += "virtual size_t ReadNStr(char *Buffer,size_t MaxLength) const;";
    s += "const char *strstr(CryString &s) const;";
    s += "const char *strstr(const char *s) const;";
    s += "int strncmp(CryString &s,size_t n) const;";
    s += "int strncmp(const char *s,size_t n) const;";
    s += "int strcmp(CryString &s) const;";
    s += "int strcmp(const char *s) const;";
    s += "const char *strcat(CryString &s);";
    s += "const char *strcat(const char *s);";
    s += "const char *FormatHex(cbyte *data,size_t length);";
    s += "EmptyObject *GetAddress();";
    s += "bool operator ==(const char *s) const;";
    s += "bool operator !=(const char *s) const;";
    s += "CryString & operator +=(const char *s);";
    s += "virtual bool IsOpen() const;";
    s += "virtual int scanf(const char *format,...) const;";
    s += "virtual size_t printf(const char *format,...);";
    s += "virtual CryString &Insert(int Index,const char *Text);";
    s += "virtual CryString &TrimLeft();";
    s += "virtual CryString &TrimRight();";
    s += "virtual CryString &Trim();";
    s += "virtual CryString &Replace(const char *_Search,const char *_Replace);";
    s += "virtual CryString &ReplaceAppend(const char *_Search,const char *_Replace,const char *_Append=0);";
    s += "int SeekTextFromStart(const char *SearchString) const;";
    s += "int SeekTextFromCurrent(const char *SearchString) const;";
    s += "virtual void Close(bool ExceptOnError=true);";
    s += "virtual bool Convert(int *i) const;";
    s += "virtual bool Convert(char *b,int len) const;";
    s += "virtual bool IsA(const char *ClassName) const;";
    s += "CryString & operator =(const char *text);";
    s += "CryString & operator = (const CryString *v);";
    s += "CryString & operator = (const CryString &v);";
    s += "void SetValue(const char * _Value);";
    s += "virtual size_t Size() const;";
    s += "virtual bool HasProperty(const char *PropertyName) const;";
    s += "virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;";
    s += "CryList *ListFromString(const char *Separator) const;";
    s += "CryList *LoadListFromString(const char *Separator,CryList *ListToLoad) const;";
    s += "virtual CryString &Insert(int Index,const char *Text);";
#ifdef VALIDATING

    s += "virtual bool Test(bool Verbose,CryObject &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));";
#endif

    l->LoadFromString(s,";");
    return l;
}

/*CryString::CryString(const char *Value)
{
    size_t len = strlen(Value);
    CryMemStream::Write(Value,len+1);
    SetPosition(len);
    SetTerminator('\0');
} */
String::String(const String &a)
{
	const char *ch = a.AsPChar();
	*this = ch;
}
String::String(const String *a)
{
    const char *ch = a->AsPChar();
    *this = ch;
}
String::String(const char *FormatStr,...)
{
    LookLikeString();
    SetTerminator('\0');
    va_list argptr;
	if (FormatStr)
{
    va_start(argptr,FormatStr);
    vsprintf(FormatStr,argptr);
}
    //SetPosition(len);
    SeekToStart();
    const char *c = (const char *)GetRaw();
    SetLength(strlen(c));
}

/*char CryString::operator[](unsigned int Offset) const
{
    if ((Offset>Length()))
        throw CryException(this,"out of range error");
    return GetRaw()[Offset];
 
} */

CryPropertyList *String::PropertyNames() const
{   // bypass stream property names because we aren't interested inthe Terminator
	CryPropertyList *n = Object::PropertyNames();
	n->AddPropertyByName("Value",this);
    return n;
}
bool String::HasProperty(const PropertyParser &PropertyName) const
{
    return ::strcmp(PropertyName,"Value")==0 ||MemStream::HasProperty(PropertyName);
}

const char *String::GetProperty(const PropertyParser &PropertyName,String &Result) const
{
    if (PropertyName=="Value")
    {
        return AsPChar();
    }
    return MemStream::GetProperty(PropertyName,Result);

}

const char *String::FormatHex(cbyte *data,size_t length)
{
    char buff[10];
    Clear();
    for(unsigned int i=0;i<length;i++)
    {
        sprintf(buff,"0x%02x",*data);
        data++;
        strcat(buff);
    }
    return AsPChar();
}

size_t String::Size() const
{
    /*const char *s = *this;
    int l = strlen(s);
        if (l < DataLength)
            return l;
        else*/
    int l =  MemStream::Size()-1;  // memstream includes the asciiz
    if (l<0)
        l = 0;
    return l;
}

bool String::Convert(int *i) const
{
    int j = sscanf(AsPChar(),"%d",i);
    return !((j==EOF) || (j==0));
}

bool String::Convert(char *b,int len) const
{
    strncpy(b,AsPChar(),len);
    return true;
}

String & String::operator =(const String *text)
{
    if (text==0)
        *this = "";
    else
        *this =(const char *) text->GetRaw();
    return *this;
}

String & String::operator =(const char *text)
{
    const char *thisText = GetBuffer();
    if (thisText != text)
    {
        MemStream::Clear();
        GetBuffer()[0] = '\0';
        if (text)
            strcat(text);
    }
    return *this;
}
String & String::operator+=(const char *text)
{
	if (text)
		strcat(text);
	return *this;
}

void String::SetValue(const char *_Value)
{
    *this = _Value;
}

String &String::Rev()
{
    char *ch1 = (char *)GetRaw();
    char *ch2 = ch1 + Length()-1;
    while(ch2>ch1)
    {
        char ch = *ch2;
        *ch2 = *ch1;
        *ch1 = ch;
        ch2--;
        ch1++;
    }
    return *this;
}

String &String::TrimLeft()
{
    const char *orgch,*ch;
    SetPosition(0);
    orgch = ch = (const char *)GetRaw();
    while((*ch) && isspace(*ch))
        ch++;
    if (ch!= orgch)
        MemStream::Clear(ch - orgch);
    return *this;
}
String &String::TrimRight()
{
    int l = Length();
    const char *orgch,*ch;
    SetPosition(0);
    if (l)
    {
        orgch = ch = (const char *)GetRaw();
        ch += (l-1);
        while((ch > orgch) && isspace(*ch))
            ch--;
        l = -l + (ch - orgch)+1;
        if (l)
            Clear(l);
    }
    return *this;
}
String &String::Trim()
{
    TrimRight();
    return TrimLeft();
}
String &String::ToUpper()
{
    int l = Length();
    char *ch;
    SetPosition(0);
    if (l)
    {
        ch = (char *)GetRaw();
        while(*ch)
        {
            if ((*ch>='a') && (*ch<='z'))
                *ch = *ch - 'a' + 'A';
            ch++;
        };
    }
    return *this;
}
String &String::ToLower()
{
    int l = Length();
    char *ch;
    SetPosition(0);
    if (l)
    {
        ch = (char *)GetRaw();
        while(*ch)
        {
            if ((*ch>='A') && (*ch<='Z'))
                *ch = *ch - 'A' + 'a';
            ch++;
        };
    }
    return *this;
}

bool String::operator ==(const char *s) const
{
    int i = Size();
    int j = strlen(s);
    if (i!=j)
        return false;
    i = ::strncmp((const char *)GetRaw(),s,i);
    return (i==0);
}
bool String::operator !=(const char *s) const
{
    int i = Size();
    int j = strlen(s);
    if (i!=j)
        return true;
    i = ::strncmp((const char *)GetRaw(),s,i);

    return (i!=0);
}


size_t String::Read(Stream *ToStream,size_t Size) const
{
    return MemStream::Read(ToStream,Size);
}

size_t String::Write(const Stream *FromStream,size_t Size)
{
    size_t p = GetPosition();
    if (p && (p >=GetLength()))
        Pos2Asciiz();
    size_t s = MemStream::Write(FromStream,Size);
    LookLikeString();
    return s;
}

size_t String::Read(char *ToBuffer,size_t t) const
{
    return MemStream::Read(ToBuffer,t);
}
void String::LookLikeString()
{
    if ((MemStream::GetLength()==0) || GetBuffer()[MemStream::GetLength()]!='\0')
        MemStream::LookLikeString();
    SetLength(strlen(*this));
}


size_t String::Write(const char *FromBuffer,size_t t)
{
    if (t==0)
        return 0;
    if (FromBuffer[t-1]=='\0')
        t--;
    size_t s = MemStream::Write(FromBuffer,t);
    //        CryMemStream::Write("",1);
    //    SetRaw(s+p,'\0');
    size_t p = GetPosition();
    LookLikeString();
    SetPosition(p);
    return s;
}
/*! returns a list of CryStrings made up from String separated by Separater (eg convert text with \n to list of strings)
The CryStrings do not contain the separator
*/
List *String::ListFromString(const char *Separator) const
{
    List *l = new List();
    return LoadListFromString(Separator,l);
}
List *String::LoadListFromString(const char *Separator,List *ListToLoad) const
{
	const char *MyString = this->AsPChar();
	List *l = ListToLoad;
	int seplen = strlen(Separator);
    while(strlen(MyString))
    {
        String *s = new String();
		const char *ch = ::strstr(MyString,Separator);
		if (ch==NULL)
		{
			*s = MyString;
			l->AddOwned(s);
			return l;
		}
		s->Write(MyString,ch - MyString);
		size_t size;
		if ((size = s->Size())>0)
		{
			const char *ch = s->strstr("//");
			const char *pch = s->AsPChar();
			if((ch==0) || (pch[size-1] !='/')) // if s is not empty and doesn't have a / at the end, then we are done with this one
			{
				l->AddOwned(s);
			}
			else
			{
				delete(s);
			}
		}
		else
			delete(s);
		MyString = ch + seplen;
	}
	return l;
}

