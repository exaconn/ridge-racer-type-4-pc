#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "rr/menu_label_render.h"
typedef struct { int x; } __attribute__((packed)) U4;
extern U4 HudPlayerLabelGlyphs[];
extern int NumLaps;

void DrawLapCounter(int a0, int a1) {
    U4 buf;
    int s4, s5, a, i;
    if (a0 == 0) {
        s5 = 0xC;
        s4 = &CurrentDrawBuf->buffer.ordering70.words[0];
    } else {
        s4 = &CurrentDrawBuf->buffer.secondaryOrderingB70.words[0];
        s5 = ((a0 - 1) << 7) + 0xC;
    }
    a = *(int *)SCRATCHPAD;
    i = 0;
    do {
        buf = HudPlayerLabelGlyphs[i];
        a = AddSpritePrim(s4, a, 0xEC + i * 0x38, s5, &buf, 0x7EF8, 0);
        i++;
    } while (i < 2);
    buf = HudPlayerLabelGlyphs[2];
    i = 0;
    if (a1 <= 0) {
        a1 = 1;
    }
    do {
        if (i == 0) {
            *(char *)&buf = a1 * 0xC;
        }
        if (i == 1) {
            *(char *)&buf = NumLaps * 0xC;
        }
        a = AddSpritePrim(s4, a, 0x118 + i * 0x14, s5, &buf, 0x7EF8, 0);
        i++;
    } while (i < 2);
    *(void **)SCRATCHPAD = AddDrawModePrim(s4, a, 0x25);
}
