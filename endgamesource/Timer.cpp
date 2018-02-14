#include "Houdini.h"
Chicken CChicken;
DWORD WQuestID=NULL;
DWORD Type;

PREBUFFINFO PREBUFFSTRUCT[] = {
	{"Battle Command",AFFECT_BATTLECOMMAND,&v_BattleCommandActive},
	{"Battle Orders",AFFECT_BATTLEORDERS,&v_BattleOrdersActive},
	{"Shout",AFFECT_SHOUT,&v_ShoutActive},
	{"Enchant",AFFECT_ENCHANT,&v_EnchantActive},
	{"Energy Shield",AFFECT_ENERGYSHIELD,&v_EnergyShieldActive},
	{"Frozen Armor",AFFECT_FROZENARMOR,&v_FrozenArmorActive},
	{"Chilling Armor",AFFECT_CHILLINGARMOR,&v_ChillingArmorActive},
	{"Shiver Armor",AFFECT_SHIVERARMOR,&v_ShiverArmorActive},
	{"Holy Shield",AFFECT_HOLYSHIELD,&v_HolyShieldActive},
	{"Cyclone Armor",AFFECT_CYCLONEARMOR,&v_CycloneArmorActive},
	{"Oak Sage",AFFECT_OAKSAGE,&v_OakSageActive},
	{"Bone Armor",AFFECT_BONEARMOR,&v_BoneArmorActive},
	{"Fade",AFFECT_FADE,&v_FadeActive},
	{"Burst Of Speed",AFFECT_BURSTOFSPEED,&v_BurstOfSpeedActive}
};

BOOL SetPrebuffsFalse()
{
	for (INT i=0;i<ArraySize(PREBUFFSTRUCT);i++)
	{
		*PREBUFFSTRUCT[i].Active=FALSE;
	}

	return TRUE;
}

BOOL CastOnUnit(WORD SkillID, UnitAny*pUnit, BOOL bLeft)
{
	SetSkill(SkillID, bLeft);
	BYTE *aPacket = new BYTE[9];
	aPacket[0] = bLeft ? 0x06 : 0x0D;
	*(DWORD*)&aPacket[1] = pUnit->dwType;
	*(DWORD*)&aPacket[5] = pUnit->dwUnitId;
	D2NET_SendPacket(9, 1, aPacket);
	delete aPacket;
	return TRUE;
}

