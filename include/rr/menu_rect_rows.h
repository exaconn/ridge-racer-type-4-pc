#ifndef RR_MENU_RECT_ROWS_H
#define RR_MENU_RECT_ROWS_H
#include "rr/layout.h"

/* Observed draw order: vertical sides, horizontal sides, then diagonals.
 * These descriptive names are inferred; no original identifiers survive. */
enum {
    MENU_RECT_INITIALIZE = 0,
    MENU_RECT_VERTICAL_LINES = 1,
    MENU_RECT_HORIZONTAL_LINES = 2,
    MENU_RECT_FIRST_DIAGONAL = 3,
    MENU_RECT_SECOND_DIAGONAL = 4,
    MENU_RECT_COMPLETE = 5,
    MENU_RECT_WAIT_FOR_RESTART = 8
};

/* Six-line menu rectangle: initialization and animation share one record. */
typedef struct {
    s16 parameter00, restart02;
    s16 x, y, width, height;
    s16 phase, tick;
} MenuRectRow10;
/* Full-width CPU coordinates; columns retain the target's stack layout. */
typedef struct {s32 x[4], y[4];} MenuRectCorners;
RR_ASSERT_SIZE(MenuRectCorners, 32);
RR_ASSERT_OFFSET(MenuRectCorners, y, 16);

/* Six directed lines: two vertical, two horizontal, then both diagonals. */
typedef struct {u8 first, last;} MenuRectEdgeCorners;
extern MenuRectEdgeCorners MenuRectLineCorners[6];
extern u8 MenuRectLineIntensitySlots[6];
RR_ASSERT_SIZE(MenuRectEdgeCorners, 2);
RR_ASSERT_OFFSET(MenuRectEdgeCorners, last, 1);
extern MenuRectRow10 *Rect16Rows;
typedef char rr_menu_row_address_fits[(sizeof(unsigned long)>=sizeof(void *))?1:-1];
RR_ASSERT_SIZE(MenuRectRow10,0x10);
RR_ASSERT_OFFSET(MenuRectRow10,x,4);
RR_ASSERT_OFFSET(MenuRectRow10,phase,12);
#endif
