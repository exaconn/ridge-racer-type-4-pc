/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/spu_init.h"

#define BANK ((RrSpuRegisterStorage *)D_800A47D8)
#define STATUS(bank) ((bank)->halves[215])

extern int D_800A4844;

int func_8007EA34();
s32 func_8007EAC4(s32 warm);

/* legacy typing of shared globals in this part of the unit */
#define D_800A47D8 ((char *)D_800A47D8)

int func_8007EA34(a0)
int a0;
{
  int idx = -1;
  int i = 0;
  int one = 1;
  int m;
  int r;
  int x;
  int b;
  int f;
  int off;
  unsigned short h;
  loop:
  m = one << i;

  if (a0 & m)
  {
    goto found;
  }
  i++;
  if (i < 0x18)
  {
    goto loop;
  }
  check:
  if (idx != (-1))
  {
    goto have;
  }

  r = -1;
  goto out;
  found:
  idx = i;

  goto check;
  have:
  a0 = idx << 4;

  a0 = a0 + ((int) D_800A47D8);
  f = D_800A4844;
  f = f & (1 << idx);
 do { a0 = *((unsigned short *) (a0 + 0xC)); x = a0 != 0; if (f == 0) { r = x << 1; goto out; } } while (0);
  r = 1;
  if (a0 != 0)
  {
    goto out;
  }
  r = 3;
  out:
  return r;

}

#undef D_800A47D8

s32 func_8007EAC4(s32 warm)
{
    register RrSpuRegisterStorage *bank __asm__("$2");
    u32 count;
    s32 i;
    u16 *entry;
    *D_800A47E8 |= 0xB0000;
    D_800A47F4=0;
    D_800A47F8=0;
    D_800A47F0=0;
    bank=BANK;
    bank->halves[192]=0;
    bank->halves[193]=0;
    bank->halves[213]=0;
    func_8007F59C();
    bank=BANK;
    bank->halves[192]=0;
    bank->halves[193]=0;
    count=0;
    if(STATUS(bank)&0x7FF) {
        /* ASM_BARRIER: Retain the initialized count before its first increment. */
        __asm__("" : "+r"(count));
        count++;
        do {
            if(count>0xF00) {
                func_8001E84C(D_80011698,D_800116A8);
                break;
            }
            count++;
        } while(STATUS(BANK)&0x7FF);
    }
    i=0;
    /* ASM_BARRIER: Keep the clear loop indexed by its initialized row count. */
    __asm__("" : "+r"(i));
    entry=D_8010EFB0;
    D_800A47FC=2;
    D_800A4800=3;
    D_800A4804=8;
    D_800A4808=7;
    /* ASM_BARRIER: Publish software configuration before capturing the bank. */
    __asm__("" : : : "memory");
    bank=BANK;
    bank->halves[214]=4;
    bank->halves[194]=0;
    bank->halves[195]=0;
    bank->halves[198]=0xFFFF;
    bank->halves[199]=0xFFFF;
    bank->halves[204]=0;
    bank->halves[205]=0;
    do {
        *entry++=0;
        i++;
    } while(i<10);
    if(warm==0) {
        const u16 *source=D_800A4818;
        /* ASM_BARRIER: Prepare the real bounded input owner before MMIO writes. */
        __asm__("" : : "r"(source));
        bank=BANK;
        D_800A47F0=0x200;
        bank->halves[200]=0;
        bank->halves[201]=0;
        bank->halves[202]=0;
        bank->halves[203]=0;
        bank->halves[216]=0;
        bank->halves[217]=0;
        bank->halves[218]=0;
        bank->halves[219]=0;
        /* ASM_BARRIER: Finish MMIO stores before preparing the transfer length. */
        __asm__("" : : : "$5", "memory");
        func_8007ED44(source,16);
        i=0;
        {
        volatile u16 *voice=BANK->halves;
        do {
            voice[0]=0;voice[1]=0;voice[2]=0x3FFF;
            voice[3]=0x200;voice[4]=0;voice[5]=0;
            /* ASM_BARRIER: Publish each voice before advancing its counter. */
            __asm__("" : "+r"(i) : : "memory");
            i++;voice+=8;
        } while(i<24);
        }
        bank=BANK;
        bank->halves[196]=0xFFFF;
        bank->halves[197]=0xFF;
        func_8007F59C();func_8007F59C();func_8007F59C();func_8007F59C();
        bank=BANK;
        bank->halves[198]=0xFFFF;
        bank->halves[199]=0xFF;
        func_8007F59C();func_8007F59C();func_8007F59C();func_8007F59C();
    }
    { register s32 result __asm__("$2")=0;
    register RrSpuRegisterStorage *end=BANK;
    
    D_800A480C=1;
    end->halves[213]=0xC000;
    D_800A4810=0;
    D_800A4814=0;
    /* ASM_BARRIER: Finish reset stores before the return phase. */
    __asm__("" : : "r"(result) : "memory");
    return result; }
}
