#ifndef RR_CD_H
#define RR_CD_H

#include "rr/layout.h"

/* Four-byte CD location copied as a block by the read state machine. */
typedef struct {
    u8 c[4];
} RrCdLocation;

/* D_801094A8 directory: the second word is converted to bytes with << 11. */
typedef struct {
    RrCdLocation location;
    s32 sectors;
} RrCdFileEntry;

/* Per-command metadata and the current drive status byte.
 * The command's low byte must select an accessible metadata entry. */
extern u32 D_800A6400[];
/* Reset clears all four bytes; command paths read the low status byte. */
typedef union {u32 word;struct {u8 status,reserved[3];} bytes;} RrCdDriveStatus;
extern RrCdDriveStatus D_800A6494;
RR_ASSERT_SIZE(RrCdDriveStatus,4);
/* Raw command dispatch: zero means success. Control retries up to four times. */
s32 CD_cw(s32 command, const u8 *parameters, u8 *result, s32 mode);
s32 CdControl(s32 command, const u8 *parameters, u8 *result);
/* Main dispatch uses mode1 and a null result buffer. Same four-attempt policy. */
s32 CdControlF(s32 command, const u8 *parameters);
/* Mode0 dispatch followed by completion; only completion result2 succeeds. */
s32 CdControlB(s32 command, const u8 *parameters, u8 *result);
s32 CD_sync(s32 mode, u8 *status);
s32 func_8008C0D8(s32 mode, u8 *status);

/* Command completion, raw sector transfer and disc-type probe. */
s32 CD_ready(s32 mode, u8 *status);
s32 func_8008C0F8(s32 mode, u8 *status);
s32 CD_getsector(u8 *buffer, u32 words);
s32 CD_getsector2(u8 *buffer, u32 words);
s32 func_8008C51C(u8 *buffer, u32 words);
u8 *CdIntToPos(s32 sector, u8 *location);
s32 func_8008F3B0(void);
extern volatile u8 *D_800A674C, *D_800A6758;
extern volatile u32 *D_800A675C, *D_800A6780, *D_800A6784;
extern volatile u32 *D_800A678C, *D_800A6790;
/* The DMA address register holds a buffer address, not an integer object id. */
extern u8 * volatile *D_800A6788;

RR_ASSERT_SIZE(RrCdLocation, 4);
RR_ASSERT_SIZE(RrCdFileEntry, 8);
RR_ASSERT_OFFSET(RrCdFileEntry, sectors, 4);

#endif
