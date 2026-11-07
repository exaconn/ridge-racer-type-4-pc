#include "types.h"

extern int SeqFadeVolume();

void SeqMuteVolume(void) {
    SeqFadeVolume(0, 0);
}
