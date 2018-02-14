#include "Houdini.h"

struct BlindProtocol {
	DOUBLE X;
	DOUBLE Y;
};
BlindProtocol BlindAttacks[] = {
	{+2,+2},
	{+2,-1},
	{-1,+3},
	{+1,-1.2},
	{0,0}
};
VOID AutoAim::GetCurrent(DWORD AimID)
{
	if (GetNameByID(AimID)=="???")
		Current=Current;
	else
	{
		Init();

		for (INT i=0;i<Players();i++)
		{
			if (pUnit->dwUnitId==AimID)
			{
				Current=i;
				break;
			}
			pUnit=pUnit->pNext;
		}
	}
}
VOID AutoAim::GetNextPlayer()
{
	if (pUnit)
	{
		DWORD OldID=GetPlayerID();
		
		GetCurrent(GetPlayerID());

		if (Current==0||Current>=Players()-1)
			ChangeTarget(1);
		else
			ChangeTarget(Current+1);

		if (GetPlayerID()!=OldID)
		PrintConsoleString("ÿc8Now targetting: %s (Level %i %s)",GetNameByID(GetPlayerID()),GetLevelByName(GetNameByID(GetPlayerID())),GetClassByName(GetNameByID(GetPlayerID())));
	}
}
VOID AutoAim::GetLastPlayer()
{
	if(pUnit)
	{		
		DWORD OldID=GetPlayerID();

		GetCurrent(GetPlayerID());

		if (Current<=1||Current>Players()-1)
			ChangeTarget(Players()-1);
		else
			ChangeTarget(Current-1);

		if (GetPlayerID()!=OldID)
		PrintConsoleString("ÿc8Now targetting: %s (Level %d %s)",GetNameByID(GetPlayerID()),GetLevelByName(GetNameByID(GetPlayerID())),GetClassByName(GetNameByID(GetPlayerID())));
	}
}
long AutoAim::GetPlayerX()
{
	if(pUnit==NULL)
		Init();

	return pUnit->Xpos;
}
long AutoAim::GetPlayerY()
{
	if(pUnit==NULL)
		Init();

	return pUnit->Ypos;
}
INT AutoAim::GetPlayerLevel()
{
	if(pUnit==NULL)
		Init();

	return pUnit->wLevel;
}
BOOL AutoAim::SelectPlayer(DWORD PID)
{
	pUnit=(RosterUnit*)*(DWORD*)0x6FBCC080;

	for (INT i=0;pUnit;i++,pUnit=pUnit->pNext)
	{
		if(pUnit->dwUnitId==PID)
		{
			Current=i;
			break;
		}
	}

	return FALSE;
}
VOID AutoAim::Init()
{
	pUnit=(RosterUnit*)*(DWORD*)0x6FBCC080;
	Current = pUnit==NULL ? 0 : 1;
}
VOID AutoAim::ChangeTarget(INT nPlayer)
{
	Init();

	for(INT i = 0;i < nPlayer && i < Players() - 1; i++)
	{
		pUnit=pUnit->pNext;
	}

	Current=nPlayer;
}
DWORD AutoAim::GetPlayerID()
{
	if(pUnit==NULL)
	{
		Init(); 
		return pUnit->dwUnitId;
	}
	return pUnit->dwUnitId;
}
char* AutoAim::GetPlayerNAME()
{
	if(pUnit==NULL)  Init();
	return pUnit->szName2;
}
DWORD AutoAim::GetPlayerCLASS()
{
	if(pUnit==NULL) Init();
	return pUnit->dwClassId;
}
BOOL AutoAim::IsPlayerFriend()
{
	return FALSE;
}
BOOL AutoAim::AttackPlayer(DWORD dwUnitId, INT UnitType)
{
	UnitAny* pTarget = (UnitAny*)GetUnit(dwUnitId, UnitType);

	if (!pTarget)
	{
		return FALSE;
	}
	else
	{
		if (pTarget->dwUnitId==Myself->dwUnitId||GetUnitDist(Myself, pTarget)>50||pTarget->dwMode==PLAYER_MODE_DEAD||pTarget->dwMode==PLAYER_MODE_DEATH)
		{
			return FALSE;
		}
		WORD SkillID = GetCurrentSkill(LeftAA);
		AttackStruct* Attack = new AttackStruct;
		D2SpellInfo Spell;

		Attack->dwAttackType = LeftAA ? ATTACKTYPE_SHIFTLEFT : ATTACKTYPE_SHIFTRIGHT;
		GetSkillInfo(SkillID, &Spell);

		if (SkillID!=D2S_TELEPORT)
		{
			Attack->dwTargetX = pTarget->pPath->xPos;
			Attack->dwTargetY = pTarget->pPath->yPos;
		}
		else 
		{
			if (pTarget->dwMode==PLAYER_MODE_RUN&&!v_BlindToggled)
			{
				Attack->dwTargetX = pTarget->pObjectPath->dwPosX;
				Attack->dwTargetY = pTarget->pObjectPath->dwPosY;
			}
			else if (!v_BlindToggled)
			{
				Attack->dwTargetX = pTarget->pPath->xPos;
				Attack->dwTargetY = pTarget->pPath->yPos;
			}
			else
			{
				Attack->dwTargetX = pTarget->pPath->xPos + BlindAttacks[v_Blind-1].X;
				Attack->dwTargetY = pTarget->pPath->yPos + BlindAttacks[v_Blind-1].Y;
			}
		}

		Attack->lpPlayerUnit = Myself;

		if (SkillID!=D2S_TELEPORT||SkillID==D2S_TELEPORT && !v_BlindToggled)
		{
			Attack->lpTargetUnit = GetUnit(pTarget->dwUnitId, pTarget->dwType);
		}
		else
		{
			Attack->lpTargetUnit = NULL;
		}

		Attack->_1 = 3;
		Attack->_2 = 4;

		if (SkillID!=D2S_TELEPORT||SkillID==D2S_TELEPORT && !v_BlindToggled)
		{
			D2CLIENT_Attack(Attack, TRUE);
		}
		else
		{
			D2CLIENT_Attack(Attack, FALSE);
		}

		delete Attack;
	}

	return TRUE;
}
BOOL AutoAim::FindNearestUnit(BOOL bMonster)
{
	INT nDistance = 9999;
	D2SpellInfo Spell;
	UnitAny* pTarget = NULL;
	WORD SkillID = GetCurrentSkill(LeftAA);
	GetSkillInfo(SkillID, &Spell);

	if (bMonster)
	{
		for(Room1* pRoom1 = Myself->pAct->pRoom1; pRoom1; pRoom1 = pRoom1->pRoomNext)
		{
			for(UnitAny* pUnit1 = pRoom1->pUnitFirst; pUnit1; pUnit1 = pUnit1->pListNext)
			{
				if (pUnit1->dwType == UNIT_TYPE_NPC)
				{
					if (!IsValidMonster(pUnit1))
						continue;

					if (IsImmune(pUnit1,Spell))
						continue;

					if (!GetUnit(pUnit1->dwUnitId,pUnit1->dwType))
						continue;

					if (pUnit1->dwMode==0||pUnit1->dwMode==13)
						continue;

					INT nUnitDist = GetUnitDist(Myself, pUnit1);

					if(nUnitDist < nDistance && nUnitDist<=50)
					{
						pTarget = pUnit1;
						nDistance = nUnitDist;
					}
				}
			}
		}
	}
	else if (!bMonster)
	{
		for (RosterUnit*pRoster=(RosterUnit*)*(DWORD*)0x6FBCC080;pRoster;pRoster=pRoster->pNext)
		{
			if (pRoster->dwUnitId==Myself->dwUnitId)
				continue;
			if (!IsHostile(pRoster->dwUnitId))
				continue;
			if (!GetUnit(pRoster->dwUnitId,UNIT_TYPE_PLAYER))
				continue;

			UnitAny* pPlayer = GetUnit(pRoster->dwUnitId,UNIT_TYPE_PLAYER);

			if (IsTownLevel(pPlayer->pPath->pRoom1->pRoom2->pLevel->dwLevelNo))
				continue;
			if (pPlayer->dwMode==0||pPlayer->dwMode==17)
				continue;

			INT nUnitDist = GetUnitDist(Myself, pPlayer);

			if(nUnitDist < nDistance && nUnitDist<=50)
			{
				pTarget = pPlayer;
				nDistance = nUnitDist;
			}
		}

		if (pTarget!=NULL)
		{
			AttackPlayer(pTarget->dwUnitId,UNIT_TYPE_PLAYER);
			return TRUE;
		}
	}

	if(pTarget == NULL || nDistance==9999)
		return FALSE;

	AttackStruct* Attack = new AttackStruct;

	Attack->dwAttackType = LeftAA ? ATTACKTYPE_SHIFTLEFT : ATTACKTYPE_SHIFTRIGHT;

	if (SkillID!=D2S_TELEPORT)
	{
		Attack->dwTargetX = pTarget->pPath->xPos;
		Attack->dwTargetY = pTarget->pPath->yPos;
	}
	else 
	{
		if (pTarget->dwMode==PLAYER_MODE_RUN)
		{
			Attack->dwTargetX = pTarget->pObjectPath->dwPosX;
			Attack->dwTargetY = pTarget->pObjectPath->dwPosY;
		}
		else
		{
			Attack->dwTargetX = pTarget->pPath->xPos;
			Attack->dwTargetY = pTarget->pPath->yPos;
		}
	}

	if(Spell.dwSpellInfoFlag & DSI_GUIDED)
	{
		Attack->dwAttackType = LeftAA ? ATTACKTYPE_UNITLEFT : ATTACKTYPE_RIGHT;
		Attack->lpTargetUnit = GetUnit(pTarget->dwUnitId, pTarget->dwType);
	}
	else
	{
		Attack->lpTargetUnit = GetUnit(pTarget->dwUnitId, pTarget->dwType);
	}

	Attack->lpPlayerUnit = Myself;

	Attack->_1 = 3;
	Attack->_2 = 4;

	D2CLIENT_Attack(Attack,TRUE);

	delete Attack;

	return TRUE;
}