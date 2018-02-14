#include "Houdini.h"
#include "Area.h"
#include "Team.h"
#define QUANTITY_STAT_ID 70

Vector GlobalVectors;
#define QUESTCOLOR	155
#define OTHER		152
#define WP			132
#define SPECIAL		109
#define DRAW(f,c) if(fileno==f){\
    if(c==QUESTCOLOR){GlobalVectors.QuestID=ObjID;}\
    if(c==OTHER){GlobalVectors.OtherID=ObjID;}\
	if(c==SPECIAL){GlobalVectors.SpecialID=ObjID;}}

DWORD GUI=0x6FB52E90;

DWORD GetExpLeft()
{
	UnitAny* pUnit=(UnitAny*)*(DWORD*)0x6FBCC3D0;
	return D2COMMON_GetUnitState3(2,D2COMMON_GetUnitState2(pUnit,0x0C,0));
}

RosterUnit* FindPartyByName(LPSTR szName)
{
	for(RosterUnit* pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext)
	if(!_stricmp(szName, pUnit->szName))
		return pUnit;

	return NULL;
}

VOID ResetValues()
{
	nGameTimer = GetTickCount();
	v_FlashDelay=-1;
	v_DrankLifePot = -1;
	v_DrankManaPot = -1;
	v_DrankLifeRej = -1;
	v_DrankManaRej = -1;
	v_TownLifeTimer = -1;
	v_TownManaTimer = -1;
	v_OakTimer=-1;
	v_BoTimer=-1;
	v_GameCount=0;	
}

BOOL SetSkill(WORD wSkillId, BOOL bLeft)
{
	if(!D2CLIENT_GetPlayerUnit()) return FALSE;

	BYTE aPacket[9];

	aPacket[0] = 0x3C;
	*(WORD*)&aPacket[1] = wSkillId;
	aPacket[3] = 0;
	aPacket[4] = (bLeft) ? 0x80 : 0;
	*(DWORD*)&aPacket[5] = 0xFFFFFFFF;

	D2NET_SendPacket(9, 1, aPacket);
	return TRUE;
}
VOID FlashPlayer(DWORD ID)
{
	BYTE UPDATE[9] = {0x4b,0,0,0,0,0,0,0,0};
	memcpy(&UPDATE[5],&ID,4);
	SendGAMEPacket(UPDATE,9);
}

VOID SQ(DWORD ID) 
{
	BYTE aPacket[7];

	aPacket[0] = 0x5d;
	aPacket[1] = 0x03;
	aPacket[2] = 0x01;
	*(DWORD*)&aPacket[3] = ID;

	D2NET_SendPacket(7, 1, aPacket);
}

BOOL IsInventoryOpen()
{
	INT State=*(DWORD*)0x6FBCC1D0;
	if(State==1) return true;
	else false;
	return false;
}
VOID  ToggleInventory(BOOL on)
{
	if(on) 
	if(IsInventoryOpen()==false)
	_asm CALL GUI
	if(!on)
	if(IsInventoryOpen())
	_asm CALL GUI
}
PCHAR ReplaceString(PCHAR source, PCHAR old, PCHAR newtext)
{
	PCHAR original = new CHAR[strlen(source)];
	strcpy(original, source);
	PCHAR temp = new CHAR[256];
	INT old_length = strlen(old);
	INT i, j, k, location = -1;
	for(i = 0; source[i] && (location == -1); ++i)
	for(j = i, k = 0; source[j] == old[k]; j++, k++)
	if(!old[k+1])
	location = i;
	if(location != -1)
	{
		for(j = 0; j < location; j++)
		temp[j] = source[j];
		for(i = 0; newtext[i]; i++, j++)
		temp[j] = newtext[i];
		for(k = location + old_length; source[k]; k++, j++)
		temp[j] = source[k];
		temp[j] = NULL;
		for(i = 0; source[i] = temp[i]; i++);
	}       
	delete original;
	return temp;
}

DWORD __declspec(naked) IsExpansion()
{
	__asm
	{
		mov eax,0x6FBB330C
		mov eax,[eax]
		retn
	}
}

GameStructInfo* GetGameInfo()
{
	DWORD Addresse=0x11B908;
	Addresse=Addresse+(DWORD)LoadLibrary("D2Client.dll");
	GameStructInfo* Returne;
	__asm
	{
		push eax
		mov eax, [Addresse]
		mov eax, [eax]
		mov [Returne],eax
		pop eax
	}
	return Returne;
}
BOOL Speak(LPSTR lpMessage, ...)
{
	if (strlen(lpMessage) > 0)
	{
		CHAR szBuffer[0x400] = "";
		va_list Args;

		va_start(Args, lpMessage);
		vsprintf(szBuffer, lpMessage, Args);
		va_end(Args);

		LPBYTE aPacket = new BYTE[(INT)strlen(szBuffer) + 6];
		memset(aPacket, 0, (INT)strlen(szBuffer) + 6);

		aPacket[0] = 0x15;
		*(LPWORD)&aPacket[1] = 1;
		memcpy(aPacket + 3, szBuffer, (INT)strlen(szBuffer));
	
		D2NET_SendPacket((INT)strlen(szBuffer) + 6, 1, aPacket);
		delete [] aPacket;
	}

	return TRUE;
}

VOID Tele(WORD x,WORD y)
{
	DWORD ID;
	BYTE TELE[5]   =  {0x5f, 0,0,   0,0};
	BYTE UPDATE[9] =  {0x4b, 0,0,0,0,     0,0,0,0};
	ID = GetPlayerID();
	memcpy(&UPDATE[5],&ID,4);
	memcpy(&TELE[1],&x,2);
	memcpy(&TELE[3],&y,2);
	SendGAMEPacket(TELE,5);
	SendGAMEPacket(UPDATE,9);
}
VOID Update(DWORD ID)
{
	BYTE UPDATE[9] =  {0x4B,0,0,0,0,0,0,0,0};
	memcpy(&UPDATE[5],&ID,4);
	SendGAMEPacket(UPDATE,9);
}

DWORD GetTileLevelNo(Room2* lpRoom2, DWORD dwTileNo)
{
	for(RoomTile* pRoomTile = lpRoom2->pRoomTiles; pRoomTile; pRoomTile = pRoomTile->pNext)
	{
		if(*(pRoomTile->nNum) == dwTileNo)
			return pRoomTile->pRoom2->pLevel->dwLevelNo;
	}
	return NULL;
}

Level* GetLevel(DWORD dwLevelNo)
{
	for(Level* pLevel = D2CLIENT_GetPlayerUnit()->pAct->pMisc->pLevelFirst; pLevel; pLevel = pLevel->pNextLevel)
		if(pLevel->dwLevelNo == dwLevelNo)
			return pLevel;

	return D2COMMON_GetLevel(D2CLIENT_GetPlayerUnit()->pAct->pMisc, dwLevelNo);
}

VOID DrawBlob(INT xpos, INT ypos,INT col)
{
	static CHAR blines[][2] = {0,-2, 4,-4, 8,-2, 4,0, 8,2, 4,4, 0,2, -4,4, -8,2, -4,0, -8,-2, -4,-4, 0,-2};
	for (INT i = 0; i < ARRAYSIZE(blines)-1; i++)
	{
		D2GFX_DrawLine(xpos+blines[i][0], ypos+blines[i][1], xpos+blines[i+1][0], ypos+blines[i+1][1], col, 0xff);
	}
}

WORD GetCurrentSkillLeft()
{
	UnitAny *pUnit = D2CLIENT_GetPlayerUnit();
	return pUnit->pInfo->pLeftSkill->pSkillInfo->wSkillId;
}
WORD GetCurrentSkillRight()
{
	UnitAny *pUnit = D2CLIENT_GetPlayerUnit();
	return pUnit->pInfo->pRightSkill->pSkillInfo->wSkillId;
}

