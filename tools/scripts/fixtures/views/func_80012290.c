/* Read the two-sector R4 directory, then convert relative boundaries to entries.
 * The disc supplies count + 1 valid offsets; sector arithmetic must fit s32. */
#include "rr/cd.h"

/* legacy typing of shared globals in this part of the unit */
#define CdDirectoryBuffer (*(s32 (*))&CdDirectoryBuffer)

#undef CdDirectoryBuffer

typedef struct {RrCdLocation location; s32 size; u8 name[16];} SearchResult;

extern s32 CdSearchFile(SearchResult *, const char *);

extern s32 CdRead(s32, void *, s32);

extern s32 CdReadSync(s32, void *);

extern s32 CdPosToInt(const RrCdLocation *);

extern void func_800963C4(s32);

extern void func_8001E84C(const char *, ...);

extern const char D_80010028[];

extern const char D_80010048[];

extern const char D_80010054[];

extern u32 *CdDirectoryBuffer;

extern RrCdFileEntry CdFileTable[];

void LoadR4Directory(void)
{
    SearchResult file;
    s32 status, found;
    /* ASM_BARRIER: retain the two-sector transfer count across CD calls. */
    register s32 sectors __asm__("$16");
    /* ASM_BARRIER: preserve the PAL diagnostic page across retries. */
    register u8 *message_page __asm__("$18");
    u32 count;
    /* ASM_BARRIER: retain the directory index independently of its cursor. */
    register u32 i __asm__("$17");
    s32 base, sector, next;
    u32 *directory;
    RrCdFileEntry *entry;
    func_8001E84C(D_80010028,D_80010048,CdDirectoryBuffer);
    found=CdSearchFile(&file,D_80010048);
    message_page=(u8 *)0x80010000U;
    if (!found) {
        func_8001E84C(D_80010054,D_80010048);
        func_800963C4(1);
    }
    sectors=2;
    /* ASM_BARRIER: keep the saved count and address opaque to constant folding. */
    __asm__("" : "+r"(sectors), "+r"(message_page));
    do {
        CdControl(2,(const u8 *)&file.location,0);
        CdRead(sectors,CdDirectoryBuffer,0x80);
        do { status=CdReadSync(1,0); } while(status>0);
    } while(status);
    func_8001E84C((const char *)(message_page+0x68),sectors);
    base=CdPosToInt(&file.location);
    directory=CdDirectoryBuffer;
    i=0;
    count=*directory++;
    sector=base+*directory++;
    entry=CdFileTable;
    while(i<count) {
        CdIntToPos(sector,entry->location.c);
        next=base+*directory++;
        entry->sectors=next-sector;
        sector=next;
        i++;
        entry++;
    }
}
