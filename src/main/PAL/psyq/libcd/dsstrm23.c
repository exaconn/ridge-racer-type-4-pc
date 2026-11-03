/* CC1_FLAGS: -fcall-used-$1 */
/* $at as the %hi() base: with $1 allocatable cc1 2.8.1 emits lui $1,%hi / sw %lo($1) and its reorg fills the jr/jal delay slot with the sw (see DECOMP_GUIDE "delay-slot store"). */
extern int D_80110308, D_801102E4, D_80110304;
void func_8008F914(int a, int b, int c) { D_80110308 = a; D_801102E4 = b; D_80110304 = c; }
