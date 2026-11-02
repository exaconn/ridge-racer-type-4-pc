typedef struct { char p[0xA]; unsigned short b; char q[0x5C - 0xC]; } E;
extern E D_800F3A38[];
extern int D_8009FDA4;
extern int func_8004F24C();

void func_8002B330(a0, a1, a2, a3, a4) int a0; int a1; char *a2; int a3; int a4; {
    int s0;
    int a;
    unsigned short b;
    switch (a1) {
    case 2:
        s0 = *(short *)a2;
        a = (a0 >> 2) & 1;
        break;
    case 4:
        s0 = *(int *)a2;
        a = (a0 >> 2) & 1;
        break;
    case 1:
    case -1:
        s0 = *(unsigned char *)a2;
        a = (a0 >> 2) & 1;
        break;
    default:
        a = (a0 >> 2) & 1;
        break;
    }
    if ((a0 & 1) == 0) {
        b = D_800F3A38[a].b;
        if ((b & 0x8000) == 0) {
            if ((b & 0x2000) == 0) {
                if (b & 0x1000) {
                    s0 -= 1;
                    goto L;
                }
                if (b & 0x4000) {
                    s0 += 1;
                    goto L;
                }
            }
        }
    } else {
        b = D_800F3A38[a].b;
        if ((b & 0x1000) == 0) {
          if ((b & 0x4000) == 0) {
            if (b & 0x8000) {
                D_8009FDA4 = 1;
                s0 -= 1;
                goto L;
            }
            if (b & 0x2000) {
                D_8009FDA4 = 2;
                s0 += 1;
                goto L;
            }
          }
        }
        D_8009FDA4 = 0;
        goto M;
    }
    goto M;
L:
    if ((a0 & 2) == 0) {
        if (s0 < a3) {
            if (s0 >= a4) {
                func_8004F24C(0, a);
            } else {
                s0 = a4;
            }
        } else {
            s0 = a3 - 1;
        }
    } else {
        func_8004F24C(0, a);
        if (s0 >= a3) {
            s0 = a4;
        } else if (s0 < a4) {
            s0 = a3 - 1;
        }
    }
M:
    switch (a1) {
    case 2: *(short *)a2 = s0; break;
    case 4: *(int *)a2 = s0; break;
    case 1:
    case -1: *(char *)a2 = s0; break;
    }
}
