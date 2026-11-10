#ifndef RR_PSYQ_LIBSND_CCADSR3_H
#define RR_PSYQ_LIBSND_CCADSR3_H

#include "rr/spu_tone_control.h"

void _SsSetNrpnVabAttr8(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data);
void _SsSetNrpnVabAttr9(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data);

#endif /* RR_PSYQ_LIBSND_CCADSR3_H */
