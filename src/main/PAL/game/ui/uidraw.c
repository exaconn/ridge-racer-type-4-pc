/* RODATA 0x80010884 0x54 */
#include "game/ui/hudmap.h"
#include "game/gfx/gfxmtx.h"
#include "game/menu/mnrows.h"
#include "game/ui/uidraw.h"
#include "rr/bios_devices.h"
#include "rr/audio_cd_controller.h"
#define EP ((E_2 *)((a0 * 16) + D_800ACC00))

typedef struct {
    short p0, f2, f4, f6, f8, fA, fC, fE, f10, f12, f14, f16, f18, f1A;
} E;

typedef struct { short f0, f2, f4, f6, f8, fA, fC, fE; } E_1;

typedef struct {
    short s0, s2, x, y, w, h, st, cnt;
} E_2;

extern int func_8003D474();
extern int rsin();
extern int rcos();
extern E *D_800F3AFC;
extern int D_800ACC00;
extern unsigned char D_8009F9C0[];
extern unsigned char D_8009F9CC[];
extern int D_800ACC14;
extern int D_800F4D8C;
extern char D_8009F9D4[];

void func_8003D6F0();
int func_8003DC50(int a0);
void func_8003DC9C(int i, short a, short b, short c, short d, short e, short f);
s32 func_8003DCEC(int a0);
int func_8003DFEC(int a);
void func_8003E040();

void func_8003D6F0(a0) int a0; {
    int s2;
    if (a0 < D_800F2ECC) {
        s2 = D_800F3AFC[a0].f12;
        switch (D_800F3AFC[a0].f10) {
        case 0:
            D_800F3AFC[a0].f1A = 2;
            D_800F3AFC[a0].f10 = 0xA;
            D_800F3AFC[a0].f18 = 0;
            D_800F3AFC[a0].f12 = 0;
            D_800F3AFC[a0].f4 = D_800F3AFC[a0].f8;
            D_800F3AFC[a0].f6 = (D_800F3AFC[a0].fA + D_800F3AFC[a0].fE) - 2;
            D_800F3AFC[a0].f14 = D_800F3AFC[a0].f8;
            D_800F3AFC[a0].f16 = (D_800F3AFC[a0].fA + D_800F3AFC[a0].fE) - 2;
            break;
        case 1:
            D_800F3AFC[a0].f18 = ((D_800F3AFC[a0].fC + 2) * rsin((s2 << 10) / 15)) / 4096;
            if (s2 < 0xF) {
                D_800F3AFC[a0].f12++;
            } else {
                D_800F3AFC[a0].f10 = 2;
                D_800F3AFC[a0].f12 = 0;
            }
            break;
        case 2:
            D_800F3AFC[a0].f1A = (D_800F3AFC[a0].fE * rsin((s2 << 10) / 15)) / 4096;
            D_800F3AFC[a0].f16 = (D_800F3AFC[a0].fA + D_800F3AFC[a0].fE) - D_800F3AFC[a0].f1A;
            D_800F3AFC[a0].f6 = D_800F3AFC[a0].f16;
            if (s2 < 0xF) {
                D_800F3AFC[a0].f12++;
            } else {
                D_800F3AFC[a0].f10 = 3;
                D_800F3AFC[a0].f12 = 0;
            }
            break;
        case 3:
            D_800F3AFC[a0].f4 = D_800F3AFC[a0].f8;
            D_800F3AFC[a0].f6 = D_800F3AFC[a0].fA;
            D_800F3AFC[a0].f14 = D_800F3AFC[a0].f8;
            D_800F3AFC[a0].f16 = D_800F3AFC[a0].fA;
            D_800F3AFC[a0].f18 = D_800F3AFC[a0].fC + 2;
            D_800F3AFC[a0].f1A = D_800F3AFC[a0].fE;
            break;
        case 4:
            D_800F3AFC[a0].f1A = (D_800F3AFC[a0].fE * rcos((s2 << 10) / 15)) / 4096;
            D_800F3AFC[a0].f16 = D_800F3AFC[a0].fA + (D_800F3AFC[a0].fE - D_800F3AFC[a0].f1A);
            D_800F3AFC[a0].f6 = D_800F3AFC[a0].f16;
            if (s2 < 0xF) {
                D_800F3AFC[a0].f12++;
            } else {
                D_800F3AFC[a0].f10 = 5;
                D_800F3AFC[a0].f12 = 0;
            }
            break;
        case 5:
            D_800F3AFC[a0].f18 = ((D_800F3AFC[a0].fC + 2) * rcos((s2 << 10) / 15)) / 4096;
            if (s2 < 0xF) {
                D_800F3AFC[a0].f12++;
            } else {
                D_800F3AFC[a0].f10 = 0xB;
                D_800F3AFC[a0].f12 = 0;
            }
            break;
        case 10:
            if (D_800F3AFC[a0].f2 != 0) {
                D_800F3AFC[a0].f10 = 1;
            }
            break;
        case 7:
            D_800F3AFC[a0].f18 = ((D_800F3AFC[a0].fC + 2) * rsin((s2 << 10) / 15)) / 4096;
            D_800F3AFC[a0].f1A = D_800F3AFC[a0].fE;
            D_800F3AFC[a0].f14 = D_800F3AFC[a0].f8;
            D_800F3AFC[a0].f16 = D_800F3AFC[a0].fA;
            if (s2 < 0xF) {
                D_800F3AFC[a0].f12++;
            } else {
                D_800F3AFC[a0].f10 = 3;
                D_800F3AFC[a0].f12 = 0;
            }
            break;
        case 6:
        case 9:
        case 11:
            break;
        case 8:
            D_800F3AFC[a0].f18 = D_800F3AFC[a0].fC + 2;
            D_800F3AFC[a0].f1A = (D_800F3AFC[a0].fE * rsin((s2 << 10) / 15)) / 4096;
            D_800F3AFC[a0].f14 = D_800F3AFC[a0].f8;
            D_800F3AFC[a0].f16 = D_800F3AFC[a0].fA + ((D_800F3AFC[a0].fE / 2) - (D_800F3AFC[a0].f1A / 2));
            if (s2 < 0xF) {
                D_800F3AFC[a0].f12++;
            } else {
                D_800F3AFC[a0].f10 = 3;
                D_800F3AFC[a0].f12 = 0;
            }
            break;
        }
        func_8003D474(a0);
    }
}

