#include "rr/spu_init.h"
#include "psyq/libspu/spukeyst.h"
#include "rr/spu_key.h"

/* The original SDK forms an MMIO byte address. Preserve complete native
 * pointers at this boundary; no address is stored in a 32-bit scalar. */
typedef char rr_spu_mmio_address_width[
    (sizeof(unsigned long) >= sizeof(void *)) ? 1 : -1];

#define STATUS(bank) ((bank)->spu_status)

int SpuGetKeyStatus(s32 voiceMask) {
    s32 selected = -1;
    s32 index = 0;
    s32 one = 1;
    SpuVoiceRegisters *voice;
    u32 keyed;
    u16 envelope;
    for (; index < 24; index++) {
        if (voiceMask & (one << index)) {
            selected = index;
            break;
        }
    }
    if (selected == -1) {
        return -1;
    }
    /* Register stride follows the actual voice-layout. */
    voice = (SpuVoiceRegisters *)(selected * sizeof(*voice) + (unsigned long)_spu_RXX);
    keyed = _spu_keystat & (1U << selected);
    envelope = voice->envx;
    if (keyed == 0) {
        return (envelope != 0) << 1;
    }
    if (envelope != 0) {
        return 1;
    }
    return 3;
}

int _spu_init(s32 warm)
{
    u32 count;
    s32 i;
    *SPU_DPCR |= 0xB0000;
    _spu_transMode = 0;
    _spu_addrMode = 0;
    _spu_tsa = 0;
    _spu_RXX->main_vol_left = 0;
    _spu_RXX->main_vol_right = 0;
    _spu_RXX->spu_control = 0;
    _spu_Fw1ts();
    _spu_RXX->main_vol_left = 0;
    _spu_RXX->main_vol_right = 0;
    count = 0;
    while (STATUS(_spu_RXX) & 0x7FF) {
        if (++count > 0xF00) {
            DebugPrintStub(SpuTimeoutFmt, SpuWaitResetStr);
            break;
        }
    }
    _spu_mem_mode = 2;
    _spu_mem_mode_plus = 3;
    _spu_mem_mode_unit = 8;
    _spu_mem_mode_unitM = 7;
    _spu_RXX->transfer_control = 4;
    _spu_RXX->reverb_vol_left = 0;
    _spu_RXX->reverb_vol_right = 0;
    _spu_RXX->key_off[0] = 0xFFFF;
    _spu_RXX->key_off[1] = 0xFFFF;
    _spu_RXX->reverb_enable[0] = 0;
    _spu_RXX->reverb_enable[1] = 0;
    for (i = 0; i < 10; i++) {
        _spu_RQ[i] = 0;
    }
    if (warm == 0) {
        _spu_RXX->pitch_modulation[0] = 0;
        _spu_RXX->pitch_modulation[1] = 0;
        _spu_RXX->noise_enable[0] = 0;
        _spu_RXX->noise_enable[1] = 0;
        _spu_RXX->cd_vol_left = 0;
        _spu_RXX->cd_vol_right = 0;
        _spu_RXX->ext_vol_left = 0;
        _spu_RXX->ext_vol_right = 0;
        _spu_tsa = 0x200;
        _spu_FwriteByIO(D_800A4818, 16);
        for (i = 0; i < 24; i++) {
            _spu_RXX->voices[i].left = 0;
            _spu_RXX->voices[i].right = 0;
            _spu_RXX->voices[i].pitch = 0x3FFF;
            _spu_RXX->voices[i].start_address = 0x200;
            _spu_RXX->voices[i].adsr1 = 0;
            _spu_RXX->voices[i].adsr2 = 0;
        }
        _spu_RXX->key_on[0] = 0xFFFF;
        _spu_RXX->key_on[1] = 0xFF;
        _spu_Fw1ts(); _spu_Fw1ts(); _spu_Fw1ts(); _spu_Fw1ts();
        _spu_RXX->key_off[0] = 0xFFFF;
        _spu_RXX->key_off[1] = 0xFF;
        _spu_Fw1ts(); _spu_Fw1ts(); _spu_Fw1ts(); _spu_Fw1ts();
    }
    _spu_inTransfer = 1;
    _spu_RXX->spu_control = 0xC000;
    _spu_transferCallback = 0;
    D_800A4814 = 0;
    return 0;
}
