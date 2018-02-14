#include "Houdini.h"
#include "ArrayEx.h"
int AreaLocaleIds[] = {0, 5055, 5054, 5053, 5052, 5051, 5050, 5049, 3714, 5047, 5046, 5045, 5044, 5043, 5042, 5041, 5040, 5039, 5038, 5037, 5036, 5035, 5034, 5033, 5032, 5031, 5030, 5029, 5028, 5027, 5026, 5025, 5024, 5023, 5022, 5021, 5020, 5019, 5018, 788, 852, 851, 850, 849, 848, 847, 846, 845, 844, 843, 842, 841, 840, 839, 838, 837, 836, 835, 834, 833, 832, 831, 830, 829, 828, 827, 826, 826, 826, 826, 826, 826, 826, 825, 824, 820, 819, 818, 817, 816, 815, 814, 813, 812, 810, 811, 809, 808, 806, 805, 807, 804, 845, 844, 803, 802, 801, 800, 799, 798, 797, 796, 795, 790, 792, 793, 794, 791, 789, 22646, 22647, 22648, 22649, 22650, 22651, 22652, 22653, 22654, 22655, 22656, 22657, 22660, 21865, 21866, 21867, 22658, 22659, 22662, 22663, 22664, 22665, 22667, 22666, 11155, 11156, 11157, 5018};

