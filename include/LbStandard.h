/*********************************************************************************
    LbStandard.h
    Header file with standard includes the LightBikes2001 classes

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
#ifndef __LBSTANDARD__
#define __LBSTANDARD__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#ifdef WIN32
#   include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>             //DC: Added GLU include

// The port on the server to be used for TCP connections.
#define LB_SERVER_TCP_PORT 5000
#define LB_SERVER_UDP_PORT 6000

#define LB_SERVER_IP_ADDRESS 192.168.1.78

#endif