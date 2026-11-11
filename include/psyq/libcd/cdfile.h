#ifndef RR_PSYQ_LIBCD_CDFILE_H
#define RR_PSYQ_LIBCD_CDFILE_H

#include "types.h"
#include "rr/layout.h"

/* SDK CdlFILE: CdlLOC position (minute, second, sector, track bytes), size
 * and name; identical to the PsyZ layout (checked in sdk_abi.c). */
typedef struct {
    u8 pos[4];
    u32 size;
    char name[16];
} CdlFILE;
RR_ASSERT_SIZE(CdlFILE, 0x18);
RR_ASSERT_OFFSET(CdlFILE, size, 4);
RR_ASSERT_OFFSET(CdlFILE, name, 8);
_Static_assert(sizeof(CdlFILE) == 0x18, "CdlFILE size");

/* PsyZ: CdlFILE *CdSearchFile(CdlFILE *fp, char *name). */
CdlFILE *CdSearchFile(CdlFILE *file, char *name);

#endif /* RR_PSYQ_LIBCD_CDFILE_H */
