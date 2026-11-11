#include "types.h"
#include "rr/library.h"
#include "rr/spu_tone_update.h"
#include "rr/spu_tone_snapshot.h"
#include "psyq/libsnd/cc_nrpn3.h"




void _SsSetNrpnVabAttr4(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data) {
    s32 value = data;
    SpuAdsrParameters envelope;
    SsUtGetVagAtr(bank, program, tone, (u8 *)&parameters);
    _SsUtResolveADSR(parameters.adsr1, parameters.adsr2, &envelope);
    envelope.attackExponential = 0;
    envelope.attackRate = value;
    _SsUtBuildADSR(&envelope, &parameters.adsr1, &parameters.adsr2);
    SsUtSetVagAtr(bank, program, tone, (u8 *)&parameters);
}

void _SsUtResolveADSR(s32 arg0, u32 arg1, SpuAdsrParameters *arg2) {
    u32 temp_v1;

    arg2->attackExponential = (s16) (arg0 & 0x8000);
    arg2->sustainExponential = (s16) (arg1 & 0x8000);
    arg2->sustainDecrease = (s16) (arg1 & 0x4000);
    temp_v1 = arg0 & 0xFFFF;
    arg2->releaseExponential = (s16) (arg1 & 0x20);
    arg2->attackRate = (s16) ((temp_v1 >> 8) & 0x7F);
    arg2->decayRate = (s16) ((temp_v1 >> 4) & 0xF);
    arg2->sustainLevel = (s16) (arg0 & 0xF);
    arg2->sustainRate = (s16) ((arg1 >> 6) & 0x7F);
    arg2->releaseRate = (s16) (arg1 & 0x1F);
}

void _SsUtBuildADSR(const SpuAdsrParameters *a0, u16 *a1, u16 *a2) {
    const SpuAdsrParameters *p;
    u16 *d; 
    int c;    register int t;    int v; 
    int e; 
    int x;
    int y; 
    int k, g;
    p = a0;
    d = a1;
    k = -0x8000;
    c = p->sustainExponential;
    g = p->sustainDecrease;
    c = c != 0;
    c = -c;
    t = c & k;
    v = t;
    c = p->attackExponential;
    c = c != 0;
    c = -c;
    e = c & k;
    if (g != 0) v = t | 0x4000;
    if (p->releaseExponential != 0) v |= 0x20;
    x = e | ((p->attackRate << 8) & 0x7F00);
    y = (p->decayRate * 0x10) & 0xF0;
    x |= y;
    t = p->releaseRate & 0x1F;
    x |= p->sustainLevel & 0xF;
    y = v | ((p->sustainRate << 6) & 0x1FC0) | t;
    *d = x;
    *a2 = y;
}

void _SsSetNrpnVabAttr5(s16 bank, s16 program, s16 tone, VagAtr parameters, s32 attribute, u8 data) {
    s32 value = data;
    SpuAdsrParameters envelope;
    SsUtGetVagAtr(bank, program, tone, (u8 *)&parameters);
    _SsUtResolveADSR(parameters.adsr1, parameters.adsr2, &envelope);
    envelope.attackExponential = 1;
    envelope.attackRate = value;
    _SsUtBuildADSR(&envelope, &parameters.adsr1, &parameters.adsr2);
    SsUtSetVagAtr(bank, program, tone, (u8 *)&parameters);
}
