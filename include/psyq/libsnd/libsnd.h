#ifndef RR_PSYQ_LIBSND_H
#define RR_PSYQ_LIBSND_H

#include "types.h"

typedef struct {
    u8 f00, f01, f02, f03, f04, f05;
    u8 pad06[0x16 - 0x06];
    u16 f16;
    u8 pad18[0x20 - 0x18];
} VagAtr;

typedef struct {
    u8 f00, f01, f02, f03, f04, pad05[11];
} ProgAtr;

#endif
