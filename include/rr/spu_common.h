#ifndef RR_SPU_COMMON_H
#define RR_SPU_COMMON_H
#include "rr/spu.h"

typedef struct {s16 left,right;} SpuCommonVolume4;
typedef struct {SpuCommonVolume4 volume;s32 reverb,mix;} SpuExternalAudio0C;
typedef struct {
    u32 mask;
    SpuCommonVolume4 master,mode,output;
    SpuExternalAudio0C cd,external;
} SpuCommonAttributes28;

/* A zero mask updates all attributes. Nonzero masks select individual fields;
 * sweep modes1..7 clamp master volume to0..127. The bank is the existing
 * complete512-byte storage owner. No bank access occurs for modifier-only or
 * unsupported nonzero masks. Attribute storage is separate from the bank. */
void SpuSetCommonAttr(SpuCommonAttributes28 *attributes);
RR_ASSERT_SIZE(SpuCommonVolume4,4);
RR_ASSERT_SIZE(SpuExternalAudio0C,0xC);
RR_ASSERT_SIZE(SpuCommonAttributes28,0x28);
RR_ASSERT_OFFSET(SpuCommonAttributes28,mode,8);
RR_ASSERT_OFFSET(SpuCommonAttributes28,cd,0x10);
RR_ASSERT_OFFSET(SpuCommonAttributes28,external,0x1C);
#endif
