/*********************************************************************************
    LbGame.cpp
    Class file for the LightBikes2001 Game subsystem

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
       James Ross
       David Capps
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
#include "LbStandard.h"
#include "LbPublic.h"
#include "LbGameImp.h"

LbGameImp::LbGameImp()
{
    os_sys=NULL;
    graph_sys=NULL;
    input_sys=NULL;
    sound_sys=NULL;
    net_sys=NULL;
}

LbGameImp::~LbGameImp()
{
    // Make double sure things have been cleaned up.
    DeInitSubsystems();
}


int LbGameImp::RunGame()
{
    LbOSLayerEvent os_event;
    LbGameEvent game_event;
    LbVector target(0,0,0), up(0,1,0);
    LbVector *eye;
    float scroll,change;
    int count=0,lastms=0,fps=0, startms=0, keycount;
    int lpress=0, rpress=0;
    char msg[16] = "", inp[32] = "", keymsg[32] = "";
    LbOSLayerKeypress keys[32];

    InitSubsystems();

    quit_flag=false;
    eye = new LbVector(0,0,0);
    change = 0.1f;
    startms = lastms = os_sys->GetMS();

    char k ;
    int i , p ;
    bool showscores;

    // Default to not showing scores.
    showscores = false ;

    // Clear the buffer that stores the typed text.
    string textbuf = "";

    // Set our handle to the default for new players.
    ownhandle = "Unnamed" ;

    // Clear players.
    for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
        players [ i ].valid = false ;

    // Clear the typed messages.
    for ( i = 0 ; i < MAX_MESSAGE_LINES ; i ++ )
        txtmsgs [ i ] = "" ;

    // Clear the scoreboard.
    for ( i = 0 ; i < MAX_SCOREBOARD_LINES ; i ++ )
        scoremsgs [ i ] = "" ;

    graph_sys->TriggerEffect(LB_GFX_FADEINTEXTURE);
    sound_sys->PlayMusicFile("TRACK1.MP3"); //just for the moment

    int wave = sound_sys->CacheWaveFile("SOUND1.WAV"); //just for the moment


    while(!quit_flag)
    {
        scroll = scroll + ( change* ( os_sys->GetMS() - startms) / 50.0f );
        startms = os_sys->GetMS();
        if (scroll > 15) change=-0.1f;
        if (scroll < -15) change=0.1f;
        if ( !(count++ % 20) ) {
            lastms = (os_sys->GetMS() - lastms);
            fps = lastms? (20000 / lastms) : 0;
            lastms = os_sys->GetMS();
            sprintf(msg,"FPS:%d",fps);
        }

        delete(eye);
        eye = new LbVector(scroll,0,-20);
        graph_sys->SetCamera(*eye,target,up);

        keycount = 32;
        inp[0] = 0;
        if (input_sys->GetOSKey(&keys[0], &keycount)) {
            for (int i=0; i<keycount; i++) {
                if (keys[i].down)
                    switch (keys[i].which) {
                        case LB_OSKEY_LEFT: sprintf(inp, "Left Key Press"); lpress++; sound_sys->PlayWaveFile(wave); break;
                        case LB_OSKEY_RIGHT:sprintf(inp, "Right Key Press"); rpress++; break;
                    }
            }
            if (keycount>0) sprintf(keymsg, "Left: %d, Right: %d", lpress, rpress);
        } else sprintf(inp, "Input Error");

        // Get text entered.
        while ( ( k = input_sys->getNextTextKey ( ) ) != 0 )
        {
            switch ( k )
            {
                case '\r' :
                    ProcessCommand ( textbuf ) ;
                    textbuf.erase() ;
                    break ;
                case 27 : // Escape key.
                    textbuf.erase ( ) ;
                    break ;
                case '\t': // Tab key, do nothing.
                    break;
                case 8 : // Backspace key.
                    if ( textbuf.size ( ) > 0 )
                        textbuf.erase ( textbuf.size ( ) - 1 ) ;
                    break ;
                default:
                    textbuf += k ;
                break ;
            }
        }

        graph_sys->StartFrame();
        // draw here
        graph_sys->SetTextColor(LbRGBAColor(1,0,0,1));
        graph_sys->DrawText(0.5f,0.82f,1.0f,"LightBikes3d");
        graph_sys->SetTextColor(LbRGBAColor(0,0,1,1));
        graph_sys->DrawText(0.6f,0.9f,1.0f,msg);
        graph_sys->SetTextColor(LbRGBAColor(1,1,0,1));
        graph_sys->DrawText(0.0f,0.25f,1.0f,inp);
        graph_sys->SetTextColor(LbRGBAColor(0,1,1,1));
        graph_sys->DrawText(0.0f,0.1f,1.0f,keymsg);

        // Display the typed text.
        graph_sys->SetTextColor ( LbRGBAColor ( 0 , 1 , 1 , 1 ) ) ;
        graph_sys->DrawText ( 0.0f , 0.02f , 0.75f , textbuf.c_str ( ) ) ;

        // Display the chat or status messages.
        for ( i = 0 ; i < MAX_MESSAGE_LINES ; i ++ )
        {
            graph_sys->SetTextColor ( LbRGBAColor ( 0 , 1 , 1 , 1 ) ) ;
            graph_sys->DrawText ( 0.05f , 0.9f - 0.04 * i , 0.5f , txtmsgs[i].c_str ( ) ) ;
        }

        if ( input_sys->IsTabDown ( ) )
        {
            // Display the players' frag count, overlayed Counter Strike style.
            for ( i = 0 ; i < MAX_SCOREBOARD_LINES ; i ++ )
            {
                graph_sys->SetTextColor ( LbRGBAColor ( 0 , 1 , 1 , 1 ) ) ;
                graph_sys->DrawText ( 0.05f , 0.7f - 0.04 * i , 0.5f , scoremsgs[i].c_str ( ) ) ;
            }
        }

        graph_sys->EndFrame();


        // poll the event queue.
        while(os_sys->PollEvent(os_event))
        {
            /* OS event:
            **  Minimise/retstore
            **  Active/deactivate
            **  Close
            **  Normal keypress (for menus, etc.)
            **  Return: Send player quip
            */
            // handle the event.
            switch ( os_event.id )
            {
                case LB_OSEVENT_QUIT:
                    quit_flag=true;// bye bye...
                    break;
            }
            // ignore unknown events...
        }

        // Process Network messages (Networking) ie. convert them from
        // strings and packets to game messages, and add them to the
        // network module's queue of game messages.
        net_sys->ProcessMessages ( ) ;

        // Probably won't do this ultimately.
        net_sys->PollSockets ( ) ;

        // Deal with network messages.
        while ( net_sys->GetNextGameEvent ( game_event ) )
        {
            switch ( game_event.id )
            {
                // Deal with players joining.
                case LB_GAME_PLAYERJOIN :

                    AddPlayer ( game_event.playerHash , game_event.message ) ;
                    ShowStatusMessage (
                        GetPlayerHandle ( game_event.playerHash ) +
                        string ( " has joined the game." ) ) ;

                                        // If we are the server send information on the other players.
                                        if ( net_sys->GetStatus () == LB_NET_SERVER )
                                        {
                                            LbGameEvent e ;
                                            for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
                                            {
                                                if ( players[i].valid == true )
                                                {
                                                    e.id = LB_GAME_PLAYERJOIN ;
                                                    e.playerHash = players[i].hash ;
                                                    e.message = GetPlayerHandle ( players[i].hash ) ;
                                                    net_sys->SendGameEvent ( e , false ) ;
                                                }
                                            }
                    }


                    break;

                // Deal with players going.
                case LB_GAME_PLAYERLEAVE :
                    ShowStatusMessage (
                        GetPlayerHandle ( game_event.playerHash ) +
                        string ( " has left the game." ) +
                        game_event.message ) ;
                    RemovePlayer ( game_event.playerHash ) ;
                    break;

                // Deal with players changing name.
                case LB_GAME_HANDCHANGE :
                    ShowStatusMessage (
                        GetPlayerHandle ( game_event.playerHash ) +
                        string ( " is now known as " ) +
                        game_event.message + string ( "." ) ) ;
                    SetPlayerHandle ( game_event.playerHash ,
                                      game_event.message ) ;
                    break;

                // Deal with incoming chat messages.
                case LB_GAME_CHAT:
                    ShowStatusMessage (
                        string ("<" ) +
                        GetPlayerHandle ( game_event.playerHash ) +
                        string ( "> " ) + game_event.message ) ;
                    break;

                // New game message.  Abandon any current games.
                case LB_GAME_NEWGAME:
                    NewGame ( ) ;
                    break ;

                // If the server decides the game is over, return to chat.
                case LB_GAME_ENDGAME:
                    EndGame ( ) ;
                    break ;

                // If a round is being played kill it off.  Return players
                // to start positions.
                case LB_GAME_NEWROUND:
                    NewRound ( ) ;
                    break ;

                // If a new map is being sent.
                case LB_GAME_NEWMAP:
                    break ;

            }

            // Rebroadcast iff we are server and it didn't come from us,
            // because otherwise we would have already broadcast it.
            if ( net_sys->GetStatus ( ) == LB_NET_SERVER &&
                game_event.playerHash != net_sys->GetOwnPlayerHash ( ) )
                    net_sys->SendGameEvent ( game_event , false ) ;
        }

        // Update game state (Game Logic)...
            /* Updateing actions:
            **  Poll for Input events
            **  Interpret player input
            **  Proccess Network messages
            **  Updating bikes
            **  Check for collisions
            **  MENU (stuff)
            **      Interface?
            **      Messages --> Network
            */

        // Put a scoreboard together.  BTW I think we should be able to detect
        // 'kills' (when you cut in front of someone) fairly accurately,
        // experiments needed to distinguish between accidental deaths and
        // kills.
        p = 0 ;
        scoremsgs[0] = "Player Name       Kills      Deaths     Ping" ;
        scoremsgs[1] = "                                            " ;
        for ( i = 2 ; i < MAX_SCOREBOARD_LINES ; i++ )
        {
            while ( players [ p ].valid == false ) p++ ;
            if ( p >= MAX_PLAYERS - 1 )
                scoremsgs [ i ] = "" ;
            else
                scoremsgs [ i ] =
                        " " + Pad ( players [ p ] . handle , 18 ) +
                        Pad ( ItoS ( players [ p ] . kills ) , 11 ) +
                        Pad ( ItoS ( players [ p ] . deaths ) , 11 ) +
                        ItoS ( players [ p ] . ping ) ;
            p ++ ;
        }
    }

    delete eye;

    DeInitSubsystems ( ) ;

    return 0;
}

