/* RODATA 0x8001140C 0x4c */
extern int D_8010EF88[];
extern int D_8010EF90[];
extern unsigned char D_800A409C[];
extern unsigned char D_800F3A24[];
extern unsigned char D_8010B2DC[];
extern unsigned char D_800ACBCC[];
extern int func_800787C4();
extern int func_8007834C();
extern int func_800783C8();

void func_80078E40(a0, a1, a2)
int a0; int a1; int a2;
{
    int r, flag, k, s3, t, v;
    /* ASM_BARRIER: pin loop counter to s2 to reproduce original register numbering */
    register int i asm("$18");
    unsigned char *p0, *p1, *pp, *b0;

    k = a0 & 0xFF;
    r = func_800787C4(k, a1 & 0xFF);
    D_8010EF88[k] = r;
    if (r != 0) {
        D_8010EF90[k] = r;
        switch (r) {
        case -2: D_800A409C[0]++; break;
        case -1: D_800A409C[1]++; break;
        case 1: D_800A409C[2]++; break;
        case 2: D_800A409C[3] += 3; break;
        case -3: D_800A409C[4]++; break;
        case 3: D_800A409C[5]++; break;
        case 4: D_800A409C[6] += 3; break;
        case -4: D_800A409C[7]++; break;
        default: D_800A409C[8]++; break;
        }
    } else {
        D_8010EF90[k] = 0;
    }
    i = 0;
    flag = 0;
    b0 = D_800F3A24;
    s3 = a0 & 0xFF;
    p0 = b0 + s3;
    p1 = D_8010B2DC + s3;
loop:
    if (D_800A409C[i] >= 3) {
        switch (i) {
        case 0:
            *p0 = 0x14; *p1 = 0; flag = 1; break;
        case 1:
            *p0 = 0x1E; *p1 = 0; flag = 1; break;
        case 2:
            if (*p0 >= 0x10) {
                D_800ACBCC[s3] = func_8007834C(s3, a1 & 0xFF);
                *p0 = 0xF - func_800783C8(D_800ACBCC[s3]);
                *p1 = 0;
            }
            flag = 1; break;
        case 3:
            if (*p0 >= 0x10) { *p0 = 0x32; *p1 = 0; }
            flag = 1; break;
        case 4:
            *p0 = 0x14; *p1 = 1; flag = 1; break;
        case 5:
            if (*p0 >= 0x10) {
                D_800ACBCC[s3] = func_8007834C(s3, a1 & 0xFF);
                *p0 = 0xF - func_800783C8(D_800ACBCC[s3]);
                *p1 = 1;
            }
            flag = 1; break;
        case 6:
            if (*p0 >= 0x10) { *p0 = 0x32; *p1 = 1; }
            flag = 1; break;
        case 7:
            if (*p0 >= 0x10) { *p0 = 0x28; *p1 = 0; }
            flag = 1; break;
        case 8:
            if (*p0 >= 0x10) { *p0 = 0x3C; *p1 = 0; }
            flag = 1; break;
        }
    }
next:
    i++;
    if (i < 9) goto loop;
    if (flag == 1 || (short)a2 == 1) {
        i = 8; pp = D_800A409C + i; do { *pp-- = 0; } while (--i >= 0);
    }
}
