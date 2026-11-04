#include "game/system/modeslot.h"
/* RODATA 0x800104c4 0x2c */
#include "rr/ui.h"
#include "game/data/ldscn1.h"
#include "game/gfx/fadectl.h"
#include "game/gfx/fadein.h"
#include "game/gfx/fadesel.h"
#include "game/menu/menutran.h"
#include "game/race/racegrid.h"
#include "game/race/ranking.h"
#include "game/system/modepad.h"
#include "game/menu/menusave.h"
#include "game/system/modeinit.h"
#include "rr/audio_cd_controller.h"
#include "rr/driver_control.h"
#include "rr/frame_environment.h"
#include "game/data/ldscn1.h"
#include "game/gfx/gfxotag.h"
#include "game/system/modeinit.h"

extern int func_80053CB0();
extern int D_800102F0;
extern char D_800104A4[];
extern short D_800AC5D0;
extern int D_800AC5D4;
extern short D_800ACBD0[];
extern int D_800ACBF8;
extern int D_800ACC18;
extern short D_800AD508;
extern short D_800AD522;
extern int D_800F2EC8;
extern unsigned short D_800F3974;
extern short D_800F3A26;
extern short D_800F4BE8;
extern short D_800F4C6A;
extern int D_800F4CA8;
extern short D_800F4CD0;
extern int D_800F4D40;
extern unsigned short D_800F4D44;
extern short D_800F4D8A;
extern char D_800F4D98[];
extern short D_800FF688;
extern int D_8010BFB8;
extern int func_8003D228();
extern char D_800104B0[];
extern int D_800ABEB0;
extern char D_800ABEBC[];
extern char *D_800AC590;
extern int D_800AC5DC;
extern short D_800AC64A;
extern short D_800AD50A;
extern int D_800F2DE4;
extern short *D_800F3AFC;
extern int D_800F3A60;
extern unsigned short D_800F4C5C;
extern void func_80032A14();
extern void func_8003A198();
extern void func_8003D6F0();
extern void func_8004F24C();
extern void func_8005D974();

void func_8002E9B0();
void func_8002EAF8();
void func_8002EC94(void);

void func_8002E9B0() {
    short *a0;
    char *a1;
    int a2;
    SetDispMask(0);
    func_80053CB0();
    a2 = 0;
    a1 = D_800F4D98;
    a0 = D_800ACBD0;
    D_800AD508 = -1;
    D_800FF688 = 0;
    D_800AC5D0 = 0;
    D_800AC5D4 = 0;
    D_8010BFB8 = 0;
    D_800AD522 = 0;
    do {
        a0[0] = 0;
        a0[1] = 0;
        a1[3] = 0;
        a1 += 4;
        a2++;
        a0 += 2;
    } while (a2 < 4);
    D_800F4CA8 = 2;
    D_800F4BE8 = 4;
    D_800F4C6A = 1;
    D_800F3A26 = 0x23;
    D_800F4D8A = 0;
    D_800ACC18 = D_800102F0;
    D_800F4D40 = D_800102F0;
    if ((unsigned)(D_800F4D44 - 2) < 3U) func_8001F254(D_800102F0, a1, a2);
    D_800F4D44 = 0;
    func_8009AEAC(1, 1, 0);
    D_800ACBF8 = 0;
    D_800F2EC8 = 0;
    D_800F4CD0 = (D_800F3974 & 1) + 7;
    func_8001E84C(D_800104A4);
}

