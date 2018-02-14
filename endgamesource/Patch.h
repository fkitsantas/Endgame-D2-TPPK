PatchHook pHooks[] = {
	{PatchBytes, GetDllOffset("D2Client.dll", 0x5C797), 0x01, 1}, // Farcast 0
	{PatchCall, GetDllOffset("D2Client.dll", 0x5F8F7),(DWORD)GameLightRadius_STUB, 5}, // Light Radius 1
	{PatchJmp, GetDllOffset("D2Client.dll", 0x40609), (DWORD)GameDrawAutomapInfo_STUB, 5}, // Automap 2
	{PatchJmp, GetDllOffset("D2Client.dll", 0x90524), (DWORD)GameDraw_STUB, 6}, // Draw 3
	{PatchCall, GetDllOffset("D2CLIENT.dll", 0x90211), (DWORD)DrawEnteringLevelText_STUB, 5}, // Draw Entering 4
	{PatchCall, GetDllOffset("D2CLIENT.dll", 0x836FE), (DWORD)DrawManaOrb_STUB, 5}, //Draw Mana 5
	{PatchCall, GetDllOffset("D2CLIENT.dll", 0x9035F), (DWORD)DrawStamina_STUB, 5}, // Draw Stamina 6
	{PatchCall, GetDllOffset("D2CLIENT.dll", 0x90358), (DWORD)DrawStaminaA_STUB, 5}, // Draw Stamina2 7
	{PatchCall, GetDllOffset("D2Client.dll", 0x9F7A5), (DWORD)KeyHookIntercept_STUB, 7}, // Key Hook 8
	{PatchJmp, GetDllOffset("D2Client.dll", 0xD21C), (DWORD)SendPacketIntercept_STUB, 6}, // Send Packet 9
    {PatchCall, GetDllOffset("D2Client.dll", 0x65111),(DWORD)ReceivePacketIntercept_STUB, 5}, // Receive Packet 10
	{PatchCall, GetDllOffset("D2Client.dll", 0xBB5B6), (DWORD)GameShake1_STUB, 6}, // Game Shake1 11
	{PatchCall, GetDllOffset("D2Client.dll", 0x7CA82), (DWORD)GameShake, 5}, // Game Shake2 12
	{PatchCall, GetDllOffset("D2Client.dll", 0x3FC1E),(DWORD)PlayerMapIntercept, 5}, // Player Name 13
	{PatchCall, GetDllOffset("D2Client.dll", 0x4FF63), (DWORD)GameCreateMissile_STUB,6},  // CM 14
	{PatchCall, GetDllOffset("D2Client.dll", 0x4FEF8), (DWORD)GameCreateMissile2_STUB,7}, // CM2 15
	{PatchCall, GetDllOffset("D2Client.dll", 0x3F69D),(DWORD)PlayerBlobIntercept, 6}, // Blob 16
    //{PatchJmp, GetDllOffset("D2Client.dll",  0x5CEB7), (DWORD)PlayerPatch, 15}, // AA 17
	{PatchCall, GetDllOffset("D2Client.dll", 0x7BB5E), (DWORD)GameFailToJoin_STUB, 6}, // FTJ 18
    {PatchCall, GetDllOffset("D2Multi.dll", 0x1463B), (DWORD)NextGameNamePatch, 5}, // NG1 19
	{PatchCall, GetDllOffset("D2Multi.dll", 0x14676), (DWORD)NextGamePasswordPatch, 5}, // NP1 20
	{PatchCall, GetDllOffset("D2Multi.dll", 0x14959), (DWORD)NextGameNamePatch, 5}, // NG2 21
    {PatchCall, GetDllOffset("D2Multi.dll", 0x14994), (DWORD)NextGamePasswordPatch, 5}, // NP2 22
	{PatchBytes, GetDllOffset("D2Client.dll", 0x58EF4),0xEB, 1}, // ?? 23
};