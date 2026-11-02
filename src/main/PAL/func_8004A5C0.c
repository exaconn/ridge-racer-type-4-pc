extern unsigned short D_8009FFEC[][8];
extern unsigned short D_800A006C[][8];
extern unsigned short D_800A00EC[][8];
extern unsigned short D_800F2F78[][3][8];
extern unsigned short D_800F4BF8[][2][8];
void func_8004A5C0(int a, int b, int c, int d) {
    int i;
    for (i = 0; i < 8; i++) {
        D_800F2F78[a][0][i] = D_8009FFEC[b][i];
        D_800F2F78[a][1][i] = D_800A006C[c][i];
        D_800F2F78[a][2][i] = D_800A00EC[d][i];
        D_800F4BF8[0][0][i] = D_800A00EC[b][i];
        D_800F4BF8[0][1][i] = D_800A006C[c][i];
    }
}
