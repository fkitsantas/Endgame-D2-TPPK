#ifdef _VARS

#define DEFINEVAR(type, name)		type name;
CArrayEx<LevelNameInfo*, LevelNameInfo*> LevelNames;
DEFINEVAR(CHAR, szLastGameName[1024])

#else

#define DEFINEVAR(type, name)		extern type name;
extern CArrayEx<LevelNameInfo*, LevelNameInfo*> LevelNames;

#endif

#undef _VARS

DEFINEVAR(CHAR, szOrigWinName[1024])
DEFINEVAR(WNDPROC,OldWNDPROC)

#ifdef _DEFINE_VARS

#define MYVAR(type, name)			type v##_##name;
#define MYCARRAY(type, name)		CArrayEx<type, type> v_##name;
#define MYTOGGLE(name)				BOOL v_##name; INT vK_##name;
#define MYKEY(name)					INT vK_##name;

#else

#define MYVAR(d1, v1)				extern d1 v_##v1;
#define MYCARRAY(type, name)		extern CArrayEx<type, type> v_##name;
#define MYTOGGLE(name)				extern BOOL v_##name; extern INT vK_##name;
#define MYKEY(name)					extern INT vK_##name;

#endif

MYVAR(CMap *, GlobalMap)
MYVAR(LPVECTOR, CurrentVector)
MYVAR(BOOL, GameInit)
MYVAR(DWORD, BoTimer)
MYVAR(DWORD, OakTimer)
MYVAR(BOOL, Flashing)
MYVAR(BOOL, HideConsole)
MYVAR(BOOL, EndThread)
MYVAR(DWORD, CurrentExp)

MYVAR(BOOL, HostPlayer)
MYVAR(BOOL, TakeNextTP)
MYVAR(BOOL, CampAny)
MYVAR(BOOL, CampTarget)
MYVAR(BOOL, MessageBusy)
MYVAR(BOOL, SpamBusy)
MYVAR(BOOL, EnchBusy)
MYVAR(BOOL, EnchantBot)
MYVAR(BOOL, AutoTele)
MYVAR(BOOL, AttackPlayer)
MYVAR(BOOL, AutoInvite)
MYVAR(BOOL, AutoPartyEnchant)
MYVAR(BOOL, AttackMonster)
MYVAR(BOOL, AttackNearPlayer)

MYVAR(BOOL, BattleCommandActive)
MYVAR(BOOL, BattleOrdersActive)
MYVAR(BOOL, ShoutActive)
MYVAR(BOOL, EnchantActive)
MYVAR(BOOL, EnergyShieldActive)
MYVAR(BOOL, HolyShieldActive)
MYVAR(BOOL, OakSageActive)
MYVAR(BOOL, CycloneArmorActive)
MYVAR(BOOL, BoneArmorActive)
MYVAR(BOOL, FadeActive)
MYVAR(BOOL, ShiverArmorActive)
MYVAR(BOOL, ChillingArmorActive)
MYVAR(BOOL, BurstOfSpeedActive)
MYVAR(BOOL, FrozenArmorActive)

MYVAR(BOOL, DiedOnce)

MYVAR(BOOL, AntiTppkBusy)
MYVAR(BOOL, FarcastEnabled)
MYVAR(BOOL, FullRadius)
MYVAR(BOOL, ClearMap)
MYVAR(BOOL, ConsoleBusy)
MYVAR(BOOL, FullLightRadius)
MYVAR(BOOL, HostileTarget)
MYVAR(BOOL, NoPortalDelay)
MYVAR(BOOL, SafeTown)
MYVAR(BOOL, SafeRejuv)
MYVAR(BOOL, UseDeathMessage)
MYVAR(BOOL, NoMinimize)
MYVAR(BOOL, CS)
MYVAR(BOOL, AutoPot)
MYVAR(BOOL, ConsoleLogs)
MYVAR(BOOL, ConfigBox)
MYVAR(BOOL, AttemptedChicken)
MYVAR(BOOL, BlindToggled)

MYVAR(UnitAny*, CurrentTarget)

