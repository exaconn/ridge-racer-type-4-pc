/* RODATA 0x80010014 0x14 */
#include "types.h"
#include "m2c_macros.h"

/* Read the two-sector R4 directory, then convert relative boundaries to entries.
 * The disc supplies count + 1 valid offsets; sector arithmetic must fit s32. */
#include "rr/cd.h"

typedef struct {RrCdLocation location; s32 size; u8 name[16];} SearchResult;

extern s32 func_8008DE84(SearchResult *, const char *);
extern s32 func_8008EEF4(s32, void *, s32);
extern s32 func_8008F090(s32, void *);
extern s32 func_8008C6A4(const RrCdLocation *);
extern void func_800963C4(s32);
extern void func_8001E84C(const char *, ...);
extern const char D_80010028[], D_80010048[], D_80010054[], D_80010068[];
extern u32 *D_800102F0;
extern RrCdFileEntry D_801094A8[];
extern int func_8008C05C();
extern s32 D_800ACC18;
extern int func_80090424();
extern int func_800904F4();
extern int D_8009DFA0, D_8009DFA4;
extern struct P { int w0, w4; short x0, y0; int c1; short x1, y1; int c2; short x2, y2; } D_80109400[];
extern struct P *D_80109438;
extern s32 D_8009DF90, D_8009DF94, D_8009DF9C;
extern s32 (*D_8009DFA8[])();
extern s32 D_8010945C, D_80109460, D_80109480, D_80109484, D_80109488;
extern RrCdLocation D_80109464;
extern u8 D_80109498;

s32 func_80012208(s32 arg0);
int func_80012210(int a0, int a1, int a2, int a3);
int func_8001223C(int a0, int a1);
void func_80012290(void);
void func_800123D4(void);
void func_80012408(void);
s32 func_80012578(void);
s32 func_800125C8(void);
s32 func_800125F8(void);
s32 func_80012640(void);
s32 func_80012684(void);
s32 func_800126F0(void);
s32 func_80012740(void);
s32 func_80012774(void);
s32 func_800127A0(void);
s32 func_800127E8(void);
void func_80012834(void);
s32 func_80012860(void);
s32 func_800128B8(void);
s32 func_8001292C(s32 a0, s32 a1);
s32 func_800129BC(s32 a0, s32 a1, s32 a2, s32 a3);
s32 func_80012A58(void);
void func_80012A68(int *a0);
void func_80012AB4(int *a0, int a1);

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

void func_800123D4(void) {
    func_8008C05C(1);
    func_80012290();
    D_800ACC18 = D_800102F0;
}

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
    D_80109438->x0 = (func_80090424(a) >> 7) + 0x50;
    D_80109438->y0 = (func_800904F4(D_8009DFA0) >> 7) + 0x80;
    D_80109438->x1 = (func_80090424(D_8009DFA0 + 0x555) >> 7) + 0x50;
    D_80109438->y1 = (func_800904F4(D_8009DFA0 + 0x555) >> 7) + 0x80;
    D_80109438->x2 = (func_80090424(D_8009DFA0 - 0x555) >> 7) + 0x50;
    D_80109438->y2 = (func_800904F4(D_8009DFA0 - 0x555) >> 7) + 0x80;
    w = 0x300000FF;
    m = 0xFFFFFF;
    g = (unsigned **)0x1F800000;
    h = 0xFF000000;
    D_80109438->c1 = 0xFF00;
    D_80109438->c2 = 0xFF0000;
    D_80109438->w4 = w;
    *((unsigned char *)D_80109438 + 3) = 6;
    D_80109438->w0 = (D_80109438->w0 & h) | (**(g + 1) & m);
    t = *(g + 1);
    *t = (*t & h) | ((unsigned)D_80109438 & m);
}

s32 func_80012578(void) {
    s32 var_v0;

    func_8008C3B0(1, 0, &D_80109498);
    var_v0 = D_80109498 & 0x10;
    if (var_v0 != 0) {
        D_8009DF90 = 5;
        var_v0 = 0;
    }
    return var_v0;
}

s32 func_800125C8(void) {
    func_8008C27C(0x13, 0);
    D_8009DF90 = 2;
    return 0;
}

s32 func_800125F8(void) {
    if (D_80109488 == 2) {
        func_80012290();
        D_8009DF90 = 3;
    } else {
        D_8009DF90 = 1;
    }
    return 0;
}

s32 func_80012640(void) {
    D_80109498 = 0x80;
    func_8008C27C(0xE, &D_80109498);
    D_8009DF90 = 4;
    D_8009DF94 = 0;
    return 0;
}

s32 func_80012684(void) {
    if (D_80109488 == 2) {
        if (++D_8009DF94 >= 4) {
            D_8009DF94 = 0;
            if (D_8009DF9C < 0) {
                D_8009DF90 = 0;
            } else {
                D_8009DF90 = 6;
            }
        }
    } else {
        D_8009DF90 = 3;
    }
    return 0;
}

s32 func_800126F0(void) {
    func_8008C3B0(1, 0, &D_80109498);
    if (!(D_80109498 & 0x10)) {
        D_8009DF90 = 1;
    }
    return 0;
}

