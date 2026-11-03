#ifndef RR_CD_INIT_H
#define RR_CD_INIT_H
#include "rr/cd.h"
#include "rr/library.h"
#include "rr/diagnostics.h"
extern const char D_80011B08[],D_80011B14[];
/* First cell of the metadata pointer table; diagnostics serialize its address. */
extern u8 *D_800A6768;
extern s32 D_800A6498;
extern u8 D_800A64A4,D_800A64A5;
/* The response owner supplies at least the three reset bytes. */
extern u8 D_800A6764[];
extern volatile u8 *D_800A6754;
s32 func_800963DC(const char *message);
#ifndef RR_CD_READ8
#define RR_CD_READ8(port) (*(port))
#define RR_CD_WRITE8(port,value) (*(port)=(value))
#define RR_CD_WRITE32(port,value) (*(port)=(value))
#endif
s32 CD_init(void);
#endif
