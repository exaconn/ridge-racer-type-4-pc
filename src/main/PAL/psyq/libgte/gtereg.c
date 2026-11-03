/* GCC_VERSION: 970318 */
void func_80091164(int *a0);
void func_80091174(int *a0);
void func_80091184(int *a0);
void func_80091194(int *a0, int *a1, int *a2);
void func_800911B4(int *a0, int *a1, int *a2);
void func_800911C8(int a0, int a1, int a2);
void func_800911DC(int a0);
void func_800911E8(int a0, int a1, int a2);
void func_800911FC(int a0, int a1, int a2, int a3);
void func_80091214(int a0, int a1, int a2);
void func_80091228(int a0, int a1, int a2);
void func_8009123C(int a0, int a1, int a2);
void func_80091250(int a0);
void func_8009125C(int a0);
void func_80091268(int a0);

void func_80091164(int *a0) {
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
}

void func_80091174(int *a0) {
    __asm__ volatile ("lwc2 $2, 0(%0)\n\tlwc2 $3, 4(%0)" : : "r"(a0));
}

void func_80091184(int *a0) {
    __asm__ volatile ("lwc2 $4, 0(%0)\n\tlwc2 $5, 4(%0)" : : "r"(a0));
}

void func_80091194(int *a0, int *a1, int *a2) {
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $2, 0(%0)\n\tlwc2 $3, 4(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $4, 0(%0)\n\tlwc2 $5, 4(%0)" : : "r"(a2));
}

void func_800911B4(int *a0, int *a1, int *a2) {
    __asm__ volatile ("lwc2 $20, 0(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $21, 0(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $22, 0(%0)" : : "r"(a2));
}

void func_800911C8(int a0, int a1, int a2) {
    __asm__ volatile ("mtc2 %0, $9" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $10" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $11" : : "r"(a2));
}

void func_800911DC(int a0) {
    __asm__ volatile ("mtc2 %0, $8" : : "r"(a0));
}

void func_800911E8(int a0, int a1, int a2) {
    __asm__ volatile ("mtc2 %0, $17" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $18" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $19" : : "r"(a2));
}

void func_800911FC(int a0, int a1, int a2, int a3) {
    __asm__ volatile ("mtc2 %0, $16" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $17" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $18" : : "r"(a2));
    __asm__ volatile ("mtc2 %0, $19" : : "r"(a3));
}

void func_80091214(int a0, int a1, int a2) {
    __asm__ volatile ("mtc2 %0, $12" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $13" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $14" : : "r"(a2));
}

void func_80091228(int a0, int a1, int a2) {
    __asm__ volatile ("ctc2 %0, $0" : : "r"(a0));
    __asm__ volatile ("ctc2 %0, $2" : : "r"(a1));
    __asm__ volatile ("ctc2 %0, $4" : : "r"(a2));
}

void func_8009123C(int a0, int a1, int a2) {
    __asm__ volatile ("mtc2 %0, $25" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $26" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $27" : : "r"(a2));
}

void func_80091250(int a0) {
    __asm__ volatile ("mtc2 %0, $30" : : "r"(a0));
}

void func_8009125C(int a0) {
    __asm__ volatile ("ctc2 %0, $27" : : "r"(a0));
}

void func_80091268(int a0) {
    __asm__ volatile ("ctc2 %0, $28" : : "r"(a0));
}
