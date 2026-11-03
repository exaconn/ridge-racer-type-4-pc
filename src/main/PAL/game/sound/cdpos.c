#include "types.h"
#include "game/sound/cdmain.h"
#include "psyq/libcd/s_1.h"
#include "game/sound/cdpos.h"
#include "rr/cd.h"
#include "rr/overlay668_stream.h"
#include "rr/menu_overlay_runtime.h"

extern int func_8008DE84();
extern int CdPosToInt();
extern char D_800A0784[];
extern char D_800A07C4[];
extern struct { char p0[0x2634]; s32 x2634; s32 x2638; char p1[0x4]; s32 x2640; } D_800F4DB8;
extern int func_80052ADC();

void func_80052760(void);

void func_80052760(void) {
    struct { u8 pad[8]; u8 b[8]; u8 c[24]; } L;
    D_800F4DB8.x2638 = -1;
    D_800F4DB8.x2640 = -1;
    if (CdControlB(1, 0, L.b) != 0) {
        u32 v = L.b[0];
        if ((v & 0x10) == 0) {
            if ((v & 1) == 0) {
                func_8008DE84(L.c, D_800A0784);
                D_800F4DB8.x2634 = CdPosToInt(L.c) & 0xFFFFFF;
                D_800F4DB8.x2638 = (int)D_800A07C4;
            }
        }
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char p0[0x2658]; u8 x2658; char p1[0xB]; s32 x2664; char p2[0x8]; s32 x2670; s32 x2674; s32 x2678; char p3[0x20]; s32 x269C; char p4[0x8]; s32 x26A8; char p5[0xC]; s32 x26B8; s32 x26BC; char p6[0x4]; s16 x26C4; s16 x26C6; s16 x26C8; s16 x26CA; char p7[0x19C]; s32 x2868; } (*))&D_800F4DB8)

void func_80052800(void) {
    D_800F4DB8.x2658 = 0xC8;
    CdControlB(0xE, &D_800F4DB8.x2658, 0);
    func_80083CD4(0, 0x7F, 0x7F);
    func_8008C12C((int)func_80053A18);
    D_800F4DB8.x2664 = -1;
    D_800F4DB8.x2670 = -1;
    D_800F4DB8.x2674 = 0;
    D_800F4DB8.x2678 = 0;
    D_800F4DB8.x269C = -1;
    D_800F4DB8.x26A8 = 0;
    D_800F4DB8.x26B8 = 0;
    D_800F4DB8.x26BC = 0;
    D_800F4DB8.x26C4 = 0;
    D_800F4DB8.x26C6 = 0;
    D_800F4DB8.x26C8 = 0;
    D_800F4DB8.x26CA = 0;
    func_80052ADC(0x7F);
    D_800F4DB8.x2868 = -1;
    D_8010E220 = 0;
}

#undef D_800F4DB8
