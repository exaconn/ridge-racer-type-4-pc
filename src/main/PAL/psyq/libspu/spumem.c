/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/spu_allocation.h"
#include "psyq/libsnd/ssvab.h"

short SsVabOpenHeadSticky(int a0, short a1, int a2);
short SsVabFakeHead(int a0, short a1, int a2);
int VsGetAddr(int a0, int a1);

/* -g retains lexical block labels used by stock MASPSX to place load delays.
 * Keep the pointer snapshot scope before the zero-size loop. */
void _spu_gcSPU(void) {
 /* ASM_BARRIER: keep this phase value in the original MIPS register. */
 register s32 i asm("$9");
 register s32 n;
 /* Merge adjacent free records, skipping deleted records. */
 n=_spu_AllocLastNum;i=0;
 if(n>=0) {
  register u32 free_bit=0x80000000U;
  register u32 deleted=0x2FFFFFFFU;
  register u32 mask=0x0FFFFFFFU;
  register SpuAllocation *base=_spu_memList;
  register s32 limit=n;
  /* ASM_BARRIER: keep this phase value in the original MIPS register. */
  register SpuAllocation *cur asm("$7")=base;
  /* ASM_BARRIER: keep this phase value in the original MIPS register. */
  register s32 j asm("$6");
  register SpuAllocation *scan;
  register SpuAllocation *next;
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
    scan=(SpuAllocation *)(scaled+(unsigned long)base);
    for(;;) {
     u32 flags=scan->address_flags;scan++;
     if(flags!=deleted)break;
     j++;
    }
    scaled=j*8;
    next=(SpuAllocation *)(scaled+(unsigned long)base);
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
  n=_spu_AllocLastNum;
 }
 /* ASM_BARRIER: preserve the phase boundary and arithmetic schedule. */
 __asm__("" : : "r"(n));
 /* Mark zero-sized records deleted. */
 i=0;
 if(n>=0) {
  register u32 deleted=0x2FFFFFFFU;
  register s32 limit=n;
  register SpuAllocation *cur;
  { SpuAllocation *snapshot=_spu_memList; cur=snapshot; }
  do {if(cur->size==0)cur->address_flags=deleted;i++;cur++;}while(i<=limit);
 }
 /* Exchange-sort live records by the numeric address. */
 {
  register s32 limit=_spu_AllocLastNum;
  i=0;
  if(limit>=0) {
   register u32 end_bit=0x40000000U;
   /* ASM_BARRIER: keep this phase value in the original MIPS register. */
   register u32 mask asm("$12");
   register SpuAllocation *base;
   register SpuAllocation *cur;
   /* ASM_BARRIER: keep this phase value in the original MIPS register. */
   register SpuAllocation *inner asm("$8");
   /* ASM_BARRIER: keep this phase value in the original MIPS register. */
   register SpuAllocation *scan asm("$4");
   s32 j;
   register s32 bound;
   register u32 flags;
   register u32 other;
   register u32 masked;
   register u32 other_low;
   register s32 check;
   /* ASM_BARRIER: keep this phase value in the original MIPS register. */
   register s32 scaled asm("$2");
   mask=0x0FFF0000U;
   base=_spu_memList;
   mask|=0xFFFFU;
   cur=base;
sort_loop:
    if(cur->address_flags&end_bit)goto sort_done;
    j=i+1;
    check=(limit<j);
    if(!check) {
     scaled=j*8;
     inner=cur;
     bound=_spu_AllocLastNum;scan=(SpuAllocation *)(scaled+(unsigned long)base);
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
    limit=_spu_AllocLastNum;i++;cur++;
    if(i<=limit)goto sort_loop;
sort_done:;
  }
 }
 /* Move the terminal record to the first deleted slot. */
 {
  register s32 last=_spu_AllocLastNum;
  i=0;
  if(last>=0) {
   register u32 end_bit=0x40000000U;
   register u32 deleted=0x2FFFFFFFU;
   /* ASM_BARRIER: keep this phase value in the original MIPS register. */
   register SpuAllocation *base asm("$6")=_spu_memList;
   register SpuAllocation *cur=base;
   u32 flags;
compact_loop:
    flags=cur->address_flags;
    if(flags&end_bit)goto compact_done;
    if(flags!=deleted)goto compact_next;
    {
     register SpuAllocation *end=(SpuAllocation *)((last*8)+(unsigned long)base);
     u32 size;
     cur->address_flags=end->address_flags;
     size=end->size;
     _spu_AllocLastNum=i;
     cur->size=size;
     goto compact_done;
    }
compact_next:
    last=_spu_AllocLastNum;i++;cur++;
    if(i<=last)goto compact_loop;
compact_done:;
  }
 }
 /* Fold trailing free records into the terminal record. */
 n=_spu_AllocLastNum;
 i=(n-1U);
 /* ASM_BARRIER: preserve the phase boundary and arithmetic schedule. */
 __asm__("" : "+r"(i));
 if(i>=0) {
  u32 free_bit;
  register u32 mask;
  register u32 end_bit;
  register SpuAllocation *base;
  register SpuAllocation *cur;
  s32 scaled;
  register u32 flags;
  register s32 last;
  register u32 size;
  register u32 address;
  scaled=i*8;
  free_bit=0x80000000U;mask=0x0FFFFFFFU;end_bit=0x40000000U;
  base=_spu_memList;cur=(SpuAllocation *)(scaled+(unsigned long)base);
tail_loop:
   flags=cur->address_flags;
   if(!(flags&free_bit))goto tail_done;
   address=flags&mask;
   last=_spu_AllocLastNum;
   address|=end_bit;
   cur->address_flags=address;
   size=cur->size;
   _spu_AllocLastNum=i;
   i--;
   cur->size=size+base[last].size;
   cur=(SpuAllocation *)((unsigned long)cur-sizeof(SpuAllocation));
   if(i>=0)goto tail_loop;
tail_done:;
 }
}

short SsVabOpenHeadSticky(int a0, short a1, int a2) { return _SsVabOpenHeadWithMode(a0, a1, VsGetAddr, a2); }

short SsVabFakeHead(int a0, short a1, int a2) { return _SsVabOpenHeadWithMode(a0, a1, VsGetAddr, a2); }

int VsGetAddr(a0, a1) { return a1; }
