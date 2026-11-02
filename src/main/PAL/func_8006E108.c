#include "types.h"
#include "m2c_macros.h"

extern int func_80090424();
extern int func_800904F4();

void func_8006E108(void *arg0, M2C_UNK arg1) {
    s16 temp_s1;
    s16 temp_v0;

    temp_s1 = func_80090424(arg1);
    temp_v0 = func_800904F4(arg1);
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
