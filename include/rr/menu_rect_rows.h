#ifndef RR_MENU_RECT_ROWS_H
#define RR_MENU_RECT_ROWS_H
#include "rr/layout.h"

/* Six-line menu rectangle: initialization and animation share one record. */
typedef struct {
    s16 parameter00, restart02;
    s16 x, y, width, height;
    s16 phase, tick;
} MenuRectRow10;
extern MenuRectRow10 *Rect16Rows;
typedef char rr_menu_row_address_fits[(sizeof(unsigned long)>=sizeof(void *))?1:-1];
RR_ASSERT_SIZE(MenuRectRow10,0x10);
RR_ASSERT_OFFSET(MenuRectRow10,x,4);
RR_ASSERT_OFFSET(MenuRectRow10,phase,12);
#endif
