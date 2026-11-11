#include "game/race/objclr.h"
#include "game/race/objrec.h"

void ClearTrackObjectStates(s32 a0) {
    int i;
    for (i = 0; i < TrackObjectCount; i++) {
        TrackObjectStates[i].f24 = 0;
        if (a0 != 1 || TrackObjectDefs[i].f8 != 0xA || TrackObjectDefs[i].f4 != 0)
            TrackObjectStates[i].f8 = 0;
        TrackObjectStates[i].f4 = 0;
        TrackObjectStates[i].f0 = 0;
        TrackObjectStates[i].f28 = 0;
    }
}
