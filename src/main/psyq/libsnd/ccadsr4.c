#include "rr/library.h"
#include "rr/spu_tone_update.h"
#include "rr/spu_tone_snapshot.h"
#include "psyq/libsnd/cc_nrpn3.h"
#include "psyq/libsnd/ccadsr4.h"

void _SsSetNrpnVabAttr10(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data) {
    s32 value = data;
    SpuAdsrParameters envelope;
    SsUtGetVagAtr(bank, program, tone, (u8 *)&parameters);
    _SsUtResolveADSR(parameters.adsr1, parameters.adsr2, &envelope);
    envelope.releaseExponential = 0;
    envelope.releaseRate = value;
    _SsUtBuildADSR(&envelope, &parameters.adsr1, &parameters.adsr2);
    SsUtSetVagAtr(bank, program, tone, (u8 *)&parameters);
}

void _SsSetNrpnVabAttr11(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data) {
    s32 value = data;
    /* Original defect: the remaining ADSR fields are not decoded here. */
    SpuAdsrParameters envelope;
    SsUtGetVagAtr(bank, program, tone, (u8 *)&parameters);
    envelope.releaseExponential = 1;
    envelope.releaseRate = value;
    _SsUtBuildADSR(&envelope, &parameters.adsr1, &parameters.adsr2);
    SsUtSetVagAtr(bank, program, tone, (u8 *)&parameters);
}
