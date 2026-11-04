/* RODATA 0x800114a0 0xc4 */
#include "game/gfx/color.h"
#include "psyq/libc/printf.h"
#include "game/data/savedat.h"
#include "game/data/savetype.h"
#include "rr/menu_selection.h"
#include "rr/overlay662_frame_step.h"
#include "rr/overlay668_playback_step.h"
#include "game/race/raceset.h"
typedef struct { char b[0x80]; } B;

extern int func_8002B75C();
extern Struct48 D_800ABE08;
extern SaveBuf *D_800ACC0C;
extern int D_800ACC28[2][8];
extern Struct40 D_800AD528;
extern Struct10 D_800AD5C8[2][8][5];
extern Struct2C D_800F2E30[3];
extern int D_800F3838[2][8][3];
extern Struct840 *D_800F3910;
extern Struct840 *D_800F3918;
extern Struct7C D_800F3920;
extern int D_800F3B28[];
extern short D_800F4D3C;
extern short D_800F4D44;
extern Struct10 D_800FF700[2][8][5];
extern Struct2C D_8010B258[3];
extern Struct48 D_8010BFD8[2];
extern StructC D_8010C088[];
extern StructC D_80118420[];
extern Struct48 D_80118458[2];
extern int D_801272F0[];
extern int func_8009305C();
extern char D_80011564[];
extern short D_8010EF98[];
extern char D_80011568[];
extern short D_8010EFA0[];

void func_80079ED8();

int func_800797E0(int arg0) {
    int a2 = 0;
    SaveBuf *s3;
    SaveBuf11 *p;
    int a1;
    int s0;
    int t0;
    int k;
    int v10;
    int v11;

    switch (arg0) {
    case 0:
    case 1:
    case 0x30:
    default:
        s3 = D_800ACC0C;
        a1 = 0;
        for (s0 = 0; (unsigned int)s0 < 0x6A9; s0++) {
            a1 += ((int *)s3)[s0];
        }
        if (s3->f1AA4 == a1) {
            D_800F3920 = s3->f280;
            D_800AD528 = s3->f2FC;
            for (s0 = 0; s0 < 3; s0++) {
                if (arg0 == 0x30) {
                    D_800F2E30[s0] = s3->f360[s0];
                } else {
                    D_8010B258[s0] = s3->f360[s0];
                }
            }
            for (s0 = 0; s0 < 9; s0++) {
                D_800ABE08.f24[s0] = s3->f33C[s0];
            }
            for (s0 = 0; s0 < 2; s0++) {
                for (t0 = 0; t0 < 8; t0++) {
                    for (k = 0; k < 3; k++) {
                        D_800F3838[s0][t0][k] = s3->f3E4[s0][t0][k];
                    }
                    for (k = 0; k < 5; k++) {
                        D_800FF700[s0][t0][k] = s3->f4A4[s0][t0][k];
                        D_800AD5C8[s0][t0][k] = s3->f9A4[s0][t0][k];
                    }
                    D_800ACC28[s0][t0] = D_800FF700[s0][t0][0].w[3];
                    D_800F4CE8[s0][t0] = D_800AD5C8[s0][t0][0].w[3];
                }
            }
            for (s0 = 0; s0 < 0x210; s0++) {
                D_800F3B28[s0] = s3->fEA4[s0];
            }
            func_8004C0EC();
            a2 = 1;
        }
        break;
    case 2:
        s3 = D_800ACC0C;
        a1 = 0;
        for (s0 = 0; (unsigned int)s0 < 0x6A9; s0++) {
            a1 += ((int *)s3)[s0];
        }
        if (s3->f1AA4 == a1) {
            for (s0 = 0; s0 < 9; s0++) {
                if (D_800F4D44 == 2 || D_800F4D44 == 3) {
                    D_80118458[D_800AC5D4 & 1].f24[s0] = s3->f33C[s0];
                    func_8002B75C(&D_80118420[D_800AC5D4 & 1], &s3->f280);
                } else if (D_800F4D44 == 4) {
                    D_8010BFD8[D_800AC5D4 & 1].f24[s0] = s3->f33C[s0];
                    func_8002B75C(&D_8010C088[D_800AC5D4], &s3->f280);
                }
            }
            for (s0 = 0; s0 < 0x210; s0++) {
                *(int *)((D_800AC5D4 & 1) * 0x840 + (s0 << 2) + (int)D_800F3B28) = s3->fEA4[s0];
            }
            a2 = 1;
        }
        break;
    case 0x10:
        v10 = *(int *)D_800ACC0C;
        D_801272F0[D_800F4D3C] = v10;
        a2 = 1;
        break;
    case 0x11:
        v11 = (p = (SaveBuf11 *)D_800ACC0C)->f280;
        D_800F4D38 = D_801272F0[D_800F4D3C] = v11;
        for (t0 = 0; t0 < 2; t0++) {
            for (s0 = 0; s0 < 0x840; s0++) {
                if (t0 == 0) {
                    D_800F3918->b[s0] = p->f300[0][s0];
                } else {
                    D_800F3910->b[s0] = p->f300[t0][s0];
                }
            }
        }
        a2 = 1;
        break;
    }
    return a2;
}

void func_80079ED8(a0, a1, a2, a3, a4, a5)
char *a0;
int a1;
short a2;
short a3;
int a4;
int a5;
{
    int i;
    int *new_var;
    a0[0] = 0x53;
    a0[1] = 0x43;
    a0[2] = 0x13;
    a0[3] = 1;
    func_80096484(a0 + 4, D_80011564, a1);
    D_8010EF98[0] = a2;
    D_8010EF98[2] = 0x10;
    D_8010EF98[1] = a3;
    D_8010EF98[3] = 1;
    func_8009305C(D_8010EF98, a0 + 0x60);
    DrawSync(0);
    i = 0;
    do {
        D_8010EF98[0] = a4;
        D_8010EF98[1] = a5;
        D_8010EF98[2] = 4;
        D_8010EF98[3] = 0x10;
        /* (new_var = &a4) is a dead address-of that keeps gcc from hoisting the li 0x10 out of the loop (found by permuter) */
        func_8009305C(D_8010EF98, a0 + 0x80 + (*(new_var = &a4), i) * 0x80);
        DrawSync(0);
        i++;
        a4 += 4;
    } while (i < 3);
}

void func_80079FF8(a0, a1, a2, a3, a4, a5, a6)
unsigned char *a0;
int a1;
short a2;
unsigned short a3;
int a4;
int a5;
B *a6;
{
    int i;
    a0[0] = 0x53;
    a0[1] = 0x43;
    a0[2] = 0x11;
    a0[3] = 1;
    func_80096484(a0 + 4, D_80011564, a1);
    a0[0x50] = 1;
    a0[0x51] = 0;
    func_80096484(a0 + 0x52, D_80011564, D_80011568);
    D_8010EFA0[0] = a2;
    D_8010EFA0[2] = 0x10;
    D_8010EFA0[1] = a3;
    D_8010EFA0[3] = 1;
    func_8009305C(D_8010EFA0, a0 + 0x60);
    DrawSync(0);
    i = 0;
    do {
        D_8010EFA0[0] = a4;
        D_8010EFA0[1] = a5;
        D_8010EFA0[2] = 4;
        D_8010EFA0[3] = 0x10;
        func_8009305C(D_8010EFA0, a0 + 0x80 + i * 0x80);
        DrawSync(0);
        i++;
        a4 += 4;
    } while (i <= 0);
    *(B *)(a0 + 0x100) = *a6;
    *(B *)(a0 + 0x180) = *a6;
    *(B *)(a0 + 0x200) = *a6;
}