INT GetSkill(WORD wSkillId)
{
	if(!D2CLIENT_GetPlayerUnit()) return 0;

	for(Skill* pSkill = D2CLIENT_GetPlayerUnit()->pInfo->pFirstSkill; pSkill; pSkill = pSkill->pNextSkill)
	if(pSkill->pSkillInfo->wSkillId == wSkillId)
	return D2COMMON_GetSkillLevel(D2CLIENT_GetPlayerUnit(), pSkill, TRUE);

	return 0;
}

INT GetLevelByN(PCHAR Name)
{
	RosterUnit* pUnit=(RosterUnit*)*(DWORD*)0x6FBCC080;

	while(pUnit!=NULL)
	{
		if(!_strcmpi(pUnit->szName,Name))
			return pUnit->wLevel;
		pUnit=pUnit->pNext;
	}

	return -1;
}
INT RandomNumber(INT high, INT low)
{
	return ((rand() % high - low) + low + 1);
}

BOOL ChickenAim(INT nValue, DWORD ID, DWORD AimID)
{
	CHAR Buffer[200];

    if(ID==GetPlayerID())
	{
		switch(nValue)
		{
		case 0:
			{
				break;
			}
		case 1:
			{
				v_HostPlayer=false; v_TakeNextTP=true; v_AntiTppkBusy=true;

				if(MakeTP())
				{
					PrintConsoleString("ÿc1Escaping to Town: %s just aimed at you",(GetNameByID(AimID)));
				}
				else D2CLIENT_ExitGame();

				break;
			}
		case 2:
			{
				if(!GetSkill(D2S_TELEPORT))
					D2CLIENT_ExitGame();
				else
				{
					SetSkill(D2S_TELEPORT, FALSE);

					PrintConsoleString("ÿc1Escaping with Teleport: %s just aimed at you",(GetNameByID(AimID)));

					BYTE Packet2[] = {0xc,00,00,00,00};
					*(DWORD*)&Packet2[1]=GetPosition().x+RandomNumber(30, -30),2;
					*(DWORD*)&Packet2[3]=GetPosition().y+RandomNumber(30, -30),2;
					SendGAMEPacket(Packet2,sizeof(Packet2));
				}
				break;
			}
		case 3:
			{
				D2CLIENT_ExitGame();
				break;
			}
		case 4:
			{
				PrintConsoleString("ÿc1Warning: %s just aimed at you",(GetNameByID(AimID)));
				break;		
			}
		default:
			{
			break;
			}
		}
	} 

	return TRUE;
}
BOOL ChickenHostile(INT nValue, DWORD PlayerID)
{
	CHAR Buf[200];

	switch(nValue)
	{
		case 0:
		{
			break;
		}            
		case 1:
		{
			if(MakeTP())
			{
				v_HostPlayer=false; v_TakeNextTP=true; v_AntiTppkBusy=true;

				PrintConsoleString("ÿc1Escaping to Town: %s just hostiled you",(GetNameByID(PlayerID)));

			}
			else D2CLIENT_ExitGame();

			break;
		}
		case 2:
		{
			if(!GetSkill(D2S_TELEPORT))
			{
				D2CLIENT_ExitGame(); return TRUE;
			}
			else
			{
				SetSkill(D2S_TELEPORT, FALSE);

				PrintConsoleString("ÿc1Escaping with Teleport: %s just hostiled you",(GetNameByID(PlayerID)));

				BYTE Packet2[] = {0xc,00,00,00,00};
				*(DWORD*)&Packet2[1]=GetPosition().x+15,2;
				*(DWORD*)&Packet2[3]=GetPosition().y-10,2;
				SendGAMEPacket(Packet2,sizeof(Packet2));
			}

			break;
		}
		case 3:
		{
			D2CLIENT_ExitGame();
			
			break;
		}
		case 4:

			PrintConsoleString("ÿc1Warning: %s just hostiled you",(GetNameByID(PlayerID)));
			break;
	
		default:

			break;
	}

	return TRUE;
}
INT GetPlayerClassName(INT nClass, LPSTR szBuffer, DWORD dwMaxLen)
{
	switch(nClass)
	{
	case 0:
		sprintf(szBuffer, "Amazon");
		break;
	case 1:
		sprintf(szBuffer, "Sorceress");
		break;
	case 2:
		sprintf(szBuffer, "Necromancer");
		break;
	case 3:
		sprintf(szBuffer, "Paladin");
		break;
	case 4:
		sprintf(szBuffer, "Barbarian");
		break;
	case 5:
		sprintf(szBuffer, "Druid");
		break;
	case 6:
		sprintf(szBuffer, "Assassin");
		break;
	}

	return (INT)strlen(szBuffer);
}
BOOL CheckParty(INT Status)
{
	switch(Status)
	{
	case 3:
		{
			return FALSE;
			break;
		}
	case 1:
		{
			return FALSE;
			break;
		}
	case 2:
		{
			return TRUE;
			break;
		}
	}
}
BOOL Spoof()
{
    BYTE aPacket2[3];
	aPacket2[0] = 0x1D;
	aPacket2[1] = 102;
	aPacket2[2] = 200;
    D2NET_ReceivePacket(aPacket2, sizeof(aPacket2));

    BYTE aPacket3[3];
	aPacket3[0] = 0x1D;
	aPacket3[1] = 99;
	aPacket3[2] = 200;
    D2NET_ReceivePacket(aPacket3, sizeof(aPacket3));

	return TRUE;
}

DWORD GetDistance(DWORD x1, DWORD y1, DWORD x2, DWORD y2)
{
	return (DWORD)sqrt((double)(( ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)))));
}

DWORD GetDistanceSquared(DWORD x1, DWORD y1, DWORD x2, DWORD y2)
{
 	return (DWORD)sqrt((double)( ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2))));
}
PCHAR CheckQuest(INT nQuest)
{
	switch(nQuest)
	{
		case 0: {
			return "Den Of Evil";
		}
		case 1: {
			return "Deckard Cain";
		}
		case 3: {
			return "Tal Rasha's Tomb";
		}
		case 6: {
			return "Claw Viper Temple";
		}
		case 7: {
			return "Summoner";
		}
		case 8: {
			return "Duriel";
		}                
		case 10: {
			return "Khalim's Will";
		}
		case 11: {
			return "Mephisto";
		}
		case 12: {
			return "Diablo";
		}
		case 13: {
			return "Diablo";
		}
		case 14: {
			return "Hell Forge";
		}
		case 15: {
			return "Shenk";
		}
		case 16: {
			return "Anya";
		}
		case 17: {
			return "Nihlathak";
		}
		case 18: {
			return "Ancients";
		}
		case 19: {
			return "Baal";
		}
		default: {
			CHAR Buf[200];
			sprintf (Buf,"Unknown[%d]",nQuest);
			return Buf;
		}
	}
}

POINT GetPosition()
{
	POINT mp;
	UnitAny* PlayerUnit = D2CLIENT_GetPlayerUnit();
	mp.x=PlayerUnit->pPath->xPos;
	mp.y=PlayerUnit->pPath->yPos;
	return mp;
}
INT GetPercent(DWORD Current,DWORD Max)
{
	float Cur1 = (INT)Current;
	float Max1 = (INT)Max;
	return (Cur1 / Max * 100);
}
VOID GetD2MousePos(POINT &Pointer)
{
	typedef unsigned short (__fastcall *pPos)();

	DWORD MouseX=0x6FB4EAC0-0x6fab0000;
	DWORD MouseY=0x6FB4EAD0-0x6fab0000;
	MouseX+=(DWORD)LoadLibrary("D2Client.dll");
	MouseY+=(DWORD)LoadLibrary("D2Client.dll");
	pPos pPosX=(pPos)MouseY;
	pPos pPosY=(pPos)MouseX;
	Pointer.x=pPosX();
	Pointer.y=pPosY();
}
VOID HoldRightUp(long X, long Y)
{
	SendMessage((HWND)D2GFX_GetHwnd(),WM_RBUTTONUP,0,MAKELONG(X,Y));
}

