#ifndef RR_OVERLAY668_DMA_H
#define RR_OVERLAY668_DMA_H
#include "rr/overlay668_wait.h"

extern u32 *ovl668_dpcr_ptr; /* DMA priority/control */
extern u32 *ovl668_dma0_bcr_ptr; /* input block count */
extern u32 *ovl668_dma0_chcr_ptr; /* input channel control */
extern u32 *ovl668_dma1_bcr_ptr; /* output block count */
extern u32 *ovl668_mdec0_cmd_ptr; /* MDEC command */
/* The address registers hold pointers; their width is the target pointer width. */
extern const void *volatile *ovl668_dma0_madr_ptr;
extern void *volatile *ovl668_dma1_madr_ptr;
/* Native tests override accesses while PAL retains volatile hardware lvalues. */
#ifndef RR_STR_READ32
#define RR_STR_READ32(port) (*(volatile u32 *)(port))
#endif
#ifndef RR_STR_WRITE32
#define RR_STR_WRITE32(port,value) (*(volatile u32 *)(port)=(value))
#endif
#endif
