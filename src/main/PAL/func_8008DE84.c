/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 2.7.2 */
/* CC1_FLAGS: -fsigned-char -mhard-float */

typedef struct {
    unsigned char pos[4];
    unsigned int size;
    char name[16];
} CdlFILE;

extern int D_800A67A4;
extern int D_800A649C;
extern int D_800A6490;
extern CdlFILE D_80110350[];

extern const char D_80011B38[];
extern const char D_80011B54[];
extern const char D_80011B6C[];
extern const char D_80011B88[];
extern const char D_80011BA8[];
extern const char D_80011BB4[];

extern int func_8008E17C(void);
extern int func_8008E440(int, char *);
extern int func_8008E4E4(int);
extern int func_8008E15C(char *, char *);
extern int func_8001E84C();

CdlFILE *func_8008DE84(CdlFILE *fp, char *name) {
    char buf[16];
    char *p;
    char *d;
    int i;
    int dir;
    /* ASM_BARRIER: use hard reg $0 so jump2 cross-jumping does not merge this return with .L8008E130 */
    register CdlFILE *zero asm("$0");

    if (D_800A67A4 != D_800A649C) {
        if (func_8008E17C() == 0)
            return 0;
        D_800A67A4 = D_800A649C;
    }
    if (*name != '\\') {
        return zero;
    err:
        buf[0] = 0;
    } else {
        buf[0] = 0;
        dir = 1;
        p = name;
        i = 0;
        do {
            for (d = buf; *p != '\\'; d++) {
                if (*p == 0)
                    goto done;
                *d = *p++;
            }
            if (*p == 0)
                goto done;
            p++;
            *d = 0;
            if ((dir = func_8008E440(dir, buf)) == -1)
                goto err;
            i++;
        } while (i < 8);
    }
done:
    if (i >= 8) {
        if (D_800A6490 > 0)
            func_8001E84C(D_80011B38, name, i);
        return 0;
    }
    if (buf[0] == 0) {
        if (D_800A6490 > 0)
            func_8001E84C(D_80011B54, name);
        return 0;
    }
    *d = 0;
    if (func_8008E4E4(dir) == 0) {
        if (D_800A6490 > 0)
            func_8001E84C(D_80011B6C);
        return 0;
    }
    if (D_800A6490 >= 2)
        func_8001E84C(D_80011B88, buf);
    for (i = 0; i < 64; i++) {
        if (D_80110350[i].name[0] == 0)
            break;
        if (func_8008E15C(D_80110350[i].name, buf)) {
            if (D_800A6490 >= 2)
                func_8001E84C(D_80011BA8, buf);
            *fp = D_80110350[i];
            return &D_80110350[i];
        }
    }
    if (D_800A6490 > 0)
        func_8001E84C(D_80011BB4, buf);
    return 0;
}
