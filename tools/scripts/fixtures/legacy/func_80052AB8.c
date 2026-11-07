#include "types.h"
#include "m2c_macros.h"

extern int CdAudioFade();

void CdAudioMute(void) {
    CdAudioFade(0, 0);
}
