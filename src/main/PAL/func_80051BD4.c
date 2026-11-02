#include "types.h"

typedef struct {
    s32 unk00;
    s16 unk04;
    s16 unk06;
    s32 unk08[5];
} Rec1C;

typedef struct {
    char pad0[0x18C4];
    s32 pos18C4[3];
    char pad1[0x18E4 - 0x18D0];
    s32 unk18E4;
    s32 unk18E8;
    Rec1C rec[2];
} State800F4DB8;

typedef struct {
    char pad0[0x94];
    s16 unk94;
    char pad1[0xC0 - 0x96];
    s32 unkC0;
    s32 unkC4;
    char pad2[0x12C - 0xC8];
} Control12C;

typedef struct {
    char pad0[0x20];
    s32 pos20[3];
    char pad1[0x184 - 0x2C];
    s32 first184;
    s32 second188;
    char pad2[0x1F4 - 0x18C];
    Control12C sub1F4;
} Vehicle320;

extern State800F4DB8 D_800F4DB8;
extern s32 D_800F2EB8;
extern struct {
    char pad0[0x10];
    s32 count10;
} D_800AC650;
extern s32 D_800AC660;
extern Vehicle320 D_80107198[];
extern s32 D_800F6748;

extern s32 func_80090758(s32);
extern void func_80050E94(s32, s32, s32, s32);

void func_80051BD4(s32 arg0) {
    s32 d[4];
    s32 min_dist[2];
    s32 min_idx[2];
    s32 prev_idx[2];
    s32 slot[2];
    s32 i;
    s32 dist;
    /* ASM_BARRIER: allocate car pointer to v1 to match original register allocation */
    register Vehicle320 *car asm("$3");
    Control12C *sub;

    prev_idx[0] = D_800F4DB8.rec[0].unk04;
    prev_idx[1] = D_800F4DB8.rec[1].unk04;

    switch (arg0) {
    case 0:
        min_idx[1] = -1;
        min_idx[0] = -1;
        min_dist[1] = D_800F2EB8;
        min_dist[0] = D_800F2EB8;
        for (i = 0; i < D_800AC660; i++) {
            car = &D_80107198[i];
            dist = (car->first184 + car->second188) - D_800F4DB8.unk18E4;
            dist %= D_800F2EB8;
            if (dist >= D_800F2EB8 / 2) {
                dist -= D_800F2EB8;
            }
            if (dist < -D_800F2EB8 / 2) {
                dist += D_800F2EB8;
            }
            if (dist < 0) {
                dist = -dist;
            }
            if (dist < min_dist[0]) {
                min_dist[1] = min_dist[0];
                min_idx[1] = min_idx[0];
                min_dist[0] = dist;
                min_idx[0] = i;
            } else if (dist < min_dist[1]) {
                min_dist[1] = dist;
                min_idx[1] = i;
            }
        }
        goto update_slots;
    case 1:
        min_dist[1] = 0x7FFFFFFF;
        min_dist[0] = 0x7FFFFFFF;
        min_idx[1] = -1;
        min_idx[0] = -1;
        for (i = 0; i < D_800AC650.count10; i++) {
            d[0] = (D_80107198[i].pos20[0] - D_800F4DB8.pos18C4[0]) / 16;
            d[1] = (D_80107198[i].pos20[1] - D_800F4DB8.pos18C4[1]) / 16;
            d[2] = (D_80107198[i].pos20[2] - D_800F4DB8.pos18C4[2]) / 16;
            dist = func_80090758(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
            if (dist < min_dist[0]) {
                min_dist[1] = min_dist[0];
                min_idx[1] = min_idx[0];
                min_dist[0] = dist;
                min_idx[0] = i;
            } else if (dist < min_dist[1]) {
                min_dist[1] = dist;
                min_idx[1] = i;
            }
        }
    update_slots:
        slot[0] = 0;
        slot[1] = 1;
        if (min_idx[0] == D_800F4DB8.rec[1].unk04) {
            slot[0] = 1;
            slot[1] = 0;
        }
        if (min_idx[1] == D_800F4DB8.rec[0].unk04) {
            slot[0] = 1;
            slot[1] = 0;
        }
        D_800F4DB8.rec[slot[0]].unk04 = min_idx[0];
        D_800F4DB8.rec[slot[1]].unk04 = min_idx[1];
        break;
    }

    for (i = 0; i < 2; i++) {
        D_800F4DB8.rec[i].unk06 = 0;
        if (prev_idx[i] != D_800F4DB8.rec[i].unk04) {
            D_800F4DB8.rec[i].unk06 = 1;
        }
    }

    sub = &D_80107198[min_idx[0]].sub1F4;
    if (sub->unk94 != 0) {
        func_80050E94(1, sub->unkC0, sub->unkC4, 1);
        D_800F6748 = slot[0] + 2;
    } else {
        sub = &D_80107198[min_idx[1]].sub1F4;
        if (sub->unk94 != 0) {
            func_80050E94(1, sub->unkC0, sub->unkC4, 1);
            D_800F6748 = slot[1] + 2;
        } else {
            func_80050E94(0, 0, 0, 1);
        }
    }
}
