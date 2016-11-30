/*

*************************************************************************

ArmageTron -- Just another Tron Lightcycle Game in 3D.
Copyright (C) 2000  Manuel Moos (manuel@moosnet.de)

**************************************************************************

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
  
***************************************************************************

*/

#include "gTeam.h"
#include "uMenu.h"
#include "ePlayer.h"
#include "eTeam.h"

class gMenuItemPlayerTeam: public uMenuItem
{
    ePlayerNetID* 	player;
    eTeam*			team;
public:
    gMenuItemPlayerTeam(uMenu *M,ePlayerNetID* p, eTeam* t )
            : uMenuItem( M, tOutput("$team_menu_join_help") ),
            player ( p ),
            team ( t)
    {
    }

    virtual void Render(REAL x,REAL y,REAL alpha=1,bool selected=0)
    {
        tOutput text;
        text.SetTemplateParameter(1 , team->Name() );
        text << "$team_menu_join";
        DisplayTextSpecial( x, y, text, selected, alpha );
    }

    virtual void Enter()
    {
        player->SetTeamWish( team );
        menu->Exit();
    }
};

class gMenuItemNewTeam: public uMenuItem
{
    ePlayerNetID* 	player;
public:
    gMenuItemNewTeam( uMenu *M,ePlayerNetID* p )
            : uMenuItem( M, tOutput("$team_menu_create_help") ),
            player ( p )
    {
    }

    virtual void Render(REAL x,REAL y,REAL alpha=1,bool selected=0)
    {
        DisplayTextSpecial( x, y, tOutput("$team_menu_create"), selected, alpha );
    }

    virtual void Enter()
    {
        player->CreateNewTeamWish();
        menu->Exit();
    }
};

class gMenuItemPlayer: public uMenuItem
{
    ePlayerNetID* player;
public:
    gMenuItemPlayer(uMenu *M,ePlayerNetID* p,
                    const tOutput& help)
            : uMenuItem( M, help ),
            player ( p )
    {
    }

    virtual void Render(REAL x,REAL y,REAL alpha=1,bool selected=0)
    {
        DisplayTextSpecial( x, y, player->GetName(), selected, alpha );
    }

    virtual void Enter()
    {
        int i;

        tOutput title;
        title.SetTemplateParameter( 1, player->GetName() );
        title << "$team_menu_player_title";
        uMenu playerMenu( title );
        tArray<uMenuItem*> items;

        for ( i = eTeam::teams.Len()-1; i>=0; --i )
        {
            eTeam *team = eTeam::teams(i);
            if ( team != player->NextTeam() && team->PlayerMayJoin( player ) )
            {
                items[ items.Len() ] = tNEW( gMenuItemPlayerTeam ) ( &playerMenu, player, team );
            }
        }

        if ( /* eTeam::NewTeamAllowed() && */
            !( player->NextTeam() && player->NextTeam()->NumHumanPlayers() == 1 &&
               player->CurrentTeam() && player->CurrentTeam()->NumHumanPlayers() == 1 )
        )
        {
            items[ items.Len() ] = tNEW( gMenuItemNewTeam ) ( &playerMenu, player );

        }

        playerMenu.Enter();

        for ( i = items.Len()-1; i>=0; --i )
        {
            delete items(i);
        }
    }
};




// bring up the team selection menu
void gTeam::TeamMenu()
{
    int i;

    uMenu Menu( tOutput("$team_menu_title") );
    tArray<uMenuItem*> items;

    for ( i = MAX_PLAYERS; i>=0; --i )
    {
        if ( ePlayer::PlayerIsInGame(i))
        {
            ePlayer* player = ePlayer::PlayerConfig( i );
            tOutput help;
            help.SetTemplateParameter(1, player->Name() );
            help << "$team_menu_player_help";
            ePlayerNetID* pni = player->netPlayer;
            if ( pni && !player->spectate )
                items[ items.Len() ] = tNEW( gMenuItemPlayer ) ( &Menu, pni, help );
        }
    }

    if ( items.Len() > 1 )
    {
        Menu.Enter();
    }
    else if ( items.Len() >= 1 )
    {
        items[0]->Enter();
    }

    for ( i = items.Len()-1; i>=0; --i )
    {
        delete items(i);
    }
}


