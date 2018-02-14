#include "Houdini.h"
Control* pD2WinEditBox;
wchar_t wszGameName[32];
wchar_t wszGamePassword[32];
PBOOL RESETBOOLS[] = {
	&v_DiedOnce,
	&v_AttackPlayer,
	&v_AttackMonster,
	&v_AttemptedChicken,
	&v_HostPlayer,
	&v_TakeNextTP,
	&v_CampAny,
	&v_CampTarget,
	&v_Flashing,
	&v_EnchantBot,
	&v_AutoTele,
	&v_AntiTppkBusy,
	&v_MessageBusy,
	&v_SpamBusy,
	&v_ConsoleLogs,
	&v_EnchBusy,
	&v_AutoPartyEnchant
};

BOOL _stdcall EditBoxCallBack(Control* hwnd,DWORD arg2,DWORD arg3)
{
	return true;
}
void wcscpy2(wchar_t *dest, char *src)
{
	do { *dest++ = *src; }while(*src++);
}
void SaveGameName()
{
	GameStructInfo *gi = GetGameInfo();
	if(*gi->szGameName)
	{
		wcscpy2(wszGameName, gi->szGameName);
		wcscpy2(wszGamePassword, gi->szGamePassword);
	}
}
void SetAllFalse(void)
{
	for (INT i=0;i<ArraySize(RESETBOOLS);i++)
	{
		*RESETBOOLS[i]=FALSE;
	}

	SetPrebuffsFalse();
}