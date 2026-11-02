struct S { char pad[0xE3]; unsigned char a; char pad1[5]; unsigned char b; char pad2[2]; unsigned short c; };
int func_80098DD4(s) struct S *s; {
    int a = s->a, b = s->b, c = s->c;
    int x = ((a + 1) >> 1) * 4;
    int y = ((b * 5 + 3) & 0xFFC) + 4;
    return x + y + c;
}
