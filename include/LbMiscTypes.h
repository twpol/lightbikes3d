/*********************************************************************************
    LbMiscTypes.h
    Header file to define useful types for LightBikes2001

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
       David Capps

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*********************************************************************************/

#ifndef __LBMISCTYPES__
#define __LBMISCTYPES__

struct LbRGBAColor
{
LbRGBAColor()
{
    r=g=b=a=0;
}

LbRGBAColor(byte nr,byte ng,byte nb,byte na)
{
	r=nr; g=ng; b=nb; a=na;
}

byte r,g,b,a;
};

enum LbBlockDirection
	{
	LB_DIR_N		=0,
	LB_DIR_S,
	LB_DIR_E,
	LB_DIR_W,
	};

enum LbCollideEvent
{
	LB_PLAYER_OK = 0,
	LB_PLAYER_DIE,
};

class LbError
{
public:

LbError(const char *msg) : error_msg(msg){}

const char *GetErrorMsg()
	{
	return error_msg;
	}

private:

const char *error_msg;
};
#endif
