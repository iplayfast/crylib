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

namespace Crystal
{
using namespace std;

#ifndef CTwoStreams
#define CTwoStreams "TwoStreams"



/*! TwoStreams purpose is only to provide a method of transporting streams within a CryObject, it doesn't own anything
	it's a convienence class
*/

class TwoStreams : public Object
{
    CryStream *Stream1;
    CryStream *Stream2;
    TwoStreams()
    {
        throw CryException("Cannot Dup this class");
    }
public:
    StdFunctions(TwoStreams,Object);
    TwoStreams(CryStream *_Stream1,CryStream *_Stream2);
    CryStream *GetStream1();
    CryStream *GetStream2();
};

void InitializeRandoms();
int RandomInt(int Low, int High);
double RandomDouble(double Low, double High);
bool InRange(int Test,int Low,int High);

#endif // TTwoStreams
}
