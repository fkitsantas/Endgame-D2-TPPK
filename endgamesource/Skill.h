#ifndef _SKILL_H
#define _SKILL_H

#define SPELL_MAX_NAME 25

typedef struct D2SpellInfo_t
{
	WORD wSpellID;
	int nClass;
	DWORD dwSpellInfoFlag;
	char szSpellName[SPELL_MAX_NAME + 1];
	DWORD dwCoolDown;
} D2SpellInfo;

WORD GetCurrentSkill(BOOL bLeft);
BOOL SilentSkill(WORD wSkillId, BOOL bLeft);
BOOL GetSkillInfo(WORD wSpell, D2SpellInfo *lpBuffer);

#endif