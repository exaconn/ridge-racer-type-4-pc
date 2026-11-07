/* CC1_FLAGS: -mno-split-addresses */
#include "rr/spu.h"

s32 SsVabTransBody(const u16 *source,s32 raw);
u32 SpuWrite(const u16 *a0, u32 a1);

s32 SsVabTransBody(const u16 *source,s32 raw) {
 s32 result;
 if((u16)raw<=16) {
  s32 bank=(s16)raw;
  /* ASM_BARRIER: Preserve the byte offset across both state-changing calls. */
  register u32 offset __asm__("$18");
  register u8 *flags __asm__("$3");
  u32 flag;
  /* ASM_BARRIER: Normalize the bank before creating the flag-page address. */
  __asm__("" : : "r"(bank) : "$3");
  flags=(u8*)0x80110000U;
  /* ASM_BARRIER: Expose the indexed flag load through its real PAL page. */
  __asm__("" : "+r"(flags));
  flag=*(u8*)(flags+bank+0x120);
  {s32 two=2;
  /* ASM_BARRIER: Keep the flag comparison constant independent of offset scaling. */
  __asm__("" : "+r"(two));
  if(flag==two) {
   register unsigned long allocation __asm__("$16");
   offset=(u32)bank*4;
   /* ASM_BARRIER: Finish scaling before materializing the allocation page. */
   __asm__("" : : "r"(offset) : "$16");
   allocation=0x80110000U;
   /* ASM_BARRIER: Read the actual allocation page into the preserved address register. */
   __asm__("" : "+r"(allocation));
   allocation=*(u32*)(allocation+offset+0x180);
   SpuSetTransferMode(0);
   if(SpuSetTransferStartAddr(allocation)) {
    register u8 *sizes __asm__("$5");
    /* ASM_BARRIER: Load the size page only after successful address programming. */
    __asm__("" : : : "$5");
    sizes=(u8*)0x80110000U;
    /* ASM_BARRIER: Expose the fresh size load in its outgoing argument register. */
    __asm__("" : "+r"(sizes));
    SpuWrite(source,*(u32*)(sizes+offset+0x138));
    result=bank;
    /* ASM_BARRIER: Capture the returned bank before publishing its success flag. */
    __asm__("" : "+r"(result));
    {
     register u8 *final __asm__("$1");
     u32 one=1;
     /* ASM_BARRIER: Materialize the success byte before its PAL page address. */
     __asm__("" : : "r"(one) : "$1");
     final=(u8*)0x80110000U;
     /* ASM_BARRIER: Keep the actual success-store address in the assembler temporary. */
     __asm__("" : "+r"(final));
     final+=result;
     final[0x120]=one;
    }
    return result;
   }
  }
 }
 }
 _spu_setInTransfer(0);
 return -1;
}

u32 SpuWrite(const u16 *a0, u32 a1) {
  if (a1 > 0x7EFF0) a1 = 0x7EFF0;
  _spu_Fw(a0, a1);
  if (_spu_transferCallback == 0) _spu_inTransfer = 0;
  return a1;
}