BOOL ExecuteEnchantQueue(PENCHANTINFO EnchInfo)
{
	if(!GameReady(TRUE) || !EnchInfo)
		return FALSE;

	if(EnchInfo->dwPlayerID == Myself->dwUnitId)
		return FALSE;

	UnitAny* lpTarget = GetUnit(EnchInfo->dwPlayerID, UNIT_TYPE_PLAYER);

	char Buffer[200];

	if(!CheckParty(GetPvPStatus(lpTarget->dwUnitId)))
	{
		sprintf(Buffer,"/w %s You are not in my party!",GetNameByID(lpTarget->dwUnitId));
		BNCLIENT_SendBNMessage(Buffer);
		return TRUE;
	}
	if(!lpTarget) 
	{
		sprintf(Buffer,"/w %s You are out of range!",GetNameByID(lpTarget->dwUnitId));
		BNCLIENT_SendBNMessage(Buffer);
		return TRUE;
	}
	if (!GetSkill(D2S_ENCHANT))
	{
		v_EnchantBot=FALSE;
		return FALSE;
	}

	if (EnchInfo->nEnchantType==ENCHANT_ME)
	{
		sprintf(Buffer,"/w %s Enchanting you!",GetNameByID(lpTarget->dwUnitId));
		BNCLIENT_SendBNMessage(Buffer);

		CastOnUnit(D2S_ENCHANT,lpTarget,FALSE);
		Sleep(500);

		return TRUE;
	}
	else if (EnchInfo->nEnchantType==ENCHANT_MERC)
	{
		if(Myself && Myself->pAct)
		{
			for(Room1* pRoom = Myself->pAct->pRoom1; pRoom; pRoom = pRoom->pRoomNext)
			{
				for(UnitAny* pUnit = pRoom->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
				{
					if(pUnit->dwType == UNIT_MONSTER)
					{
						if(pUnit->dwTxtFileNo == MERC_A1 || pUnit->dwTxtFileNo == MERC_A2
						|| pUnit->dwTxtFileNo == MERC_A3 || pUnit->dwTxtFileNo == MERC_A5)
						{
							if(D2CLIENT_GetMonsterOwner(pUnit->dwUnitId) == lpTarget->dwUnitId)
							{
								sprintf(Buffer,"/w %s Enchanting your mercenary!",GetNameByID(lpTarget->dwUnitId));
								BNCLIENT_SendBNMessage(Buffer);	

								CastOnUnit(D2S_ENCHANT,pUnit,FALSE);
								Sleep(500);

								return TRUE;
							}
						}
					}
				}
			}
		}			
		sprintf(Buffer,"/w %s Couldn't find your mercenary!",GetNameByID(lpTarget->dwUnitId));
		BNCLIENT_SendBNMessage(Buffer);

		return TRUE;
	}
	else if (EnchInfo->nEnchantType==ENCHANT_ALL)
	{
		sprintf(Buffer,"/w %s Enchanting you and your summons!",GetNameByID(lpTarget->dwUnitId));
		BNCLIENT_SendBNMessage(Buffer);

		CastOnUnit(D2S_ENCHANT,lpTarget,FALSE);
		Sleep(500);

		if(Myself && Myself->pAct)
		{
			for(Room1* pRoom = Myself->pAct->pRoom1; pRoom; pRoom = pRoom->pRoomNext)
			{
				for(UnitAny* pUnit = pRoom->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
				{
					if(pUnit->dwType == UNIT_MONSTER)
					{
						if(pUnit->dwTxtFileNo == MERC_A1 || pUnit->dwTxtFileNo == MERC_A2
						|| pUnit->dwTxtFileNo == MERC_A3 || pUnit->dwTxtFileNo == MERC_A5)
						{
							if(D2CLIENT_GetMonsterOwner(pUnit->dwUnitId) == lpTarget->dwUnitId)
							{
								CastOnUnit(D2S_ENCHANT,pUnit,FALSE);
								Sleep(500);

								goto ChantAll;
							}
						}
					}
				}
			}
		}
ChantAll:
		if(Myself && Myself->pAct)
		{
			for(Room1* pRoom = Myself->pAct->pRoom1; pRoom; pRoom = pRoom->pRoomNext)
			{
				for(UnitAny* pUnit = pRoom->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
				{
					if(pUnit->dwType == UNIT_MONSTER)
					{
						if(pUnit->dwTxtFileNo == MERC_A1 || pUnit->dwTxtFileNo == MERC_A2
						|| pUnit->dwTxtFileNo == MERC_A3 || pUnit->dwTxtFileNo == MERC_A5)
						{
							continue;
						}
						else if(D2CLIENT_GetMonsterOwner(pUnit->dwUnitId) == lpTarget->dwUnitId)
						{
							CastOnUnit(D2S_ENCHANT,pUnit,FALSE);
							Sleep(500);
						}
					}
				}
			}
		}

		return TRUE;
	}

	return FALSE;
}

VOID SendMouseClick(INT x, INT y, INT clicktype)
{
	LPARAM lp = x + (y << 16);
	switch(clicktype)
	{
	case 0:
		SendMessage(D2WIN_GetHwnd(), WM_LBUTTONDOWN, 0, lp);
		break;
	case 1:
		SendMessage(D2WIN_GetHwnd(), WM_LBUTTONUP, 0, lp);
		break;
	case 2:
		SendMessage(D2WIN_GetHwnd(), WM_RBUTTONDOWN, 0, lp);
		break;
	case 3:
		SendMessage(D2WIN_GetHwnd(), WM_RBUTTONUP, 0, lp);
		break;
	}
}

DWORD WINAPI TickThread(LPVOID ThreadType)
{
	INT nThreadType = (INT)ThreadType;

	switch(nThreadType)
	{
	case THREAD_PRIMARY:

		while(TRUE)
		{
			if(!v_GameInit && Myself)
			{
				OnGameEvent(FALSE);
			}
			else if(v_GameInit && !Myself)
			{
				OnGameEvent(TRUE);
			}
			if(GetGameInfo()->szCharName!=NULL)
			{
				TimerTick();
			}
			else
			{
				if (v_Flashing) v_Flashing=FALSE;
			}

			Sleep(10);
		}		

		break;

	case THREAD_CHICKEN:

		while (TRUE)
		{
			if (IsTownLevel(GetPlayerArea()))
			{
				if (v_AttemptedChicken)
				{
					v_AttemptedChicken=FALSE;
				}
			}
			for (INT i=0;i<ArraySize(PREBUFFSTRUCT);i++)
			{
				BOOL bTemp=*PREBUFFSTRUCT[i].Active;
				BOOL bActive=D2COMMON_GetUnitState(Myself, PREBUFFSTRUCT[i].Type) ? TRUE : FALSE;

				if (bActive && !bTemp || !bActive && bTemp)
				{
					PCHAR pChar=bActive ? "ÿc2Activated" : "ÿc1Deactivated";
					PrintConsoleString("ÿc4%s ÿc0has been %s",PREBUFFSTRUCT[i].Name,pChar);
					*PREBUFFSTRUCT[i].Active=bActive;
				}
			}
			if (D2COMMON_GetUnitState(Myself, AFFECT_OAKSAGE))
			{
				if (v_OakTimer == -1)
					v_OakTimer = GetTickCount();
			}
			if (D2COMMON_GetUnitState(Myself, AFFECT_BATTLEORDERS))
			{
				if (v_BoTimer == -1)
					v_BoTimer = GetTickCount();

			}
			if (!D2COMMON_GetUnitState(Myself, AFFECT_OAKSAGE))
			{
				v_OakTimer = -1;

			}
			if (!D2COMMON_GetUnitState(Myself, AFFECT_BATTLEORDERS))
			{
				v_BoTimer = -1;
			}
			
			if (GetTickCount() - v_OakTimer > 500 || v_OakTimer == -1)
			{
				if (GetTickCount() - v_BoTimer > 500 || v_BoTimer == -1)
				{			
					CChicken.ChickenCheck();
				}
			}

			Sleep(25);
		}

		break;

	case THREAD_ATTACK:

		while (TRUE)
		{
			while (!v_AttackPlayer && !v_AttackMonster && !v_AttackNearPlayer || IsTownLevel(GetPlayerArea()))
			{
				Sleep(1);
			}

			FormatAttack();

			Sleep(10);
		}

		break;

		// Starts teleport thread
	case THREAD_TELEPORT:

		if(v_TeleportQueue.IsEmpty())
		{
			DriverDestroyThread(v_TeleportThread);
		}

		WQuestID=NULL;
		Type=NULL;

		v_TeleportQueue.Lock();
		while(!v_TeleportQueue.IsEmpty())
		{
			v_AutoTele=true;

			BOOL bArrived = FALSE;
			INT nIterations = 0;

			if(GetCurrentSkill(FALSE) != D2S_TELEPORT)
				SetSkill(D2S_TELEPORT, FALSE);

			while(GetCurrentSkill(FALSE) != D2S_TELEPORT)
				Sleep(1);

			BYTE aPacket[5];
			aPacket[0] = 0x0C;
			*(LPWORD)&aPacket[1] = (WORD)v_TeleportQueue[0].x;
			*(LPWORD)&aPacket[3] = (WORD)v_TeleportQueue[0].y;
			D2NET_SendPacket(5, 1, aPacket);


			while(nIterations < 1000 && v_AutoTele)
			{
				if(!GameReady(TRUE) || IsTownLevel(GetPlayerArea()))
				{
					v_TeleportQueue.RemoveAll();
					v_TeleportQueue.Unlock();
					DriverDestroyThread(v_TeleportThread);
					break;
				}

				if(CalculateDistance(Myself->pPath->xPos, Myself->pPath->yPos, v_TeleportQueue[0].x, v_TeleportQueue[0].y) <= 5)
				{
					bArrived = TRUE;
					break;
				}

				Sleep(1);
				nIterations++;
			}

			if(!bArrived)
			{
				v_TeleportQueue.RemoveAll();
				v_TeleportQueue.Unlock();
				return 0;
			}

			v_TeleportQueue.RemoveAt(0);
			v_TeleportQueue.FreeExtra();
		}

		Sleep(100);

		if(QuestID==1)
		{
			WQuestID=GlobalVectors.QuestID;
			Type=0x05;
		}
		if(QuestID==2)
		{
			WQuestID=GlobalVectors.OtherID;
			Type=0x05;
		}
		if(QuestID==3)
		{
			WQuestID=GlobalVectors.SpecialID;
			Type=0x05;
		}

		
		if(WQuestID && v_CurrentVector->dwTargetType != -1 && v_CurrentVector->nType != 3)
		{
			BYTE EnterPacket[]={0x13,0x05,0,0,0, 0x00,0x00,0x00,0x00};
			*(DWORD*)&EnterPacket[1]=Type;
			*(DWORD*)&EnterPacket[5]=WQuestID;
			SendGAMEPacket(EnterPacket,sizeof(EnterPacket));
			goto EndLoop;
		}	

		if(v_CurrentVector)
		{
			for(Room1* pRoom = Myself->pAct->pRoom1; pRoom; pRoom = pRoom->pRoomNext)
			{
				for(UnitAny* pUnit = pRoom->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
				{
					if(pUnit->dwType == UNIT_TYPE_TILE && v_CurrentVector->dwTargetType == -1)
					{
						if(v_GlobalMap->GetTileLevelNo(pRoom->pRoom2, pUnit->dwTxtFileNo) == v_CurrentVector->dwTarget)
						{
							Interact(pUnit->dwUnitId, pUnit->dwType, FALSE);
							goto EndLoop;
						}
					}
					if(pUnit->dwType == UNIT_TYPE_OBJECT && v_CurrentVector->nType == 3)
					{
						if(pUnit->dwTxtFileNo == v_CurrentVector->dwTarget)
						{
							Interact(pUnit->dwUnitId, pUnit->dwType, FALSE);
							goto EndLoop;
						}
					}
				}
			}
EndLoop:;

		}

		v_TeleportQueue.Unlock();

		v_AutoTele=false;

		Sleep(200);

		DriverDestroyThread(v_TeleportThread);

		break;

	case THREAD_ENCHANT:

		while (TRUE)
		{
			Sleep(500);

			if(!v_EnchantBot)
				continue;

			if(v_EnchantQueue.IsEmpty() || v_EnchantQueue.IsLocked)
				continue;

			ExecuteEnchantQueue(v_EnchantQueue[0]);
			v_EnchantQueue.RemoveAt(0);
			v_EnchantQueue.FreeExtra();

			Sleep(1000);

			v_EnchBusy=FALSE;
		}


		break;

	case THREAD_ANTIPK:

		while(TRUE)
		{
			Sleep(1);				

			if(v_AntiTppkBusy)
			{
				for(Room1* pRoom = Myself->pAct->pRoom1; pRoom && v_AntiTppkBusy && !IsTownLevel(GetPlayerArea()); pRoom = pRoom->pRoomNext)
				{
					for(UnitAny* pUnit = pRoom->pUnitFirst; pUnit && v_AntiTppkBusy && !IsTownLevel(GetPlayerArea()); pUnit = pUnit->pListNext)
					{
						if(pUnit->dwType == UNIT_TYPE_OBJECT && pUnit->dwTxtFileNo == 59)
						{
							if(!stricmp(pUnit->pObjectData->szOwner, Myself->pPlayerData->szName))
							{
								if(GetUnitDist(Myself, pUnit) < 10)
								{
									for(INT i = 0; i < 5 ; i++)
									{
										if(IsTownLevel(GetPlayerArea()))
										{
											v_AntiTppkBusy = v_TakeNextTP = false;

											goto ContinueIt;
										}

										Interact(pUnit->dwUnitId, UNIT_TYPE_OBJECT, TRUE);

										Sleep(100);
									}
								}
							}
						}
					}
				}
			}

ContinueIt:

			continue;
		}

		break;
	}

	return TRUE;
}
void TimerTick() 
{
	if(v_TakeNextTP || v_AntiTppkBusy)
	{
		if(IsTownLevel(GetPlayerArea()))
		{
			v_TakeNextTP=v_AntiTppkBusy=FALSE;
		}
	}
	if(v_CampAny || v_CampTarget)
	{
		D2CLIENT_clickMap(3,Camp.x,Camp.y,8);
	}				
	if (v_CS)
	{
		if (GetAsyncKeyState(VK_SNAPSHOT))
		{
			if(!v_HideConsole) v_HideConsole=TRUE;

			while (GetAsyncKeyState(VK_SNAPSHOT))
			{
				Sleep(500);
			}

			if(v_HideConsole) v_HideConsole=FALSE;
		}
	}
	if(v_Flashing && GetTickCount() - v_FlashTimer >= v_FlashDelay*10)
	{
		if(FindUnit(Auto.GetPlayerID(),0)!=NULL)
		{
			FlashPlayer(Auto.GetPlayerID());						
			v_FlashTimer=GetTickCount();
		}
	}
}
DWORD GetUnitDist(UnitAny* pUnit1, UnitAny* pUnit2)
{
	DWORD dwDist[4] = {0};

	switch(pUnit1->dwType)
	{
	case UNIT_TYPE_PLAYER:
	case UNIT_TYPE_MISSILE:
	case UNIT_TYPE_ITEM:
		dwDist[0] = pUnit1->pPath->xPos;
		dwDist[1] = pUnit1->pPath->yPos;
		break;

	case UNIT_TYPE_OBJECT:
		dwDist[0] = pUnit1->pObjectPath->dwPosX;
		dwDist[1] = pUnit1->pObjectPath->dwPosY;
		break;
	}

	switch(pUnit2->dwType)
	{
	case UNIT_TYPE_PLAYER:
	case UNIT_TYPE_MISSILE:
	case UNIT_TYPE_ITEM:
	case UNIT_TYPE_NPC:
		dwDist[2] = pUnit2->pPath->xPos;
		dwDist[3] = pUnit2->pPath->yPos;
		break;

	case UNIT_TYPE_OBJECT:
		dwDist[2] = pUnit2->pObjectPath->dwPosX;
		dwDist[3] = pUnit2->pObjectPath->dwPosY;
		break;
	}

	for(int x = 0; x < 4; x++)
		if(!dwDist[x])
			return INFINITE;

	return (DWORD)CalculateDistance(dwDist[0], dwDist[1], dwDist[2], dwDist[3]);
}
INT nMode[] = {
	PLAYER_MODE_DEATH,
	PLAYER_MODE_BEING_HIT,
	PLAYER_MODE_ATTACK1,
	PLAYER_MODE_ATTACK2,
	PLAYER_MODE_BLOCK,
	PLAYER_MODE_CAST,
	PLAYER_MODE_THROW,
	PLAYER_MODE_KICK,
	PLAYER_MODE_DEAD,
	PLAYER_MODE_SEQUENCE,
	PLAYER_MODE_BEING_KNOCKBACK
};
VOID FormatAttack()
{
	for (int i=0;i<ArraySize(nMode);i++)
	{
		if (Myself->dwMode==nMode[i])
		{
			return;
		}
	}
	if (v_AntiTppkBusy || !GameReady(FALSE) || IsTownLevel(GetPlayerArea()))
	{
		v_AttackPlayer=v_AttackMonster=v_AttackNearPlayer=FALSE;
	}
	else if(v_AttackPlayer && Players()>1 && GetNameByID(Auto.GetPlayerID())!="???")
	{
		Auto.AttackPlayer(Auto.GetPlayerID(), UNIT_TYPE_PLAYER);
	}
	else if (v_AttackMonster)
	{
		Auto.FindNearestUnit(TRUE);
	}
	else if (v_AttackNearPlayer)
	{
		Auto.FindNearestUnit(FALSE);
	}
}

BOOL IsImmune(UnitAny* pUnit,D2SpellInfo Spell)
{
	WORD SkillID=GetCurrentSkill(LeftAA);

	if (Spell.dwSpellInfoFlag &DSI_COLD)
	{
		if (D2COMMON_GetUnitStat(pUnit,43,0)+300>399)
			return TRUE;
	}
	if (Spell.dwSpellInfoFlag &DSI_FIRE)
	{
		if (D2COMMON_GetUnitStat(pUnit,39,0)+300>399)
			return TRUE;
	}
	if (Spell.dwSpellInfoFlag &DSI_LIGHTNING)
	{
		if (D2COMMON_GetUnitStat(pUnit,41,0)+300>399)
			return TRUE;
	}
	if (Spell.dwSpellInfoFlag &DSI_POISON)
	{
		if (D2COMMON_GetUnitStat(pUnit,45,0)+300>399)
			return TRUE;
	}
	if (Spell.dwSpellInfoFlag &DSI_PHYSICAL)
	{
		if (Spell.dwSpellInfoFlag &DSI_LIGHTNING)
			return FALSE;

		if (D2COMMON_GetUnitStat(pUnit,36,0)+300>399)
			return TRUE;
	}
	if (Spell.dwSpellInfoFlag &DSI_MAGICAL)
	{
		if (SkillID==D2S_BLESSEDHAMMER)
			return FALSE;

		if (D2COMMON_GetUnitStat(pUnit,37,0)+300>399)
			return TRUE;
	}

	return FALSE;
}