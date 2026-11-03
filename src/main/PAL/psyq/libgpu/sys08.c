/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"
#include "rr/draw_environment.h"

typedef struct { char pad[0x1C]; unsigned w; } W3L2_P;

extern const char D_80011F38[];
extern RrLibGraphicsDriverPrefix *D_800ABAB8;
extern int func_80096444();
extern char D_80011F4C[];
extern char D_80011F64[];

void func_80093344(s32 ordering_table);
int func_800933B4(int *a0);
void func_80093474(int a, W3L2_P *b);

void func_80093344(s32 ordering_table) {
    /* ASM_BARRIER: Keep the third dispatch argument distinct from the final constant zero. */
    register s32 second;
    register s32 third;
    RrLibGraphicsDriverPrefix *driver;
    if (D_800ABAC2.level >= 2) D_800ABABC(D_80011F38, ordering_table);
    second = ordering_table;
    driver = D_800ABAB8;
    /* ASM_BARRIER: Load the current driver before preparing the dispatch zeros. */
    __asm__("" : "+r"(driver) : : "$6");
    third = 0;
    /* ASM_BARRIER: Prevent reuse of the third argument when preparing the fourth zero. */
    __asm__("" : "+r"(third));
    driver->dispatch(driver->draw_ordering_table, second, third, 0);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 (*(int *(*))&D_800ABAB8)
#define D_800ABABC (*(int (*(*))())&D_800ABABC)
#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

int func_800933B4(int *a0) {
    unsigned char *p = &D_800ABAC2;
    if (*p >= 2) {
        D_800ABABC(D_80011F4C, a0);
    }
    func_80093E70((u32 *)(a0 + 7), (RrFrameEnvironment *)a0);
    a0[7] |= 0xFFFFFF;
    ((int (*)())D_800ABAB8[2])(D_800ABAB8[6], a0 + 7, 0x40, 0);
    func_80096444(p + 0xE, a0, 0x5C);
    return (int)a0;
}

#undef D_800ABAB8
#undef D_800ABABC
#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 (*(char *(*))&D_800ABAB8)
#define D_800ABABC (*(int (*(*))())&D_800ABABC)
#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

void func_80093474(int a, W3L2_P *b) {
    char *t;
    if (D_800ABAC2[0] >= 2) D_800ABABC(D_80011F64, a, b);
    t = (char *)b + 0x1C;
    func_80093E70((u32 *)t, (RrFrameEnvironment *)b);
    b->w = (b->w & 0xFF000000) | (a & 0xFFFFFF);
    (*(int (**)())(D_800ABAB8 + 8))(*(int *)(D_800ABAB8 + 0x18), t, 0x40, 0);
    func_80096444(D_800ABAC2 + 0xE, b, 0x5C);
}

#undef D_800ABAB8
#undef D_800ABABC
#undef D_800ABAC2
