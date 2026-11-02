#include "rr/tables.h"
typedef struct { s16 a, b; } P2;
extern RrEntry800A4258 D_800A4258[];
extern s16 D_800A4540[][2];
extern s16 D_800F3A1C;
extern s16 D_800FFC00;
extern u8 D_8010B2DC[];

void func_8007C8D4(void) {
    s32 i = D_800A4258[D_800F3A1C].i;
    s32 a = D_800A4540[i][0];
    s32 b = D_800A4540[i][1];
    if (D_800F3A1C == 0x3B) {
        if (D_8010B2DC[D_800FFC00] == 1) {
            D_800F3A1C = a;
            return;
        }
        D_800F3A1C = b;
        return;
    }
    D_800F3A1C = a;
}