VOID Hostile(DWORD ID)
{
	PCHAR pName=GetNameByID(ID);
	if(GetLevelByName(pName)>=9 && !IsHostile(ID))
	{
		BYTE Hostile[7] = {0x5d,0x04,0x01,0,0,0,0};
		*(DWORD*)&Hostile[3]=ID;
		SendGAMEPacket(Hostile,sizeof(Hostile));
	}
}
VOID MyMultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, INT cbMultiByte, LPWSTR lpWideCharStr, INT cchWideChar)
{
	MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
	for(DWORD i = 0; i < wcslen(lpWideCharStr); i++)
	{
		if(lpWideCharStr[i] >= 0xFF && lpWideCharStr[i + 1] == L'c')
		{
			if(lpWideCharStr[i + 2] >= L'0' && lpWideCharStr[i + 2] <= L'9')
			{
				lpWideCharStr[i] = 0xFF;
			}
		}
	}
}

VOID DrawRectangle(INT x, INT y, INT x2, INT y2, DWORD dwColor, DWORD dwTrans)
{
	D2GFX_DrawRectangle(x,y,x2,y2,dwColor,dwTrans);
	D2DrawRectFrame(x,y,x2,y2);
}

VOID PrintFormatString(LPSTR lpFormat, ...)
{
	CHAR szOutput[0x400] = "ÿc4Houdiniÿc0 :: ";
	WCHAR wOutput[0x400] = L"";
	va_list Args;

	if(!Myself )
		return;

	va_start(Args, lpFormat);
	vsprintf(szOutput + strlen(szOutput), lpFormat, Args);
	va_end(Args);

	MyMultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szOutput, (INT)strlen(szOutput), wOutput, (INT)strlen(szOutput));
	D2CLIENT_PrintGameString(wOutput, 0);
}

VOID PrintString(LPSTR lpFormat, ...)
{
	CHAR szOutput[0x400] = "";
	WCHAR wOutput[0x400] = L"";
	va_list Args;

	if(!Myself )
		return;

	va_start(Args, lpFormat);
	vsprintf(szOutput + strlen(szOutput), lpFormat, Args);
	va_end(Args);

	MyMultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szOutput, (INT)strlen(szOutput), wOutput, (INT)strlen(szOutput));
	D2CLIENT_PrintGameString(wOutput, 0);
}
VOID OverheadMessage(PCHAR Message)
{
	CHAR lpszText[300];
	sprintf(lpszText,Message);
	OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
	if(pMsg)
	{
		D2COMMON_FixOverheadMsg(pMsg, NULL);
		Myself->pOMsg = pMsg;
	}
}

VOID PrintMessageBottomLeft(PCHAR Message, CHAR Color)
{
	typedef VOID (_stdcall *pPrint)(wchar_t* Text, CHAR Color);
	pPrint Print = (pPrint)(DWORD)(0x6FB21500-0x6fab0000+(DWORD)LoadLibrary("D2Client.dll"));
	wchar_t Buffer[0x130];
	MultiByteToWideChar(0, 1, Message, 100, Buffer, 100);
	Print (Buffer, Color);
};

VOID DrawTextToScreen(PCHAR szwText, INT x, INT y, INT color, INT font)
{
	wchar_t Buffer[900];
	MultiByteToWideChar(0, 1, szwText, 50, Buffer, 100);
	DWORD dwOld=D2WIN_SetTextSize(font);
	DWORD dwXLength=D2WIN_GetTextLength(Buffer);
	D2WIN_DrawText(Buffer,x,y,color,0);
	D2WIN_SetTextSize(dwOld);
}

VOID DrawTextToScreenRight(PCHAR szwText, INT x, INT y, INT color, INT font)
{
	wchar_t Buffer[100];
	MultiByteToWideChar(0, 1, szwText, 50, Buffer, 100);
	DWORD dwOld=D2WIN_SetTextSize(font);
	DWORD dwXLength=D2WIN_GetTextLength(Buffer);
	D2WIN_DrawText(Buffer,x-dwXLength,y,color,0);
	D2WIN_SetTextSize(dwOld);
}

VOID DrawTextToScreenCenter(PCHAR szwText, INT x, INT y, INT color, INT font)
{
	wchar_t Buffer[100];
	MultiByteToWideChar(0, 1, szwText, 50, Buffer, 100);
	DWORD dwOld=D2WIN_SetTextSize(font);
	DWORD dwXLength=D2WIN_GetTextLength(Buffer);
	D2WIN_DrawText(Buffer,x-(dwXLength/2),y,color,0);
	D2WIN_SetTextSize(dwOld);
}

VOID WinampHook(INT xPos, INT yPos, DWORD dwColor, INT dwFont, INT dwCenter, LPSTR Text,...)
{
	POINT nPos = {xPos, yPos};
	DWORD dwOldSize, wWidth, dwFileNo;

	CHAR szBuffer[800] = "";
	va_list Args;
	va_start(Args, Text);
	vsprintf_s(szBuffer, Text, Args);
	va_end(Args);

	wchar_t wBuffer[0x130];
	MultiByteToWideChar(0, 1, szBuffer, 100, wBuffer, 100);

	dwOldSize = D2WIN_SetTextSize(dwFont);
	if (dwCenter != -1)
	{
		D2WIN_GetTextWidthFileNo(wBuffer, &wWidth, &dwFileNo);
		nPos.x -= (wWidth >> dwCenter);
	}
	
	D2WIN_DrawText (wBuffer,nPos.x, nPos.y,dwColor,-1);
	D2WIN_SetTextSize(dwOldSize);
} 


DWORD GetClientHandle()
{
	return (DWORD)LoadLibrary("D2Client.dll");
}

INT __declspec(naked) D2CLIENT_GetPlayerXOffset2()
{
	__asm
	{
		call GetClientHandle
		add eax, 0x5D650
		call eax
		ret
	}
}

INT __declspec(naked) D2CLIENT_GetPlayerYOffset2()
{
	__asm
	{
		call GetClientHandle
		add eax, 0x5D670
		call eax
		ret
	}
}
VOID  MapToScreenCoords(POINT& rMapPosition)
{
	D2COMMON_MapToAbsScreen(&rMapPosition.x, &rMapPosition.y);
	rMapPosition.x -= D2CLIENT_GetMouseXOffset();
	rMapPosition.y -= D2CLIENT_GetMouseYOffset();
}
VOID ScreenToAutomapRelative(POINT* pos, INT x, INT y)
{
	x +=D2CLIENT_GetMouseXOffset();
	y +=D2CLIENT_GetMouseYOffset();
	ScreenToAutomap(pos, y*2+x, y*2-x);
}

VOID fpEnumUnits(fp EnumFunction,DWORD Type)
{
	Level* pLevel = GetLevel(D2CLIENT_GetPlayerUnit()->pAct->pMisc->pLevelFirst->dwLevelNo);

	for(Room2 *pRoom2 = pLevel->pRoom2First;pRoom2;pRoom2=(Room2*)pRoom2->pRoom2Other)
	{
		for(Room1* pRoom1 = pRoom2->pRoom1;pRoom1;pRoom1=pRoom1->pRoomNext)
		{
			for(UnitAny* pUnit = pRoom2->pRoom1->pUnitFirst;pUnit;pUnit=pUnit->pListNext)
			{
				if(pUnit)
				if(pUnit->dwType==Type || Type == UNIT_ALL)
				if(!EnumFunction(pUnit))
				return;
			}
		}
	}
}


 VOID __declspec(naked) __fastcall D2DrawRectFrameWrap(LPRECT pRect)
 { 
	 
	__asm
	{
		mov eax,ecx
		call D2CLIENT_DrawRectFrame
		ret
	}

}
VOID D2DrawRectFrame(INT x,INT y,INT x2,INT y2)
{
	RECT Frame;
	Frame.left = x;
	Frame.top = y;
	Frame.right = x2;
	Frame.bottom = y2;
	D2DrawRectFrameWrap(&Frame);
}

