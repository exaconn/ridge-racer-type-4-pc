#include "types.h"
#include "rr/badge_rows.h"
#include "game/ui/uicnt.h"
#include "rr/menu_label_render.h"

extern int SelectedStoryBranchId;
extern int HudCountdownDigitSprites[];

void DrawBadgeCountdownSprites(int a0, void *a1, int a2);
s32 ChecksumBytes(u8 *p, int n);

void DrawBadgeCountdownSprites(int a0, void *a1, int a2) {
    char sp[8];
    int s4;
    int s2;
    int i;
    int off;
    int rem;

    s4 = BadgeRows[a2].x04 + 0x10;
    s2 = BadgeRows[a2].y06 + 0x20;
    rem = SelectedStoryBranchId % 5;
    a1 = AddSpritePrim(a0, a1, s4, s2, &HudCountdownDigitSprites[rem], 0x4424, 0);
    for (i = 0; i < 2; i++) {
        off = i * 0x28 - 8;
        a1 = AddSpritePrim(a0, a1, s4 + off, s2, &HudCountdownDigitSprites[i + 5], 0x4424, 0);
    }
    AddDrawModePrim(a0, a1, 0x39);
}

s32 AddOrSubtractByte(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_v0;

    if (arg2 == 0) {
        var_v0 = arg0 + arg1;
    } else {
        var_v0 = arg0 - arg1;
    }
    return var_v0 & 0xFF;
}

void ByteToCodeChars(s32 arg0, void *arg1) {
    ((s8 *)arg1)[0] = (s8) ((arg0 & 0xF) + 0x41);
    ((s8 *)arg1)[1] = (s8) (((u32) (arg0 & 0xFF) >> 4) + 0x4B);
}

s32 CodeCharsToByte(s32 arg0, s32 arg1) {
    return (((arg1 - 0x4B) * 0x10) | ((arg0 - 0x41) & 0xF)) & 0xFF;
}

void XorBytes37(u8 *arg0, s32 arg1) {
    s32 var_v1;
    u8 *var_a0;

    var_a0 = arg0;
    var_v1 = 0;
    if (arg1 > 0) {
        do {
            var_v1 += 1;
            *var_a0 ^= 0x37;
            var_a0 += 1;
        } while (var_v1 < arg1);
    }
}

s32 ChecksumBytes(u8 *p, int n) {
    int i;
    u8 s = 0;
    for (i = 0; i < n; i++) {
        s += *p++;
    }
    return s;
}
