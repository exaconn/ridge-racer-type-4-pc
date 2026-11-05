/* RODATA 0x80010014 0x14 */
#include "types.h"
#include "scratchpad.h"
#include "m2c_macros.h"

/* Read the two-sector R4 directory, then convert relative boundaries to entries.
 * The disc supplies count + 1 valid offsets; sector arithmetic must fit s32. */
#include "rr/cd.h"
#include "psyq/libcd/dsfinish.h"
#include "psyq/libcd/s_1.h"
#include "game/data/cdfile1.h"
#include "rr/course_choices.h"

typedef struct {RrCdLocation location; s32 size; u8 name[16];} SearchResult;

extern s32 func_8008DE84(SearchResult *, const char *);
extern s32 func_8008EEF4(s32, void *, s32);
extern s32 CdPosToInt(const RrCdLocation *);
extern void func_800963C4(s32);
extern void func_8001E84C(const char *, ...);
extern const char D_80010028[];
extern const char D_80010048[];
extern const char D_80010054[];
extern const char D_80010068[];
extern u32 *D_800102F0;
extern RrCdFileEntry D_801094A8[];
extern int rsin();
extern int rcos();
extern int D_8009DFA0;
extern int D_8009DFA4;
extern struct P { int w0, w4; short x0, y0; int c1; short x1, y1; int c2; short x2, y2; } D_80109400[];
extern struct P *D_80109438;

int func_80012210(int a0, int a1, int a2, int a3);
int func_8001223C(int a0, int a1);
void func_80012408(void);

s32 func_80012208(s32 arg0) {
    return arg0 * 2;
}

int func_80012210(int a0, int a1, int a2, int a3) {
    int v = ((a1 * 4 + a0) * 0x14) + a2;
    if (a3 == 0) return v + 0x12;
    return v + 0x152;
}

int func_8001223C(int a0, int a1) {
    switch (a0) {
    case 1: return a1 + 0x292;
    case 2: return a1 + 0x2D9;
    case 3: return a1 + 0x384;
    case 4: return a1 + 0x397;
    case 0: return a1 + 0x3A0;
    }
}

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
        CdControl(2,(const u8 *)&file.location,0);
        func_8008EEF4(sectors,D_800102F0,0x80);
        do { status=func_8008F090(1,0); } while(status>0);
    } while(status);
    func_8001E84C((const char *)(message_page+0x68),sectors);
    base=CdPosToInt(&file.location);
    directory=D_800102F0;
    i=0;
    count=*directory++;
    sector=base+*directory++;
    entry=D_801094A8;
    while(i<count) {
        CdIntToPos(sector,entry->location.c);
        next=base+*directory++;
        entry->sectors=next-sector;
        sector=next;
        i++;
        entry++;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800102F0 (*(s32 (*))&D_800102F0)

void func_800123D4(void) {
    func_8008C05C(1);
    func_80012290();
    D_800ACC18 = D_800102F0;
}

#undef D_800102F0

void func_80012408(void)
{
    int a, v, w;
    unsigned *t;
    unsigned **g;
    register unsigned m asm("$5");
    unsigned h;
    a = D_8009DFA0 + 0x10;
    v = D_8009DFA4 ^ 1;
    D_8009DFA4 = v;
    D_8009DFA0 = a;
    D_80109438 = &D_80109400[v];
    D_80109438->x0 = (rsin(a) >> 7) + 0x50;
    D_80109438->y0 = (rcos(D_8009DFA0) >> 7) + 0x80;
    D_80109438->x1 = (rsin(D_8009DFA0 + 0x555) >> 7) + 0x50;
    D_80109438->y1 = (rcos(D_8009DFA0 + 0x555) >> 7) + 0x80;
    D_80109438->x2 = (rsin(D_8009DFA0 - 0x555) >> 7) + 0x50;
    D_80109438->y2 = (rcos(D_8009DFA0 - 0x555) >> 7) + 0x80;
    w = 0x300000FF;
    m = 0xFFFFFF;
    g = (unsigned **)SCRATCHPAD;
    h = 0xFF000000;
    D_80109438->c1 = 0xFF00;
    D_80109438->c2 = 0xFF0000;
    D_80109438->w4 = w;
    *((unsigned char *)D_80109438 + 3) = 6;
    D_80109438->w0 = (D_80109438->w0 & h) | (**(g + 1) & m);
    t = *(g + 1);
    *t = (*t & h) | ((unsigned)D_80109438 & m);
}
