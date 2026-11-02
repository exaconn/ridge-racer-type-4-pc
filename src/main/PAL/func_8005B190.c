extern int func_8005B8D8();
extern int func_8005D500();
extern int func_8005ECD4();
extern int func_8005ABAC();
struct Q { char pad[0x60]; int f60; int *f64; short f68, f6a; int f6c; int f70; };
extern struct { char pad[0x3D8]; struct Q *q; } D_800AC650;
extern int *D_800A1F88[];
extern int D_800F7620;

void func_8005B190(void) {
    D_800AC650.q->f68 = 0;
    D_800AC650.q->f6a = 0;
    D_800AC650.q->f60 = 0;
    D_800AC650.q->f64 = D_800A1F88[D_800F7620];
    D_800AC650.q->f6c = ((short *)D_800AC650.q->f64)[D_800AC650.q->f68 * 4];
    D_800AC650.q->f70 = ((short *)D_800AC650.q->f64)[D_800AC650.q->f68 * 4 + 1];
    switch (D_800AC650.q->f6c) {
    case 0:
        func_8005B8D8(D_800AC650.q->f70);
        break;
    case 1:
        func_8005D500(D_800AC650.q->f70);
        break;
    case 2:
        func_8005ECD4(D_800AC650.q->f70);
        break;
    case 3:
        func_8005ABAC(D_800AC650.q->f70);
        break;
    }
}
