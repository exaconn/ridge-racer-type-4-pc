#include "rr/channels.h"
void PadSetMainMode(s32 selector, s32 first, s32 second) {
    func_800994CC(D_800ABC7C(selector), first & 0xFF, second & 0xFF);
}
