extern void func_80052024();
extern char D_800F4DB8[];
void func_800523C8(int arg0) {
 int k = arg0 * 3; int t = arg0 + 2; char *v, *s, *b;
 func_80052024(t); b = D_800F4DB8; s = b + (((k << 4) + arg0) << 5); v = b + t * 0x1C;
 *(int *)(s + 0x68) = *(int *)(v + 0x18FC);
 *(unsigned short *)(s + 0x5C) = *(unsigned short *)(v + 0x1900);
 *(unsigned short *)(s + 0x5A) = *(unsigned short *)(v + 0x1902);
 *(int *)(s + 0x64) = *(int *)(v + 0x1904);
}
