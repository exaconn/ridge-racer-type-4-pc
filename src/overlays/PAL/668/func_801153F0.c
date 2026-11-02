#include "rr/overlay668_stream.h"
extern s32 func_8008F1C4(s32);

void ovl668_func_801153F0(u8 *location) {
    u8 mode = 0x80;
    do {
        while (func_8008C140(2, location, 0) == 0) {
        }
        while (func_8008C140(14, &mode, 0) == 0) {
        }
        func_8008B154(3);
    } while (func_8008F1C4(480) == 0);
}
