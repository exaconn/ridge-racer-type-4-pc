#include "types.h"
#include "rr_asm.h"

struct S
{
  char pad[0x12];
  short x;
  char pad2[0x3C - 0x14];
};

typedef struct {
    u16 h0, h2, h4, h6, h8, ha, hc, he, h10, h12, pad14, pad16;
    s32 r18;
    s32 r1c;
} StackFrame;

extern struct S *D_800ABEA4;
extern s32 D_800ABECC;

void func_80026A20(int a, int b, short *c);
s32 func_80026A74(s32 *pos, s32 startIdx, s32 maxIter);

void func_80026A20(int a, int b, short *c)
{
  int q = a / 5;
  int k = 0x800 - (&D_800ABEA4[a])->x;
  int new_var;
  new_var = k;
  k = (b - new_var) & 0xFFF;
  c[1] = k;
  c[0] = q;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEA4 (*(s32 (*))&D_800ABEA4)

s32 func_80026A74(s32 *pos, s32 startIdx, s32 maxIter) {
    StackFrame sp;
    s32 idx;
    s32 cnt;
    s32 px, pz;
    s32 next;
    s32 w78, w80;
    char *p, *q;
    /* ASM_BARRIER: GTE NCLIP operand registers ($s0, $t8, $t9) */
    register s32 r1 __asm__("$16");
    /* ASM_BARRIER: GTE NCLIP operand registers ($s0, $t8, $t9) */
    register s32 r2 __asm__("$24");
    /* ASM_BARRIER: GTE NCLIP operand registers ($s0, $t8, $t9) */
    register s32 r3 __asm__("$25");

    cnt = 0;
    idx = startIdx;
    px = pos[0];
    pz = pos[2];
    for (;;) {
        next = (idx + 1) % D_800ABECC;
        p = (char *)D_800ABEA4 + idx * 0x3C;
        q = (char *)D_800ABEA4 + next * 0x3C;
        w78 = *(s32 *)(p + 8);
        w80 = *(s32 *)(p + 12);
        sp.h4 = sp.h8 = *(u16 *)(q + 8) - (u16)w78;
        sp.h6 = sp.ha = *(u16 *)(q + 12) - (u16)w80;
        sp.h0 = (u16)px - (u16)w78;
        sp.h2 = (u16)pz - (u16)w80;
        sp.h4 += *(u16 *)(q + 0);
        sp.h6 += *(u16 *)(q + 2);
        sp.h8 += *(u16 *)(q + 4);
        sp.ha += *(u16 *)(q + 6);
        sp.hc = *(u16 *)(p + 0);
        sp.he = *(u16 *)(p + 2);
        sp.h10 = *(u16 *)(p + 4);
        sp.h12 = *(u16 *)(p + 6);
        r3 = *(s32 *)&sp.h0;
        r1 = *(s32 *)&sp.hc;
        r2 = *(s32 *)&sp.h10;
        RR_GTE_NCLIP3(r1, r2, r3, &sp.r18);
        r1 = *(s32 *)&sp.h8;
        r2 = *(s32 *)&sp.h4;
        r3 = *(s32 *)&sp.h0;
        RR_GTE_NCLIP3(r1, r2, r3, &sp.r1c);
        if (sp.r18 >= 0 && sp.r1c >= 0) {
            break;
        }
        cnt++;
        if (!(cnt & 1)) {
            idx -= cnt;
        } else {
            idx += cnt;
        }
        idx = (D_800ABECC + idx) % D_800ABECC;
        if (cnt > maxIter) {
            idx = startIdx;
            break;
        }
    }
    return idx;
}

#undef D_800ABEA4
