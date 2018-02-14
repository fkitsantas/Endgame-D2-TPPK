#include "Houdini.h"

POINT ptPath[200];

BOOL PathToVector(INT nVectorType)
{
	POINT ptStart, ptEnd = {0};
	LPVECTOR lpTarget = NULL;
	INT nCount = 0;
	DWORD dwLevelX, dwLevelY;
	CMatrix<WORD, WORD> GMap;
	CPathFinder PathFinder;
	CMap g_collisionMap;

	if(v_TeleportQueue.IsLocked || !v_TeleportQueue.IsEmpty())
		return FALSE;

	for(INT i = 0; i < v_CurrentVectors.GetSize(); i++)
	{
		if(v_CurrentVectors[i]->nType == nVectorType)
		{
			lpTarget = v_CurrentVectors[i];
			break;
		}
	}

	if(!lpTarget)
		return FALSE;

	v_GlobalMap->CreateCollisionMap();
	v_GlobalMap->ExportCollisionMap(GMap);

	dwLevelX = GetUnitLevel(Myself)->dwPosX * 5;
	dwLevelY = GetUnitLevel(Myself)->dwPosY * 5;

	ptStart.x = Myself->pPath->xPos - dwLevelX;
	ptStart.y = Myself->pPath->yPos - dwLevelY;

	if(!GMap.IsValidIndex(ptStart.x, ptStart.y))
	{
		return FALSE;
	}

	ptEnd.x = (lpTarget->ptPos.x / 32) - dwLevelX;
	ptEnd.y = (lpTarget->ptPos.y / 32) - dwLevelY;

	if(!GMap.IsValidIndex(ptEnd.x, ptEnd.y))
	{
		return FALSE;
	}

	PathFinder.SetPathingMap(GMap.GetData(), GMap.GetCX(), GMap.GetCY());
	nCount = PathFinder.CreateBestPath(ptStart, ptEnd, ptPath, 200);

	if(nCount == 0)
	{
		return FALSE;
	}

	for(INT i = 1; i < nCount; i++)
	{
		ptPath[i].x += dwLevelX;
		ptPath[i].y += dwLevelY;

		v_TeleportQueue.Add(ptPath[i]);
	}

	v_CurrentVector = lpTarget;

	v_TeleportThread=DriverCreateThread((LPVOID)TickThread, (LPVOID)THREAD_TELEPORT);

	PrintConsoleString("ÿc9Starting auto-teleport sequence, nodes: %d", v_TeleportQueue.GetSize());

	return TRUE;
}

CPathFinder::CPathFinder()
{
	m_pMap = NULL;
	m_nSizeX = 0;
	m_nSizeY = 0;
	memset(&m_ptStart, 0, sizeof(POINT));
	memset(&m_ptEnd, 0, sizeof(POINT));
}

CPathFinder::~CPathFinder()
{
}

BOOL CPathFinder::CreateBestPath(POINT ptStart, POINT ptEnd, LPPOINT lpBuffer, DWORD dwMaxCount)
{
	POINT ptPos;
	INT nFlag, nIterations = 0;
	DWORD dwFound;

	if(lpBuffer == NULL || dwMaxCount == 0 || m_nSizeX <= 0 || m_nSizeY <= 0 || m_pMap == NULL)
		return 0;

	if(ptStart.x > m_nSizeX || ptStart.y > m_nSizeY || ptEnd.x > m_nSizeX || ptEnd.y > m_nSizeY)
		return 0;

	memset(lpBuffer, 0, sizeof(POINT) * dwMaxCount);
	m_ptStart = ptStart;
	m_ptEnd = ptEnd;

	CreateDistanceTable();

	lpBuffer[0] = ptStart;
	dwFound = 1;
	ptPos = ptStart;

	nFlag = GetNextMove(ptPos);
	while(nFlag != PATH_FAILED && dwFound < dwMaxCount)
	{
		if(nIterations > 5000)
			return 0;

		if(nFlag == PATH_REACHED)
		{
			lpBuffer[dwFound] = ptEnd;
			dwFound++;
			return dwFound;
		}

		int nRedundancy = CheckRedundancy(lpBuffer, dwFound, ptPos);
		if(nRedundancy == -1)
		{
			lpBuffer[dwFound] = ptPos;
			dwFound++;
		}
		else
		{
			dwFound = nRedundancy + 1;
			lpBuffer[dwFound] = ptPos;
		}	

		nFlag = GetNextMove(ptPos);
		nIterations++;
	}

	return FALSE;
}

VOID CPathFinder::SetPathingMap(LPWORD *pMap, INT nSizeX, INT nSizeY)
{
	m_pMap = pMap;
	m_nSizeX = nSizeX;
	m_nSizeY = nSizeY;
}

PATHRETURN CPathFinder::GetNextMove(POINT& pos, int nAdjust)
{	
	if(CalculateDistance(m_ptEnd, pos) <= TELEPORT_RANGE)
	{
		pos = m_ptEnd;
		return PATH_REACHED;
	}

	if (!IsValidIndex(pos.x, pos.y))
		return PATH_FAILED;

	BlockLocation(pos, nAdjust);

	POINT p, best;
	int value = RANGE_INVALID;

	for (p.x = pos.x - TELEPORT_RANGE; p.x <= pos.x + TELEPORT_RANGE; p.x++)
	{
		for (p.y = pos.y - TELEPORT_RANGE; p.y <= pos.y + TELEPORT_RANGE; p.y++)
		{			
			if (!IsValidIndex(p.x, p.y))
				continue;

			if (m_pMap[p.x][p.y] < value && CalculateDistance(p, pos) <= TELEPORT_RANGE)
			{				
				value = m_pMap[p.x][p.y];
				best = p;					
			}			
		}
	}

	if (value >= RANGE_INVALID)
		return PATH_FAILED;

	pos = best;
	BlockLocation(pos, nAdjust);	
	return PATH_CONTINUE;
}

BOOL CPathFinder::CreateDistanceTable()
{
	if(m_pMap == NULL)
		return FALSE;

	for (int x = 0; x < m_nSizeX; x++)	
	{
		for (int y = 0; y < m_nSizeY; y++)
		{
			if ((m_pMap[x][y] % 2) == 0)
				m_pMap[x][y] = (SHORT)CalculateDistance(x, y, m_ptEnd.x, m_ptEnd.y);
			else
				m_pMap[x][y] = RANGE_INVALID;
		}
	}

	m_pMap[m_ptEnd.x][m_ptEnd.y] = 1;	
	return TRUE;
}

BOOL CPathFinder::CheckRedundancy(LPPOINT lpPath, INT nCount, POINT ptPos)
{
	if(lpPath == NULL || nCount == 0)
		return -1;

	for(INT i = 0; i < nCount; i++)
	{
		if(CalculateDistance(ptPos, lpPath[i]) <= (TELEPORT_RANGE / 3))
			return i;
	}

	return -1;
}

BOOL CPathFinder::IsValidIndex(INT nX, INT nY)
{
	return nX >= 0 && nX < m_nSizeX && nY >= 0 && nY < m_nSizeY;
}

VOID CPathFinder::BlockLocation(POINT pos, INT nRange)
{
	nRange = max(nRange, 1);

	for(INT i = pos.x - nRange; i < pos.x + nRange; i++)
	{
		for(INT j = pos.y - nRange; j < pos.y + nRange; j++)
		{
			if (IsValidIndex(i, j))
				m_pMap[i][j] = RANGE_INVALID;
		}
	}
}