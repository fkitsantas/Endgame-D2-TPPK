#include "Houdini.h"
#include "Vars.h"
#include "D2Structs.h"

Level* GetLevelPointer(ActMisc *pActMisc, int nLevel)
{
	if ((!pActMisc) || (nLevel < 0))
		return false;

	for (Level *pLevel = pActMisc->pLevelFirst; pLevel; pLevel = pLevel->pNextLevel)
	{
		if (!pLevel)
			continue;

		if ((pLevel->dwLevelNo == nLevel) && (pLevel->dwSizeX > 0))
			return pLevel;
	}
	return D2COMMON_GetLevel(pActMisc, nLevel);
}
bool RevealLevel(Level *pLevel)
{
	if (!pLevel)
		return false;

	if (!InitAutomapLayer(pLevel->dwLevelNo))
		return false;

	UnitAny *pUnit = D2CLIENT_GetPlayerUnit ();
	if (!pUnit)
		return false;

	for (Room2 *pRoom2 = pLevel->pRoom2First; pRoom2; pRoom2 = pRoom2->pRoom2Other)
	{
		bool nAdded = false;

		if (!pRoom2->pRoom1)
		{
			D2COMMON_AddRoomData(pLevel->pMisc->pAct,pLevel->dwLevelNo, pRoom2->dwPosX, pRoom2->dwPosY, pUnit->pPath->pRoom1);
			nAdded = true;
		}

		if (!pRoom2->pRoom1)
			continue;


		if(cfg.Maphack)
		{
			D2CLIENT_RevealAutomapRoom(pRoom2->pRoom1, TRUE, (*p_D2CLIENT_AutomapLayer));
		}

		DrawPresets(pRoom2);

		if (nAdded)
			D2COMMON_RemoveRoomData(pLevel->pMisc->pAct, pLevel->dwLevelNo, pRoom2->dwPosX, pRoom2->dwPosY, D2CLIENT_GetPlayerUnit()->pPath->pRoom1);
	}

	InitAutomapLayer(pUnit->pPath->pRoom1->pRoom2->pLevel->dwLevelNo);

	return true;
}


BOOL RevealAct ()
{
	UnitAny *pUnit = D2CLIENT_GetPlayerUnit ();
	if (!pUnit)
		return false;

	int townLevels[6] = {1, 40, 75, 103, 109, 132};
	
	for (int nLevel = townLevels[pUnit->dwAct] + 1; nLevel < townLevels[pUnit->dwAct + 1]; nLevel++)
	{
		Level *pLevel = GetLevelPointer(pUnit->pAct->pMisc, nLevel);

		if(!pLevel)
			continue;

		if(!pLevel->pRoom2First)
			D2COMMON_InitLevel(pLevel);

		if (!pLevel->pRoom2First)
			continue;

		RevealLevel(pLevel);
	}
	return true;
}
DWORD __declspec(naked) __fastcall D2CLIENT_InitAutomapLayer_STUB(DWORD nLayerNo)
{
	__asm 
	{
		push eax;
		mov eax, ecx;
		call D2CLIENT_InitAutomapLayer_I;
		pop eax;
		ret;
	}
}

AutomapLayer* InitAutomapLayer(DWORD levelno)
{
	AutomapLayer2 *pLayer = D2COMMON_GetLayer(levelno);
	if (!pLayer)
		return false;
	return D2CLIENT_InitAutomapLayer(pLayer->nLayerNo);
}


