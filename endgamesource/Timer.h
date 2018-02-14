#ifndef TIMER_H
#define TIMER_H

typedef struct t_EnchantInfo {
	DWORD dwPlayerID;
	INT nEnchantType;
}ENCHANTINFO, *PENCHANTINFO;

typedef struct t_PrebuffInfo {
	PCHAR Name;
	INT Type;
	PBOOL Active;
}PREBUFFINFO;

BOOL SetPrebuffsFalse();

#endif

BOOL CastOnUnit(WORD SkillID, UnitAny*pUnit, BOOL bLeft);
DWORD WINAPI TickThread(LPVOID ThreadType);
VOID SendMouseClick(INT x, INT y, INT clicktype);
VOID FormatAttack();
void TimerTick();
DWORD GetUnitDist(UnitAny* pUnit1, UnitAny* pUnit2);
BOOL IsImmune(UnitAny* pUnit,D2SpellInfo Spell);