struct R { short x, y, w, h; };
extern int func_80092D50(), func_80092F64(), func_80092FFC();

void func_8001E854(int a0, int a1, int a2)
{
    struct R r;
    unsigned short *s;
    unsigned t;
    if (a1 < 0) {
        r.x = a2 * 16 + 0x280;
        r.y = 0x40;
        r.w = 0x10;
        r.h = 1;
        func_80092F64(&r, 0, 0, 0);
        goto done;
    }
    a0 += a1 * 0x840;
    a0 += 4;
    t = *(unsigned *)a0;
    a0 += 4;
    if (t & 8) {
        s = (unsigned short *)a0;
        r.x = a2 * 16 + 0x280;
        r.y = 0x40;
        r.w = s[4];
        r.h = s[5];
        func_80092FFC(&r, (char *)s + 0xC);
        func_80092D50(0);
        a0 = (int)s + ((*(unsigned *)s >> 2) << 2);
    }
    s = (unsigned short *)a0;
    r.x = a2 * 16 + 0x280;
    r.y = 0;
    r.w = s[4];
    r.h = s[5];
    if (r.w > 0) {
        if (r.h > 0) {
            func_80092FFC(&r, (char *)s + 0xC);
done:
            func_80092D50(0);
        }
    }
}
