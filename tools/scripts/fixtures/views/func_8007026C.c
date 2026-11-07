/* CC1_FLAGS: -fno-schedule-insns */

#include "rr/relative_file.h"

void RelocateRelativeFile(RrRelativeHeader *input){
 RrRelativeHeader *base=input;
 /* ASM_BARRIER: Preserve the entry cursor allocation through the loop. */
 register RrRelativeSlot *cursor __asm__("$6")=(RrRelativeSlot*)(base+1);
 u32 count=base->count;
 /* ASM_BARRIER: Preserve the relocation index allocation through the loop. */
 register u32 i __asm__("$7");
 u8 *first,*second;
 u32 last;
 
 D_800F2EBC=base;
 D_800F39A4=count;
 i=0;
 count*=8U;
 /* ASM_BARRIER: Keep the raw count publication before the first relocations. */
 __asm__("" : : "r"(count), "m"(D_800F39A4));
 first=rr_relative_file_address(base,base->first[0].relative);
 second=rr_relative_file_address(base,base->first[1].relative);
 base->first[0].absolute=first;
 base->first[1].absolute=second;
 /* ASM_BARRIER: Keep the in-place slot store before publishing its pointer. */
 __asm__("" : "=r"(second) : "0"(second), "m"(base->first[1]));
 D_800F391C=second;
 base->first[2].absolute=rr_relative_file_address(base,base->first[2].relative);
 D_800F2EB4=base->first[2].absolute;
 D_800F29E0=rr_relative_file_address(base,base->offsets[0]);
 D_800F4D34=rr_relative_file_address(base,base->offsets[1]);
 D_800ADAE8=rr_relative_file_address(base,base->offsets[2]);
 D_800F4D28=rr_relative_file_address(base,base->offsets[3]);
 D_800F4C60=rr_relative_file_address(base,base->offsets[4]);
 D_800ABEA8=rr_relative_file_address(base,base->offsets[5]);
 D_800F4C20=rr_relative_file_address(base,base->offsets[6]);
 D_800ABE9C=rr_relative_file_address(base,base->offsets[7]);
 D_800FFC40=rr_relative_file_address(base,base->offsets[8]);
 D_800F4C30=rr_relative_file_address(base,base->offsets[9]);
 D_800ABEA0=rr_relative_file_address(base,base->offsets[10]);
 D_800F4C78=rr_relative_file_address(base,base->offsets[11]);
 D_800ABEB8=rr_relative_file_address(base,base->offsets[12]);
 D_800F4C6C=rr_relative_file_address(base,base->offsets[13]);
 D_800ABEB4=rr_relative_file_address(base,base->offsets[14]);
 last=base->offsets[15];
 D_800F3820=cursor;
 D_800ACC04=rr_relative_file_address(base,last);
 if(count!=0)do {
  cursor->absolute=rr_relative_file_address(base,cursor->relative);
  /* ASM_BARRIER: Reload the shared count after each pointer write while
   * retaining its original address register across the loop. */
  __asm__("" : "+m"(D_800F39A4) : : "$3", "$4");
  i++;
  cursor++;
 }while(i<D_800F39A4*8U);
}