MYVAR(INT,FlashTimer)
MYVAR(INT,FlashDelay)
MYVAR(INT,Blind)
MYVAR(INT,SpamDelay)
MYVAR(INT,GameCount)
MYVAR(INT,LoadCount)
MYVAR(INT,TownLife)
MYVAR(INT,ExitLife)
MYVAR(INT,TownMana)
MYVAR(INT,ExitMana)
MYVAR(INT,DrinkLifePot)
MYVAR(INT,DrinkLifeRej)
MYVAR(INT,DrinkManaPot)
MYVAR(INT,DrinkManaRej)
MYVAR(INT,LifePotDelay)
MYVAR(INT,ManaPotDelay)
MYVAR(INT,RejuvDelay)
MYVAR(INT,DrankLifePot)
MYVAR(INT,DrankLifeRej)
MYVAR(INT,DrankManaPot)
MYVAR(INT,DrankManaRej)
MYVAR(INT,TownLifeTimer)
MYVAR(INT,TownManaTimer)
MYVAR(INT,ChickenNamelock)
MYVAR(INT,ChickenHostile)

MYVAR(DWORD, ConsoleDelay)

MYVAR(DWORD, TickThreads)
MYVAR(DWORD, ChantThread)
MYVAR(DWORD, AttackThread)
MYVAR(DWORD, ChickenThread)
MYVAR(DWORD, AntiThread)
MYVAR(DWORD, TeleportThread)

MYCARRAY(LPVECTOR, CurrentVectors)
MYCARRAY(POINT, TeleportQueue)
MYCARRAY(PENCHANTINFO, EnchantQueue)
MYCARRAY(DWORD, ReportKills)

MYCARRAY(PCONSOLEMESSAGE, ConsoleList)

DEFINEVAR(CHAR, Enchant1[512])
DEFINEVAR(CHAR, Enchant2[512])
DEFINEVAR(CHAR, Enchant3[512])
DEFINEVAR(CHAR, WinampPre[100])

DEFINEVAR(CHAR, Spam1[500])
DEFINEVAR(CHAR, Spam2[500])
DEFINEVAR(CHAR, Spam3[500])
DEFINEVAR(CHAR, Spam4[500])
DEFINEVAR(CHAR, Spam5[500])
DEFINEVAR(CHAR, Spam6[500])
DEFINEVAR(CHAR, Spam7[500])
DEFINEVAR(CHAR, Spam8[500])
DEFINEVAR(CHAR, Spam9[500])
DEFINEVAR(CHAR, Spam10[500])
DEFINEVAR(CHAR, Spam11[500])
DEFINEVAR(CHAR, Spam12[500])
DEFINEVAR(CHAR, Spam13[500])
DEFINEVAR(CHAR, Spam14[500])
DEFINEVAR(CHAR, Spam15[500])
DEFINEVAR(CHAR, Spam16[500])
DEFINEVAR(CHAR, Spam17[500])
DEFINEVAR(CHAR, Spam18[500])
DEFINEVAR(CHAR, Line1[500])
DEFINEVAR(CHAR, Line2[500])
DEFINEVAR(CHAR, Line3[500])
DEFINEVAR(CHAR, Line4[500])
DEFINEVAR(CHAR, Line5[500])
DEFINEVAR(CHAR, Line6[500])
DEFINEVAR(CHAR, Line7[500])
DEFINEVAR(CHAR, Line8[500])
DEFINEVAR(CHAR, Line9[500])
DEFINEVAR(CHAR, Line10[500])
DEFINEVAR(CHAR, Line11[500])
DEFINEVAR(CHAR, Line12[500])
DEFINEVAR(CHAR, Line13[500])
DEFINEVAR(CHAR, Line14[500])
DEFINEVAR(CHAR, Line15[500])
DEFINEVAR(CHAR, Line16[500])
DEFINEVAR(CHAR, Line17[500])
DEFINEVAR(CHAR, Line18[500])

DEFINEVAR(CHAR, szPath[65535])

#undef MYVAR
#undef MYCARRAY
#undef MYTOGGLE
#undef MYKEY