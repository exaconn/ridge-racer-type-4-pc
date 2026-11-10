#include "rr/library.h"
#include "rr/spu_tone_snapshot.h"
#include "rr/spu_tone_update.h"
#include "psyq/libsnd/utrev3.h"
#include "psyq/libsnd/cc_98.h"
void _SsSetNrpnVabAttr1(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data) {
    SsUtGetVagAtr(bank, program, tone, (u8 *)&parameters);
    parameters.mode = data;
    SsUtSetVagAtr(bank, program, tone, (u8 *)&parameters);
    if (data == 0) SsUtReverbOff();
    else if (data == 4) SsUtReverbOn();
}
