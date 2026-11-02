/* CC1_FLAGS: -fcall-used-$1 */
/* $at as the %hi() base: with $1 allocatable cc1 2.8.1 emits lui $1,%hi / sw %lo($1) and its reorg fills the jr/jal delay slot with the sw (see DECOMP_GUIDE "delay-slot store"). */
extern int D_800A6730;
void func_8008DDA0(int a) { D_800A6730 = a; }