s32 func_80012740(void) {
    func_8008C27C(2, &D_80109464);
    D_8009DF90 = 7;
    return 0;
}

s32 func_80012774(void) {
    if (D_80109488 == 2) {
        D_8009DF90 = 8;
    } else {
        D_8009DF90 = 0xB;
    }
    return 0;
}

s32 func_800127A0(void) {
    if (func_8008EEF4(D_80109460, D_80109480, 0x80)) {
        D_8009DF90 = 9;
    } else {
        D_8009DF90 = 0xB;
    }
    return 0;
}

s32 func_800127E8(void) {
    s32 r = func_8008F090(1, 0);
    if (r == 0) {
        D_8009DF90 = 0xA;
    } else if (r == -1) {
        D_8009DF90 = 0xB;
    }
    return 0;
}

void func_80012834(void) {
    D_8009DF9C = -1;
    D_8009DF90 = 0;
    D_80109484 = D_80109460 << 0xB;
}

s32 func_80012860(void)
{
    func_8008C3B0(1, 0, &D_80109498);
    if (D_80109498 & 0x10) {
        D_8009DF90 = 5;
        return 0;
    }
    D_8009DF90 = 6;
    return 0;
}

s32 func_800128B8(void) {
    if (D_8009DF90 > 0) {
        D_80109488 = func_8008C0D8(1, 0);
        if (D_80109488 != 0) {
            return D_8009DFA8[D_8009DF90]();
        }
    }
    return 0;
}

s32 func_8001292C(s32 a0, s32 a1) {
    if (D_8009DF90 != 0) return 0;
    D_80109480 = a1;
    D_8009DF9C = a0;
    D_8009DF90 = 6;
    D_8010945C = 0;
    D_80109464 = D_801094A8[a0].location;
    D_80109484 = 0;
    D_80109460 = D_801094A8[D_8009DF9C].sectors;
    return 1;
}

s32 func_800129BC(s32 a0, s32 a1, s32 a2, s32 a3) {
    if (D_8009DF90 != 0) return 0;
    D_80109480 = a3;
    D_8009DF9C = a0;
    D_8009DF90 = 6;
    D_8010945C = a1;
    func_8008C5A0(func_8008C6A4(&D_801094A8[a0]) + a1, D_80109464.c);
    D_80109460 = a2;
    D_80109484 = 0;
    return 1;
}

s32 func_80012A58(void) {
    return D_80109484;
}

void func_80012A68(int *a0)
{
    int n = *a0;
    int *p = a0 + 1;
    int t;
    if (n-- > 0) {
        int *q = a0 + 3;
        do {
            t = n--;
            *p += (int)a0;
            p += 3;
            *q += (int)a0;
            q += 3;
        } while (t > 0);
    }
}

void func_80012AB4(int *a0, int a1) {
    int n = a0[0];
    char **list = (char **)(a0 + 3);
    int x = (a1 << 6) + 0x180;
    int tp_x = (x & 0x3FF) >> 6;
    int clut_x = (x >> 4) & 0x3F;
    int u_add = a1 << 6;
    int u_mask = ~0xC0;
    int clut2 = a1 + 0x1028;
    int t, z;

    while (z = 0, ((t = n--) + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z) != 0) {
        char *p = *list++;
        unsigned int cmd;
        while (cmd = *(unsigned int *)p, p += 4, cmd != 0) {
            int count = cmd >> 16;
            switch (cmd & 0xFFFF) {
            case 0:
                p += count * 16;
                break;
            case 2:
                p += count * 24;
                break;
            case 1:
                while (count-- > 0) {
                    *(short *)(p + 0xE) = (*(short *)(p + 0xE) & ~0x1F) | tp_x;
                    *(short *)(p + 0xA) = (*(short *)(p + 0xA) & ~0x403C) | clut_x;
                    p += 0x18;
                }
                break;
            case 3:
                while (count-- > 0) {
                    if (*(short *)(p + 0x16) < 0) {
                        *(short *)(p + 0x16) = 0xA;
                        *(short *)(p + 0x10) = (*(short *)(p + 0x10) & u_mask) | u_add;
                        *(short *)(p + 0x14) = (*(short *)(p + 0x14) & u_mask) | u_add;
                        *(short *)(p + 0x18) = (*(short *)(p + 0x18) & u_mask) | u_add;
                        *(short *)(p + 0x1A) = (*(short *)(p + 0x1A) & u_mask) | u_add;
                        *(short *)(p + 0x12) = (*(short *)(p + 0x12) & ~0x403C) | clut2;
                        *(short *)(p + 0x1E) |= 0x78;
                    } else {
                        *(short *)(p + 0x16) = (*(short *)(p + 0x16) & ~0x1F) | tp_x;
                        *(short *)(p + 0x12) = (*(short *)(p + 0x12) & ~0x403C) | clut_x;
                        *(short *)(p + 0x1E) |= 0x78;
                    }
                    p += 0x20;
                }
                break;
            }
        }
    }
}
