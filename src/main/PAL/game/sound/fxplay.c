#include "types.h"
#include "m2c_macros.h"
#include "game/camera/campath.h"
#include "game/sound/sndvab.h"
#include "psyq/libsnd/ssopenq.h"
#include "game/sound/fxramp2.h"
#include "game/sound/sndvab.h"
#include "psyq/libsnd/sspause.h"
#include "psyq/libsnd/sspause2.h"
#include "psyq/libspu/smalloc2.h"
#include "rr/bios_devices.h"

extern int func_80053A9C();
extern struct { s32 a, b, c; } D_800A0CD8[];
extern struct {
    char pad[0x24]; s16 h[1];
    char p2[0x2858 - 0x26]; s32 w[1];
    char p3[0x2864 - 0x285C]; s32 x2864, x2868, x286C;
    char p4[0x287C - 0x2870]; s32 x287C;
} SoundState;
extern int func_80053AF4();
extern s16 D_800F761C;

void func_80053BB4(int a0);
void func_80053CB0(void);
void func_80053CD4(void);
void func_80053CFC(void);

void func_80053BB4(int a0) {
    int arr[2] = {0x6C, 0x7F};
    if (SoundState.x2868 != a0) {
        func_80053CB0();
        SoundState.x2868 = a0;
        SoundState.x2864 = (s16)SsSeqOpen(SoundState.w[a0], SoundState.h[a0 + 2]);
        SsSetMarkCallback((s16)SoundState.x2864, 0, func_80053DE8);
        ReverbReset();
        SetReverbDepthBoth(0x32);
        SsSeqPlay((s16)SoundState.x2864, 1, 0);
        func_80053A9C((s16)arr[a0]);
        func_80096434(&SoundState.x286C, 0x1C);
        SoundState.x287C = D_800A0CD8[a0].a;
        func_8005B190();
    }
}

void func_80053CB0(void) {
    func_80053AF4(0, 0);
}

void func_80053CD4(void) {
    func_800822D0(D_800F761C);
}

void func_80053CFC(void) {
    func_80082440(D_800F761C);
}
