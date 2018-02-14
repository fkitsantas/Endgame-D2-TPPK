#include "Houdini.h"
#include "D2Helpers.h"
DWORD PlayerRoster=0x11C080;

DWORD  __declspec(naked) __fastcall D2CLIENT_CheckLoot(DWORD pUnit)
{
	__asm
	{
		mov eax, ecx
		call D2CLIENT_CheckLootASM
		retn
	}
}
RosterUnit* FindPartyById(DWORD dwUnitId)
{
	for(RosterUnit* pUnit = (RosterUnit*)*(DWORD*)PlayerRoster; pUnit; pUnit = pUnit->pNext)
		if(pUnit->dwUnitId == dwUnitId)
			return pUnit;
		_asm nop
		_asm fnop
	return NULL;
}
DWORD GetPvpFlags(DWORD dwPlayerId)
{
	DWORD dwFlags = 0;
	UnitAny* Me=(UnitAny*)D2CLIENT_GetPlayerUnit();
	for(RosterUnit* pUnit = (RosterUnit*)*(DWORD*)PlayerRoster; pUnit; pUnit = pUnit->pNext)
	{
		if(pUnit->dwUnitId != dwPlayerId)
		continue;
		if(pUnit->dwUnitId != Me->dwUnitId)
		{
			if(TestPvpFlag(dwPlayerId, Me->dwUnitId, 8))
			dwFlags |= PVP_HOSTILED_YOU;
			if(TestPvpFlag(Me->dwUnitId, dwPlayerId, 8))
			dwFlags |= PVP_HOSTILED_BY_YOU;
			if(pUnit->dwPartyFlags == 2)
			dwFlags |= PVP_INVITED_YOU;
			if(pUnit->dwPartyFlags == 4)
			dwFlags |= PVP_INVITED_BY_YOU;
			if(pUnit->dwPartyFlags == 20)
			dwFlags |= PVP_ALLIED_WITH_YOU;
			if(pUnit->wPartyId != 0xFFFF && pUnit->dwPartyFlags == 1)
			{
				RosterUnit* pMe = FindPartyById(Me->dwUnitId);
				if(pMe->wPartyId == 0xFFFF)
				dwFlags |= PVP_ALLIED;
				else if(pMe->wPartyId == pUnit->wPartyId)
				dwFlags |= PVP_ALLIED;
			}
			return dwFlags;
		}
	}
	return dwFlags;
}
