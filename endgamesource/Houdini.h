#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500

#define ArraySize(x) (sizeof(x) / sizeof(x[0]))
#define _CRT_SECURE_NO_DEPRECATE
extern char szModulePath[259];

#pragma comment(lib, "shlwapi.lib")

#include <process.h>
#include <windows.h>
#include <wininet.h>
#include <time.h>
#include <stdio.h>
#include <ctime>
#include <Winbase.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <shlwapi.h>
#include <ctime>
#include <math.h>
#include <direct.h>
#include <stdlib.h>
#include <errno.h>
#include <io.h>
#include <string.h>
#include <new>
//Header Files
#include "AutoParty.h"
#include "D2Ptrs.h"
#include "D2Structs.h"
#include "D2Studio.h"
#include "Constants.h"
#include "Intercepts.h"
#include "Winamp.h"
#include "asm.h"
#include "Area.h"
#include "ArrayEx.h"
#include "Driver.h"
#include "AutoAim.h"
#include "BoxHook.h"
#include "Chicken.h"
#include "CMapIncludes.h"
#include "Common.h"
#include "Config.h"
#include "D2Helpers.h"
#include "DrawHook.h"
#include "Extra.h"
#include "InfoBox.h"
#include "Internet.h"
#include "ItemLevel.h"
#include "ItemViewer.h"
#include "KeyHook.h"
#include "LinkedList.h"
#include "Maphack.h"
#include "Matrix.h"
#include "Monsters.h"
#include "Offset.h"
#include "PathFinder.h"
#include "ReceivePacket.h"
#include "SendPacket.h"
#include "Skill.h"
#include "Team.h"
#include "Timer.h"
#include "Vars.h"


#define CToW(szString, wString)				(MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szString, (int)strlen(szString), wString, (int)strlen(szString)+1))
#define	WToC(wString, szString)				(WideCharToMultiByte(CP_ACP, 0, wString, -1, szString, (int)sizeof(szString), 0, 0));


#define FONTCOLOR_WHITE		0
#define FONTCOLOR_RED		1
#define FONTCOLOR_GREEN		2
#define FONTCOLOR_BLUE		3
#define FONTCOLOR_GOLD		4
#define FONTCOLOR_GREY		5
#define FONTCOLOR_BLACK		6
#define FONTCOLOR_BROWN		7
#define FONTCOLOR_ORANGE	8
#define FONTCOLOR_YELLOW	9
#define FONTCOLOR_PURPLE	10

#define VERSION_NO		"v2.1.0"

#define THREAD_PRIMARY	1
#define THREAD_CHICKEN	2
#define THREAD_ATTACK	3
#define THREAD_TELEPORT 4
#define THREAD_ENCHANT	5
#define THREAD_ANTIPK	6

#define ENCHANT_ME 1
#define ENCHANT_MERC 2
#define ENCHANT_ALL 3

extern AutoAim Auto;
extern LinkedList* PlayerFriendList;
extern LinkedList* PlayerEnemyList;
extern LinkedList* PlayerHCPKList;
extern LinkedList* PlayerIHPKList;
extern LinkedList* EnchList;
