#include "Houdini.h"

bool ListOnce=false; // Players
bool GainOnce=false; // Authing

PKInfoStruct PKInfo;
AUTHNUMBER Authn;

#define INTERNET_OPEN_TYPE_PRECONFIG      0
//#define INTERNET_FLAG_EXISTING_CONNECT 536870912

typedef void* (WINAPI * __InternetOpen) (char*,DWORD,DWORD,DWORD,DWORD);
typedef void* (WINAPI * __InternetOpenUrl) (void*,char*,DWORD,DWORD,DWORD,DWORD);
typedef void  (WINAPI * __InternetCloseHandle) (HANDLE);
typedef BOOL  (WINAPI * __InternetReadFileA) ( HANDLE, LPVOID ,DWORD ,LPDWORD);

char* GetSiteText(char* Addr ,char* Para) {
  HANDLE hInternet = NULL;
  HANDLE hInternetUrl = NULL;
  char szString[2049]={0};
  char szSite[1024]={0};
  sprintf(szSite,"%s?%s",Addr,Para);


   HANDLE gHandle = LoadLibrary("wininet");
   if(!gHandle)
      return 0;

   __InternetOpen fp1;
   fp1=(__InternetOpen)(DWORD)GetProcAddress((HINSTANCE)gHandle,"InternetOpenA");
   __InternetOpenUrl fp2;
   fp2=(__InternetOpenUrl)(DWORD)GetProcAddress((HINSTANCE)gHandle,"InternetOpenUrlA");
   __InternetCloseHandle fp3;
   fp3=(__InternetCloseHandle)(DWORD)GetProcAddress((HINSTANCE)gHandle,"InternetCloseHandle");
   __InternetReadFileA fp4;
   fp4=(__InternetReadFileA)(DWORD)GetProcAddress((HINSTANCE)gHandle,"InternetReadFile");
   if(!fp1 || !fp2 || !fp3 || !fp4)
      return 0;

  DWORD dwBytes = 0;
  hInternet = fp1(0,INTERNET_OPEN_TYPE_PRECONFIG,0,0,0);
  if(hInternet == NULL) return 0;
  hInternetUrl = fp2(hInternet,szSite,NULL,0,INTERNET_FLAG_EXISTING_CONNECT,0);
  if(hInternetUrl == NULL) return 0;
  fp4(hInternetUrl,&szString,2048,&dwBytes); // 2048 = max. 2kb
  if(hInternetUrl == NULL) return NULL;

  fp3(hInternetUrl);
  fp3(hInternet);
  return szString;
} // Closing GetText

//#define INTERNET_OPEN_TYPE_PRECONFIG      0

typedef void* (WINAPI * __InternetOpen) (char*,DWORD,DWORD,DWORD,DWORD);
typedef void* (WINAPI * __InternetOpenUrl) (void*,char*,DWORD,DWORD,DWORD,DWORD);

