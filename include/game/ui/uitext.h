#ifndef RR_GAME_UI_UITEXT_H
#define RR_GAME_UI_UITEXT_H

#include "types.h"

typedef struct {
    signed char f0, f1;
    unsigned char f2, f3;
    union {
        unsigned short s[6];
        struct {
            short x, y;
            unsigned short u, v, cx, cy;
        } coords;
    } geom;
    unsigned char b0, b1;
    char reserved[2];
} ImageRow20;

extern ImageRow20 *ImageRows;

int DrawUiRow(s32 arg0);
void DrawUiBox(s32 x, s32 y, s32 width, s32 height, s32 red, s32 green, s32 blue);
int AllocImageRows(void *a);
void InitImageRow(int i, int a, int b, int j);
int UpdateImageRow(int a0);
void DrawImageRow(int a0, int a1, int a2, int a3);

#endif /* RR_GAME_UI_UITEXT_H */
