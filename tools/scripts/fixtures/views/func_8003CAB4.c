#include "rr/race_types.h"

extern s16 GameModeType;

extern void SetupRace();

extern unsigned short GameTypeSceneIds[];

extern unsigned short ActiveSceneId;

void SetupRaceAndSceneId(void) {
    SetupRace();
    ActiveSceneId = GameTypeSceneIds[GameModeType];
}
