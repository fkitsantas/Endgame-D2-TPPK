#include "Houdini.h"

CHICKENTYPE TownVar[] = {
	{"Life",&v_TownLife,&v_TownLifeTimer},
	{"Mana",&v_TownMana,&v_TownManaTimer},
};
CHICKENTYPE RejuvVar[] = {
	{"Life",&v_DrinkLifeRej,&v_DrankLifeRej},
	{"Mana",&v_DrinkManaRej,&v_DrankManaRej},
};

BYTE CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin)
{
	if (dwVal == 0 || dwMaxVal == 0)
		return 0;

	BYTE iRes = (BYTE)((double)dwVal / (double)dwMaxVal * 100.0);
	if (iRes > 100)
		iRes = 100;

	return max(iRes, iMin);
}

VOID Chicken::ChickenCheck()
{
	if (GameReady(TRUE))
	{		
		if (D2CLIENT_GetPlayerUnit() && !IsTownLevel(GetPlayerArea()))
		{
			DOUBLE LifePercent = CalcPercent((GetUnitStat(Myself, STAT_HP) >> 8), (GetUnitStat(Myself, STAT_MAXHP) >> 8));
			DOUBLE ManaPercent = CalcPercent((GetUnitStat(Myself, STAT_MANA) >> 8), (GetUnitStat(Myself, STAT_MAXMANA) >> 8));
			DOUBLE CurrentLife = GetUnitStat(Myself, STAT_HP);

			if (CurrentLife==0)
				return;

			if (v_AutoPot)
			{
				if (v_DrinkLifePot > 0 && LifePercent <= v_DrinkLifePot)
				{
					if (GetTickCount() - v_DrankLifePot > v_LifePotDelay
						&& !D2COMMON_GetUnitState(Myself,AFFECT_HEALTHPOT))
					{
						if (SupplyHealth())
						{
							v_DrankLifePot = GetTickCount();
						}
					}	
				}
				if (v_DrinkManaPot > 0 && ManaPercent <= v_DrinkManaPot)
				{
					if (GetTickCount() - v_DrankManaPot > v_ManaPotDelay
						&& !D2COMMON_GetUnitState(Myself,AFFECT_MANAPOT))
					{
						if (SupplyMana())
						{
							v_DrankManaPot = GetTickCount();
						}
					}
				}
				if (v_DrinkLifeRej > 0 && LifePercent <= v_DrinkLifeRej)
				{
					if (GetTickCount() - v_DrankLifeRej > v_RejuvDelay)
					{
						if (SupplyJuv(LIFE))
						{
							//v_DrankLifeRej = GetTickCount();
							Sleep(50);
						}
						else if (v_SafeRejuv)
							D2CLIENT_ExitGame();
					}
				}
				if (v_DrinkManaRej > 0 && ManaPercent <= v_DrinkManaRej)
				{
					if (GetTickCount() - v_DrankManaRej > v_RejuvDelay)
					{
						if (SupplyJuv(MANA))
						{
							//v_DrankManaRej = GetTickCount();
							Sleep(50);
						}
						else if (v_SafeRejuv)
							D2CLIENT_ExitGame();
					}
				}
			}

			if (v_ExitLife!=0 && LifePercent <= v_ExitLife)
			{
				D2CLIENT_ExitGame();
				return;
			}
			if (v_ExitMana!=0 && ManaPercent <= v_ExitMana)
			{
				D2CLIENT_ExitGame();
				return;
			}

			if (v_TownLife!=0 && LifePercent<=v_TownLife)
			{
				if (GetTickCount() - v_TownLifeTimer > 500)
				{
					if (MakeTP() && !v_AttemptedChicken)
					{
						TownChicken(LIFE);
					}
					else
						D2CLIENT_ExitGame();
				}
			}
			if (v_TownMana!=0 && ManaPercent<=v_TownMana)
			{
				if (GetTickCount() - v_TownManaTimer > 500)
				{
					if (MakeTP() && !v_AttemptedChicken)
					{
						TownChicken(MANA);
					}
					else
						D2CLIENT_ExitGame();
				}
			}
		}
	}
}
BOOL Chicken::TownChicken(INT iType)
{
	if (cfg.PrintChicken)
	{	
		PrintConsoleString("ÿc1Escaping to Town: %s dropped below %i%%",TownVar[iType].pType,*TownVar[iType].Setting);
	}
	v_AntiTppkBusy=v_TakeNextTP=TRUE;

	*TownVar[iType].Timer=GetTickCount();
	
	if (v_SafeTown)
		v_AttemptedChicken=true;

	return TRUE;
}

BOOL Chicken::SupplyJuv(INT iType)
{
	LPSTR szItemCodes[] = {"rvl", "rvs"};
	UnitAny* pItem;

	for (INT i = 0; i < ArraySize(szItemCodes); i++)
	{
		pItem = FindItem(szItemCodes[i], STORAGE_BELT);

		if (!pItem) pItem = FindItem(szItemCodes[i], STORAGE_INVENTORY);

		if (!pItem) continue;

		if (UseItem(pItem))
		{		
			if (cfg.PrintChicken)
			{
				PrintConsoleString("ÿc1Drank a rejuv! %s dropped below %i%%",RejuvVar[i].pType,*RejuvVar[i].Setting);
			}

			*RejuvVar[i].Timer=GetTickCount();

			return TRUE;
		}
	}

	return FALSE;
}
BOOL Chicken::SupplyHealth()
{
	LPSTR szItemCodes[] = {"hp5", "hp4", "hp3", "hp2", "hp1"};
	UnitAny* pItem;

	for (INT i = 0; i < ArraySize(szItemCodes); i++)
	{
		pItem = FindItem(szItemCodes[i], STORAGE_BELT);

		if (!pItem) pItem = FindItem(szItemCodes[i], STORAGE_INVENTORY);

		if (!pItem) continue;

		if (UseItem(pItem))
		{		
			if (cfg.PrintChicken)
				PrintConsoleString("ÿc1Drank a life potion! Life dropped below %i%%",v_DrinkLifePot);

			return TRUE;
		}
	}

	return FALSE;
}
BOOL Chicken::SupplyMana()
{
	LPSTR szItemCodes[] = {"mp5", "mp4", "mp3", "mp2", "mp1"};
	UnitAny* pItem;

	for (INT i = 0; i < ArraySize(szItemCodes); i++)
	{
		pItem = FindItem(szItemCodes[i], STORAGE_BELT);

		if (!pItem) pItem = FindItem(szItemCodes[i], STORAGE_INVENTORY);

		if (!pItem) continue;

		UseItem(pItem);

		if (UseItem(pItem))
		{		
			if (cfg.PrintChicken)
				PrintConsoleString("ÿc1Drank a mana potion! Mana dropped below %i%%",v_DrinkManaPot);

			return TRUE;
		}
	}

	return FALSE;
}