/*********************************************************************************
    LbNet.h
    Header file for the LightBikes2001 Networking subsystem classes

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
       Chris Skepper

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
#ifndef __LBNET__
#define __LBNET__

#define LB_NET_DISCONNECTED 1
#define LB_NET_CLIENT 2
#define LB_NET_SERVER 3
#define LB_NET_ERROR 4


enum LbGameEventId
{
    LB_GAME_PLAYERJOIN = 0 ,
    LB_GAME_PLAYERLEAVE = 1 ,
    LB_GAME_HANDCHANGE = 2 ,
    LB_GAME_CHAT = 3 ,
    LB_GAME_AWAY = 8 ,
    LB_GAME_BACK = 9 ,
    LB_GAME_NEWROUND = 10 ,
    LB_GAME_NEWGAME = 20 ,
    LB_GAME_ENDGAME = 21 ,
    LB_GAME_NEWMAP = 40
};

struct LbGameEvent
{
    LbGameEventId id ;
    int playerHash ;
    string message;
} ;

struct LbGamePositionUpdate
{
    int playerHash ;
    int sequence ;
    float x1 , y1 , x2 , y2 ;
    byte level ;
    byte direction ;
} ;

class LbNetSys
{
    public:
        virtual void InitialiseServerUDP ( int server_port , int client_port ) = 0 ;
        virtual void InitialiseClientUDP ( int server_port , int client_port ) = 0 ;
        virtual bool GetNextPositionUpdate ( LbGamePositionUpdate & u ) = 0 ;
        virtual void PollSocketsUDP ( ) = 0 ;
        virtual void SendPositionUpdate( LbGamePositionUpdate & u ) = 0 ;
        virtual int GetStatus ( ) = 0 ;
        virtual bool GetNextGameEvent (  LbGameEvent &e ) = 0 ;
        virtual void SendGameEvent ( LbGameEvent &e , bool includeourself ) = 0 ;
        virtual void ProcessMessages ( ) = 0 ;
        virtual void Init ( LbOSLayerSys *os_sys ) = 0 ;
        virtual void PollSockets ( ) = 0 ;
        virtual void InitialiseClientTCP ( const char * address , int port ) = 0 ;
        virtual void InitialiseServerTCP ( int port ) = 0 ;
        virtual int GetOwnPlayerHash ( ) = 0 ;
        virtual string GetError ( ) = 0 ;
        virtual void SocketError ( int c ) = 0 ;
        virtual void ResetConnections ( ) = 0 ;
        // empty virtual destructor to ensure proper cleanup
        virtual ~LbNetSys(){}
};

LbNetSys *CreateNetSys ( LbOSLayerSys *os_sys ) ;

#endif