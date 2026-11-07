#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define ImageRows ((E *)ImageRows)

#undef ImageRows

/* legacy typing of shared globals in this part of the unit */
#define ImageRows ((E_1 *)ImageRows)

#undef ImageRows

/* legacy typing of shared globals in this part of the unit */
#define ImageRows ((E_2 *)ImageRows)

#undef ImageRows

extern s32 rcos();

extern int ImageRows;

extern int UpdateRect16Row();

int UpdateImageRow(int a0) {
    int s1;
    int s2;
    int state;
    unsigned short val;
    int pad[16];

    s2 = 0;
    state = ImageRows[a0].f1;
    s1 = ImageRows[a0].f2;
    switch (state) {
    case -1:
        if (UpdateRect16Row(ImageRows[a0].f3) == 1) {
            ImageRows[a0].f1 = 0;
        }
        break;
    case 0:
        ImageRows[a0].f2 = 0;
        ImageRows[a0].f1 = 1;
    case 1:
        s2 = 1;
        val = (s1 << 7) / 30;
        DrawImageRow(a0, 0, val, 0);
        if (s1 >= 30) {
            ImageRows[a0].f2 = 0;
            ImageRows[a0].f1 = 2;
        } else {
            ImageRows[a0].f2++;
        }
        break;
    case 2:
    case 4:
    default:
        s2 = 2;
        DrawImageRow(a0, 0, 0x80, 0);
        break;
    case 3:
        val = rcos((s1 << 10) / 30) / 32;
        DrawImageRow(a0, 1, val, 0);
        if (s1 >= 30) {
            ImageRows[a0].f1 = 5;
            ImageRows[a0].f2 = 0;
        } else {
            ImageRows[a0].f2++;
        }
        break;
    case 5:
        break;
    case 6:
        s2 = 1;
        val = ((30 - s1) << 7) / 30;
        DrawImageRow(a0, 0, val, 0);
        if (s1 >= 30) {
            ImageRows[a0].f2 = 0;
            ImageRows[a0].f1 = 7;
        } else {
            ImageRows[a0].f2++;
        }
        break;
    case 7:
        s2 = 1;
        DrawImageRow(a0, 0, 0, 0);
        break;
    }
    return s2;
}
