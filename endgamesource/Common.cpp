#include "Houdini.h"

bool GetPlayerRoster(RosterUnit* pUnit, DWORD nUnitId)
{
	for (RosterUnit* pUnit = D2CLIENT_PlayerList; pUnit; pUnit = pUnit->pNext)
		if (pUnit->dwUnitId == nUnitId)
			return true;
	return false;
}
int GetPvPStatus(DWORD nUnitId)
{
	RosterUnit* pUnit;
	for (pUnit = D2CLIENT_PlayerList; pUnit; pUnit = pUnit->pNext)
		if (pUnit->dwUnitId == nUnitId)
			break;

	if (!pUnit)
		return PVP_HOSTILE;

	UnitAny* mypUnit = D2CLIENT_GetPlayerUnit ();
	
	RosterUnit* prPlayer;
	for (prPlayer = D2CLIENT_PlayerList; prPlayer; prPlayer = prPlayer->pNext)
		if (prPlayer->dwUnitId == mypUnit->dwUnitId)
			break;
		

	if (TestPvpFlag(nUnitId, mypUnit->dwUnitId, 8) || TestPvpFlag(mypUnit->dwUnitId, nUnitId, 8))
		return PVP_HOSTILE;

	if (pUnit->dwPartyFlags & PARTY_IN_PARTY && prPlayer->wPartyId == pUnit->wPartyId)
		return PVP_FRIENDLY;

	return PVP_NEUTRAL;
}