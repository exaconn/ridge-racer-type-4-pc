#ifndef RR_PSYQ_LIBCD_CDFILE_H
#define RR_PSYQ_LIBCD_CDFILE_H

#include "types.h"
#include "rr/layout.h"

typedef struct {
    u8 pos[4];
    u32 size;
    char name[16];
} CdlFILE;

typedef struct {
    int a;
    int b;
    int c;
    char name[32];
} CdDirPath2C;

RR_ASSERT_SIZE(CdlFILE, 0x18);
RR_ASSERT_OFFSET(CdlFILE, size, 4);
RR_ASSERT_OFFSET(CdlFILE, name, 8);
RR_ASSERT_SIZE(CdDirPath2C, 0x2C);
RR_ASSERT_OFFSET(CdDirPath2C, name, 0x0C);

extern int DS_cachedDir;
extern CdlFILE DsDirFileCache[];

CdlFILE *CdSearchFile(CdlFILE *fp, char *name);
int DS_newmedia(void);
s32 DS_searchdir(s32 key, const char *name);

#endif /* RR_PSYQ_LIBCD_CDFILE_H */
