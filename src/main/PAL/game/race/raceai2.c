/* RODATA 0x800106b0 0x1c */
#include "types.h"
#include "scratchpad.h"
#include "rr/player_status.h"
#include "m2c_macros.h"
#include "game/race/racegrid.h"
#include "game/sound/sndcmd.h"
#include "game/system/pad.h"
#include "game/system/pad3.h"
#include "game/ui/hudmap.h"
#include "game/ui/uipart1.h"
#include "rr/bios_events.h"
#include "rr/frame_environment.h"
#include "rr/driver_control.h"
#include "rr/menu_background.h"
#include "rr/audio_loop.h"
#include "game/data/globals.h"
#include "game/gfx/drawbuf.h"

typedef struct { int k; short f; } E;

struct w3g2s { char pad[0x17]; signed char a; char b; signed char c; short d; };

extern int func_80096F54();
extern int D_800F2AA8;
extern int D_800F2AAC;
extern int func_8002A604();
extern int D_800AC5DC;
extern int D_800F2DE4;
extern short D_800F3A26;
extern short D_800F4C6A;
extern short D_800F4D8A;
extern short D_800FF688;
extern int func_8002AF8C();
extern int func_80034FC0();
extern int func_80035024();
extern s16 D_800F4BE8;
extern char D_800ABEBE[];
extern char D_800AC648[];
extern char D_800ACBD8[];
extern char D_800F3914[];
extern char D_800F3A30[];
extern char D_800F4DA0[];
extern char D_8010C0A0[];
extern short D_800AD50A;
extern short D_800FFC10;
extern int func_8001F5B4();
extern int func_8001FCC8();
extern int func_8001FD20();
extern int func_800207D4();
extern char D_80010678[];
extern char D_80010684[];
extern short D_800AC5D0;
extern int D_800F3A60;
extern char D_80010694[];
extern char D_800106A4[];
extern int func_8004FC18();
extern s32 D_800F2EC8;
extern int func_801150CC();
extern int func_80117C78();
extern int func_801154D8();
extern int func_80118634();
extern int func_80115388();
extern int D_8010C0E0;
extern char D_800ACC68;
extern int func_8003A198();
extern signed char D_800AD4D8[];
extern E *D_800F2DE8[];
extern int D_800F39C8;
extern int D_800F4C64;
extern unsigned char D_8009F72C[];

int func_800356A4();
void func_80035768(void);
void func_80035844(void);
void func_8003597C(void);
void func_80035B84();
void func_80035E28(void);
void func_80036030(void);
void func_80036068(void);
void func_80036098(void);
void func_80036174(void);
void func_80036270(void);
void func_80036330(void);
void func_8003645C(void);
void func_80036538(void);
void func_800365BC(void);
void func_800365E4(void);
void func_8003660C(void);

