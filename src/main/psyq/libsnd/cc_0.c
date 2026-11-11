#include "rr/library.h"
#include "rr/spu_tone_snapshot.h"
#include "rr/spu_tone_update.h"
#include "psyq/libsnd/cc_0.h"
void _SsSetNrpnVabAttr0(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data) {
    SsUtGetVagAtr(bank, program, tone, (u8 *)&parameters);
    parameters.prior = data;
    SsUtSetVagAtr(bank, program, tone, (u8 *)&parameters);
}
