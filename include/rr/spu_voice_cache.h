#ifndef RR_SPU_VOICE_CACHE_H
#define RR_SPU_VOICE_CACHE_H
#include "rr/spu.h"
#include "rr/channels.h"

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

RR_ASSERT_OFFSET(SpuCachedVolumeEntry, program06, 6);
RR_ASSERT_OFFSET(SpuCachedVolumeEntry, tone08, 8);
RR_ASSERT_OFFSET(SpuCachedVolumeEntry, tone0A, 0xA);
RR_ASSERT_OFFSET(ChannelControlRow36, value06, 6);
#endif