void DrawMonsters ()
{
	//Grab Player Unit and check if valid
	UnitAny *pPlayer = D2CLIENT_GetPlayerUnit ();
	if (!pPlayer)
		return;

	int dwLevelNo = pPlayer->pPath->pRoom1->pRoom2->pLevel->dwLevelNo;
	//if ((dwLevelNo == 1) || (dwLevelNo == 40) || (dwLevelNo == 75) || (dwLevelNo == 103) || (dwLevelNo == 109))
	//	return;

	for (Room1 *pRoom = pPlayer->pAct->pRoom1; pRoom; pRoom = pRoom->pRoomNext)
	{
		for (UnitAny *pUnit = pRoom->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
		{
			if (pUnit->dwType == UNIT_MONSTER)
			{
				if (!IsValidMonster(pUnit))				
					continue;

				if(cfg.DrawMonsters)
				{
					int Color = cfg.MonsterColor;

					if(pUnit->pMonsterData->fBoss & 1) Color = cfg.BossMonsterColor;
					if(pUnit->pMonsterData->fChamp & 1 || pUnit->pMonsterData->fMinion & 1) Color = cfg.ChampionMonsterColor;




					char nMonTxt[40] = "";
					if(	pUnit->dwTxtFileNo >= 420 && pUnit->dwTxtFileNo <= 424 ||
						pUnit->dwTxtFileNo == 417 || pUnit->dwTxtFileNo == 418 || 
						pUnit->dwTxtFileNo == 363 || pUnit->dwTxtFileNo == 364 || 
						pUnit->dwTxtFileNo == 357 || pUnit->dwTxtFileNo == 428)
					{
						Color=10;
					}
					else
					{
					if (D2COMMON_GetUnitStat(pUnit, 36, 0) + 300 > 399)//Physical
						strcat_s(nMonTxt, "ÿc4i");
					if (D2COMMON_GetUnitStat(pUnit, 43, 0) + 300 > 399)//Cold
						strcat_s(nMonTxt, "ÿc3i");
					if (D2COMMON_GetUnitStat(pUnit, 39, 0) + 300 > 399)//Fire
						strcat_s(nMonTxt, "ÿc1i");
					if (D2COMMON_GetUnitStat(pUnit, 41, 0) + 300 > 399)//Light
						strcat_s(nMonTxt, "ÿc9i");
					if (D2COMMON_GetUnitStat(pUnit, 45, 0) + 300 > 399)//Poison
						strcat_s(nMonTxt, "ÿc2i");
					if (D2COMMON_GetUnitStat(pUnit, 37, 0) + 300 > 399)//Magic
						strcat_s(nMonTxt, "ÿc8i");
					}

					if(cfg.DrawMonsters)
					{
						DrawCross(pUnit->pPath->xPos, pUnit->pPath->yPos, Color, true);

						if(cfg.DrawImunities)
							if (strlen(nMonTxt) > 0 && pUnit->dwMode != 0 && pUnit->dwMode != 12)
								TextHook(pUnit->pPath->xPos - 3, pUnit->pPath->yPos, 0, true, 6, 2, "%s", nMonTxt);
					}
				}
			}
			else if (pUnit->dwType == 3)
			{
				int missileColor = -1;

				if (pPlayer->dwUnitId == pUnit->dwOwnerId)
						missileColor = cfg.MyMissile;

				if ((pUnit->dwOwnerType == UNIT_PLAYER) && (missileColor == -1))
				{
					switch (GetPvPStatus(pUnit->dwOwnerId))
					{
						case PVP_NEUTRAL:
						{
							missileColor = cfg.NeutralMissile; //153;
						} 
						break;
						case PVP_HOSTILE:
						{
							missileColor = cfg.EnemyMissile; //98;
						} 
						break;
						case PVP_FRIENDLY:
						{
							missileColor = cfg.FriendMissile; //132;
						} 
						break;
					}
				}
				if ((pUnit->dwOwnerType == UNIT_MONSTER) && (missileColor == -1))
				{
					UnitAny *pOwner = D2CLIENT_FindServerSideUnit(pUnit->dwOwnerId, pUnit->dwOwnerType);
					if (!pOwner)
					missileColor = 98;
					else if (D2CLIENT_GetMonsterOwner(pOwner->dwUnitId) != -1)
					{
						switch (GetPvPStatus(D2CLIENT_GetMonsterOwner(pOwner->dwUnitId)))
						{
							case PVP_NEUTRAL:
							{
								missileColor = 153;
							}
							break;
							case PVP_HOSTILE:
							{
								missileColor = 98;
							} 
							break;
							case PVP_FRIENDLY: 
							{
								missileColor = 132;
							} 
							break;
						}
					}
				}

				if(cfg.DrawMissiles && missileColor!=-1) BoxHook(pUnit->pPath->xPos - 3, pUnit->pPath->yPos - 3, 3, 3, missileColor, 5, true);
			}
		}
	}
}

VOID DrawAutomapText()
{
	UnitAny *pUnit = D2CLIENT_GetPlayerUnit();
	wchar_t* pLocaleText;
	if (!LevelNames.IsEmpty ())
	{
		int townLevels[6] = {1, 40, 75, 103, 109, 132};

		for (int i = 0; i < LevelNames.GetSize(); i++)
		{
			LevelNameInfo* pInfo = LevelNames.ElementAt(i);

			if (pInfo->nAct != pUnit->dwAct)
				continue;

			char nLevelName[70];
			pLocaleText = D2LANG_GetLocaleText(AreaLocaleIds[pInfo->nLevelId]);
			WToC(pLocaleText, nLevelName);

			if(pInfo->nLevelId== pUnit->pAct->pMisc->dwStaffTombLevel)
			{
				strcat_s(nLevelName, sizeof(nLevelName), "ÿc2*");

				INT TombLoc[] = {12508,12514,12513,12519,12617,12698,12779};

				for(int i = 0; i < ArraySize(TombLoc); i++)
				{
					if(pInfo->nX == TombLoc[i])
					{
						cfg.RealTombX = i;

						break;
					}
				}
			}
			if(cfg.LevelNames) TextHook(pInfo->nX, pInfo->nY, 0, true, 6, 1, "%s", nLevelName);
		}
	}
}

BOOL IsValidMonster(UnitAny *pUnit)
{
	if (!pUnit)
		return false;
	
	if(pUnit->dwMode==0 || pUnit->dwMode==12) 
		return false;

	if(	pUnit->dwTxtFileNo >= 420 && pUnit->dwTxtFileNo <= 424 ||
		pUnit->dwTxtFileNo == 417 || pUnit->dwTxtFileNo == 418 || 
		pUnit->dwTxtFileNo == 363 || pUnit->dwTxtFileNo == 364 || 
		pUnit->dwTxtFileNo == 357 || pUnit->dwTxtFileNo == 428)
	{
		if(D2CLIENT_GetMonsterOwner(pUnit->dwUnitId) == Myself->dwUnitId)
			return false;

		switch (GetPvPStatus(D2CLIENT_GetMonsterOwner(pUnit->dwUnitId)))
		{
			case PVP_NEUTRAL:
			{
				return true;
				break;
			} 
			case PVP_HOSTILE:
			{
				return true;
				break;
			} 
			case PVP_FRIENDLY:
			{
				return false;
				break;
			} 
		}
		return false;
	}

	if (pUnit->dwTxtFileNo >= 110 && pUnit->dwTxtFileNo <= 113 || pUnit->dwTxtFileNo == 608 && pUnit->dwMode == 8)
		return false;
	
	if (pUnit->dwTxtFileNo == 68 && pUnit->dwMode == 14)
		return false;

	if (pUnit->dwTxtFileNo == 261 && pUnit->dwMode == 14)
		return false;

	DWORD badMonIds[] = {227, 258, 283, 326, 327, 328, 329, 330, 410, 411, 412, 413, 414, 415, 416, 366, 406,
	351, 352, 353, 266, 408, 516, 517, 518, 519, 522, 523, 543, 543, 545, 562, 563, 564, 565, 566, 574};

	for (DWORD n = 0; n < ArraySize(badMonIds) - 1; n++)
	{
		if (pUnit->dwTxtFileNo == badMonIds[n])
			return false;
	}

	if (D2COMMON_GetUnitStat(pUnit, 172, 0) == 2) 
		return false;

	wchar_t* name = D2CLIENT_GetUnitName(pUnit);
	char tmp[52];
	WToC(name, tmp);

	if ((strcmp(tmp,"an evil force") == 0) || (strcmp(tmp, "dummy") == 0))
		return false;

	return true;
}
DWORD __declspec(naked) __fastcall D2CLIENT_GetUnitName_STUB(DWORD UnitAny)
{
	__asm
	{
		mov eax, ecx
		jmp D2CLIENT_GetUnitName_I
	}
}