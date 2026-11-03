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

typedef struct { char p[7]; char b; } Z;

extern char D_8009F8FC;

extern unsigned char D_800F4D7C;

extern int func_8001223C();

extern int func_8001292C();

extern int func_80012A58();

extern char D_800FFC48[];

extern int func_800129BC();

extern int func_80032914();

extern int func_80029358();

extern int func_80043608();

extern int D_800F4CA8;

extern int D_800AD564;

extern char D_80104DB0[];

extern Z *D_800AC590;

int func_8003B934(void) {
    int r = 0;
    char *s;
    int a1;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
    case 1:
        if (func_8001292C(func_8001223C(1, 0x42), D_800FFC48)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (func_80012A58()) {
            s = D_800FFC48;
            func_80043608(s);
            s += 0x4E20;
            D_800F4D7C = 3;
            D_800AC590 = (Z *)s;
            func_80032914(s);
    case 3:
            if (D_800F4CA8 == 2) {
                a1 = 0x44;
            } else if (D_800F4CA8 == 4) {
                a1 = 0x46;
                D_800AC590->b = 2;
            } else {
                a1 = 0x45;
            }
            if (func_800129BC(func_8001223C(1, a1), D_800AD564 << 2, 4, D_80104DB0)) {
                D_800F4D7C = 4;
            }
        }
        break;
    case 4:
        if (func_80012A58()) {
            func_80029358(D_80104DB0);
            D_800F4D7C = 5;
    case 5:
            r = 1;
        }
        break;
    }
    return r;
}
