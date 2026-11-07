/* RODATA 0x8001085c 0x18 */
#include "game/data/cdfile2.h"
#include "game/data/ldscn4.h"
#include "game/data/ldscn5.h"
#include "game/data/cdfile1.h"
#include "game/data/cdfile2.h"
#include "rr/course_choices.h"
extern void func_800436E8();
extern unsigned char D_800F4D7C;

int func_8003BEA8(int a) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(0, a + 1), LoadCursor) != 0) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded() != 0) {
            func_800436E8(LoadCursor);
    case 3:
            if (CdFileReadStart(GetR4FileIndex(0, a + 10), LoadCursor) != 0) {
                D_800F4D7C = 4;
            }
        }
        break;
    case 4:
        if (CdFileBytesLoaded() != 0) {
            D_800AC64C = LoadCursor;
            RelocateOffsetTable(LoadCursor);
            D_800F4D7C = 5;
    case 5:
            r = 1;
        }
        break;
    }
    return r;
}