/**
 ** To go in a library at some point, or be replaced.
 **/
string LbGameImp::Pad ( string s , int l )
{
    if ( s.size ( ) > l ) return s.substr ( 0 , l ) ;
    else if ( s.size ( ) == l ) return s ;
    else return s + string ( l - s.size ( ) , ' ' ) ;
}

/**
 ** To go in a library at some point, or be replaced.
 **/
string LbGameImp::ItoS ( int i )
{
    char temp [ 20 ] ;
    itoa ( i , temp , 10 ) ;
    return string ( temp ) ;
}

/**
 ** Add the message to the messages at the top of the screen.
 ** Implements a nice shifting scrolling effect.
 **/
void LbGameImp::ShowStatusMessage ( const string & txt )
{
    int i ;

    // Fill first unfilled line.
    for ( i = 0 ; i < MAX_MESSAGE_LINES ; i ++ )
        if ( txtmsgs [ i ] == "" )
        {
            txtmsgs [ i ] = txt ;
            return ;
        }

    // Otherwise shift up, and fill last line.
    for ( i = 0 ; i < MAX_MESSAGE_LINES - 1 ; i ++ )
        txtmsgs [ i ] = txtmsgs [ i + 1 ] ;
    txtmsgs [ MAX_MESSAGE_LINES - 1 ] = txt ;
}

