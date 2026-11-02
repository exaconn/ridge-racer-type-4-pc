#include "rr/race.h"

extern short D_800F4D44;
extern unsigned char D_800F4D98[];
extern int func_80037618();

void func_8002E868(a0, a1, a2) int a0; int a1; unsigned char *a2; {
    register RrRaceVehicle *p asm("$7"); /* ASM_BARRIER: pin p to a3 */
    int t;
    int v;
    int b;
    register int c asm("$3"); /* ASM_BARRIER: pin c to v1 */
    register int r asm("$4"); /* ASM_BARRIER: pin r to a0 */
    p = (RrRaceVehicle *)a0; /* ASM_BARRIER: pin p to a3 */
    r = 0;
    if (D_800F4D44 == 1) {
        a2 = D_800F4D98;
        t = p->flags1E4 & 7;
        c = a2[0];
        r = t < 4;
        b = a2[t * 4];
        r ^= 1;
        if (c == b) {
            p->slotIndex1E6 = r;
        } else if (a2[0x14] == b) {
            v = r | 2; p->slotIndex1E6 = v;
        } else if (a2[0x18] == b) {
            v = r | 4; p->slotIndex1E6 = v;
        } else if (a2[0x1C] == b) {
            v = r | 6; p->slotIndex1E6 = v;
        }
    } else if (D_800F4D44 == 5) {
        int u = p->flags1E4;
        if ((u & 7) >= 5) r = 1;
        v = (((u - 1) & 3) << 1) | r; p->slotIndex1E6 = v;
    } else if (D_800F4D44 == 6) {
        p->slotIndex1E6 = 2;
    } else {
        int w = (short)p->flags1E4 - 1;
        p->slotIndex1E6 = (w % 2 + 2) << 1;
    }
    v = 0x100; p->maxSpeed2EC = v;
    func_80037618(p, ((short)p->flags1E4 - 1) & 7, a2, p);
}
