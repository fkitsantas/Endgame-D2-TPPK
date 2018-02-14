#include "Houdini.h"

#define CELLNO_MYSHRINES 1176
#define CELLNO_SHRINE 310
#define NUMOF_SHRINES 23


void __fastcall OverrideShrinePatch(AutomapCell *newnode, AutomapCell *currnode)
{
	if ((newnode->nCellNo < CELLNO_MYSHRINES) || (newnode->nCellNo >= (CELLNO_MYSHRINES+NUMOF_SHRINES))) return;
	if (currnode->nCellNo >= newnode->nCellNo) return;
	currnode->fSaved = newnode->fSaved;
	currnode->nCellNo = newnode->nCellNo;
}

void __declspec(naked) OverrideShrinePatch_ASM()
{
	__asm {
		pushad
		mov edx,esi
		call OverrideShrinePatch
		popad
		mov edx,D2CLIENT_OverrideShrinePatch_ORIG
		mov edx,[4*eax+edx]
		ret
	}
}