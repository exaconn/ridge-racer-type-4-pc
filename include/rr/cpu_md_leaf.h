#ifndef RR_CPU_MD_LEAF_H
#define RR_CPU_MD_LEAF_H
#include "types.h"
/* Explicit HI/LO state for a leaf translation unit. Stock gcc2.8 rejects a
 * normal MD pattern in a TU that also declares HI/LO inline-asm clobbers (even
 * a minimal C product followed by MULTU). Binding the hardware registers and
 * issuing every MD operation through this interface avoids that reload bug.
 * Constant shifts/adds remain C; do not introduce an ordinary variable product
 * or division into an including TU. The native branch is an arithmetic model
 * of one CPU's register pair. No target storage or additional function exists.
 *
 * MULT starts an operation; HI/LO read the corresponding completed result.
 * Memory clobbers retain surrounding publication/load phases. DIV uses the
 * stock assembler's checked signed pseudo operation, including BREAK7/6.
 * Its native reference asserts the valid, nontrapping argument domain.
 *
 * FINISH16 appends an immediate low16 coefficient before MULT, filling the
 * dividend's load slot. LO_ACCUM_SHL2 transfers LO while finishing a numerator
 * ((acc+add)<<2); out and acc are distinct. These fused pipeline groups prevent
 * gcc2.8 from coalescing the accumulator into the addend's physical register.
 * All GP/HI/LO outputs and the assembler temporary are declared explicitly. */
#if defined(__mips__)
register u32 rr_md_hi __asm__("hi");
register u32 rr_md_lo __asm__("lo");
#define RR_MD_MULT_S(a,b) __asm__ volatile("mult %2,%3" : "=h"(rr_md_hi),"=l"(rr_md_lo) : "r"(a),"r"(b) : "memory")
#define RR_MD_MULT_S_FINISH16(a,coefficient,low) __asm__ volatile("ori %2,%2," #low ";mult %3,%2" : "=h"(rr_md_hi),"=l"(rr_md_lo),"=&r"(coefficient) : "r"(a),"2"(coefficient) : "memory")
#define RR_MD_MULT_U(a,b) __asm__ volatile("multu %2,%3" : "=h"(rr_md_hi),"=l"(rr_md_lo) : "r"(a),"r"(b) : "memory")
#define RR_MD_LO(out) __asm__ volatile("mflo %0" : "=r"(out) : "l"(rr_md_lo) : "memory")
#define RR_MD_LO_ACCUM_SHL2(out,acc,add) __asm__ volatile("mflo %0;addu %1,%1,%4;sll %1,%1,2" : "=&r"(out),"=r"(acc) : "l"(rr_md_lo),"1"(acc),"r"(add) : "memory")
#define RR_MD_HI(out) __asm__ volatile("mfhi %0" : "=r"(out) : "h"(rr_md_hi) : "memory")
/* The stock assembler expands the checked signed DIV pseudo operation. */
#define RR_MD_DIV_S(out,a,b) __asm__ volatile("div %0,%3,%4" : "=r"(out),"=h"(rr_md_hi),"=l"(rr_md_lo) : "r"(a),"r"(b) : "$1","memory")
#else
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
#endif
