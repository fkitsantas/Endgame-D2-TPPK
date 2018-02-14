#include "Houdini.h"

DWORD __declspec(naked) __fastcall InitAutomapLayer_STUB(DWORD nLayerNo)
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
CMap::CMap()
{
	m_ActLevels[0] = 1;
	m_ActLevels[1] = 40;
	m_ActLevels[2] = 75;
	m_ActLevels[3] = 103;
	m_ActLevels[4] = 109;
	m_ActLevels[5] = 137;

	memset(m_RevealedActs, 0, sizeof(UCHAR) * 5);
	m_LastLevel = -1;
}

CMap::~CMap()
{
}

VOID CMap::RevealAutomap()
{
	UnitAny* pPlayer = *p_D2CLIENT_PlayerUnit;
	Act* pAct = pPlayer->pAct;
	DWORD dwAct = pPlayer->pAct->dwAct;

	if(m_RevealedActs[dwAct])
		return;

	for(INT i = 0; i < m_LevelExits.GetSize(); i++)
		delete m_LevelExits[i];
	m_LevelExits.RemoveAll();

	for(INT nLevel = m_ActLevels[dwAct]; nLevel < m_ActLevels[dwAct + 1]; nLevel++)
	{
		Level* pLevel = GetLevel(pAct->pMisc, nLevel);

		if(!pLevel)
			break;

		if(!pLevel->pRoom2First)
			D2COMMON_InitLevel(pLevel);

		InitAutomapLayer(nLevel);
		RevealLevel(pLevel);
	}

	InitAutomapLayer(pPlayer->pPath->pRoom1->pRoom2->pLevel->dwLevelNo);
	m_RevealedActs[dwAct] = 1;
}

VOID CMap::RevealLevel(Level* pLevel)
{
	for(Room2* pRoom2 = pLevel->pRoom2First; pRoom2; pRoom2 = pRoom2->pRoom2Next)
		RevealRoom(pRoom2);
}

VOID CMap::RevealRoom(Room2* pRoom2)
{/*
	BOOL bAdded = FALSE;

	if(pRoom2->dwPresetType == 2)
	{
		DWORD dwPresetNo = *pRoom2->pType2Info;

		if(dwPresetNo == 38 || dwPresetNo == 39) return;
		if(dwPresetNo >= 401 && dwPresetNo <= 405) return;
		if(dwPresetNo == 836 || dwPresetNo == 863) return;
	}

	if(!pRoom2->pRoom1)
	{
		AddAutomapRoom(pRoom2);
		bAdded = TRUE;
	}

	RevealRoom1(pRoom2);

	if(bAdded)
		RemoveAutomapRoom(pRoom2);*/
}


VOID CMap::RevealRoom1(Room2* pRoom)
{
	D2CLIENT_RevealAutomapRoom(pRoom->pRoom1, 1, *p_D2CLIENT_AutomapLayer);

	for(PresetUnit* pUnit = pRoom->pPreset; pUnit; pUnit = pUnit->pPresetNext)
	{
		INT nCell = 0, nXOffset = 0, nYOffset = 0;

		if(pUnit->dwType == UNIT_TYPE_NPC)
		{
			if(pUnit->dwTxtFileNo == 256) nCell = 300;
			if(pUnit->dwTxtFileNo == 745) nCell = 745;
		}

		if(pUnit->dwType == UNIT_TYPE_OBJECT)
		{
			nCell = GetUnitCellNumber(pUnit->dwTxtFileNo, pRoom->pLevel->dwLevelNo);
		}

		if(pUnit->dwType == UNIT_TYPE_TILE)
		{
			DWORD dwTargetLevel = GetTileLevelNo(pRoom, pUnit->dwTxtFileNo);
			if(dwTargetLevel)
			{
				for(INT i = 0; i < m_LevelExits.GetSize(); i++)
				{
					if((m_LevelExits[i]->ptPos.x == (pRoom->dwPosX * 5) + pUnit->dwPosX) && 
						(m_LevelExits[i]->ptPos.y == (pRoom->dwPosY * 5) + pUnit->dwPosY))
						return;
				}

				LPCAVEDESC LevelDesc = new CAVEDESC;
				CHAR szLevel[0x40] = "";

				memset(LevelDesc, 0, sizeof(CAVEDESC));
				D2GetMapName((INT)dwTargetLevel, szLevel, 0x40);

				sprintf(LevelDesc->szName, "%s", szLevel);
				LevelDesc->ptPos.x = (pRoom->dwPosX * 5) + pUnit->dwPosX - (8 << 1);
				LevelDesc->ptPos.y = (pRoom->dwPosY * 5) + pUnit->dwPosY - 10;
				LevelDesc->dwLevelNo = pRoom->pLevel->dwLevelNo;
				LevelDesc->dwTargetLevelNo = dwTargetLevel;

				if(dwTargetLevel == pRoom->pLevel->pMisc->dwStaffTombLevel)
					strcat(LevelDesc->szName, "ÿc2*");

				m_LevelExits.Add(LevelDesc);
			}
		}

		if(nCell)
			AddRoomCell(pUnit->dwPosX, pUnit->dwPosY, nCell, pRoom);
	}
}

