#ifndef RR_PSYQ_LIBSND_VMCHVAL_H
#define RR_PSYQ_LIBSND_VMCHVAL_H

#include "types.h"

/* Packed bank/row selectors use the low16 bits. Output pointers may overlap. */
s32 _SsVmGetSeqVol(s32 input, u16 *left, u16 *right);
s32 _SsVmGetSeqLVol(s32 input);
s32 _SsVmGetSeqRVol(s32 input);

#endif /* RR_PSYQ_LIBSND_VMCHVAL_H */
