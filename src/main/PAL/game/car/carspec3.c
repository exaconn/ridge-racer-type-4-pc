#define D_800AC650 D_800AC650_prefix
#include "rr/race.h"
#include "game/car/carspec3.h"
#undef D_800AC650

extern int func_80049228();
extern int func_80049430();
extern int func_80049558();
extern struct { char pad[0x14]; int mode; char pad2[0x528]; RrRaceSpecEntry30 *tab; } D_800AC650;
extern short D_800F4D44;
void func_80049A60(a0) RrRaceVehicle *a0; {
    int s0 = 0;
    RrRaceSpecEntry30 *s2;
    RrRaceVehicleControl *s3;
    int a, b;
    int lo;
    /* ASM_BARRIER: pin loop counter/pointers/temp to original a3/a2/a1/a0 */
    register int k asm("$7");
    register short * a2 asm("$6");
    register short * a1 asm("$5");
    register int tt asm("$4");
    int w;
    int v1;
    register int v2;
    s2 = D_800AC650.tab + (a0->flags1E4 & 0xF) - 1;
    s3 = (RrRaceVehicleControl *)((char *)a0 + 0x1F4);
    if (D_800AC650.mode == 1) {
        s0 = func_80049558(a0, s0);
        a = (a0->flags1E4 & 0xF) - 1;
        func_80049228(a, a, s0, 0);
    } else if (D_800AC650.mode == 2) {
        func_80049228(0, 2, func_80049558(a0, 0), 0);
        s0 = func_80049558(a0, 1);
        func_80049228(1, 3, s0, 0);
        func_80049430((a0->flags1E4 & 0xF) - 1, 2, 3);
    }
    b = s3->boostTimer44;
    if (b > 0) {
        if (D_800F4D44 == 1) {
            v2 = b << 10;
            /* ASM_BARRIER: keep sll before the lh (original schedule) */
            __asm__ volatile ("" : "=r"(v2) : "0"(v2));
            v1 = s3->divisor11C;
            if (v1 < 0) v1 = -v1;
            lo = v2 / v1;
            k = 1;
            if (s0 < lo) s0 = lo;
            a2 = (short *)((char *)s2 + 2);
            a1 = (short *)((char *)s3 + 2);
            do {
                v1 = s2->ratios08[0];
                tt = ((RrRaceSpecEntry30 *)a2)->ratios08[0];
                ((RrRaceVehicleControl *)a1)->gearRatios100[0] = ((((v1 - tt) * s0) / 1024) + tt) * 4;
                a2++;
                k++;
                a1++;
            } while (k < 6);
            v1 = s2->altSpeed28;
            w = s2->baseSpeed00;
            s3->maxSpeedF8 = ((((v1 - w) * lo) / 1024) + w) << 8;
        }
    }
}
