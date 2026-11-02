#include "rr/overlay667_packet.h"

s32 ovl667_func_80114E34(void) {
    u8 checksum;
    u32 i;
    if (func_80096EA4(ovl667_D_80118ED8) == 1) {
        ovl667_D_80118EF4 = 0;
        ovl667_D_80118EEF = 0;
        checksum = 0;
        i = 0;
        do {
            checksum ^= ((u8 *)&ovl667_D_80118F10.packet)[i+4];
            i++;
        } while (i < 8);
        if (checksum != ovl667_D_80118F10.packet.checksum01) ovl667_D_80118EEF = 1;
        if (ovl667_D_80118F10.packet.flags00 >> 7) {
            if ((ovl667_D_80118F10.packet.flags00 >> 6) & 1) ovl667_D_80118EF0 = 2;
            else ovl667_D_80118EF0 = 1;
        }
        ovl667_D_80118F08 = ovl667_D_80118F10.packet.flags00;
        if (ovl667_D_80118F09 == ovl667_D_80118F08) {
            ovl667_D_80118F8D = (ovl667_D_80118F08 + 7) & 7;
        } else if (ovl667_D_80118F08 == ovl667_D_80118F09 + 1) {
            ovl667_D_80118F8D = ovl667_D_80118F09;
            ovl667_D_80118EC8 = 3;
        } else if (ovl667_D_80118F09 == ovl667_D_80118F08 + 2) {
            ovl667_D_80118F8D = ovl667_D_80118F08;
            ovl667_D_80118F09 = (ovl667_D_80118F08 + 9) & 7;
        } else ovl667_D_80118F8D = ovl667_D_80118F08;
        ovl667_D_80118F20.packets[ovl667_D_80118F08] = ovl667_D_80118F10.packet;
        func_80096F44(D_800F2AA8, &ovl667_D_80118F10.packet, 12);
        return 1;
    }
    if (ovl667_D_80118EE8 != 0) return -1;
    func_8009AEAC(2,0,0);
    func_80096F44(D_800F2AA8, &ovl667_D_80118F10.packet, 12);
    ovl667_D_80118EE8 = 1;
    ovl667_D_80118EF8 = (s32)((u32)ovl667_D_80118EF8 + 1U);
    return 0;
}
