/* RODATA 0x80010420 0x20 */

typedef struct {
    int x;
} __attribute__((packed)) U4;

typedef struct {
    unsigned char b0;
    unsigned char b1;
    unsigned short h2;
    unsigned char pad4[4];
    unsigned short h8;
    unsigned char padA[6];
    int w10;
    unsigned char pad14[0xE];
    short s22;
    short s24;
    short s26;
    unsigned char pad28[0x14];
    short s3C;
    unsigned char pad3E[0x1E];
} S5C;

typedef struct {
    short h0;
    short h2;
    short h4;
    short h6;
    short h8;
    short hA;
    short hC;
    short hE;
    short h10;
    short h12;
    short h14;
    short h16;
    short h18;
    short h1A;
    short h1C;
    short h1E;
    short h20;
    short h22;
    short h24;
    short h26;
    short h28;
    short h2A;
    short h2C;
    short h2E;
} S30;

typedef struct {
    short h0;
    short s12;
    short s14;
    short pad[5];
} S10_sub;

typedef struct {
    char pad[0x10];
    S10_sub sub[2];
} S_AD528;

typedef struct {
    char pad[0xB0];
    int sB0;
} SA0;

extern short D_8009FFB8[];
extern int D_800AC5A8;
extern int D_800ACBE0[];
extern int D_800ACBF0[];
extern int D_800AD504;
extern short D_800AD508;
extern U4 *D_800AD510[];
extern S_AD528 D_800AD528;
extern unsigned char D_800ADAD8[][4];
extern S30 D_800F2F78[];
extern S5C D_800F3A38[];
extern int D_800F3AF0[];
extern short D_800F4D44;
extern int D_800FF6B0;
extern int D_80107188;

