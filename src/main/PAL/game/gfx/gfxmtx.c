#include "rr/track_lighting.h"
#include "rr/matrix_workspace.h"
#include "psyq/libgte/gtemat7.h"
#include "game/gfx/gfxmtx.h"
#include "game/menu/mnrows.h"
#include "game/ui/uidraw.h"
#include "game/ui/uitext.h"
#include "rr/badge_rows.h"
#include "rr/bios_devices.h"
#include "rr/audio_cd_controller.h"

void SetLightColorsScaled(int a0)
{
    MatrixWords32 colors;
    int i;
    int x;

    for (i = 0; i < 3; i++) {
        LightMatrix.matrix.fields.rotation00[i][0] = D_8009F934[i].x;
        LightMatrix.matrix.fields.rotation00[i][1] = D_8009F934[i].y;
        LightMatrix.matrix.fields.rotation00[i][2] = D_8009F934[i].z;
        colors.fields.rotation00[0][i] = D_8009F928[i].red << 4;
        colors.fields.rotation00[1][i] = D_8009F928[i].green << 4;
        colors.fields.rotation00[2][i] = D_8009F928[i].blue << 4;
    }
    x = D_8009F924[0] * a0;
    {
        int y = D_8009F924[1] * a0;
        x >>= 8;
        y >>= 8;
        SetBackColor(x, y, (D_8009F924[2] * a0) >> 8);
    }
    SetColorMatrix(&colors);
}

void SetLightColorsFromRow(s32 index)
{
    MatrixWords32 colors;
    u16 *s0;
    int i = 0;
    unsigned short *a3 = LightMatrix.halves;
    unsigned char *a1;
    unsigned short *a2;
    TrackLighting2C *base = TrackLightTable;
    a1 = (unsigned char *)(index * sizeof(*base) + (unsigned long)base);
    a2 = (unsigned short *)a1;
L:
    {
        u16 *v;
        s0 = colors.halves;
        v = s0 + i;
        i++;
        a3[0] = a2[8];
        a3[1] = a2[9];
        a3[2] = a2[10];
        a2 += 4;
        v[0] = a1[4] << 4;
        v[3] = a1[5] << 4;
        v[6] = a1[6] << 4;
        a3 += 3;
        a1 += 4;
    }
    if (i < 3) goto L;
    {
        TrackLighting2C *current = TrackLightTable;
        unsigned char *q = (unsigned char *)(index * sizeof(*current) + (unsigned long)current);
        SetBackColor(q[0x28], q[0x29], q[0x2A]);
    }
    SetColorMatrix(&colors);
}

int StepMenuCameraTransition(int flags)
{
    u16 ready = MENU_TRANSITION_ALL;
    int index;
    int rowCount;
    const BadgeRow1C *badge;
    if (flags & MENU_TRANSITION_BADGES) {
        index = 0;
        rowCount = BadgeRowCount;
        if (rowCount > 0) {
            int settledState = 0xB;
            int limit = rowCount;
            badge = BadgeRows;
            do {
                short state = badge->state10;
                if (state != 0 && state != settledState) {
                    ready &= ~MENU_TRANSITION_BADGES;
                    break;
                }
                index++;
                badge++;
            } while (index < limit);
        }
    }
    if (flags & MENU_TRANSITION_IMAGES) {
        const ImageRow20 *image;
        index = 0;
        rowCount = ImageRowCount;
        if (rowCount > 0) {
            int settledState = 5;
            int limit = rowCount;
            image = ImageRows;
            do {
                signed char state = image->f1;
                if (state != 0) {
                    index++;
                    if (state != settledState) {
                        ready &= ~MENU_TRANSITION_IMAGES;
                        break;
                    }
                } else index++;
                image++;
            } while (index < limit);
        }
    }
    return ready;
}

int AllocBadgeRows(void *a0) {
    int n = BadgeRowCount * 28;
    BadgeRows = (BadgeRow1C *)a0;
    PsyqBzero(a0, n);
    DebugPrintStub(StrSizeEqLdFmt, n);
    return n;
}

