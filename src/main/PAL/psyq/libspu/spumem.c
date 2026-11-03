/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/spu_allocation.h"
#include "psyq/libsnd/ssvab.h"

short func_8008AA74(int a0, short a1, int a2);
short func_8008AAA8(int a0, short a1, int a2);
int func_8008AADC(a0, a1);

/* -g retains lexical block labels used by stock MASPSX to place load delays.
 * Keep the pointer snapshot scope before the zero-size loop. */
void func_8008A774(void) {
 /* ASM_BARRIER: keep this phase value in the original MIPS register. */
 register s32 i asm("$9");
 register s32 n;
 /* Merge adjacent free records, skipping deleted records. */
 n=D_800A48BC;i=0;
 if(n>=0) {
  register u32 free_bit=0x80000000U;
  register u32 deleted=0x2FFFFFFFU;
  register u32 mask=0x0FFFFFFFU;
  register RrSpuAllocation *base=D_800A48C0;
  register s32 limit=n;
  /* ASM_BARRIER: keep this phase value in the original MIPS register. */
  register RrSpuAllocation *cur asm("$7")=base;
  /* ASM_BARRIER: keep this phase value in the original MIPS register. */
  register s32 j asm("$6");
  register RrSpuAllocation *scan;
  register RrSpuAllocation *next;
  register u32 other;
  /* ASM_BARRIER: keep this phase value in the original MIPS register. */
  register u32 next_address asm("$2");
  register u32 current_address;
  register u32 size;
  /* ASM_BARRIER: keep this phase value in the original MIPS register. */
  register s32 scaled asm("$2");
  do {
   if(cur->address_flags&free_bit) {
    j=i+1;
    scaled=j*8;
    scan=(RrSpuAllocation *)((unsigned long)scaled+(unsigned long)base);
    for(;;) {
     u32 flags=scan->address_flags;scan++;
     if(flags!=deleted)break;
     j++;
    }
    scaled=j*8;
    next=(RrSpuAllocation *)((unsigned long)scaled+(unsigned long)base);
    other=next->address_flags;
    if(other&free_bit) {
     next_address=other&mask;
     current_address=cur->address_flags;
     size=cur->size;
     current_address=(current_address&mask)+size;
     if(next_address==current_address) {
      next->address_flags=deleted;
      cur->size=cur->size+next->size;
      continue;
     }
    }
   }
   cur++;i++;
  }while(i<=limit);
  /* Reload the last index after merging; the negative path skips this read. */
  n=D_800A48BC;
 }
 /* ASM_BARRIER: preserve the phase boundary and arithmetic schedule. */
 __asm__("" : : "r"(n));
 /* Mark zero-sized records deleted. */
 i=0;
 if(n>=0) {
  register u32 deleted=0x2FFFFFFFU;
  register s32 limit=n;
  register RrSpuAllocation *cur;
  { RrSpuAllocation *snapshot=D_800A48C0; cur=snapshot; }
  do {if(cur->size==0)cur->address_flags=deleted;i++;cur++;}while(i<=limit);
 }
 /* Exchange-sort live records by the numeric address. */
 {
  register s32 limit=D_800A48BC;
  i=0;
  if(limit>=0) {
   register u32 end_bit=0x40000000U;
   /* ASM_BARRIER: keep this phase value in the original MIPS register. */
   register u32 mask asm("$12");
   register RrSpuAllocation *base;
   register RrSpuAllocation *cur;
   /* ASM_BARRIER: keep this phase value in the original MIPS register. */
   register RrSpuAllocation *inner asm("$8");
   /* ASM_BARRIER: keep this phase value in the original MIPS register. */
   register RrSpuAllocation *scan asm("$4");
   /* ASM_BARRIER: keep this phase value in the original MIPS register. */
   register s32 j asm("$6");
   register s32 bound;
   register u32 flags;
   register u32 other;
   register u32 masked;
   register u32 other_low;
   register s32 check;
   /* ASM_BARRIER: keep this phase value in the original MIPS register. */
   register s32 scaled asm("$2");
   mask=0x0FFF0000U;
   base=D_800A48C0;
   mask|=0xFFFFU;
   cur=base;
sort_loop:
    if(cur->address_flags&end_bit)goto sort_done;
    j=i+1;
    /* ASM_BARRIER: preserve the phase boundary and arithmetic schedule. */
    __asm__("" : "+r"(j));
    check=(limit<j);
    if(!check) {
     scaled=j*8;
     inner=cur;
     bound=D_800A48BC;scan=(RrSpuAllocation *)((unsigned long)scaled+(unsigned long)base);
inner_loop:
      other=scan->address_flags;
      if(other&end_bit)goto inner_done;
      other_low=other&mask;
      flags=inner->address_flags;
      masked=flags&mask;
      if(other_low<masked) {
       u32 a,b;
       inner->address_flags=other;
       a=scan->size;b=inner->size;
       inner->size=a;
       scan->address_flags=flags;
       scan->size=b;
      }
      j++;scan++;
     if(j<=bound)goto inner_loop;
inner_done:;
    }
    limit=D_800A48BC;i++;cur++;
    if(i<=limit)goto sort_loop;
sort_done:;
  }
 }
 /* Move the terminal record to the first deleted slot. */
 {
  register s32 last=D_800A48BC;
  i=0;
  if(last>=0) {
   register u32 end_bit=0x40000000U;
   register u32 deleted=0x2FFFFFFFU;
   /* ASM_BARRIER: keep this phase value in the original MIPS register. */
   register RrSpuAllocation *base asm("$6")=D_800A48C0;
   register RrSpuAllocation *cur=base;
   u32 flags;
compact_loop:
    flags=cur->address_flags;
    if(flags&end_bit)goto compact_done;
    if(flags!=deleted)goto compact_next;
    {
     register RrSpuAllocation *end=(RrSpuAllocation *)((unsigned long)(last*8)+(unsigned long)base);
     u32 size;
     cur->address_flags=end->address_flags;
     size=end->size;
     D_800A48BC=i;
     cur->size=size;
     goto compact_done;
    }
compact_next:
    last=D_800A48BC;i++;cur++;
    if(i<=last)goto compact_loop;
compact_done:;
  }
 }
 /* Fold trailing free records into the terminal record. */
 n=D_800A48BC;
 i=(s32)((u32)n-1U);
 /* ASM_BARRIER: preserve the phase boundary and arithmetic schedule. */
 __asm__("" : "+r"(i));
 if(i>=0) {
  /* ASM_BARRIER: keep this phase value in the original MIPS register. */
  register u32 free_bit asm("$8");
  register u32 mask;
  register u32 end_bit;
  register RrSpuAllocation *base;
  register RrSpuAllocation *cur;
  /* ASM_BARRIER: keep this phase value in the original MIPS register. */
  register s32 scaled asm("$2");
  register u32 flags;
  register s32 last;
  register u32 size;
  register u32 address;
  scaled=i*8;
  free_bit=0x80000000U;mask=0x0FFFFFFFU;end_bit=0x40000000U;
  base=D_800A48C0;cur=(RrSpuAllocation *)((unsigned long)scaled+(unsigned long)base);
  /* ASM_BARRIER: preserve the phase boundary and arithmetic schedule. */
  __asm__("" : "+r"(free_bit));
tail_loop:
   flags=cur->address_flags;
   if(!(flags&free_bit))goto tail_done;
   address=flags&mask;
   last=D_800A48BC;
   address|=end_bit;
   cur->address_flags=address;
   size=cur->size;
   D_800A48BC=i;
   i--;
   cur->size=size+base[last].size;
   cur=(RrSpuAllocation *)((unsigned long)cur-sizeof(RrSpuAllocation));
   if(i>=0)goto tail_loop;
tail_done:;
 }
}

short func_8008AA74(int a0, short a1, int a2) { return func_8008AAE4(a0, a1, func_8008AADC, a2); }

short func_8008AAA8(int a0, short a1, int a2) { return func_8008AAE4(a0, a1, func_8008AADC, a2); }

int func_8008AADC(a0, a1) { return a1; }
