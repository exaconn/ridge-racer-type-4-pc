#include "m2c_macros.h"

s32 PackChoiceGlyph(void *arg0) {
    return (M2C_FIELD(arg0, u8 *, 0) * 0xA0) + (M2C_FIELD(arg0, u8 *, 1) * 0x28) + M2C_FIELD(arg0, u8 *, 2);
}
