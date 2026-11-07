/* RODATA 0x800107c4 0x74 */
#include "game/data/bankrec.h"
#include "game/data/cdfile2.h"
#include "game/data/trkld1.h"
#include "game/data/trkld6.h"
#include "game/menu/mntrow.h"
#include "game/sound/sndcmd.h"
#include "game/car/carmove1.h"
#include "game/data/cdfile2.h"
#include "game/data/trkld1.h"
#include "game/menu/menuslot.h"
#include "game/data/ldscn1.h"
#include "game/data/ldscn4.h"
#include "game/menu/mnrows.h"
#include "game/race/raceinfo.h"
#include "game/data/cdfile1.h"
#include "game/data/cdfile2.h"
#include "rr/audio_loop.h"
#include "game/data/ldscn1.h"
typedef struct { char p[7]; char b; } Z;

extern int func_8004E008();
extern short RaceContext[];
extern unsigned char D_800F4D7C;
extern int CdDirectoryBuffer;
extern int LoadCursor;
extern int D_800F4D40;
extern int D_800F4D70;
extern int InitMenuArenas();
extern int D_800F4D74;
extern short D_800F3914;
extern int func_8001E854();
extern struct { short a; short b; } D_800ACBD0[];
extern char *D_800F3B24;
extern char D_800F3B28[];
extern unsigned char D_800F4D98[][4];
extern int func_800436E8();
extern char D_800FFC48[];
extern int func_8005AF5C();
extern int func_8005B2D4();
extern int func_80053BB4();
extern int func_80043608();
extern struct { char pad[4]; signed char f4; char pad2[0x1B]; int f20; } *D_800FF68C;
extern int D_800F4CA8;
extern struct { char pad[0x17]; signed char f17; char pad2[2]; short f1A; unsigned char f1C; } D_800AD4D8;
extern int D_800AD564;
extern unsigned char D_800AD4F4;
extern char D_80104DB0[];
extern Z *D_800AC590;
extern char *TrackNodes;
extern int TrackNodeCount;
extern char D_8010C220[];

int func_8003A79C(void);
int func_8003A874(void);
int func_8003A9E8(void);
int func_8003AAE8(void);
int func_8003ABA4(void);
int func_8003AC50(void);
int func_8003AE4C(int arg0, int arg1);
int func_8003B040(void);
int func_8003B12C(void);
int func_8003B294(void);
int func_8003B398(void);
int func_8003B47C(void);
int func_8003B588(void);
int func_8003B934(void);
int func_8003BAB0(void);
int func_8003BBBC(void);

int func_8003A79C(void) {
    int r = 0;
    int i;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
        func_8004E008();
    case 1:
        if (func_8004E5E8() == 1) {
            D_800F4D7C = 2;
    case 2:
            i = 0;
            do {
                RaceContext[i] = i + 1;
                i++;
            } while (i < 7);
            RaceContext[7] = 0;
            D_800F4D7C = 3;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

int func_8003A874(void) {
    int r = 0;
    int v;
    switch (D_800F4D7C) {
    case 0:
        if (D_800F4D44 == 4) {
            r = 1;
            D_800F4D70 = D_800F4D40;
            LoadCursor = D_800F4D40;
            break;
        }
        if (D_800F4D44 == 5 && D_800F4D2C == 6) {
            r = 1;
            LoadCursor = D_800F4D40;
            break;
        }
        D_800F4D7C = 1;
        LoadCursor = CdDirectoryBuffer;
        D_8009F8FC = *(unsigned char *)&D_800F4D44;
        if (*(unsigned char *)&D_800F4D44 >= 5) {
            D_8009F8FC = 0;
        }
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, D_8009F8FC + 0xB), LoadCursor)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        v = CdFileBytesLoaded();
        if (v) {
            LoadCursor += v;
    case 3:
            r = 1;
            D_800F4D40 = LoadCursor;
        }
        break;
    }
    return r;
}

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)
#define D_800F4D2C (*(unsigned char (*))&D_800F4D2C)

