/* RODATA 0x800110a0 0x1c */
/* CC1_FLAGS: -fno-strength-reduce */
#include "game/data/cdfile2.h"
#include "game/data/trkld1.h"
#include "game/data/trkld5.h"
#include "game/data/trkld5.h"
#include "game/data/trkld1.h"
#include "game/data/trkld6.h"
#include "game/course/trkres2.h"
#include "game/data/cdfile1.h"
#include "game/data/cdfile2.h"
#include "game/sound/sndcmd.h"
#include "rr/controller_hud.h"
#include "rr/course_choices.h"
#include "game/gfx/model.h"
extern struct { char pad[0x56C]; int a, b, c, d; } RaceContext;
extern struct { char pad[0x10]; int f10; char pad2[0x19CC - 0x14]; int f19cc; } SoundState;
extern short D_800F4D44;
extern short D_800F3826;
extern int D_800F6784;
extern unsigned short D_800A02C0;
extern short D_800F3914;
extern int func_800436E8();
extern int LoadVab();
extern char D_800A0234[];
extern char *D_800ACBB4;
extern char D_8010C210[];
extern int D_800A0270[];
extern int LoadVab();

void func_8004E414(void);
int func_8004E5E8(void);
int func_8004E70C(void);
int func_8004E8CC(void);
int func_8004E990();

void func_8004E414(void) {
    switch (D_800F4D44) {
    case 6:
        SoundState.f19cc = 1;
        RaceContext.a = 5;
        RaceContext.b = func_8004E3B0(0);
        RaceContext.c = -1;
        SoundState.f10 = 0;
        RaceContext.d = D_800F3826 + 1;
        break;
    case 0:
        SoundState.f19cc = 1;
        RaceContext.a = 5;
        RaceContext.b = func_8004E3B0(0);
        RaceContext.c = -1;
        RaceContext.d = -1;
        SoundState.f10 = 0;
        break;
    default:
    case 1:
    case 5:
        SoundState.f19cc = 0;
        RaceContext.a = 4;
        RaceContext.b = func_8004E3B0(0);
        RaceContext.c = -1;
        RaceContext.d = 0;
        SoundState.f10 = 0;
        break;
    case 2:
    case 3:
        SoundState.f19cc = 1;
        RaceContext.a = 5;
        RaceContext.b = func_8004E3B0(0);
        RaceContext.c = func_8004E3B0(1);
        RaceContext.d = -1;
        SoundState.f10 = 1;
        break;
    case 4:
        D_800F6784 = 1;
        RaceContext.a = 5;
        if (D_800AD508 != 0) {
            RaceContext.b = func_8004E3B0(0);
            RaceContext.c = func_8004E3B0(1);
        } else {
            RaceContext.b = func_8004E3B0(2);
            RaceContext.c = func_8004E3B0(3);
        }
        D_800ACBC8 = -1;
        D_800F4DC8 = 1;
        break;
    }
}

