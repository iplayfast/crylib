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
#include <stdlib.h>

#include "Utility.h"

using namespace Crystal;
using namespace std;

//////////////////////////////////////////////////////////////////////////////
// TwoStreams
//////////////////////////////////////////////////////////////////////////////
/*! TwoStreams purpose is only to provide a method of transporting streams within a Object, it doesn't own anything
	it's a convienence class
*/

TwoStreams::TwoStreams(Stream *_Stream1,Stream *_Stream2)
{
    Stream1 = _Stream1;
    Stream2 = _Stream2;
}
Stream *TwoStreams::GetStream1()
{
    return Stream1;
}
Stream *TwoStreams::GetStream2()
{
    return Stream2;
}

//////////////////////////////////////////////////////////////////////////////
// Utility functions, not class bound
//////////////////////////////////////////////////////////////////////////////

namespace Crystal
{

void InitializeRandoms()
{
    srand(4711);
}


int RandomInt(int Low, int High)
{
    return rand() % (High-Low+1) + Low;
}

double RandomDouble(double Low, double High)
{
    return ((double) rand() / RAND_MAX) * (High-Low) + Low;
}

bool InRange(int Test,int Low,int High)
{
    return (Test>=Low) && (Test<=High);
}
}

