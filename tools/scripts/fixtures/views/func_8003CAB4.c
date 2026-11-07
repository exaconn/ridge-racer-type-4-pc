#include "rr/race_types.h"

extern s16 D_800F4D44;

extern void SetupRace();

extern unsigned short D_8009F914[];

extern unsigned short D_800AD50A;

void func_8003CAB4(void) {
    SetupRace();
    D_800AD50A = D_8009F914[D_800F4D44];
}
