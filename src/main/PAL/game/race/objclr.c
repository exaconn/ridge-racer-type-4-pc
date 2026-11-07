/* CC1_FLAGS: -fno-strength-reduce */
#include "game/race/objclr.h"
extern int *TrackObjectDefs;
extern int *TrackObjectStates;
int ClearTrackObjectStates(a0) int a0; {
    int a3; 
    int n; 
    int *a1; 
    int *a2; 
    register int t __asm__("$2"); /* ASM_BARRIER: pin to the original register allocation */
    int x; 
    int one; 
    a3 = 0;
    t = TrackObjectCount;
    if (t > 0) {
        one = 1;
        n = t;
        a1 = TrackObjectStates;
        a2 = TrackObjectDefs;
        do {
            a1[9] = 0;
            if (a0 != one || (x = a2[2], t = 0xA, x != t) || a2[1] != 0)
                a1[2] = 0;
            a1[1] = 0;
            a1[0] = 0;
            a1[10] = 0;
            a1 += 11;
            a3 += 1;
            a2 += 12;
        } while (a3 < n);
    }
}
