/* RODATA 0x800115ec 0x20 */
extern short D_800FFC18;
extern short D_800FFC00;
extern short D_800AC594;
extern short D_800F3A1C;
extern unsigned char D_800F3A24[];
extern struct { char pad[0x28]; int x28; int x2c; char pad2[0x2C]; } D_800F3A38[];

int func_8007B654()
{
    int a1;
    int a2;
    int v;
    a2 = -1;
    switch (D_800FFC18) {
    case 0:
        a1 = 0;
        break;
    case 1:
    case 5:
        a1 = 0x20;
        break;
    case 2:
        a1 = 0x21;
        break;
    case 3:
        a1 = 0x17;
        break;
    case 6:
    case 7:
        a1 = 0x29;
        break;
    case 4:
        break;
    }
    v = D_800F3A24[D_800FFC00];
    switch (v) {
    case 0x28:
    case 0x3C:
    case 0x1E:
        if (D_800F3A38[D_800AC594].x28 != 0 || D_800F3A38[D_800AC594].x2c != 0) {
            D_800F3A1C = a1;
        }
        break;
    case 0x14:
    case 0x32:
        a2 = 1;
        break;
    case 0x5A:
        break;
    default:
        a2 = 1;
        break;
    }
    return a2;
}
