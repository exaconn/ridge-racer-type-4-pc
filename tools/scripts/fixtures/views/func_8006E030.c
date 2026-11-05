#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define rsin ((int (*)())rsin)

#define rcos ((int (*)())rcos)

#undef rsin

#undef rcos

/* legacy typing of shared globals in this part of the unit */
#define rsin ((int (*)())rsin)

#define rcos ((int (*)())rcos)

#undef rsin

#undef rcos

extern short rsin();

extern short rcos();

void BuildRotMatrixZ(short *a0, int a1) {
    short s = rsin(a1);
    short c = rcos(a1);
    a0[1] = -s;
    a0[0] = c;
    a0[4] = c;
    a0[2] = 0;
    a0[3] = s;
    a0[5] = 0;
    a0[6] = 0;
    a0[7] = 0;
    a0[8] = 0x1000;
}
