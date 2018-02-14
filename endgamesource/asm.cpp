#include "Houdini.h"

DWORD __declspec(naked) __fastcall GetUnit_STUB(DWORD dwUnitId, DWORD dwUnitType)
{
	__asm
	{
		pop eax;
		push edx;
		push eax;

		shl edx, 9;
		mov eax, D2CLIENT_GetUnitFromId_I;
		add edx, eax;
		mov eax, ecx;
		and eax, 0x7F;
		jmp D2CLIENT_GetUnitFromId_II;
	}
}

DWORD __declspec(naked) __fastcall D2CLIENT_clickParty_ASM(DWORD RosterUnit, DWORD Mode)
{
	__asm
	{
		mov eax, ecx
		jmp D2CLIENT_clickParty_I
	}
}

DWORD __declspec(naked) GetPlayerID(VOID)
{
	__asm
	{
		MOV EAX,DWORD PTR DS:[0x6FBCC3D0]
		MOV ECX,DWORD PTR DS:[EAX+0xC]
		mov eax, ecx
		ret
	}
}

DWORD SendPacketAddr=0x91A0;

void SendGAMEPacket(BYTE* Packet,DWORD PacketSize)
{
	DWORD size1 = PacketSize;
	__asm
	{
		pushad
		mov eax, Packet 
		push Packet		
		mov ebx, size1
		mov edx, 0x6FB291A0
		call edx
		popad
	}
}

DWORD p_D2CLIENT_pUnitTableM=0x11A960;

DWORD D2CLIENT_GetUnitFromId_M=0x1E550;

DWORD __declspec(naked) __fastcall D2CLIENT_GetUnitFromId_STUB(DWORD unitid, DWORD unittype)
{
	__asm
	{
		pop eax;
		push edx;
		push eax;

		shl edx, 9;
		mov eax, p_D2CLIENT_pUnitTableM;
		add edx, eax;
		mov eax, ecx;
		and eax, 0x7F;
		jmp D2CLIENT_GetUnitFromId_M;
	}
}

WORD GetPlayerX(DWORD ID)
{
	UnitAny* pUnit=(UnitAny*)D2CLIENT_GetUnitFromId_STUB(ID,0);
	if(pUnit!=NULL)
	return pUnit->pPath->xPos;
	return 0;
}

WORD GetPlayerY(DWORD ID)
{
	UnitAny* pUnit=(UnitAny*)D2CLIENT_GetUnitFromId_STUB(ID,0);
	if(pUnit!=NULL)
	return pUnit->pPath->yPos;
	return 0;
}

DWORD D2CLIENT_TestPvpFlag_M=0x30DD0;

DWORD __declspec(naked) __fastcall TestPvpFlag_STUB(DWORD planum1, DWORD planum2, DWORD flagmask)
{
	__asm
	{
		push esi
		push [esp+8]
		mov esi, edx
		mov edx, ecx
		call D2CLIENT_TestPvpFlag_M
		pop esi
		ret 4
	}
}

DWORD LifeAddr=0x6FD856E0;

DWORD __declspec(naked) GetCurrentLife()
{
	__asm
	{
		mov eax, 0x6FBCC3D0
		mov eax, [eax]
		push 0
		push 6
		push eax
		call DWORD PTR DS: [LifeAddr]
		sar eax, 8
		ret 
	}
}

DWORD __declspec(naked) GetMaxLife()
{
	__asm
	{
		mov eax, 0x6FBCC3D0
		mov eax, [eax]
		push 0
		push 7
		push eax
		call DWORD PTR DS: [LifeAddr]
		sar eax, 8
		ret 
	}
}

DWORD __declspec(naked) GetCurrentMana()
{
	__asm
	{
		mov eax, 0x6FBCC3D0
		mov eax, [eax]
		push 0
		push 8
		push eax
		call DWORD PTR DS: [LifeAddr]
		sar eax, 8
		ret 
	}
}

DWORD __declspec(naked) GetMaxMana()
{
	__asm
	{
		mov eax, 0x6FBCC3D0
		mov eax, [eax]
		push 0
		push 9
		push eax
		call DWORD PTR DS: [LifeAddr]
		sar eax, 8
		ret 
	}
}

void __fastcall NextGameNamePatch(Control* box, BOOL (__stdcall *FunCallBack)(Control*,DWORD,DWORD))
{
	D2WIN_SetEditBoxText(box, wszGameName);
	D2WIN_SelectEditBoxText(box);
	D2WIN_SetEditBoxProc(box, FunCallBack);
}

void __fastcall NextGamePasswordPatch(Control* box, BOOL (__stdcall *FunCallBack)(Control*,DWORD,DWORD))
{
	D2WIN_SetEditBoxText(box, wszGamePassword);
	D2WIN_SelectEditBoxText(box);
	D2WIN_SetEditBoxProc(box, FunCallBack);
}

DWORD __declspec(naked) __fastcall D2CLIENT_GetUIVar_STUB(DWORD varno)
{
	__asm 
	{
		mov eax, ecx;
		jmp D2CLIENT_GetUiVar_I;
	}
}

VOID __declspec(naked) GameShake1_STUB()
{
	__asm
	{
		je OldCode;

		add dword ptr [esp], 0xC;
		ret;

OldCode:
		mov [ecx], esi;
		mov ecx, [esp + 0x1C];
		ret;
	}
}

VOID __stdcall GameShake(LPDWORD lpX, LPDWORD lpY)
{
	D2CLIENT_CalculateShake(lpX, lpY);

	*p_D2CLIENT_xShake = 0;
	*p_D2CLIENT_yShake = 0;
}