VOID CMap::AddRoomCell(INT xPos, INT yPos, INT nCell, Room2* pRoom)
{
	if(nCell == 376 && pRoom->pLevel->dwLevelNo == MAP_A4_RIVER_OF_FLAME)
		return;

	AutomapCell* pCell = D2CLIENT_NewAutomapCell();

	xPos += pRoom->dwPosX * 5;
	yPos += pRoom->dwPosY * 5;

	pCell->nCellNo = nCell;
	pCell->xPixel = (((xPos - yPos) * 16) / 10) + 1;
	pCell->yPixel = (((xPos + yPos) * 8) / 10) - 3;
	D2CLIENT_AddAutomapCell(pCell, &((*p_D2CLIENT_AutomapLayer)->pObjects));
}

VOID CMap::AddAutomapRoom(Room2* pRoom2)
{
	UnitAny* pUnit = *p_D2CLIENT_PlayerUnit;
	D2COMMON_AddRoomData(pRoom2->pLevel->pMisc->pAct,
		pRoom2->pLevel->dwLevelNo,
		pRoom2->dwPosX,
		pRoom2->dwPosY,
		pUnit->pPath->pRoom1);
}

VOID CMap::RemoveAutomapRoom(Room2* pRoom2)
{
	UnitAny* pUnit = *p_D2CLIENT_PlayerUnit;
	D2COMMON_RemoveRoomData(pRoom2->pLevel->pMisc->pAct,
		pRoom2->pLevel->dwLevelNo,
		pRoom2->dwPosX,
		pRoom2->dwPosY,
		pUnit->pPath->pRoom1);
}

BOOL CMap::GetCaveExits(LPCAVEDESC *lpLevel, INT nMaxExits)
{
	INT nTotal = 0;

	for(INT i = 0; i < m_LevelExits.GetSize(); i++)
	{
		if(nTotal > nMaxExits)
			return FALSE;

		lpLevel[nTotal] = m_LevelExits[i];
		nTotal++;
	}

	return nTotal;
}

INT CMap::GetUnitCellNumber(DWORD dwTxtFileNo, DWORD dwLevelNo)
{
	if(dwTxtFileNo == 397) return 318;
	if(dwTxtFileNo == 371) return 301;
	if(dwTxtFileNo == 152) return 300;
	if(dwTxtFileNo == 460) return 1468;

	if(dwTxtFileNo == 402 && dwLevelNo == MAP_A2_CANYON_OF_THE_MAGI) return 0;
	if(dwTxtFileNo == 267 && dwLevelNo != MAP_A3_KURAST_DOCKS && dwLevelNo != MAP_A4_THE_PANDEMONIUM_FORTRESS) return 0;
	if(dwTxtFileNo == 376 && dwLevelNo == MAP_A4_RIVER_OF_FLAME) return 376;

	if(dwTxtFileNo > 574)
		return 0;

	ObjectTxt* pTxt = D2COMMON_GetObjectTxt(dwTxtFileNo);
	if(pTxt->nAutoMap == 310)
		return (pTxt->nSubClass & 1) ? 310 : 0;

	return pTxt->nAutoMap;
}

DWORD CMap::GetTileLevelNo(Room2* lpRoom2, DWORD dwTileNo)
{
	for(RoomTile* pRoomTile = lpRoom2->pRoomTiles; pRoomTile; pRoomTile = pRoomTile->pNext)
	{
		if(*(pRoomTile->nNum) == dwTileNo)
			return pRoomTile->pRoom2->pLevel->dwLevelNo;
	}

	return NULL;
}