BOOL ConnectURL(char* szURL) {

   HANDLE hURL;
   HANDLE hInternetSession;

   HANDLE gHandle = LoadLibrary("wininet");
   if(!gHandle)
      return 0;

   __InternetOpen fp1;
   fp1=(__InternetOpen)(DWORD)GetProcAddress((HINSTANCE)gHandle,"InternetOpenA");
   __InternetOpenUrl fp2;
   fp2=(__InternetOpenUrl)(DWORD)GetProcAddress((HINSTANCE)gHandle,"InternetOpenUrlA");
   
   if(!fp1 || !fp2)
      return 0;

   hInternetSession = fp1("Microsoft Internet Explorer",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
   hURL = fp2(hInternetSession,szURL,NULL,0,0,0);
   CloseHandle(hURL);
   CloseHandle(hInternetSession);
   return (BOOL)FreeLibrary((HINSTANCE)gHandle);
}

void Decrypt(char* buffer, char* String,int xorer) 
{ for(int i=0;i<strlen(String);
i++) { buffer[i] = ((char)String[i])-xorer;
} }

// start of urls to use

char* AuthChecks() { // http://hcpk.net/AuthCheckiNg.php
	char* Buffer=(char*)malloc(512);
	for(int i=0;i<512;i++)
		Buffer[i] = 0x00;
        Decrypt(Buffer,"s{E::sn{v9yp:L€sNspnvtYr9{s{",11);
	return Buffer;
}
char* GetKey() { // &key=96846572F96725226A64DBB06D82C
	char* Buffer=(char*)malloc(512);
	for(int i=0;i<512;i++)
		Buffer[i] = 0x00;
	Decrypt(Buffer,"1vp„HDAC?A@B=QDAB=@==ALA?OMM;AOC=N",11);
	return Buffer;
}
char* HcpkURL() { // http://hcpk.net/AuthCheckiNg.php
	char* Buffer=(char*)malloc(512);
	for(int i=0;i<512;i++)
		Buffer[i] = 0x00;
	Decrypt(Buffer,"s{E::sn{v9yp:L€sNspnvtYr9{s{",11);
	return Buffer;
}
char* HCPKAUTHLIST() { // http://hcpk.net/AuthlIsT.php
	char* Buffer=(char*)malloc(512);
	for(int i=0;i<512;i++)
		Buffer[i] = 0x00;
	Decrypt(Buffer,"s{E::sn{v9yp:L€swT~_9{s{",11);
	return Buffer;
}
///////////////////////// URLS




bool CheckIfAllowed() {
	GainOnce=false;
	if(GainOnce)
		return TRUE;
	char Parameter[512];
	char Buffer[512];
	sprintf(Parameter,"action=check&a=%s&r=%s&c=%s",GetGameInfo()->szAccountName,GetGameInfo()->szRealmName,GetGameInfo()->szCharName);
	sprintf(Buffer,"%s",GetSiteText(AuthChecks(),Parameter));
	if(Buffer[0]=='N'&&Buffer[1]=='A'&&Buffer[2]=='H') 
		return true;
		return false;
}


void GA(void)
{
if(ListOnce)
	return;
	char Buffer[2048]={0};
	char Parameter[1024];
	char* argv[1024];
	sprintf(Parameter,"action=version");
	strcpy(Buffer,GetSiteText(AuthChecks(),Parameter));
	DWORD argc=MakeParameter(Buffer,argv,'|',true);
	Authn.LifeAddr1=atoi(argv[1]);
	}

void CheckAuth(void) {
    char Parameter[512];
	char Buffer[512];
	sprintf(Parameter,"action=check&a=%s&r=%s&c=%s",GetGameInfo()->szAccountName,GetGameInfo()->szRealmName,GetGameInfo()->szCharName);
	sprintf(Buffer,"%s",GetSiteText(AuthChecks(),Parameter));
    if(Buffer[0]=='N'&&Buffer[1]=='A'&&Buffer[2]=='H')  {
	//	GA();
        }
    else {
		TerminateProcess(GetCurrentProcess(),0);
		UninstallPatches();
		RemovePatchs();      
        }
    }

/*DWORD WINAPI PrintPlayerStats(void* p) {
	GainOnce=false;
	if(GainOnce)
	return TRUE;
	GainOnce=true;
	char *szBuffer= (char*)p;
	char Buffer[2048]={0};
	char Parameter[1024];
	char* argv[1024];
sprintf(Parameter,"action=info&a=%s&c=%s&r=%s",GetGameInfo()->szAccountName,GetGameInfo()->szCharName,GetGameInfo()->szRealmName);
strcpy(Buffer,GetSiteText(AuthChecks(),Parameter));
DWORD argc=MakeParameter(Buffer,argv,'|',true);
	strcpy(PKInfo.ForumName,argv[0]);
	PKInfo.TotalKills=atoi(argv[1]);
	PKInfo.TotalScore=atoi(argv[2]);
	PKInfo.Rank=atoi(argv[3]);
	PKInfo.Ready=true;
	TimeOn++;
return true;
}
*/
//BOOL UpdatePKInfo() {
//	CreateThread(0,0,PrintPlayerStats,0,0,0);
//	return true;
//}

DWORD WINAPI PrintThread(void* p) {
	char *szBuffer= (char*)p;
	char Buffer[2048]={0};
	sprintf(Buffer,"ÿc3::ÿc1 Last Kill::ÿc0 %s",GetSiteText(HCPKAUTHLIST(),"action=lpk"));
	PrintConsoleString(Buffer,0);
	return 0;
}

void PrintLastPK() {
	CreateThread(0,0,PrintThread,0,0,0);

}


void GetPlayers() {
	if(ListOnce==false) {
		Sleep(50);

			GA();
			PKInfo.Ready=true;
			TimeOn++;
			ListOnce=true;
	return;
}

}

HANDLE hInternetSession;   
HANDLE hURL;

BOOL OpenURL(char* szURL) {

   HANDLE hURL;
   HANDLE hInternetSession;

   HANDLE gHandle = LoadLibrary("wininet");
   if(!gHandle)
      return false;

   __InternetOpen fp1;
   fp1=(__InternetOpen)(DWORD)GetProcAddress((HINSTANCE)gHandle,"InternetOpenA");
   __InternetOpenUrl fp2;
   fp2=(__InternetOpenUrl)(DWORD)GetProcAddress((HINSTANCE)gHandle,"InternetOpenUrlA");
   
   if(!fp1 || !fp2)
      return false;

   hInternetSession = fp1("Microsoft Internet Explorer",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
   hURL = fp2(hInternetSession,szURL,NULL,0,0,0);
   CloseHandle(hURL);
   CloseHandle(hInternetSession);
   return (BOOL)FreeLibrary((HINSTANCE)gHandle);
}



DWORD WINAPI SubmitThread(void* p) {
	char *szBuffer= (char*)p;
if(!OpenURL(szBuffer))
PrintString("Error: Can't submit the kill",1);
//UpdatePKInfo();
delete szBuffer;
return 0;
}

BOOL AddURL(char* PKname, int Level, char* Class) {

	GameStructInfo* pInfo=(
	GameStructInfo*)GetGameInfo();
RosterUnit* pUnit=(RosterUnit*)*(DWORD*)0x6FBCC080; // Updated

									char szTime[64]={0};
									char* szBuffer = (char*)malloc(2024);
									memset(szBuffer,0,2024);
									time_t ttTime;
									time(&ttTime);
									
									char Type[1024]={0};
									if(IsExpansion())
										strcpy(Type,"Expansion");
									else strcpy(Type,"Classic");
									strftime(szTime, 1024, "%b %d %Y - %H:%M:%S",localtime(&ttTime));
									sprintf(szBuffer,"%s?action=add&t=%s&d=%s&c=%s&l=%d&k=%s&r=%s&g=%s%s",
										AuthChecks(),
										szTime,
										PKname,
										Class,
										Level,
										pInfo->szAccountName,
										pInfo->szRealmName,
										Type,
										GetKey()
									);

									CreateThread(0,0,SubmitThread,(LPVOID)szBuffer,0,0);
								return true;
}



DWORD WINAPI PrintPkStats(void* p) {
	GainOnce=false;
	if(GainOnce)
	return TRUE;
	GainOnce=true;
	char *szBuffer= (char*)p;
	char Buffer[2048]={0};
	char Parameter[512];
sprintf(Parameter,"action=info&a=%s&c=%s&r=%s&l=%s",GetGameInfo()->szAccountName,GetGameInfo()->szCharName,GetGameInfo()->szRealmName);
sprintf(Buffer,"ÿc3::ÿc1 HCTPPK:ÿc0 %s",GetSiteText(HCPKAUTHLIST(),Parameter));
	PrintConsoleString(Buffer,0);
	return 0;
}

void PrintPkStats() {
	CreateThread(0,0,PrintPkStats,0,0,0);
	return;	
}

DWORD WINAPI UpdatedStats(void* p) {
	GainOnce=false;
	if(GainOnce)
	return TRUE;
	GainOnce=true;
	char *szBuffer= (char*)p;
	char Buffer[2048]={0};
	char Parameter[512];
sprintf(Parameter,"action=statsonkill&a=%s&c=%s&r=%s&l=%s",GetGameInfo()->szAccountName,GetGameInfo()->szCharName,GetGameInfo()->szRealmName);
sprintf(Buffer,"ÿc3::ÿc1 HCTPPK:ÿc0 %s",GetSiteText(HCPKAUTHLIST(),Parameter));
	PrintConsoleString(Buffer,0);
	return 0;
}

void UpdatedStats() {
	CreateThread(0,0,UpdatedStats,0,0,0);
	return;	
}