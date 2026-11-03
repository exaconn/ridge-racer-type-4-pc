#include "types.h"
#include "m2c_macros.h"
#include "game/data/ldscn1.h"
#include "game/gfx/poly1.h"
#include "game/menu/menutra21.h"

typedef struct {
    char name[10];
    unsigned short car;
} StructC;

typedef struct {
    char name[10];
    unsigned short car;
    int time;
} Struct10;

extern int D_8009EA04[2][8];
extern StructC D_8009EA44[5];
extern unsigned char D_8009F900[2][8];
extern int D_800AC5B0[8];
extern int D_800ACC28[2][8];
extern Struct10 D_800AD5C8[2][8][5];
extern int D_800F3838[2][8][3];
extern int D_800F3B00[8];
extern int D_800F4CE8[2][8];
extern Struct10 D_800FF700[2][8][5];
extern int func_80096434();
extern char D_800ABE08[];
extern unsigned short D_8009EA80[];
extern unsigned char D_800F2FE0[];
extern unsigned char D_800F3B28[];
M2C_UNK func_8001E84C(M2C_UNK *);
/* extern */
/* extern */
/* extern */
M2C_UNK func_80092CB8(M2C_UNK);
/* extern */
extern s32 D_800102F0;
extern M2C_UNK D_800103B8;
extern s32 D_800AC5DC;
extern s32 D_800ACC18;
extern s32 D_800F2DE4;
extern s16 D_800F3A26;
extern s16 D_800F4C5C;
extern s16 D_800F4C6A;
extern s8 D_800F4D7C;
extern s16 D_800F4D8A;
extern s16 D_800FFC18;
extern s32 D_8010BFBC;
extern int func_8004FA98();
extern int func_80071570();
extern s16 D_800AD55A;

void func_8001EEE4(void);
void func_8001F194(void);
void func_8001F254(void);
void func_8001F290(void);
void func_8001F2CC(void);
void func_8001F2D4(void);
void func_8001F374(void);
void func_8001F400(void);

void func_8001EEE4(void) {
    int i, j, k;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            int c = 0x2FD;
            D_800F3B00[j] = D_8009EA04[0][j];
            D_800AC5B0[j] = (D_8009EA04[0][j] - 0x2FD) * D_8009F900[1][j] + (D_8009F900[1][j] * 1000 + c);
            D_800ACC28[i][j] = D_8009EA04[1][j];
            D_800F4CE8[i][j] = (D_8009EA04[1][j] - 0x2FD) * D_8009F900[0][j] + (D_8009F900[0][j] * 1000 + c);
            for (k = 0; k < 3; k++) {
                D_800F3838[i][j][k] = D_8009EA04[1][j];
            }
        }
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            for (k = 0; k < 5; k++) {
                *(int *)&D_800FF700[i][j][k].name[0] = *(int *)&D_8009EA44[k].name[0];
                *(int *)&D_800FF700[i][j][k].name[4] = *(int *)&D_8009EA44[k].name[4];
                *(int *)&D_800AD5C8[i][j][k].name[0] = *(int *)&D_8009EA44[k].name[0];
                *(int *)&D_800AD5C8[i][j][k].name[4] = *(int *)&D_8009EA44[k].name[4];
                D_800FF700[i][j][k].time = D_800ACC28[i][j] + k * 3000;
                {
                    int t = k * 3000;
                    D_800AD5C8[i][j][k].time = D_800F4CE8[i][j] + t * D_8009F900[0][j];
                }
                D_800FF700[i][j][k].car = D_8009EA44[k].car;
                D_800AD5C8[i][j][k].car = D_8009EA44[k].car;
            }
        }
    }
}

void func_8001F194(void)
{
    /* ASM_BARRIER: register pins reproduce original t/a register allocation */
    register int a2 asm("$6");
    register int a3 asm("$7");
    int t1;
    int t2;
    register int t4 asm("$12");
    register int e asm("$13");
    unsigned short *t6;
    register unsigned short *t0 asm("$8");
    short *t3;
    char *a1;
    unsigned short u;
    func_80096434(D_800ABE08, 0x90);
    func_80096434(D_800ABE08 + 0x24, 0x90);
    t4 = 0;
    e = 8;
    t6 = D_8009EA80;
    t2 = 0x20;
    t1 = t4;
    do {
        a2 = 0;
        t3 = (short *)(t2 + D_800ABE08);
        t0 = t6;
        a3 = t1;
loop:
        a1 = a3 + D_800ABE08;
        if (a2 == e) {
            int m;
            __asm__ volatile("" : "=r"(m) : "0"(-1)); /* ASM_BARRIER: keep -1 inside branch */
            *t3 = m;
        } else {
            u = *t0;
            a1[3] = (a2 / 2) + (a2 < 4);
            *(unsigned short *)a1 = u;
        }
        t0++;
        a2++;
        a3 += 4;
        if (a2 < 9) goto loop;
        t2 += 0x24;
        t4++;
        t1 += 0x24;
    } while (t4 < 4);
}

void func_8001F254(void) {
    int i;
    for (i = 0; i < 0x840; i++) {
        D_800F3B28[i] = D_800F2FE0[i];
    }
}

void func_8001F290(void) {
    int i;
    for (i = 0; i < 0x840; i++) {
        D_800F2FE0[i] = D_800F3B28[i];
    }
}

void func_8001F2CC(void) {
}

void func_8001F2D4(void) {
    int pad[6];
    func_80092CB8(0);
    func_8001E84C(&D_800103B8);
    D_800ACC18 = D_800102F0;
    func_80071440();
    D_800FFC18 = 0;
    func_8007CDC4(0);
    D_800AC5DC = 0x80;
    D_800F2DE4 = 0;
    D_800F4D8A = 0;
    D_800F4D7C = 0;
    D_8010BFBC = 0;
    D_800F4C5C = 0x203;
    D_800F4C6A = 1;
    D_800F3A26 = 2;
    D_800F4D8A = 0;
}

/* legacy typing of shared globals in this part of the unit */
#define func_8007CDC4 ((int (*)())func_8007CDC4)
#define D_800F4C5C (*(u16 (*))&D_800F4C5C)

void func_8001F374(void) {
    s32 temp_v0;

    temp_v0 = func_8003BBBC();
    if (temp_v0 == 1) {
        D_800F2DE4 += 1;
        if (func_80071570(func_8007CDC4(1) == 1) == temp_v0) {
            func_8004FA98(D_800AD55A);
            D_800F4D8A = 0;
            D_800F4C6A = (s16) ((u16) D_800F4C5C >> 8);
            D_800F3A26 = D_800F4C5C & 0xFF;
        }
    }
}

#undef func_8007CDC4
#undef D_800F4C5C

void func_8001F400(void){ int a[6];
/* ASM_BARRIER: empty-frame function needs jr/addiu sp delay-slot epilogue form */
asm volatile("" ::: "memory"); }
