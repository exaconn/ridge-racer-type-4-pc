#include "types.h"
#include "rr/bios_devices.h"
#include "rr/spu.h"
#include "rr/spu_tone_snapshot.h"

struct B { char b[0x5BC]; };

struct E { struct B x; char pad[1568 - 0x5BC]; };

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

typedef struct {
    s32 bank00;
    s32 data04;
    s32 group08;
} RrAudioSetupArg;

extern struct { char pad[0x614]; volatile u16 m[1][5]; char r[2]; } D_800F4DB8[];
extern struct E D_800F4E2C[];
extern void func_8008A674();
extern s32 func_8008AA74();
extern void func_8008B074();

void func_80050844(int a0, int a1, int a2, int a3);
void func_80050894(struct B *a0, int a1);
void func_80050978(RrAudioSetupArg *arg0);

void func_80050844(int a0, int a1, int a2, int a3) {
    u16 old = D_800F4DB8[a3].m[a1][a0];
    if (a2 >= 0) D_800F4DB8[a3].m[a1][a0] = a2;
}

void func_80050894(struct B *a0, int a1) {
    D_800F4E2C[a1].x = *a0;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(RrAudioOwner (*))&D_800F4DB8)

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

#undef D_800F4DB8
