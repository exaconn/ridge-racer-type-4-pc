/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

#define D_800F4D2C (*(unsigned char (*))&D_800F4D2C)

#undef D_8009F8FC

#undef D_800F4D2C

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define D_800F3B24 (*(int (*))&D_800F3B24)

#undef D_800F3B24

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define func_800436E8 ((void (*)())func_800436E8)

#define func_8004E008 ((void (*)())func_8004E008)

#define func_8004F600 ((void (*)())func_8004F600)

#undef func_800436E8

#undef func_8004E008

#undef func_8004F600

extern char D_8009F8FC;

extern unsigned char D_800F4D7C;

extern int func_8001223C();

extern int func_8001292C();

extern int func_80012A58();

extern int D_800ACC18;

extern short D_800F3914;

extern void func_800292A8();

extern char *D_800ABEA4;

extern int D_800ABECC;

extern char *D_800F2EC4;

extern int D_8010BFC8;

int func_8003BAB0(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_8009F8FC = 0;
        D_800F4D7C = 1;
        D_8010BFC8 = 0;
    case 1:
        if (func_8001292C(func_8001223C(1, D_800F3914 + 0x38), D_800ACC18) != 0) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (func_80012A58() != 0) {
            func_800292A8(D_800ACC18);
            D_800F4D7C = 3;
            D_800F2EC4 = D_800ABEA4 + D_800ABECC * 60;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}
