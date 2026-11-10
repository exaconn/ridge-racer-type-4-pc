#ifndef RR_PSYQ_LIBSND_CC_NRPN3_H
#define RR_PSYQ_LIBSND_CC_NRPN3_H

#include "rr/spu_tone_control.h"

void _SsSetNrpnVabAttr4(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data);
/* Resolve initializes all nine fields. Build requires initialized fields;
 * the original handlers 11..14 still violate that contract (tracked debt). */
typedef struct {
    u16 attackRate, decayRate, sustainLevel, sustainRate, releaseRate;
    s16 attackExponential, sustainExponential, releaseExponential, sustainDecrease;
} SpuAdsrParameters;
RR_ASSERT_SIZE(SpuAdsrParameters, 18);
RR_ASSERT_OFFSET(SpuAdsrParameters, attackExponential, 10);
RR_ASSERT_OFFSET(SpuAdsrParameters, sustainDecrease, 16);
void _SsUtResolveADSR(s32 adsr1, u32 adsr2, SpuAdsrParameters *parameters);
void _SsUtBuildADSR(const SpuAdsrParameters *parameters, u16 *adsr1, u16 *adsr2);
void _SsSetNrpnVabAttr5(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data);

#endif /* RR_PSYQ_LIBSND_CC_NRPN3_H */
