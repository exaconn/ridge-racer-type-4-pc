#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800F2AB0 ((E *)D_800F2AB0)

#undef D_800F2AB0

/* legacy typing of shared globals in this part of the unit */
#define D_800F2AB0 ((E_1 *)D_800F2AB0)

#undef D_800F2AB0

/* legacy typing of shared globals in this part of the unit */
#define D_800F2AB0 ((E_2 *)D_800F2AB0)

#undef D_800F2AB0

extern s32 rcos();

extern int D_800F2AB0;

extern int func_8003DCEC();

int func_8003FBA8(int a0) {
    int s1;
    int s2;
    int state;
    unsigned short val;
    int pad[16];

    s2 = 0;
    state = D_800F2AB0[a0].f1;
    s1 = D_800F2AB0[a0].f2;
    switch (state) {
    case -1:
        if (func_8003DCEC(D_800F2AB0[a0].f3) == 1) {
            D_800F2AB0[a0].f1 = 0;
        }
        break;
    case 0:
        D_800F2AB0[a0].f2 = 0;
        D_800F2AB0[a0].f1 = 1;
    case 1:
        s2 = 1;
        val = (s1 << 7) / 30;
        func_8003FE78(a0, 0, val, 0);
        if (s1 >= 30) {
            D_800F2AB0[a0].f2 = 0;
            D_800F2AB0[a0].f1 = 2;
        } else {
            D_800F2AB0[a0].f2++;
        }
        break;
    case 2:
    case 4:
    default:
        s2 = 2;
        func_8003FE78(a0, 0, 0x80, 0);
        break;
    case 3:
        val = rcos((s1 << 10) / 30) / 32;
        func_8003FE78(a0, 1, val, 0);
        if (s1 >= 30) {
            D_800F2AB0[a0].f1 = 5;
            D_800F2AB0[a0].f2 = 0;
        } else {
            D_800F2AB0[a0].f2++;
        }
        break;
    case 5:
        break;
    case 6:
        s2 = 1;
        val = ((30 - s1) << 7) / 30;
        func_8003FE78(a0, 0, val, 0);
        if (s1 >= 30) {
            D_800F2AB0[a0].f2 = 0;
            D_800F2AB0[a0].f1 = 7;
        } else {
            D_800F2AB0[a0].f2++;
        }
        break;
    case 7:
        s2 = 1;
        func_8003FE78(a0, 0, 0, 0);
        break;
    }
    return s2;
}