void func_8002EAF8()
{
  int i;
  int y;
  int v;
  short c7;
  func_8001ECE8(0, 0, 0, 3);
  c7 = 7;
  i = 0;
  D_800F2DE4 = 0;
  D_800ABEB0 = 0;
  D_800AC5DC = 0x80;
  D_800F4D2C = 0;
  D_800AC64A = -1;
  D_800ABEBC[3] = 0;
  D_800ABEBC[2] = 0;
  D_800ABEBC[1] = 0;
  D_800ABEBC[0] = 0;
  D_800AD50A = 0x104;
  while (i < (D_800F4CD0 + 1))
  {
    v = i;
    if (D_800F4CD0 == c7)
    {
      if (i == D_800F4CD0)
      {
        v = 8;
      }
    }
    func_8003E928(i, 0x6C, (i * 0x22) + 0x40, 0x54, 0x20, 0, 0, 1, 0x7F, v);
    i++;
  }

  if (func_8002F8E4() == 0x140)
  {
    D_800F3970 = 1;
  }
  D_800AC590[7] = 2;
  func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
  D_800F3AFC[1] = 1;
  func_8009AEAC(1, 1, 0);
  func_8001E84C(D_800104B0);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEB0 (*(unsigned (*))&D_800ABEB0)
#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)
#define D_800F3AFC ((W7C0_F *)D_800F3AFC)
#define D_800AC590 ((unsigned char *)D_800AC590)
#define D_800F4D80 ((unsigned char *)D_800F4D80)
#define func_8003D228 ((void (*)())func_8003D228)
#define func_8009AEAC ((void (*)())func_8009AEAC)

void func_8002EC94(void)
{
    int s0;
    int v1;
    int a1;
    int s0b;
    int c3, c4;
    W7C0_F *a0;

    D_800F2DE4 = D_800F2DE4 + 1;
    D_800ABEB0 = D_800ABEB0 + 1;
    s0 = func_8002F0E0();
    switch (D_800F4D8A) {
    case 0:
    case 10:
        func_8009AEAC(1, 1, 0);
        if (D_800F4BE0 > 0) {
            func_8003D6F0(0);
            if (D_800F3AFC->s10 == 3) {
                if (D_800F3A60 == 0 && D_800F4D8A != 0 && (signed char)D_800AC590[6] != 4)
                    goto call;
                if (D_800F4D8A != 0xA)
                    goto els;
                func_8004F24C(3);
                D_800AC590[4] = D_800AC590[4] + 1;
                goto c8;
            set:
                D_800AC590[4] = v1;
                goto call;
            els:
                D_800F4D8A = 0xA;
            c8:
                v1 = (signed char)D_800AC590[4];
                if (v1 < 4) {
                    c3 = 3;
                    c4 = 4;
                    a1 = D_800F4BE0;
                    a0 = D_800F3AFC;
                    do {
                        if ((a1 >> v1) & 1)
                            goto set;
                        if (v1 == c3)
                            a0->s10 = c4;
                        v1++;
                    } while (v1 < 4);
                }
            call:
                func_80032A14(D_800F3AFC->s8 + 0xA, D_800F3AFC->sA + 0x28, D_800AC590);
                return;
            } else if (D_800F3AFC->s10 == 0xB) {
                D_800F4D8A = 1;
                D_800F4BE0 = 0;
                D_800ABEB0 = 0;
                return;
            }
        } else {
            D_800F4D8A = 1;
            D_800ABEB0 = 0;
            return;
        }
        return;
    case 1:
        func_8009AEAC(1, 1, 0);
        if (D_800F3A40 & 0x800) {
            if (s0 == 0)
                func_8005C7A4();
            func_8004F24C(1);
            func_8005CA04();
            D_800AC64A = 0;
            func_80034E14();
            D_800F4D8A = 2;
            func_8003D228(0, 0x10, 0x1B6, 0x40, 0x22, 0, 0xF00);
            D_800F3AFC->s2 = 1;
        }
        if (D_800ABEB0 >= 0x2EE)
            D_800F4D8A = 5;
        return;
    case 2:
        func_80034E14();
        s0b = D_800AC64A;
        if (D_800F4D80[0x14] == 2) {
            if (func_8002F1BC() != 0) {
                D_800F4D8A = 3;
                D_800AC5D0 = 0x40;
                func_8003A198();
                func_8005C8D8(D_800F4D2C);
                D_800F3AFC->s10 = 4;
            }
        }
        if (s0b != D_800AC64A)
            D_800ABEB0 = 0;
        if (D_800ABEB0 >= 0xBB8)
            D_800F4D8A = 5;
        func_8002F100();
        func_8003D6F0(0);
        return;
    case 3:
        if (D_800AC5D0 <= 0) {
            if (func_8003A9E8() == 1) {
                D_800F4D8A = 0;
                D_800F4C6A = D_800F4C5C >> 8;
                D_800F3A26 = D_800F4C5C & 0xFF;
            }
        } else {
            D_800AC5D0 = D_800AC5D0 - 1;
        }
        func_8002F100();
        func_8003D6F0(0);
        return;
    case 5:
        func_8009AEAC(1, 1, 0);
        if (D_8010BFBC & 1) {
            D_800F4C6A = 2;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        } else {
            func_8005D974(0);
        }
        D_8010BFBC = D_8010BFBC + 1;
        if (D_8010BFBC >= 0x64)
            D_8010BFBC = 0;
        return;
    }
}

#undef D_800ABEB0
#undef D_800F4D8A
#undef D_800F3AFC
#undef D_800AC590
#undef D_800F4D80
#undef func_8003D228
#undef func_8009AEAC
