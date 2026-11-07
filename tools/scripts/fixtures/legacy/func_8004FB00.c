#include "types.h"
extern int SlotKeyOffAll();
extern int func_80050E94();
extern int func_800512C4();
extern int func_80051538();
extern int func_800517F8();
extern int CdAudioMute();
extern int func_8004F9C8();
extern int func_80051B38();
extern int D_800F4DBC;
struct E { char pad[0x38]; u32 flags; char r[1568 - 0x3C]; };
extern struct E SoundState[];
void SoundStopAll(void) {
    int i;
    struct E *p;
    for (i = 0, p = SoundState; i < 4; i++, p++) {
        if (p->flags & 1) SlotKeyOffAll(i);
    }
    for (i = 0; i < 2; i++) func_80050E94(0, 0, 0, i);
    for (i = 0; i < 2; i++) func_800512C4(i, 0, 0);
    func_80051538(0, 0, 0);
    func_800517F8(5, 0x1E00, 0, 0x40);
    CdAudioMute();
    func_8004F9C8();
    func_80051B38();
    D_800F4DBC = 1;
}
