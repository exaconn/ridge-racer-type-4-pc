#include "rr/library.h"
#include "rr/spu_tone_update.h"
#include "rr/spu_tone_snapshot.h"
#include "psyq/libsnd/cc_nrpn3.h"
#include "psyq/libsnd/ccadsr.h"
void _SsSetNrpnVabAttr6(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data) {
    s32 value = data;
    SpuAdsrParameters envelope;
    SsUtGetVagAtr(bank, program, tone, (u8 *)&parameters);
    _SsUtResolveADSR(parameters.adsr1, parameters.adsr2, &envelope);
    
    envelope.decayRate = value;
    _SsUtBuildADSR(&envelope, &parameters.adsr1, &parameters.adsr2);
    SsUtSetVagAtr(bank, program, tone, (u8 *)&parameters);
}
