extern void func_80085AEC(short);
struct S { int a; int b; int c; int d; char pad[0x610]; };
extern struct S D_800F4DF0[];
void func_8004FFA4(int arg0) {
 int i; int s0; struct S *t = &D_800F4DF0[arg0]; s0 = t->c; i = 0; while (i < t->d) { func_80085AEC(s0); i++; s0++; }
 t->a = 0;
}
