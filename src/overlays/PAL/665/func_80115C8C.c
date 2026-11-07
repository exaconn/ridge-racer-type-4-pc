#include "types.h"
extern void ClearScreenOverlayState(void);
extern void InitBadgeRow(s32, s32, s32, s32, s32, s32, s32);
extern void func_8002AF8C(s32, s32, s32);
extern u8 D_800ACC68;
extern s16 *BadgeRows;
void ovl665_func_80115C8C(void)
{
    ClearScreenOverlayState();
    D_800ACC68 = 2;
    InitBadgeRow(0, 12, 304, 296, 88, 0, 0);
    BadgeRows[1] = 1;
    func_8002AF8C(1, 1, 0);
}
