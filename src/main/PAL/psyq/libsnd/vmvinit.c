#include "psyq/libspu/libspu.h"
/* RODATA 0x800117b8 0x3c */
/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libsnd/vmpitch.h"
#include "psyq/libsnd/vmvinit.h"
#include "rr/spu.h"
#include "rr/channels.h"

extern char D_8010F90A[];
extern char D_8010F90E[];
extern char D_8010F918[];
extern char D_8010F91A[];
extern char D_8010F91C[];
extern char D_8010F91E[];
extern unsigned short _spu_voice_centerNote[];

void SsUtAllKeyOff(s32 mode) {
    SpuVoiceAttr L;
    short s0;
    int v0, v1, a1;
    int c1, c18, cff;
    L.mask = 0x60093;
    s0 = 0;
    L.pitch = 0x1000;
    L.addr = 0x1000;
    L.adsr1 = 0x80FF;
    L.volume.left = 0;
    L.volume.right = 0;
    L.adsr2 = 0x4000;
    if (spuVmMaxVoice > 0) {
        c1 = 1;
        c18 = 0x18;
        cff = 0xFF;
        do {
            v1 = s0;
            a1 = c1 << v1;
            if ((_snd_vmask & a1) == 0) {
                v0 = v1 * 0x36;
                *(short *)(D_8010F90A + v0) = c18;
                *(short *)(D_8010F90E + v0) = 0;
                *(short *)(D_8010F918 + v0) = cff;
                *(short *)(D_8010F91A + v0) = 0;
                *(short *)(D_8010F91C + v0) = 0;
                *(short *)(D_8010F91E + v0) = cff;
                L.voice = a1;
                SpuSetVoiceAttr(&L);
                _svm_voice_cursor = s0;
                _SsVmKeyOffNow(1);
            }
            s0++;
        } while (s0 < spuVmMaxVoice);
    }
}

