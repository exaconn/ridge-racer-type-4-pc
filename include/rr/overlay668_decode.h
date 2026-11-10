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
extern StrStreamPrefix *ActiveStrStreamPtr;
void ovl668_setup_stream_ring(LibInterruptCallback callback);
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
extern StrQuantizationCommand ovl668_mdec_quant_cmd;
extern StrMatrixCommand ovl668_mdec_scale_cmd;

/* Reset the resident callback driver for mode0, then dispatch the device mode. */
void ovl668_mdec_reset(s32 mode);
void ovl668_mdec_reset_hw(s32 mode);

/* Forward copies of16,16,32 words. Output has room for64 words; when it
 * overlaps table storage, later reads observe earlier writes. Returns output. */
u32 *ovl668_mdec_get_env(u32 *output);
/* Publish the first32 input words to quantization storage, then submit both
 * stored commands. Matrix storage stays live through the first submission.
 * Input may overlap the tables; returns the original full input pointer. */
u32 *ovl668_mdec_put_env(u32 *input);
void ovl668_mdec_start_in_dma(const void *command,u32 word_count);
/* The first halfword of the little-endian command header. */
u32 ovl668_mdec_buf_size(const StrCommandHeader *header);
/* Exchange interrupt callback channel1 and return its previous full pointer. */
LibInterruptCallback ovl668_mdec_out_callback(LibInterruptCallback callback);

RR_ASSERT_SIZE(StrCommandHeader,4);
RR_ASSERT_SIZE(StrQuantizationCommand,132);
RR_ASSERT_OFFSET(StrQuantizationCommand,luma,4);
RR_ASSERT_OFFSET(StrQuantizationCommand,chroma,68);
RR_ASSERT_SIZE(StrMatrixCommand,132);
RR_ASSERT_OFFSET(StrMatrixCommand,matrix,4);
void ovl668_stop_stream_playback(void);
#endif
