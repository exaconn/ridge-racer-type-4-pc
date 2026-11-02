#include "rr/selection_initializer.h"
void ovl667_func_801161A8(s32 arg0) {
    RrRaceVehicle *var_s0;
    s16 *temp_s0;
    s16 *temp_t0_2;
    s32 temp_a0;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 var_s1;
    s32 placeY,placeZ;
    /* ASM_BARRIER: preserve the original rotation publication register. */
    register s32 rotation __asm__("$2");
    RrMenuGridPoint *temp_a3_2;

    D_800F2DE4 = 0;
    D_800ABEB0 = 0;
    D_800AC64A = 0;
    D_800AC5E0 = 3;
    func_8003A198();
    D_800F4D70.second = D_800102F0 + 0x2D000;
    func_8003A1B4();
    D_800AC5D4 = arg0;
    D_800ACC18 = D_800F4D70.second;
    func_8003E928(0, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
    /* ASM_BARRIER: keep the published arena cell across the first UI service.
     * Read/write memory annotation retains its value and original address lifetime. */
    __asm__("" : "+m"(D_800ACC18));
    func_8003E928(1, 0x18, 0x80, 0x44, 0x20, 0, 0, 1, 0, 0x36);
    func_8003E928(2, 0x18, 0xA8, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    func_8003E928(3, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
    func_8003E928(4, 0x18, 0x80, 0x44, 0x20, 0, 0, 1, 0, 0x65);
    func_8003E928(5, 0x18, 0xA8, 0x44, 0x20, 0, 0, 1, 0, 0x35);
    func_8003E928(6, 0x18, 0xD0, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    func_8003E928(7, 0x74, 0x58, 0x1C, 0x20, 0, 0, 1, 0, 0x37);
    func_8003E928(8, 0x74, 0x80, 0x1C, 0x20, 0, 0, 1, 0, 0x38);
    func_8003E928(9, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
    func_8003E928(0xA, 0x18, 0x80, 0x44, 0x20, 0, 0, 1, 0, 0x36);
    func_8003E928(0xB, 0x18, 0xA8, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    func_8003E928(0xC, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
    func_8003E928(0xD, 0x18, 0x80, 0x44, 0x20, 0, 0, 1, 0, 0x65);
    func_8003E928(0xE, 0x18, 0xA8, 0x44, 0x20, 0, 0, 1, 0, 0x35);
    func_8003E928(0xF, 0x18, 0xD0, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    func_8003E928(0x10, 0x74, 0x58, 0x1C, 0x20, 0, 0, 1, 0, 0x37);
    func_8003E928(0x11, 0x74, 0x80, 0x1C, 0x20, 0, 0, 1, 0, 0x38);
    func_8003E928(0x12, 0xC, 0x40, 0x44, 0x20, 0, 0, 1, 0, 0x54);
    func_8003E928(0x13, 0xC, 0x68, 0x44, 0x20, 0, 0, 1, 0, 3);
    func_8003E928(0x14, 0xC, 0x90, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    func_8003E928(0x15, 0xC, 0x170, 0xD0, 0x20, 0, 0, 8, 4, (s32) D_800AC648);
    func_8003E928(0x17, 0x68, 0x68, 0x44, 0x20, 0, 0, 1, 5, 3);
    func_8003E928(0x18, 0x68, 0x40, 0x44, 0x20, 0, 0, 0, 0x7E, 0);
    func_80013890(5);
    D_800AC64A = 0;
    D_800AC5E0 = 3;
    func_8003A198();
    func_80037110();
    D_8010C0B8 = 0;
    D_800ACC18 = D_800F4D40;
    if (!(D_800AC5D4 & 1)) {
        D_800F4D70.first = D_800F4D40;
    } else {
        D_800ACC18 = D_800F4D70.first;
    }
    var_s1 = 0;
    placeY=10;placeZ=308;
    var_s0 = D_800ABEF8;
    RR_PROJECTION_SCRATCH->word18 = 0x78;
    RR_PROJECTION_SCRATCH->word08 = -0x10;
    RR_PROJECTION_SCRATCH->word0C = -0xF8;
    RR_PROJECTION_SCRATCH->word1C = 0;
    RR_PROJECTION_SCRATCH->word20 = 0;
    RR_PROJECTION_SCRATCH->word10 = -0x60;
    do {
        /* ASM_BARRIER: retain the current vehicle across its reset callback. */
        __asm__("" : "+r"(var_s0));
        func_8002E4F4(var_s0);
        var_s0->pos20[1] = placeY;
        var_s0->pos20[2] = placeZ;
        rotation=0x800;
        var_s0->rot54 = rotation;
        /* ASM_BARRIER: publish vehicle fields before advancing the loop phase. */
        __asm__("" : : "r"(var_s1) : "memory");
        var_s1 += 1;
        var_s0++;
    } while (var_s1 < 2);
    D_8010C0C0[0][0] = func_800370BC(D_8010BFD8[0][0]);
    D_8010C0C0[0][1] = func_800370BC(D_8010BFD8[0][1]);
    D_8010C0C0[1][0] = func_800370BC(D_8010BFD8[1][0]);
    D_8010C0C0[1][1] = func_800370BC(D_8010BFD8[1][1]);
    temp_a0 = arg0 & 1;
    if (D_8010C0C0[temp_a0][1] == 0) {
        D_8010C0D8[temp_a0] = 0;
    }
    temp_a2 = D_800AC5D4 & 1;
    func_80037040((s8 *)&D_800F4D98 + D_800AC5D4 * 4,
        D_8010BFD8[temp_a2][D_8010C0D8[temp_a2]][D_8010C0D0[temp_a2]].glyph);
    temp_a2_2 = D_800AC5D4 & 1;
    temp_s0 = &D_8010C0D8[temp_a2_2];
    temp_t0_2 = &D_8010C0D0[temp_a2_2];
    temp_a3_2 = &D_800ACBD0[D_800AC5D4];
    temp_a3_2->first = D_8010BFD8[temp_a2_2][*temp_s0][*temp_t0_2].first;
    temp_a3_2->second = D_8010BFD8[temp_a2_2][*temp_s0][*temp_t0_2].second;
}