int func_8003A9E8(void) {
    int r = 0;
    int v;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        LoadCursor = CdDirectoryBuffer;
        D_8009F8FC = D_800F4D2C;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, D_8009F8FC + 1), LoadCursor)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        v = CdFileBytesLoaded();
        if (v) {
            int t;
            D_800F4D7C = 3;
            t = LoadCursor + v;
            LoadCursor = t;
            D_800F4D40 = t;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}

#undef D_8009F8FC
#undef D_800F4D2C

int func_8003AAE8(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        LoadCursor = D_800F4D40;
        func_8004E008();
    case 1:
        if (func_8004E184()) {
            D_800F4D40 = LoadCursor;
            func_8004F600();
            D_800F4D7C = 2;
            InitMenuArenas();
            LoadCursor = D_800F4D74;
    case 2:
            r = 1;
        }
        break;
    }
    return r;
}

int func_8003ABA4(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
    case 1:
        if (RequestSceneLoad(D_800F3914)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (SceneLoadStep()) r = 1;
        break;
    case 3:
        r = 1;
        break;
    }
    return r;
}

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

int func_8003AC50(void) {
    int s0;
    int s1 = 0;
    int r;
    int a1;
    int t;
    char *s2;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 3;
        D_8009F8FC = 0;
    case 3:
        s0 = D_8009F8FC;
        if (s0 < 0) goto l5;
        if (func_80013514(D_800F4D98[s0][1], D_800F4D98[s0][0], D_800F4D98[s0][2] % 20, D_800ACBD0[s0].a, s0) == 1) {
            D_800F4D7C = 4;
        }
        break;
    case 4:
        s0 = D_8009F8FC;
        r = func_800135BC();
        if (r != 1) break;
        a1 = D_800ACBD0[s0].b;
        if (a1 >= 2) {
            s2 = D_800F3B24;
            a1 -= 2;
        } else {
            if (a1 == r) {
                s2 = D_800F3B28 + (s0 & 1) * 0x840;
                a1 = 0;
            } else {
                a1 = -1;
            }
        }
        func_8001E854(s2, a1, s0);
        D_8009F8FC++;
        s1 = -D_8009F8FC;
        if (D_8009F8FC != 4) {
            D_800F4D7C = 3;
        } else {
l5:
            D_800F4D7C = 5;
        }
        break;
    case 5:
        s1 = 1;
        if (D_800F4D44 == 4) D_800F4D70 = LoadCursor;
        break;
    default:
        break;
    }
    return s1;
}

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

int func_8003AE4C(int arg0, int arg1) {
    int r;
    int q4 = 0;
    int a1;
    char *q3;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
        break;
    case 1:
        arg0 += D_8009F8FC;
        if (func_80013514(D_800F4D98[arg0][1], D_800F4D98[arg0][0], D_800F4D98[arg0][2] % 20, D_800ACBD0[arg0].a, arg0) == 1) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        r = func_800135BC();
        if (r != 1) break;
        arg0 += D_8009F8FC;
        a1 = D_800ACBD0[arg0].b;
        if (a1 >= 2) {
            q3 = D_800F3B24;
            a1 -= 2;
        } else {
            if (a1 == r) {
                q3 = D_800F3B28 + (arg0 & 1) * 0x840;
                a1 = 0;
            } else {
                a1 = -1;
            }
        }
        if (D_800F4D44 != 1) {
            func_8001E854(q3, a1, arg0);
        }
        D_8009F8FC++;
        if (D_8009F8FC == arg1) {
            D_800F4D7C = 3;
        } else {
            D_800F4D7C = 1;
        }
        break;
    case 3:
        q4 = 1;
        break;
    default:
        break;
    }
    return q4;
}

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

