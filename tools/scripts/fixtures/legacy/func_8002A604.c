#include "types.h"
#include "m2c_macros.h"

extern int func_80096434();
extern M2C_UNK ScreenFadeState;
extern M2C_UNK MenuFrameState;

void ClearScreenOverlayState(void) {
    func_80096434(&MenuFrameState, 0x20);
    func_80096434(&ScreenFadeState, 0xC);
}