PCHAR mitoa(INT integer)
{
	CHAR Buffer[100]={0};
	sprintf_s(Buffer,"%d",integer);
	return Buffer;
}
enum {CLASS_AMAZON=0,CLASS_SORCERESS,CLASS_NECROMANCER,CLASS_PALADIN,CLASS_BARBARIAN,CLASS_DRUID,CLASS_ASSASSIN};
PCHAR GetD2ClassName(DWORD ClassID)
{
	switch(ClassID)
	{
	case CLASS_AMAZON:
		return "Amazon";
	case CLASS_SORCERESS:
		return "Sorceress";
	case CLASS_NECROMANCER:
		return "Necromancer";
	case CLASS_PALADIN:
		return "Paladin";
	case CLASS_BARBARIAN:
		return "Barbarian";
	case CLASS_DRUID:
		return "Druid";
	case CLASS_ASSASSIN:
		return "Assassin";
	default:
		return "???";
	}
}

PCHAR GetD2ClassName2(DWORD ClassID)
{
	switch(ClassID)
	{
	case CLASS_AMAZON:
		return "amazon";
	case CLASS_SORCERESS:
		return "sorceress";
	case CLASS_NECROMANCER:
		return "necromancer";
	case CLASS_PALADIN:
		return "paladin";
	case CLASS_BARBARIAN:
		return "barbarian";
	case CLASS_DRUID:
		return "druid";
	case CLASS_ASSASSIN:
		return "assassin";
	default:
		return "???";
	}
}
DWORD GetPlayerArea(VOID)
{
	if(D2CLIENT_GetPlayerUnit())

		if(D2CLIENT_GetPlayerUnit()->pPath)

			if(D2CLIENT_GetPlayerUnit()->pPath->pRoom1)

				if(D2CLIENT_GetPlayerUnit()->pPath->pRoom1->pRoom2)

					if(D2CLIENT_GetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel)

			return D2CLIENT_GetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel->dwLevelNo;

	return NULL;
}
BOOL IsTownLevel(INT nLevel)
{
	INT szTownNames[] = {MAP_A1_ROGUE_ENCAMPMENT, MAP_A2_LUT_GHOLEIN, MAP_A3_KURAST_DOCKS, MAP_A4_THE_PANDEMONIUM_FORTRESS, MAP_A5_HARROGATH};
	for (INT i=0;i<ArraySize(szTownNames);i++)
	{
		if(nLevel==szTownNames[i])
			return TRUE;
	}
	return FALSE;
}
INT GetTPs()
{
	UnitAny* pPlayer=(UnitAny*)*(DWORD*)0x6FBCC3D0;
	INT Quant=0;

	if(pPlayer->pInventory)
	{
		UnitAny* p = D2COMMON_GetItemFromInventory(pPlayer->pInventory);

		while(p)
		{
			if(p->pItemData->ItemLocation != STORAGE_STASH && p->pItemData->ItemLocation != STORAGE_CUBE)
			{
				ItemTxt* pTxt=D2COMMON_GetItemText(p->dwTxtFileNo);

				if(pTxt)
				{
					CHAR szCode[4];
					strncpy(szCode,pTxt->szCode,4);
					szCode[3]='\0';
					if(!_strcmpi(szCode,"tbk"))
					{
						if(D2COMMON_GetUnitStat(p,QUANTITY_STAT_ID,0))
						Quant=Quant+ D2COMMON_GetUnitStat(p,QUANTITY_STAT_ID,0);
					}
					if(!_strcmpi(szCode,"tsc"))
					{
						Quant=Quant+1;
					}
				}
			}
			p=D2COMMON_GetNextItemFromInventory(p);
		}
	}
	return Quant;
}

UnitAny* FindItem(LPCSTR szCode, INT nLoc)
{
	ItemTxt *pTxt = NULL;
	for(UnitAny* pItem = Myself->pInventory->pFirstItem; pItem; pItem = pItem->pItemData->pNextInvItem)
	{
		if(GetItemLocation(pItem) == nLoc)
		{
			pTxt = D2COMMON_GetItemText(pItem->dwTxtFileNo);
			CHAR szTemp[4] = {0};
			if(!GetItemCode(pItem, szTemp, 3))
				return NULL;

			if(!stricmp(szCode, szTemp))
				return pItem;
		}
	}
	return NULL;
}
INT GetItemLocation(UnitAny* pItem)
{
	if(!pItem || !pItem->pItemData)
		return -1;

	switch(pItem->pItemData->ItemLocation)
	{
	case STORAGE_INVENTORY:
		return STORAGE_INVENTORY;

	case STORAGE_TRADE:
		if(pItem->pItemData->NodePage == NODEPAGE_STORAGE)
			return STORAGE_TRADE;

	case STORAGE_CUBE:
		return STORAGE_CUBE;

	case STORAGE_STASH:
		return STORAGE_STASH;

	case STORAGE_NULL:
		switch(pItem->pItemData->NodePage)
		{
		case NODEPAGE_EQUIP:
			return STORAGE_EQUIP;

		case NODEPAGE_BELTSLOTS:
			return STORAGE_BELT;
		}
	}
	return STORAGE_NULL;
}

BOOL GetItemCode(UnitAny* pItem, LPSTR szBuffer, DWORD dwMax)
{
	if(!pItem)
		return FALSE;

	ItemTxt *pTxt = D2COMMON_GetItemText(pItem->dwTxtFileNo);
	::memcpy(szBuffer, pTxt->szCode, min(3, dwMax));
	return (INT)strlen(szBuffer);
}

BOOL UseItem(UnitAny* Unit)
{
	if(!Unit) return FALSE;

	BYTE aPacket[13] = {0};
	switch(GetItemLocation(Unit))
	{
	case STORAGE_INVENTORY:
		aPacket[0] = 0x20;
		*(LPDWORD)&aPacket[1] = Unit->dwUnitId;
		*(LPDWORD)&aPacket[5] = Myself->pPath->xPos;
		*(LPDWORD)&aPacket[9] = Myself->pPath->yPos;
		break;

	case STORAGE_BELT:
		aPacket[0] = 0x26;
		*(LPDWORD)&aPacket[1] = Unit->dwUnitId;
		*(LPDWORD)&aPacket[5] = 0;
		*(LPDWORD)&aPacket[9] = 0;
		break;
	}
	if(aPacket[0]) SendGAMEPacket(aPacket, sizeof(aPacket));

	return TRUE;
}
BOOL MakeTP()
{
	UnitAny* pUnit = NULL;

	if(!GameReady(FALSE) || IsTownLevel(GetPlayerArea()))
		return FALSE;

	pUnit = FindItem("tbk", STORAGE_INVENTORY);
	if(!pUnit || GetUnitStat(pUnit, STAT_AMMOQUANTITY) == 0)
	{
		pUnit = FindItem("tsc", STORAGE_INVENTORY);
		if(!pUnit)
		{
			pUnit = FindItem("tsc", STORAGE_BELT);
			if(!pUnit)
			{
				return FALSE;
			}
		}
	}

	UseItem(pUnit);

	return TRUE;
}

DWORD __declspec(naked) __fastcall PartyMode(LPSTR szName,DWORD Mode)
{
	__asm
	{
		MOV EAX,EDX
		MOV ECX,ECX
		MOV EBX,0x6FB12400
		JMP EBX
		ret
	}
}

#define PLAYERMODE_NOPARTY 0xFFFF

