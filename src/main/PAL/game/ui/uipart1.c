#include "game/camera/campath.h"
#include "rr/frame_storage_types.h"
#include "game/camera/camrun.h"
/* RODATA 0x80010448 0x18 */
#include "types.h"
#include "scratchpad.h"
#include "rr/geometry.h"
#include "rr/gpu.h"
#include "rr/race.h"
#include "game/sound/sndvab.h"
#include "game/ui/window1.h"
#include "rr/driver_control.h"
#include "rr/bios_devices.h"
#include "rr/menu_selection.h"
#include "rr/sprite_emit.h"
#include "rr/race_setup.h"
#include "rr/menu_background.h"
#include "game/data/ldscn1.h"
#include "game/system/pad.h"
#include "game/ui/uipart1.h"
#include "rr/trig.h"
#include "game/course/trkintp.h"
#include "rr/badge_rows.h"
#include "game/ui/badgespr.h"
#include "rr/animated_glyph.h"

typedef struct { unsigned char c[4]; } B4;

typedef struct { unsigned char a, b, c, d; } S;

typedef struct {
    unsigned char u, v, w, h;
} __attribute__((packed)) R4;

typedef struct {
    unsigned char a, b, c, d;
} S_ACC68;

typedef struct {
    short s0;
    short s2;
    unsigned char b4;
    unsigned char b5;
    unsigned char b6;
    unsigned char b7;
    int w8;
    unsigned char bC;
    unsigned char bD;
    short sE;
    short s10;
    short s12;
    short s14;
    short s16;
    short s18;
    short s1A;
    short s1C;
    short s1E;
} S_F4CB0;

typedef struct { u8 c[8]; } B8;

extern S ScreenFadeState;
extern B4 ScreenFadeColors[];
extern unsigned short D_80010440[];
extern R4 D_8009EE70[];
extern R4 D_8009EEC8;
extern signed char D_8009EF3C[][4];
extern short D_8009EFD8[];
extern short D_8009EFE8[];
extern R4 D_8009EFF8;
extern void *AddSpritePrim();
extern void *AddTileSemiTransPrim();
extern S_F4CB0 MenuFrameState;
extern s8 D_8009EFFC[];
extern char D_80010460[];
extern s32 MenuStepDirection;
extern void PlaySfx();
extern char RaceTimeDigits[];
extern int NumLaps;
extern int LapTimes[];
extern char TimeTextMMSS[];
extern unsigned short D_8009F01C[];
extern char D_8009F020[];
extern int ReplayState;

void ClearScreenOverlayState(void);
void DrawMenuFrame(int arg0);
void DrawArrowTrianglePair();
void DrawMenuStepArrows();
void StepMenuValue();
int FramesToMilliseconds(int a, int b);
void DrawRaceTime();
void CopyStringClearTail(char *d, char *s);
void CopyStringPadSpaces();
void FormatDecimalDigits();
void FormatRaceTime(char *a, int b);
void DrawLapTimeList(int a0);
char *FormatFrameTimeMMSS();
void DrawTimeRemaining(int a);
void DrawTimeRemainingForPlayer(int a, int b);
char *FormatMillisecondsTime(int t);

void UpdateScreenFade(int a0) {
    B4 sp;
    int a, t, a1;
    S *st = &ScreenFadeState;
    CurrentDrawBuf->buffer.environment00.draw00.background = 0;
    a = (a0 >> 1) & 1;
    st->c = a;
    if (a != st->d) ScreenFadeState.a = 0;
    st->d = a;
    t = st->b;
    switch (ScreenFadeState.a) {
    case 0:
        sp = ScreenFadeColors[a ^ 1];
        st->b = 0;
        ScreenFadeState.a = 1;
        break;
    case 1:
        {
            B4 *p = &ScreenFadeColors[st->c];
            B4 *q = &ScreenFadeColors[st->c ^ 1];
            a1 = 0x1E - t;
            sp.c[0] = (p->c[0] * t + q->c[0] * a1) / 30;
            sp.c[1] = (p->c[1] * t + q->c[1] * a1) / 30;
            sp.c[2] = (p->c[2] * t + q->c[2] * a1) / 30;
        }
        if (t >= 0x1E) {
            ScreenFadeState.a = 2;
            st->b = 0;
        } else {
            if (t == 2) {
                if (MenuFrameState.s0 == 0) SetDispMask(1);
            }
            ScreenFadeState.b = ScreenFadeState.b + 1;
        }
        break;
    case 2:
        sp = ScreenFadeColors[st->c];
        break;
    }
    {
        int t = *(int *)SCRATCHPAD;
        *(int *)SCRATCHPAD = AddTileGrid2x2(&CurrentDrawBuf->buffer.ordering70.words[703], t, 0, 0, 0x140, 0x200, sp.c[0], sp.c[1], sp.c[2]);
    }
    DrawMenuFrame(a0 >> 2);
    if (MainMenuChoice != 6 && MainMenuChoice != 8) {
        SetPresentationDrawEnabled(a0 & 1);
        StepCameraScript();
    }
}

