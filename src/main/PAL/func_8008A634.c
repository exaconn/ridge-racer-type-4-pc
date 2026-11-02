/* CC1_FLAGS: -fcall-used-$1 */
/* $at as the %hi() base: with $1 allocatable cc1 2.8.1 emits lui $1,%hi / sw %lo($1) and its reorg fills the jr/jal delay slot with the sw (see DECOMP_GUIDE "delay-slot store"). */
extern short D_801100E8;
void func_8008A634() { D_801100E8 = 0; }
