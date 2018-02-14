#include <windows.h>
#include "Houdini.h"

// Editing the Item shit thing .......................... dot dot dot 111
WORD PlayerX() {
UnitAny* pUnit=(UnitAny*)*(DWORD*)0x6FBCC3D0;
return pUnit->pPath->xPos;
}

WORD PlayerY() {
UnitAny* pUnit=(UnitAny*)*(DWORD*)0x6FBCC3D0;
return pUnit->pPath->yPos;
}

byte iBackup[6];
byte iBackup2[6];
byte iBackup3[5];

bool DrawPlayerInventory=false;

void  ViewInventoryPatch1_ASM();
void  ViewInventoryPatch2_ASM();
void  ViewInventoryPatch3_ASM();

void UninstallPatches() {
	WriteBytes((void*)((DWORD)LoadLibrary("D2Client.dll")+0xAF242),&iBackup,sizeof(iBackup));
	WriteBytes((void*)((DWORD)LoadLibrary("D2Client.dll")+0xAE914),&iBackup2,sizeof(iBackup2));
	WriteBytes((void*)((DWORD)LoadLibrary("D2Client.dll")+0xAD93F),&iBackup3,sizeof(iBackup3));
}

void InstallPatches() {
	PatchCall((DWORD)((DWORD)LoadLibrary("D2Client.dll")+0xAF242),(DWORD)ViewInventoryPatch1_ASM,6);
	PatchCall((DWORD)((DWORD)LoadLibrary("D2Client.dll")+0xAE914),(DWORD)ViewInventoryPatch2_ASM,6);
	PatchCall((DWORD)((DWORD)LoadLibrary("D2Client.dll")+0xAD93F),(DWORD)ViewInventoryPatch3_ASM,5);
}

void InitItemViewer() {
	WriteBytes(&iBackup,(void*)((DWORD)LoadLibrary("D2Client.dll")+0xAF242),sizeof(iBackup));
	WriteBytes(&iBackup2,(void*)((DWORD)LoadLibrary("D2Client.dll")+0xAE914),sizeof(iBackup2));
	WriteBytes(&iBackup3,(void*)((DWORD)LoadLibrary("D2Client.dll")+0xAD93F),sizeof(iBackup3));
}

UnitAny* GetPlayer() {
UnitAny* pUnit=(UnitAny*)FindUnit(Auto.GetPlayerID(),0);
	if(pUnit==NULL) {
		Auto.Init();
	}

	return (UnitAny*)FindUnit(Auto.GetPlayerID(),0);
}

DWORD viewingInventory=0;

UnitAny *ViewInventoryPatch1()
{
	return GetPlayer();
}

void __declspec(naked) ViewInventoryPatch1_ASM()
{
	__asm {
		push eax;
		call ViewInventoryPatch1;
		mov esi, eax;
		pop eax;
		ret;
	}
}


void __declspec(naked) ViewInventoryPatch2_ASM()
{
	__asm {
		push eax;
		call ViewInventoryPatch1;
		mov ebx, eax;
		pop eax;
		ret;
	}
}


void __declspec(naked) ViewInventoryPatch3_ASM()
{
	__asm {
	
//edi = default unit
		push eax
		call ViewInventoryPatch1
		mov edi,eax
		pop eax
//original code
		test eax,eax
		mov ecx, [edi+0x60]
		ret
		
	}
}