#ifndef RR_PSYQ_LIBSPU_H
#define RR_PSYQ_LIBSPU_H

#include "types.h"

#include "psyq/sdk_types.h"

typedef struct {
    short left;
    short right;
} SpuVolume;

/* SDK SpuVoiceAttr with the PsyZ u_long members (checked in sdk_abi.c). */
typedef struct {
    PsyqULong voice;
    PsyqULong mask;
    SpuVolume volume;
    SpuVolume volmode;
    SpuVolume volumex;
    unsigned short pitch;
    unsigned short note;
    unsigned short sample_note;
    short envx;
    PsyqULong addr;
    PsyqULong loop_addr;
    int a_mode;
    int s_mode;
    int r_mode;
    unsigned short ar;
    unsigned short dr;
    unsigned short sr;
    unsigned short rr;
    unsigned short sl;
    unsigned short adsr1;
    unsigned short adsr2;
} SpuVoiceAttr;

#endif
