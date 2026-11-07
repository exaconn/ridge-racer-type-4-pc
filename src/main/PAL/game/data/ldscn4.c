/* RODATA 0x8001083c 0x1c */
#include "game/data/cdfile2.h"
#include "game/data/trkld6.h"
#include "game/gfx/fadespr.h"
#include "game/data/ldscn4.h"
#include "game/data/cdfile1.h"
#include "game/data/cdfile2.h"
#include "rr/course_choices.h"
extern void func_800436E8();
extern void func_8004E008();
extern int func_80096464();
extern int CdDirectoryBuffer;
extern int D_800F4D40;
extern unsigned char D_800F4D7C;

int func_8003BD2C(void) {
    int r = 0;
    int t;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, 0x10), LoadCursor) != 0) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded() != 0) {
            func_800436E8(LoadCursor);
            D_800F4D7C = 3;
            LoadCursor = CdDirectoryBuffer;
        }
        break;
    case 3:
        if (CdFileReadStart(GetR4FileIndex(1, 10), LoadCursor) != 0) {
            D_800F4D7C = 4;
        }
        break;
    case 4:
        t = CdFileBytesLoaded();
        if (t != 0) {
            LoadCursor += t;
            D_800F4D7C = 5;
            D_800F4D40 = LoadCursor;
            func_8004E008();
        }
        break;
    case 5:
        if (func_8004E990() == 1) {
            func_8005C13C(func_80096464() & 1);
            D_800F4D7C = 6;
    case 6:
            r = 1;
        }
        break;
    }
    return r;
}
