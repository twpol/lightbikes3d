/***    ******************************************************************************
    LbGraphicsLevel.cpp
    Implementation file for the level renderer.

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
#include "LbGraphicsImp.h"

/*
** LbGraphicsLevel methods
*/

void LbGraphicsLevelImp::SetBlock(LbBaseBlock *btype,int x,int y)
{
}

void LbGraphicsLevelImp::DrawLevel(LbVector &offset)
{
    glBegin(GL_QUADS);
        // Floor.
        glColor4f( 0.3f, 0.3f, 0.3f, 0.5f );
        glVertex3f( -90.0f , 0.0f  , -90.0f );
        glVertex3f(  90.0f , 0.0f  , -90.0f );
        glVertex3f(  90.0f , 0.0f  , 90.0f );
        glVertex3f(  -90.0f , 0.0f  , 90.0f );
        // Walls.
        glColor4f( 0.7f, 0.7f, 0.7f, 0.5f );
        glVertex3f( -90.0f , 0.0f  , -90.0f );
        glVertex3f(  90.0f , 0.0f  , -90.0f );
        glVertex3f(  90.0f , 4.0f  , -90.0f );
        glVertex3f(  -90.0f , 4.0f  , -90.0f );

        glColor4f( 0.7f, 0.7f, 0.7f, 0.5f );
        glVertex3f( 90.0f , 0.0f , -90.0f );
        glVertex3f( 90.0f , 0.0f  , 90.0f );
        glVertex3f( 90.0f , 4.0f  , 90.0f );
        glVertex3f( 90.0f , 4.0f  , -90.0f );

        glColor4f( 0.7f, 0.7f, 0.7f, 0.5f );
        glVertex3f( 90.0f , 0.0f , 90.0f );
        glVertex3f( -90.0f , 0.0f , 90.0f );
        glVertex3f( -90.0f , 4.0f , 90.0f );
        glVertex3f( 90.0f , 4.0f , 90.0f );

        glColor4f( 0.7f, 0.7f, 0.7f, 0.5f );
        glVertex3f( -90.0f , 0.0f  , 90.0f );
        glVertex3f( -90.0f , 0.0f  , -90.0f );
        glVertex3f( -90.0f , 4.0f  , -90.0f );
        glVertex3f( -90.0f , 4.0f  , 90.0f );

    glEnd();
}

/*
** LbGraphicsLevelImp methods
*/
LbGraphicsLevelImp::LbGraphicsLevelImp()
{
}

LbGraphicsLevelImp::~LbGraphicsLevelImp()
{
}

