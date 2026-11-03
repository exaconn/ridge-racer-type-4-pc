#include "rr/race_types.h"
#include "rr/menu_choice.h"
#include "game/ui/uipart1.h"
#include "game/race/raceset.h"

typedef struct { int key; short idx; } W10_Q;

typedef struct { char p[0x184]; int a; int b; char q[0x62]; short n; } W10_O;

void func_800295B8(s32);
void *func_8004F2D8(s32, s32);
void func_8004FC18(s16, s16);
s32 func_80096464(void);
extern u8 D_8009F910[];
extern s32 D_800AC5B0[];
extern s16 D_800AC5D0;
extern s32 D_800ACC28[2][8];
extern s16 D_800AD522;
extern s32 D_800F2EB8;
extern s32 D_800F3838[2][8][3];
extern s16 D_800F3914;
extern s32 D_800F3A30;
extern s32 D_800F3B00[];
extern s32 D_800F4BA8[];
extern s32 D_800F4C64;
extern s32 D_800F4C74;
extern s32 D_800F4C80[];
extern s32 D_800F4CE8[2][8];
extern s16 D_800F4D44;
extern s32 D_800FF6B0;
extern s32 D_8010C0F8[3];
extern s32 D_8010C120[3];
extern s32 D_8010C12C;
extern s16 D_8010C134;
extern s32 D_8010C140[3];
extern short D_800AC59C;
extern W10_Q *D_800F2DE8[];
extern W10_O *D_800FFC20[];
extern void func_8003CAF4();
extern unsigned short D_8009F914[];
extern unsigned short D_800AD50A;

void func_8003C254(RrRaceVehicle *arg0);
void func_8003C96C(void);
void func_8003CAB4(void);

