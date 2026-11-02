
extern int func_8001F400();
extern int func_8001FD20();
extern char D_80011680[];
extern char D_80011688[];
extern char D_80011690[];
extern int D_800ACC14;
void func_8007D65C(int a0)
{
  int new_var;
  int new_var2;
  func_8001F400(0x28, 0xA0, D_80011680, 0x414);
  func_8001F400(0x68, 0xA0, D_80011688, 0x414);
  func_8001F400(0xC2, 0xA0, D_80011690, 0x414);
  new_var2 = *((int *) 0x1F800000);
  *((int *) 0x1F800000) = (new_var = func_8001FD20(D_800ACC14 + 0x74, new_var2, 0x20, (a0 << 5) + 0xB2, 0x104, 0xC, 0x80, 0x20, 0x20));
}
