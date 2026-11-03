/* CC1_FLAGS: -mno-split-addresses */
#include "rr/spu.h"

s32 func_80089624(s16 bank, s16 program, s32 value) {
    if(func_8008A564(bank, program)!=0)return -1;
    D_801100EC[program].value01=value;
    return D_801100EC[program].value01;
}

s32 func_80089694(s16 bank, s16 program) {
    if(func_8008A564(bank, program)!=0)return -1;
    return D_801100EC[program].value01;
}

s32 func_800896E4(s16 bank, s16 program, s32 value) {
    if(func_8008A564(bank, program)!=0)return -1;
    D_801100EC[program].value04=value;
    return D_801100EC[program].value04;
}

s32 func_80089754(s16 bank, s16 program) {
    if(func_8008A564(bank, program)!=0)return -1;
    return D_801100EC[program].value04;
}

