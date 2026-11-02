
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
extern int func_80025EB0();
extern int func_8006E368();
extern int func_80090424();
extern int func_800904F4();
extern int D_800ABECC;
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
