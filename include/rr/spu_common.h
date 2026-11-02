#ifndef RR_SPU_COMMON_H
#define RR_SPU_COMMON_H
#include "rr/spu.h"

typedef struct {s16 left,right;} RrSpuCommonVolume4;
typedef struct {RrSpuCommonVolume4 volume;s32 reverb,mix;} RrSpuExternalAudio0C;
typedef struct {
    u32 mask;
    RrSpuCommonVolume4 master,mode,output;
    RrSpuExternalAudio0C cd,external;
} RrSpuCommonAttributes28;

/* A zero mask updates all attributes. Nonzero masks select individual fields;
 * sweep modes1..7 clamp master volume to0..127. The bank is the existing
 * complete512-byte storage owner. No bank access occurs for modifier-only or
 * unsupported nonzero masks. Attribute storage is separate from the bank. */
void func_80082674(RrSpuCommonAttributes28 *attributes);
RR_ASSERT_SIZE(RrSpuCommonVolume4,4);
RR_ASSERT_SIZE(RrSpuExternalAudio0C,0xC);
RR_ASSERT_SIZE(RrSpuCommonAttributes28,0x28);
RR_ASSERT_OFFSET(RrSpuCommonAttributes28,mode,8);
RR_ASSERT_OFFSET(RrSpuCommonAttributes28,cd,0x10);
RR_ASSERT_OFFSET(RrSpuCommonAttributes28,external,0x1C);
#endif