void func_80029660(char *arg0, int arg1) {
    SA0 *a0;
    short t3;
    short t5;
    short t1;
    short a1;
    register int t2 asm("t2"); /* ASM_BARRIER: pin t2 for idx copy */
    int idx;
    int b;
    int f;
    int mode;

    a0 = (SA0 *)(arg0 + 0x1F4);
    if (D_800FF6B0 != 0) {
        if (D_800AD504 == 2) {
            if (D_800AC5A8 < D_80107188) {
                *(U4 *)D_800ADAD8[arg1] = D_800AD510[arg1][D_800AC5A8];
                if (D_800F4D44 == 4) {
                    if (arg1 == 3) {
                        D_800AC5A8 += 1;
                    }
                } else if (arg1 == 1 || D_800F4D44 < 2 || D_800F4D44 == 6) {
                    D_800AC5A8 += 1;
                }
            }
        } else {
            t3 = 0;
            t2 = idx = arg1 & 1;
            if (D_800FF6B0 < 4) {
                mode = D_800F3A38[idx].w10;
                if (mode == 0) {
                    t1 = 0;
                    if (D_800F3A38[idx].h2 & D_800F2F78[idx].h4) {
                        t1 = 0xFF;
                    }
                    b = (D_800F3A38[idx].h2 & D_800F2F78[idx].h6) != 0;
                    {
                        register int f1 asm("v1"); /* ASM_BARRIER: pin f1 to v1 */
                        f1 = (D_800F3A38[idx].h2 & D_800F2F78[idx].h0) != 0;
                        if ((D_800F3A38[idx].h2 & D_800F2F78[idx].h2) > 0) {
                            f1 |= 2;
                        }
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].h8) > 0) {
                            f1 |= 4;
                        }
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].hA) > 0) {
                            f1 |= 8;
                        }
                        f = f1;
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].hE) > 0) {
                            f |= 0x10;
                        }
                    }
                    if (D_800F3A38[idx].b1 == 0x73) {
                        t5 = (D_800F3A38[idx].s3C * 0x7F) / 128;
                        t3 = f | 0x80;
                    } else {
                        t5 = 0;
                        t3 = f | 0x20;
                    }
                    a0->sB0 += b * 4;
                    if (b == 0) {
                        a0->sB0 = 0;
                    }
                    if (a0->sB0 >= 0x74) {
                        a0->sB0 = 0x73;
                    }
                    a1 = a0->sB0 + (b * 140);
                } else if (mode == 1) {
                    t5 = (D_800F3A38[idx].s3C * 0x7F) / D_8009FFB8[D_800AD528.sub[idx].s12];
                    t1 = 0;
                    if (D_800F3A38[idx].h2 & D_800F2F78[idx].h14) {
                        t1 = 0xFF;
                    }
                    a1 = 0;
                    if (D_800F3A38[idx].h2 & D_800F2F78[idx].h16) {
                        a1 = 0xFF;
                    }
                    {
                        int fm1;
                        fm1 = ((D_800F3A38[idx].h8 & D_800F2F78[idx].h18) != 0) * 4;
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].h1A) > 0) {
                            fm1 |= 8;
                        }
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].h1E) > 0) {
                            fm1 |= 0x10;
                        }
                        t3 = fm1 | 0x40;
                    }
                    switch (D_800AD528.sub[idx].s14) {
                    case 0:
                    case 5: {
                        int i0;
                        t1 = (D_800F3A38[({ i0 = t2; __asm__("" : "+r"(i0)); i0; })].s22 * 0xFF) / 106; /* ASM_BARRIER: keep index copy */
                        a1 = (D_800F3A38[i0].s24 * 0xFF) / 106;
                        break;
                    }
                    case 1:
                    case 6: {
                        int i1;
                        t1 = (D_800F3A38[({ i1 = t2; __asm__("" : "+r"(i1)); i1; })].s24 * 0xFF) / 106; /* ASM_BARRIER: keep index copy */
                        a1 = (D_800F3A38[i1].s22 * 0xFF) / 106;
                        break;
                    }
                    case 2: {
                        int i2;
                        a1 = (D_800F3A38[({ i2 = t2; __asm__("" : "+r"(i2)); i2; })].s26 * 0xFF) / 106; /* ASM_BARRIER: keep index copy */
                        break;
                    }
                    case 3: {
                        int i3;
                        t1 = (D_800F3A38[({ i3 = t2; __asm__("" : "+r"(i3)); i3; })].s24 * 0xFF) / 106; /* ASM_BARRIER: keep index copy */
                        a1 = (D_800F3A38[i3].s26 * 0xFF) / 106;
                        break;
                    }
                    case 4:
                    case 7:
                        break;
                    }
                } else if (mode == 2) {
                    t5 = (D_800F3A38[idx].s3C * 0x7F) / 128;
                    t1 = 0;
                    if (D_800F3A38[idx].h2 & D_800F2F78[idx].h24) {
                        t1 = 0xFF;
                    }
                    b = (D_800F3A38[idx].h2 & D_800F2F78[idx].h26) != 0;
                    a0->sB0 += b * 4;
                    if (b == 0) {
                        a0->sB0 = 0;
                    }
                    if (a0->sB0 >= 0x74) {
                        a0->sB0 = 0x73;
                    }
                    a1 = a0->sB0 + (b * 140);
                    {
                        register unsigned short u2 asm("a0"); /* ASM_BARRIER: pin h2 load to a0 */
                        register int tand asm("v1"); /* ASM_BARRIER: pin h20 mask to v1 */
                        int fb, fc;
                        u2 = D_800F3A38[idx].h2;
                        tand = u2 & D_800F2F78[idx].h20;
                        f = tand != 0;
                        if ((u2 & D_800F2F78[idx].h22) > 0) {
                            f |= 2;
                        }
                        fb = f;
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].h28) > 0) {
                            fb |= 4;
                        }
                        fc = fb;
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].h2A) > 0) {
                            fc |= 8;
                        }
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].h2E) > 0) {
                            fc |= 0x10;
                        }
                        t3 = fc | 0x60;
                    }
                } else {
                    a1 = 0;
                    t1 = 0;
                }
            } else {
                t1 = 0;
                a1 = 0;
            }
            D_800ADAD8[arg1][0] = t5;
            D_800ADAD8[arg1][1] = t1;
            D_800ADAD8[arg1][2] = a1;
            D_800ADAD8[arg1][3] = t3;
            if (D_800F4D44 == 4) {
                if (D_800AD508 != 0) {
                    if (arg1 >= 2) {
                        *(int *)D_800ADAD8[arg1] = D_800ACBE0[t2];
                    } else {
                        D_800F3AF0[t2] = *(int *)D_800ADAD8[arg1];
                        *(int *)D_800ADAD8[arg1] = D_800ACBF0[t2];
                    }
                } else {
                    if (arg1 < 2) {
                        *(int *)D_800ADAD8[arg1] = D_800ACBE0[t2];
                    } else {
                        D_800F3AF0[t2] = *(int *)D_800ADAD8[arg1];
                        *(int *)D_800ADAD8[arg1] = D_800ACBF0[t2];
                    }
                }
                __asm__ volatile("" : : "r"(t2)); /* ASM_BARRIER: keep t2 live so t2<<2 allocates v1 */
                if (D_800AD504 == 1) {
                    if (D_800AC5A8 < D_80107188) {
                        D_800AD510[arg1][D_800AC5A8] = *(U4 *)D_800ADAD8[arg1];
                        if (arg1 == 3) {
                            D_800AC5A8 += 1;
                        }
                    }
                }
            } else if (D_800AD504 == 1) {
                if (D_800AC5A8 < D_80107188) {
                    D_800AD510[arg1][D_800AC5A8] = *(U4 *)D_800ADAD8[arg1];
                    if (arg1 == 1 || D_800F4D44 < 2 || D_800F4D44 == 6) {
                        D_800AC5A8 += 1;
                    }
                }
            }
        }
    }
}
