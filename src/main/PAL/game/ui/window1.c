/* RODATA 0x800103c0 0x1c */
#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primattr.h"
#include "psyq/libgpu/settrans.h"
#include "rr/race_setup.h"
#include "rr/hud_number.h"
#include "rr/draw_environment.h"
#include "rr/overlay662_draw_area.h"
#include "rr/animated_glyph.h"
#include "game/ui/badge1.h"

struct P { unsigned addr:24; unsigned len:8; unsigned char r, g, b, code; short x; unsigned short y; unsigned char u, v; unsigned short clut; short w, h; };

typedef struct {
    u16 x00;
    u16 y02;
    u16 w04;
    u16 h06;
    u8 u08;
    u8 pad09;
    u8 v0A;
    u8 pad0B;
    u16 clut0C;
    u16 pad0E;
    s32 semiTrans10;
} HudSpriteDef14;

extern char D_8009EBBC[];
extern char D_8009EB1C[];
extern char D_8009EC48[];
extern char D_800D0160[];

void InitHudSpritePacket(struct P *a0, HudSpriteDef14 *a1);
void SubmitRaceHudPackets();
void DrawDigitString12();
void DrawDigitString8();
void DrawDigitString16();

void InitHudSpritePacket(struct P *a0, HudSpriteDef14 *a1) {
    ((u8 *)a0)[3] = 4;
    a0->code = 0x64;
    *(u16 *)&a0->x = a1->x00;
    a0->y = a1->y02;
    *(u16 *)&a0->w = a1->w04;
    *(u16 *)&a0->h = a1->h06;
    a0->u = a1->u08;
    a0->v = a1->v0A;
    a0->clut = a1->clut0C;
    if (a1->semiTrans10 != 0) {
        a0->code |= 2;
    } else {
        a0->code &= 0xFD;
    }
    a0->code |= 1;
}

void InitRaceHudPackets(int a0)
{
    int n, i, j, a, b;
    char *tab;
    char *base;
    char *s1;
    char *s0;
    switch (a0) {
    case 0:
    case 5:
    default:
        n = 4;
        tab = D_8009EBBC;
        break;
    case 1:
        n = 8;
        tab = D_8009EB1C;
        break;
    case 2:
    case 3:
    case 4:
    case 6:
        if ((D_80107178 == 2 && a0 == 4) || a0 == 6) {
            n = 6;
        } else {
            n = 12;
        }
        tab = D_8009EC48;
        break;
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < 2; j++) {
            s1 = D_800D0160 + j * 0x22778;
            s0 = s1 + (i * 0x14 + 0x18);
            InitHudSpritePacket(s0, tab + i * 0x14);
            if (i != 0) AddPrim(s1 + (i * 0x14 + 4), s0);
        }
    }
}

void SubmitRaceHudPackets(int a0) {
    short sp[4];
    int *s0;
    int a1, d, t;
    int *v;
    int *s1;
    switch (a0) {
    case 0:
    default:
        a1 = 4;
        s0 = (int *)(&CurrentDrawBuf->buffer.ordering70.words[0]);
        break;
    case 1:
        a1 = 8;
        s0 = (int *)(&CurrentDrawBuf->buffer.ordering70.words[0]);
        break;
    case 2:
    case 3:
    case 4:
        if ((D_80107178 == 2 && a0 == 3) || a0 == 4) {
            a1 = 6;
            s0 = (int *)(&CurrentDrawBuf->buffer.ordering70.words[0]);
        } else {
            a1 = 0xC;
            s0 = (int *)(&CurrentDrawBuf->buffer.secondaryOrderingB70.words[0]);
        }
        break;
    }
    d = CurrentDrawBuf;
    v = (int *)(a1 * 0x14 + d + 0x22674);
    *v = (*v & 0xFF000000) | (*s0 & 0xFFFFFF);
    *s0 = (*s0 & 0xFF000000) | ((d + 0x22688) & 0xFFFFFF);
    s1 = AddDrawModePrim(s0, *(int **)SCRATCHPAD, 5);
    *(int **)SCRATCHPAD = s1;
    if (a0 >= 2) {
        sp[2] = 0x140;
        sp[0] = 0;
        sp[3] = 0x100;
        t = DrawBufIndex; sp[1] = t << 8;
        SetDrawArea(s1, sp);
        *s1 = (*s1 & 0xFF000000) | (*s0 & 0xFFFFFF);
        *s0 = (*s0 & 0xFF000000) | ((int)s1 & 0xFFFFFF);
        *(int **)SCRATCHPAD = s1 + 3;
    }
}

