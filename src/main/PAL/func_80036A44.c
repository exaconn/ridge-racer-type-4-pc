extern int func_8001E84C();
extern int func_8002A344();
extern int func_8002AF8C();
extern int func_80036C08();
extern void func_80036D40(void);
extern int func_80039988();
extern int func_8003A1B4();
extern int func_8003AE4C();
extern int func_8003EC88();
extern char D_800106EC[];
extern char D_800106F8[];
extern int D_8009FDA4;
extern int D_800ACC18;
extern short D_800AD4F2;
extern unsigned D_800F2DE4;
extern short D_800F3A26;
extern int D_800F3A60;
extern struct { char pad[0x10]; short h10; } *D_800F3AFC;
extern short D_800F4C6A;
extern short D_800F4D8A;
extern struct { char pad[4]; signed char b4; char b5; signed char b6; } *D_800FF68C;

void func_80036A44(void) {
    int a;
    int t;
    int r;
    int s1;
    D_800F2DE4 += 1;
    if (D_800FF68C->b4 >= 0x22) {
        func_8003EC88(0);
        a = 0x10006;
    } else {
        a = 0x10007;
    }
    func_8002A344(a);
    if (func_80036C08(D_8009FDA4) != 0) {
        if (D_800F3AFC->h10 == 3) {
            func_80036D40();
            return;
        }
    }
    if (D_800FF68C->b4 == 1) {
        if (D_800F3A60 != 0 || D_800F2DE4 >= 0x1F5) {
            t = D_800FF68C->b6;
            if (t == 4) {
                D_800FF68C->b4 = 0x22;
                D_800F3AFC->h10 = t;
                return;
            }
        }
    }
    if (D_800AD4F2 == 2) {
        r = func_8003AE4C(0, 1);
        if (r == 1) {
            if (D_800F3AFC->h10 == 0xB) {
                func_8002AF8C(0, 1, 0);
                func_8001E84C(D_800106EC);
                s1 = D_800ACC18;
                func_8003A1B4();
                func_80039988();
                D_800F4C6A = r;
                D_800F3A26 = 0x22;
                D_800F4D8A = 0;
                D_800ACC18 = s1;
                func_8001E84C(D_800106F8);
            }
        }
    }
}
