#include "Houdini.h"
#include "Automap.h"

INT nLastLevel = 0;
INT nWaypoints[] = {119, 157, 156, 402, 323, 288, 237, 324, 238, 496, 511, 494};
PINT iDummy=0;
DWORD TimeOn;

PCHAR ChickSetting[] = {"ÿc1OFF","ÿc2Town","ÿc2Tele","ÿc2Exit","ÿc2Warn"};

STATDISPLAY STATSTRUCT[] = {
	{"Fire Resist",STAT_RES,STAT_FIRERESIST,1},
	{"Cold Resist",STAT_RES,STAT_COLDRESIST,3},
	{"Light Resist",STAT_RES,STAT_LIGHTNINGRESIST,9},
	{"Poison Resist",STAT_RES,STAT_POISONRESIST,2},
	{"Magic Find %",STAT_OTHER,STAT_MAGICFIND,8},
	{"Gold Find %",STAT_OTHER,STAT_GOLDFIND,8},
	{"Faster Cast",STAT_OTHER,STAT_FASTERCAST,8},
	{"Faster Attack",STAT_OTHER,STAT_IAS,8},
};
CONFIGDISPLAY CONFIGSTRUCT[] = {
	{"Town Life %",CFG_INT,&v_TownLife},
	{"Quit Life %",CFG_INT,&v_ExitLife},
	{"Town Mana %",CFG_INT,&v_TownMana},
	{"Quit Mana %",CFG_INT,&v_ExitMana},
	{"",CFG_BLANK,&iDummy},
	{"Use Auto Potion",CFG_BOOL,&v_AutoPot},
	{"Life Potion %",CFG_INT,&v_DrinkLifePot},
	{"Life Rejuv %",CFG_INT,&v_DrinkLifeRej},
	{"Mana Potion %",CFG_INT,&v_DrinkManaPot},
	{"Mana Rejuv %",CFG_INT,&v_DrinkManaRej},
	{"",CFG_BLANK,&iDummy},
	{"Chicken Hostile",CFG_ANTI,&v_ChickenHostile},
	{"Chicken Namelock",CFG_ANTI,&v_ChickenNamelock},
	{"",CFG_BLANK,&iDummy},
	{"Check Safe Rejuv",CFG_BOOL,&v_SafeRejuv},
	{"Check Safe Town",CFG_BOOL,&v_SafeTown},
	{"",CFG_BLANK,&iDummy},
	{"Hostile Target Only",CFG_BOOL,&v_HostileTarget},
	{"No Portal Delay",CFG_BOOL,&v_NoPortalDelay},
	{"Use Death Message",CFG_BOOL,&v_UseDeathMessage},
	{"",CFG_BLANK,&iDummy},
	{"Anti-Minimize",CFG_BOOL,&v_NoMinimize},
	{"Clean Screenshots",CFG_BOOL,&v_CS},
	{"Blind Spot",CFG_BLIND,&v_Blind},
};
VOID GetVectorArrows()
{

		LPLEVELEXIT pLevels[0x40] = {NULL};

		for (INT i = 0; i < v_CurrentVectors.GetSize(); i++)
			delete v_CurrentVectors[i];

		v_CurrentVectors.RemoveAll();

		if (IsTownLevel(GetPlayerArea()))
			return;

		v_GlobalMap->CreateCollisionMap();
		v_GlobalMap->GetLevelExits(pLevels, 0x40);

		for (INT i = 0; i < ArraySize(nWaypoints); i++)
		{
			Room2* pRoom;
			PresetUnit* pUnit = FindPresetUnit(nWaypoints[i], UNIT_TYPE_OBJECT, &pRoom);
			if (!pUnit)
				continue;

			LPVECTOR pVect = new VECTOR;
			pVect->nType = VECTOR_WAYPOINT;
			pVect->ptPos.x = ((pRoom->dwPosX * 5) + pUnit->dwPosX) * 32;
			pVect->ptPos.y = ((pRoom->dwPosY * 5) + pUnit->dwPosY) * 32;
			pVect->dwTarget = nWaypoints[i];
			pVect->dwTargetType = UNIT_TYPE_OBJECT;


			POINT Calc; 
			Calc.x = pVect->ptPos.x; 
			Calc.y = pVect->ptPos.y; 
			DWORD UnitID = pVect->dwTarget; 

			POINT DrawCalc; 
			MapToScreenCoords(Calc); 
			ScreenToAutomapRelative(&DrawCalc,Calc.x,Calc.y);

			GlobalVectors.WPID=UnitID;
			GlobalVectors.WPPos.x=DrawCalc.x;
			GlobalVectors.WPPos.y=DrawCalc.y;

			v_CurrentVectors.Add(pVect);
			break;
		}

		for (INT j = 0; j < ArraySize(MapVectors); j++)
		{
			if (MapVectors[j].dwCurrentLevel != GetUnitLevel(Myself)->dwLevelNo)
				continue;

			if (MapVectors[j].dwTargetType == TARGET_LEVEL)
			{
				for (INT i = 0; i < 0x40; i++)
				{
					if (!pLevels[i])
						break;

					if (MapVectors[j].dwTarget != pLevels[i]->dwTargetLevel)
						continue;

					for (INT k = 0; k < v_CurrentVectors.GetSize(); k++)
					{
						if (v_CurrentVectors[k]->nType == MapVectors[j].dwVectorType)
							goto ContinueIt;
					}

					LPVECTOR pVect = new VECTOR;
					pVect->ptPos.x = pLevels[i]->ptPos.x * 32;
					pVect->ptPos.y = pLevels[i]->ptPos.y * 32;
					pVect->nType = MapVectors[j].dwVectorType;
					pVect->dwTarget = MapVectors[j].dwTarget;
					pVect->dwTargetType = MapVectors[j].dwTargetType;
					v_CurrentVectors.Add(pVect);
				}
			}
			else
			{
				Room2* pRoom;
				PresetUnit* pUnit = FindPresetUnit(MapVectors[j].dwTarget, MapVectors[j].dwTargetType, &pRoom);
				UnitAny *Me = D2CLIENT_GetPlayerUnit();

				if (!pUnit) continue;

				if (MapVectors[j].dwCurrentLevel==MAP_A2_CANYON_OF_THE_MAGI)
				{
					for (int i = 0; i < LevelNames.GetSize(); i++)
					{
						LevelNameInfo* pInfo = LevelNames.ElementAt(i);

						if (pInfo->nAct != Me->dwAct) continue;

						if (pInfo->nLevelId== Me->pAct->pMisc->dwStaffTombLevel)
						{							
							INT TombLoc[] = {12508,12514,12513,12519,12617,12698,12779};

							for (int i = 0; i < ArraySize(TombLoc); i++)
							{
								if (pInfo->nX == TombLoc[i])
								{
									cfg.RealTombX = i;

									break;
								}
							}
						}
					}

					INT TombFile[] = {41,40,39,38,42,43,44};

					for (int i=0;i<ArraySize(TombFile);i++)
					{
						if (pUnit->dwTxtFileNo==TombFile[i]&&cfg.RealTombX==i)
						{
							LPVECTOR pVect = new VECTOR;
							pVect->ptPos.x = ((pRoom->dwPosX * 5) + pUnit->dwPosX) * 32;
							pVect->ptPos.y = ((pRoom->dwPosY * 5) + pUnit->dwPosY) * 32;
							pVect->nType = MapVectors[j].dwVectorType;
							pVect->dwTargetType = MapVectors[j].dwTargetType;
							v_CurrentVectors.Add(pVect);
						}
					}
				}
				for (INT k = 0; k < v_CurrentVectors.GetSize(); k++)
				{
					if (v_CurrentVectors[k]->nType == MapVectors[j].dwVectorType && MapVectors[j].dwCurrentLevel != MAP_A2_CANYON_OF_THE_MAGI)
						goto ContinueIt;
				}

				
				if (MapVectors[j].dwCurrentLevel!=MAP_A2_CANYON_OF_THE_MAGI)
				{
					LPVECTOR pVect = new VECTOR;
					pVect->ptPos.x = ((pRoom->dwPosX * 5) + pUnit->dwPosX) * 32;
					pVect->ptPos.y = ((pRoom->dwPosY * 5) + pUnit->dwPosY) * 32;
					pVect->nType = MapVectors[j].dwVectorType;
					pVect->dwTargetType = MapVectors[j].dwTargetType;
					v_CurrentVectors.Add(pVect);
				}
			}
ContinueIt:
			continue;
		}
}
VOID DrawVectorArrows()
{
	if(!GameReady(FALSE) || IsTownLevel(GetPlayerArea()))
		return;

	INT nColors[] = {cfg.NextColor, cfg.PrevColor, cfg.WaypointColor, cfg.SpecialColor};

	POINT ptPlayer, ptPos;
	GetPlayerPosition(&ptPlayer);

	for(INT i = 0; i < v_CurrentVectors.GetSize(); i++)
	{
		if(nColors[v_CurrentVectors[i]->nType - 1] == -1) continue;

		ScreenToAutomap(&ptPos, v_CurrentVectors[i]->ptPos.x, v_CurrentVectors[i]->ptPos.y);
		DrawPlayerBlob(ptPos.x, ptPos.y, nColors[v_CurrentVectors[i]->nType - 1]);
	}

	return;
}
void DrawPlines(void)
{
	POINT Player;
	POINT Target;
	if(Auto.GetPlayerID()!=GetPlayerID()) 
	if(FindUnit(Auto.GetPlayerID(),0))
	if(((UnitAny*)FindUnit(Auto.GetPlayerID(),0))->pPath)
	{
		UnitAny* NearstPlayer=(UnitAny*)FindUnit(Auto.GetPlayerID(),0);
		if(NearstPlayer->pPlayerData)
		{			
			Player.x=GetPlayerX(GetPlayerID());
			Player.y=GetPlayerY(GetPlayerID());
			Target.x=NearstPlayer->pPath->xPos;
			Target.y=NearstPlayer->pPath->yPos;
			POINT DrawCalc,TargetCalc;
			MapToScreenCoords(Player);
			ScreenToAutomapRelative(&DrawCalc,Player.x,Player.y);
			MapToScreenCoords(Target);
			ScreenToAutomapRelative(&TargetCalc,Target.x,Target.y);
			D2DrawLine(DrawCalc.x,DrawCalc.y,TargetCalc.x,TargetCalc.y,11);
		}
	}
}
void DrawYonTarget() 
{
	if (v_HideConsole) return;

	if(Auto.GetPlayerID()!=GetPlayerID()||Auto.GetPlayerID()!=NULL)
	{
		if(FindUnit(Auto.GetPlayerID(),0))
		{
			POINT PlayerPos;
			UnitAny* TargetUnit = (UnitAny*)FindUnit(Auto.GetPlayerID(),0);
			RosterUnit *pUnit=(RosterUnit*)*(DWORD*)0x6FBCC080;
			pUnit=pUnit->pNext;
			if(TargetUnit->dwUnitId!=GetPlayerID())
			{
				PlayerPos.x=TargetUnit->pPath->xPos;
				PlayerPos.y=TargetUnit->pPath->yPos;
				MapToScreenCoords(PlayerPos);
				if(PlayerPos.x!=NULL&&PlayerPos.y!=NULL)
				{
					D2GFX_DrawRectangle(PlayerPos.x-30,PlayerPos.y-55,PlayerPos.x+20,PlayerPos.y+35,cfg.AimBoxColor,0);
					D2DrawRectFrame(PlayerPos.x-30,PlayerPos.y-55,PlayerPos.x+20,PlayerPos.y+35);
					DrawTextToScreen("¿",PlayerPos.x-11,PlayerPos.y+11,4,3);
				}
			}
		}
	}
}
VOID DrawBox(INT X1, INT Y1, INT X2, INT Y2, INT nLineColor, INT nBackGroundColor, INT nTrans)
{
	D2GFX_DrawRectangle(X1, Y1, X2, Y2, nBackGroundColor, nTrans);
	D2GFX_DrawLine(X1, Y1, X1 + (X2 - X1), Y1, nLineColor, -1);
	D2GFX_DrawLine(X1 + (X2 - X1), Y1, X1 + (X2 - X1), Y1 + (Y2 - Y1), nLineColor, -1);
	D2GFX_DrawLine(X1 + (X2 - X1), Y1 + (Y2 - Y1), X1, Y1 + (Y2 - Y1), nLineColor, -1);
	D2GFX_DrawLine(X1, Y1 + (Y2 - Y1), X1, Y1, nLineColor, -1);
}
void LifeManaPercents(void) 
{
	if (v_HideConsole) return;

	int LifeWarning;
	int ManaWarning;
	
	char Buffer[100];
	DWORD MaxLife= GetMaxLife();
	DWORD MaxMana= GetMaxMana();
	DWORD CurrentLife=GetCurrentLife();
	DWORD CurrentMana=GetCurrentMana();

	DWORD LifePercent= GetPercent(CurrentLife,MaxLife);
	if(LifePercent > 100)
		LifePercent = 100;

	DWORD ManaPercent=GetPercent(CurrentMana,MaxMana);
	if(ManaPercent > 100)
		ManaPercent = 100;

	if (LifePercent > 79)//80+
	{
		LifeWarning = 2;
	}
	if (LifePercent  > 59 && LifePercent < 80)//60 - 79
	{
		LifeWarning = 9;
	}
	if (LifePercent >= 0 && LifePercent < 60)//0-59
	{
		LifeWarning = 1;
	}

	if (ManaPercent > 79)
	{
		ManaWarning = 2;
	}
	if (ManaPercent > 59 && ManaPercent  < 80)
	{
		ManaWarning = 9;
	}
	if (ManaPercent >= 0 && ManaPercent < 60)
	{
		ManaWarning = 1;
	}

	int x = 59;
	int x2 = 715;
	int y = 535;

	D2GFX_DrawRectangle(x-5,y,x+32,y+18,8,2);
	D2DrawRectFrame(x-5,y,x+32,y+18);

	D2GFX_DrawRectangle(x2,y,x2+35,y+18,8,2); 
	D2DrawRectFrame(x2,y,x2+35,y+18); 

	sprintf(Buffer,"%i%%",LifePercent);
	DrawTextToScreenCenter(Buffer,x+13,y+14,LifeWarning,4);

	sprintf(Buffer,"%i%%",ManaPercent);
	DrawTextToScreenCenter(Buffer,x2+17,y+14,ManaWarning,4);
}
BOOL IsScreenClear(INT nScreen)
{
	BOOL bClear = TRUE;

	if((nScreen == 0 || nScreen == 3) && bClear)
		bClear = (!GetUiVar(UI_PARTY) && !GetUiVar(UI_MERC) && !GetUiVar(UI_CUBE) && !GetUiVar(UI_WPMENU) && !GetUiVar(UI_CHARACTER));

	if((nScreen == 1 || nScreen == 3) && bClear)
		bClear = (!GetUiVar(UI_INVENTORY) && !GetUiVar(UI_SKILL));

	if((nScreen == 2 || nScreen == 3) && bClear)
		bClear = (!GetUiVar(UI_NPCSHOP) && !GetUiVar(UI_STASH) && !GetUiVar(UI_MSGS) && !GetUiVar(UI_TRADE) && !GetUiVar(UI_MAINMENU));

	return bClear;
}
BOOL DrawMessageConsole()
{
	CArrayEx<PCONSOLEMESSAGE, PCONSOLEMESSAGE> TempMessages;

	if (v_ConfigBox || v_ConsoleList.IsEmpty() || !IsScreenClear(3))
	{
		if (v_ConsoleLogs)
			v_ConsoleLogs=false;
		return FALSE;
	}

	if (v_ConsoleLogs)
	{
		if (GetUiVar(UI_MSGS)||GetUiVar(UI_MAINMENU)||GetUiVar(UI_CONFIG))
		{
			v_ConsoleLogs = FALSE;
			return FALSE;
		}

		if (cfg.IncludeConsole&&cfg.PlayerInfo || !cfg.IncludeConsole&&!v_HideConsole)
		{
			
		D2GFX_DrawRectangle(42, 30, 758, 450, 8, 2);
		D2GFX_DrawLine(42, 30, 758, 30, 0, -1);
		D2GFX_DrawLine(758, 30, 758, 450, 0, -1);
		D2GFX_DrawLine(758, 450, 42, 450, 0, -1);
		D2GFX_DrawLine(42, 450, 42, 30, 0, -1);

			DWORD dwOldSize = D2WIN_SetTextSize(4);
		
			for(INT i = 0; i < v_ConsoleList.GetSize(); i++)
				D2WIN_DrawText(v_ConsoleList[i]->Message, 42, 47 + (i * 16), 0, -1);

			D2WIN_SetTextSize(dwOldSize);
		}
	}
	else
	{
		for(INT i = ((v_ConsoleList.GetSize() > 3) ? v_ConsoleList.GetSize() - 3 : 0); i < v_ConsoleList.GetSize(); i++)
		{
			if(v_ConsoleDelay == 0 || ((GetTickCount() - v_ConsoleList[i]->TimeStamp) < v_ConsoleDelay))
				TempMessages.Add(v_ConsoleList[i]);
		}

		if(TempMessages.IsEmpty())
			return FALSE;

		INT nConsoleHeight = (TempMessages.GetSize() - 1) * 15;
		INT nConsoleWidth = 385;

		if (cfg.IncludeConsole&&cfg.PlayerInfo || !cfg.IncludeConsole&&!v_HideConsole)
		{
			
		D2GFX_DrawRectangle(127, 525 - nConsoleHeight, 672, 550, 8, 2);
		D2GFX_DrawLine(127, 525 - nConsoleHeight, 672, 525 - nConsoleHeight, 0, -1);
		D2GFX_DrawLine(672, 525 - nConsoleHeight, 672, 550, 0, -1);
		D2GFX_DrawLine(672, 550, 127, 550, 0, -1);
		D2GFX_DrawLine(127, 550, 127, 525 - nConsoleHeight, 0, -1);

			DWORD dwSize = D2WIN_SetTextSize(4);

			for(INT i = 0; i < TempMessages.GetSize(); i++)
				D2WIN_DrawText(TempMessages[i]->Message, 142, (544 - nConsoleHeight) + (i * 16) + 3, 0, -1);

			D2WIN_SetTextSize(dwSize);
		}
	}

	return TRUE;
}
void RevealActOnce() 
{
	if(GetPlayerArea()!=nLastLevel) 
	{
		UnitAny *pUnit = D2CLIENT_GetPlayerUnit (); 
		Level *pLevel = GetLevelPointerD(pUnit->pAct->pMisc, GetPlayerArea());
		RevealLevel(pLevel);
		GlobalVectors.OtherPos.x=0;
		GlobalVectors.OtherPos.y=0;
		GlobalVectors.QuestPos.x=0;
		GlobalVectors.QuestPos.y=0;
		GlobalVectors.WPPos.x=0;
		GlobalVectors.WPPos.y=0;
		GlobalVectors.SpecialPos.x=0;
		GlobalVectors.SpecialPos.y=0;
	}
}
void ScreenToAutomap(POINT *ptPos, int x, int y)
{
	ptPos->x = ((x - y)/2/(*(INT*)p_D2CLIENT_Divisor))-(*p_D2CLIENT_Offset).x+8;
	ptPos->y = ((x + y)/4/(*(INT*)p_D2CLIENT_Divisor))-(*p_D2CLIENT_Offset).y-8;
	if(D2CLIENT_GetAutomapSize())
	{
		--ptPos->x;
		ptPos->y += 5;
	}
}
void DrawCross(INT nX, INT nY, DWORD dwColor, BOOL Automap)
{
	POINT nPos = {nX, nY};
	if (Automap)
	{
		if (!p_D2CLIENT_AutomapOn)
			return;
		ScreenToAutomap(&nPos, nX * 32, nY * 32);
	}

	CHAR szLines[][2] = {0,-2, 4,-4, 8,-2, 4,0, 8,2, 4,4, 0,2, -4,4, -8,2, -4,0, -8,-2, -4,-4, 0,-2};
	for(INT x = 0; x < ArraySize(szLines) - 1; x++)
		D2GFX_DrawLine(nPos.x + szLines[x][0], nPos.y + szLines[x][1], nPos.x + szLines[x+1][0], nPos.y + szLines[x+1][1], dwColor, -1);
}

