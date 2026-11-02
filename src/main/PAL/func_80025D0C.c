struct S { char pad[0x12]; short x; char pad2[0x3C-0x14]; };
extern struct S *D_800ABEA4;
extern int D_800ABECC;
extern int func_80025C94();
void func_80025D0C(int a, int b)
{
  int n = (a + 1) % D_800ABECC;
  register struct S *t asm("$5"); register struct S *p asm("$3"); struct S *q; t = D_800ABEA4; p = &t[a]; q = &t[n]; func_80025C94(p->x, q->x, b, n);
}
