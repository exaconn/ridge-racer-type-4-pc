#ifndef RR_OVERLAY668_CONTROL_H
#define RR_OVERLAY668_CONTROL_H
#include "rr/overlay668_decode.h"

/* Flags bit0 selects the inverse of command bit27; bit1 selects bit25.
 * Submit the updated command using its first halfword as the word count. */
void ovl668_mdec_in(StrCommandHeader *header,u32 flags);
void ovl668_mdec_out(void *output,u32 words);
/* Mode0 forwards the blocking helper result. Other modes query status bits
 * 29 and24 respectively. */
s32 ovl668_mdec_in_sync(s32 mode);
s32 ovl668_mdec_out_sync(s32 mode);
LibInterruptCallback ovl668_mdec_in_callback(LibInterruptCallback callback);

void ovl668_mdec_start_out_dma(void *output,u32 words);
s32 ovl668_mdec_wait_in_dma(void);
s32 ovl668_mdec_wait_out_dma(void);
u32 ovl668_mdec_read_status(void);
#endif
