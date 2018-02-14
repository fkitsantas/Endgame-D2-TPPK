#define _VARS
#define INITVAB(n,a) n=n+(DWORD)LoadLibraryA(a);
#include "Houdini.h"
char szModulePath[259];

AutoAim Auto;
LinkedList* EnchList;

BOOL WINAPI DllMain(HINSTANCE hDll,DWORD dwReason,LPVOID lpReserved) 
{
	if(dwReason==DLL_PROCESS_ATTACH) 
	{
		EnchList=new LinkedList;
		DefineOffsets();

		MessageBeep(-1);


		//strcpy(szModulePath,(char*)*(DWORD*)((DWORD)hDll+0xB00)); //Internet
		GetModuleFileName(hDll,szModulePath,249); *(strrchr(szModulePath,'\\')+1) = 0;

		GetModuleFileNameA(hDll,szPath,MAX_PATH);
		PathRemoveFileSpecA(szPath);
		strcat_s(szPath,sizeof(szPath),"\\");

		ReadMainConfig();
		ReadBps();
		ReadBoxes();
		InstallPatchs();

		INITVAB(DontBreak,"D2NET.dll") 
		INITVAB(BreakItUp,"D2NET.dll") 
		INITVAB(PlayerRoster,"D2CLIENT.dll")
		INITVAB(p_D2CLIENT_pUnitTableM,"D2CLIENT.dll")
		INITVAB(D2CLIENT_GetUnitFromId_M,"D2CLIENT.dll")
		INITVAB(D2CLIENT_TestPvpFlag_M,"D2CLIENT.dll")  
	
		InitItemViewer();

		if(D2GFX_GetHwnd())
		{
			GetWindowText(D2GFX_GetHwnd (), szOrigWinName, sizeof(szOrigWinName));
			OldWNDPROC = (WNDPROC)GetWindowLong(D2GFX_GetHwnd(),GWL_WNDPROC);
			SetWindowLong(D2GFX_GetHwnd(),GWL_WNDPROC,(LONG)GameWindowEvent);
		}

		if(*(DWORD*)0x6FBCC3D0!=NULL) 
		{
			if(GameReady(FALSE))
			Load();   
		}

		v_TickThreads=DriverCreateThread((LPVOID)TickThread, (LPVOID)THREAD_PRIMARY);
	};
	if(dwReason==DLL_PROCESS_DETACH)
	{
	}
	return true;
	
};