void ClearScreenOverlayState(void) {
    PsyqBzero(&MenuFrameState, 0x20);
    PsyqBzero(&ScreenFadeState, 0xC);
}


void DrawMenuFrame(int arg0) {
    R4 sp30;
    int s7;
    int y;
    int sp38;
    int s0;
    int s6;
    int v;
    int s1, j, m, k;
    int s4;
    int s5;
    unsigned short sp40;
    void *a1;
    int c;
    R4 *r;
    int s2;
    unsigned short *pc;
    int col1, col2, col3;

    s7 = 0x80;
    if (MenuFrameState.b6 != arg0) {
        MenuFrameState.b7 = MenuFrameState.b6;
    }
    y = 0x1C0;
    MenuFrameState.b6 = arg0;
    s0 = MenuFrameState.s2;
    s6 = MenuFrameState.b6;
    sp38 = ScreenFadeState.c;

    switch (MenuFrameState.s0) {
    case 0:
        MenuFrameState.s12 = 0x38;
        MenuFrameState.s1A = 0x1B8;
        MenuFrameState.s16 = MenuFrameState.s1E = 2;
        if (ScreenFadeState.a == 2) {
            MenuFrameState.s0 = 1;
            MenuFrameState.b4 = ScreenFadeState.c;
        }
        break;
    case 1:
        MenuFrameState.s14 = MenuFrameState.s1C = (rsin((s0 << 10) / 30) * 0x140) / 0x1000;
        if (s0 >= 30) {
            MenuFrameState.s0 = 2;
            MenuFrameState.s2 = 0;
        } else {
            MenuFrameState.s2++;
        }
        break;
    case 2:
        v = (rsin((s0 << 10) / 20) * 0x20) / 0x1000;
        MenuFrameState.s16 = MenuFrameState.s1E = v * 2 + 2;
        MenuFrameState.s12 = 0x38 - v;
        MenuFrameState.s1A = 0x1B8 - v;
        if (s0 >= 20) {
            MenuFrameState.s0 = 3;
            MenuFrameState.s2 = 0;
        } else {
            MenuFrameState.s2++;
        }
    case 3:
        if (ScreenFadeState.a == 1) {
            MenuFrameState.s0 = 4;
            MenuFrameState.s2 = 0;
        }
        if (ScreenFadeState.a != 2) {
            s6 = MenuFrameState.b7;
            sp38 = ScreenFadeState.c ^ 1;
        }
        break;
    case 4:
        if (s0 >= 20) {
            if (ScreenFadeState.a == 2) {
                MenuFrameState.s0 = 5;
                MenuFrameState.s2 = 0;
                MenuFrameState.b4 = ScreenFadeState.c;
            }
        } else {
            MenuFrameState.s2++;
        }
        s7 = ((20 - s0) * 0x80) / 20;
        sp38 = ScreenFadeState.c ^ 1;
        s6 = MenuFrameState.b7;
        break;
    case 5:
        if (s0 >= 20) {
            MenuFrameState.s0 = 3;
            MenuFrameState.s2 = 0;
        } else {
            MenuFrameState.s2++;
        }
        s7 = (s0 * 0x80) / 20;
        break;
    }

    pc = D_80010440;
    sp40 = (((sp38 + 1) & 3) << 5) | 0x1E;
    a1 = AddDrawAreaPrim(&CurrentDrawBuf->buffer.ordering70.words[0], *(void **)SCRATCHPAD, 0, 0, 0x140, 0x200);
    a1 = AddSpriteWithDrawMode(&CurrentDrawBuf->buffer.ordering70.words[0], a1, 0x10, 0x24, 0x20, 0x10, 0, 0x20, sp40, 0x7F38, 1, (unsigned short)s7);
    s1 = 0;
    s4 = 0x30;
    s5 = 0x24;
    s7++;

    c = D_8009EF3C[s6][0];
    while (c != -1) {
        r = &D_8009EE70[c];
        a1 = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, s4, s5, r, 0x7F38, ((unsigned short)s7 << 1) | 1);
        c = D_8009EF3C[s6][++s1];
        { int t = s4 + 2; s4 = t + r->w; }
    }

    if (MenuFrameState.w8 > 0) {
        a1 = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, s4, s5, &D_8009EEC8, 0x7F38, ((unsigned short)s7 << 1) | 1);
        D_8009EFF8.u = MenuFrameState.w8 * 8;
        { int t = s4 + 2; s4 = t + D_8009EEC8.w; }
        a1 = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, s4, s5, &D_8009EFF8, 0x7F38, 1, ((unsigned short)s7 << 1) | 1);
    }

    a1 = AddTileSemiTransPrim(
        &CurrentDrawBuf->buffer.ordering70.words[0], a1, 8, 0x38, 0x130, 2,
        col1 = *pc, col1, col1
    );
    a1 = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, sp40);
    a1 = AddDrawAreaPrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, MenuFrameState.s10, MenuFrameState.s12, MenuFrameState.s14, MenuFrameState.s16);

    for (j = 2; j < 7; j++) {
        s4 = D_8009EFD8[j];
        s5 = (j >= 2) ? 0x1BE : 0x1AA;
        sp30 = D_8009EE70[j];
        if ((unsigned int)(j - 3) < 2) {
            s2 = (PlayerControllers[ActivePlayerIndex & 1].protocol10 == 1);
            if (s2 == 1) {
                sp30.u += 0x14;
                if (j == 4) {
                    s4 -= 4;
                } else {
                    s4 -= 2;
                }
            }
        } else if (j == 5) {
            if (MenuFrameState.bC != 1) {
                sp30.w -= 0x14;
            }
        } else if (j == 6) {
            if (MenuFrameState.bD != 1) {
                sp30.w -= 0x1E;
            }
        }
        a1 = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, s4, s5, &sp30, 0x7F38, ((unsigned short)s7 << 1) | 1);
    }

    if (MenuFrameState.sE == 0) {
        s2 = 0x32;
    } else {
        s2 = 0x31;
    }
    if (ScreenFadeState.a == 2) {
        j = sp38 + 0x7DBA;
        a1 = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, D_8009EFE8[7], y, 1, 0xC, 0xF0, 0x7A, 0x14);
        a1 = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, D_8009EFE8[7] + 8, s5, &D_8009EE70[s2], j, 0);
    }

    col2 = *pc;
    a1 = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, sp40);
    {
        u8 *p;
        int w, w2, w3, d;
        for (j = 3; j < 7; j++) {
            a1 = AddTileSemiTransPrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, D_8009EFE8[j], (j >= 3) ? y : 0x1A8, 1, 0xC, col2, col2, col2);
            /* ASM_BARRIER: define p after last loop call so p is live-in to next block without crossing calls */
            __asm__("" : "=r"(p));
        }
        w = 0x130;
        /* ASM_BARRIER: create anti-dependency from p load to w2 birthing insn in sched1 */
        __asm__("" : "=r"(w2) : "0"(w), "r"(p));
        /* ASM_BARRIER: chain birthing insn w3 so 16(sp) store stall cycle is filled before a3=0x1B8 */
        __asm__("" : "=r"(w3) : "0"(w2));
        a1 = AddTileSemiTransPrim(
            (p = CurrentDrawBuf->bytes) + 0x70, a1, 8, 0x1B8,
            (col3 = *pc, w3), 2,
            col3, col3, ({ /* ASM_BARRIER: keep w3 live past 16(sp) store so combine preserves w3 birthing insn */ __asm__("" : "=r"(d) : "r"(w3), "m"(*(char *)col3)); col3; })
        );
        if (MenuFrameState.bD != 1) {
            a1 = AddTileSemiTransPrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, 0xEE, 0x1C6, 0x16, 2, col3, col3, col3);
        }
        if (MenuFrameState.bC != 1) {
            a1 = AddTileSemiTransPrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, 0xA8, 0x1C6, 0x12, 2, col3, col3, col3);
            /* ASM_BARRIER: make REG_N_SETS > 1 to prevent birthing_insn_p boost in sched1 */
            __asm__ volatile("" : "+r"(col3), "=r"(p) : "m"(CurrentDrawBuf), "m"(CurrentDrawBuf), "m"(CurrentDrawBuf), "m"(CurrentDrawBuf), "m"(MenuFrameState.bC), "m"(MenuFrameState.bC));
        }
        /* ASM_BARRIER: extend live range of d so global-alloc assigns %hi(CurrentDrawBuf) to $s5 */
        __asm__ ("" : : "r"(d));
        a1 = AddDrawAreaPrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, MenuFrameState.s18, MenuFrameState.s1A, MenuFrameState.s1C, MenuFrameState.s1E);
        *(void **)SCRATCHPAD = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, sp40);
    }
}

