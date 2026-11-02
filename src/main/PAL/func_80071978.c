typedef struct {
    int x;
} __attribute__((packed)) U4;

typedef struct {
    unsigned short v0[4];
    unsigned short v1[4];
    unsigned short v2[4];
    int pad[5];
    int c0;
    int c1;
    int c2;
} InTri;

typedef struct {
    char pad0[0x78];
    U4 sxy0;
    U4 sxy1;
    U4 sxy2;
    char pad84[0xB4 - 0x84];
    unsigned short v0[6];
    unsigned short v1[6];
    unsigned short v2[6];
} Sp7C;

typedef struct {
    unsigned int tag;
    int c0;
    U4 sxy0;
    int c1;
    U4 sxy1;
    int c2;
    U4 sxy2;
} PolyG3;

extern int func_80091764();
extern void func_80095A44();

void func_80071978(InTri *a0, int a1, int a2) {
    int sp20;
    int sp24;
    int i;
    int otz;
    unsigned int *ot;
    unsigned int *ot_base;
    int *hw = (int *)0x1F800000;
    PolyG3 *pkt = (PolyG3 *)hw[0];
    Sp7C *sp = (Sp7C *)0x1F80007C;

    for (i = 0; i < a1; i++, a0++) {
        sp->v0[0] = a0->v0[0];
        sp->v0[1] = a0->v0[1];
        sp->v0[2] = a0->v0[2];
        sp->v1[0] = a0->v1[0];
        sp->v1[1] = a0->v1[1];
        sp->v1[2] = a0->v1[2];
        sp->v2[0] = a0->v2[0];
        sp->v2[1] = a0->v2[1];
        sp->v2[2] = a0->v2[2];
        otz = func_80091764(sp->v0, sp->v1, sp->v2, &sp->sxy0, &sp->sxy1, &sp->sxy2, &sp20, &sp24) >> (hw[0x18] + a2);
        if (otz > 0 && otz < 0x1C0) {
            if (a2 == 2) {
                otz = 0x1BF;
            }
            ot_base = (unsigned int *)hw[1];
            pkt->sxy0 = sp->sxy0;
            pkt->sxy1 = sp->sxy1;
            pkt->sxy2 = sp->sxy2;
            pkt->c0 = a0->c0;
            pkt->c1 = a0->c1;
            pkt->c2 = a0->c2;
            pkt->tag = 0x06000000;
            ((char *)pkt)[7] = 0x32;
            ot = ot_base + (otz + 0x80);
            func_80095A44(ot, pkt++);
            pkt->c0 = 0xE1000220;
            pkt->tag = *ot;
            ((char *)pkt)[3] = 1;
            *ot = ((unsigned int)pkt << 8) >> 8;
            pkt = (PolyG3 *)((char *)pkt + 8);
        }
    }
    hw[0] = (int)pkt;
}