VOID HostileAll()
{
	INT PartyId[0x04];
	memset(PartyId,0x00,sizeof(PartyId));
	BOOL bLowLevel = false;
	BOOL bLowLevel2 = false;

	for(RosterUnit* pUnit = (*p_D2CLIENT_PlayerUnitList); pUnit; pUnit = pUnit->pNext)
	{
		if(pUnit->dwUnitId == Myself->dwUnitId)
		continue;


		if(pUnit->wLevel < 9)
		{
			continue;
		}
		if(GetPvPStatus(pUnit->dwUnitId) == PVP_HOSTILE)
		{
			continue;
		}

		BOOL bPartyTemp = false;

		if(pUnit->wPartyId != PLAYERMODE_NOPARTY)
			for(INT x = 0; x < ArraySize(PartyId); x++)
				if(pUnit->wPartyId == PartyId[x])
					bPartyTemp = true;

		if(bPartyTemp)
			continue;

		if(pUnit->wPartyId != PLAYERMODE_NOPARTY)
		PartyId[ArraySize(PartyId)-1] = pUnit->wPartyId;

		switch(GetPvPStatus(pUnit->dwUnitId))
		{
			case PVP_NEUTRAL:
			{
				Hostile(pUnit->dwUnitId);
			}
			case PVP_FRIENDLY:
			{
				Hostile(pUnit->dwUnitId);
			}
		}
	}
	free(PartyId);
} 


