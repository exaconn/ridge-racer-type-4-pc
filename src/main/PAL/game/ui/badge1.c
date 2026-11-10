#include "game/ui/badge1.h"
#include "types.h"
#include "rr/badge_rows.h"
#include "rr/draw_environment.h"
#include "psyq/libgpu/primatt5.h"
#include "rr/animated_glyph.h"
#include "rr/hud_number.h"

struct GpuDrawAreaPacket12 { int a, b, c; };

#include "rr/sprite_emit.h"

extern int SetDrawArea();
extern char DrawBuffers[];

void *AddDrawAreaPrim(u32 *a0, void *cursor, int a2, int a3, int a4, int a5);

void *AddTilePrim(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void *AddTileGrid2x2(u32 *a0, void *a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

void *AddDrawAreaPrim(u32 *a0, void *cursor, int a2, int a3, int a4, int a5) {
    struct GpuDrawAreaPacket12 *a1=cursor;
    short sp[4];
    char *p = DrawBuffers;
    int t;
    sp[0] = a2;
    if (((short *)p)[0x5E / 2] == ((short *)p)[0x227D6 / 2]) {
        sp[1] = a3;
    } else {
        t = DrawBufIndex;
        sp[1] = a3 + (t << 8);
    }
    sp[2] = a4;
    sp[3] = a5;
    SetDrawArea(a1, sp);
    AddPrim(a0, a1++);
    return a1;
}

void *AddDrawModePrim(u32 *ordering, GpuDrawModePacket *packet, u16 page) {
    SetDrawMode(packet, 0, 1, page, 0);
    AddPrim(ordering, (u32 *)packet++);
    return packet;
}


void *AddTilePrim(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    SetTile(arg1);
    ((s16 *)arg1)[4] = arg2;
    ((s16 *)arg1)[5] = arg3;
    ((s16 *)arg1)[6] = (s16) arg4;
    ((s16 *)arg1)[7] = (s16) arg5;
    ((s8 *)arg1)[4] = (s8) arg6;
    ((s8 *)arg1)[5] = (s8) arg7;
    ((s8 *)arg1)[6] = (s8) arg8;
    AddPrim(arg0, arg1);
    return arg1 + 0x10;
}

void *AddTileGrid2x2(u32 *a0, void *a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
    int i = 0;
    int w = a4 / 2;
    int h = a5 / 2;
    do {
        int x = a2 + w * (i & 1);
        int y = a3;
        if (i >= 2)
            y += h;
        a1 = AddTilePrim(a0, a1, x, y, w, h, a6, a7, a8);
        i++;
    } while (i < 4);
    return a1;
}