void SetMenuFrameOptions(s32 a, s32 b, s32 c) { MenuFrameState.sE = a; MenuFrameState.bC = b; MenuFrameState.bD = c; }

void DrawArrowTrianglePair(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    s32 i, j, k, t0, sel, sg, off;
    register s32 s1 __asm__("$17"); /* ASM_BARRIER: pin i+1 to s1 so loop var i gets s2 (saved reg order) */
    s8 *tb;
    Vec2s *p, *bp;
    char *ub;
    struct { Vec2s v[3]; s32 pad; B8 u; } w;
    i = 0;
    bp = w.v;
    tb = D_8009EFFC;
    t0 = *(s32 *)SCRATCHPAD;
    w.u = *(B8 *)D_80010460;
L0:
    j = 0;
L1:
    k = 0;
    sg = 1 - (i & 1) * 2;
    p = bp;
    off = j << 1;
    do {
        p->x = (*(s8 *)(off + (s32)tb) - a2) * sg + a0;
        p->y = ((s8 *)(off + tb))[1] + a1;
        off += 4;
        k++;
        p++;
    } while (k < 3);
    s1 = i + 1;
    sel = 0;
    if (a5 == s1) sel = 0 < j;
    j++;
    sg = &CurrentDrawBuf->buffer.ordering70.words[0];
    ub = (char *)&w.u;
    t0 = EmitPolyF3(sg, t0, ((s32 *)w.v)[0], ((s32 *)w.v)[1], ((s32 *)w.v)[2], ub + sel * 4, 0);
    if (j < 2) goto L1;
    i = s1;
    if (i < 2) goto L0;
    *(s32 *)SCRATCHPAD = t0;
}


