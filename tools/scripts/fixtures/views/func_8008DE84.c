/* MASPSX_FLAGS: --assembler-profile=gnu */

/* GCC_VERSION: 2.7.2 */

/* CC1_FLAGS: -fsigned-char -mhard-float */

/* legacy typing of shared globals in this part of the unit */
#define func_80096414 ((int (*)(void *, const char *, int))func_80096414)

#undef func_80096414

typedef struct {
    unsigned char pos[4];
    unsigned int size;
    char name[16];
} CdlFILE;

extern int D_800A67A4;

extern int CD_nopen;

extern int CD_debug;

extern CdlFILE D_80110350[];

extern const char D_80011B38[];

extern const char D_80011B54[];

extern const char D_80011B6C[];

extern const char D_80011B88[];

extern const char D_80011BA8[];

extern const char D_80011BB4[];

extern int DS_cachefile(int);

extern int func_8001E84C();

CdlFILE *CdSearchFile(CdlFILE *fp, char *name) {
    char buf[16];
    char *p;
    char *d;
    int i;
    int dir;
    /* ASM_BARRIER: use hard reg $0 so jump2 cross-jumping does not merge this return with .L8008E130 */
    register CdlFILE *zero asm("$0");

    if (D_800A67A4 != CD_nopen) {
        if (DS_newmedia() == 0)
            return 0;
        D_800A67A4 = CD_nopen;
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
            if ((dir = DS_searchdir(dir, buf)) == -1)
                goto err;
            i++;
        } while (i < 8);
    }
done:
    if (i >= 8) {
        if (CD_debug > 0)
            func_8001E84C(D_80011B38, name, i);
        return 0;
    }
    if (buf[0] == 0) {
        if (CD_debug > 0)
            func_8001E84C(D_80011B54, name);
        return 0;
    }
    *d = 0;
    if (DS_cachefile(dir) == 0) {
        if (CD_debug > 0)
            func_8001E84C(D_80011B6C);
        return 0;
    }
    if (CD_debug >= 2)
        func_8001E84C(D_80011B88, buf);
    for (i = 0; i < 64; i++) {
        if (D_80110350[i].name[0] == 0)
            break;
        if (func_8008E15C(D_80110350[i].name, buf)) {
            if (CD_debug >= 2)
                func_8001E84C(D_80011BA8, buf);
            *fp = D_80110350[i];
            return &D_80110350[i];
        }
    }
    if (CD_debug > 0)
        func_8001E84C(D_80011BB4, buf);
    return 0;
}
