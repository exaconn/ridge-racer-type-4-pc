/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "types.h"

#define R(o) (*(unsigned short *)(D_800A6760 + (o)))

/* legacy typing of shared globals in this part of the unit */
#define CD_cbsync (*(void (*(*))(u8, u8 *))&CD_cbsync)

#define CD_cbready (*(void (*(*))(u8, u8 *))&CD_cbready)

#undef CD_cbsync

#undef CD_cbready

/* legacy typing of shared globals in this part of the unit */
#define D_800A674C ((unsigned char *)D_800A674C)

#define D_800A6750 ((unsigned char *)D_800A6750)

#define D_800A6754 ((unsigned char *)D_800A6754)

#undef D_800A674C

#undef D_800A6750

#undef D_800A6754

/* legacy typing of shared globals in this part of the unit */
#define D_800A674C ((signed char *)D_800A674C)

#define D_800A6754 ((signed char *)D_800A6754)

#undef D_800A674C

#undef D_800A6754

/* legacy typing of shared globals in this part of the unit */
#define D_800A674C ((unsigned char *)D_800A674C)

#define D_800A6750 ((unsigned char *)D_800A6750)

#define D_800A6754 ((unsigned char *)D_800A6754)

#undef D_800A674C

#undef D_800A6750

#undef D_800A6754

extern volatile u8 *D_800A674C;

extern volatile u8 *D_800A6750;

extern volatile u8 *D_800A6754;

extern unsigned char *D_800A6758;

int CD_initvol(void) {
    unsigned char b[4];
    if (R(0x1B8) == 0 && R(0x1BA) == 0) {
        R(0x180) = 0x3FFF;
        R(0x182) = 0x3FFF;
    }
    R(0x1B0) = 0x3FFF;
    R(0x1B2) = 0x3FFF;
    R(0x1AA) = 0xC001;
    b[2] = 0x80;
    b[0] = 0x80;
    b[3] = 0;
    b[1] = 0;
    *D_800A674C = 2;
    *D_800A6754 = b[0];
    *D_800A6758 = b[1];
    *D_800A674C = 3;
    *D_800A6750 = b[2];
    *D_800A6754 = b[3];
    *D_800A6758 = 0x20;
    return 0;
}
