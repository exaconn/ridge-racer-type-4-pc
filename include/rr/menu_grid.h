#ifndef RR_MENU_GRID_H
#define RR_MENU_GRID_H

#include "rr/layout.h"

/* Two words copied with unaligned loads/stores; both are captured first. */
typedef struct {
    u32 first, second;
} __attribute__((packed)) GridRow8;

typedef struct {
    s32 w00;
    s16 width04, height06, step_x08, step_y0A;
    s32 x0C, y10, z14, unk18, rot_z1C, rot_y20, rot_x24, unk28;
    s16 count2C;
    u8 columns2E, rows2F;
    u8 u30, v31, du32, dv33;
    u16 tpage34, clut36;
} MenuGrid38;

extern MenuGrid38 *MenuGridRecords;
extern GridRow8 D_8009FDA8[];

/* The PAL routine captures this RAM page and reloads its pointer cell after
 * record writes. Model the cell at its byte offset, retaining native width. */
typedef struct {
    u8 reserved00[0x3A28];
    MenuGrid38 *records3A28;
} MenuGridPointerPage;
RR_ASSERT_OFFSET(MenuGridPointerPage, records3A28, 0x3A28);

/* Index and layout select accessible records/rows; their byte offsets must fit
 * signed32. Count is nonzero and abs(count)<=6, so all original divisions are
 * defined. X/Y narrow to signed16 before division. Copies all eight layout bytes
 * after publishing the dimensions and steps, then scales bytes 0x32/0x33 in place.
 * Pointer slots retain full native widths; the PAL slot is at 0x800F3A28. */
void InitMenuGrid(s32 index, s32 layout, s32 count, s32 x, s32 y);

RR_ASSERT_SIZE(GridRow8, 8);
RR_ASSERT_OFFSET(GridRow8, second, 4);
RR_ASSERT_SIZE(MenuGrid38, 0x38);
RR_ASSERT_OFFSET(MenuGrid38, width04, 4);
RR_ASSERT_OFFSET(MenuGrid38, height06, 6);
RR_ASSERT_OFFSET(MenuGrid38, step_x08, 8);
RR_ASSERT_OFFSET(MenuGrid38, step_y0A, 0xA);
RR_ASSERT_OFFSET(MenuGrid38, count2C, 0x2C);
RR_ASSERT_OFFSET(MenuGrid38, columns2E, 0x2E);
RR_ASSERT_OFFSET(MenuGrid38, rows2F, 0x2F);
RR_ASSERT_OFFSET(MenuGrid38, u30, 0x30);

#endif
