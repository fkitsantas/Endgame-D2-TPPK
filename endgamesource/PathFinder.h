#ifndef _PATHFINDER_H
#define _PATHFINDER_H

#define RANGE_INVALID	10000
#define TELEPORT_RANGE	38
enum PATHRETURN
{
	PATH_REACHED = 0,
	PATH_CONTINUE,
	PATH_FAILED,
};

extern POINT ptPath[200];

class CPathFinder
{
public:
	
	CPathFinder();
	virtual ~CPathFinder();

	VOID SetPathingMap(LPWORD *pMap, INT nSizeX, INT nSIzeY);
	BOOL CreateBestPath(POINT ptStart, POINT ptTarget, LPPOINT ptPath, DWORD dwPath);

protected:
	PATHRETURN GetNextMove(POINT& ptTarget, INT nAdjust = 2);
	BOOL CreateDistanceTable();

	BOOL IsValidIndex(INT nX, INT nY);
	VOID BlockLocation(POINT ptPos, INT nRange);
	BOOL CheckRedundancy(LPPOINT lpPath, INT nCount, POINT ptPos);
	
	LPWORD *m_pMap;
	INT m_nSizeX;
	INT m_nSizeY;
	POINT m_ptStart, m_ptEnd;
};

BOOL PathToVector(INT nVectorType);

#endif
