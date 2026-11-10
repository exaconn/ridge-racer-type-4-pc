#include "game/system/pad.h"
#include "rr/overlay662_pixel_dispatch.h"

void ovl662_func_80124E4C(void)
{
    GaragePixelEdit4 operation;
    u8 *pixel;
    GarageSelectionPrefix *state;
    s32 x;
    register s32 y __asm__("$4");
    register u32 coordX __asm__("$4");
    register u32 offset __asm__("$2");
    register s32 dx __asm__("$5");
    s32 dy;
    register u32 packed __asm__("$3");

    if (ovl662_func_8011E6C8(82, 71, 128, 256) == 0 &&
        (ovl662_D_80127668.words[1] & 1)) {
        if (ovl662_D_801276A8.layer23 == 0) {
            pixel = (u8 *)ovl662_D_801276A8.image38[ovl662_D_801276A8.active21];
        } else {
            pixel = (u8 *)ovl662_D_801276A8.image40[ovl662_D_801276A8.active21];
        }
        state = &ovl662_D_801276A8;
        x = (u8)state->reset08[0];
        dx = (u8)state->reset08[6];
        y = (u8)state->reset08[1];
        dy = (u8)state->reset08[7];
        x += dx;
        y += dy;
        operation.y = y;
        /* Only the initialized coordinate byte is an input at each barrier. */
        __asm__("" : : "m"(operation.y));
        offset = (u8)y;
        operation.x = x;
        __asm__("" : : "m"(operation.x));
        coordX = (u8)x;
        offset *= 32;
        offset += coordX >> 1;
        pixel += offset;
        if (coordX & 1) {
            operation.previous = *pixel >> 4;
        } else {
            operation.previous = *pixel & 15;
        }
        if (PlayerControllers[0].input00.fields.type != 0x12 &&
            (PlayerControllers[0].input00.fields.buttons & 2) && (PlayerControllers[0].input00.fields.buttons & 1)) {
            operation.replacement = 0;
        } else {
            operation.replacement = ovl662_D_801276A8.config22;
        }
        if (operation.previous != operation.replacement) {
            ovl662_func_8011D120();
            packed = operation.y * 0x100U;
            packed = operation.x | packed;
            packed |= operation.previous * 0x10000U;
            FloodFill4bpp(pixel, packed | (operation.replacement * 0x1000000U));
        }
    }
}
