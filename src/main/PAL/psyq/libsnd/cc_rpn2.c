#include "rr/library.h"
#include "rr/spu_tone_update.h"
#include "rr/spu_tone_snapshot.h"
#include "psyq/libsnd/cc_nrpn3.h"
#include "psyq/libsnd/cc_rpn2.h"
void _SsSetNrpnVabAttr13(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data) {
    /* Original defect: the remaining ADSR fields are not decoded here. */
    SpuAdsrParameters envelope;
    SsUtGetVagAtr(bank, program, tone, (u8 *)&parameters);
    parameters.vibT = data;
    _SsUtBuildADSR(&envelope, &parameters.adsr1, &parameters.adsr2);
    SsUtSetVagAtr(bank, program, tone, (u8 *)&parameters);
}
