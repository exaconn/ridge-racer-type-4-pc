#include "rr/gpu.h"

#include "rr/animated_glyph.h"

/* legacy typing of shared globals in this part of the unit */
#define func_80093C24 ((s32 (*)())func_80093C24)

#define func_80095A44 ((s32 (*)())func_80095A44)

#define func_80095B94 ((s32 (*)())func_80095B94)

#define D_800ACC14 ((char *)D_800ACC14)

#undef func_80093C24

#undef func_80095A44

#undef func_80095B94

#undef D_800ACC14

/* legacy typing of shared globals in this part of the unit */
#define func_80095A44 ((void (*)())func_80095A44)

#define func_80095AF4 ((void (*)())func_80095AF4)

#define func_80095B24 ((void (*)())func_80095B24)

#define func_80095B94 ((void (*)())func_80095B94)

#undef func_80095A44

#undef func_80095AF4

#undef func_80095B24

#undef func_80095B94

extern s32 func_80095AF4();

extern s32 func_80095B24();

extern u8 D_8009ED38[];

void func_8001F40C(a0, a1, a2)
s32 a0, a1;
u8 *a2;
{
    RrSpritePacket *s3, *s2;
    s32 c, a, v, row, col, u, vv;
    s3 = *(RrSpritePacket **)0x1F800000;
    s2 = s3;
    for (; *a2 != 0; a0 += 10) {
            a = *a2 - 0x20;
            a2++;
            if (a != 0) {
                a = D_8009ED38[a * 2] + (D_8009ED38[a * 2 + 1] << 5);
                row = a / 21;
                col = a % 21;
                u = col * 12;
                vv = row * 24 + 0x24;
                func_80095B94(s3);
                s2->w = 12;
                s2->h = 24;
                func_80095B24(s3, 1);
                func_80095AF4(s3, 1);
                s2->x = a0;
                s2->y = a1;
                s2->u = u;
                s2->v = vv;
                s2->clut = 0x5B20;
                func_80095A44(D_800ACC14 + 0x70, s2++);
                s3++;
            }
    }
    func_80093C24(s3, 0, 1, 0x58, 0);
    func_80095A44(D_800ACC14 + 0x70, s3);
    *(char **)0x1F800000 = (char *)s3 + 0xC;
}
