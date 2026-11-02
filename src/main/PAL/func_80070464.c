typedef struct { int a[8]; } S32B;
typedef struct { unsigned short x, y, z; } V3;
extern int func_8006E09C();
extern int func_8006E108();
extern int func_80090914();
extern int func_80090DB4();
extern short D_800AC0A4;
extern int D_800AC538;
extern short D_800AD50C;
extern S32B D_800AD598;
extern short D_800F4D44;
extern V3 D_800FF690;
extern int D_800FF6B0;
extern V3 D_800FF6B8;
extern S32B D_8010E710;
extern int D_8010E730;

int func_80070464(void) {
    char sp[32];
    int r = 0;
    int *s = (int *)0x1F800000;
    if (D_8010E730 != 0) {
        if (D_800AD50C != 0) {
            if (D_800AC538 == 0) {
                if (D_800F4D44 != 0) {
                    r = D_800FF6B0 == 2;
                }
            }
        }
    }
    if (r != 0) {
        D_8010E710 = *(S32B *)(s + 10);
        *(S32B *)(s + 10) = D_800AD598;
        s[7] += 0x800;
        func_8006E09C(sp, 0x800);
        func_80090914(sp, &D_800FF690, &D_800FF690);
        func_80090914(sp, &D_800FF6B8, &D_800FF6B8);
        func_8006E108(sp, D_800AC0A4 * 2);
        func_80090DB4(sp, &D_800FF690);
        func_80090DB4(sp, &D_800FF6B8);
        D_800FF690.x = -D_800FF690.x;
        D_800FF690.y = -D_800FF690.y;
        D_800FF690.z = -D_800FF690.z;
        D_800FF6B8.x = -D_800FF6B8.x;
        D_800FF6B8.y = -D_800FF6B8.y;
        D_800FF6B8.z = -D_800FF6B8.z;
    }
    return r;
}