/**
 ** Return player's handle (name) as a string.
 **/
string LbGameImp::GetPlayerHandle ( int playerhash )
{
    int i ;
    for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
        if ( players [ i ].valid == true && playerhash == players [ i ].hash )
            if ( players [ i ] .handle != "" )
                return players [ i ] .handle ;
    return "Noname" ;
}

/**
 ** Set the handle (name) of a player.
 **/
bool LbGameImp::SetPlayerHandle ( int hash , const string & handle )
{
    int i ;
    for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
        if ( players [ i ].valid == true &&
             hash == players [ i ].hash )
        {
             players [ i ] .handle = handle ;
             return true ;
        }
    return false ;
}

/**
 ** Add player.
 **/
void LbGameImp::AddPlayer ( int hash , const string & handle )
{
    // Check for existing player.
    if ( SetPlayerHandle ( hash , handle ) ) return ;

    int i ;
    for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
        if ( players [ i ].valid == false )
        {
            players [ i ] .valid = true ;
            players [ i ] .hash = hash ;
            if ( handle == "" )
                players [ i ] .handle = "New Player" ;
            else players [ i ] .handle = handle ;
            return ;
        }
    // No spare slots.
}

/**
 ** Remove player.
 **/
void LbGameImp::RemovePlayer ( int hash  )
{
    int i;
    for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
        if ( players [ i ].valid == true && hash == players [ i ].hash )
            players [ i ].valid = false ;
}

/**
 ** New Round.
 **/
void LbGameImp::NewRound ( )
{
    // Set up start positions etc.
}

/**
 ** New Game.
 **/
void LbGameImp::NewGame ( )
{
    int i ;
    // Reset scores.
    for ( i = 0 ; i < MAX_PLAYERS ; i++ )
    {
        players [ i ] .kills = 0  ;
        players [ i ] .deaths = 0 ;
    }
}

/**
 ** End Game.
 **/
void LbGameImp::EndGame ( )
{
    // Set up start positions etc.
}

/**
 ** Takes a command entered, and deals with it appropriately.
 **/
