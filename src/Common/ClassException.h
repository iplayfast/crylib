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
#include "CryObject.h"
namespace Crystal
{
using namespace std;
#ifndef TCryException
#define TCryException "CryException"


/// CryException is a bare bones class that is used for showing failures
/*!Exceptions are used for failures as opposed to return values. In this way functions are always assumed to
have worked if they return
*/
class CryException
{
    int _errno;
    char *Error;
public:
    CryException(const CryException &E);
    CryException(const char *FormatStr,...);
    CryException(const Object *Object,const char *FormatStr,...);
    CryException(const Object *Object,const CryException &E);
    //    CryException(const CryObject *Object,const CryException &E1,const CryException &E2);
    operator const char *() const;
    CryException(const Object *Object,int ErrorNumber,const char *FormatStr,...);
    CryException(const Object *Object,const char *sError,const long value);
    CryException(const Object *Object,const char *sError,const long value,const char *sError2);
    ~CryException();
    void SetErrno(int ErrorNumber);
    int GetErrno() const;
    void SetErrorStr(char *NewError);
};

/// simple default class for exceptions
class CryExceptUnknown : public CryException
{
public:
    CryExceptUnknown();
};
#endif
}
