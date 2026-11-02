
typedef struct 
{
  int pad[6];
  int f;
} E;
extern E *D_800F4D8C;
void func_8004139C(a0, a1)
int a0;
int a1;
{
  int i;
  int new_var;
  for (i = 0; i < 4; i++)
  {
    new_var = i;
    D_800F4D8C[a1 + new_var].f = a0;
    new_var = 0xC000;
    a0 = a0 + new_var;
  }

}
