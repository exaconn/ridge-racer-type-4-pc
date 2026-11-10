#include "rr/ui.h"
void InitUiRow(s32 index,s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 count,s32 g,s32 h,...) {
 register s32 i;
 u32 offset;
 register UiRow24 *tail;
 /* ASM_BARRIER: Keep the real PAL pointer page in its shared address register. */
 register u8 *page __asm__("$9");
 if(index<UiRowCount) {
  /* Pointer storage is re-read after byte/halfword stores that can alias it. */
  page=(u8*)0x800F0000U;
  /* Preserve PAL offset wrapping while widening the signed offset for host pointers. */
  offset=(u32)index*sizeof(UiRow24);
  ((UiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)*(UiRow24 *volatile *)(page+0x4D80)))->byte15=g;
  ((UiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)*(UiRow24 *volatile *)(page+0x4D80)))->byte16=h;
  ((UiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)*(UiRow24 *volatile *)(page+0x4D80)))->byte01=f;
  ((UiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)*(UiRow24 *volatile *)(page+0x4D80)))->byte14=e;
  {
   UiRow24 *row=(UiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)*(UiRow24 *volatile *)(page+0x4D80));
   row->byte02=count;
   /* Capture this fresh base before the halfword stores, as in the original. */
   tail=*(UiRow24 *volatile *)(page+0x4D80);

   row->half04=a;row->half06=b;row->half08=c;row->half0A=d;
  }
  ((UiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)tail))->byte17=0;
  i=0;
  ((UiRow24 *)((unsigned long)(long)(s32)offset+(unsigned long)*(UiRow24 *volatile *)(page+0x4D80)))->half20=0;
  if(count>0) {
   register u8 *loop_page=page;
   register u32 clear_offset;
   clear_offset=offset;
   do {
    *(u8 *)((unsigned long)(long)(s32)clear_offset+(unsigned long)*(UiRow24 *volatile *)(loop_page+0x4D80)+i+RR_FIELD_OFFSET(UiRow24,cells))=0;
    i++;
   }while(i<count);
  }
 }
}