void func_8003C254(RrRaceVehicle *arg0) {
    RrRaceVehicleControl *s0;
    s16 v1;
    s32 msg;
    s32 target;

    s0 = (RrRaceVehicleControl *)((u8 *)arg0 + 0x1F4);
    v1 = s0->steering94.count22;
    if (v1 > 0) {
        if (v1 <= D_800F3A30) {
            D_800F4BA8[v1 - 1] += 1;
            if (D_800F4BA8[s0->steering94.count22 - 1] > 0xFFFF) {
                D_800F4BA8[s0->steering94.count22 - 1] = 0x10000;
            }
            D_800F4C80[s0->steering94.count22 - 1] = func_8002B574(
                D_800F4BA8[s0->steering94.count22 - 1],
                func_80096464() % 40
            );
            if (D_800F4C80[s0->steering94.count22 - 1] > 0x927BE) {
                D_800F4C80[s0->steering94.count22 - 1] = 0x927BF;
            }
            if (D_800F4D44 == 0) {
                D_8010C120[D_8010C12C] = D_800F4C80[s0->steering94.count22 - 1];
            }
        }
    }
    if (s0->steering94.count22 * D_800F2EB8 <= arg0->second + arg0->first) {
        if (s0->steering94.count22 <= D_800F3A30) {
            s0->steering94.count22 = (u16)s0->steering94.count22 + 1;
            if (s0->steering94.count22 == 1) {
                D_8010C114 = 0;
            } else {
                D_8010C114 = 0x3C;
            }
            D_8010C108 = D_8010C120[D_8010C12C];
            D_8010C10C = D_8010C0F8[D_8010C12C];
            D_8010C104 = D_8010C12C;
            D_8010C12C = 0;
            D_8010C134 = 0;
            if (D_800F4C74 > D_800F4C80[s0->steering94.count22 - 2]) {
                if (s0->steering94.count22 != 1) {
                    func_8004F2D8(9, 0);
                    D_800F4C74 = D_800F4C80[s0->steering94.count22 - 2];
                    s0->steering94.finish20 = (u16)s0->steering94.count22 - 2;
                    if (D_800F4D44 == 0) {
                        D_8010C0F8[0] = D_8010C120[0];
                        D_8010C0F8[1] = D_8010C120[1];
                        D_8010C0F8[2] = D_8010C120[2];
                    }
                }
            }
            if ((u16)(D_800F3914 - 6) < 2U) {
                target = D_800F3A30 - 1;
            } else {
                target = D_800F3A30;
            }
            if (s0->steering94.count22 == target) {
                func_800295B8(0);
            } else if (s0->steering94.count22 > D_800F3A30) {
                if (D_800FF6B0 < 4) {
                    D_8010C134 = -1;
                    D_800FF6B0 = 4;
                    D_800AC5D0 = 0x4B;
                    func_8004FC18(0, 0x47);
                    msg = 0x13;
                    if (D_800F4D44 == 1) {
                        /* ASM_BARRIER: clobber msg on entry to mode-1 branch so CSE retains msg = 0x13 and frees $a0 for phase17 */
                        __asm__ volatile("" : "=r"(msg));
                        if (arg0->half1EE <= D_8009F910[D_800AD4D8.phase17 / 2]) {
                            msg = 0x13;
                        } else {
                            if (D_800AD4D8.retries08[D_800AD4D8.phase17] > 0) {
                                msg = 0x14;
                            } else {
                                msg = -1;
                            }
                        }
                    }
                    if (msg >= 0) {
                        func_8004F2D8(msg, 1);
                    }
                    if (arg0->half1EE < 8 && D_800F4D44 < 2) {
                        msg = 0;
                        D_800F4C64 = 0;
                        if (D_800F3A30 > 0) {
                            do {
                                D_800F4C64 += D_800F4C80[msg];
                                msg++;
                            } while (msg < D_800F3A30);
                        }
                        if (D_800F4C64 > 0x927BE) {
                            D_800F4C64 = 0x927BF;
                        }
                        if (D_800F4D44 == 0) {
                            if (D_800F4CE8[D_800AD522][D_800F3914] > D_800F4C64) {
                                D_800F4CE8[D_800AD522][D_800F3914] = D_800F4C64;
                            }
                            if (D_800ACC28[D_800AD522][D_800F3914] > D_800F4C74) {
                                D_800ACC28[D_800AD522][D_800F3914] = D_800F4C74;
                            }
                            D_800F3838[D_800AD522][D_800F3914][0] = D_8010C0F8[0];
                            D_800F3838[D_800AD522][D_800F3914][1] = D_8010C0F8[1];
                            D_800F3838[D_800AD522][D_800F3914][2] = D_8010C0F8[2];
                        } else if (D_800F4D44 == 1) {
                            if (D_800AC5B0[D_800F3914] > D_800F4C64) {
                                D_800AC5B0[D_800F3914] = D_800F4C64;
                            }
                            if (D_800F3B00[D_800F3914] > D_800F4C74) {
                                D_800F3B00[D_800F3914] = D_800F4C74;
                            }
                        }
                    }
                }
            }
        } else {
            goto check_split;
        }
    } else {
check_split:
        if (D_8010C140[D_8010C12C] + D_800F2EB8 * (s0->steering94.count22 - 1) <= arg0->second + arg0->first) {
            if (D_800F4D44 == 0) {
                D_8010C114 = 0x3C;
                D_8010C104 = D_8010C12C;
                D_8010C108 = D_8010C120[D_8010C12C];
                D_8010C10C = D_8010C0F8[D_8010C12C];
                D_8010C12C += 1;
            }
        }
    }
    if (D_8010C134 == 0) {
        msg = 9 - (D_800F3A30 - s0->steering94.count22);
        if (msg == 9) {
            msg = 0xF;
        }
        if (msg >= 6) {
            func_8004F2D8(msg, 0);
        }
        D_8010C134 = -1;
    }
}

void func_8003C96C(void)
{
  int i;
  int j;
  if (D_800FF6B0 < 4)
  {
    for (i = 0; i < D_800AC59C; i++)
    {
      W10_Q *q = D_800F2DE8[i];
      W10_O **o = &D_800FFC20[q->idx];
      q->key = (*o)->a + (*o)->b;
      (*o)->n = i;
    }
    for (i = 0; i < D_800AC59C - 1; i++)
    {
      for (j = D_800AC59C - 1; i < j; j--)
      {
        W10_Q *a2 = D_800F2DE8[j - 1];
        W10_Q *a1 = D_800F2DE8[j];
        int a0 = a2->key;
        int v1 = a1->key;
        if (a0 < v1 || (a0 == v1 && a1->idx == 0))
        {
          D_800F2DE8[j] = a2;
          D_800F2DE8[j - 1] = a1;
        }
      }
    }
  }
}

void func_8003CAB4(void) {
    func_8003CAF4();
    D_800AD50A = D_8009F914[D_800F4D44];
}
