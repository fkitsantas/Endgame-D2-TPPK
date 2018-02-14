#ifndef _COLLISIONMAP_H
#define _COLLISIONMAP_H

#include "ArrayEx.h"
#include "Matrix.h"

#define MAP_DATA_NULL		-1
#define MAP_DATA_AVOID		11115
#define MAP_DATA_FILL		11111

typedef struct CaveDescriptor_t
{
	CHAR szName[0x40];
	POINT ptPos;
	DWORD dwLevelNo;
	DWORD dwTargetLevelNo;
} CAVEDESC, *LPCAVEDESC;

typedef struct LevelExit_t
{
	POINT ptPos;
	DWORD dwTargetLevel;
} LEVELEXIT, *LPLEVELEXIT;

class CMap
{
public:
	CMap();
	virtual ~CMap();

	VOID RevealAutomap();
	BOOL CreateCollisionMap();
	Level* GetLevel(ActMisc* misc, DWORD levelno);
	BOOL GetCaveExits(LPCAVEDESC *lpLevel, INT nMaxExits);
	BOOL GetLevelExits(LPLEVELEXIT *lpLevel, INT nMaxExits);

	WORD GetCollisionInfo(INT nX, INT nY);
	BOOL ExportCollisionMap(CMatrix<WORD, WORD>& cMatrix);

	DWORD GetTileLevelNo(Room2* lpRoom2, DWORD dwTileNo);

protected:
	VOID AddAutomapRoom(Room2* pRoom2);
	VOID RemoveAutomapRoom(Room2* pRoom2);

	VOID RevealLevel(Level* pLevel);
	VOID RevealRoom(Room2* pRoom);
	VOID RevealRoom1(Room2* pRoom);
	VOID AddRoomCell(INT xPos, INT yPos, INT nCell, Room2* pRoom);

	INT GetUnitCellNumber(DWORD dwTxtFileNo, DWORD dwLevelNo);
	AutomapLayer2* InitAutomapLayer(DWORD dwLayer);

	BOOL AddCollisionData(Room2* pRoom2, CArrayEx<DWORD, DWORD>& aSkip);
	BOOL AddCollisionData(CollMap* pColl);
	BOOL FillGaps();
	BOOL IsGap(INT nX, INT nY);

	UCHAR m_ActLevels[6];
	UCHAR m_RevealedActs[5];
	CArrayEx<LPCAVEDESC, LPCAVEDESC> m_LevelExits;

	INT m_LastLevel;
	INT m_SizeX;
	INT m_SizeY;
	POINT m_LevelOrigin;
	CMatrix<WORD, WORD> m_Map;
};

#endif