#include "rr/gpu.h"

typedef struct {
    char pad[0x2A8];
    short s2A8;
    char pad2[0x76];
} S_800ABEF8;

extern S_800ABEF8 D_800ABEF8[];
extern char *D_800ACC14;
extern int D_800F3A30;
extern int D_800F4C74;
extern int D_800F4C80[];
extern int func_8002B5D0();
extern int func_80095A44();

void func_8002BA28(int a0) {
    /* ASM_BARRIER: pin &D_800ABEF8[a0] base pointer to $a0 */
    register S_800ABEF8 *p0 asm("a0");
    /* ASM_BARRIER: reserve $v0 across D_800F3A30 %hi address load so %hi(D_800F3A30) uses $v1 */
    register int dummy asm("v0");
    /* ASM_BARRIER: pin D_800F3A30 value load to $v1 */
    register int n asm("v1");
    int s7;
    int i;
    int s3;
    int v;
    int y;
    RrSpritePacket *p;

    s7 = D_800ABEF8[a0].s2A8;
    p0 = &D_800ABEF8[a0];
    /* ASM_BARRIER: combine D_800ABEF8[a0] address into $a0 and start $v0 live range before D_800F3A30 %hi */
    __asm__("" : "+r"(p0), "=r"(dummy));
    v = D_800F3A30;
    /* ASM_BARRIER: order prologue stack stores before D_800F3A30 %lo load and p0->s2A8 load in sched2 */
    __asm__("" : "=m"(*p0), "=m"(D_800F3A30) : "r"(dummy));
    s7 = p0->s2A8;
    n = D_800F3A30;
    if (n > 0) {
        i = 0;
        do {
            if (i == s7) {
                /* ASM_BARRIER: prevent jump_optimize from merging identical then/else branches and give i higher alloc priority than GIV */
                __asm__("" : : "r"(i));
                s3 = 0x7FF8;
            } else {
                s3 = 0x7FF8;
            }
            v = D_800F4C80[i];
            y = i * 10 + 0x2C;
            func_8002B5D0(0xF0, y, v, s3, 0);
            p = *(RrSpritePacket **)0x1F800000;
            p->len = 4;
            p->code = 0x64;
            p->x = 0xE7;
            p->y = y;
            p->u = i * 12 + 12;
            p->v = 0xA8;
            p->clut = s3;
            p->code |= 1;
            p->w = 0xC;
            p->h = 0xC;
            func_80095A44(D_800ACC14 + 0x70, p);
            p++;
            *(RrSpritePacket **)0x1F800000 = p;
            i++;
        } while (i < D_800F3A30);
    }
    func_8002B5D0(0xF0, 0x1C, D_800F4C74, s3, 0);
}