int func_8003DC50(int a0) {
    int n = D_800AC540 << 4;
    D_800ACC00 = a0;
    func_80096434(a0, n);
    func_8001E84C(D_80010874, n);
    return n;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC00 ((E_1 *)D_800ACC00)

void func_8003DC9C(int i, short a, short b, short c, short d, short e, short f) {
    if (i < D_800AC540) {
        E_1 *p = (E_1 *)((i << 4) + (int)D_800ACC00);
        p->f0 = f;
        p->f2 = 0;
        p->f4 = a;
        p->f6 = b;
        p->f8 = c;
        p->fA = d;
        p->fC = e;
    }
}

#undef D_800ACC00

int func_8003DCEC(int a0) {
    int lx[4];
    int ly[4];
    int lv[4];
    int r;
    int hh;
    int v;
    int k;
    int a1, a3;
    E_2 *e0, *e1, *e2, *e3, *e4, *e5;
    unsigned char *p;
    int *q;
    int *q2;
    int *xs, *ys;
    unsigned char *tb;

    v = a0 < D_800AC540;
    if (v != 0) {
        r = 0;
        e0 = EP;
        a1 = e0->st;
        a3 = e0->cnt;
        k = 3;
        q = &lv[3];
        do {
            *q = 0;
            k--;
            q--;
        } while (k >= 0);
        e1 = EP;
        lx[0] = e1->x;
        ly[0] = e1->y;
        lx[1] = e1->x + e1->w - 1;
        ly[1] = e1->y;
        lx[2] = e1->x;
        ly[2] = e1->y + e1->h - 1;
        lx[3] = e1->x + e1->w - 1;
        ly[3] = e1->y + e1->h - 1;
        switch (a1) {
        case 0:
            e2 = EP;
            e2->st = 1;
            e2->cnt = 0;
            break;
        case 5:
            r = 1;
            hh = 0x100;
            k = 3;
            q2 = &lv[3];
            do {
                *q2 = hh;
                k--;
                q2--;
            } while (k >= 0);
            break;
        case 4:
            lv[2] = 0x100;
        case 3:
            lv[1] = 0x100;
        case 2:
            lv[0] = 0x100;
        case 1:
            lv[a1 - 1] = (a3 << 8) / 6;
            if (a3 < 6) {
                e3 = EP;
                e3->cnt = e3->cnt + 1;
            } else {
                e4 = EP;
                e4->cnt = 0;
                e4->st = e4->st + 1;
            }
            break;
        case 8:
            e5 = EP;
            if (e5->s2 != 0) {
                e5->st = 1;
            }
            break;
        default:
            break;
        }
        k = 0;
        xs = lx;
        ys = ly;
        tb = D_8009F9CC;
        p = D_8009F9C0;
    loop:
        func_80022694(D_800ACC14 + 0x70, *(int *)((char *)xs + (p[0] << 2)), *(int *)((char *)ys + (p[0] << 2)), *(int *)((char *)xs + (p[1] << 2)), *(int *)((char *)ys + (p[1] << 2)), lv[*(unsigned char *)(k + (int)tb)], 1, 8, 8, 8, -1);
        p += 2;
        k++;
        if (k < 6) goto loop;
        v = r;
    }
    return v;
}

int func_8003DFEC(int a) {
    int sz = D_800F4C1C * 28;
    D_800F4D8C = a;
    func_80096434(a, sz);
    func_8001E84C(D_800108D8, sz);
    return sz;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4D8C ((char *)D_800F4D8C)

void func_8003E040(a0, a1, a2, a3)
int a0;
int a1;
int a2;
int a3;
{
  char *s;
  char *new_var;
  char *q;
  if (a0 < D_800F4C1C)
  {
    *((char *) (((a0 * 0x1C) + ((int) D_800F4D8C)) + 2)) = a1;
    *((char *) (((a0 * 0x1C) + ((int) D_800F4D8C)) + 7)) = a2;
    s = D_8009F9D4 + (a3 * 0x14);
    new_var = (char *) ((a0 * 0x1C) + ((int) D_800F4D8C));
    q = new_var;
    *((short *) (q + 8)) = *((short *) (s + 0));
    *((short *) (q + 0xA)) = *((short *) (s + 2));
    *((short *) (q + 0xC)) = *((short *) (s + 4));
    *((short *) (q + 0xE)) = *((short *) (s + 6));
    q[1] = s[0xE];
    *((char *) (((a0 * 0x1C) + ((int) D_800F4D8C)) + 5)) = s[0xF];
    *((char *) (((a0 * 0x1C) + ((int) D_800F4D8C)) + 6)) = s[0x10];
    q = (char *) ((a0 * 0x1C) + ((int) D_800F4D8C));
    *((short *) (q + 0x10)) = *((short *) (s + 8));
    *((short *) (q + 0x12)) = *((short *) (s + 0xA));
    q[0x14] = s[0xC];
    *((char *) (((a0 * 0x1C) + ((int) D_800F4D8C)) + 0x15)) = s[0xD];
  }
}

#undef D_800F4D8C
