/* CC1_FLAGS: -mno-split-addresses */

#include "rr/spu.h"

/* Nonzero low16 pitch, with the fixed PAL tables: both searches always match. */
u32 _spu_pitch2note(s32 first,s32 second,u32 pitch) {
 register s32 top=0;
 register s32 i;
 register s32 note;
 /* ASM_BARRIER: Preserve the fine-search result and accumulated caller word. */
 register s32 fine __asm__("$9");
 s32 value;
 register u16 *table;
 u32 result;
 if((u16)pitch>=0x4000)pitch=0x3FFF;
 i=0;value=(u16)pitch;
 do {if((value>>i)&1)top=i;i++;}while(i<14);
 pitch=(u16)pitch<<(15-top);
 i=11;value=(u16)pitch;
 table=&SvmSemitoneTable[11];
 for(;i>=0;i--,table--) {
  if((u32)value>=*table){note=i;break;}
 }
 value=((u32)(u16)pitch<<15)/SvmSemitoneTable[(u16)note];
{
  register u16 *fine_table;
  i=127;
  fine_table=&SvmPitchFineTable[127];
  value=(u16)value;
  for(;i>=0;i--,fine_table--) {
   if((u32)value>=*fine_table){fine=i;break;}
  }
 }
 {
  /* ASM_BARRIER: Reuse the result register for two consecutive, disjoint sums. */
  register u32 next __asm__("$2")=(u32)(fine+1);
  register u32 total __asm__("$2");
  register u32 part;
  fine=(u32)second+next;
  part=(u32)(top-12);
  total=(u32)note+((u32)first+part*12U);
  {
   /* ASM_BARRIER: Reuse the consumed first argument for the packed low halfword. */
   register u32 packed __asm__("$4")=(u16)fine;
   part=packed>>7;
   note=total+part;
   result=(u32)(u16)note<<8;
   /* ASM_BARRIER: Finish the note bits before masking the independent fine bits. */
   __asm__("" : "+r"(result), "+r"(packed));
   packed&=0x7E;
   /* ASM_BARRIER: Retain unmasked search values until the final result merge. */
   __asm__("" : : "r"(top),"r"(note),"r"(fine));
   return result|packed;
  }
 }
}
