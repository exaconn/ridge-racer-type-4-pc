#ifndef RR_CD_POLL_H
#define RR_CD_POLL_H
#include "rr/cd.h"
#include "rr/timing.h"
#include "rr/diagnostics.h"
/* Names are indexed by command0..27 and completion code0..6. The timeout
 * diagnostic requires the two status bytes to select those seven names. */
extern const char *D_800A64AC[28],*D_800A652C[7];
extern u8 D_800A64A5,D_800A6764[];
extern s32 D_80110330;
extern u32 D_80110334;
extern const char *D_80110338;
extern const char D_80011A14[],D_80011A24[],D_80011B20[];
s32 func_800963DC(const char *message);
void CD_flush(void);
/* Mode0 waits; a nonzero mode polls once. Returns-1 on timeout,0 when idle,
 * and1 when a nonblocking poll finds DMA busy. Counters wrap in32 bits. */
s32 CD_datasync(s32 mode);
#endif
