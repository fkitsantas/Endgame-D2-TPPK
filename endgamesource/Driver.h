#ifndef _COMMONSTRUCTS_H
#define _COMMONSTRUCTS_H

typedef struct ThreadInfo_t
{
	HANDLE hThread;
	DWORD dwThreadId;
} THREADINFO, *PTHREADINFO;

typedef struct DllInfo_t
{
	DWORD dwBaseAddress;
	DWORD dwImageSize;
	CHAR szModulePath[MAX_PATH];
	DWORD dwMagicNumber;
} DLLINFO, *PDLLINFO;

typedef struct PatchInfo_t
{
	DWORD dwPatchAddress;
	DWORD dwPatchLength;
	LPBYTE OldCode;
} PATCHINFO, *PPATCHINFO;

typedef struct KeyVar_t
{
	LPSTR lpPrint;
	LPINT lpKey;
	LPBOOL lpToggle;
} KEYVAR, *PKEYVAR;

typedef struct TargetInfo_t
{
	CHAR PlayerName[0x10];
	DWORD UnitId;
	DWORD ClassId;
	BYTE Level;
	BYTE Life;
	BOOL Flashing;
	INT Resists[6];
} TARGETINFO, *PTARGETINFO;

#endif

#ifndef _DRIVER_H
#define _DRIVER_H

#define CTL_LOADER_INIT		1
#define CTL_QUERY_PROCESS	2
#define CTL_MODULE_INIT		3
#define CTL_MODULE_PATCH	4
#define CTL_MODULE_UNLOAD	5
#define CTL_THREAD_CREATE	6
#define CTL_THREAD_DESTROY	7

#define DriverInteract 'WtfC'

typedef struct DriverMessage_t
{
	ULONG ControlCode;
	ULONG MagicNumber;
	LPVOID InputBuffer;
	ULONG InputSize;
	LPVOID OutputBuffer;
	ULONG OutputSize;
} DRIVERMESSAGE, *PDRIVERMESSAGE;

typedef LONG NTSTATUS;

typedef struct _UNICODE_STRING 
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _SYSTEM_LOAD_AND_CALL_IMAGE
{
	UNICODE_STRING ModuleName;
} SYSTEM_LOAD_AND_CALL_IMAGE, *PSYSTEM_LOAD_AND_CALL_IMAGE;

typedef enum _SYSTEM_INFORMATION_CLASS 
{ 
	SystemBasicInformation,
	SystemProcessorInformation,
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemNotImplemented1,
	SystemProcessesAndThreadsInformation,
	SystemCallCounts,
	SystemConfigurationInformation,
	SystemProcessorTimes,
	SystemGlobalFlag,
	SystemNotImplemented2,
	SystemModuleInformation,
	SystemLockInformation,
	SystemNotImplemented3,
	SystemNotImplemented4,
	SystemNotImplemented5,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPagefileInformation,
	SystemInstructionEmulationCounts,
	SystemInvalidInfoClass1,
	SystemCacheInformation,
	SystemPoolTagInformation,
	SystemProcessorStatistics,
	SystemDpcInformation,
	SystemNotImplemented6,
	SystemLoadImage,
	SystemUnloadImage,
	SystemTimeAdjustment,
	SystemNotImplemented7,
	SystemNotImplemented8,
	SystemNotImplemented9,
	SystemCrashDumpInformation,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemLoadAndCallImage,
	SystemPrioritySeparation,
	SystemNotImplemented10,
	SystemNotImplemented11,
	SystemInvalidInfoClass2,
	SystemInvalidInfoClass3,
	SystemTimeZoneInformation,
	SystemLookasideInformation,
	SystemSetTimeSlipEvent,
	SystemCreateSession,
	SystemDeleteSession,
	SystemInvalidInfoClass4,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemAddVerifier,
	SystemSessionProcessesInformation,
} SYSTEM_INFORMATION_CLASS;

typedef NTSTATUS (__stdcall *ZWSETSYSTEMINFORMATION)(IN DWORD SystemInformationClass,
													 IN PVOID SystemInformation,
													 IN ULONG SystemInformationLength);

VOID SendKMDMessage(DWORD ControlCode, LPVOID InputBuffer, DWORD InputSize, LPVOID OutputBuffer, DWORD OutputSize);
DWORD DriverCreateThread(LPVOID lpAddress, LPVOID lpParam);
VOID DriverDestroyThread(DWORD dwThreadId);

#endif