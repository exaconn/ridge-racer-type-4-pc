#include "rr/library.h"
#include "rr/spu_tone_snapshot.h"
#include "rr/spu_tone_update.h"
#include "psyq/libsnd/cc_nrpn2.h"
void _SsSetNrpnVabAttr3(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data) {
    SsUtGetVagAtr(bank, program, tone, (u8 *)&parameters);
    parameters.max = data;
    SsUtSetVagAtr(bank, program, tone, (u8 *)&parameters);
}