void LbGameImp::ProcessCommand ( string t )
{
    LbGameEvent e ;
    t += " " ;
    if ( t.substr ( 0 , 1 ) == "\\" || t.substr ( 0 , 1 ) == "/" )
    {
        int m = t.find_first_of ( " " ) ;
        string cmd = t.substr ( 1 , m - 1 ) ,
               prm = t.substr ( m + 1 , t.size ( ) - m - 2 ) ;

        // If a command only allowed on the server is issued.
        if ( cmd == "newgame" ||
             cmd == "newround" ||
             cmd == "endgame" ||
             cmd == "newmap" )
        {
            if ( net_sys->GetStatus ( ) == LB_NET_SERVER )
            {
                e.message = prm ;

                if ( cmd == "newgame" )
                    e.id = LB_GAME_NEWGAME ;
                else if  ( cmd == "newround" )
                    e.id = LB_GAME_NEWROUND ;
                else if  ( cmd == "endgame" )
                    e.id = LB_GAME_ENDGAME ;
                else if ( cmd == "newmap" )
                    e.id = LB_GAME_NEWMAP ;
            }
            else
            {
                ShowStatusMessage ( "Error: This is not a server.") ;
                return ;
            }
        }

        // If a general program message was typed.
        else if ( cmd == "quit" )
        {
            quit_flag = true ;
            return ;
        }
        else if ( cmd == "connect" )
        {
            net_sys->ConnectToServer ( prm.c_str() , LB_SERVER_TCP_PORT ) ;
            if ( net_sys->GetStatus ( ) == LB_NET_CONNECTEDTOSERVER )
                ShowStatusMessage ( "Status: Connected to server." ) ;
            else
            {
                ShowStatusMessage ( "Error: Could not connect." ) ;
                return ;
            }

            // Send the join message.
            e.id = LB_GAME_PLAYERJOIN ;
            e.message = ownhandle;
        }
        else if ( cmd == "startserver" )
        {
            net_sys->InitiateServer ( prm.c_str() , LB_SERVER_TCP_PORT ) ;
            if ( net_sys->GetStatus ( ) == LB_NET_SERVER )
                ShowStatusMessage ( "Status: Server running." ) ;
            else
            {
                ShowStatusMessage ( "Error: Could not start server." ) ;
                return ;
            }

            // Send the join message.
            e.id = LB_GAME_PLAYERJOIN ;
            e.message = ownhandle;
        }
        else if ( cmd == "hand" )
        {
            ownhandle = prm ;
            e.message = prm ;
            e.id = LB_GAME_HANDCHANGE ;
        }
        else if ( cmd == "leave" )
        {
            e.message = prm ;
            e.id = LB_GAME_PLAYERLEAVE ;
        }
        else
        {
            // Command was invalid.
            ShowStatusMessage ( "Error: Unknown command." ) ;
            return ;
        }
    }
    else
    {
        // No command means it's chat.
        e.id = LB_GAME_CHAT ;

        // Strip whitespace from the start and end of t.
        int i , j ;
        i = t.find_first_not_of ( "\n\r \t" , 0 ) ;
        j = t.find_last_not_of ( "\n\r \t" ) ;

        // If the user only entered whitespace then forget it.
        if ( j == -1 ) return ;

        // Copy the non-whitespace part of the entry.
        e.message = t.substr ( i , j - i + 1 ) ;
    }

    // Attach our player hash and dispatch the message.
    e.playerHash = net_sys->GetOwnPlayerHash ( ) ;
    net_sys->SendGameEvent ( e , ( net_sys->GetStatus ( ) == LB_NET_SERVER ) ) ;
}

/**
 ** Starts stuff up.
 **/
void LbGameImp::InitSubsystems()
{
    os_sys=CreateOSLayerSys();
    graph_sys=CreateGraphicsSys(os_sys);
    input_sys=CreateInputSys(os_sys);
    sound_sys=CreateSoundSys(os_sys);
    net_sys=CreateNetSys(os_sys);
}

/**
 ** Shuts stuff down.
 **/
void LbGameImp::DeInitSubsystems()
{
    if(graph_sys!=NULL)
    {
        delete graph_sys;
        graph_sys=NULL;
    }

    if(input_sys!=NULL)
    {
        delete input_sys;
        input_sys=NULL;
    }

    if(sound_sys!=NULL)
    {
        delete sound_sys;
        sound_sys=NULL;
    }

    if(net_sys!=NULL)
    {
        delete net_sys;
        net_sys=NULL;
    }

    if(os_sys!=NULL)
    {
        delete os_sys;
        os_sys=NULL;
    }
};

/**
 ** Create stuff.
 **/
LbGameSys *CreateGameSys()
{
    LbGameSys *rval=new LbGameImp;
    assert(rval!=NULL);

    return rval;
}
