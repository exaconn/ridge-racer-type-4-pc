#include "types.h"
#include "game/sound/sndlis2.h"
extern int SoundCalcCarParams();
extern int SlotSetLevelProgram();
struct A { char p[0x5A]; u16 x5A, x5C; char q[4]; s32 x64, x68; char r[1568 - 0x6C]; };
struct R { char p[0x248]; u16 x248, x24A; char q[800 - 0x24C]; };
extern struct A SoundState[];
extern struct R ComCars[];
void SoundUpdateComSlots(void) {
    int i;
    for (i = 0; i < 2; i++) {
        char *c = (char *)SoundState + i * 0x1C;
        struct R *r = &ComCars[*(s16 *)(c + 0x18F0)];
        SoundCalcCarParams(i);
        if (*(s16 *)(c + 0x18F0) != -1) {
            SoundState[i + 2].x68 = *(s32 *)(c + 0x18FC);
            SoundState[i + 2].x5C = *(u16 *)(c + 0x1900);
            SoundState[i + 2].x5A = *(u16 *)(c + 0x1902);
            SoundState[i + 2].x64 = *(s32 *)(c + 0x1904);
            SlotSetLevelProgram(r->x24A, r->x248, i + 2);
        }
    }
}
