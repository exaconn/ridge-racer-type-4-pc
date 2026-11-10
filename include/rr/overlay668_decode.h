#ifndef RR_OVERLAY668_DECODE_H
#define RR_OVERLAY668_DECODE_H
#include "rr/library.h"

/* Stream state prefix: two display buffers followed by the queue header.
 * Pointer fields retain their native width outside the PAL build. */
typedef struct {
    u8 reserved00[0x10];
    void *commands10[2];
    void *buffers18[2];
    LibQueueRecord32 *queue20;
} StrStreamPrefix;
extern StrStreamPrefix *D_800AC708;
void ovl668_func_80114EE8(LibInterruptCallback callback);
RR_ASSERT_OFFSET(StrStreamPrefix, commands10, 0x10);
RR_ASSERT_OFFSET(StrStreamPrefix, buffers18, 0x18);
RR_ASSERT_OFFSET(StrStreamPrefix, queue20, 0x20);
RR_ASSERT_SIZE(StrStreamPrefix, 0x24);

typedef union {u32 word;u16 halves[2];} StrCommandHeader;
typedef struct {
    StrCommandHeader header;
    u32 luma[16],chroma[16];
} StrQuantizationCommand;
typedef struct {StrCommandHeader header;u32 matrix[32];} StrMatrixCommand;
extern StrQuantizationCommand ovl668_D_80116D10;
extern StrMatrixCommand ovl668_D_80116D94;

/* Reset the resident callback driver for mode0, then dispatch the device mode. */
void ovl668_func_80116298(s32 mode);
void ovl668_func_80116558(s32 mode);

/* Forward copies of16,16,32 words. Output has room for64 words; when it
 * overlaps table storage, later reads observe earlier writes. Returns output. */
u32 *ovl668_func_801162CC(u32 *output);
/* Publish the first32 input words to quantization storage, then submit both
 * stored commands. Matrix storage stays live through the first submission.
 * Input may overlap the tables; returns the original full input pointer. */
u32 *ovl668_func_80116358(u32 *input);
void ovl668_func_80116648(const void *command,u32 word_count);
/* The first halfword of the little-endian command header. */
u32 ovl668_func_801163F0(const StrCommandHeader *header);
/* Exchange interrupt callback channel1 and return its previous full pointer. */
LibInterruptCallback ovl668_func_80116534(LibInterruptCallback callback);

RR_ASSERT_SIZE(StrCommandHeader,4);
RR_ASSERT_SIZE(StrQuantizationCommand,132);
RR_ASSERT_OFFSET(StrQuantizationCommand,luma,4);
RR_ASSERT_OFFSET(StrQuantizationCommand,chroma,68);
RR_ASSERT_SIZE(StrMatrixCommand,132);
RR_ASSERT_OFFSET(StrMatrixCommand,matrix,4);
void ovl668_func_80114DF4(void);
#endif
