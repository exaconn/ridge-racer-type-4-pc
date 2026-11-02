#include "rr/overlay_extra_labels.h"
#include "rr/badge_rows.h"

extern u8 ovl662_D_80126234[4];
extern u8 ovl662_D_80126238[4];
extern u8 ovl662_D_8012623C[3];
extern s32 ovl662_D_801272F0[3];
extern s16 D_800F4D3C;
extern RrUiUvRect D_8009FA1C[];
void func_8002B814(char *output, s32 value, s32 digitsMinusOne);
void func_8002B75C(char *destination, char *source);
char *ovl662_func_8011883C(s32 value);
void *func_80020508(u32 *, void *, s32, s32, s32, s32, s32, s32, s32);

void *ovl662_func_801183C0(u32 *ord, void *cur, s32 row)
{
    /* Only slot [1] is used; the array keeps the value in the frame at sp+44. */
    s32 time[2];
    s32 j;
    char text[8];
    s32 i;
    s32 width;
    char *p;
    s32 x, y;

    x = D_800F3AFC[row].x04;
    y = D_800F3AFC[row].y06;
    y += 32;
    width = D_800F3AFC[row].width0C;
    for (i = 0; i < 3; i++) {
        x += ovl662_D_80126234[i];
        cur = func_8001FF84(ord, cur, x, y, &D_8009FA1C[ovl662_D_8012623C[i]], 0x7FF8, 0);
    }
    for (i = 7, p = text + 7; i >= 0; i--) {
        *p-- = 0;
    }
    y -= 8;
    for (i = 0; i < 3; i++) {
        x = D_800F3AFC[row].x04 + 8;
        func_8002B814(text, i + 1, 0);
        text[1] = 0;
        cur = func_8001F5B4(ord, cur, x, y + ovl662_D_80126238[i + 1], text, 0x7FF8, 0, 0);
        time[1] = ovl662_D_801272F0[i];
        if (time[1] != -1) {
            x += ovl662_D_80126234[1];
            cur = func_8001FF84(ord, cur, x, y + ovl662_D_80126238[i + 1], &D_8009FA1C[42], 0x7FF8, 0);
            text[0] = i + 'A';
            text[1] = 0;
            cur = func_8001F5B4(ord, cur, x + (D_8009FA1C[42].w + 2), y + ovl662_D_80126238[i + 1], text, 0x7FF8, 0, 0);
            func_8002B75C(text, ovl662_func_8011883C(time[1]));
            cur = func_8001F5B4(ord, cur, x + ovl662_D_80126234[2], y + ovl662_D_80126238[i + 1], text, 0x7FF8, 1, 0);
        } else {
            cur = func_8001FD20(ord, cur, x + ovl662_D_80126234[1], y + ovl662_D_80126238[i + 1] + 8, 36, 2, 255, 255, 255);
        }
        if (i == D_800F4D3C) {
            cur = func_80020508(ord, cur, D_800F3AFC[row].x04, y + ovl662_D_80126238[i + 1] - 4, width, 24, 32, 32, 32);
        }
    }
    cur = func_8001FCC8(ord, cur, 56);
    y += 28;
    x = D_800F3AFC[row].x04;
    cur = func_8001FD20(ord, cur, x, y, width, 2, 255, 255, 255);
    y -= 28;
    for (i = 0; i < 4; i++) {
        x = D_800F3AFC[row].x04;
        for (j = 0; j < 2; j++) {
            x += ovl662_D_80126234[j + 1];
            cur = func_8001FD20(ord, cur, x, y + ovl662_D_80126238[i], 2, 16, 255, 255, 255);
        }
    }
    return cur;
}