int func_8003B040(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        LoadCursor = (int)D_800FFC48;
        D_800F4D7C = 1;
        D_8009F8FC = 0;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, D_8009F8FC + 0x11), LoadCursor)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded()) {
            func_800436E8(LoadCursor);
            D_800F4D7C = 3;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}

#undef D_8009F8FC

int func_8003B12C(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        func_8005B2D4(0);
        D_8009F8FC = 0;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, 0xF), LoadCursor)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded()) {
            func_800436E8(LoadCursor);
            D_800F4D7C = 3;
            LoadCursor = D_800ACC1C;
            func_8004E008();
        }
        break;
    case 3:
        if (func_8004E70C()) {
            func_8005AF5C();
            func_8004F600();
            if (D_800F4D44 == 1) {
                goto s7;
            }
            D_800F4D7C = 5;
        }
        break;
    case 5:
        if (func_800137B0()) {
            D_800F4D7C = 6;
        }
        break;
    case 6:
        if (func_800137D4()) {
s7:
            D_800F4D7C = 7;
        }
        break;
    case 7:
        r = 1;
        break;
    }
    return r;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F3B24 (*(int (*))&D_800F3B24)

int func_8003B294(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        if (D_800F4D44 == 1) {
            D_800F4D7C = 3;
        } else {
            D_800F4D7C = 1;
        }
        D_800AC5A0 = CdDirectoryBuffer + 0xAF000;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, 0x12), D_800AC5A0)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded()) {
            D_800F4D7C = 3;
            D_800F3B24 = D_800AC5A0;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}

#undef D_800F3B24

int func_8003B398(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_8009F8FC = 0;
        D_800F4D7C = 1;
        LoadCursor = D_800F4D74;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, 0x13), LoadCursor)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded()) {
            func_800436E8(LoadCursor);
            D_800F4D7C = 3;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}

