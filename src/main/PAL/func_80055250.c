/* RODATA 0x800111b0 0x1c */
#include "types.h"
extern int func_80054EF4();
extern int func_80055040();
extern int func_800550A0();
extern int func_80055100();
extern int func_80055160();
extern int func_800551C0();
int func_80055250(int a0, int a1) {
    switch (a0) {
    case 0: return func_80054EF4(a1, 0);
    case 1: return func_80054EF4(a1, 1);
    case 2: return func_80055040(a1);
    case 3: return func_800550A0(a1);
    case 4: return func_80055100(a1);
    case 5: return func_80055160(a1);
    case 6: return func_800551C0(a1);
    }
}
