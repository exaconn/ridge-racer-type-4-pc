#include "types.h"
#include "m2c_macros.h"

extern M2C_UNK CD_pos;

M2C_UNK *CdLastPos(void) {
    return &CD_pos;
}
