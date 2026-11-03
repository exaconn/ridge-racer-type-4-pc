#include "game/effect/carfx.h"
#include "game/effect/carfx.h"
#include "game/effect/spark.h"
extern short D_800A0F9C[];

void func_80057750() {
    int a0, a1, lo, l1, l2, a2, a3;
    lo = D_800A0F9C[0];
    a0 = *(int *)(D_800AC89C + 0x104);
    if (a0 >= lo) {
        if (a0 < D_800A0F9C[1]) {
            func_80057138(a0 - lo, D_800A0F9C[1] - lo - 0x20, lo);
        }
    }
    l2 = D_800A0F9C[2];
    a2 = *(int *)(D_800AC89C + 0x104);
    if (a2 >= l2) {
        if (a2 < D_800A0F9C[3]) {
            func_800572A8(a2 - l2, D_800A0F9C[3] - l2 - 0x20);
        }
    }
    l1 = D_800A0F9C[4];
    a1 = *(int *)(D_800AC89C + 0x104);
    if (a1 >= l1) {
        if (a1 < D_800A0F9C[5]) {
            func_8005748C(0, a1 - l1, D_800A0F9C[5] - l1 - 0x20);
        }
    }
    l1 = D_800A0F9C[6];
    a1 = *(int *)(D_800AC89C + 0x104);
    if (a1 >= l1) {
        if (a1 < D_800A0F9C[7]) {
            func_8005748C(1, a1 - l1, D_800A0F9C[7] - l1 - 0x20);
        }
    }
    l1 = D_800A0F9C[8];
    a1 = *(int *)(D_800AC89C + 0x104);
    if (a1 >= l1) {
        if (a1 < D_800A0F9C[9]) {
            func_8005748C(2, a1 - l1, D_800A0F9C[9] - l1 - 0x20);
        }
    }
}
