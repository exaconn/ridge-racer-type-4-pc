#ifndef RR_OVERLAY668_DMA_H
#define RR_OVERLAY668_DMA_H
#include "rr/overlay668_wait.h"
#include "psyq/sdk_types.h"

/* The original driver programmed DMA channels 0/1 and the MDEC ports
 * directly. The host routes the same transfers through the PsyZ MDEC
 * device (third_party/psyz/psyz/src/psyz/libpress.c): MDEC_in submits a
 * command block (first word = MDEC command), MDEC_out reads decoded words,
 * the sync calls return 0 on completion or -1 on a failed transfer and
 * MDEC_status returns the MDEC status word. */
void MDEC_reset(int mode);
void MDEC_in(void *buf, int size);
void MDEC_out(void *buf, int size);
int MDEC_in_sync(void);
int MDEC_out_sync(void);
PsyqULong MDEC_status(void); /* PsyZ: u_long MDEC_status(void). */
#endif