int func_8003B47C(void) {
    int r = 0;
    int v;
    switch (D_800F4D7C) {
    case 0:
        D_8009F8FC = 0;
        D_800F4D7C = 1;
        GlyphRowCount = 4;
    case 1:
        if (CdFileReadRange(GetR4FileIndex(1, 0x40), 0, 0xC0, LoadCursor)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        v = CdFileBytesLoaded();
        if (v) {
            func_8004139C(LoadCursor, 0);
            D_800F4D7C = 3;
            LoadCursor += v;
        }
        break;
    case 3:
        r = 1;
        break;
    }
    return r;
}

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

int func_8003B588(void) {
    int s3 = 0;
    int s0, s1, s2, a0, v0, k;

    switch (D_800F4D7C) {
    case 0:
        k = 0xAF000;
        *(unsigned char *)&D_8009F8FC = 0;
        D_800AC5A0 = CdDirectoryBuffer + k;
        v0 = D_800AC5A0;
        D_800AC5A0 = v0;
        D_800FF68C = (void *)v0;
        v0 += 0x348;
        D_800AC5A0 = v0;
        if (D_800F4CA8 == 1 && D_800AD4D8.f17 == 7 && D_800AD4D8.f1A == 4) {
            func_80053BB4(1);
        }
        func_80032914(D_800FF68C);
        D_800F4D7C = 1;
        break;
    case 1:
        if (D_800F4CA8 == 1) {
            s2 = 0x41;
            s1 = 0x14;
            s2 = D_800AD4D8.f1C * 9 + (D_800AD4D8.f17 + s1);
        } else {
            s2 = 0x41;
            s1 = 4;
        }
        if (CdFileReadRange(GetR4FileIndex(1, s2), s1 * D_800AD564, s1, D_800AC5A0)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        s2 = CdFileBytesLoaded();
        if (s2) {
            func_80029358(D_800AC5A0);
            D_800F4D7C = 3;
            D_800AC5A0 += s2;
            D_800FF68C->f20 = D_800AC5A0;
        }
        break;
    case 3:
        if (CdFileReadStart(GetR4FileIndex(1, 0x42), D_800AC5A0)) {
            D_800F4D7C = 4;
        }
        break;
    case 4:
        if (CdFileBytesLoaded()) {
            func_80043608(D_800AC5A0);
            D_800F4D7C = 5;
        }
        break;
    case 5:
        s2 = D_800AD4F4 * 9 + 0x1C;
        a0 = D_8009F8FC;
        s0 = D_800ACBFC[D_800FF68C->f4 * 8 + a0];
        if (s0 != -1) {
            if (D_800F4CA8 == 0) {
                s1 = 8;
                s0 <<= 3;
            } else if (D_800F4CA8 == 1) {
                s1 = 8;
                if (a0 == 0) {
                    s1 = 0x2E;
                    s0 = s0 * s1;
                } else {
                    s0 <<= 3;
                    s0 += 0xE6;
                }
            } else {
                s1 = 8;
                s0 <<= 3;
            }
            if (CdFileReadRange(GetR4FileIndex(1, s2), s0, s1, D_800AC5A0)) {
                D_800F4D7C = 6;
            }
        } else {
            D_800F4D7C = 7;
        }
        break;
    case 6:
        s2 = CdFileBytesLoaded();
        if (s2) {
            if (D_8009F8FC == 0) {
                func_800436E8(D_800AC5A0);
            } else {
                D_800AC5A0 = D_800AC5A0 + s2;
            }
            D_8009F8FC = D_8009F8FC + 1;
            s0 = D_800ACBFC[D_800FF68C->f4 * 8 + D_8009F8FC];
            if (D_8009F8FC == 8 || s0 == -1) {
                D_800F4D7C = 7;
            } else {
                D_800F4D7C = 5;
            }
        }
        break;
    case 7:
        s3 = 1;
        break;
    }
    return s3;
}

#undef D_8009F8FC

int func_8003B934(void) {
    int r = 0;
    char *s;
    int a1;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, 0x42), D_800FFC48)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded()) {
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
            if (CdFileReadRange(GetR4FileIndex(1, a1), D_800AD564 << 2, 4, D_80104DB0)) {
                D_800F4D7C = 4;
            }
        }
        break;
    case 4:
        if (CdFileBytesLoaded()) {
            func_80029358(D_80104DB0);
            D_800F4D7C = 5;
    case 5:
            r = 1;
        }
        break;
    }
    return r;
}

int func_8003BAB0(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_8009F8FC = 0;
        D_800F4D7C = 1;
        D_8010BFC8 = 0;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, D_800F3914 + 0x38), LoadCursor) != 0) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded() != 0) {
            SetTrackNodeData(LoadCursor);
            D_800F4D7C = 3;
            D_800F2EC4 = TrackNodes + TrackNodeCount * 60;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}

/* legacy typing of shared globals in this part of the unit */
#define func_800436E8 ((void (*)())func_800436E8)
#define func_8004E008 ((void (*)())func_8004E008)
#define func_8004F600 ((void (*)())func_8004F600)

int func_8003BBBC(void) {
    int r = 0;
    int t;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, 0), LoadCursor) != 0) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded() != 0) {
            func_800436E8(LoadCursor);
            func_8004E008();
            D_800F4D7C = 3;
        }
        break;
    case 3:
        if (func_8004E03C(D_8010C220) != 0) {
            func_8004F600();
            D_800F4D7C = 4;
            LoadCursor = CdDirectoryBuffer;
        }
        break;
    case 4:
        if (CdFileReadStart(GetR4FileIndex(1, 10), LoadCursor) != 0) {
            D_800F4D7C = 5;
        }
        break;
    case 5:
        t = CdFileBytesLoaded();
        if (t != 0) {
            LoadCursor += t;
            D_800F4D7C = 6;
            D_800F4D40 = LoadCursor;
            case 6:
            r = 1;
        }
        break;
    }
    return r;
}

#undef func_800436E8
#undef func_8004E008
#undef func_8004F600
