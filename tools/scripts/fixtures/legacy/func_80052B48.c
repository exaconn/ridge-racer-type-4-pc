#include "types.h"
#include "m2c_macros.h"

extern int SsSetSerialVol();
extern u8 D_800AD558;

void CdAudioSetVolumeRaw(s32 arg0) {
    s32 var_a0;
    u32 temp_a1;

    var_a0 = (arg0 & 0xFF) * D_800AD558;
    if (var_a0 < 0) {
        var_a0 += 0xF;
    }
    temp_a1 = (u32) var_a0 >> 4;
    SsSetSerialVol(0, temp_a1, temp_a1);
}