Level* CMap::GetLevel(ActMisc* pMisc, DWORD dwLevelNo)
{
	for(Level* pLevel = pMisc->pLevelFirst; pLevel; pLevel = pLevel->pNextLevel)
		if (pLevel->dwLevelNo == dwLevelNo)
			return pLevel;

	return D2COMMON_GetLevel(pMisc, dwLevelNo);
}

AutomapLayer2* CMap::InitAutomapLayer(DWORD levelno)
{
	AutomapLayer2* pLayer = D2COMMON_GetLayer(levelno);
	return (AutomapLayer2*)InitAutomapLayer_STUB(pLayer->nLayerNo);
}

BOOL CMap::CreateCollisionMap()
{
	if(GetUnitLevel(Myself)->dwLevelNo == m_LastLevel)
		return TRUE;

	if(!Myself)
		return FALSE;

	if(!Myself->pPath->pRoom1)
		return FALSE;

	Level* pLevel = Myself->pPath->pRoom1->pRoom2->pLevel;
	if(!pLevel)
		return FALSE;

	if(m_Map.IsCreated())
		m_Map.Destroy();

	m_LastLevel = pLevel->dwLevelNo;
	m_LevelOrigin.x = pLevel->dwPosX * 5;
	m_LevelOrigin.y = pLevel->dwPosY * 5;
	m_SizeX = pLevel->dwSizeX * 5;
	m_SizeY = pLevel->dwSizeY * 5;

	m_Map.Lock();
	if(!m_Map.Create(pLevel->dwSizeX * 5, pLevel->dwSizeY * 5, MAP_DATA_NULL))
	{
		m_Map.Unlock();
		return FALSE;
	}

	CArrayEx<DWORD, DWORD> aSkip;
	AddCollisionData(Myself->pPath->pRoom1->pRoom2, aSkip);

	FillGaps();
	FillGaps();

	m_Map.Unlock();
	return TRUE;
}

BOOL CMap::IsGap(INT nX, INT nY)
{
	if(m_Map[nX][nY] % 2)
		return FALSE;

	INT nSpaces = 0;
	INT i = 0;

	for(i = nX - 2; i <= nX + 2 && nSpaces < 3; i++)
	{
		if(i < 0 || i >= m_Map.GetCX() || (m_Map[i][nY] % 2))
			nSpaces = 0;
		else
			nSpaces++;
	}

	if(nSpaces < 3)
		return TRUE;

	nSpaces = 0;
	for(i = nY - 2; i <= nY + 2 && nSpaces < 3; i++)
	{
		if(i < 0 || i >= m_Map.GetCY() || (m_Map[nX][i] % 2))
			nSpaces = 0;
		else
			nSpaces++;
	}

	return nSpaces < 3;
}

BOOL CMap::FillGaps()
{
	if(!m_Map.IsCreated())
		return FALSE;

	INT nCX = m_Map.GetCX();
	INT nCY = m_Map.GetCY();

	for(INT x = 0; x < nCX; x++)
	{
		for(INT y = 0; y < nCY; y++)
		{
			if(IsGap(x, y))
				m_Map[x][y] = MAP_DATA_FILL;
		}
	}

	return TRUE;
}

BOOL CMap::AddCollisionData(CollMap* pColl)
{
	if(!pColl)
		return FALSE;

	INT nX = pColl->dwPosGameX - m_LevelOrigin.x;
	INT nY = pColl->dwPosGameY - m_LevelOrigin.y;
	INT nCX = pColl->dwSizeGameX;
	INT nCY = pColl->dwSizeGameY;

	if(!m_Map.IsValidIndex(nX, nY))
		return FALSE;

	INT nLimitX = nX + nCX;
	INT nLimitY = nY + nCY;

	LPWORD p = pColl->pMapStart;
	if(!p)
		return FALSE;

	for(INT y = nY; y < nLimitY; y++)
	{
		for(INT x = nX; x < nLimitX; x++)
		{
			m_Map[x][y] = *p;

			if(m_Map[x][y] == 1024)
				m_Map[x][y] = MAP_DATA_AVOID;

			p++;
		}
	}

	return TRUE;
}

