#include "types.h"
#include "m2c_macros.h"
#include "psyq/libpad/padctl.h"
#include "game/car/carstat.h"
#include "rr/bios_devices.h"
#include "rr/channel_init.h"
#include "rr/runtime_settings.h"
#include "rr/race_setup.h"

extern int D_800F3A38[];
extern char D_8010C170[];
extern char D_8010C1A0[];
extern unsigned short D_8009FFEC[][8];
extern unsigned short D_800A006C[][8];
extern unsigned short D_800A00EC[][8];
extern unsigned short D_800F2F78[][3][8];
extern unsigned short D_800F4BF8[][2][8];

void func_8004A5C0(int a, int b, int c, int d);

void func_8004A55C(void) {
    func_80096434(D_800F3A38, 0xB8);
    func_80096434(D_8010C170, 0x30);
    D_800F3A38[4] = -1;
    D_800F3A38[27] = -1;
    PadInitDirect(D_8010C1A0, D_8010C1A0 + 0x28);
    func_80097854();
}

void func_8004A5C0(int a, int b, int c, int d) {
    int i;
    for (i = 0; i < 8; i++) {
        D_800F2F78[a][0][i] = D_8009FFEC[b][i];
        D_800F2F78[a][1][i] = D_800A006C[c][i];
        D_800F2F78[a][2][i] = D_800A00EC[d][i];
        D_800F4BF8[0][0][i] = D_800A00EC[b][i];
        D_800F4BF8[0][1][i] = D_800A006C[c][i];
    }
}

void func_8004A678(void) {
    func_8004A5C0(0, M2C_FIELD(&D_800AD528, s16 *, 0), M2C_FIELD(&D_800AD528, s16 *, 0x14), M2C_FIELD(&D_800AD528, s8 *, 0x2B));
    func_8004A5C0(1, M2C_FIELD(&D_800AD528, s16 *, 4), M2C_FIELD(&D_800AD528, s16 *, 0x24), M2C_FIELD(&D_800AD528, s8 *, 0x2F));
}
