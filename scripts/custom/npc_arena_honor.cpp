/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: npc_arena_honor
SD%Complete: 100%
SDComment: by tempura, corrected by /dev/rsa
SDCategory: custom
EndScriptData */

#include "precompiled.h"
#include "sc_creature.h"
#include "sc_gossip.h"
//#include "Player.h"

#define GOSSIP_ITEM_ARENA_TO_HONOR "Change 50 Arena to 1000 Honor"
#define GOSSIP_ITEM_HONOR_TO_ARENA "Change 1000 Honor to 50 Arena"

bool GossipHello_npc_arena_honor(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_HONOR_TO_ARENA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_ARENA_TO_HONOR, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->SEND_GOSSIP_MENU(3961,_Creature->GetGUID());
	return true;
}

bool GossipSelect_npc_arena_honor(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        if (pPlayer->GetHonorPoints() >= 1000)
        {
            pPlayer->ModifyHonorPoints(-1000);
            pPlayer->ModifyArenaPoints(+50);
        }
        else
            DoScriptText(UNSUCCESSFUL_HONOR, pCreature);
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
        if (pPlayer->GetHonorPoints() >= 10000)
        {
            pPlayer->ModifyHonorPoints(-10000);
            pPlayer->ModifyArenaPoints(+500);
        }
        else
            DoScriptText(UNSUCCESSFUL_HONOR, pCreature);
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 3)
    {
		if (player->GetHonorPoints() >= 1000)
        {
            pPlayer->ModifyArenaPoints(-100);
            pPlayer->ModifyHonorPoints(+2000);
        }
    }
    else if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
        if (player->GetArenaPoints() >= 50 && player->GetHonorPoints() <= 74000)
        {
            pPlayer->ModifyArenaPoints(-1000);
            pPlayer->ModifyHonorPoints(+20000);
        }
    }
    player->CLOSE_GOSSIP_MENU();
    return true;
}

void AddSC_npc_arena_honor()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_arena_honor";
    newscript->pGossipHello =  &GossipHello_npc_arena_honor;
    newscript->pGossipSelect = &GossipSelect_npc_arena_honor;
    newscript->RegisterSelf();
}
