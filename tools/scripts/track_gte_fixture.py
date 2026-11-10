"""Exact transfer/command adapters for current track-object GTE intrinsics.

The existing fixtures record individual loads, reads, stores and commands.
Pipeline NOPs have no architectural data effect in the native fixture; original
and linked MIPS still execute them. Arithmetic models and cases stay unchanged.
"""

GTE_TRANSFER_ADAPTERS = r'''
#ifndef RR_GTE14_LWC2_V0
#define RR_GTE14_LWC2_V0(p) do {load(0,(const void *)(p));load(1,(const u8 *)(p)+4);} while(0)
#endif
#ifndef RR_GTE_LWC2_V012_3P
#define RR_GTE_LWC2_V012_3P(a,b,c) do {load(0,(const void *)(a));load(1,(const u8 *)(a)+4);load(2,(const void *)(b));load(3,(const u8 *)(b)+4);load(4,(const void *)(c));load(5,(const u8 *)(c)+4);} while(0)
#endif
#ifndef RR_GTE14_NOP2
#define RR_GTE14_NOP2() ((void)0)
#endif
#ifndef RR_GTE_OP
#define RR_GTE_OP(word) command(word)
#endif
#ifndef RR_GTE14_RTPT
#define RR_GTE14_RTPT() command(0x4A280030)
#endif
#ifndef RR_GTE_RTPT_NOP2
#define RR_GTE_RTPT_NOP2() command(0x4A280030)
#endif
#ifndef RR_GTE_READ_IR3_NOP
#define RR_GTE_READ_IR3_NOP(output) ((output)=(s32)readData(11))
#endif
'''
