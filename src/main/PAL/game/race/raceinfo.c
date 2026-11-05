#include "scratchpad.h"
#include "rr/race.h"
#include "game/ui/window1.h"
#include "game/ui/hudmap2.h"
#include "game/ui/uipart1.h"
#include "game/data/ldscn1.h"
#include "game/race/raceinfo.h"
#include "rr/overlay662_distance.h"
#include "rr/matrix_workspace.h"
#include "game/race/raceset.h"

struct T { int pad[2]; int a; int b; int pad2[11]; };

extern int D_800ACC14;
extern signed char D_8009F688[];
extern int D_8009F654[];
extern int D_8009F66C[];
extern short D_800F4D44;
extern short D_800F3914;
extern unsigned char D_8009F900[];
extern int D_800F3A30;
extern short D_8009F678[];
extern int D_800F3B00[];
extern int D_800ACC28[][8];
extern short D_800AC59C;
extern short D_800ABEC8;
extern int func_8001FF84();
extern int func_8001FCC8();
extern int func_8001FD20();
extern int func_8002B814();
extern int func_80032EC8();
extern struct T *D_800ABEA4;
extern int D_800ABECC;
extern short D_800AD522;
extern char D_800AD528[];
extern short D_8010BFD0[];
extern void func_800172DC();
extern void BuildRotMatrixZ();
extern void BuildRotMatrixY();
extern void BuildRotMatrixX();
extern int func_8006E368();
extern int rsin();
extern int rcos();
extern void MulMatrix();
extern void ApplyMatrix();

void func_80032F44(int arg0, int arg1);
void func_80033368(int unusedSelection,int unusedMode);
void func_80033474(RrRaceVehicle *a0, int a1);

void func_80032F44(int arg0, int arg1) {
    char sp28[8];
    int i;
    int s4;
    int p;
    char *b;

    p = *(int *)SCRATCHPAD;
    for (i = 0; i < 4; i++) {
        int v = D_8009F688[i + (arg1 << 2)];
        int y;
        if (v == -1) {
            break;
        }
        y = 0x128 + i * 0x10;
        p = func_8001FF84(D_800ACC14 + 0x70, p, 0x16, y, &D_8009F654[v], 0x7FA4, 1);
        if (i == 1) {
            p = func_8001FF84(D_800ACC14 + 0x70, p, 0x4A, y, &D_8009F654[8], 0x7FA4, 1);
        }
    }

    s4 = arg0 >= 8;
    arg0 &= 7;
    p = func_8001FF84(D_800ACC14 + 0x70, p, 0x3A, 0x104, &D_8009F66C[s4], s4 + 0x75E4, 0);
    p = func_8001FCC8(D_800ACC14 + 0x70, p, 0x59);
    *(int *)SCRATCHPAD = func_8001FD20(D_800ACC14 + 0x70, p, 0x10, 0xE0, 2, 0x88, 0x4C, 0x4C, 0x4C);

    if (D_800F4D44 < 2) {
        D_800F3A30 = D_8009F900[D_800F3914 + (D_800F4D44 == 1 ? 8 : 0)];
    }

    i = 7;
    b = sp28 + 7;
    do {
        *b = 0;
        i--;
        b--;
    } while (i >= 0);

    for (i = 0; i < 2; i++) {
        if (i == 0) {
            func_8002B814(sp28, D_800F3A30, 0);
        } else {
            func_8002B814(sp28, D_8009F678[arg0], 3);
        }
        func_80020F20(D_800ACC14 + 0x70, 0x2E + i * 8, 0x128 + i * 0x10, sp28);
    }

    if (arg1 == 0) {
        for (i = 0; i < 2; i++) {
            int val;
            if (D_800F4D44 == 1) {
                if (i == 0) {
                    val = D_800F3B00[D_800F3914];
                } else {
                    val = D_800AC5B0[D_800F3914];
                }
            } else {
                if (i == 0) {
                    val = D_800ACC28[s4][D_800F3914];
                } else {
                    val = D_800F4CE8[s4][D_800F3914];
                }
            }
            func_80020F20(D_800ACC14 + 0x70, 0x44 + i * 8, 0x148 + i * 0x10, func_8002BDB4(val));
        }
    } else if (arg1 == 1) {
        i = 7;
        b = sp28 + 7;
        do {
            *b = 0;
            i--;
            b--;
        } while (i >= 0);

        for (i = 0; i < 2; i++) {
            if (i == 0) {
                func_8002B814(sp28, D_800AC59C - 2, 0);
            } else {
                func_8002B814(sp28, D_800ABEC8 + 1, 0);
            }
            func_80020F20(D_800ACC14 + 0x70, 0x3A - i * 0xC, 0x148 + i * 0x10, sp28);
        }
    }

    *(int *)SCRATCHPAD = func_80032EC8(D_800ACC14 + 0x70, *(int *)SCRATCHPAD, 0x16, 0xE4, arg0);
}