void DrawMenuStepArrows(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4, s32 a5, s32 a6) {
    s32 i, k, t0, g, sel, sg;
    register s32 one __asm__("$18"); /* ASM_BARRIER: pin s-reg order (saved reg priority) */
    s32 a3; 
    register s32 a4 __asm__("$20"); /* ASM_BARRIER: pin s-reg order (saved reg priority) */
    s32 a0, a1;
    s32 a2; 
    Vec2s *p, *bp;
    char *ub;
    s8 *tb, *nv;
    struct { Vec2s v[3]; s32 pad; B8 u; } w;
    a0 = p0; a1 = p1; a2 = p2; a3 = p3;
    i = 0;
    bp = w.v;
    one = 1;
    a4 = p4;
    t0 = *(s32 *)SCRATCHPAD;
    g = MenuStepDirection;
    nv = D_8009EFFC;
    w.u = *(B8 *)D_80010460;
L0:
    k = 0;
    sg = one - (i & 1) * 2;
    p = bp;
    tb = nv;
    do {
        p->x = (tb[0] - a2) * sg + a0;
        p->y = tb[1] + a1;
        tb += 4;
        k++;
        p++;
    } while (k < 3);
    if (a3 == one) g = 0;
    if (g == one && i == 0) {
        sel = 1;
    } else {
        sel = 0;
        if (g == 2) sel = i == 1;
    }
    sel ^= a3;
    if ((a4 != a5 || i != 0) && (a4 != a6 - 1 || i != one)) {
        ub = (char *)&w.u;
        t0 = EmitPolyF3(&CurrentDrawBuf->buffer.ordering70.words[0], t0, ((s32 *)w.v)[0], ((s32 *)w.v)[1], ((s32 *)w.v)[2], ub + (sel << 2), 0);
    }
    i++;
    if (i < 2) goto L0;
    *(s32 *)SCRATCHPAD = t0;
    if (a3 == 1) {
        *(s32 *)SCRATCHPAD = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[0], t0, a0, a1 - 0xC, a3, 0x18, 0xFF, 0xFF, 0xFF);
    }
}


