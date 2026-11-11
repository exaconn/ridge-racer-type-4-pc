#ifndef RR_EMBLEM_TIM_TYPES_H
#define RR_EMBLEM_TIM_TYPES_H
#include "rr/layout.h"
#include "rr/palette16.h"
enum {RR_EMBLEM_TIM_BYTES=0x840,RR_EMBLEM_TIM_WORDS=0x210,RR_CUSTOM_EMBLEM_COUNT=2};
/* File copies may be unaligned; word and palette users select an aligned image. */
typedef struct {u8 bytes[RR_EMBLEM_TIM_BYTES];} EmblemTim840;
typedef struct {
    u32 header[5];
    Palette16 palette;
    u32 imageHeader[3];
    u32 pixels[512];
} EmblemTimLayout840;
typedef union {
    EmblemTim840 file;
    u32 words[RR_EMBLEM_TIM_WORDS];
    EmblemTimLayout840 layout;
} EmblemTimImage840;
typedef union {
    EmblemTimImage840 images[RR_CUSTOM_EMBLEM_COUNT];
    u32 words[RR_CUSTOM_EMBLEM_COUNT*RR_EMBLEM_TIM_WORDS];
    u8 bytes[RR_CUSTOM_EMBLEM_COUNT*RR_EMBLEM_TIM_BYTES];
} CustomEmblemImages1080;
RR_ASSERT_SIZE(EmblemTim840,0x840);
RR_ASSERT_SIZE(EmblemTimImage840,0x840);
RR_ASSERT_SIZE(EmblemTimLayout840,0x840);
RR_ASSERT_SIZE(CustomEmblemImages1080,0x1080);
RR_ASSERT_OFFSET(EmblemTimLayout840,palette,0x14);
RR_ASSERT_OFFSET(EmblemTimLayout840,pixels,0x40);
#endif
