#ifndef RR_OVERLAY668_WAIT_H
#define RR_OVERLAY668_WAIT_H
#include "rr/overlay668_control.h"

/* Pointer cells for MDEC status and output DMA control. Access the hardware
 * words through volatile-qualified lvalues; the pointer cells are ordinary RAM. */
extern u32 *ovl668_mdec1_ctrl_ptr;
extern u32 *ovl668_dma1_chcr_ptr;
extern char ovl668_mdec_in_timeout_msg[];
extern char ovl668_mdec_out_timeout_msg[];
s32 ovl668_mdec_handle_timeout(const char *message);
#endif
