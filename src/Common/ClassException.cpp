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
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>

#include "ClassException.h"

using namespace Crystal;
using namespace std;

//---------------------------------------------------------------------------
// CryException
//---------------------------------------------------------------------------
#pragma package(smart_init)

Exception::Exception(const char *FormatStr,...)
{
    char Buffer[1024];
    va_list argptr;
    va_start(argptr,FormatStr);
    int size = ::vsnprintf(Buffer,1024,FormatStr,argptr);
    errno = 0;
    Error = new char[size + 1];
    strncpy(Error,Buffer,size);
    Error[size] = '\0';
}

Exception::Exception(const Object *Object,const char *FormatStr,...)
{
    char Buffer[1024];
    va_list argptr;
    va_start(argptr,FormatStr);
    vsprintf(Buffer,FormatStr,argptr);
    const char *n = Object->ChildClassName();
    Error = new char[strlen(n) + strlen(Buffer)+5];
    errno = -1;
    strcpy(Error,"In ");
    strcat(Error,n);
    strcat(Error," ");
    strcat(Error,Buffer);
}


Exception::Exception(const Exception &E)
{
    Error = new char[strlen(E)+1];
    errno = E.GetErrno();
    strcpy(Error,E);
}
Exception::~Exception()
{
    delete[] Error;
}

Exception::operator const char *() const
{
    return Error;
}

void Exception::SetErrno(int ErrorNumber)
{
    _errno = ErrorNumber;
}
int Exception::GetErrno() const
{
    return _errno;
}

/*CryException::CryException(const Object *_Object,const CryException &E1,const CryException &E2)
{
 
    Error = new char[strlen(E1) + strlen(E2) + 1];
    strcpy(Error,E1);
    strcat(Error,E2);
    errno = E1.errno;
    if (E2.errno!=-1)
        errno = E2.errno;
} */
Exception::Exception(const Object *Object,const Exception &E)
{
    Error = new char[strlen(E)+1];
    errno = E.GetErrno();
    strcpy(Error,E);
}

Exception::Exception(const Object *Object,int ErrorNumber,const char *FormatStr,...)
{
    errno = ErrorNumber;
    char Buffer[1024];
    va_list argptr;
    va_start(argptr,FormatStr);
    vsprintf(Buffer,FormatStr,argptr);
    const char *n = Object->ChildClassName();
    Error = new char[strlen(n) + strlen(Buffer)+5];
    errno = -1;
    strcpy(Error,"In ");
    strcat(Error,n);
    strcat(Error," ");
    strcat(Error,Buffer);


}

Exception::Exception(const Object *Object,const char *sError,const long value)
{
    char sv[40];
    errno = -1;
    sprintf(sv,"%ld",value);
    Error = new char[strlen(sError) + strlen(sv) + 1];
    strcpy(Error,sError);
    strcat(Error,sv);
}
Exception::Exception(const Object *Object,const char *sError1,const long value,const char *sError2)
{
    char sv[40];
    errno = -1;
    sprintf(sv,"%ld",value);
    Error = new char[strlen(sError1) + strlen(sv) + strlen(sError2) + 1];
    strcpy(Error,sError1);
    strcat(Error,sv);
    strcat(Error,sError2);
}
void Exception::SetErrorStr(char *NewError)
{
    delete[] Error;
    Error = new char[strlen(NewError)+1];
    strcpy(Error,NewError);
}

///----------------------------------------------------------------------------
// CryExceptUnknown
///----------------------------------------------------------------------------

CryExceptUnknown::CryExceptUnknown() : Exception("Unknown")
{}
