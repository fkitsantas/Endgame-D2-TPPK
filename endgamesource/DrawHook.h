#ifndef DRAWHOOK_H
#define DRAWHOOK_H

#define CFG_BLANK 0
#define CFG_INT 1
#define CFG_BOOL 2
#define CFG_ANTI 3
#define CFG_BLIND 4

#define STAT_RES 1
#define STAT_OTHER 2

typedef struct t_ConfigDisplay {
	PCHAR Setting;
	INT Type;
	PVOID Var;
} CONFIGDISPLAY;

typedef struct t_StatDisplay {
	PCHAR Name;
	INT Type;
	INT Stat;
	INT Color;
} STATDISPLAY;

#endif

VOID ConfigInteract(INT MouseX, INT MouseY);
BOOL DrawMessageConsole();
int Players();
BOOL IsHostile(DWORD UnitID);
int GetTextLength(char* szwText, int font);
void GameDraw();
void GameDraw2();
void DrawConsole();
void DrawUIVars();
void DrawAutomapInfos();
void ScreenHooks();
void RevealActOnce();
void DrawMonsters();
void DrawPlines();
void DrawYonTarget();
void LifeManaPercents();
void PlayerInfo();
void EnumObjects();
void DrawAbsAboveTarget();
DWORD __fastcall D2CLIENT_GetUnitName_STUB(DWORD UnitAny);
VOID ScreenToAutomap(POINT *ptPos, int x, int y);
VOID DrawCross(INT nX, INT nY, DWORD dwColor, BOOL Automap);
void TextHook(INT xPos, INT yPos, DWORD dwColor, BOOL Automap, INT dwFont, INT dwCenter, LPSTR Text,...);
void BoxHook(INT x, INT y, INT xSize, INT ySize, DWORD dwColor, DWORD Trans, BOOL Automap);
void D2Draw(INT xPos, INT yPos, DWORD dwColor,INT wCenter, LPSTR Text,...);
extern DWORD TimeOn;

VOID DrawVectorArrows();
VOID GetPlayerPosition(LPPOINT ptPlayer);
BOOL IsScreenClear(INT nScreen);
VOID GetPlayerPosition2(LPPOINT ptPlayer);
VOID GetVectorArrows();

typedef struct VectorTarget_t
{
	POINT ptPos;
	INT nType;
	DWORD dwTarget;
	DWORD dwTargetType;
} VECTOR, *LPVECTOR;

struct Vector {
POINT WPPos;
POINT OtherPos;
POINT QuestPos;
POINT SpecialPos;


DWORD WPID;
DWORD OtherID;
DWORD QuestID;
DWORD SpecialID;

};
extern Vector GlobalVectors;