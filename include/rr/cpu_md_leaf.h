#ifndef RR_CPU_MD_LEAF_H
#define RR_CPU_MD_LEAF_H
#include "types.h"
/* Explicit HI/LO multiply/divide state of the original arithmetic, kept as
 * a model of one CPU register pair: MULT starts an operation and HI/LO read
 * its completed result. DIV asserts the valid, nontrapping domain. */
#include <assert.h>
static u32 rr_md_hi,rr_md_lo;
#define RR_MD_MULT_S(a,b) do {u64 rr_p=(u64)((s64)(s32)(a)*(s64)(s32)(b));rr_md_lo=(u32)rr_p;rr_md_hi=(u32)(rr_p>>32);} while(0)
#define RR_MD_MULT_S_FINISH16(a,coefficient,low) do {s32 rr_v=(a);(coefficient)|=(low);RR_MD_MULT_S(rr_v,coefficient);} while(0)
#define RR_MD_MULT_U(a,b) do {u64 rr_p=(u64)(u32)(a)*(u32)(b);rr_md_lo=(u32)rr_p;rr_md_hi=(u32)(rr_p>>32);} while(0)
#define RR_MD_LO(out) ((out)=rr_md_lo)
#define RR_MD_LO_ACCUM_SHL2(out,acc,add) do {u32 rr_acc=(u32)(acc),rr_add=(u32)(add);(out)=rr_md_lo;(acc)=(s32)((rr_acc+rr_add)<<2);} while(0)
#define RR_MD_HI(out) ((out)=rr_md_hi)
#define RR_MD_DIV_S(out,a,b) do {s32 rr_a=(a),rr_b=(b);s64 rr_q;assert(rr_b!=0 && !(rr_a==(-2147483647-1)&&rr_b==-1));rr_q=(s64)rr_a/rr_b;rr_md_lo=(u32)rr_q;rr_md_hi=(u32)((s64)rr_a-rr_q*rr_b);(out)=rr_md_lo;} while(0)
#endif
