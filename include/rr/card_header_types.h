#ifndef RR_CARD_HEADER_TYPES_H
#define RR_CARD_HEADER_TYPES_H

#include "rr/layout.h"

typedef struct {u8 bytes[0x80];} CardHeaderBlock128;
typedef struct {u8 bytes[0x4000];} CardFileImage4000;
RR_ASSERT_SIZE(CardFileImage4000,0x4000);

/* The first 128 bytes contain the card title and 16-colour palette. Animated
 * icons occupy three 128-byte frames. The static variant stores one frame and
 * three copies of its extra block, reaching the application data at 0x280. */
typedef struct {
    u8 magic00[2], iconMode02, blockCount03;
    char title04[64];
    u8 reserved44[12];
    u8 flags50[2];
    char applicationTag52[14];
    u16 palette60[16];
    union {
        CardHeaderBlock128 frames[4];
        struct {CardHeaderBlock128 image, extra[3];} single;
    } payload80;
} CardFileHeader280;

RR_ASSERT_SIZE(CardHeaderBlock128,0x80);
RR_ASSERT_SIZE(CardFileHeader280,0x280);
RR_ASSERT_OFFSET(CardFileHeader280,title04,4);
RR_ASSERT_OFFSET(CardFileHeader280,flags50,0x50);
RR_ASSERT_OFFSET(CardFileHeader280,applicationTag52,0x52);
RR_ASSERT_OFFSET(CardFileHeader280,palette60,0x60);
RR_ASSERT_OFFSET(CardFileHeader280,payload80,0x80);

#endif
