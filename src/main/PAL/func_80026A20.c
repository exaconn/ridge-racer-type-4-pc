
struct S
{
  char pad[0x12];
  short x;
  char pad2[0x3C - 0x14];
};
extern struct S *D_800ABEA4;
void func_80026A20(int a, int b, short *c)
{
  int q = a / 5;
  int k = 0x800 - (&D_800ABEA4[a])->x;
  int new_var;
  new_var = k;
  k = (b - new_var) & 0xFFF;
  c[1] = k;
  c[0] = q;
}