void SpuSetVoiceAttr(SpuVoiceAttr *attributes) {
    volatile struct { int counter; unsigned int accumulator; } work;
    int voiceIndex;
    int registerIndex;
    unsigned int mask;
    int allFields;
    int volumeMode;
    unsigned int value;
    int envelopeMode;
    unsigned int note;

    mask = attributes->mask;
    allFields = (mask == 0);
    /* The address helper takes halfword indices. Convert that same index to
     * the complete channel block, preserving the live hardware-bank root. */
    for (voiceIndex = 0; voiceIndex < 24; voiceIndex++) {
        if (attributes->voice & (1 << voiceIndex)) {
            registerIndex = voiceIndex * 8;
            if (allFields || (mask & 0x10)) {
                ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->pitch = attributes->pitch;
            }
            if (allFields || (mask & 0x40)) {
                _spu_voice_centerNote[voiceIndex] = attributes->sample_note;
            }
            if (allFields || (mask & 0x20)) {
                value = _spu_voice_centerNote[voiceIndex];
                note = attributes->note;
                ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->pitch =
                    _spu_note2pitch(value >> 8, value & 0xFF, note >> 8, note & 0xFF);
            }
            if (allFields || (mask & 0x1)) {
                volumeMode = 0;
                value = attributes->volume.left & 0x7FFF;
                if (allFields || (mask & 0x4)) {
                    switch (attributes->volmode.left) {
                    case 1:
                        volumeMode = 0x8000;
                        break;
                    case 2:
                        volumeMode = 0x9000;
                        break;
                    case 3:
                        volumeMode = 0xA000;
                        break;
                    case 4:
                        volumeMode = 0xB000;
                        break;
                    case 5:
                        volumeMode = 0xC000;
                        break;
                    case 6:
                        volumeMode = 0xD000;
                        break;
                    case 7:
                        volumeMode = 0xE000;
                        break;
                    }
                }
                if (volumeMode != 0) {
                    if (attributes->volume.left >= 0x80) {
                        value = 0x7F;
                    } else if (attributes->volume.left < 0) {
                        value = 0;
                    }
                }
                ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->left = value | volumeMode;
            }
            if (allFields || (mask & 0x2)) {
                volumeMode = 0;
                value = attributes->volume.right & 0x7FFF;
                if (allFields || (mask & 0x8)) {
                    switch (attributes->volmode.right) {
                    case 1:
                        volumeMode = 0x8000;
                        break;
                    case 2:
                        volumeMode = 0x9000;
                        break;
                    case 3:
                        volumeMode = 0xA000;
                        break;
                    case 4:
                        volumeMode = 0xB000;
                        break;
                    case 5:
                        volumeMode = 0xC000;
                        break;
                    case 6:
                        volumeMode = 0xD000;
                        break;
                    case 7:
                        volumeMode = 0xE000;
                        break;
                    }
                }
                if (volumeMode != 0) {
                    if (attributes->volume.right >= 0x80) {
                        value = 0x7F;
                    } else if (attributes->volume.right < 0) {
                        value = 0;
                    }
                }
                ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->right = value | volumeMode;
            }
            if (allFields || (mask & 0x80)) {
                _spu_FsetRXXa(registerIndex | 3, attributes->addr);
            }
            if (allFields || (mask & 0x10000)) {
                _spu_FsetRXXa(registerIndex | 7, attributes->loop_addr);
            }
            if (allFields || (mask & 0x20000)) {
                ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->adsr1 = attributes->adsr1;
            }
            if (allFields || (mask & 0x40000)) {
                ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->adsr2 = attributes->adsr2;
            }
            if (allFields || (mask & 0x800)) {
                value = attributes->ar;
                if (value >= 0x80) {
                    value = 0x7F;
                }
                envelopeMode = 0;
                if (allFields || (mask & 0x100)) {
                    switch (attributes->a_mode) {
                    case 5:
                        envelopeMode = 0x80;
                        break;
                    }
                }
                {
                    /* ASM_BARRIER: Keep loaded hardware register in $v0 before masking into $v1. */
                    register unsigned short currentEnvelope __asm__("$2") =
                        ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->adsr1;
                    /* ASM_BARRIER: Keep masked hardware register in $v1 across envelope combine. */
                    register unsigned int preservedBits __asm__("$3") = currentEnvelope & 0xFF;
                    ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->adsr1 =
                        preservedBits | ((value | envelopeMode) << 8);
                }
            }
            if (allFields || (mask & 0x1000)) {
                value = attributes->dr;
                if (value >= 0x10) {
                    value = 0xF;
                }
                ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->adsr1 =
                    (((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->adsr1 & 0xFF0F) | (value << 4);
            }
            if (allFields || (mask & 0x2000)) {
                value = attributes->sr;
                if (value >= 0x80) {
                    value = 0x7F;
                }
                envelopeMode = 0x100;
                if (allFields || (mask & 0x200)) {
                    switch (attributes->s_mode) {
                    case 1:
                        envelopeMode = 0;
                        break;
                    case 5:
                        envelopeMode = 0x200;
                        break;
                    case 7:
                        envelopeMode = 0x300;
                        break;
                    }
                }
                {
                    /* ASM_BARRIER: Keep loaded hardware register in $v0 before masking into $v1. */
                    register unsigned short currentEnvelope __asm__("$2") =
                        ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->adsr2;
                    /* ASM_BARRIER: Keep masked hardware register in $v1 across envelope combine. */
                    register unsigned int preservedBits __asm__("$3") = currentEnvelope & 0x3F;
                    ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->adsr2 =
                        preservedBits | ((value | envelopeMode) << 6);
                }
            }
            if (allFields || (mask & 0x4000)) {
                value = attributes->rr;
                if (value >= 0x20) {
                    value = 0x1F;
                }
                envelopeMode = 0;
                if (allFields || (mask & 0x400)) {
                    switch (attributes->r_mode) {
                    case 3:
                        break;
                    case 7:
                        envelopeMode = 0x20;
                        break;
                    }
                }
                ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->adsr2 =
                    (((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->adsr2 & 0xFFC0) | (value | envelopeMode);
            }
            if (allFields || (mask & 0x8000)) {
                value = attributes->sl;
                if (value >= 0x10) {
                    value = 0xF;
                }
                ((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->adsr1 =
                    (((SpuVoiceRegisters *)((u8 *)_spu_RXX + registerIndex * sizeof(u16)))->adsr1 & 0xFFF0) | value;
            }
        }
    }
    work.accumulator = 1;
    for (work.counter = 0; work.counter < 2; work.counter++) {
        work.accumulator *= 13;
    }
}