void DrawDigitString12(s32 a0, s32 a1, u8 *a2, s32 a3) {
    SpritePacketSignedClut *p = *(SpritePacketSignedClut **)SCRATCHPAD;
    s32 t;
    s32 kb;
    s16 c;
    kb = 0xA8;
    if (*a2 != 0) {
        do {
            s32 ch = *a2;
            a2++;
            p->len = 4;
            p->code = 0x64;
            p->x = a0;
            t = ch - 0x30;
            p->y = a1;
            if (t >= 0xD) {
                c = ch * 8 - 0x14C;
                p->w = 4;
                p->h = 8;
                if (t == 0xF)
                    a0 += 5;
                else
                    a0 += 4;
            } else {
                c = t * 12;
                a0 += 10;
                p->w = 12;
                p->h = 12;
            }
            p->u = c;
            p->v = kb;
            p->code |= 1;
            p->clut = a3;
            p->addr = ((SpritePacketSignedClut *)(&CurrentDrawBuf->buffer.ordering70.words[0]))->addr;
            ((SpritePacketSignedClut *)(&CurrentDrawBuf->buffer.ordering70.words[0]))->addr = p;
            p++;
        } while (*a2 != 0);
    }
    SetDrawMode(p, 0, 1, 5, 0);
    AddPrim(&CurrentDrawBuf->buffer.ordering70.words[0], p);
    *(SpritePacketSignedClut **)SCRATCHPAD = (SpritePacketSignedClut *)((char *)p + 0xC);
}


void DrawDigitString8(SpritePacket *a0, s32 a1, s32 a2, u8 *a3, s32 a4) {
    SpritePacket *p = *(SpritePacket **)SCRATCHPAD;
    s32 c, u;
    s32 kb;
    kb = 0xB4;
    if (*a3 != 0) {
        do {
            c = *a3;
            a3++;
            p->len = 4;
            p->code = 0x64;
            p->x = a1;
            p->y = a2;
            p->w = 8;
            p->h = 8;
            c -= 0x30;
            u = c * 8 + 0x6C;
            if (c >= 10) a1 += 3; else a1 += 7;
            p->u = u;
            p->v = kb;
            p->code |= 1;
            p->clut = a4;
            p->addr = a0->addr;
            a0->addr = p;
            p++;
        } while (*a3 != 0);
    }
    *(s32 *)SCRATCHPAD = AddDrawModePrim(a0, p, 5);
}


void DrawDigitString16(struct P *a0, int a1, int a2, unsigned char *a3) {
    struct P *p = *(struct P **)SCRATCHPAD;
    int c, u, kh;
    if (*a3 != 0) {
        kh = 0x10;
        do {
            c = *a3;
            a3++;
            p->len = 4;
            p->code = 0x64;
            p->x = (short)a1;
            p->y = (unsigned short)a2;
            c -= 0x30;
            u = c * 8;
            if (c < 10) {
                a1 += 5;
                p->w = 8;
                p->h = kh;
            } else {
                a1 += 3;
                p->w = 4;
                p->h = kh;
            }
            SetShadeTex(p, 1);
            SetSemiTrans(p, 1);
            { int v58 = 0x58; p->u = u; p->v = v58; p->clut = 0x7FA4; }
            p->addr = a0->addr;
            a0->addr = p;
            p++;
        } while (*a3 != 0);
    }
    *(int *)SCRATCHPAD = AddDrawModePrim(a0, p, 0x59);
}