BOOL D2GetMapName(BYTE iMapID, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if(lpszBuffer == NULL)
		return FALSE;

	lpszBuffer[0] = '\0';
	::memset(lpszBuffer, 0, sizeof(TCHAR) * dwMaxChars);
	if(dwMaxChars == 0)
		return FALSE;

	switch (iMapID)
	{	
		///////////////////////////////////////////////////
		// Act 1 Maps
		///////////////////////////////////////////////////
	case MAP_A1_ROGUE_ENCAMPMENT:
		strncpy(lpszBuffer, "Rogue Encampment", dwMaxChars);
		break;

	case MAP_A1_BLOOD_MOOR:
		strncpy(lpszBuffer, "Blood Moor", dwMaxChars);
		break;

	case MAP_A1_COLD_PLAINS:
		strncpy(lpszBuffer, "Cold Plains", dwMaxChars);
		break;

	case MAP_A1_STONY_FIELD:
		strncpy(lpszBuffer, "Stony Field", dwMaxChars);
		break;

	case MAP_A1_DARK_WOOD:
		strncpy(lpszBuffer, "Dark Wood", dwMaxChars);
		break;

	case MAP_A1_BLACK_MARSH:
		strncpy(lpszBuffer, "Black Marsh", dwMaxChars);
		break;

	case MAP_A1_DEN_OF_EVIL:
		strncpy(lpszBuffer, "Den of Evil", dwMaxChars);
		break;

	case MAP_A1_CAVE_LEVEL_1:
		strncpy(lpszBuffer, "Cave Level 1", dwMaxChars);
		break;

	case MAP_A1_UNDERGROUND_PASSAGE_LEVEL_1:
		strncpy(lpszBuffer, "Underground Passage Level 1", dwMaxChars);
		break;

	case MAP_A1_HOLE_LEVEL_1:
		strncpy(lpszBuffer, "Hole Level 1", dwMaxChars);
		break;

	case MAP_A1_PIT_LEVEL_1:
		strncpy(lpszBuffer, "Pit Level 1", dwMaxChars);
		break;

	case MAP_A1_CAVE_LEVEL_2:
		strncpy(lpszBuffer, "Cave Level 2", dwMaxChars);
		break;

	case MAP_A1_UNDERGROUND_PASSAGE_LEVEL_2:
		strncpy(lpszBuffer, "Underground Passage Level 2", dwMaxChars);
		break;

	case MAP_A1_HOLE_LEVEL_2:
		strncpy(lpszBuffer, "Hole Level 2", dwMaxChars);
		break;

	case MAP_A1_PIT_LEVEL_2:
		strncpy(lpszBuffer, "Pit Level 2", dwMaxChars);
		break;

	case MAP_A1_BURIAL_GROUNDS:
		strncpy(lpszBuffer, "Burial Grounds", dwMaxChars);
		break;

	case MAP_A1_CRYPT:
		strncpy(lpszBuffer, "Crypt", dwMaxChars);
		break;

	case MAP_A1_MAUSOLEUM:
		strncpy(lpszBuffer, "Mausoleum", dwMaxChars);
		break;

	case MAP_A1_FORGOTTEN_TOWER:
		strncpy(lpszBuffer, "Forgotten Tower", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_1:
		strncpy(lpszBuffer, "Tower Cellar Level 1", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_2:
		strncpy(lpszBuffer, "Tower Cellar Level 2", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_3:
		strncpy(lpszBuffer, "Tower Cellar Level 3", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_4:
		strncpy(lpszBuffer, "Tower Cellar Level 4", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_5:
		strncpy(lpszBuffer, "Tower Cellar Level 5", dwMaxChars);
		break;

	case MAP_A1_TAMOE_HIGHLAND:
		strncpy(lpszBuffer, "Tamoe Highland", dwMaxChars);
		break;

	case MAP_A1_MONASTERY_GATE:
		strncpy(lpszBuffer, "Monastery Gate", dwMaxChars);
		break;

	case MAP_A1_OUTER_CLOISTER:
		strncpy(lpszBuffer, "Outer Cloister", dwMaxChars);
		break;

	case MAP_A1_BARRACKS:
		strncpy(lpszBuffer, "Barracks", dwMaxChars);
		break;

	case MAP_A1_JAIL_LEVEL_1:
		strncpy(lpszBuffer, "Jail Level 1", dwMaxChars);
		break;

	case MAP_A1_JAIL_LEVEL_2:
		strncpy(lpszBuffer, "Jail Level 2", dwMaxChars);
		break;

	case MAP_A1_JAIL_LEVEL_3:
		strncpy(lpszBuffer, "Jail Level 3", dwMaxChars);
		break;

	case MAP_A1_INNER_CLOISTER:
	case MAP_A1_INNER_CLOISTER_2:
		strncpy(lpszBuffer, "Inner Cloister", dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_1:
		strncpy(lpszBuffer, "Catacombs Level 1", dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_2:
		strncpy(lpszBuffer, "Catacombs Level 2", dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_3:
		strncpy(lpszBuffer, "Catacombs Level 3", dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_4:
		strncpy(lpszBuffer, "Catacombs Level 4", dwMaxChars);
		break;

	case MAP_A1_TRISTRAM:
		strncpy(lpszBuffer, "Tristram", dwMaxChars);
		break;

	case MAP_A1_THE_SECRET_COW_LEVEL:
		strncpy(lpszBuffer, "The Secret Cow Level", dwMaxChars);
		break;


		///////////////////////////////////////////////////
		// Act 2 Maps
		///////////////////////////////////////////////////
	case MAP_A2_LUT_GHOLEIN:
		strncpy(lpszBuffer, "Lut Gholein", dwMaxChars);
		break;

	case MAP_A2_ROCKY_WASTE:
		strncpy(lpszBuffer, "Rocky Waste", dwMaxChars);
		break;

	case MAP_A2_DRY_HILLS:
		strncpy(lpszBuffer, "Dry Hills", dwMaxChars);
		break;

	case MAP_A2_FAR_OASIS:
		strncpy(lpszBuffer, "Far Oasis", dwMaxChars);
		break;

	case MAP_A2_LOST_CITY:
		strncpy(lpszBuffer, "Lost City", dwMaxChars);
		break;

	case MAP_A2_VALLEY_OF_SNAKES:
		strncpy(lpszBuffer, "Valley of Snakes", dwMaxChars);
		break;

	case MAP_A2_CANYON_OF_THE_MAGI:
		strncpy(lpszBuffer, "Cayon of the Magi", dwMaxChars);
		break;

	case MAP_A2_SEWERS_LEVEL_1:
		strncpy(lpszBuffer, "Sewers Level 1", dwMaxChars);
		break;

	case MAP_A2_SEWERS_LEVEL_2:
		strncpy(lpszBuffer, "Sewers Level 2", dwMaxChars);
		break;

	case MAP_A2_SEWERS_LEVEL_3:
		strncpy(lpszBuffer, "Sewers Level 3", dwMaxChars);
		break;

	case MAP_A2_HAREM_LEVEL_1:
		strncpy(lpszBuffer, "Harem Level 1", dwMaxChars);
		break;

	case MAP_A2_HAREM_LEVEL_2:
		strncpy(lpszBuffer, "Harem Level 2", dwMaxChars);
		break;

	case MAP_A2_PALACE_CELLAR_LEVEL_1:
		strncpy(lpszBuffer, "Palace Cellar Level 1", dwMaxChars);
		break;

	case MAP_A2_PALACE_CELLAR_LEVEL_2:
		strncpy(lpszBuffer, "Palace Cellar Level 2", dwMaxChars);
		break;

	case MAP_A2_PALACE_CELLAR_LEVEL_3:
		strncpy(lpszBuffer, "Palace Cellar Level 3", dwMaxChars);
		break;

	case MAP_A2_STONY_TOMB_LEVEL_1:
		strncpy(lpszBuffer, "Stony Tomb Level 1", dwMaxChars);
		break;

	case MAP_A2_HALLS_OF_THE_DEAD_LEVEL_1:
		strncpy(lpszBuffer, "Halls of the Dead Level 1", dwMaxChars);
		break;

	case MAP_A2_HALLS_OF_THE_DEAD_LEVEL_2:
		strncpy(lpszBuffer, "Halls of the Dead Level 2", dwMaxChars);
		break;

	case MAP_A2_CLAW_VIPER_TEMPLE_LEVEL_1:
		strncpy(lpszBuffer, "Claw Viper Temple Level 1", dwMaxChars);
		break;

	case MAP_A2_STONY_TOMB_LEVEL_2:
		strncpy(lpszBuffer, "Stony Tomb Level 2", dwMaxChars);
		break;

	case MAP_A2_HALLS_OF_THE_DEAD_LEVEL_3:
		strncpy(lpszBuffer, "Halls of the Dead Level 3", dwMaxChars);
		break;

	case MAP_A2_CLAW_VIPER_TEMPLE_LEVEL_2:
		strncpy(lpszBuffer, "Claw Viper Temple Level 2", dwMaxChars);
		break;

	case MAP_A2_MAGGOT_LAIR_LEVEL_1:
		strncpy(lpszBuffer, "Maggot Lair Level 1", dwMaxChars);
		break;

	case MAP_A2_MAGGOT_LAIR_LEVEL_2:
		strncpy(lpszBuffer, "Maggot Lair Level 2", dwMaxChars);
		break;

	case MAP_A2_MAGGOT_LAIR_LEVEL_3:
		strncpy(lpszBuffer, "Maggot Lair Level 3", dwMaxChars);
		break;

	case MAP_A2_ANCIENT_TUNNELS:
		strncpy(lpszBuffer, "Ancient Tunnels", dwMaxChars);
		break;

	case MAP_A2_TAL_RASHAS_TOMB_1:
	case MAP_A2_TAL_RASHAS_TOMB_2:
	case MAP_A2_TAL_RASHAS_TOMB_3:
	case MAP_A2_TAL_RASHAS_TOMB_4:
	case MAP_A2_TAL_RASHAS_TOMB_5:
	case MAP_A2_TAL_RASHAS_TOMB_6:
	case MAP_A2_TAL_RASHAS_TOMB_7:
		strncpy(lpszBuffer, "Tal Rasha's Tomb", dwMaxChars);
		break;

	case MAP_A2_TAL_RASHAS_CHAMBER:
		strncpy(lpszBuffer, "Tal Rasha's Chamber", dwMaxChars);
		break;

	case MAP_A2_ARCANE_SANCTUARY:
		strncpy(lpszBuffer, "Arcane Sanctuary", dwMaxChars);
		break;


		///////////////////////////////////////////////////
		// Act 3 Maps
		///////////////////////////////////////////////////
	case MAP_A3_KURAST_DOCKS:
		strncpy(lpszBuffer, "Kurast Docks", dwMaxChars);
		break;

	case MAP_A3_SPIDER_FOREST:
		strncpy(lpszBuffer, "Spider Forest", dwMaxChars);
		break;

	case MAP_A3_GREAT_MARSH:
		strncpy(lpszBuffer, "Great Marsh", dwMaxChars);
		break;

	case MAP_A3_FLAYER_JUNGLE:
		strncpy(lpszBuffer, "Flayer Jungle", dwMaxChars);
		break;

	case MAP_A3_LOWER_KURAST:
		strncpy(lpszBuffer, "Lower Kurast", dwMaxChars);
		break;

	case MAP_A3_KURAST_BAZAAR:
		strncpy(lpszBuffer, "Kurast Bazaar", dwMaxChars);
		break;

	case MAP_A3_UPPER_KURAST:
		strncpy(lpszBuffer, "Upper Kurast", dwMaxChars);
		break;

	case MAP_A3_KURAST_CAUSEWAY:
		strncpy(lpszBuffer, "Kurast Causeway", dwMaxChars);
		break;

	case MAP_A3_TRAVINCAL:
		strncpy(lpszBuffer, "Travinval", dwMaxChars);
		break;

	case MAP_A3_ARCHNID_LAIR:
		strncpy(lpszBuffer, "Archnid Lair", dwMaxChars);
		break;

	case MAP_A3_SPIDER_CAVERN:
		strncpy(lpszBuffer, "Spider Cavern", dwMaxChars);
		break;

	case MAP_A3_SWAMPY_PIT_LEVEL_1:
		strncpy(lpszBuffer, "Swampy Pit Level 1", dwMaxChars);
		break;

	case MAP_A3_SWAMPY_PIT_LEVEL_2:
		strncpy(lpszBuffer, "Swampy Pit Level 2", dwMaxChars);
		break;

	case MAP_A3_FLAYER_DUNGEON_LEVEL_1:
		strncpy(lpszBuffer, "Flayer Dungeon Level 1", dwMaxChars);
		break;

	case MAP_A3_FLAYER_DUNGEON_LEVEL_2:
		strncpy(lpszBuffer, "Flayer Dungeon Level 2", dwMaxChars);
		break;

	case MAP_A3_SWAMPY_PIT_LEVEL_3:
		strncpy(lpszBuffer, "Swampy Pit Level 3", dwMaxChars);
		break;

	case MAP_A3_FLAYER_DUNGEON_LEVEL_3:
		strncpy(lpszBuffer, "Flayer Dungeon Level 3", dwMaxChars);
		break;

	case MAP_A3_SEWERS_LEVEL_1:
		strncpy(lpszBuffer, "Sewers Level 1", dwMaxChars);
		break;

	case MAP_A3_SEWERS_LEVEL_2:
		strncpy(lpszBuffer, "Sewers Level 2", dwMaxChars);
		break;

	case MAP_A3_RUINED_TEMPLE:
		strncpy(lpszBuffer, "Ruined Temple", dwMaxChars);
		break;

	case MAP_A3_DISUSED_FANE:
		strncpy(lpszBuffer, "Disused Fane", dwMaxChars);
		break;

	case MAP_A3_FORGOTTEN_RELIQUARY:
		strncpy(lpszBuffer, "Forgotten Reliquary", dwMaxChars);
		break;

	case MAP_A3_FORGOTTEN_TEMPLE:
		strncpy(lpszBuffer, "Forgotten Temple", dwMaxChars);
		break;

	case MAP_A3_RUINED_FANE:
		strncpy(lpszBuffer, "Ruined Fane", dwMaxChars);
		break;

	case MAP_A3_DISUSED_RELIQUARY:
		strncpy(lpszBuffer, "Disused Reliquary", dwMaxChars);
		break;

	case MAP_A3_DURANCE_OF_HATE_LEVEL_1:
		strncpy(lpszBuffer, "Durance of Hate Level 1", dwMaxChars);
		break;

	case MAP_A3_DURANCE_OF_HATE_LEVEL_2:
		strncpy(lpszBuffer, "Durance of Hate Level 2", dwMaxChars);
		break;

	case MAP_A3_DURANCE_OF_HATE_LEVEL_3:
		strncpy(lpszBuffer, "Durance of Hate Level 3", dwMaxChars);
		break;

		///////////////////////////////////////////////////
		// Act 4 Maps
		///////////////////////////////////////////////////
	case MAP_A4_THE_PANDEMONIUM_FORTRESS:
		strncpy(lpszBuffer, "The Pandemonium Fortress", dwMaxChars);
		break;

	case MAP_A4_OUTER_STEPPES:
		strncpy(lpszBuffer, "Outer Steppes", dwMaxChars);
		break;

	case MAP_A4_PLAINS_OF_DESPAIR:
		strncpy(lpszBuffer, "Plains of Despair", dwMaxChars);
		break;

	case MAP_A4_CITY_OF_THE_DAMNED:
		strncpy(lpszBuffer, "City of the Damned", dwMaxChars);
		break;

	case MAP_A4_RIVER_OF_FLAME:
		strncpy(lpszBuffer, "River of Flame", dwMaxChars);
		break;

	case MAP_A4_THE_CHAOS_SANCTUARY:
		strncpy(lpszBuffer, "The Chaos Sanctuary", dwMaxChars);
		break;

		///////////////////////////////////////////////////
		// Act 5 Maps
		///////////////////////////////////////////////////
	case MAP_A5_HARROGATH:
		strncpy(lpszBuffer, "Harrogath", dwMaxChars);
		break;

	case MAP_A5_THE_BLOODY_FOOTHILLS:
		strncpy(lpszBuffer, "The Bloody Foothills", dwMaxChars);
		break;

	case MAP_A5_FRIGID_HIGHLANDS:
		strncpy(lpszBuffer, "Frigid Highlands", dwMaxChars);
		break;

	case MAP_A5_ARREAT_PLATEAU:
		strncpy(lpszBuffer, "Arreat Plateau", dwMaxChars);
		break;

	case MAP_A5_CRYSTALLINE_PASSAGE:
		strncpy(lpszBuffer, "Crystalline Passage", dwMaxChars);
		break;

	case MAP_A5_FROZEN_RIVER:
		strncpy(lpszBuffer, "Frozen River", dwMaxChars);
		break;

	case MAP_A5_GLACIAL_TRAIL:
		strncpy(lpszBuffer, "Glacial Trail", dwMaxChars);
		break;

	case MAP_A5_DRIFTER_CAVERN:
		strncpy(lpszBuffer, "Drifter Cavern", dwMaxChars);
		break;

	case MAP_A5_FROZEN_TUNDRA:
		strncpy(lpszBuffer, "Frozen Tundra", dwMaxChars);
		break;

	case MAP_A5_THE_ANCIENTS_WAY:
		strncpy(lpszBuffer, "The Ancients' Way", dwMaxChars);
		break;

	case MAP_A5_ICY_CELLAR:
		strncpy(lpszBuffer, "Icy Cellar", dwMaxChars);
		break;

	case MAP_A5_ARREAT_SUMMIT:
		strncpy(lpszBuffer, "Arreat Summit", dwMaxChars);
		break;

	case MAP_A5_HALLS_OF_PAIN:
		strncpy(lpszBuffer, "Halls of Pain", dwMaxChars);
		break;

	case MAP_A5_ABADDON:
		strncpy(lpszBuffer, "Abaddon", dwMaxChars);
		break;

	case MAP_A5_PIT_OF_ACHERON:
		strncpy(lpszBuffer, "Pit of Acheron", dwMaxChars);
		break;

	case MAP_A5_INFERNAL_PIT:
		strncpy(lpszBuffer, "Infernal Pit", dwMaxChars);
		break;

	case MAP_A5_NIHLATHAKS_TEMPLE:
		strncpy(lpszBuffer, "Nihlathak's Temple", dwMaxChars);
		break;

	case MAP_A5_HALLS_OF_ANGUISH:
		strncpy(lpszBuffer, "Halls of Anguish", dwMaxChars);
		break;

	case MAP_A5_HALLS_OF_VAUGHT:
		strncpy(lpszBuffer, "Halls of Vaught", dwMaxChars);
		break;	

	case MAP_A5_WORLDSTONE_KEEP_LEVEL_1:
		strncpy(lpszBuffer, "Worldstone Keep Level 1", dwMaxChars);
		break;

	case MAP_A5_WORLDSTONE_KEEP_LEVEL_2:
		strncpy(lpszBuffer, "Worldstone Keep Level 2", dwMaxChars);
		break;

	case MAP_A5_WORLDSTONE_KEEP_LEVEL_3:
		strncpy(lpszBuffer, "Worldstone Keep Level 3", dwMaxChars);
		break;

	case MAP_A5_THRONE_OF_DESTRUCTION:
		strncpy(lpszBuffer, "Throne of Destruction", dwMaxChars);
		break;

	case MAP_A5_WORLDSTONE_KEEP:
		strncpy(lpszBuffer, "The Worldstone Chamber", dwMaxChars);
		break;

	default:
		return 0;
		break;
	}

	return FALSE;
}
INT D2GetScreenSizeX()
{
	INT Sizes[2] = {640, 800};
	return Sizes[D2GFX_GetScreenSize() / 2];
}

INT D2GetScreenSizeY()
{
	INT Sizes[] = {480, 600};
	return Sizes[D2GFX_GetScreenSize() / 2];
}

DWORD GetUnitIDByXY(LONG Wx,LONG Wy, Room2* pRoom2)
{
	UnitAny* pUnit;
	if(!pRoom2)
	return 0;
	if(!pRoom2->pRoom1)
	return 0;
	if(!pRoom2->pRoom1->pUnitFirst)
	return 0;
	pUnit=pRoom2->pRoom1->pUnitFirst;
	if(!pUnit->pObjectPath)
	return 0;
	while(pUnit)
	{
		if(pUnit->dwType!=UNIT_PLAYER)
		if(Wx==pUnit->pObjectPath->dwPosX)
		if(Wy==pUnit->pObjectPath->dwPosY)
		return pUnit->dwUnitId;
		pUnit=pUnit->pListNext;
	}
	return 0;
}
PresetUnit* FindPresetUnit(DWORD dwTxtFileNo, DWORD dwType, Room2* *pRoom)
{
	for(Room2* pRoom2 = GetUnitLevel(Myself)->pRoom2First; pRoom2; pRoom2 = pRoom2->pRoom2Other)
	{
		for(PresetUnit* pUnit = pRoom2->pPreset; pUnit; pUnit = pUnit->pPresetNext)
		{
			if(pUnit->dwType == dwType && pUnit->dwTxtFileNo == dwTxtFileNo)
			{
				*pRoom = pRoom2;
				return pUnit;
			}
		}
	}

	return NULL;
}
Level* GetLevelPointerD(ActMisc *pActMisc, INT nLevel)
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
Level* GetUnitLevel(UnitAny* pUnit)
{
	if(pUnit)
		if(pUnit->pPath)
			if(pUnit->pPath->pRoom1)
				if(pUnit->pPath->pRoom1->pRoom2)
					return pUnit->pPath->pRoom1->pRoom2->pLevel;

	return NULL;
}

VOID GetPlayerPosition(LPPOINT ptPlayer)
{
	ScreenToAutomapRelative(ptPlayer, D2GetScreenSizeX()/2, (D2GetScreenSizeY()-48)/2);
}
VOID D2DrawLine(INT x,INT y,INT x2, INT y2, DWORD color)
{
	D2GFX_DrawLine(x,y,x2,y2,color,0xff);
}

VOID DrawPlayerBlob(INT xpos, INT ypos,INT col)
{
	static CHAR blines[][2] = {0,-2, 4,-4, 8,-2, 4,0, 8,2, 4,4, 0,2, -4,4, -8,2, -4,0, -8,-2, -4,-4, 0,-2};
	for (INT i = 0; i < ARRAYSIZE(blines)-1; i++)
	{
		D2GFX_DrawLine(xpos+blines[i][0], ypos+blines[i][1], xpos+blines[i+1][0], ypos+blines[i+1][1], col, 0xff);
	}
	POINT Player;
	Player.x=GetPlayerX(GetPlayerID());
	Player.y=GetPlayerY(GetPlayerID());
	POINT DrawCalc;
	MapToScreenCoords(Player);
	ScreenToAutomapRelative(&DrawCalc,Player.x,Player.y);
	D2GFX_DrawLine(DrawCalc.x,DrawCalc.y,xpos,ypos,col,0xff);

}
VOID MiniMapCell(INT xpos, INT ypos, INT actnum,INT lvlnum, INT fileno, long rx, long ry,DWORD ObjID)
{
	lvlnum = D2CLIENT_GetPlayerUnit()->pAct->pRoom1->pRoom2->pLevel->dwLevelNo;	

	if(GetPlayerArea()==LEVEL_CATACOMBS_LEVEL_4)
	{
		DRAW(17,OTHER);
	}
	if(GetPlayerArea()==LEVEL_FAR_OASIS)
	{
		DRAW(47,QUESTCOLOR);
	}
	if(GetPlayerArea()==LEVEL_HAREM_LEVEL_1)
	{
		DRAW(25,OTHER);
		DRAW(29,QUESTCOLOR);
	}
	if(GetPlayerArea()==LEVEL_HAREM_LEVEL_2)
	{
		DRAW(27,OTHER);
		DRAW(29,QUESTCOLOR);
	}
	if(GetPlayerArea()==LEVEL_PALACE_CELLAR_LEVEL_1)
	{
		DRAW(27,OTHER);
		DRAW(32,QUESTCOLOR);
	}
	if(GetPlayerArea()==LEVEL_PALACE_CELLAR_LEVEL_2)
	{
		DRAW(31,OTHER);
		DRAW(32,QUESTCOLOR);
	}
	if(GetPlayerArea()==LEVEL_PALACE_CELLAR_LEVEL_3)
	{
		DRAW(31,OTHER);
	}
	if(GetPlayerArea()==LEVEL_CANYON_OF_THE_MAGI)
	{
		INT TileNo[] = {41,40,39,38,42,43,44};

		DRAW(TileNo[cfg.RealTombX],QUESTCOLOR);
	}
	if(GetPlayerArea()==LEVEL_TAL_RASHAS_TOMB1||GetPlayerArea()==LEVEL_TAL_RASHAS_TOMB2||GetPlayerArea()==LEVEL_TAL_RASHAS_TOMB3||GetPlayerArea()==LEVEL_TAL_RASHAS_TOMB4||GetPlayerArea()==LEVEL_TAL_RASHAS_TOMB5||GetPlayerArea()==LEVEL_TAL_RASHAS_TOMB6||GetPlayerArea()==LEVEL_TAL_RASHAS_TOMB7)
	{
		DRAW(45,OTHER)
	}
	if(GetPlayerArea()==LEVEL_TRAVINCAL)
	{
		DRAW(64,QUESTCOLOR);
	}
}

VOID NewEnum() 
{ 
	UnitAny *pUnit = D2CLIENT_GetPlayerUnit(); 
	Level *pLevel = GetLevelPointerD(pUnit->pAct->pMisc, GetPlayerArea()); 

	BOOL FoundTile=false;

	for(Room2 *pRoom2 = pLevel->pRoom2First;pRoom2;pRoom2=(Room2*)pRoom2->pRoom2Other) 
	{ 
		for(PresetUnit *unit = pRoom2->pPreset;unit;unit=unit->pPresetNext) 
		{ 
			if(unit->dwType != UNIT_TYPE_TILE && unit->dwType!=UNIT_TYPE_OBJECT &&  unit->dwType!=UNIT_TYPE_NPC)
				continue;
			if(unit->dwTxtFileNo==580 && unit->dwType == UNIT_TYPE_OBJECT && GetPlayerArea()==LEVEL_BLOOD_MOOR)
				continue;
			if(unit->dwTxtFileNo==37 && unit->dwType == UNIT_TYPE_OBJECT && GetPlayerArea()==LEVEL_LOST_CITY)
				continue;
			if(unit->dwTxtFileNo==13 && unit->dwType == UNIT_TYPE_OBJECT)
				continue;
			if(unit->dwTxtFileNo==14 && unit->dwType == UNIT_TYPE_OBJECT)
				continue;
			if(unit->dwTxtFileNo==15 && unit->dwType == UNIT_TYPE_OBJECT)
				continue;
			if(unit->dwTxtFileNo==16 && unit->dwType == UNIT_TYPE_OBJECT)
				continue;
			if(unit->dwTxtFileNo==2 && unit->dwType == UNIT_TYPE_OBJECT)
				continue;
			if(unit->dwTxtFileNo==32 && unit->dwType == UNIT_TYPE_TILE && FoundTile)
				continue;
			if(unit->dwTxtFileNo==32 && unit->dwType == UNIT_TYPE_TILE && !FoundTile)
				FoundTile=true;

			PresetUnit pu; 
			pu.dwTxtFileNo = unit->dwTxtFileNo; 
			pu.dwType = unit->dwType; 
			pu.dwPosX = unit->dwPosX+pRoom2->dwPosX*5; 
			pu.dwPosY = unit->dwPosY+pRoom2->dwPosY*5; 
			POINT Calc; 
			Calc.x = pu.dwPosX; 
			Calc.y = pu.dwPosY; 
			DWORD UnitID=GetUnitIDByXY(pu.dwPosX,pu.dwPosY,pRoom2); 
			POINT DrawCalc; 
			MapToScreenCoords(Calc); 
			ScreenToAutomapRelative(&DrawCalc,Calc.x,Calc.y);
			
 			if(!IsTownLevel(GetPlayerArea()))				
			MiniMapCell(DrawCalc.x,DrawCalc.y,D2CLIENT_GetPlayerUnit()->dwAct+1,GetPlayerArea(),pu.dwTxtFileNo,pu.dwPosX,pu.dwPosY,UnitID);
		}
	}
}
VOID AbsToScreenCoords(POINT& rMapPosition)
{
	D2COMMON_AbsScreenToMap(&rMapPosition.x, &rMapPosition.y);
	rMapPosition.x -= D2CLIENT_GetMouseXOffset();
	rMapPosition.y -= D2CLIENT_GetMouseYOffset();
}
INT MakeParameter(PCHAR szInput, PCHAR* argv, CHAR splitchar, BOOL bSkipQuote)
{
	PCHAR p=szInput;
	INT i=0;
	while(*szInput!='\0')
	{
		BOOL bQuoteOn=FALSE;
		while((*szInput!=splitchar || bQuoteOn) && *szInput!='\0')
		{
			if( *szInput=='\"' || *szInput=='\'' || *szInput=='´' || *szInput=='`')
				if (bSkipQuote)
					bQuoteOn=!bQuoteOn;
			szInput++;
		}
		if (*szInput!='\0')
			*szInput++='\0';
		argv[i++]=p;
		++p+=(szInput-p);
	}

	return i; 
}