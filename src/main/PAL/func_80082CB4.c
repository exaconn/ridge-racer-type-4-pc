extern void (*D_800A4D44[])();
void func_80082CB4(void) {
 void (**p)() = D_800A4D44;
 /* ASM_BARRIER: keeps D_800A4D44 address in one register (lui+addiu) so p[-1] uses a negative offset */
 __asm__("" : "=r"(p) : "0"(p));
 if (p[0] != 0) p[0]();
 p[-1]();
}
