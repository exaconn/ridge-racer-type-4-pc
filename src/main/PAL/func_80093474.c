/* CC1_FLAGS: -mno-split-addresses */
typedef struct { char pad[0x1C]; unsigned w; } W3L2_P;
#include "rr/draw_environment.h"
extern int func_80096444();
extern char D_80011F64[];
extern char *D_800ABAB8;
extern int (*D_800ABABC)();
extern unsigned char D_800ABAC2[];
void func_80093474(int a, W3L2_P *b) {
    char *t;
    if (D_800ABAC2[0] >= 2) D_800ABABC(D_80011F64, a, b);
    t = (char *)b + 0x1C;
    func_80093E70((u32 *)t, (RrFrameEnvironment *)b);
    b->w = (b->w & 0xFF000000) | (a & 0xFFFFFF);
    (*(int (**)())(D_800ABAB8 + 8))(*(int *)(D_800ABAB8 + 0x18), t, 0x40, 0);
    func_80096444(D_800ABAC2 + 0xE, b, 0x5C);
}
