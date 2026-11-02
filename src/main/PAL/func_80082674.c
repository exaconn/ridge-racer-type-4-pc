/* CC1_FLAGS: -mno-split-addresses */
#include "rr/spu_common.h"
#define SPU(index) (((RrSpuRegisterStorage*)D_800A47D8)->halves[index])
void func_80082674(RrSpuCommonAttributes28 *attributes) {
    RrSpuRegisterStorage *bank;
    s16 temp_a2;
    s16 temp_a3;
    s16 temp_v1;
    s16 temp_v1_2;
    s16 var_v0_3;
    s16 var_v0_4;
    s16 var_v0_5;
    s16 var_v0_6;
    s32 mask;
    s32 all;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_v0;
    s32 var_v0_2;
    register u16 leftVolume __asm__("$6");
    u16 leftSweep;
    u16 rightVolume;
    register u16 rightSweep __asm__("$8");

    leftVolume = 0; asm("" : "+r"(leftVolume));
    mask = attributes->mask;
    all = mask == 0;
    rightVolume = 0;
    if (all == 0) {
        if (mask & 1) {
            if (mask & 4) {
                goto block_3;
            }
            goto block_12;
        }
    } else {
block_3:
        temp_v1 = attributes->mode.left;
        switch (temp_v1) {
        case 1:
            var_a1 = 0x8000;
            break;
        case 2:
            var_a1 = 0x9000;
            break;
        case 3:
            var_a1 = 0xA000;
            break;
        case 4:
            var_a1 = 0xB000;
            break;
        case 5:
            var_a1 = 0xC000;
            break;
        case 6:
            var_a1 = 0xD000;
            break;
        case 7:
            var_a1 = 0xE000;
            break;
        case 0:
        default:
block_12:
            leftVolume = attributes->master.left;
            var_a1 = 0;
            break;
        }
block_13:
        var_v0 = leftVolume & 0x7FFF;
        if (var_a1 != 0) {
            temp_a3 = (s16) attributes->master.left;
            leftSweep = 0x7F;
            if (temp_a3 < 0x80) {
                leftSweep = 0;
                if (temp_a3 >= 0) {
                    leftSweep = attributes->master.left;
                }
            }
            var_v0 = leftSweep & 0x7FFF;
        }
        SPU(192) = (s16) (var_v0 | var_a1);
    }
    if (all == 0) {
        if (mask & 2) {
            if (mask & 8) {
                goto block_22;
            }
            goto block_31;
        }
    } else {
block_22:
        temp_v1_2 = attributes->mode.right;
        switch (temp_v1_2) {
        case 1:
            var_a1_2 = 0x8000;
            break;
        case 2:
            var_a1_2 = 0x9000;
            break;
        case 3:
            var_a1_2 = 0xA000;
            break;
        case 4:
            var_a1_2 = 0xB000;
            break;
        case 5:
            var_a1_2 = 0xC000;
            break;
        case 6:
            var_a1_2 = 0xD000;
            break;
        case 7:
            var_a1_2 = 0xE000;
            break;
        case 0:
        default:
block_31:
            rightVolume = attributes->master.right;
            var_a1_2 = 0;
            break;
        }
block_32:
        var_v0_2 = rightVolume & 0x7FFF;
        if (var_a1_2 != 0) {
            temp_a2 = (s16) attributes->master.right;
            rightSweep = 0x7F;
            if (temp_a2 < 0x80) {
                rightSweep = 0;
                if (temp_a2 >= 0) {
                    rightSweep = attributes->master.right;
                }
            }
            var_v0_2 = rightSweep & 0x7FFF;
        }
        SPU(193) = (s16) (var_v0_2 | var_a1_2);
    }
    if ((all != 0) || (mask & 0x40)) {
        SPU(216) = (u16) attributes->cd.volume.left;
    }
    if ((all != 0) || (mask & 0x80)) {
        SPU(217) = (u16) attributes->cd.volume.right;
    }
    if ((all != 0) || (mask & 0x400)) {
        SPU(218) = (u16) attributes->external.volume.left;
    }
    if ((all != 0) || (mask & 0x800)) {
        SPU(219) = (u16) attributes->external.volume.right;
    }
    if ((all != 0) || (mask & 0x100)) {
        if (attributes->cd.reverb == 0) {
            bank=(RrSpuRegisterStorage*)D_800A47D8; var_v0_3 = bank->halves[213] & 0xFFFB;
        } else {
            bank=(RrSpuRegisterStorage*)D_800A47D8; var_v0_3 = bank->halves[213] | 4;
        }
        bank->halves[213] = var_v0_3;
    }
    if ((all != 0) || (mask & 0x200)) {
        if (attributes->cd.mix == 0) {
            bank=(RrSpuRegisterStorage*)D_800A47D8; var_v0_4 = bank->halves[213] & 0xFFFE;
        } else {
            bank=(RrSpuRegisterStorage*)D_800A47D8; var_v0_4 = bank->halves[213] | 1;
        }
        bank->halves[213] = var_v0_4;
    }
    if ((all != 0) || (mask & 0x1000)) {
        if (attributes->external.reverb == 0) {
            bank=(RrSpuRegisterStorage*)D_800A47D8; var_v0_5 = bank->halves[213] & 0xFFF7;
        } else {
            bank=(RrSpuRegisterStorage*)D_800A47D8; var_v0_5 = bank->halves[213] | 8;
        }
        bank->halves[213] = var_v0_5;
    }
    if ((all != 0) || (mask & 0x2000)) {
        if (attributes->external.mix == 0) {
            bank=(RrSpuRegisterStorage*)D_800A47D8; var_v0_6 = bank->halves[213] & 0xFFFD;
        } else {
            bank=(RrSpuRegisterStorage*)D_800A47D8; var_v0_6 = bank->halves[213] | 2;
        }
        bank->halves[213] = var_v0_6;
    }
}
