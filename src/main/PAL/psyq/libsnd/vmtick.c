/* CC1_FLAGS: -fcall-used-$1 */
/* $at as the %hi() base: with $1 allocatable cc1 2.8.1 emits lui $1,%hi / sw %lo($1) and its reorg fills the jr/jal delay slot with the sw (see DECOMP_GUIDE "delay-slot store"). */
#include "psyq/libsnd/vmtick.h"
#include "rr/spu_voice_cache.h"
void func_80087044() { D_801100A0 = 0; }
