typedef struct {
    int w0, w4, w8, wC;
    char pad10[4];
    char b14;
    char pad15[7];
    char b1C;
    char pad1D[3];
    signed char b20;
    unsigned char b21;
    signed char b22;
    signed char b23;
    char pad24[0x54 - 0x24];
    short h54;
    char pad56[0x88 - 0x56];
    int w88;
    char pad8C[4];
    int w90;
    char pad94[4];
    int w98;
    char pad9C[0xB0 - 0x9C];
} Ch;
extern Ch *D_8010F068[];
extern int func_80083994(), func_80089E1C();

void func_800833A0(a0, a1)
int a0;
int a1;
{
    register Ch **a3 __asm__("$7"); /* ASM_BARRIER: table slot pointer in a3 as in the original */
    register int p0 __asm__("$18"); /* ASM_BARRIER: original copies arg0 into s2 after the offset math */
    register int p1 __asm__("$17"); /* ASM_BARRIER: original copies arg1 into s1 */
    register int a2 __asm__("$6"); /* ASM_BARRIER: channel offset in a2 as in the original */
    Ch *s0;
    int v1, off;

    a3 = (Ch **)((int)(short)a0 * 4 + (int)D_8010F068);
    a2 = (short)a1 * 0xB0;
    s0 = (Ch *)((int)*a3 + a2);
    p0 = a0;
    p1 = a1;
    v1 = s0->b20;
    s0->b21++;
    if (v1 == 0) {
        s0->w88 = 0;
        s0->b1C = 0;
        s0->w90 = 0;
        if (((Ch *)(a2 + (int)*a3))->w98 & 0x400) {
            s0->w0 = s0->wC;
        } else {
            s0->w0 = s0->w4;
        }
        return;
    }
    if ((signed char)s0->b21 < v1) {
        s0->w88 = 0;
        s0->b1C = 0;
        s0->w90 = 0;
        if (((Ch *)(a2 + (int)*a3))->w98 & 0x400) {
            v1 = s0->wC;
            s0->w0 = s0->wC;
        } else {
            v1 = s0->w4;
            s0->w0 = s0->w4;
        }
        s0->w8 = v1;
        return;
    }
    ((Ch *)(a2 + (int)*a3))->w98 &= ~1;
    ((Ch *)(a2 + (int)*a3))->w98 &= ~8;
    ((Ch *)(a2 + (int)*a3))->w98 &= ~2;
    ((Ch *)(a2 + (int)*a3))->w98 |= 0x200;
    ((Ch *)(a2 + (int)*a3))->w98 |= 4;
    s0->b14 = 0;
    if (((Ch *)(a2 + (int)*a3))->w98 & 0x400) {
        s0->w8 = s0->wC;
    } else {
        s0->w8 = s0->w4;
    }
    if (s0->b22 != -1) {
        s0->b14 = 0;
        func_80083994(s0->b22, s0->b23);
        func_80089E1C((short)(p0 | (p1 << 8)));
    }
    func_80089E1C((short)(p0 | (p1 << 8)));
    s0->w90 = s0->h54;
}
