struct V8 { unsigned char c[8]; };
struct M { int a[8]; };
extern struct V8 D_800101E4;
extern struct { char pad[0xA8]; unsigned short v; } *D_8010BF78;
extern int func_800197C8();
extern int func_80091FF4();
extern int func_80092194();
extern int func_80092334();
extern char D_8009E88C[];

void func_8001BEF8(void)
{
    struct V8 sp10;
    struct M sp18;
    int t;
    sp10 = D_800101E4;
    D_8010BF78->v += 0x20;
    sp18.a[4] = 0x1000;
    sp18.a[2] = 0x1000;
    sp18.a[3] = 0;
    sp18.a[1] = 0;
    t = (short)D_8010BF78->v;
    sp18.a[0] = 0x1000;
    func_80092194(t, &sp18);
    func_80091FF4(0x100, &sp18);
    func_80092334(0xAA, &sp18);
    func_800197C8(D_8009E88C, &sp18, &sp10, 0xC8);
}