BOOL CMap::AddCollisionData(Room2* pRoom2, CArrayEx<DWORD, DWORD>& aSkip)
{
	if(!pRoom2)
		return FALSE;

	if(pRoom2->pLevel->dwLevelNo != m_LastLevel)
		return FALSE;

	if(aSkip.Find((DWORD)pRoom2) != -1)
		return TRUE;

	aSkip.Add((DWORD)pRoom2);

	BOOL bAdd = FALSE;
	if(!pRoom2->pRoom1)
	{
		AddAutomapRoom(pRoom2);
		bAdd = TRUE;
	}

	if(pRoom2->pRoom1)
		AddCollisionData(pRoom2->pRoom1->Coll);

	Room2* *pRoomNear = pRoom2->pRoom2Near;
	for(DWORD i = 0; i < pRoom2->dwRoomsNear; i++)
		AddCollisionData(pRoomNear[i], aSkip);

	if(bAdd)
		RemoveAutomapRoom(pRoom2);

	return TRUE;
}

BOOL CMap::GetLevelExits(LPLEVELEXIT *lpLevel, INT nMaxExits)
{
	LPCAVEDESC pCaves[0x40] = {NULL};
	POINT ptExitPoints[0x40][2];
	INT nTotalPoints = 0, nCurrentExit = 0;

	GetCaveExits(pCaves, 0x40);
	for(INT i = 0; i < 0x40; i++)
	{
		if(!pCaves[i])
			break;

		if(pCaves[i]->dwLevelNo == GetUnitLevel(Myself)->dwLevelNo)
		{
			if(nCurrentExit >= nMaxExits)
				return FALSE;

			lpLevel[nCurrentExit] = new LEVELEXIT;
			lpLevel[nCurrentExit]->dwTargetLevel = pCaves[i]->dwTargetLevelNo;
			lpLevel[nCurrentExit]->ptPos.x = pCaves[i]->ptPos.x + (8 << 1);
			lpLevel[nCurrentExit]->ptPos.y = pCaves[i]->ptPos.y + 10;
			nCurrentExit++;
		}
	}
	for(INT i = 0; i < m_Map.GetCX(); i++)
	{
		if(!(m_Map[i][0] % 2))
		{
			ptExitPoints[nTotalPoints][0].x = i;
			ptExitPoints[nTotalPoints][0].y = 0;

			for(i++; i < m_Map.GetCX(); i++)
			{
				if(m_Map[i][0] % 2)
				{
					ptExitPoints[nTotalPoints][1].x = i - 1;
					ptExitPoints[nTotalPoints][1].y = 0;
					break;
				}
			}

			nTotalPoints++;
			break;
		}
	}

	for(INT i = 0; i < m_Map.GetCX(); i++)
	{
		if(!(m_Map[i][m_Map.GetCY() - 1] % 2))
		{
			ptExitPoints[nTotalPoints][0].x = i;
			ptExitPoints[nTotalPoints][0].y = m_Map.GetCY() - 1;

			for(i++; i < m_Map.GetCX(); i++)
			{
				if((m_Map[i][m_Map.GetCY() - 1] % 2))
				{
					ptExitPoints[nTotalPoints][1].x = i - 1;
					ptExitPoints[nTotalPoints][1].y = m_Map.GetCY() - 1;
					break;
				}
			}

			nTotalPoints++;
			break;
		}
	}

	for(INT i = 0; i < m_Map.GetCY(); i++)
	{
		if(!(m_Map[0][i] % 2))
		{
			ptExitPoints[nTotalPoints][0].x = 0;
			ptExitPoints[nTotalPoints][0].y = i;

			for(i++; i < m_Map.GetCY(); i++)
			{
				if((m_Map[0][i] % 2))
				{
					ptExitPoints[nTotalPoints][1].x = 0;
					ptExitPoints[nTotalPoints][1].y = i - 1;
					break;
				}
			}

			nTotalPoints++;
			break;
		}
	}

	for(INT i = 0; i < m_Map.GetCY(); i++)
	{
		if(!(m_Map[m_Map.GetCX() - 1][i] % 2))
		{
			ptExitPoints[nTotalPoints][0].x = m_Map.GetCX() - 1;
			ptExitPoints[nTotalPoints][0].y = i;

			for(i++; i < m_Map.GetCY(); i++)
			{
				if((m_Map[m_Map.GetCX() - 1][i] % 2))
				{
					ptExitPoints[nTotalPoints][1].x = m_Map.GetCX() - 1;
					ptExitPoints[nTotalPoints][1].y = i - 1;
					break;
				}
			}

			nTotalPoints++;
			break;
		}
	}

	LPPOINT ptCenters = new POINT[nTotalPoints];
	for(INT i = 0; i < nTotalPoints; i++)
	{
		INT nXDiff = ptExitPoints[i][1].x - ptExitPoints[i][0].x;
		INT nYDiff = ptExitPoints[i][1].y - ptExitPoints[i][0].y;
		INT nXCenter = 0, nYCenter = 0;

		if(nXDiff > 0)
		{
			if(nXDiff % 2)
				nXCenter = ptExitPoints[i][0].x + ((nXDiff - (nXDiff % 2)) / 2);
			else
				nXCenter = ptExitPoints[i][0].x + (nXDiff / 2);
		}

		if(nYDiff > 0)
		{
			if(nYDiff % 2)
				nYCenter = ptExitPoints[i][0].y + ((nYDiff - (nYDiff % 2)) / 2);
			else
				nYCenter = ptExitPoints[i][0].y + (nYDiff / 2);
		}

		ptCenters[i].x = nXCenter ? nXCenter : ptExitPoints[i][0].x;
		ptCenters[i].y = nYCenter ? nYCenter : ptExitPoints[i][0].y;
	}


	for(Room2* pRoom = GetUnitLevel(Myself)->pRoom2First; pRoom; pRoom = pRoom->pRoom2Other)
	{
		Room2 **pNear = pRoom->pRoom2Near;
		for(DWORD i = 0; i < pRoom->dwRoomsNear; i++)
		{
			if(pNear[i]->pLevel->dwLevelNo != GetUnitLevel(Myself)->dwLevelNo)
			{
				INT nRoomX = pRoom->dwPosX * 5;
				INT nRoomY = pRoom->dwPosY * 5;

				for(INT j = 0; j < nTotalPoints; j++)
				{
					if((ptCenters[j].x + m_LevelOrigin.x) >= (WORD)nRoomX && (ptCenters[j].x + m_LevelOrigin.x) <= (WORD)(nRoomX + (pRoom->dwSizeX * 5)))
					{
						if((ptCenters[j].y + m_LevelOrigin.y) >= (WORD)nRoomY && (ptCenters[j].y + m_LevelOrigin.y) <= (WORD)(nRoomY + (pRoom->dwSizeY * 5)))
						{
							if(nCurrentExit >= nMaxExits)
							{
								return FALSE;
							}
							
							lpLevel[nCurrentExit] = new LEVELEXIT;
							lpLevel[nCurrentExit]->dwTargetLevel = pNear[i]->pLevel->dwLevelNo;
							lpLevel[nCurrentExit]->ptPos.x = ptCenters[j].x + m_LevelOrigin.x;
							lpLevel[nCurrentExit]->ptPos.y = ptCenters[j].y + m_LevelOrigin.y;
							nCurrentExit++;
						}
					}
				}

				break;
			}
		}

		BOOL bAdded = FALSE;

		if(!pRoom->pRoom1)
		{
			D2COMMON_AddRoomData(Myself->pAct, pRoom->pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, Myself->pPath->pRoom1);
			bAdded = TRUE;
		}
			
		for(PresetUnit* pUnit = pRoom->pPreset; pUnit; pUnit = pUnit->pPresetNext)
		{

			if(nCurrentExit >= nMaxExits)
			{
				if(bAdded)
					D2COMMON_RemoveRoomData(Myself->pAct, pRoom->pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, Myself->pPath->pRoom1);
				return FALSE;
			}

			if(pUnit->dwType == UNIT_TILE)
			{

				DWORD dwTargetLevel = GetTileLevelNo(pRoom, pUnit->dwTxtFileNo);

				if(dwTargetLevel)
				{
					BOOL bExists = FALSE;

					for(INT i = 0; i < nCurrentExit; i++)
						{
							if((lpLevel[i]->ptPos.x == (pRoom->dwPosX * 5) + pUnit->dwPosX) && 
								(lpLevel[i]->ptPos.y == (pRoom->dwPosY * 5) + pUnit->dwPosY))
								bExists = TRUE;
						}

					if(!bExists)
					{
						lpLevel[nCurrentExit] = new LEVELEXIT;
						lpLevel[nCurrentExit]->dwTargetLevel = dwTargetLevel;
						lpLevel[nCurrentExit]->ptPos.x = (pRoom->dwPosX * 5) + pUnit->dwPosX;
						lpLevel[nCurrentExit]->ptPos.y = (pRoom->dwPosY * 5) + pUnit->dwPosY;
						nCurrentExit++;
					}
				}
			}
		}

		if(bAdded)
		D2COMMON_RemoveRoomData(Myself->pAct, pRoom->pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, Myself->pPath->pRoom1);
		}
	return TRUE;


	//LPCAVEDESC pCaves[0x40] = {NULL};
	//POINT ptExitPoints[0x40][2];
	//INT nTotalPoints = 0, nCurrentExit = 0;

	//GetCaveExits(pCaves, 0x40);
	//for(INT i = 0; i < 0x40; i++)
	//{
	//	if(!pCaves[i])
	//		break;

	//	if(pCaves[i]->dwLevelNo == GetUnitLevel(Myself)->dwLevelNo)
	//	{
	//		if(nCurrentExit >= nMaxExits)
	//			return FALSE;

	//		lpLevel[nCurrentExit] = new LEVELEXIT;
	//		lpLevel[nCurrentExit]->dwTargetLevel = pCaves[i]->dwTargetLevelNo;
	//		lpLevel[nCurrentExit]->ptPos.x = pCaves[i]->ptPos.x + (8 << 1);
	//		lpLevel[nCurrentExit]->ptPos.y = pCaves[i]->ptPos.y + 10;
	//		nCurrentExit++;
	//	}
	//}

	//for(INT i = 0; i < m_Map.GetCX(); i++)
	//{
	//	if(!(m_Map[i][0] % 2))
	//	{
	//		ptExitPoints[nTotalPoints][0].x = i;
	//		ptExitPoints[nTotalPoints][0].y = 0;

	//		for(i++; i < m_Map.GetCX(); i++)
	//		{
	//			if(m_Map[i][0] % 2)
	//			{
	//				ptExitPoints[nTotalPoints][1].x = i - 1;
	//				ptExitPoints[nTotalPoints][1].y = 0;
	//				break;
	//			}
	//		}

	//		nTotalPoints++;
	//		break;
	//	}
	//}

	//for(INT i = 0; i < m_Map.GetCX(); i++)
	//{
	//	if(!(m_Map[i][m_Map.GetCY() - 1] % 2))
	//	{
	//		ptExitPoints[nTotalPoints][0].x = i;
	//		ptExitPoints[nTotalPoints][0].y = m_Map.GetCY() - 1;

	//		for(i++; i < m_Map.GetCX(); i++)
	//		{
	//			if((m_Map[i][m_Map.GetCY() - 1] % 2))
	//			{
	//				ptExitPoints[nTotalPoints][1].x = i - 1;
	//				ptExitPoints[nTotalPoints][1].y = m_Map.GetCY() - 1;
	//				break;
	//			}
	//		}

	//		nTotalPoints++;
	//		break;
	//	}
	//}

	//for(INT i = 0; i < m_Map.GetCY(); i++)
	//{
	//	if(!(m_Map[0][i] % 2))
	//	{
	//		ptExitPoints[nTotalPoints][0].x = 0;
	//		ptExitPoints[nTotalPoints][0].y = i;

	//		for(i++; i < m_Map.GetCY(); i++)
	//		{
	//			if((m_Map[0][i] % 2))
	//			{
	//				ptExitPoints[nTotalPoints][1].x = 0;
	//				ptExitPoints[nTotalPoints][1].y = i - 1;
	//				break;
	//			}
	//		}

	//		nTotalPoints++;
	//		break;
	//	}
	//}

	//for(INT i = 0; i < m_Map.GetCY(); i++)
	//{
	//	if(!(m_Map[m_Map.GetCX() - 1][i] % 2))
	//	{
	//		ptExitPoints[nTotalPoints][0].x = m_Map.GetCX() - 1;
	//		ptExitPoints[nTotalPoints][0].y = i;

	//		for(i++; i < m_Map.GetCY(); i++)
	//		{
	//			if((m_Map[m_Map.GetCX() - 1][i] % 2))
	//			{
	//				ptExitPoints[nTotalPoints][1].x = m_Map.GetCX() - 1;
	//				ptExitPoints[nTotalPoints][1].y = i - 1;
	//				break;
	//			}
	//		}

	//		nTotalPoints++;
	//		break;
	//	}
	//}

	//LPPOINT ptCenters = new POINT[nTotalPoints];
	//for(INT i = 0; i < nTotalPoints; i++)
	//{
	//	INT nXDiff = ptExitPoints[i][1].x - ptExitPoints[i][0].x;
	//	INT nYDiff = ptExitPoints[i][1].y - ptExitPoints[i][0].y;
	//	INT nXCenter = 0, nYCenter = 0;

	//	if(nXDiff > 0)
	//	{
	//		if(nXDiff % 2)
	//			nXCenter = ptExitPoints[i][0].x + ((nXDiff - (nXDiff % 2)) / 2);
	//		else
	//			nXCenter = ptExitPoints[i][0].x + (nXDiff / 2);
	//	}

	//	if(nYDiff > 0)
	//	{
	//		if(nYDiff % 2)
	//			nYCenter = ptExitPoints[i][0].y + ((nYDiff - (nYDiff % 2)) / 2);
	//		else
	//			nYCenter = ptExitPoints[i][0].y + (nYDiff / 2);
	//	}

	//	ptCenters[i].x = nXCenter ? nXCenter : ptExitPoints[i][0].x;
	//	ptCenters[i].y = nYCenter ? nYCenter : ptExitPoints[i][0].y;
	//}

	//for(Room2* pRoom = GetUnitLevel(Myself)->pRoom2First; pRoom; pRoom = pRoom->pRoom2Other)
	//{
	//	Room2* *pNear = pRoom->pRoom2Near;
	//	for(DWORD i = 0; i < pRoom->dwRoomsNear; i++)
	//	{
	//		if(pNear[i]->pLevel->dwLevelNo != GetUnitLevel(Myself)->dwLevelNo)
	//		{
	//			INT nRoomX = pRoom->dwPosX * 5;//((pRoom->pLevel->dwPosX * 5) + pRoom->dwPosX);
	//			INT nRoomY = pRoom->dwPosY * 5;//((pRoom->pLevel->dwPosY * 5) + pRoom->dwPosY);

	//			for(INT j = 0; j < nTotalPoints; j++)
	//			{
	//				if((ptCenters[j].x + m_LevelOrigin.x) >= (WORD)nRoomX && (ptCenters[j].x + m_LevelOrigin.x) <= (WORD)(nRoomX + (pRoom->dwSizeX * 5)))
	//				{
	//					if((ptCenters[j].y + m_LevelOrigin.y) >= (WORD)nRoomY && (ptCenters[j].y + m_LevelOrigin.y) <= (WORD)(nRoomY + (pRoom->dwSizeY * 5)))
	//					{
	//						if(nCurrentExit >= nMaxExits)
	//							return FALSE;

	//						lpLevel[nCurrentExit] = new LEVELEXIT;
	//						lpLevel[nCurrentExit]->dwTargetLevel = pNear[i]->pLevel->dwLevelNo;
	//						lpLevel[nCurrentExit]->ptPos.x = ptCenters[j].x + m_LevelOrigin.x;
	//						lpLevel[nCurrentExit]->ptPos.y = ptCenters[j].y + m_LevelOrigin.y;
	//						nCurrentExit++;
	//					}
	//				}
	//			}

	//			break;
	//		}
	//	}
	//}

	//return TRUE;
}

WORD CMap::GetCollisionInfo(INT nX, INT nY)
{
	nX -= m_LevelOrigin.x;
	nY -= m_LevelOrigin.y;

	if(!m_Map.IsValidIndex(nX, nY))
		return NULL;

	return m_Map[nX][nY];
}

BOOL CMap::ExportCollisionMap(CMatrix<WORD, WORD>& cMatrix)
{
	m_Map.Lock();
	m_Map.ExportData(cMatrix);
	m_Map.Unlock();

	return TRUE;
}