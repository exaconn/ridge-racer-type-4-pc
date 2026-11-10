#include "rr/overlay662_transition.h"

void ovl662_enter_emblem_selector(void) {
    ovl662_init_emblem_selector();
    ModeGroup = 1;
    ModeIndex = 0x46;
    SubmodeState = 0;
    UpdateScreenFade(0x10002);
}
