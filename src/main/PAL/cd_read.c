/* Candidate module: CD read state machine, 80012578..80012A68.
 * Adjacent matching functions share one directory and location definition.
 * This grouping does not establish the original compiler unit boundary. */
#include "rr/cd.h"

extern s32 func_80012290();
extern s32 func_8008C6A4(), func_8008EEF4(), func_8008F090();

extern s32 D_8009DF90, D_8009DF94, D_8009DF9C;
extern s32 (*D_8009DFA8[])();
extern s32 D_8010945C, D_80109460, D_80109480, D_80109484, D_80109488;
extern RrCdLocation D_80109464;
extern u8 D_80109498;
extern RrCdFileEntry D_801094A8[];

s32 func_80012578(void) {
    s32 var_v0;

    func_8008C3B0(1, 0, &D_80109498);
    var_v0 = D_80109498 & 0x10;
    if (var_v0 != 0) {
        D_8009DF90 = 5;
        var_v0 = 0;
    }
    return var_v0;
}

s32 func_800125C8(void) {
    func_8008C27C(0x13, 0);
    D_8009DF90 = 2;
    return 0;
}

s32 func_800125F8(void) {
    if (D_80109488 == 2) {
        func_80012290();
        D_8009DF90 = 3;
    } else {
        D_8009DF90 = 1;
    }
    return 0;
}

s32 func_80012640(void) {
    D_80109498 = 0x80;
    func_8008C27C(0xE, &D_80109498);
    D_8009DF90 = 4;
    D_8009DF94 = 0;
    return 0;
}

s32 func_80012684(void) {
    if (D_80109488 == 2) {
        if (++D_8009DF94 >= 4) {
            D_8009DF94 = 0;
            if (D_8009DF9C < 0) {
                D_8009DF90 = 0;
            } else {
                D_8009DF90 = 6;
            }
        }
    } else {
        D_8009DF90 = 3;
    }
    return 0;
}

s32 func_800126F0(void) {
    func_8008C3B0(1, 0, &D_80109498);
    if (!(D_80109498 & 0x10)) {
        D_8009DF90 = 1;
    }
    return 0;
}

s32 func_80012740(void) {
    func_8008C27C(2, &D_80109464);
    D_8009DF90 = 7;
    return 0;
}

s32 func_80012774(void) {
    if (D_80109488 == 2) {
        D_8009DF90 = 8;
    } else {
        D_8009DF90 = 0xB;
    }
    return 0;
}

s32 func_800127A0(void) {
    if (func_8008EEF4(D_80109460, D_80109480, 0x80)) {
        D_8009DF90 = 9;
    } else {
        D_8009DF90 = 0xB;
    }
    return 0;
}

s32 func_800127E8(void) {
    s32 r = func_8008F090(1, 0);
    if (r == 0) {
        D_8009DF90 = 0xA;
    } else if (r == -1) {
        D_8009DF90 = 0xB;
    }
    return 0;
}

void func_80012834(void) {
    D_8009DF9C = -1;
    D_8009DF90 = 0;
    D_80109484 = D_80109460 << 0xB;
}

s32 func_80012860(void)
{
    func_8008C3B0(1, 0, &D_80109498);
    if (D_80109498 & 0x10) {
        D_8009DF90 = 5;
        return 0;
    }
    D_8009DF90 = 6;
    return 0;
}

s32 func_800128B8(void) {
    if (D_8009DF90 > 0) {
        D_80109488 = func_8008C0D8(1, 0);
        if (D_80109488 != 0) {
            return D_8009DFA8[D_8009DF90]();
        }
    }
    return 0;
}

s32 func_8001292C(s32 a0, s32 a1) {
    if (D_8009DF90 != 0) return 0;
    D_80109480 = a1;
    D_8009DF9C = a0;
    D_8009DF90 = 6;
    D_8010945C = 0;
    D_80109464 = D_801094A8[a0].location;
    D_80109484 = 0;
    D_80109460 = D_801094A8[D_8009DF9C].sectors;
    return 1;
}

s32 func_800129BC(s32 a0, s32 a1, s32 a2, s32 a3) {
    if (D_8009DF90 != 0) return 0;
    D_80109480 = a3;
    D_8009DF9C = a0;
    D_8009DF90 = 6;
    D_8010945C = a1;
    func_8008C5A0(func_8008C6A4(&D_801094A8[a0]) + a1, D_80109464.c);
    D_80109460 = a2;
    D_80109484 = 0;
    return 1;
}

s32 func_80012A58(void) {
    return D_80109484;
}