BOOL GameReady(BOOL DeathCheck)
{
	if(DeathCheck)
	{
		if(D2CLIENT_GetPlayerUnit())
		{
			if(	D2CLIENT_GetPlayerUnit()->dwMode == PLAYER_MODE_DEAD ||
				D2CLIENT_GetPlayerUnit()->dwMode == PLAYER_MODE_DEATH )
				return FALSE;
		}
	}


	if(D2CLIENT_GetPlayerUnit())
		if(D2CLIENT_GetPlayerUnit()->pInventory)
			if(D2CLIENT_GetPlayerUnit()->pPath)
				if(D2CLIENT_GetPlayerUnit()->pPath->xPos)
					if(D2CLIENT_GetPlayerUnit()->pPath->pRoom1)
						if(D2CLIENT_GetPlayerUnit()->pPath->pRoom1->pRoom2)
							if(D2CLIENT_GetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel)
								if (D2CLIENT_GetPlayerUnit()->pAct)
									if(GetPlayerArea() != NULL)
										return TRUE;
	return FALSE;
}


void DrawPresets (Room2 *pRoom2)
{
	UnitAny *pPlayer = D2CLIENT_GetPlayerUnit ();

	for (PresetUnit *pUnit = pRoom2->pPreset; pUnit; pUnit = pUnit->pPresetNext)
	{
		int mCell = -1;
		if (pUnit->dwType == 1)
		{
			if (pUnit->dwTxtFileNo == 256)
				mCell = 300;
			if (pUnit->dwTxtFileNo == 745)
				mCell = 745;
		}
		else if (pUnit->dwType == 2)
		{
			if (pUnit->dwTxtFileNo == 580 && pRoom2->pLevel->dwLevelNo == 79)		
				mCell = 318;
				
			if (pUnit->dwTxtFileNo == 371) 
				mCell = 301;

			if (pUnit->dwTxtFileNo == 152) 
				mCell = 300;
			if (pUnit->dwTxtFileNo == 460) 
				mCell = 1468;
			if ((pUnit->dwTxtFileNo == 402) && (pRoom2->pLevel->dwLevelNo == 46))
				mCell = 0;
			if ((pUnit->dwTxtFileNo == 267) && (pRoom2->pLevel->dwLevelNo != 75) && (pRoom2->pLevel->dwLevelNo != 103))
				mCell = 0;
			if ((pUnit->dwTxtFileNo == 376) && (pRoom2->pLevel->dwLevelNo == 107))
				mCell = 376;
			if (mCell == -1)
			{
				if(pUnit->dwTxtFileNo > 574)
					mCell = pUnit->dwTxtFileNo - 574;
				else
				{
					ObjectTxt *obj = D2COMMON_GetObjectTxt(pUnit->dwTxtFileNo);

					if (mCell == -1)
					{
						if(obj->nAutoMap == 310)
							mCell = (obj->nSubClass & 1) ? 310 : 0;
						else if(mCell == -1)
							mCell = obj->nAutoMap;
					}
				}
			}
		} 
		else if (pUnit->dwType == 5)
		{
			LevelNameInfo *pInfo = new LevelNameInfo;
			for (RoomTile *pTile = pRoom2->pRoomTiles; pTile; pTile = pTile->pNext)
			{
				if (*(pTile->nNum) == pUnit->dwTxtFileNo)
				{
					pInfo->nLevelId = pTile->pRoom2->pLevel->dwLevelNo;
					break;
				}
			}
			pInfo->nAct = pPlayer->dwAct;
			pInfo->nX = (pUnit->dwPosX + (pRoom2->dwPosX * 5) - (8 << 1));
			pInfo->nY = (pUnit->dwPosY + (pRoom2->dwPosY * 5) - 10);
			LevelNames.Add(pInfo); 
		}

		if ((mCell > 0) && (mCell < 1258))
		{
			AutomapCell *pCell = D2CLIENT_NewAutomapCell();
			pCell->nCellNo = mCell;
			int pX = (pUnit->dwPosX + (pRoom2->dwPosX * 5));
			int pY = (pUnit->dwPosY + (pRoom2->dwPosY * 5));
			pCell->xPixel = (((pX - pY) * 16) / 10) + 1;
			pCell->yPixel = (((pY + pX) * 8) / 10) - 3;

			if(cfg.Maphack)
			D2CLIENT_AddAutomapCell(pCell, &((*p_D2CLIENT_AutomapLayer)->pObjects));
		}
	}
}