#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "game/demo/splash.h"
#include "rr/driver_control.h"
#include "rr/badge_rows.h"
#include "game/ui/badgespr.h"
extern int SplashLogoAlpha;

int SplashLogoUpdate(int a0) {
    int s5, cur;
    int *g = (int *)SCRATCHPAD;
    g[0] = AddTileGrid2x2(&CurrentDrawBuf->buffer.ordering70.words[703], g[0], 0, 0, 0x140, 0x200, 0, 0, 0);
    SplashLogoFrame = SplashLogoFrame + 1;
    if (SplashLogoFrame >= 3) {
        if (SplashLogoFrame == 3) {
            SetDispMask(1);
            SplashLogoAlpha = 0;
        }
        s5 = &CurrentDrawBuf->buffer.ordering70.words[0];
        cur = g[0];
        if (SplashLogoFrame < 0x56) {
            SplashLogoAlpha = SplashLogoAlpha + 3;
        } else if (SplashLogoFrame < 0x60) {
            SplashLogoAlpha = 0xFF;
        } else if (a0 == 0) {
            SplashLogoFrame = SplashLogoFrame - 1;
        } else if (SplashLogoFrame >= 0xB6) {
            return 1;
        } else {
            SplashLogoAlpha = SplashLogoAlpha - 3;
        }
        if (SplashLogoAlpha < 0) {
            SplashLogoAlpha = 0;
        } else if (SplashLogoAlpha >= 0x100) {
            SplashLogoAlpha = 0xFF;
        }
        *(void **)SCRATCHPAD = AddSpriteWithDrawMode(s5, AddSpriteWithDrawMode(s5, AddSpriteWithDrawMode(s5, cur, 0x64, 0xC4, 0x78, 0x20, 0, 0, 0x2C, 0x3FF0, 1, SplashLogoAlpha), 0xDC, 0xC4, 8, 0x10, 0, 0x20, 0x2C, 0x3FF0, 1, SplashLogoAlpha), 0x64, 0xEC, 0x7C, 0x18, 0x80, 0, 0x2C, 0x3FB0, 1, SplashLogoAlpha);
    }
    return 0;
}
