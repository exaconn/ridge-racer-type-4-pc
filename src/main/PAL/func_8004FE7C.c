#include "rr/audio_loop.h"
void func_8004FE7C(s32 a0, int a1, int a2) {
    int v = D_800F4DC0;
    func_80086B00((s16)a0, (s16)((s32)((u32)a1 * (u32)v) >> 7), (s16)((s32)((u32)a2 * (u32)v) >> 7));
}
