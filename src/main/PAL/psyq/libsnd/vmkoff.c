/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channels.h"
#include "psyq/libsnd/vmkoff.h"

extern signed char D_80110100[];
extern char *D_801100F8;

void func_80089E1C(s32 input);

void func_80089E1C(s32 input) {
    register s32 maximum=D_801100FC;
    register u32 index;
    s32 wanted;
    u32 one;
    s32 next;
    /* ASM_BARRIER: Read the active count before initializing the saved loop counter. */
    __asm__("" : "+r"(maximum) : : "$16");
    index=0;
    /* ASM_BARRIER: Retain a byte-index computation for the initial loop entry. */
    __asm__("" : "+r"(index));
    if(maximum>0) {
        one=1;
        wanted=(s16)input;
        next=(u8)index;
again: {
            s32 i=next;
            if(!(D_800A48F8 & (one << i)) && D_8010F908[i].match10==wanted) {
                D_80110118=(u8)index;
                func_80088DA4(0);
            }
            index++;
        }
        {
            s32 limit=D_801100FC;
            s32 test;
            /* ASM_BARRIER: Reload the count before the independent byte comparison. */
            __asm__("" : : "r"(limit) : "$2");
            test=(u8)index;
            /* ASM_BARRIER: Separate the comparison byte from the next body's byte index. */
            __asm__("" : "+r"(test));
            next=(u8)index;
            if(test<limit)goto again;
        }
    }
}

unsigned char func_80089ED4(a0, a1) char *a0; char *a1; {
  unsigned char n = 0; signed char i = 0;
  if (D_80110100[0] > 0) {
    do {
      int ie = i;
      char *t = (char *)(((D_80110100[7] << 4) + ie) * 32 + (int)D_801100F8);
      if (!(t[6] > D_80110100[2]) && !(t[7] < D_80110100[2])) {
        a1[n] = t[0x16];
        a0[n++] = i;
      }
      i++;
    } while (i < D_80110100[0]);
  }
  return n; }