void StepMenuValue(int a0, int a1, char *a2, int a3, int a4) {
    int s0;
    int a;
    unsigned short b;
    switch (a1) {
    case 2:
        s0 = *(short *)a2;
        a = (a0 >> 2) & 1;
        break;
    case 4:
        s0 = *(int *)a2;
        a = (a0 >> 2) & 1;
        break;
    case 1:
    case -1:
        s0 = *(unsigned char *)a2;
        a = (a0 >> 2) & 1;
        break;
    default:
        a = (a0 >> 2) & 1;
        break;
    }
    if ((a0 & 1) == 0) {
        b = PlayerControllers[a].edges08.fields.repeat;
        if ((b & 0x8000) == 0) {
            if ((b & 0x2000) == 0) {
                if (b & 0x1000) {
                    s0 -= 1;
                    goto L;
                }
                if (b & 0x4000) {
                    s0 += 1;
                    goto L;
                }
            }
        }
    } else {
        b = PlayerControllers[a].edges08.fields.repeat;
        if ((b & 0x1000) == 0) {
          if ((b & 0x4000) == 0) {
            if (b & 0x8000) {
                MenuStepDirection = 1;
                s0 -= 1;
                goto L;
            }
            if (b & 0x2000) {
                MenuStepDirection = 2;
                s0 += 1;
                goto L;
            }
          }
        }
        MenuStepDirection = 0;
    }
    if (0) {
L:
        if ((a0 & 2) == 0) {
            if (s0 < a3) {
                if (s0 >= a4) {
                    PlaySfx(0, a);
                } else {
                    s0 = a4;
                }
            } else {
                s0 = a3 - 1;
            }
        } else {
            PlaySfx(0, a);
            if (s0 >= a3) {
                s0 = a4;
            } else if (s0 < a4) {
                s0 = a3 - 1;
            }
        }
    }
    switch (a1) {
    case 2: *(short *)a2 = s0; break;
    case 4: *(int *)a2 = s0; break;
    case 1:
    case -1: *(char *)a2 = s0; break;
    }
}

int FramesToMilliseconds(int a, int b)
{
    int q = a / 25;
    a -= q * 25;
    a = a * 40 + b;
    return q * 1000 + a;
}

void DrawRaceTime(int a0, int a1, int b, int a3, int off) {
    if (b >= 0) {
        int m = b / 1000;
        int r = m % 60;
        int t;
        m = m / 60;
        t = b % 1000;
        RaceTimeDigits[1] = m + 0x30;
        RaceTimeDigits[6] = t / 100 + 0x30;
        RaceTimeDigits[7] = t / 10 % 10 + 0x30;
        RaceTimeDigits[8] = t % 10 + 0x30;
        RaceTimeDigits[3] = r / 10 + 0x30;
        RaceTimeDigits[4] = r % 10 + 0x30;
    } else {
        RaceTimeDigits[1] = 0x3A;
        RaceTimeDigits[3] = 0x3A;
        RaceTimeDigits[4] = 0x3A;
        RaceTimeDigits[6] = 0x3A;
        RaceTimeDigits[7] = 0x3A;
        RaceTimeDigits[8] = 0x3A;
    }
    DrawDigitString12(a0, a1, RaceTimeDigits + off, a3);
}

void CopyStringClearTail(char *d, char *s)
{
    int i = 0;
    char *p;
    if (*s) {
        do {
            d[i] = *s++;
            i++;
        } while (*s);
    }
    while (*(p = d + i++)) {
        *p = 0;
    }
}

void CopyStringPadSpaces(char *d, unsigned char *s, int n) {
    int i = 0;
    if (*s != 0) do { unsigned char c = *s; s++; d[i] = c; i++; } while (*s != 0);
    if (i < n) do { d[i] = 0x20; i++; } while (i < n);
    d[i] = 0;
}

void FormatDecimalDigits(char *a, int b, int c) {
    int t;
    a[c] = b % 10 + 0x30;
    t = b / 10;
    switch (c) {
    case 1:
        a[0] = t + 0x30;
        return;
    case 2: {
        int h = b / 100;
        a[0] = h + 0x30;
        a[1] = t - h * 10 + 0x30;
        return;
    }
    case 3: {
        int k = b / 1000;
        int h = b / 100;
        a[0] = k + 0x30;
        a[1] = h - k * 10 + 0x30;
        a[2] = t - h * 10 + 0x30;
        return;
    }
    }
}

