
extern int func_8001FCC8();
extern int func_8001FF84();
extern int func_80090424();
extern char D_8009F648[];
extern int D_800ABEB0;
extern int D_800ACC14;
void func_80032964(a0, a1)
int a0;
int a1;
{
  int v = func_80090424((D_800ABEB0 << 6) & 0xFC0);
  int new_var;
  if (v < 0)
  {
    v += 0x7F;
  }
  new_var = *((int *) 0x1F800000);
  *((int *) 0x1F800000) = func_8001FCC8(D_800ACC14 + 0x70, func_8001FF84(D_800ACC14 + 0x70, new_var, a0, a1, D_8009F648, 0x1B56, ((v >> 7) + 0x80) * 2), 0x25);
}
