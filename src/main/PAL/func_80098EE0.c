/* GCC_VERSION: 2.7.2 */
struct S { char pad[0x46]; unsigned char c46, c47, c48; };
extern int func_8009964C(), func_8009966C(), func_8009968C(), func_800996AC();
void func_80098EE0(s) struct S *s; {
    switch (s->c46) {
    case 2: func_8009964C(s, s->c47); break;
    case 3: func_8009966C(s, s->c47); break;
    case 4:
        if (s->c48 == 0) func_8009968C(s, s->c47);
        else func_800996AC(s);
        break;
    }
}
