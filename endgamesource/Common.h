#define PVP_HOSTILE 1
#define PVP_FRIENDLY 2
#define PVP_NEUTRAL 3
#define PARTY_NOT_IN_PARTY 0x00
#define PARTY_IN_PARTY 0x01
#define PARTY_INVITED_YOU 0x02
#define PARTY_INVITED_BY_YOU 0x04

int GetPvPStatus(DWORD nUnitId);
bool GetPlayerRoster(RosterUnit* pUnit, DWORD nUnitId);