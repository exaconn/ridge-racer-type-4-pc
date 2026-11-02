
extern int func_8002E828();
extern int func_8006E1AC();
extern short D_8009F694[];
extern short D_8009F6A0[];
extern struct 
{
  char pad[0x178];
  int v;
  char pad2[0x320 - 0x17C];
} D_800ABEF8[];
extern int D_800AC5D4;
extern struct 
{
  short a;
  unsigned short b;
  char pad[0x58];
} D_800F3A38[];
void func_80033A74(int a)
{
  int b1 = (int) D_8009F6A0;
  int o = a * 6;
  short *p = (short *) (o + b1);
  short *q;
  int i;
  /* ASM_BARRIER: pin scratchpad base to $a0 like the original */
  register int *g asm("$4");
  g = (int *) 0x1F800000;
  g[2] = p[0];
  g[3] = p[1];
  g[4] = p[2];
  q = (short *) (o + ((int) D_8009F694));
  g[6] = q[0];
  g[7] = q[1];
  g[8] = q[2];
  func_8006E1AC(g, q);
  {
    int t;
    t = 0x1F800028;
    /* ASM_BARRIER: forces the 0x1F800028 li to be scheduled before the li a0,2 (original delay-slot layout) */
    asm volatile("" : "=r"(t) : "0"(t));
    func_8002E828(2, t);
  }
  ;
  if (D_800F3A38[D_800AC5D4 & 1].b & 4)
  {
    D_800ABEF8[D_800AC5D4 & 1].v += 0xC0;
  }
  else
    if (D_800F3A38[D_800AC5D4 & 1].b & 8)
  {
    D_800ABEF8[D_800AC5D4 & 1].v -= 0xC0;
  }
  i = D_800ABEF8[D_800AC5D4 & 1].v;
  if (i > 0xF00)
  {
    D_800ABEF8[D_800AC5D4 & 1].v = 0xF00;
  }
  else
    if (i < (-0xF00))
  {
    D_800ABEF8[D_800AC5D4 & 1].v = -0xF00;
  }
}
