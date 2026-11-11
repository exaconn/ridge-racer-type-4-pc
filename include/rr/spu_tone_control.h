#ifndef RR_SPU_TONE_CONTROL_H
#define RR_SPU_TONE_CONTROL_H
#include "psyq/libsnd/vab_format.h"

/* Dispatch passes a complete tone record by value, an attribute-index word
 * and one data byte. The attribute index is a separate argument, not part of
 * the tone record. Tone handlers fetch the live attributes before updating;
 * reverb handlers ignore the incoming tone record. */
typedef void (*SvmToneControl)(s16 bank, s16 program, s16 tone,
                               VagAtr parameters, s32 attribute, u8 data);
extern SvmToneControl SsFCALLCcEntry[];
#endif
