#include "rr/overlay665_calibration.h"

void ovl665_func_80115798(void)
{
    ClearScreenOverlayState();
    MenuCursor = 0;
    InitBadgeRow(0, 12, 304, 296, 88, 0, 0);
    BadgeRows[0].half02 = 1;
    func_8002AF8C(1, 1, 0);
    func_8001E84C(ovl665_D_801148B4, Options.display.x34, Options.display.y38);
}
