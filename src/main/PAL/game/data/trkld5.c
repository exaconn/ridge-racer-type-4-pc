/* RODATA 0x80011088 0x14 */
#include "rr/track_resources.h"
#include "game/data/cdfile2.h"
#include "game/data/trkld1.h"
#include "game/data/trkld1.h"
#include "game/data/trkld5.h"
#include "game/course/trkres2.h"
#include "game/data/cdfile1.h"
#include "game/data/cdfile2.h"
#include "game/data/selection.h"

typedef struct { unsigned char a, b, c, d; } T;

extern short D_800ABEC8;
extern void *LoadCursor;
extern short D_800AD520;
extern short D_800F3914;

int func_8004E260()
{
    int r;
    int r2;
    int r3;
    switch (D_800A021C) {
    case 0:
        r = CdFileReadStart(GetR4FileIndex(2, D_800F3914 + 0x16), D_8010C1F0);
        if (r == 0) goto out;
        goto inc;
    case 1:
        r2 = CdFileBytesLoaded();
        if (r2 != 0) {
            LoadTrackResource((RrTrackResourceFile *)D_8010C1F0);
            D_8010C1F0 += AlignUp4(r2);
            D_800A021C += 1;
        }
        break;
    case 2:
        r3 = func_8004D628(D_800F3914, D_800AD520, D_800ABEC8);
        goto chk;
    case 3:
        r3 = func_8004D994(D_800F3914, D_800AD520);
    chk:
        if (r3 != 0) {
    inc:
            D_800A021C += 1;
        }
        break;
    case 4:
        D_800A021C = 0;
        LoadCursor = D_8010C1F0;
        return 1;
    }
    r = 0;
out:
    return r;
}

int func_8004E3B0(int i) {
    T *base = D_800F4D98;
    T *t = base + i;
    int r = 0;
    if (t->c != 0x27) {
        r = t->b;
        if (t->c == 0x13) {
            r += 4;
        }
    } else if (t->b == 0) {
        r = (t->a == 2) * 8;
    }
    return r;
}
