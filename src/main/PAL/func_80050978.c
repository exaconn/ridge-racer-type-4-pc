#include "types.h"

typedef struct {
    u32 flags00;
    s32 bank04;
    s32 voice08;
    s32 count0C;
    s32 level10;
    s32 program14;
    s32 previous18;
    s32 reserved1C;
    s16 level20;
    s16 gain22;
    s16 pan24;
    s16 reserved26;
    u8 pad28[0x5DC - 0x28];
    s16 programs5DC[2][5];
    u16 duration5F0;
    u16 gain5F2;
    u16 spread5F4;
    u16 reserved5F6;
    struct {
        s16 coarse;
        s16 fine;
    } pitch5F8[2][5];
} RrSlot620;

typedef struct {
    u8 pad00[0x30];
    s16 bank30;
    u8 pad32[6];
    RrSlot620 slots38[4];
} RrAudioOwner;

extern RrAudioOwner D_800F4DB8;

typedef struct {
    s32 bank00;
    s32 data04;
    s32 group08;
} RrAudioSetupArg;

extern void func_8008A674();
extern void func_80096434();
extern s32 func_8008AA74();
extern s32 func_8008AEC4();
extern void func_8008B074();
extern void func_80050894();
extern void func_800850C4();

void func_80050978(RrAudioSetupArg *arg0) {
    u8 sp10[32];
    s32 i, j, k;

    if (D_800F4DB8.bank30 >= 0) {
        func_8008A674(D_800F4DB8.bank30);
        D_800F4DB8.bank30 = -1;
    }

    func_80096434(&D_800F4DB8.slots38[2], 0x620);
    func_80096434(&D_800F4DB8.slots38[3], 0x620);

    if (arg0->bank00 != -1) {
        D_800F4DB8.bank30 = func_8008AA74(arg0->bank00, -1, 0x5D000);
        D_800F4DB8.slots38[2].bank04 = D_800F4DB8.slots38[3].bank04 = D_800F4DB8.bank30;
    }

    if (arg0->data04 != -1) {
        D_800F4DB8.bank30 = func_8008AEC4(arg0->data04, (s16)D_800F4DB8.slots38[2].bank04);
        D_800F4DB8.slots38[2].bank04 = D_800F4DB8.slots38[3].bank04 = D_800F4DB8.bank30;
        func_8008B074(1);
    }

    if (arg0->group08 != -1) {
        func_80050894(arg0->group08, 2);
        func_80050894(arg0->group08, 3);
    }

    for (j = 2; j < 4; j++) {
        D_800F4DB8.slots38[j].flags00 = 1;
        D_800F4DB8.slots38[j].level20 = 0x56;
        D_800F4DB8.slots38[j].gain22 = 0x80;
        D_800F4DB8.slots38[j].pan24 = 0x40;
        D_800F4DB8.slots38[j].reserved26 = 0;
        D_800F4DB8.slots38[j].count0C = 3;
    }

    D_800F4DB8.slots38[2].voice08 = 0xE;
    D_800F4DB8.slots38[3].voice08 = 0x11;

    for (i = 2; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            func_800850C4((s16)D_800F4DB8.slots38[i].bank04, D_800F4DB8.slots38[i].programs5DC[0][j], 0, sp10);
            k = (i & 1) * 2 + 0x3C;
            D_800F4DB8.slots38[i].pitch5F8[0][j].coarse = k - sp10[12];
            D_800F4DB8.slots38[i].pitch5F8[0][j].fine = (sp10[13] + sp10[12]) << 7;
            func_800850C4((s16)D_800F4DB8.slots38[i].bank04, D_800F4DB8.slots38[i].programs5DC[1][j], 0, sp10);
            D_800F4DB8.slots38[i].pitch5F8[1][j].coarse = k - sp10[12];
            D_800F4DB8.slots38[i].pitch5F8[1][j].fine = (sp10[13] + sp10[12]) << 7;
        }
    }
}
