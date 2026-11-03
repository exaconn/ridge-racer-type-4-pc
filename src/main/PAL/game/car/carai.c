#include "rr/track_projection.h"
#include "rr/race.h"
#include "game/car/carspec3.h"
#include "game/race/racedist.h"
#include "game/car/carai.h"
#include "game/course/trkproj.h"

typedef struct { char p0[0x54]; unsigned short s54, s56; char p1[4]; short s5C; unsigned short s5E; short s60; } W;
typedef struct { char p0[0x1E4]; short s1E4; } V;
extern int D_800AC738;
extern char D_80107198[];
extern int D_800F4DEC;
extern int D_800FF6B0;
extern struct { char pad[0x34]; int s34; } D_800F4DB8;

extern int func_80047A50();
extern int func_800486C4();
extern int func_80048DB0();
extern int func_80048A6C();
extern int func_80096464();

void func_8004A170(void) {
    int i;
    char *s0;
    W *s1;
    /* ASM_BARRIER: pin rand() results to $v0 as in the original */
    register int r __asm__("$2");
    int q;

    D_800AC738 = 0x1F80007C;
    func_80049C68();
    switch (D_800FF6B0) {
    case 0:
        D_800F4DEC = 0;
        break;
    case 1:
        D_800F4DB8.s34 = D_800F4DB8.s34 + 1;
        break;
    }
    for (i = 0; i < 0xB; i++) {
        s0 = D_80107198 + i * 0x320;
        s1 = (W *)(s0 + 0x1F4);
        if (((V *)s0)->s1E4 > 0) {
            func_8004A080((RrRaceVehicle *)s0);
            if (D_800FF6B0 < 2) {
                if (D_800FF6B0 == 0) {
                    r = func_80096464();
                    q = r / 1000;
                    s1->s5C = r - q * 1000 + 0x1D4C;
                    r = func_80096464(q);
                    s1->s54 = 1;
                    s1->s56 = 0x400;
                    s1->s60 = 0;
                    s1->s5E = r % 100 + 0x190;
                } else if (s1->s54 == 1) {
                    /* ASM_BARRIER: pin the sum to $v0 like the original */
                    register int u __asm__("$2");
                    /* ASM_BARRIER: pin the masked sum to $v1 like the original (s5C load goes to $v0) */
                    register int m __asm__("$3");
                    u = s1->s56;
                    u += s1->s5E;
                    s1->s56 = u;
                    /* ASM_BARRIER: keep the sh before the andi as in original */
                    __asm__ volatile("" : "=r"(u) : "0"(u));
                    m = u & 0xFFFF;
                    /* ASM_BARRIER: keep andi before the s5C load as in original */
                    __asm__ volatile("" : "=r"(m) : "0"(m));
                    if (s1->s5C < m) {
                        s1->s54 = 0;
                        r = func_80096464();
                        q = r / 1000;
                        s1->s5C = r - q * 1000 + 0x157C;
                        r = func_80096464(q);
                        s1->s5E = r % 100 + 0xC8;
                    }
                } else {
                    s1->s56 = s1->s56 - s1->s5E;
                    if (s1->s56 < s1->s5C) {
                        s1->s54 = 1;
                        r = func_80096464();
                        q = r / 1000;
                        s1->s5C = r - q * 1000 + 0x1D4C;
                        r = func_80096464(q);
                        s1->s5E = r % 100 + 0x190;
                    }
                }
            } else {
                func_80049A60(s0);
                func_8004638C(s0);
                func_80046DB8(s0);
                func_80047A50(s0);
                func_800486C4(s0);
                func_80048DB0(s0);
                func_80048A6C(s0);
            }
        }
    }
}