void func_80033368(int unusedSelection,int unusedMode) {
    struct { int x, y, z; } v;
    int i, r;
    struct T *t;
    int b;
    if (D_800AD522 != 0) {
        i = (D_8010BFC8 >> 2) % D_800ABECC;
    } else {
        int n = D_800ABECC;
        r = (D_8010BFC8 >> 2) % n;
        r++;
        i = n - r;
    }
    b = (int)D_800ABEA4;
    t = (struct T *)(i * 0x3C + b);
    v.x = t->a;
    v.z = t->b;
    D_8010BFC8 += 1;
    func_800587E4(D_800ACC14 + 0x70, D_800F2EC4, &v);
}

void func_80033474(RrRaceVehicle *a0, int a1)
{
  int sp10[3];
  char m20[0x20];
  char m40[0x20];
  int *g = (int *) SCRATCHPAD;
  int s0;
  int v0;
  int v1;
  int i;
  int k;
  char **new_var;
  char *p;
  char *q;
  short *s5;
  if (a1 >= 0x4B)
  {
    return;
  }
  if (a1 < 2)
  {
    func_800172DC();
    D_8010BFD0[0] = 0;
    D_8010BFD0[2] = -0x100;
    D_8010BFD0[1] = -0x100;
    if ((a1 == 1) && (a0 == D_800ABEF8))
    {
      i = 0;
      k = 2;
      p = D_800AD528;
      q = (char *)&D_800AC650;
      do
      {
        *((int *) (q + 0xA8)) = *((signed char *) (p + 0x2A));
        *((char *) ((*(new_var = &p)) + 0x2A)) = k;
        p += 4;
        q += 4;
        i++;
      }
      while (i < 2);
    }
  }
  else
  {
    s0 = (a1 << 11) / 75;
    v0 = rsin(s0);
    s5 = D_8010BFD0;
    if (v0 < 0)
    {
      v0 += 7;
    }
    s5[0] = v0 >> 3;
    v0 = -(rcos(s0) << 9);
    if (v0 < 0)
    {
      v0 += 0xFFF;
    }
    s5[2] = v0 >> 12;
    s5[1] = ((a1 << 8) / 75) - 0x200;
  }
  BuildRotMatrixY(m20, 0x800 - a0->rot54);
  BuildRotMatrixX(m40, a0->rot50);
  MulMatrix(m20, m40);
  BuildRotMatrixZ(m40, a0->rot58);
  MulMatrix2(m20, m40);
  ApplyMatrix(m40, D_8010BFD0, sp10);
  g[2] = a0->pos10[0] + sp10[0];
  g[3] = a0->pos10[1] + sp10[1];
  g[4] = a0->pos10[2] + sp10[2];
  v1 = rsin(i = a0->rot54) * 0x60;
  if (v1 < 0)
  {
    v1 += 0xFFF;
  }
  sp10[0] = (a0->pos10[0] + (v1 >> 12)) - g[2];
  sp10[1] = a0->pos10[1] - g[3];
  v1 = rcos(a0->rot54) * 0x60;
  if (v1 < 0)
  {
    v1 += 0xFFF;
  }
  v1 >>= 12;
  sp10[2] = (a0->pos10[2] + v1) - g[4];
  v0 = func_8006E368(sp10[0], sp10[2]);
  g[7] = 0x400 - v0;
  v0 = func_80090758((sp10[0] * sp10[0]) + (sp10[2] * sp10[2]));
  v0 = func_8006E368(sp10[1], v0 >> 6);
  g[6] = 0x400 - v0;
  g[8] = 0;
  if (a1 < 0x49)
  {
    return;
  }
  a0->flags1E4 |= 0x100;
  if ((a1 == 0x49) && (a0 == D_800ABEF8))
  {
    i = 0;
    q = (char *)&D_800AC650;
    p = D_800AD528;
    do
    {
      *((unsigned char *) (p + 0x2A)) = *((unsigned char *) (q + 0xA8));
      q += 4;
      i++;
      p += 4;
    }
    while (i < 2);
  }
}
