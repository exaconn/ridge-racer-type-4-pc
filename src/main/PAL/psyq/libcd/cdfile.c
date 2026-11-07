/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 2.7.2 */
/* CC1_FLAGS: -fsigned-char -mhard-float */
#include "rr/library.h"
#include "psyq/libcd/dsread.h"
#include "psyq/libcd/dsfile.h"
#include "psyq/libcd/cdfile.h"
#include "psyq/libcd/dsfile.h"
#include "rr/audio_cd_controller.h"

typedef struct {
    unsigned char pos[4];
    unsigned int size;
    char name[16];
} CdlFILE;

typedef struct {
    int x;
} __attribute__((packed)) U4;

struct DirPath {
    int a;
    int b;
    int c;
    char name[32];
};

extern int D_800A67A4;
extern int CD_nopen;
extern int CD_debug;
extern CdlFILE D_80110350[];
extern const char D_80011B38[];
extern const char D_80011B54[];
extern const char D_80011B6C[];
extern const char D_80011B88[];
extern const char D_80011BA8[];
extern const char D_80011BB4[];
extern int func_80096414();
extern struct DirPath D_80110950[];
extern const char D_80011BC4[];
extern const char D_80011BF0[];
extern const char D_80011BF8[];
extern const char D_80011C28[];
extern const char D_80011C4C[];
extern const char D_80011C6C[];
extern const char D_80011C80[];
extern void func_80096444(char *, void *, int);
extern char D_8011095C[];
extern s32 func_80096404(const char *, const char *);

CdlFILE *CdSearchFile(CdlFILE *fp, char *name);
int func_8008E15C(int a, int b);
int DS_newmedia(void);
s32 DS_searchdir(s32 key, const char *name);

CdlFILE *CdSearchFile(CdlFILE *fp, char *name) {
    char buf[16];
    char *p;
    char *d;
    int i;
    int dir;
    /* ASM_BARRIER: use hard reg $0 so jump2 cross-jumping does not merge this return with .L8008E130 */
    register CdlFILE *zero asm("$0");

    if (D_800A67A4 != CD_nopen) {
        if (DS_newmedia() == 0)
            return 0;
        D_800A67A4 = CD_nopen;
    }
    if (*name != '\\') {
        return zero;
    err:
        buf[0] = 0;
    } else {
        buf[0] = 0;
        dir = 1;
        p = name;
        i = 0;
        do {
            for (d = buf; *p != '\\'; d++) {
                if (*p == 0)
                    goto done;
                *d = *p++;
            }
            if (*p == 0)
                goto done;
            p++;
            *d = 0;
            if ((dir = DS_searchdir(dir, buf)) == -1)
                goto err;
            i++;
        } while (i < 8);
    }
done:
    if (i >= 8) {
        if (CD_debug > 0)
            func_8001E84C(D_80011B38, name, i);
        return 0;
    }
    if (buf[0] == 0) {
        if (CD_debug > 0)
            func_8001E84C(D_80011B54, name);
        return 0;
    }
    *d = 0;
    if (DS_cachefile(dir) == 0) {
        if (CD_debug > 0)
            func_8001E84C(D_80011B6C);
        return 0;
    }
    if (CD_debug >= 2)
        func_8001E84C(D_80011B88, buf);
    for (i = 0; i < 64; i++) {
        if (D_80110350[i].name[0] == 0)
            break;
        if (func_8008E15C(D_80110350[i].name, buf)) {
            if (CD_debug >= 2)
                func_8001E84C(D_80011BA8, buf);
            *fp = D_80110350[i];
            return &D_80110350[i];
        }
    }
    if (CD_debug > 0)
        func_8001E84C(D_80011BB4, buf);
    return 0;
}

int func_8008E15C(int a, int b) { return func_80096414(a, b, 12) == 0; }

/* legacy typing of shared globals in this part of the unit */
#define func_80096414 ((int (*)(void *, const char *, int))func_80096414)

int DS_newmedia(void) {
    int sp18;
    unsigned char *p;
    int i;
    unsigned char *p8c;
    int pad;
    int dummy;

    if (func_8008E780(1, 0x10, D_80111F50) != 1) {
        if (CD_debug > 0) {
            func_8001E84C(D_80011BC4);
        }
        return 0;
    }
    if (func_80096414(D_80111F50 + 1, D_80011BF0, 5) != 0) {
        if (CD_debug > 0) {
            func_8001E84C(D_80011BF8);
        }
        return 0;
    }
    p8c = D_80111F50;
    /* ASM_BARRIER: prevent cse2 from folding s0+0x8C back to symbol+0x8C */
    __asm__ volatile("" : "=r"(p8c) : "0"(p8c));
    *(U4 *)&sp18 = *(U4 *)(p8c + 0x8C);
    if (func_8008E780(1, sp18, p8c) != 1) {
        if (CD_debug > 0) {
            func_8001E84C(D_80011C28, sp18);
        }
        return 0;
    }
    p = p8c;
    if (CD_debug >= 2) {
        func_8001E84C(D_80011C4C);
    }
    i = 0;
    while (p < p8c + 0x800) {
        if (p[0] == 0) {
            break;
        }
        *(U4 *)&D_80110950[i].c = *(U4 *)(p + 2);
        D_80110950[i].b = p[6];
        D_80110950[i].a = i + 1;
        func_80096444(D_80110950[i].name, p + 8, p[0]);
        dummy = (p < p8c + 0x800);
        D_80110950[i].name[p[0]] = 0;
        pad = (p[0] & 1) + 8;
        p += p[0] + pad;
        if (CD_debug >= 2) {
            func_8001E84C(
                D_80011C6C,
                D_80110950[i].c,
                D_80110950[i].a,
                D_80110950[i].b,
                D_80110950[i].name
            );
        }
        i++;
        if (i >= 0x80) {
            break;
        }
    }
    if (i < 0x80) {
        D_80110950[i].b = 0;
    }
    D_800A67A0 = 0;
    if (CD_debug >= 2) {
        func_8001E84C(D_80011C80, i);
    }
    return 1;
}

#undef func_80096414

s32 DS_searchdir(s32 key, const char *name) {
    s32 i = 0;
    register char *entry_name;
    /* ASM_BARRIER: Retain the original table-offset register across comparisons. */
    register s32 offset __asm__("$17");
    /* ASM_BARRIER: Keep independently initialized scan count and byte offset without folding their zeros. */
    __asm__("" : "=r"(i) : "0"(i) : "$18");
    entry_name = D_8011095C;
    offset = 0;
    do {
        register u8 *page;
        s32 entry_key;
        /* PAL mapping: page + offset + 0x954 addresses D_80110954[i]. */
        page = (u8 *)0x80110000U;
        /* ASM_BARRIER: Expose the PAL page without folding its table offset into a macro load. */
        __asm__("" : "=r"(page) : "0"(page));
        page += offset;
        entry_key = ((RrLibNamedEntry *)(page + 0x954))->key;
        if (entry_key == 0) break;
        if (entry_key == key) {
            if (func_80096404(name, entry_name) == 0) return i + 1;
        }
        entry_name += 0x2C;
        offset += 0x2C;
        i++;
    } while (i < 128);
    return -1;
}
