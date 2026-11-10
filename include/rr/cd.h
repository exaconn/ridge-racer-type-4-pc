#ifndef RR_CD_H
#define RR_CD_H

#include "rr/layout.h"

/* Four-byte CD location copied as a block by the read state machine. */
typedef struct {
    u8 c[4];
} CdLocation;

/* CdFileTable directory: the second word is converted to bytes with << 11. */
typedef struct {
    CdLocation location;
    s32 sectors;
} CdFileEntry;

/* Per-command metadata and the current drive status byte.
 * The command's low byte must select an accessible metadata entry. */
extern u32 CD_com_info[];
/* Reset clears all four bytes; command paths read the low status byte. */
typedef union {
    u32 word;
    struct {
        u8 status,reserved[3];
    } bytes;
} CdDriveStatus;
extern CdDriveStatus CD_status;
extern s32 CD_status1;
extern u8 CD_pos[4];
extern u8 CD_mode, CD_com;
RR_ASSERT_SIZE(CdDriveStatus,4);
/* Raw command dispatch: zero means success. Control retries up to four times. */
#ifdef __mips__
s32 CD_cw(u8 command, const u8 *parameters, u8 *result, s32 mode);
#else
s32 CD_cw(s32 command, const u8 *parameters, u8 *result, s32 mode);
#endif
s32 CdControl(s32 command, const u8 *parameters, u8 *result);
/* Main dispatch uses mode1 and a null result buffer. Same four-attempt policy. */
s32 CdControlF(s32 command, const u8 *parameters);
/* Mode0 dispatch followed by completion; only completion result2 succeeds. */
s32 CdControlB(s32 command, const u8 *parameters, u8 *result);
s32 CD_sync(s32 mode, u8 *status);
s32 CdSync(s32 mode, u8 *status);

/* Command completion, raw sector transfer and disc-type probe. */
s32 CD_ready(s32 mode, u8 *status);
s32 CdReady(s32 mode, u8 *status);
s32 CD_getsector(u8 *buffer, u32 words);
s32 CD_getsector2(u8 *buffer, u32 words);
s32 CdGetSector(u8 *buffer, u32 words);
u8 *CdIntToPos(s32 sector, u8 *location);
s32 CdGetDiskType(void);
extern volatile u8 *CD_reg0, *CD_REG2, *CD_reg3;
extern volatile u32 *COM_DELAY, *CD_DELAY, *CD_dpcr;
extern volatile u32 *DMA3_BCR, *DMA3_CHCR;
/* The DMA address register holds a buffer address, not an integer object id. */
extern u8 * volatile *DMA3_MADR;

RR_ASSERT_SIZE(CdLocation, 4);
RR_ASSERT_SIZE(CdFileEntry, 8);
RR_ASSERT_OFFSET(CdFileEntry, sectors, 4);

#endif
