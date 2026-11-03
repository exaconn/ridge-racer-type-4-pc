typedef struct {
    char pad0[0x2664];
    int f2664;
    char pad1[0x2670 - 0x2668];
    int f2670;
    char pad2[0x2678 - 0x2674];
    int f2678;
    char pad3[0x2694 - 0x267C];
    int f2694;
    char pad4[0x269C - 0x2698];
    int f269C;
    char pad5[0x26A4 - 0x26A0];
    int f26A4;
    char pad6[0x26B0 - 0x26A8];
    int f26B0;
    int f26B4;
    int f26B8;
    char pad7[0x26C4 - 0x26BC];
    unsigned short h26C4;
    char pad8[0x26CE - 0x26C6];
    unsigned short h26CE;
} S;
extern S D_800F4DB8;

void func_80052C70()
{
    switch (D_800F4DB8.f269C) {
    case 0:
    case 1:
        if (D_800F4DB8.f2670 != 2) {
            D_800F4DB8.f2670 = 2;
            D_800F4DB8.f2678 = 0;
            D_800F4DB8.h26CE = D_800F4DB8.h26C4;
        }
        if (D_800F4DB8.f269C == 1) {
            D_800F4DB8.f26B8 = D_800F4DB8.f269C;
        } else {
            D_800F4DB8.f26B8 = 0;
            D_800F4DB8.f2694 = D_800F4DB8.f2664;
        }
        break;
    case 2:
        if (D_800F4DB8.f2670 != 3) {
            D_800F4DB8.f2670 = 3;
            D_800F4DB8.f26A4 = 0;
            D_800F4DB8.f2678 = 0;
        }
        break;
    }
    if (D_800F4DB8.f2670 == 1) {
        if (D_800F4DB8.f26B0 < 0x20) {
            D_800F4DB8.f26B4 = D_800F4DB8.f2670;
            return;
        }
        D_800F4DB8.f2670 = 2;
        D_800F4DB8.f2678 = 0;
        D_800F4DB8.h26CE = D_800F4DB8.h26C4;
    }
}
