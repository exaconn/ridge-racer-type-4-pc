#include "types.h"
#include "m2c_macros.h"

struct A
{
  char p0[0x20];
  int f20;
  char p1[4];
  int f28;
  char p2[0xC4];
  int fF0;
  int fF4;
  char p3[0x78];
  short f170;
  char p4[0x78];
  short f1EA;
};

extern int func_80025D98();
extern s32 func_80025EB0();
extern u32 func_80025FBC();
extern s32 func_8006E368();
extern s32 func_80090424();
extern s32 func_800904F4();
extern s32 D_800ABECC;

void func_80024A88(void *arg0);
int func_80024BCC(struct A *a);

void func_80024A88(void *arg0) {
    s32 sp[4];
    s32 temp_a1;
    s32 temp_hi;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s2;
    s32 var_v1;
    s32 var_v1_2;
    u32 temp_v0;

    temp_s2 = M2C_FIELD(arg0, s32 *, 0xF4);
    temp_s0 = M2C_FIELD(arg0, s32 *, 0xF0) - 2;
    var_v1 = temp_s0;
    if (temp_s0 < 0) {
        var_v1 = D_800ABECC + temp_s0;
    }
    temp_hi = var_v1 % (s32) D_800ABECC;
    temp_s0 = temp_hi;
    func_80025D98(temp_hi, sp, M2C_FIELD(arg0, s16 *, 0x170));
    temp_s0_2 = 0x800 - func_80025EB0(temp_hi, M2C_FIELD(arg0, s16 *, 0x170));
    var_v1_2 = func_800904F4(temp_s0_2) * temp_s2;
    if (var_v1_2 < 0) {
        var_v1_2 += 0xFFF;
    }
    sp[0] += var_v1_2 >> 0xC;
    temp_a1 = sp[2] - ((s32) (func_80090424(temp_s0_2) * temp_s2) / 4096);
    sp[2] = temp_a1;
    temp_v0 = func_80025FBC(M2C_FIELD(arg0, s16 *, 0x1D4), 0x400 - func_8006E368(sp[0] - M2C_FIELD(arg0, s32 *, 0x20), temp_a1 - M2C_FIELD(arg0, s32 *, 0x28)));
    M2C_FIELD(arg0, s16 *, 0x1D4) = (s16) ((u16) M2C_FIELD(arg0, s16 *, 0x1D4) + ((s32) ((temp_v0 >> 0x1F) + temp_v0) >> 1));
}

int func_80024BCC(struct A *a)
{
  int sp[4];
  int s0;
  int idx;
  /* ASM_BARRIER: pin z to v0 (original loads the lh into v0, not v1) */
  register int z asm("$2");
  /* ASM_BARRIER: pin constant 0x800 to s0 (original: addiu s0,zero,0x800; subu s0,s0,v0) */
  int t; register int c asm("$16");
  z = a->f1EA;
  t = a->fF0;
  if (z || a->f20)
  {
    s0 = t + 2;
  }
  else
  {
    s0 = t + 2;
  }
  if (z == 0)
  {
    s0 = t - 2;
  }
  s0 = (s0 + D_800ABECC) % D_800ABECC;
  func_80025D98(s0, sp, a->f170);
  t = func_80025EB0(s0, a->f170);
  c = 0x800;
  s0 = c - t;
  sp[0] += (func_800904F4(s0) * a->fF4) >> 12;
  sp[2] -= (func_80090424(s0) * a->fF4) >> 12;
  return 0x400 - func_8006E368(sp[0] - a->f20, sp[2] - a->f28);
}