void FormatRaceTime(char *a, int b)
{
  int s;
  int t;
  int m;
  int r;
  if (b >= 0)
  {
    if (s)
    {
s = b / 1000; m = s / 60; r = s % 60; t = b % 1000;
      FormatDecimalDigits(a, m, 1);
      FormatDecimalDigits(a + 3, r, 1);
      FormatDecimalDigits(a + 6, t, 2);
      return;
    }
    else
    {
s = b / 1000; m = s / 60; r = s % 60; t = b % 1000;
      FormatDecimalDigits(a, m, 1);
      FormatDecimalDigits(a + 3, r, 1);
      FormatDecimalDigits(a + 6, t, 2);
      return;
    }
  }
  a[8] = 0x2D; a[7] = 0x2D; a[6] = 0x2D; a[4] = 0x2D; a[3] = 0x2D; a[1] = 0x2D; a[0] = 0x2D;
}

void DrawLapTimeList(int a0) {
    /* ASM_BARRIER: pin &PlayerCars[a0] base pointer to $a0 */
    register RaceVehicle *p0 asm("$4");
    int dummy;
    /* ASM_BARRIER: pin NumLaps value load to $v1 */
    register int n asm("$3");
    int s7;
    int i;
    int s3;
    int v;
    int y;
    SpritePacket *p;

    s7 = PlayerCars[a0].control.dynamics94.steering.finish20;
    p0 = &PlayerCars[a0];
    /* ASM_BARRIER: combine PlayerCars[a0] address into $a0 and start $v0 live range before NumLaps %hi */
    __asm__("" : "+r"(p0), "=r"(dummy));
    v = NumLaps;
    /* ASM_BARRIER: order prologue stack stores before NumLaps %lo load and p0->s2A8 load in sched2 */
    __asm__("" : "=m"(*p0), "=m"(NumLaps) : "r"(dummy));
    s7 = p0->control.dynamics94.steering.finish20;
    n = NumLaps;
    if (n > 0) {
        i = 0;
        do {
            if (i == s7) {
                /* ASM_BARRIER: prevent jump_optimize from merging identical then/else branches and give i higher alloc priority than GIV */
                __asm__("" : : "r"(i));
                s3 = 0x7FF8;
            } else {
                s3 = 0x7FF8;
            }
            v = LapTimes[i];
            y = i * 10 + 0x2C;
            DrawRaceTime(0xF0, y, v, s3, 0);
            p = *(SpritePacket **)SCRATCHPAD;
            p->len = 4;
            p->code = 0x64;
            p->x = 0xE7;
            p->y = y;
            p->u = i * 12 + 12;
            p->v = 0xA8;
            p->clut = s3;
            p->code |= 1;
            p->w = 0xC;
            p->h = 0xC;
            AddPrim(&CurrentDrawBuf->buffer.ordering70.words[0], p);
            p++;
            *(SpritePacket **)SCRATCHPAD = p;
            i++;
        } while (i < NumLaps);
    }
    DrawRaceTime(0xF0, 0x1C, BestLapTime, s3, 0);
}

char *FormatFrameTimeMMSS(int a) {
    if (a >= 0) {
        int s = a / 25;
        int m = s / 60;
        int r = s % 60;
        TimeTextMMSS[0] = m / 10 + 0x30;
        TimeTextMMSS[1] = m % 10 + 0x30;
        TimeTextMMSS[3] = r / 10 + 0x30;
        TimeTextMMSS[4] = r % 10 + 0x30;
    } else {
        TimeTextMMSS[0] = 0x2D; TimeTextMMSS[1] = 0x2D; TimeTextMMSS[3] = 0x2D; TimeTextMMSS[4] = 0x2D;
    }
    return TimeTextMMSS;
}

void DrawTimeRemaining(int a)
{
    char *p = (CurrentDrawBuf->buffer.packets1670 + 0x2107C);
    int r = FormatFrameTimeMMSS(a);
    int i = a < 0x5DC;
    int t = D_8009F01C[i];
    int s = (NumLaps - 3) * 10 + 0x50;
    DrawDigitString8(&CurrentDrawBuf->buffer.ordering70.words[0], 0x111, s, r, t);
    ((short *)p)[5] = s;
}

