extern int func_800195BC();
extern int func_800912B4();
extern int func_80093B14();
extern int func_80093B94();

typedef struct { char *cur; char *ot; } SPD;
#define GS ((SPD *)0x1F800000)
#define W(o) (*(int *)(GS->cur + (o)))
#define LNK(n, t) \
    W(0) = *(int *)(GS->ot + (t)); \
    *(char *)(GS->cur + 3) = (n); \
    *(int *)(GS->ot + (t)) = (int)GS->cur; \
    *(char *)(GS->ot + (t) + 3) = 0

void func_800197C8(int a0, int a1, short *a2, int a3) {
    short sp10[4];
    int c; 
    register int *p __asm__("$4"); /* ASM_BARRIER: pin temp reg to match original allocation */
    int k; 
    int m; 
    int *q, *p1;
    int n;

    W(8) = 0x65000000;
    W(0xC) = (a2[1] << 16) | a2[0];
    W(0x14) = a2[2] | 0x01000000;
    W(0x10) = 0;
    W(4) = 0xE1000307;
    LNK(5, 0x10);
    p1 = (int *)(GS->cur + 0x18);
    GS->cur = (char *)p1;
    if (a2[3] >= 0x101) {
        p1[2] = 0x65000000;
        W(0xC) = ((a2[1] + 0x100) << 16) | a2[0];
        W(0x14) = ((a2[3] - 0x100) << 16) | a2[2];
        W(0x10) = 0;
        W(4) = 0xE1000317;
        LNK(5, 0x10);
        GS->cur = GS->cur + 0x18;
    }
    sp10[2] = 0x140;
    sp10[0] = 0;
    sp10[1] = 0;
    sp10[3] = 0x1E0;
    func_80093B14(GS->cur, sp10);
    LNK(2, 0x10);
    GS->cur = GS->cur + 0xC;
    func_80093B94(GS->cur, sp10);
    LNK(2, 0x10);
    GS->cur = GS->cur + 0xC;
    func_800912B4((short)(unsigned short)a2[2] / 2, (short)(unsigned short)a2[3] / 2);
    func_800195BC(a0, a1);
    c = a3 << 16;
    n = a3 << 8;
    m = 0x66000000;
    n = n | m;
    c = c | n;
    c = c | a3;
    p = (int *)GS->cur;
    p[2] = c;
    p[3] = 0xFFFF0000;
    p[5] = a2[2] | 0x01000000;
    p[4] = 0;
    p[1] = 0xE1000307;
    p[0] = *(int *)(GS->ot + 0x10);
    *(char *)((int)p + 3) = 5;
    *(int *)(GS->ot + 0x10) = (int)p;
    *(char *)(GS->ot + 0x13) = 0;
    p = p + 6;
    q = p;
    q[2] = c;
    q[3] = 0xFF0000;
    q[5] = ((a2[3] - 0x100) << 16) | a2[2];
    q[4] = 0;
    k = 0xE1000317;
    q[1] = k;
    q[0] = *(int *)(GS->ot + 0x10);
    *(char *)((int)q + 3) = 5;
    *(int *)(GS->ot + 0x10) = (int)q;
    *(char *)(GS->ot + 0x13) = 0;
    p = p + 6;
    GS->cur = (char *)p;
    p[1] = 0x60000000;
    W(8) = 0;
    W(0xC) = (a2[3] << 16) | a2[2];
    W(0) = *(int *)(GS->ot + 0xAF0);
    *(char *)(GS->cur + 3) = 3;
    *(int *)(GS->ot + 0xAF0) = (int)GS->cur;
    *(char *)(GS->ot + 0xAF3) = 0;
    sp10[0] = 0x1C0;
    sp10[1] = 0;
    p = (int *)(GS->cur + 0x10);
    GS->cur = (char *)p;
    sp10[2] = a2[2];
    sp10[3] = a2[3];
    func_80093B14(p, sp10);
    LNK(2, 0xAF0);
    GS->cur = GS->cur + 0xC;
    func_80093B94(GS->cur, sp10);
    LNK(2, 0xAF0);
    GS->cur = GS->cur + 0xC;
}
