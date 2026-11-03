#include "rr/ui.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800F4D80 (*(int (*))&D_800F4D80)

#undef D_800F4D80

/* legacy typing of shared globals in this part of the unit */
#define D_800F4D80 ((UiElem24 *)D_800F4D80)

#define func_8001FD20 ((s32 (*)())func_8001FD20)

#define func_80020074 ((s32 (*)())func_80020074)

#undef D_800F4D80

#undef func_8001FD20

#undef func_80020074

void func_8003E928(s32 index,s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 count,s32 g,s32 h,...) {
 register s32 i;
 u32 offset;
 register RrUiRow24 *tail;
 /* ASM_BARRIER: Keep the real PAL pointer page in its shared address register. */
 register u8 *page __asm__("$9");
 if(index<D_800F4BEC) {
  /* Pointer storage is re-read after byte/halfword stores that can alias it. */
  page=(u8*)0x800F0000U;
  /* Preserve PAL offset wrapping while widening the signed offset for host pointers. */
  offset=(u32)index*sizeof(RrUiRow24);
  ((RrUiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)*(RrUiRow24 *volatile *)(page+0x4D80)))->byte15=g;
  ((RrUiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)*(RrUiRow24 *volatile *)(page+0x4D80)))->byte16=h;
  ((RrUiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)*(RrUiRow24 *volatile *)(page+0x4D80)))->byte01=f;
  ((RrUiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)*(RrUiRow24 *volatile *)(page+0x4D80)))->byte14=e;
  {
   RrUiRow24 *row=(RrUiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)*(RrUiRow24 *volatile *)(page+0x4D80));
   row->byte02=count;
   /* Capture this fresh base before the halfword stores, as in the original. */
   tail=*(RrUiRow24 *volatile *)(page+0x4D80);

   row->half04=a;row->half06=b;row->half08=c;row->half0A=d;
  }
  ((RrUiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)tail))->byte17=0;
  i=0;
  ((RrUiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)*(RrUiRow24 *volatile *)(page+0x4D80)))->half20=0;
  if(count>0) {
   register u8 *loop_page=page;
   register u32 clear_offset;
   clear_offset=offset;
   do {
    *(u8 *)((unsigned long)(long)(s32)clear_offset+(unsigned long)*(RrUiRow24 *volatile *)(loop_page+0x4D80)+i+RR_FIELD_OFFSET(RrUiRow24,cells))=0;
    i++;
   }while(i<count);
  }
 }
}
