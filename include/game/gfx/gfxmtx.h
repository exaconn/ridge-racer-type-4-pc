#ifndef RR_GAME_GFX_GFXMTX_H
#define RR_GAME_GFX_GFXMTX_H

#include "types.h"
#include "rr/lighting_types.h"

/* Direction components are copied as halfword bit patterns into the GTE matrix. */
typedef struct {u16 x, y, z, reserved06;} LightDirectionBits8;
extern u8 D_8009F924[4];
extern PreviewColor4 D_8009F928[3];
extern LightDirectionBits8 D_8009F934[3];

extern char StrSizeEqLdFmt[];
void SetLightColorsScaled(int a0);
void SetLightColorsFromRow(s32 index);
enum {
    MENU_TRANSITION_BADGES = 1,
    MENU_TRANSITION_IMAGES = 2,
    MENU_TRANSITION_ALL = MENU_TRANSITION_BADGES | MENU_TRANSITION_IMAGES
};
/* Return readiness bits for both groups; flags selects which groups to scan.
 * Unselected groups and groups with a nonpositive row count are ready. */
int StepMenuCameraTransition(int flags);
int AllocBadgeRows(void *a0);

RR_ASSERT_SIZE(LightDirectionBits8, 8);

#endif /* RR_GAME_GFX_GFXMTX_H */
