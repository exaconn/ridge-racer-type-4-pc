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
        LightMatrix.matrix.fields.rotation00[i][0] = MenuCarLightVectors[i].x;
        LightMatrix.matrix.fields.rotation00[i][1] = MenuCarLightVectors[i].y;
        LightMatrix.matrix.fields.rotation00[i][2] = MenuCarLightVectors[i].z;
        colors.fields.rotation00[0][i] = MenuCarLightColors[i].red << 4;
        colors.fields.rotation00[1][i] = MenuCarLightColors[i].green << 4;
        colors.fields.rotation00[2][i] = MenuCarLightColors[i].blue << 4;
    }
    x = MenuCarDefaultAmbient[0] * a0;
    {
        int y = MenuCarDefaultAmbient[1] * a0;
        x >>= 8;
        y >>= 8;
        SetBackColor(x, y, (MenuCarDefaultAmbient[2] * a0) >> 8);
    }
    SetColorMatrix(&colors);
}

void SetLightColorsFromRow(s32 index)
{
    MatrixWords32 colors;
    int i;
    const PreviewLighting40 *row = &TrackLightTable[index].lighting04;
    for (i = 0; i < 3; i++) {
        LightMatrix.matrix.fields.rotation00[i][0] = row->matrix.fields.vectors[i].x;
        LightMatrix.matrix.fields.rotation00[i][1] = row->matrix.fields.vectors[i].y;
        LightMatrix.matrix.fields.rotation00[i][2] = row->matrix.fields.vectors[i].z;
        colors.fields.rotation00[0][i] = row->matrix.fields.colors[i].red << 4;
        colors.fields.rotation00[1][i] = row->matrix.fields.colors[i].green << 4;
        colors.fields.rotation00[2][i] = row->matrix.fields.colors[i].blue << 4;
    }
    SetBackColor(row->ambient[0], row->ambient[1], row->ambient[2]);
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
    int n = BadgeRowCount * sizeof(BadgeRow1C);
    BadgeRows = (BadgeRow1C *)a0;
    PsyqBzero(a0, n);
    DebugPrintStub(StrSizeEqLdFmt, n);
    return n;
}


/* Initial values of the globals this module owns. */
char StrSizeEqLdFmt[100] = {115, 105, 122, 101, 32, 61, 32, 37, 108, 100, 10};
u8 MenuCarDefaultAmbient[4] = {0x68, 0x70, 0x80};
PreviewColor4 MenuCarLightColors[3] = {{.red = 0xC8, .green = 0xC8, .blue = 0xC8}, {.red = 0xC8, .green = 0xC8, .blue = 0xC8}, {.red = 0xC8, .green = 0xC8, .blue = 0xC8}};
LightDirectionBits8 MenuCarLightVectors[3] = {{.x = 0xC0E, .y = 0x21}, {.y = 0x1000}, {.y = 0xF000}};
int BadgeRowCount;
