/* MASPSX_FLAGS: --assembler-profile=gnu */

/* GCC_VERSION: 2.7.2 */

/* CC1_FLAGS: -mhard-float */

#include "rr/spu.h"

/* legacy typing of shared globals in this part of the unit */
#define D_8010FE18 (*(u8 (*)[])&D_8010FE18)

#define D_801100EC ((SpuProgram10 *)D_801100EC)

#define D_801100F8 ((SpuTone20 *)D_801100F8)

#define func_8008A564 ((s32 (*)(s16 bank, s16 program))func_8008A564)

#undef D_8010FE18

#undef D_801100EC

#undef D_801100F8

#undef func_8008A564

s32 func_80089754(s16 bank, s16 program) {
    if(func_8008A564(bank, program)!=0)return -1;
    return D_801100EC[program].value04;
}
