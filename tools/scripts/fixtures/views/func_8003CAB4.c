#include "rr/race_types.h"

extern s16 D_800F4D44;

extern void func_8003CAF4();

extern unsigned short D_8009F914[];

extern unsigned short D_800AD50A;

void func_8003CAB4(void) {
    func_8003CAF4();
    D_800AD50A = D_8009F914[D_800F4D44];
}
