
extern int func_80093BFC();
typedef struct 
{
  char pad[0xB6C];
  unsigned int w[1];
} S;
extern S *D_800ACC14;
void func_800555E0()
{
  int new_var;
  unsigned int *p = *((unsigned int **) 0x1F800000);
  unsigned int *new_var2;
  unsigned int *ot;
  func_80093BFC(p, 1);
  ot = D_800ACC14->w;
  *p = (new_var = ((*p) & 0xFF000000) | ((*ot) & 0xFFFFFF));
  *ot = ((*ot) & 0xFF000000) | (((unsigned int) p) & 0xFFFFFF);
  new_var2 = p;
  *((unsigned int **) 0x1F800000) = (p = new_var2 + 3);
}