int func_8004E5E8(void) {
    switch (D_800A0218) {
    case 0:
        func_8004E414();
        D_800A0218 += 1;
    case 1:
        if (func_8004E194() != 0) {
            D_800A021C = 0;
            D_800A0220 = 0;
            D_800A0224 = 0;
            D_8010C1F0 = LoadCursor;
            *(int *)&D_800A02C0 = D_800F3914;
            if (D_800F3914 >= 8) {
                D_800F3914 = 7;
            }
            D_800A0218 += 1;
        }
        return 0;
    case 2:
        if (func_8004E260() != 0) {
            D_800A0218 += 1;
            D_800F3914 = D_800A02C0;
        }
        return 0;
    case 3:
        return 1;
    default:
        return 0;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define LoadCursor ((char *)LoadCursor)
#define SoundState (*(char (*)[])&SoundState)
#define D_8010C1F0 ((char *)D_8010C1F0)

int func_8004E70C(void)
{
    int i, i4, r, ret;
    char *s1, *s3;
    char *t;
    switch (D_800A021C) {
    case 0:
        ret = r = CdFileReadStart(GetR4FileIndex(2, 1), D_8010C1F0);
        if (r == 0) break;
        D_800A021C = D_800A021C + 1;
    case 1:
        if (CdFileBytesLoaded() == 0) { ret = 0; break; }
        t = D_8010C1F0 + *(int *)(D_8010C1F0 + 4);
        RegisterModelBank(t, 7);
        i = 0;
        RelocateOffsetTable(D_8010C1F0 + *(int *)(D_8010C1F0 + 8));
        s3 = SoundState;
        s1 = D_800A0234;
        D_800ACBB4 = D_8010C1F0 + *(int *)(D_8010C1F0 + 8);
        do {
            *(char **)D_8010C210 = D_8010C1F0 + *(int *)(D_8010C1F0 + i * 4 + 0x14);
            *(char **)(D_8010C210 + 4) = D_8010C1F0 + *(int *)(D_8010C1F0 + i * 4 + 0x1C);
            LoadVab(D_8010C210, *(int *)(s1 + 4), *(int *)(s1 + 8));
            s1 += 0xC;
            *(char **)(s3 + 0x2858) = D_8010C1F0 + *(int *)(D_8010C1F0 + i * 4 + 0xC);
            s3 += 4;
            i++;
        } while (i < 2);
        func_800436E8(D_8010C1F0 + *(int *)(D_8010C1F0 + 0x24));
        t = AlignUp4(D_8010C1F0 + *(int *)(D_8010C1F0 + 0x1C));
        LoadCursor = t;
        D_8010C1F0 = t;
        D_800A021C = 0;
        func_8004F600();
        ret = 1;
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}

#undef LoadCursor
#undef SoundState
#undef D_8010C1F0

/* legacy typing of shared globals in this part of the unit */
#define func_800436E8 ((int (*)())func_800436E8)
#define RegisterModelBank ((int (*)())RegisterModelBank)
#define D_8010C1F0 ((char *)D_8010C1F0)

int func_8004E8CC(void) {
    int v;
    switch (D_800A021C) {
    case 0:
        v = CdFileReadStart(GetR4FileIndex(2, 2), D_8010C1F0);
        if (v != 0) {
            D_800A021C += 1;
    case 1:
            if (CdFileBytesLoaded() != 0) {
                RegisterModelBank(D_8010C1F0 + *(int *)(D_8010C1F0 + 4), 8);
                func_800436E8(D_8010C1F0 + *(int *)(D_8010C1F0 + 8));
                D_800A021C = 0;
                return 1;
            }
            v = 0;
        }
        return v;
    default:
        return 0;
    }
}

#undef func_800436E8
#undef RegisterModelBank
#undef D_8010C1F0

/* legacy typing of shared globals in this part of the unit */
#define LoadCursor ((int *)LoadCursor)
#define D_8010C1F0 ((int *)D_8010C1F0)
#define D_8010C210 (*(int (*)[])&D_8010C210)
#define func_800436E8 ((int (*)())func_800436E8)

int func_8004E990()
{
    int r;
    int *q;
    switch (D_800A021C) {
    case 0:
        r = CdFileReadStart(GetR4FileIndex(2, 0), D_8010C1F0);
        if (r != 0) {
            D_800A021C++;
    case 1:
            if (CdFileBytesLoaded()) {
                q = D_800A0270;
                D_8010C210[0] = (int)D_8010C1F0 + D_8010C1F0[1];
                D_8010C210[1] = (int)D_8010C1F0 + D_8010C1F0[2];
                LoadVab(D_8010C210, q[1], q[2]);
                func_800436E8((int)D_8010C1F0 + D_8010C1F0[3]);
                D_800A021C = 0;
                D_8010C1F0 = (int *)((int)D_8010C1F0 + D_8010C1F0[2]);
                LoadCursor = D_8010C1F0;
                return 1;
            }
            r = 0;
        }
        break;
    default:
        r = 0;
    }
    return r;
}

#undef LoadCursor
#undef D_8010C1F0
#undef D_8010C210
#undef func_800436E8
