/* Read the two-sector R4 directory, then convert relative boundaries to entries.
 * The disc supplies count + 1 valid offsets; sector arithmetic must fit s32. */
#include "rr/cd.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800102F0 (*(s32 (*))&D_800102F0)

#undef D_800102F0

typedef struct {RrCdLocation location; s32 size; u8 name[16];} SearchResult;

extern s32 func_8008DE84(SearchResult *, const char *);

extern s32 func_8008EEF4(s32, void *, s32);

extern s32 func_8008F090(s32, void *);

extern s32 func_8008C6A4(const RrCdLocation *);

extern void func_800963C4(s32);

extern void func_8001E84C(const char *, ...);

extern const char D_80010028[];

extern const char D_80010048[];

extern const char D_80010054[];

extern u32 *D_800102F0;

extern RrCdFileEntry D_801094A8[];

void func_80012290(void)
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
    func_8001E84C(D_80010028,D_80010048,D_800102F0);
    found=func_8008DE84(&file,D_80010048);
    message_page=(u8 *)0x80010000U;
    if (!found) {
        func_8001E84C(D_80010054,D_80010048);
        func_800963C4(1);
    }
    sectors=2;
    /* ASM_BARRIER: keep the saved count and address opaque to constant folding. */
    __asm__("" : "+r"(sectors), "+r"(message_page));
    do {
        func_8008C140(2,(const u8 *)&file.location,0);
        func_8008EEF4(sectors,D_800102F0,0x80);
        do { status=func_8008F090(1,0); } while(status>0);
    } while(status);
    func_8001E84C((const char *)(message_page+0x68),sectors);
    base=func_8008C6A4(&file.location);
    directory=D_800102F0;
    i=0;
    count=*directory++;
    sector=base+*directory++;
    entry=D_801094A8;
    while(i<count) {
        func_8008C5A0(sector,entry->location.c);
        next=base+*directory++;
        entry->sectors=next-sector;
        sector=next;
        i++;
        entry++;
    }
}
