#include "m2c_macros.h"

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

void BuildRotMatrixX(void *arg0, M2C_UNK arg1) {
    s16 temp_s1;
    s16 temp_v0;

    temp_s1 = rsin(arg1);
    temp_v0 = rcos(arg1);
    M2C_FIELD(arg0, s16 *, 0) = 0x1000;
    M2C_FIELD(arg0, s16 *, 2) = 0;
    M2C_FIELD(arg0, s16 *, 4) = 0;
    M2C_FIELD(arg0, s16 *, 6) = 0;
    M2C_FIELD(arg0, s16 *, 8) = temp_v0;
    M2C_FIELD(arg0, s16 *, 0xA) = (s16) -temp_s1;
    M2C_FIELD(arg0, s16 *, 0xC) = 0;
    M2C_FIELD(arg0, s16 *, 0xE) = temp_s1;
    M2C_FIELD(arg0, s16 *, 0x10) = temp_v0;
}
