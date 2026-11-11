#ifndef RR_MENU_GLYPH_TYPES_H
#define RR_MENU_GLYPH_TYPES_H
#include "rr/layout.h"

/* Controller commands/stages inferred from the actual transition graph. */
enum {
    MENU_GLYPH_RECTANGLE_DELAY = -7,
    MENU_GLYPH_START_RECTANGLE_DELAY = -6,
    MENU_GLYPH_WAIT_RECTANGLE = -5,
    MENU_GLYPH_INIT_RECTANGLE = -4,
    MENU_GLYPH_IDLE = -3,
    MENU_GLYPH_RESET = -2,
    MENU_GLYPH_FADE_BACKGROUND = -1,
    MENU_GLYPH_PREPARE_IMAGE = 0,
    MENU_GLYPH_BLEND_IMAGE = 1,
    MENU_GLYPH_DISPLAY_IMAGE = 2
};

/* A menu image row owns its live CPU image pointer. PAL uses a 0x1C-byte
 * record; a native host retains the complete pointer and its alignment. */
typedef struct {
    u8 frameIndex00, frameCount01;
    s8 control02;
    u8 tick03, reserved04, frameDuration05, imageSectors06, glyph07;
    s16 x08, y0A, w0C, h0E;
    u16 texturePage10, clut12;
    u8 u14, v15, reserved16[2];
    u8 *imageBase18;
} MenuGlyphRow1C;

typedef struct {
    s16 x00, y02, w04, h06;
    u16 page08, clut0A;
    u8 u0C, v0D, frameCount0E, frameDuration0F, imageSectors10, reserved11[3];
} MenuGlyphPreset14;

RR_ASSERT_OFFSET(MenuGlyphRow1C, frameIndex00, 0);
RR_ASSERT_OFFSET(MenuGlyphRow1C, control02, 2);
RR_ASSERT_OFFSET(MenuGlyphRow1C, tick03, 3);
RR_ASSERT_OFFSET(MenuGlyphRow1C, glyph07, 7);
RR_ASSERT_OFFSET(MenuGlyphRow1C, x08, 8);
RR_ASSERT_OFFSET(MenuGlyphRow1C, texturePage10, 0x10);
RR_ASSERT_OFFSET(MenuGlyphRow1C, u14, 0x14);
RR_ASSERT_OFFSET(MenuGlyphRow1C, imageBase18, 0x18);
#if defined(mips)
RR_ASSERT_SIZE(MenuGlyphRow1C, 0x1C);
#else
RR_ASSERT_SIZE(MenuGlyphRow1C, 0x18 + sizeof(void *));
#endif
RR_ASSERT_SIZE(MenuGlyphPreset14, 0x14);
RR_ASSERT_OFFSET(MenuGlyphPreset14, frameCount0E, 0xE);
RR_ASSERT_OFFSET(MenuGlyphPreset14, imageSectors10, 0x10);
#endif
