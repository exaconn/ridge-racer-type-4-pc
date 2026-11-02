typedef struct {
    char name[10];
    unsigned short car;
} StructC;

typedef struct {
    char name[10];
    unsigned short car;
    int time;
} Struct10;

extern int D_8009EA04[2][8];
extern StructC D_8009EA44[5];
extern unsigned char D_8009F900[2][8];
extern int D_800AC5B0[8];
extern int D_800ACC28[2][8];
extern Struct10 D_800AD5C8[2][8][5];
extern int D_800F3838[2][8][3];
extern int D_800F3B00[8];
extern int D_800F4CE8[2][8];
extern Struct10 D_800FF700[2][8][5];

void func_8001EEE4(void) {
    int i, j, k;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            int c = 0x2FD;
            D_800F3B00[j] = D_8009EA04[0][j];
            D_800AC5B0[j] = (D_8009EA04[0][j] - 0x2FD) * D_8009F900[1][j] + (D_8009F900[1][j] * 1000 + c);
            D_800ACC28[i][j] = D_8009EA04[1][j];
            D_800F4CE8[i][j] = (D_8009EA04[1][j] - 0x2FD) * D_8009F900[0][j] + (D_8009F900[0][j] * 1000 + c);
            for (k = 0; k < 3; k++) {
                D_800F3838[i][j][k] = D_8009EA04[1][j];
            }
        }
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            for (k = 0; k < 5; k++) {
                *(int *)&D_800FF700[i][j][k].name[0] = *(int *)&D_8009EA44[k].name[0];
                *(int *)&D_800FF700[i][j][k].name[4] = *(int *)&D_8009EA44[k].name[4];
                *(int *)&D_800AD5C8[i][j][k].name[0] = *(int *)&D_8009EA44[k].name[0];
                *(int *)&D_800AD5C8[i][j][k].name[4] = *(int *)&D_8009EA44[k].name[4];
                D_800FF700[i][j][k].time = D_800ACC28[i][j] + k * 3000;
                {
                    int t = k * 3000;
                    D_800AD5C8[i][j][k].time = D_800F4CE8[i][j] + t * D_8009F900[0][j];
                }
                D_800FF700[i][j][k].car = D_8009EA44[k].car;
                D_800AD5C8[i][j][k].car = D_8009EA44[k].car;
            }
        }
    }
}