void TextHook(INT xPos, INT yPos, DWORD dwColor, BOOL Automap, INT dwFont, INT dwCenter, LPSTR Text,...)
{
	POINT nPos = {xPos, yPos};
	DWORD dwOldSize, wWidth, dwFileNo;

	if (Automap)
	{
		if (!p_D2CLIENT_AutomapOn)
		return;
		ScreenToAutomap(&nPos, xPos * 32, yPos * 32);
	}

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

void BoxHook(INT x, INT y, INT xSize, INT ySize, DWORD dwColor, DWORD Trans, BOOL Automap)
{
	POINT Start = {x, y};
	POINT End = {xSize, ySize};
	if (Automap) 
	{
		if (!p_D2CLIENT_AutomapOn)
		return;
		ScreenToAutomap(&Start, x * 32, y * 32);
	}
	return D2GFX_DrawRectangle(Start.x, Start.y, End.x + Start.x, End.y + Start.y, dwColor, Trans);
}

void Information()
{
	if (v_HideConsole) return;

	if (cfg.ExtraInfo)
	{
		INT color1=4,color2=1,color3=2,space=16,font=4;

		INT X=750,Y=475;

		TextHook(X,Y,0,0,font,0,"ÿc%iFlashing: ",color1);
		v_Flashing ? TextHook(X,Y,0,0,font,-1,"ÿc%iON",color3) : TextHook(X,Y,0,0,font,-1,"ÿc%iOFF",color2);
		Y-=space;

		TextHook(X,Y,0,0,font,0,"ÿc%iChicken Hostile: ",color1);
		TextHook(X,Y,0,0,font,-1,"%s",ChickSetting[v_ChickenHostile]);
		Y-=space;

		TextHook(X,Y,0,0,font,0,"ÿc%iAuto Pot: ",color1);
		v_AutoPot ? TextHook(X,Y,0,0,font,-1,"ÿc%iON",color3) : TextHook(X,Y,0,0,font,-1,"ÿc%iOFF",color2);
		Y-=space;

		TextHook(X,Y,0,0,font,0,"ÿc%iExit-Life: ",color1);
		v_ExitLife>0 ? TextHook(X,Y,0,0,font,-1,"ÿc%i%i",color3,v_ExitLife) : TextHook(X,Y,0,0,font,-1,"ÿc%iOFF",color2);
		Y-=space;

		TextHook(X,Y,0,0,font,0,"ÿc%iToggle: ",color1);
		LeftAA ? TextHook(X,Y,0,0,font,-1,"ÿc%iLeft",color2) : TextHook(X,Y,0,0,font,-1,"ÿc%iRight",color2);
	}
}
void DrawAutomapInfos(void)
{
	if(!v_ClearMap && !v_HideConsole)
	{
		DrawAutomapText(); DrawMonsters();
		if(cfg.DrawVectors) DrawVectorArrows();
		if(cfg.PlayerLine) DrawPlines();
	}
	else if(cfg.PlayerInfo && !v_HideConsole)
	{
		DrawAutomapText(); DrawMonsters();
		if(cfg.DrawVectors) DrawVectorArrows();
		if(cfg.PlayerLine) DrawPlines();
	}

	if(cfg.PlayerInfo&&cfg.IHPKTag&&!v_HideConsole)
		TextHook(780,12,0,0,13,0,"ÿc3Endgame Member: Miguel", D2CLIENT_FPS, *p_D2CLIENT_Skip, D2CLIENT_Ping);

	DrawOriginal();
}

BOOL IsHostile(DWORD UnitID)
{
	if (GetPvPStatus(UnitID)==1)
	{
		return TRUE;
	}

	return FALSE;
}
BOOL IsTarget(DWORD UnitID)
{
	if (Auto.GetPlayerID()==UnitID)
	{
		return TRUE;
	}

	return FALSE;
}

int Players()
{
	int i;
	RosterUnit *pUnit=(RosterUnit*)*(DWORD*)PlayerRoster;
	for(i=0;pUnit!=NULL;i++)
	pUnit=pUnit->pNext;
	return i;
}

int GetTextLength(char* szwText, int font)
{
	wchar_t Buffer[500];
	MultiByteToWideChar(0, 1, szwText, 500, Buffer, 500);
	DWORD dwOld=D2WIN_SetTextSize(font);
	DWORD dwXLength=D2WIN_GetTextLength(Buffer);
	D2WIN_SetTextSize(dwOld);
	return dwXLength;
}
/*
BOOL TargetList()
{
	if (v_HideConsole)
		return FALSE;

	INT X=10,
		X2=X+GetTextLength("Name",1)+125,
		X3=X2+GetTextLength("Class",1)+55,
		Y=475;

	if (Players()>1)
	{	
		RosterUnit*pUnit=(RosterUnit*)*(DWORD*)PlayerRoster;
		pUnit=pUnit->pNext;

		Y-=(16*(Players()-1));

		TextHook(X,Y,0,false,1,-1,"ÿc3Name");
		TextHook(X2,Y,0,false,1,-1,"ÿc3Class");
		TextHook(X3,Y,0,false,1,-1,"ÿc3Level");

		Y+=16;

		char *t_Status, *t_Target;

		for (int i=0;i<Players()-1;i++,Y+=16)
		{
			t_Status = IsTarget(pUnit->dwUnitId) ? "ÿc9**" : "";
			t_Target = IsHostile(pUnit->dwUnitId) ? "ÿc8**" : "";

			TextHook(X,Y,0,false,1,-1,"%s%sÿc9%s",t_Status,t_Target,GetNameByID(pUnit->dwUnitId));
			TextHook(X2,Y,0,false,1,-1,"ÿc1%s",GetClassByName(GetNameByID(pUnit->dwUnitId)));
			TextHook(X3,Y,0,false,1,-1,"ÿc8%i",GetLevelByName(GetNameByID(pUnit->dwUnitId)));

			pUnit=pUnit->pNext;
		}

		return TRUE;
	}

	TextHook(X,Y,0,false,1,-1,"ÿc3No Players");

	return TRUE;
}

*/
VOID ChangeValue(INT i)
{
	INT iTemp; BOOL bTemp;

	switch (CONFIGSTRUCT[i].Type)
	{
	case CFG_BOOL:
		bTemp=!(*(PBOOL)CONFIGSTRUCT[i].Var);
		*(PBOOL)CONFIGSTRUCT[i].Var=bTemp;
		break;
	case CFG_ANTI:
		iTemp=*(PINT)CONFIGSTRUCT[i].Var+1;
		*(PINT)CONFIGSTRUCT[i].Var=iTemp>4 ? 0 : iTemp;
		break;
	case CFG_BLIND:
		iTemp=*(PINT)CONFIGSTRUCT[i].Var+1;
		*(PINT)CONFIGSTRUCT[i].Var=iTemp>5 ? 1 : iTemp;
		break;
	case CFG_INT:
		iTemp=*(PINT)CONFIGSTRUCT[i].Var+5;
		*(PINT)CONFIGSTRUCT[i].Var=iTemp>99 ? 0 : iTemp;
		break;
	default:
		break;
	}
}
VOID ConfigInteract(INT MouseX, INT MouseY)
{
	INT Y=82, i=MouseX<400 ? 0 : 14, j=MouseX<400 ? 14 : ArraySize(CONFIGSTRUCT);

	for (i;i<j;i++,Y+=16)
	{
		if (MouseY>=Y && MouseY<=(Y+15))
		{
			ChangeValue(i);
		}
	}
}
INT GetDiffPenalty()
{
	INT iPen=0;

	wchar_t* pLocaleText;
	int DiffIds[] = {5156, 5155, 5154};
	char nDiff[25];
	pLocaleText = D2LANG_GetLocaleText(DiffIds[D2CLIENT_GetDifficulty()]);
	WToC(pLocaleText, nDiff);

	if(strncmp(nDiff,"Nightmare",strlen("Nightmare")) == 0)
	{
		iPen=40;
	}
	else if(strncmp(nDiff,"Hell",strlen("Hell")) == 0)
	{
		iPen=100;
	}

	return iPen;
}
INT TrueResist(INT iStat, BOOL bMax)
{
	INT iValue=GetUnitStat(Myself,iStat);
	iValue-=GetDiffPenalty();

	if (bMax)
	{
		INT iMax = GetUnitStat(Myself,(iStat+1)) + 75 > 95 ? 95 : 75;
		return iValue>iMax ? iMax : iValue;
	}

	return iValue;
}
BOOL DrawConfigBox()
{
	if (v_ConsoleLogs || !IsScreenClear(3))
	{
		if (v_ConfigBox)
			v_ConfigBox=FALSE;

		return FALSE;
	}
	else if (v_ConfigBox && !v_HideConsole)
	{
		D2GFX_DrawRectangle(42, 30, 758, 450, 0x00, 2);
		D2GFX_DrawLine(42, 30, 758, 30, 0x08, -1);
		D2GFX_DrawLine(42, 52, 758, 52, 0x08, -1);
		D2GFX_DrawLine(758, 30, 758, 450, 0x08, -1);
		D2GFX_DrawLine(758, 330, 42, 330, 0x08, -1);
		D2GFX_DrawLine(758, 450, 42, 450, 0x08, -1);
		D2GFX_DrawLine(42, 450, 42, 30, 0x08, -1);

		BOOL bTemp; PCHAR pChar=""; CHAR bChar[200];
		INT nFont=8,X=110,iTemp;

		TextHook(42 + (716/2),48,0,0,nFont,1,"ÿc8Config Box");

		for (INT i=0,Y=94;i<ArraySize(CONFIGSTRUCT);i++,Y+=16)
		{
			if (i==14)
			{
				X+=350; Y=94;
			}
			if (CONFIGSTRUCT[i].Type==CFG_INT||CONFIGSTRUCT[i].Type==CFG_BLIND)
			{
				!(*(PINT)CONFIGSTRUCT[i].Var) ? sprintf(bChar,"ÿc1OFF") : sprintf(bChar,"ÿc2%i",*(PINT)CONFIGSTRUCT[i].Var);

				TextHook(X,Y,0,0,nFont,-1,"ÿc3%s: %s",CONFIGSTRUCT[i].Setting,bChar);
			}
			else if (CONFIGSTRUCT[i].Type==CFG_BOOL)
			{
				pChar = *(PBOOL)CONFIGSTRUCT[i].Var ? "ÿc2True" : "ÿc1False";

				TextHook(X,Y,0,0,nFont,-1,"ÿc3%s: %s",CONFIGSTRUCT[i].Setting,pChar);
			}
			else if (CONFIGSTRUCT[i].Type==CFG_ANTI)
			{
				TextHook(X,Y,0,0,nFont,-1,"ÿc3%s: ÿc2%s",CONFIGSTRUCT[i].Setting,ChickSetting[(*(PINT)CONFIGSTRUCT[i].Var)]);
			}
			else if (CONFIGSTRUCT[i].Type==CFG_BLANK)
			{
				continue;
			}
		}

		X=110;

		for (INT i=0,Y=370;i<ArraySize(STATSTRUCT);i++,Y+=16)
		{
			if(i==4)
			{
				X+=350; Y=370;
			}

			INT S=STATSTRUCT[i].Stat;
			
			switch (STATSTRUCT[i].Type)
			{
			case STAT_RES:
				{
					TextHook(X,Y,0,0,nFont,-1,"ÿc4%s: ÿc%i%i [%i]",STATSTRUCT[i].Name,STATSTRUCT[i].Color,TrueResist(S,TRUE),TrueResist(S,FALSE));
					break;
				}
			case STAT_OTHER:
				{
					TextHook(X,Y,0,0,nFont,-1,"ÿc4%s: ÿc%i%i",STATSTRUCT[i].Name,STATSTRUCT[i].Color,GetUnitStat(Myself,STATSTRUCT[i].Stat));
					break;
				}
			default:
				break;
			}
		}
	}

	return TRUE;
}

void ScreenHooks(void)
{
	if (cfg.PlayerInfo&&cfg.AimBoxEnabled) DrawYonTarget();

//if (cfg.PlayerInfo&&cfg.ShowTargetList) TargetList();

	if (cfg.PlayerInfo) 
		Information();
	if(TargetToggled) TargetWindow();
		if(DrinkToggled) DrinkWindow();
		if(StatToggled) StatWindow(GetPlayerID());
		if(WinampToggled) WinampWindow();
		if(ChickenToggled) ChickenWindow();
		//if(UtilToggled) UtilWindow();


}
void TimeStamp()
{
	if (v_HideConsole) return;

	char szTime[200];
	int nTime = ((GetTickCount () - nGameTimer) / 1000);
	sprintf_s(szTime, 200, "%.2d:%.2d:%.2d", nTime/3600, (nTime/60)%60, nTime%60);

	char curTime[200]; time_t ttTime; time(&ttTime);
	strftime(curTime, 100, "%m/%d/%y",localtime(&ttTime));

	char curTime2[200]; time(&ttTime);
	strftime(curTime2, 100, "%H:%M:%S",localtime(&ttTime));

}

void GameDraw(void)
{
	RevealActOnce();

	NewEnum();

	if (cfg.PlayerInfo&&cfg.ShowLifeManaPer) LifeManaPercents();

	if (cfg.PlayerInfo&&cfg.TimeStamp) TimeStamp();

	if (DrawPlayerInventory) 
	{
		UnitAny* PlayerUnit=(UnitAny*)FindUnit(Auto.GetPlayerID(),0);

		int xPlayer=562; int yPlayer=310;

		if (!PlayerUnit || PlayerUnit->pPath && GetDistance(PlayerX(),
			PlayerY(),PlayerUnit->pPath->xPos,PlayerUnit->pPath->yPos)>60) 
		{ 
			DrawPlayerInventory=false; ToggleInventory(false); UninstallPatches();
		} 
		else 
		{
			DrawTextToScreenCenter(Auto.GetPlayerNAME(),xPlayer,yPlayer,8,0);

			if(IsInventoryOpen()==false) 
			{
				UninstallPatches();
				DrawPlayerInventory=false;
			}
		}
	}
}
void GameDraw2(void)
{
	if(GetUnitLevel(Myself)->dwLevelNo != nLastLevel)
	{
		GetVectorArrows();

		nLastLevel = GetUnitLevel(Myself)->dwLevelNo;
	}

	DrawMessageConsole();
	DrawConfigBox();
}