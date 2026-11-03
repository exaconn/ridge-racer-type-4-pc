/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 2.7.2 */
/* CC1_FLAGS: -fsigned-char */
#include "psyq/libcd/dsread.h"
#include "psyq/libcd/cdfile.h"
#include "psyq/libcd/dsfile.h"

typedef struct {
    int x;
} __attribute__((packed)) U4;

/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 2.7.2 */
/* CC1_FLAGS: -fsigned-char */

typedef struct {
    unsigned char pos[4];
    int size;
    char name[16];
} CdlFILE;

struct DirPath {
    int a;
    int b;
    int c;
    char name[32];
};

extern int D_800A6490;
extern struct DirPath D_8011092C[];
extern CdlFILE D_80110350[];
extern const char D_80011CA4[];
extern const char D_80011CC4[];
extern unsigned short D_80011CE0;
extern short D_80011CE4;
extern char D_80011CE6;
extern const char D_80011CE8[];
extern const char D_80011D04[];

extern int func_8001E84C();
extern void func_8008C5A0(int, void *);
extern void func_80096444(char *, void *, int);

int func_8008E4E4(int dir) {
    int sp18;
    unsigned char *p;
    int i;

    if (dir == D_800A67A0) {
        return 1;
    }
    if (func_8008E780(1, D_8011092C[dir].a, D_80111F50) != 1) {
        if (D_800A6490 > 0) {
            func_8001E84C(D_80011CA4);
            return -1;
        }
        return -1;
    }
    p = D_80111F50;
    if (D_800A6490 >= 2) {
        func_8001E84C(D_80011CC4);
    }
    i = 0;
    while (p < D_80111F50 + 0x800) {
        if (p[0] == 0) {
            break;
        }
        *(U4 *)&sp18 = *(U4 *)(p + 2);
        func_8008C5A0(sp18, D_80110350[i].pos);
        *(U4 *)&D_80110350[i].size = *(U4 *)(p + 10);
        switch (i) {
        case 0:
            __builtin_memcpy(D_80110350[0].name, &D_80011CE0, 2);
            break;
        case 1:
            __builtin_memcpy(D_80110350[1].name, &D_80011CE4, 3);
            break;
        default:
            func_80096444(D_80110350[i].name, p + 0x21, p[0x20]);
            D_80110350[i].name[p[0x20]] = 0;
            break;
        }
        if (D_800A6490 >= 2) {
            func_8001E84C(
                D_80011CE8,
                D_80110350[i].pos[0],
                D_80110350[i].pos[1],
                D_80110350[i].pos[2],
                D_80110350[i].size,
                D_80110350[i].name
            );
        }
        p += p[0];
        i++;
        if (i >= 0x40) {
            break;
        }
    }
    D_800A67A0 = dir;
    if (i < 0x40) {
        D_80110350[i].name[0] = 0;
    }
    if (D_800A6490 >= 2) {
        func_8001E84C(D_80011D04, i);
    }
    return 1;
}
