/* GCC_VERSION: 2.7.2 */
#include "types.h"
int func_8008B394(int, int *);          /* extern */
int func_8008BB14(int *, int);          /* extern */
extern int D_800A6370;
extern s32 D_800A6390;
extern s32 *D_800A6394;
extern int func_8008BA7C;
extern int func_8008BAE8;

int *func_8008BA24(void) {
    *D_800A6394 = 0x100;
    D_800A6390 = 0;
    func_8008BB14(&D_800A6370, 8);
    func_8008B394(0, &func_8008BA7C);
    return &func_8008BAE8;
}
