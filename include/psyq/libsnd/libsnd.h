#ifndef RR_PSYQ_LIBSND_H
#define RR_PSYQ_LIBSND_H

#include "types.h"

typedef struct {
    u8 f00, f01, f02, f03, f04, f05, f06, f07, f08, f09, f0A, f0B, f0C, f0D;
    u16 f0E;
    u16 f10, f12, f14, f16;
    u8 pad18[0x20 - 0x18];
} VagAtr;

typedef struct {
    u8 f00, f01, f02, f03, f04, pad05[11];
} ProgAtr;

#endif
