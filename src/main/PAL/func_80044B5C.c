typedef struct { char pad[0x57C]; } B;
typedef struct { int a; int b; } P;
typedef struct { int pad[20]; P e[2]; } S;
extern B D_800D0DD8[];
extern B D_800AE660[];
extern S D_800AC650;

void func_80044B5C(void) {
    int i;
    for (i = 0; i < 2; i++) {
        D_800AC650.e[i].a = (int)&D_800AE660[i];
        D_800AC650.e[i].b = (int)&D_800D0DD8[i];
    }
}
