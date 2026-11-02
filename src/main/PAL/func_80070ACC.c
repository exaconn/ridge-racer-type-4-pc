typedef struct { int a[8]; } M;
extern int func_8006E030();
extern int func_8006E09C();
extern int func_8006E108();
extern int func_80090B14();
extern int func_80090CA4();
extern int func_80090DB4();
extern int func_800910B4();
extern int func_80091144();
extern M D_800F39A8;

void func_80070ACC(char *p) {
    M m10, m30;
    int v[3];
    func_8006E09C(&m10, *(int *)(p + 0xDC));
    func_8006E108(&m30, *(int *)(p + 0xD8));
    func_80090DB4(&m30, &m10);
    func_8006E030(&m30, *(int *)(p + 0xE0));
    func_80090DB4(&m30, &m10);
    func_80090B14(&D_800F39A8, p + 0xC8, v);
    m10.a[5] = (D_800F39A8.a[5] + v[0]) >> 1;
    m10.a[6] = (D_800F39A8.a[6] + v[1]) >> 1;
    m10.a[7] = (D_800F39A8.a[7] + v[2]) >> 1;
    func_80091144(&m10);
    func_80090CA4(&m10, &D_800F39A8);
    func_800910B4(&m10);
}
