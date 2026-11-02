typedef struct { char pad[0x568]; int *f568; } T;
extern T D_800AC650;
void func_8004D4DC(int *a) {
    D_800AC650.f568 = a;
    a[4] += (int)a;
    D_800AC650.f568[5] += (int)a;
    D_800AC650.f568[6] += (int)a;
    D_800AC650.f568[7] += (int)a;
    D_800AC650.f568[8] += (int)a;
    D_800AC650.f568[9] += (int)a;
    D_800AC650.f568[10] += (int)a;
}
