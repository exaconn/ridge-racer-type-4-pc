/* GCC_VERSION: 2.7.2 */
#include "types.h"
#include "m2c_macros.h"

extern int D_801143A8[];
extern int D_801143AC[];
extern int D_801143B0[];
extern int func_8008B3C4();

int func_80096DA4();
int func_80096DB8();
int func_80096DCC();
void func_80096DE0(s32 arg0);

int func_80096DA4(a0) int a0; { int *p = D_801143A8; int r = *p; *p = a0; return r; }

int func_80096DB8(a0) int a0; { int *p = D_801143AC; int r = *p; *p = a0; return r; }

int func_80096DCC(a0) int a0; { int *p = D_801143B0; int r = *p; *p = a0; return r; }

void func_80096DE0(s32 arg0) {
    func_8008B3C4(3, arg0);
}