void DrawTimeRemainingForPlayer(int a, int b)
{
    char *p = CurrentDrawBuf->buffer.packets1670 + (b * 120 + 0x2107C);
    char *q;
    int r;
    int i;
    int t;
    if (b == 0) {
        q = &CurrentDrawBuf->buffer.ordering70.words[0];
    } else {
        q = &CurrentDrawBuf->buffer.secondaryOrderingB70.words[0];
    }
    r = FormatFrameTimeMMSS(a);
    i = a < 0x5DC;
    t = D_8009F01C[i];
    DrawDigitString8(q, 0x111, ((short *)p)[5], r, t);
}

char *FormatMillisecondsTime(int t)
{
    int a, b, c;
    char *p;

    a = t / 1000;
    b = a / 60;
    c = a % 60;
    a = t % 1000;
    p = D_8009F020;
    p[0] = b / 10 + '0';
    p[1] = b % 10 + '0';
    p[3] = c / 10 + '0';
    p[4] = c % 10 + '0';
    p[6] = a / 100 + '0';
    p[7] = a / 10 % 10 + '0';
    p[8] = a % 10 + '0';
    return p;
}

void ResetVehicleShake(RaceVehicle *vehicle) {
    vehicle->impactFlags1A0 = 0;
    vehicle->state1A4 = 0;
    vehicle->stateTimer1A5 = 0;
    vehicle->steerAngle1A2 = 0;
    vehicle->shakeActive19C = 0;
    vehicle->shakeTimer19E = 0;
    vehicle->shakeX198 = 0;
    vehicle->shakeY19A = 0;
    vehicle->bumpPitch1B4 = 0;
    vehicle->shakeRollOffset1B6 = 0;
    vehicle->shakePitch1B0 = 0;
    vehicle->shakeRoll1B2 = 0;
    vehicle->steeringLock1B8 = 0;
    vehicle->steerInput1BA = 0;
}

void StartCollisionShake(RaceVehicle *vehicle, s32 impactKind) {
    s16 reverseTrackHeading, perpendicularHeading, speed, strength;
    int scaledSine, panCosine, pan, shakeX, shakeY, volume;
    u16 boundedSpeed;
    vehicle->shakeActive19C = 1;
    reverseTrackHeading = 0x800 - vehicle->trackAngle1EC;
    perpendicularHeading = (reverseTrackHeading + (vehicle->lateralVelF4 < 0 ? -0x400 : 0x400)) & 0xFFF;
    if (impactKind < 3) {
        speed = vehicle->input;
        if (speed > 800) {
            boundedSpeed = 1800;
            if (speed < 1801) boundedSpeed = (u16)vehicle->input;
            scaledSine = (s16)boundedSpeed * rsin((s16)AngleDiffAbs(reverseTrackHeading, vehicle->heading1D4));
            strength = scaledSine / 65536 + 4;
        } else {
            strength = 12;
            if (speed < 240) strength = 30;
        }
        shakeX = (rsin(perpendicularHeading) * strength) >> 4;
        shakeY = (rcos(perpendicularHeading) * strength) >> 4;
        volume = 100;
        vehicle->shakeTimer19E = 30;
    } else {
        shakeX = (rsin(perpendicularHeading) * 8) >> 4;
        shakeY = (rcos(perpendicularHeading) * 8) >> 4;
        vehicle->shakeTimer19E = 15;
        volume = 50;
    }
    vehicle->shakeX198 = shakeX;
    vehicle->shakeY19A = shakeY;
    if (vehicle->flags1E4 & 0x10) {
        panCosine = rcos(AngleDiffAbs(reverseTrackHeading, vehicle->heading1D4)) >> 6;
        if (vehicle->lateralVelF4 < 0) panCosine = -panCosine;
        pan = panCosine + 0x40;
        if (vehicle == &PlayerCars[0] || vehicle == &PlayerCars[1]) {
            if (ReplayState != 2) PlaySfxVolPan(0x21, volume, (s16)pan, -1);
        }
    }
}

void SetVehicleShake(RaceVehicle *vehicle, s32 x, s32 y) {
    vehicle->shakeActive19C = 2;
    vehicle->shakeX198 = x;
    vehicle->shakeY19A = y;
    vehicle->shakeTimer19E = 15;
}
