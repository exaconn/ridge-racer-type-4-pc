/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 2.7.2 */
/* CC1_FLAGS: -fsigned-char */

typedef struct {
    int x;
} __attribute__((packed)) U4;

struct DirPath {
    int a;
    int b;
    int c;
    char name[32];
};

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
extern int func_80096414(void *, const char *, int);
extern int func_8001E84C();
extern void func_80096444(char *, void *, int);

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
