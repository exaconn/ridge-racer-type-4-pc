/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"

typedef struct {
    int x;
} __attribute__((packed)) U4;

struct DirPath {
    int a;
    int b;
    int c;
    char name[32];
};

typedef struct {
    unsigned char pos[4];
    int size;
    char name[16];
} CdlFILE;

extern int func_80096414();
extern int D_800A67A0;
extern int D_800A6490;
extern struct DirPath D_80110950[];
extern unsigned char D_80111F50[];
extern const char D_80011BC4[];
extern const char D_80011BF0[];
extern const char D_80011BF8[];
extern const char D_80011C28[];
extern const char D_80011C4C[];
extern const char D_80011C6C[];
extern const char D_80011C80[];
extern int func_8008E780(int, int, void *);
extern int func_8001E84C();
extern void func_80096444(char *, void *, int);
extern char D_8011095C[];
extern s32 func_80096404(const char *, const char *);
extern struct DirPath D_8011092C[];
extern CdlFILE D_80110350[];
extern const char D_80011CA4[];
extern const char D_80011CC4[];
extern unsigned short D_80011CE0;
extern short D_80011CE4;
extern char D_80011CE6;
extern const char D_80011CE8[];
extern const char D_80011D04[];
extern void func_8008C5A0(int, void *);

int func_8008E15C(int a, int b);
int func_8008E17C(void);
s32 func_8008E440(s32 key, const char *name);
int func_8008E4E4(int dir);

int func_8008E15C(int a, int b) { return func_80096414(a, b, 12) == 0; }

int func_8008E17C(void) {
    int sp18;
    unsigned char *p;
    int i;
    unsigned char *p8c;
    int pad;
    int dummy;

    if (func_8008E780(1, 0x10, D_80111F50) != 1) {
        if (D_800A6490 > 0) {
            func_8001E84C(D_80011BC4);
        }
        return 0;
    }
    if (func_80096414(D_80111F50 + 1, D_80011BF0, 5) != 0) {
        if (D_800A6490 > 0) {
            func_8001E84C(D_80011BF8);
        }
        return 0;
    }
    p8c = D_80111F50;
    /* ASM_BARRIER: prevent cse2 from folding s0+0x8C back to symbol+0x8C */
    __asm__ volatile("" : "=r"(p8c) : "0"(p8c));
    *(U4 *)&sp18 = *(U4 *)(p8c + 0x8C);
    if (func_8008E780(1, sp18, p8c) != 1) {
        if (D_800A6490 > 0) {
            func_8001E84C(D_80011C28, sp18);
        }
        return 0;
    }
    p = p8c;
    if (D_800A6490 >= 2) {
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
        if (D_800A6490 >= 2) {
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
    if (D_800A6490 >= 2) {
        func_8001E84C(D_80011C80, i);
    }
    return 1;
}

s32 func_8008E440(s32 key, const char *name) {
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

int func_8008E4E4(int dir) {
    int sp18;
    unsigned char *p;
    int i;

    if (dir == D_800A67A0) {
        return 1;
    }
    if (func_8008E780(1, D_8011092C[dir].a, D_80111F50) != 1) {
        if (D_800A6490 > 0) {
            func_8001E84C(D_80011CA4);
            return -1;
        }
        return -1;
    }
    p = D_80111F50;
    if (D_800A6490 >= 2) {
        func_8001E84C(D_80011CC4);
    }
    i = 0;
    while (p < D_80111F50 + 0x800) {
        if (p[0] == 0) {
            break;
        }
        *(U4 *)&sp18 = *(U4 *)(p + 2);
        func_8008C5A0(sp18, D_80110350[i].pos);
        *(U4 *)&D_80110350[i].size = *(U4 *)(p + 10);
        switch (i) {
        case 0:
            __builtin_memcpy(D_80110350[0].name, &D_80011CE0, 2);
            break;
        case 1:
            __builtin_memcpy(D_80110350[1].name, &D_80011CE4, 3);
            break;
        default:
            func_80096444(D_80110350[i].name, p + 0x21, p[0x20]);
            D_80110350[i].name[p[0x20]] = 0;
            break;
        }
        if (D_800A6490 >= 2) {
            func_8001E84C(
                D_80011CE8,
                D_80110350[i].pos[0],
                D_80110350[i].pos[1],
                D_80110350[i].pos[2],
                D_80110350[i].size,
                D_80110350[i].name
            );
        }
        p += p[0];
        i++;
        if (i >= 0x40) {
            break;
        }
    }
    D_800A67A0 = dir;
    if (i < 0x40) {
        D_80110350[i].name[0] = 0;
    }
    if (D_800A6490 >= 2) {
        func_8001E84C(D_80011D04, i);
    }
    return 1;
}
