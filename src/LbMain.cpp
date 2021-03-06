/*********************************************************************************
    LbMain.cpp
    Class file for the LightBikes2001 Main system

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black

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
#include "LbStandard.h"
#include "LbPublic.h"

int WINAPI WinMain(HINSTANCE hInstance,  // handle to current instance
						 HINSTANCE hPrevInstance,  // handle to previous instance
						 LPSTR lpCmdLine,      // pointer to command line
						 int nCmdShow          // show state of window
						 )
{
LbGameSys *main_game=CreateGameSys();

int ret_val=main_game->RunGame();
 
delete main_game;
main_game=NULL;
 
return ret_val;
}
