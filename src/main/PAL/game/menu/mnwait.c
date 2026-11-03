#include "types.h"
#include "m2c_macros.h"

typedef struct 
{
  int pad[6];
  int f;
} E;

extern E *D_800F4D8C;
extern int func_8001ECE8();
extern s32 D_800AC5DC;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;

void func_8004139C();
void func_800413E0(void);

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

void func_800413E0(void) {
    func_8001ECE8(0, 0, 0, 2);
    D_800AC5DC = 0x80;
    D_800F4C6A = 1;
    D_800F3A26 = 8;
    D_800F4D8A = 0;
}
