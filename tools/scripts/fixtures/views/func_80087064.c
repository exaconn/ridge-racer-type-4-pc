/* MASPSX_FLAGS: --assembler-profile=gnu */

/* GCC_VERSION: 2.7.2 */

/* CC1_FLAGS: -mhard-float */

#include "types.h"

typedef struct {
    short left;
    short right;
} SpuVolume;

typedef struct {
    unsigned int voice;
    unsigned int mask;
    SpuVolume volume;
    SpuVolume volmode;
    SpuVolume volumex;
    unsigned short pitch;
    unsigned short note;
    unsigned short sample_note;
    short envx;
    unsigned int addr;
    unsigned int loop_addr;
    int a_mode;
    int s_mode;
    int r_mode;
    unsigned short ar;
    unsigned short dr;
    unsigned short sr;
    unsigned short rr;
    unsigned short sl;
    unsigned short adsr1;
    unsigned short adsr2;
} SpuVoiceAttr;

typedef struct {
    s16 value00;
    u16 age02;
    u16 value04;
    u16 value06;
    u8 pad08[0x1D - 8];
    s8 flags1D;
    s16 field1E;
    u8 pad20[0x2A - 0x20];
    s16 field2A;
    u8 pad2C[0x36 - 0x2C];
} SpuVoiceRow36;

extern u16 _svm_okon1;

extern u16 _svm_okon2;

extern u16 _svm_orev1;

extern u16 _svm_orev2;

extern u16 _svm_onos1;

extern u16 _svm_onos2;

extern SpuVoiceRow36 _svm_voice[];

extern u8 _svm_sreg_dirty[];

extern void (*_autopan)(s32);

extern u16 _svm_sreg_buf[];

extern void (*_autovol)(s32);

extern s32 _svm_envx_ptr;

extern s32 _svm_envx_hist[];

extern u16 _svm_okof1;

extern u16 _svm_okof2;

extern s8 spuVmMaxVoice;

extern s8 _svm_auto_kof_mode;

extern void SpuSetVoiceAttr(SpuVoiceAttr *);

extern void SpuSetNoiseVoice(s32, s32);

extern void SpuSetReverbVoice(s32, s32);

extern void SpuSetKey(s32, s32);

extern void SpuGetVoiceEnvelope(s32, u16 *);

void _SsVmFlush(void) {
    SpuVoiceAttr attr;
    s32 v;
    s32 mask_all;
    s32 bit;
    s32 *p;
    u8 hi;

    _svm_envx_ptr = (_svm_envx_ptr + 1) & 0xF;
    *(s32 *)((u8 *)_svm_envx_hist + (_svm_envx_ptr << 2)) = 0;
    p = _svm_envx_hist;

    v = 0;
    if (spuVmMaxVoice > 0) {
        do {
            SpuGetVoiceEnvelope(v, &_svm_voice[v].value06);
            if (_svm_voice[v].value06 == 0) {
                _svm_envx_hist[_svm_envx_ptr] |= (1 << v);
            }
            v++;
        } while (v < spuVmMaxVoice);
    }

    if (_svm_auto_kof_mode == 0) {
        mask_all = -1;
        p = _svm_envx_hist;
        for (v = 0; v < 15; v++) {
            mask_all &= p[v];
        }
        v = 0;
        if (spuVmMaxVoice > 0) {
            do {
                bit = 1 << v;
                if (mask_all & bit) {
                    if (_svm_voice[v].flags1D == 2) {
                        if (v < 16) {
                            hi = 0;
                        } else {
                            bit = 0;
                            hi = 1 << (v - 16);
                        }
                        SpuSetNoiseVoice(0, (hi << 16) | (short)bit);
                    }
                    _svm_voice[v].flags1D = 0;
                }
                v++;
            } while (v < spuVmMaxVoice);
        }
    }

    _svm_okon1 &= ~_svm_okof1;
    _svm_okon2 &= ~_svm_okof2;

    for (v = 0; v < 24; v++) {
        if (_svm_voice[v].field1E != 0) {
            _autovol(v);
        }
        if (_svm_voice[v].field2A != 0) {
            _autopan(v);
        }
    }

    for (v = 0; v < 24; v++) {
        attr.mask = 0;
        attr.voice = 1 << v;
        if (_svm_sreg_dirty[v] & 1) {
            attr.mask = 3;
            attr.volume.left = _svm_sreg_buf[v * 8];
            attr.volume.right = _svm_sreg_buf[v * 8 + 1];
        }
        if (_svm_sreg_dirty[v] & 4) {
            attr.mask |= 0x10;
            attr.pitch = _svm_sreg_buf[v * 8 + 2];
        }
        if (_svm_sreg_dirty[v] & 8) {
            attr.mask |= 0x80;
            attr.addr = _svm_sreg_buf[v * 8 + 3] << 3;
        }
        if (_svm_sreg_dirty[v] & 0x10) {
            attr.mask |= 0x60000;
            attr.adsr1 = _svm_sreg_buf[v * 8 + 4];
            attr.adsr2 = _svm_sreg_buf[v * 8 + 5];
        }
        if (attr.mask != 0) {
            SpuSetVoiceAttr(&attr);
        }
        _svm_sreg_dirty[v] = 0;
    }

    SpuSetKey(0, ((u8)_svm_okof2 << 16) | _svm_okof1);
    SpuSetKey(1, ((u8)_svm_okon2 << 16) | _svm_okon1);
    SpuSetReverbVoice(8, ((u8)_svm_orev2 << 16) | _svm_orev1);
    SpuSetNoiseVoice(8, ((u8)_svm_onos2 << 16) | _svm_onos1);

    _svm_okof1 = 0;
    _svm_okof2 = 0;
    _svm_okon1 = 0;
    _svm_okon2 = 0;
    _svm_onos1 = 0;
    _svm_onos2 = 0;
}
