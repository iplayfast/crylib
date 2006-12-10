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

#include "ClassMemStream.h"
namespace Crystal
{
using namespace std;
#ifndef CCryString
#define CCryString  "CryString"
#ifdef __BORLANDC__
#define strcasecmp stricmp
#endif
/// string handling as though it were a stream
class CryString : public CryMemStream
{
protected:
    void LookLikeString();
public:
    StdFunctions(CryString,CryMemStream);
	CryString();
	CryString(const CryString &a);
	CryString(const CryString *a);
	virtual ~CryString()
	{}
	//    CryString(const char *Value);
	CryString(const char *FormatStr,...);
	virtual void Clear()
	{
        SetRaw(0,0);
        SetLength(0);
        SetPosition(0);
        CryMemStream::Clear();
    }
    virtual CryFunctionDefList *GetFunctions(const char *Type=0) const;

    // 0 == all, 1.. from start, -1..-N from end
    virtual void Clear(int amount);
    virtual size_t Length() const;

    // SetLength set's the position of the asciiz, ie 0 = ""
    virtual void SetLength(int i);
    virtual int Seek(int offset,int whence) const;
	virtual CryPropertyList* PropertyNames() const;
virtual int GetPropertyCount() const;
    CryString &Delete(int start,int amount);
    //    const char* ClassName() const;
    //    virtual const char *ChildClassName() const;
    virtual operator const char *() const;
    /// Simplify whitespace in string
    CryString& simplify();

    //    virtual CryObject *Dup() const;
    virtual size_t Read(CryStream *ToStream,size_t Size) const;
    virtual size_t Write(const CryStream *FromStream,size_t Size);
    virtual size_t Read(CryStream *ToStream) const;
    virtual size_t Write(const CryStream *FromStream);
    virtual size_t Read(char *ToBuffer,size_t t) const;
    virtual size_t Write(const char *FromBuffer,size_t t);
    virtual size_t WriteNStr(const char *Buffer);
    virtual size_t ReadNStr(char *Buffer,size_t MaxLength) const;
    int Pos(const char *s,size_t StartIndex);
    int Pos(const char *s) const;
    /// returns NULL if not found
    const char *strstr(CryString &s) const;
    const char *strstr(const char *s) const;
    int strncmp(CryString &s,size_t n) const;
    int strncmp(const char *s,size_t n) const;
    int strcmp(CryString &s) const;
    int strcmp(const char *s) const;
    virtual CryString &Rev();
    const char *strcat(CryString &s);
    const char *strcat(const char *s);
    const char *FormatHex(cbyte *data,size_t length);
    const CryString *GetAddress();
    bool operator ==(const char *s) const ;
	bool operator !=(const char *s) const;
	CryString & operator +=(const char *s);
	virtual bool IsOpen() const;
    virtual int scanf(const char *format,...) const;
    virtual size_t printf(const char *format,...);
    virtual CryString &TrimLeft();
    virtual CryString &TrimRight();
    virtual CryString &Trim();
    virtual CryString &ToUpper();
    virtual CryString &ToLower();
    virtual CryString &Insert(int Index,const char *Text);
    virtual CryString &Replace(const char *_Search,const char *_Replace=0);
    virtual CryString &ReplaceAppend(const char *_Search,const char *_Replace,const char *_Append=0);
    virtual CryString &ExclusiveAppend(const char *_Append);
    /// returns offset from start of string or -1 if not found
    int SeekTextFromStart(const char *SearchString) const;
    /// returns offset from start of string or -1 if not found
	int SeekTextFromCurrent(const char *SearchString) const;
	/// Return a non-const reference to the ith character -i refrences from the tail of the string back
	char& operator[](int i);

    virtual void Close(bool ExceptOnError=true);
    virtual bool Convert(int *i) const;
    virtual bool Convert(char *b,int len) const;
    //    virtual bool Convert(double *d) const;
    CryString & operator =(const char *text);
    CryString & operator = (const CryString *v);
    CryString & operator = (const CryString &v);
    void SetValue(const char *_Value);
    virtual size_t Size() const;
    virtual bool HasProperty(const CryPropertyParser &PropertyName) const;
    virtual bool SetProperty(const CryPropertyParser &PropertyName,const char *PropertyValue);

    virtual const char *GetProperty(const CryPropertyParser &PropertyName,CryString &Result) const;
#ifdef VALIDATING

    virtual bool Test(bool Verbose,Object &Object,bool (CallBack)(bool Verbose,const char *Result,bool fail));
#endif

    List *ListFromString(const char *Separator) const;
    List *LoadListFromString(const char *Separator,List *ListToLoad) const;
    /* TODO : Impliment */
    virtual void Sort(int CompareType=0);
    virtual int CompareLogical(int CompareType,const Object *Test) const;
    virtual bool LessThen(int CompareType,const Object *Test) const;
	virtual bool GreaterThen(int CompareType,const Object *Test)const;
	virtual bool EqualTo(int CompareType,const Object *Test)const;
	virtual int HashValue()const;
}
;//CryString
#endif	// TCryString
}
