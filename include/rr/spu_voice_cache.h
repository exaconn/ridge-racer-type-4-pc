#ifndef RR_SPU_VOICE_CACHE_H
#define RR_SPU_VOICE_CACHE_H
#include "rr/spu.h"
#include "rr/channels.h"

/* Partial 32-byte tone row in the full-width selected tone allocation. */
typedef struct {
    u8 pad00[0x10];
    u16 value10, value12;
    u8 pad14[0xC];
} SpuTone32;

extern u32 _svm_envx_hist[16];
extern u16 _svm_damper;
/* Program/voice halfwords at 0x80110116/118; the byte selection starts at
 * 0x80110107 and its voice halfword is aligned at byte offset 0x11. */
extern s16 D_80110116[];
extern s8 D_80110107[];

/* Selected voice is 0..23, established by the selector writers. The selected
 * program/tone allocations must cover the signed program and signed byte
 * bank/program indices. Tone source may overlap the software cache: value12
 * is loaded after publishing value10. All accessed halfwords are aligned. */
void _SsVmDoAllocate(void);

RR_ASSERT_SIZE(SpuTone32, 0x20);
RR_ASSERT_OFFSET(SpuTone32, value10, 0x10);
RR_ASSERT_OFFSET(SpuTone32, value12, 0x12);
RR_ASSERT_OFFSET(SpuProgram, value0C, 0xC);
RR_ASSERT_OFFSET(SpuProgram, value0E, 0xE);
RR_ASSERT_OFFSET(SpuCachedVolumeEntry, program06, 6);
RR_ASSERT_OFFSET(SpuCachedVolumeEntry, tone08, 8);
RR_ASSERT_OFFSET(SpuCachedVolumeEntry, tone0A, 0xA);
RR_ASSERT_OFFSET(ChannelControlRow36, value06, 6);
#endif