void func_80035478(s32 mode) {
    u8 text[4];
    s32 remaining,row,statusY,waitY,labelY;
    s8 status;
    unsigned long address;

    if (mode>=0) {
        /* Integer cursor retains native pointer width. Only the four in-owner
         * addresses are converted back to pointers; the final decrement is dead. */
        row=3;
        address=(unsigned long)&text[3];
        do {
            *(u8*)address=0;
            row--;
            address--;
        } while (row>=0);
        row=0;
        do {
            labelY=row*194+64;
            waitY=row*194+150;
            statusY=row*194+120;
            text[0]=row+((s32)((u32)(D_800AD508^1)<<16)>>15)+49;
            func_8001F40C(12,labelY,D_80010600);
            func_8001F40C(88,labelY,text);
            status=D_800ABEBC[1-mode][row];
            switch (status) {
            case 0:
                func_8001F40C(60,statusY,D_8001060C);
                break;
            case 1:
                func_8001F40C(60,statusY,D_80010628);
                func_8001F40C(60,waitY,D_8001063C);
                break;
            case -1:
                func_8001F40C(60,statusY,D_8001064C);
                break;
            }
            row++;
        } while (row<2);
        row=D_800AC5A4/50;
        text[0]=row/10+48;
        text[1]=row%10+48;
        func_8001F40C(44,244,text);
        func_8001F40C(32,244,D_80010658);
        remaining=(s32)(500U-(u32)D_800AC5A4);
        func_8001F40C(((s32)((u32)remaining<<8)/500)+32,224,D_80010674);
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800AD508 (*(short (*))&D_800AD508)

int func_800356A4(a0, a1, a2, a3) int a0, a1, a2, a3;
{
    int r;
    if (D_800AD508 != 0) {
        r = -1;
        if (func_80096F54(D_800F2AAC) == a2) {
            if (a3 != 0) {
                if (func_80096F44(D_800F2AA8, a0, a2) == a2) goto zero;
neg:
                return -1;
            }
            goto zero;
        }
        return r;
    }
    if (func_80096F44(D_800F2AA8, a1, a2) != a2) goto neg;
    if (a3 != 0) {
        r = -1;
        if (func_80096F54(D_800F2AAC, a0, a2) != a2) return r;
    }
zero:
    return 0;
}

#undef D_800AD508

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEBC (*(char (*)[])&D_800ABEBC)

void func_80035768(void) {
    unsigned a;
    SetDispMask(0);
    func_8001ECE8(0, 0, 0, 2);
    D_800ABEBC[0] = 1;
    D_800ABEBC[1] = 0;
    D_800ABEBC[2] = 1;
    D_800ABEBC[3] = 0;
    func_8002A604();
    D_800AC5A4 = 0x1F4;
    D_800F2DE4 = 0;
    D_800AC5DC = 0x80;
    D_800F4D8A = 0;
    D_800FF688 = 0;
    a = ((unsigned)func_8009AEAC(0, 0, 0) >> 7) & 3;
    if (a == 3) {
        D_800F4C6A = 1;
        D_800F3A26 = a;
        D_800F4D8A = 0;
    } else {
        D_800F4C6A = 1;
        D_800F3A26 = 0x1E;
        D_800F4D8A = 0;
    }
}

#undef D_800ABEBC

/* legacy typing of shared globals in this part of the unit */
#define D_800AD508 (*(s16 (*))&D_800AD508)

void func_80035844(void) {
    D_800F2DE4 += 1;
    if (func_80034FC0() != 1) {
        if (func_80035024(0) != 0) {
            D_800F4BE8 = 8;
            D_800F4C6A = 1;
            D_800F3A26 = 0x23;
            D_800F4D8A = 0;
        }
        func_80035478(D_800AD508);
        func_80022694(D_800ACC14 + 0x70, 8, 0xF0, 0x138, 0xF0, D_800F2DE4 * 8, 1, 8, 8, 8, 0);
        func_80022694(D_800ACC14 + 0x70, 8, 0x10C, 0x138, 0x10C, D_800F2DE4 * 8, 1, 8, 8, 8, 0);
        func_8002AF8C(1, 1, 1);
        func_8002A344(0x1003E);
    }
}

#undef D_800AD508

void func_8003597C(void) {
    int s = D_800FF688;
    if (s == 1) goto st1;
    if (s < 2) {
        if (s == 0) goto st0;
        goto other;
    }
    if (s == 2) goto st2;
    goto other;
st0:
    SetDispMask(0);
    func_80034E84();
    func_8009AEAC(3, 0, 0);
    func_8009AEAC(5, 0, 0);
    func_8009AEAC(3, 0, 0);
    D_800FF688 = 1;
    return;
st1:
    if (func_800356A4(D_800ABEBE, D_800ABEBE - 2, 2, 1) == -1) { D_800FF688 = 3; return; }
    if (func_800356A4(D_8010C0A0, D_8010C0A0 - 0x18, 0x18, 1) == -1) { D_800FF688 = 3; return; }
    if (func_800356A4(D_800F4DA0, D_800F4DA0 - 8, 8, 1) == -1) { D_800FF688 = 3; return; }
    if (func_800356A4(D_800ACBD8, D_800ACBD8 - 8, 8, 1) == -1) { D_800FF688 = 3; return; }
    if (func_800356A4(D_800F3914, D_800F3914, 2, 0) == -1) { D_800FF688 = 3; return; }
    if (func_800356A4(D_800AC648, D_800AC648, 2, 0) == -1) { D_800FF688 = 3; return; }
    if (func_800356A4(D_800F3A30, D_800F3A30, 4, 0) == -1) { D_800FF688 = 3; return; }
    D_800FF688 = 2;
    func_80034F18();
    SetDispMask(1);
    return;
st2:
    func_8009AEAC(3, 0, 0);
    D_800FFC10 = 0;
    D_800F4BE8 = 0xA;
    D_800AD50A = 0x11A;
    D_800F4C6A = 1;
    D_800F3A26 = 0x23;
    D_800F4D8A = 0;
    return;
other:
    func_80034F18();
    SetDispMask(1);
    D_800F4C6A = 1;
    D_800F3A26 = 0x2C;
    D_800F4D8A = 0;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F2DE4 (*(unsigned int (*))&D_800F2DE4)
#define D_800F3A26 (*(unsigned short (*))&D_800F3A26)
#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)

void func_80035B84()
{
  int s3 = D_800F4D8A;
  unsigned int new_var;
  int *g;
  switch (s3)
  {
    case 0:
      SetDispMask(0);
      func_8001ECE8(0, 0, 0, 3);
      D_800AC5DC = 0x80;
      func_80036068();
      D_800F2DE4 = 0;
      D_800F4D8A += 1;
      return;

    case 1:
      SetDispMask(1);
      D_800F4D8A += 1;
      return;

    case 2:
      D_800F2DE4 += 1;
      func_8009AEAC(1, 1, 0);
      g = (int *) SCRATCHPAD;
      new_var = *g;
      *g = func_8001FCC8(D_800ACC14 + 0x70, func_8001FD20(D_800ACC14 + 0x70, func_8001F5B4(D_800ACC14 + 0x70, func_8001F5B4(D_800ACC14 + 0x70, new_var, 0x78, 0x100, D_80010678, 0x7F38, 0, 1), 0x78, 0x114, D_80010684, 0x7F38, 0, 1), 0, 0, 0x140, 0x200, 0x80, 0, 0), 0x5E);
      if (D_800F3A60 == 0)
    {
      if (D_800F2DE4 < 0xFB)
      {
        return;
      }
    }
      D_800F4D8A = 3;
      D_800AC5D0 = 0x20;
      return;

    case 3:
      if (D_800AC5D0 > 0)
    {
      D_800AC5D0 = ((unsigned short) D_800AC5D0) - 1;
    }
      func_800207D4(D_800ACC14 + 0x70, (0x20 - D_800AC5D0) * 8, 0x45);
      *((int *) SCRATCHPAD) = func_8001FD20(D_800ACC14 + 0x74, *((int *) SCRATCHPAD), 0, 0, 0x140, 0x200, (0x20 - D_800AC5D0) * 4, 0, 0);
      if (D_800AC5D0 <= 0)
    {
      D_800F4C6A = 1;
      D_800F3A26 = s3;
      D_800F4D8A = 0;
    }
      break;

  }

}

#undef D_800F2DE4
#undef D_800F3A26
#undef D_800F4D8A

/* legacy typing of shared globals in this part of the unit */
#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)
#define D_800F2DE4 (*(unsigned int (*))&D_800F2DE4)

void func_80035E28(void)
{
  int s = D_800F4D8A;
  int new_var;
  switch (s)
  {
    case 0:
      SetDispMask(0);
      func_8001ECE8(0, 0, 0, 3);
      D_800AC5DC = 0x80;
      D_800F2DE4 = 0;
      D_800F4D8A++;
      return;

    case 1:
      SetDispMask(1);
      D_800F4D8A++;
      return;

    case 2:
      D_800F2DE4++;
      new_var = *((int *) SCRATCHPAD);
      *((int *) SCRATCHPAD) = func_8001FCC8(D_800ACC14 + 0x70, func_8001F5B4(D_800ACC14 + 0x70, func_8001F5B4(D_800ACC14 + 0x70, new_var, 0x78, 0x100, D_80010694, 0x7F38, 0, 1), 0x78, 0x114, D_800106A4, 0x7F38, 0, 1), 0x3E);
      if (D_800F3A60 == 0)
    {
      if (D_800F2DE4 < 0xFB)
      {
        return;
      }
    }
      D_800AC5D0 = 0x20;
      D_800F4D8A = 3;
      return;

    case 3:
      if (D_800AC5D0 > 0)
    {
      D_800AC5D0 = D_800AC5D0 - 1;
    }
      func_800207D4(D_800ACC14 + 0x70, (0x20 - D_800AC5D0) * 8, 0x45);
      if (D_800AC5D0 <= 0)
    {
      D_800F4C6A = 1;
      D_800F3A26 = s;
      D_800F4D8A = 0;
    }
      break;

  }

}

#undef D_800F4D8A
#undef D_800F2DE4

void func_80036030(void) {
    func_8004BB54(0);
    func_8004BB54(1);
    func_8004B864(0);
    func_8004B864(1);
}

void func_80036068(void) {
    func_8004FC18(0, 0);
    func_80036030();
    D_800F2EC8 = 0;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)

void func_80036098(void) {
    if (D_800F4D8A == 0) {
        SetDispMask(0);
        func_80036068();
        D_800AC5DC = 0x80;
        D_800F4D8A = 1;
    } else if (D_800F4D8A == 1) {
        D_800F4D8A = 2;
    } else if (D_800F4D8A == 2) {
        D_800F4BE8 = 6;
        D_800F4C6A = 1;
        D_800F3A26 = 0x23;
        D_800F4D8A = 0;
    }
    *(int *)SCRATCHPAD = func_8001FD20(D_800ACC14 + 0x70, *(int *)SCRATCHPAD, 0, 0, 0x140, 0x100, 8, 8, 8);
}

#undef D_800F4D8A

void func_80036174(void) {
    func_8001ECE8(0, 0, 0, 2);
    D_800AC5DC = 0x80;
    D_800F2DE4 = 0;
    D_8010C0E0 = 0;
    D_800ACC68 = 2;
    func_8004F600();
    switch (D_800F4D44) {
    case 1:
        func_801150CC();
        D_800AD50A = 0x130;
        break;
    case 2:
    case 3:
        func_80117C78();
        D_800AD50A = 0x13A;
        break;
    case 0:
        func_801154D8();
        D_800AD50A = 0x136;
        break;
    case 4:
        func_80118634();
        D_800AD50A = 0x13E;
        break;
    case 6:
        func_80115388();
        D_800AD50A = 0x153;
        break;
    default:
        D_800AD50A = 0x103;
        break;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F2DE4 (*(u32 (*))&D_800F2DE4)

void func_80036270(void) {
    s32 temp_s0;

    D_800F2DE4 += 1;
    func_8002A344(1);
    if ((u32) D_800F2DE4 < 0x1C2U) {
        if (D_800F3A60 != 0) {
            goto block_2;
        }
        goto block_5;
    }
block_2:
    if (D_8010C0E0 <= 0) {
        D_8010C0E0 += 1;
        D_800F2DE4 = 0;
        return;
    }
block_5:
    temp_s0 = D_8010C0E0;
    if (temp_s0 == 1) {
        if (D_800F3A60 != 0) {
            D_800F2DE4 = 0;
            func_8003A198();
            D_800F4C6A = (s16) temp_s0;
            D_800F3A26 = 0xB;
            D_800F4D8A = 0;
        }
    }
}

#undef D_800F2DE4

void func_80036330(void) {
    int a2;
    int a0;
    int a1;
    a2 = D_800F39C8 + 0x1E8;
    if (D_800AD4EF >= 0) {
        a1 = 0;
        if (D_800AD4EF >= 2) {
            a1 = 2;
            if (D_800AD4EF >= 4) {
                goto b3;
            }
        }
    } else {
b3:
        a1 = 4;
    }
    a0 = 0;
    do {
        if (D_800F2DE8[a0]->f == 0) {
            D_800AD4D8[D_800AD4D8[0x17]] = a0 + 1;
            if (a0 == 0) {
                if (a1 < 4) {
                    if (func_8002B574(*(int *)(a2 + ((D_800AD4D8[0x17] - a1) << 2)), 0, a2) > D_800F4C64) {
                        D_800AD4D8[D_800AD4D8[0x17]] = 0;
                    }
                }
            }
            break;
        }
        a0 += 1;
    } while (a0 < 8);
    if (D_800AD4D8[0x19] < 9) {
        D_800AD4D8[0x19] = D_800AD4D8[D_800AD4D8[0x17]];
    }
}

void func_8003645C(void) {
    int i, j;
    int t[2];
    for (i = 0; i < 7; i++) {
        for (j = 7; i < j; j--) {
            t[0] = D_800F2DE8[j - 1]->k;
            t[1] = D_800F2DE8[j]->k;
            if (t[0] < t[1] || (t[0] == t[1] && D_800F2DE8[j]->f == 0)) {
                E *x = D_800F2DE8[j - 1];
                E *y = D_800F2DE8[j];
                D_800F2DE8[j] = x;
                D_800F2DE8[j - 1] = y;
            }
        }
    }
    if (D_800F4D44 == 1) {
        func_80036330();
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800AD4D8 (*(struct w3g2s (*))&D_800AD4D8)

void func_80036538(void) {
    struct w3g2s *p;
    if (D_800F4D44 == 1) {
        p = &D_800AD4D8;
        if (p->c > D_8009F72C[p->a / 2]) p->d = 5;
        else p->d = 4;
    }
    D_800F4C6A = 1;
    D_800F3A26 = 0x17;
    D_800F4D8A = 0;
}

#undef D_800AD4D8

void func_800365BC(void) {
    func_8003645C();
    func_80036538();
}

void func_800365E4(void) {
    func_8003645C();
    func_80036538();
}

void func_8003660C(void) {
    func_800365E4();
}
