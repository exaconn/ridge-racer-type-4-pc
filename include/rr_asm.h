#ifndef RR_ASM_H
#define RR_ASM_H
/* Named hardware transport and documented pipeline gaps (docs/BARRIERS.md).
 * Existing arithmetic/register-allocation asm stays visible in its owning C
 * function until replaced with matching C; moving it here does not retire it.
 * RR_NOP_DEP(): a single `nop` reproducing a load/store dependency gap the original assembler left (maspsx would place it after a loop label). */
#define RR_NOP_DEP() __asm__ volatile("nop")
/* RR_NOP_DEP_ON(value): keep that pipeline gap after the load producing value. */
#define RR_NOP_DEP_ON(value) __asm__ volatile("nop" : : "r"(value))
/* GTE (COP2) helpers. All use $t4-$t6 ($12-$14) as scratch exactly like the original hand-written PsyQ-style macros. */
/* RR_GTE_SETLCM(p): load the 0x14-byte light-colour matrix block at p into GTE control regs $16-$20 (LCM). */
#define RR_GTE_SETLCM(p) __asm__ volatile ( \
    "lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $16; ctc2 $13, $17; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $18; ctc2 $13, $19; ctc2 $14, $20" \
    : : "r"(p) : "$12", "$13", "$14")
/* RR_GTE_SETLLM(p): light matrix block (control regs $8-$12). */
#define RR_GTE_SETLLM(p) __asm__ volatile ( \
    "lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $8; ctc2 $13, $9; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $10; ctc2 $13, $11; ctc2 $14, $12" \
    : : "r"(p) : "$12", "$13", "$14")
/* RR_GTE_SETROT(p): rotation matrix block (control regs $0-$4). */
#define RR_GTE_SETROT(p) __asm__ volatile ( \
    "lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" \
    : : "r"(p) : "$12", "$13", "$14")
/* RR_GTE_SETTR(p): translation vector (control regs $5-$7) from 3 words at p. */
#define RR_GTE_SETTR(p) __asm__ volatile ( \
    "lw $12, 0(%0); lw $13, 4(%0); lw $14, 8(%0); ctc2 $12, $5; ctc2 $13, $6; ctc2 $14, $7" \
    : : "r"(p) : "$12", "$13", "$14")
/* RR_GTE_SETFC(r,g,b): far colour (control regs $21-$23) from three register values. */
#define RR_GTE_SETFC(r, g, b) __asm__ volatile ( \
    "ctc2 %0, $21; ctc2 %1, $22; ctc2 %2, $23" : : "r"(r), "r"(g), "r"(b))
/* RR_GTE_SETBK(r,g,b): background colour (control regs $13-$15). */
#define RR_GTE_SETBK(r, g, b) __asm__ volatile ( \
    "ctc2 %0, $13; ctc2 %1, $14; ctc2 %2, $15" : : "r"(r), "r"(g), "r"(b))

/* Raw GTE command words and single-matrix-half loader (central, ex w3t0_gte.h). */
#define RR_GTE_SETMAT(p, c0) __asm__ volatile ( \
    "lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $" #c0 "; ctc2 $13, $" #c0 "+1" : : "r"(p) : "$12", "$13")
#define RR_GTE_OP(w) __asm__ volatile (".word " #w)
#define GRTPS ".word 0x4A180001"
#define GRTPT ".word 0x4A280030"
#define GNCLIP ".word 0x4B400006"
#define GAVSZ3 ".word 0x4B58002D"
#define GAVSZ4 ".word 0x4B68002E"
#define GDPCS ".word 0x4A780010"
#define GDPCT ".word 0x4AF8002A"
#define GINTPL ".word 0x4A980011"
#define GGPF ".word 0x4B98003D"
#define GCC ".word 0x4B38041C"
#define GSQR ".word 0x4AA80428"
#define GNCCS ".word 0x4B08041B"
#define GNCCT ".word 0x4B18043F"
/* Load packed SXY0, SXY2, SXY1 in PsyQ order and run NCLIP. MAC0 remains in
 * the GTE so a caller may read it after its depth test. */
#define RR_GTE_NCLIP_SXY3(v12, v13, v14) \
    __asm__ volatile ("mtc2 %0, $12\n\tmtc2 %2, $14\n\tmtc2 %1, $13\n\tnop\n\tnop\n\t.word 0x4B400006" : : "r"(v12), "r"(v13), "r"(v14))
/* RR_GTE_NCLIP3(v12, v13, v14, out): load three packed screen-XY words into SXY0..SXY2 ($12-$14, written $12,$14,$13 like PsyQ gte_ldsxy3), run NCLIP, store MAC0 ($24) to *out. */
#define RR_GTE_NCLIP3(v12, v13, v14, out) do { \
    __asm__ volatile ("mtc2 %0, $12\n\tmtc2 %2, $14\n\tmtc2 %1, $13\n\tnop\n\tnop\n\t.word 0x4B400006" : : "r"(v12), "r"(v13), "r"(v14)); \
    __asm__ volatile ("swc2 $24, 0(%0)" : : "r"(out) : "memory"); } while (0)
/* Single-instruction / fused GTE helpers (TransformRroVertices). */
#define RR_GTE_MTC2_IR0(x) __asm__ volatile ("mtc2 %0, $8" : : "r"(x))
/* RGB/code input word consumed by the GTE lighting and depth-cue commands. */
#define RR_GTE_MTC2_RGB(x) __asm__ volatile ("mtc2 %0, $6" : : "r"(x))
#define RR_GTE_LWC2_RGB(p) __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(p))
#define RR_GTE_SWC2_RGB2(p) __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(p))
/* RR_GTE_DPCS_FC4(q): far colour = 3 bytes at q, each << 4 (ctc2 $21-$23), then DPCS. */
#define RR_GTE_DPCS_FC4(q) __asm__ volatile ( \
    "lbu $12, 0(%0); lbu $13, 1(%0); lbu $14, 2(%0); sll $12, $12, 4; sll $13, $13, 4; sll $14, $14, 4; ctc2 $12, $21; ctc2 $13, $22; ctc2 $14, $23; nop; nop; .word 0x4A780010" \
    : : "r"(q) : "$12", "$13", "$14")
/* Set far-color control registers from the low three bytes, scaled by 16.
 * Unlike RR_GTE_DPCS_FC4, this leaves the depth-cue command to the caller. */
#define RR_GTE_SETFC_BYTES3(p) __asm__ volatile ( \
    "lbu $12, 0(%0); lbu $13, 1(%0); lbu $14, 2(%0); sll $12, $12, 4; sll $13, $13, 4; sll $14, $14, 4; ctc2 $12, $21; ctc2 $13, $22; ctc2 $14, $23" \
    : : "r"(p) : "$12", "$13", "$14")
/* Spark endpoints share one staging vector: load V1, replace the vector,
 * then load V2 and project the three GTE input slots. */
#define RR_GTE14_LWC2_V1(p) __asm__ volatile ("lwc2 $2, 0(%0); lwc2 $3, 4(%0)" : : "r"(p) : "memory")
#define RR_GTE_RTPT_LDV2(p) __asm__ volatile ("lwc2 $4, 0(%0); lwc2 $5, 4(%0); nop; nop; .word 0x4A280030" : : "r"(p) : "memory")
#define RR_GTE14_SWC2_SZ3(p) __asm__ volatile ("swc2 $19, 0(%0)" : : "r"(p) : "memory")
#define RR_GTE14_SWC2_SXY1(p) __asm__ volatile ("swc2 $13, 0(%0)" : : "r"(p) : "memory")
/* RR_GTE_MVMVA_LDV0(p): load vector V0 (VXY0/VZ0) from 3 shorts at p and run MVMVA sf=1 mx=rot v=V0 cv=none lm=0; RR_GTE_STORE_IR(p): store IR1..IR3 ($9-$11) to p. */
#define RR_GTE_MVMVA_LDV0(p) do { __asm__ volatile ("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(p)); __asm__ volatile ("nop; nop"); __asm__ volatile (".word 0x4A080012"); } while (0)
#define RR_GTE_STORE_IR(p) __asm__ volatile ("swc2 $9, 0(%0); swc2 $10, 4(%0); swc2 $11, 8(%0)" : : "r"(p))
/* DrawTrackObjectLitModel: MVMVA sf=1 mx=LLM v=V0 cv=BK lm=0 on the vector at p (lwc2 $0/$1); RR_GTE_SWC2_IR3(p): swc2 $11; RR_GTE_TR_FROM_IR(): TR = IR1..IR3. */
#define RR_GTE_MVMVA_LLM_BK_LDV0(p) do { __asm__ volatile ("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(p)); __asm__ volatile ("nop; nop"); __asm__ volatile (".word 0x4A4A2012"); } while (0)
#define RR_GTE_SWC2_IR3(p) __asm__ volatile ("swc2 $11, 0(%0)" : : "r"(p) : "memory")
#define RR_GTE_TR_FROM_IR() __asm__ volatile ("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; ctc2 $12, $5; ctc2 $13, $6; ctc2 $14, $7" : : : "$12", "$13", "$14")
/* Read the signed IR3 data register; the caller supplies its CPU delay. */
#define RR_GTE_READ_IR3(out) __asm__ volatile ("mfc2 %0, $11" : "=r"(out))
/* Glow-fan projection: read signed IR3 with its CPU transfer delay, then use
 * an identity rotation and packed XY inputs whose Z registers are zero. */
#define RR_GTE_READ_IR3_NOP(out) __asm__ volatile ("mfc2 %0, $11\n\tnop" : "=r"(out))
#define RR_GTE_SETROT_IDENTITY_CLEAR_Z() __asm__ volatile ( \
    "ctc2 %0, $0\n\tctc2 $0, $1\n\tctc2 %0, $2\n\tctc2 $0, $3\n\tctc2 %0, $4\n\tmtc2 $0, $1\n\tmtc2 $0, $3\n\tmtc2 $0, $5" : : "r"(0x1000))
#define RR_GTE_RTPT_PACKED_XY(p) __asm__ volatile ( \
    "lwc2 $0, 0(%0)\n\tlwc2 $2, 4(%0)\n\tlwc2 $4, 8(%0)\n\tnop\n\tnop\n\t.word 0x4A280030" : : "r"(p))
/* StepIntroScene: RR_GTE_LWC2_IR(p): load IR1..IR3 ($9-$11) from 3 words at p; RR_GTE_INTPL(): two nops then INTPL. */
#define RR_GTE_LWC2_IR(p) __asm__ volatile ("lwc2 $9, 0(%0); lwc2 $10, 4(%0); lwc2 $11, 8(%0)" : : "r"(p) : "memory")
#define RR_GTE_INTPL() __asm__ volatile ("nop; nop; .word 0x4A980011")
/* StepIntroScene: RR_GTE_SETFC_P(p): far colour from 3 words at p via $t2-$t4; RR_COPY16_T(dst, src, off): copy 16 bytes src+off -> dst via $t2-$t4 (3 loads, 3 stores, 4th word last). */
#define RR_GTE_SETFC_P(p) __asm__ volatile ( \
    "lw $10, 0(%0); lw $11, 4(%0); lw $12, 8(%0); ctc2 $10, $21; ctc2 $11, $22; ctc2 $12, $23" \
    : : "r"(p) : "$10", "$11", "$12")
#define RR_COPY16_T(dst, src, off) __asm__ ( \
    "lw $10, %2(%1); lw $11, %2+4(%1); lw $12, %2+8(%1); sw $10, 0(%0); sw $11, 4(%0); sw $12, 8(%0); lw $10, %2+12(%1); nop; sw $10, 12(%0)" \
    : : "r"(dst), "r"(src), "i"(off) : "$10", "$11", "$12", "memory")
/* DrawTrackObjectBillboard: RR_GTE_SETBK_P(p): background colour (ctc2 $13-$15) from 3 words at p via $t4-$t6; RR_GTE_RTPT_LDV(a,b,c): load V0..V2 from three vectors (lwc2 0..5), two nops, RTPT; RR_GTE_RTPS_LDV(p): load V0, two nops, RTPS; RR_GTE_SWC2_SXYZ(p): store SXY0..SXY2 ($12-$14) to p; RR_GTE_SWC2_SXY2(p): store SXY2 ($14). */
#define RR_GTE_SETBK_P(p) __asm__ volatile ( \
    "lw $12, 0(%0); lw $13, 4(%0); lw $14, 8(%0); ctc2 $12, $13; ctc2 $13, $14; ctc2 $14, $15" \
    : : "r"(p) : "$12", "$13", "$14")
#define RR_GTE_RTPT_LDV(a, b, c) do { __asm__ volatile ("lwc2 $0, 0(%0); lwc2 $1, 4(%0); lwc2 $2, 0(%1); lwc2 $3, 4(%1); lwc2 $4, 0(%2); lwc2 $5, 4(%2)" : : "r"(a), "r"(b), "r"(c)); __asm__ volatile ("nop; nop"); __asm__ volatile (".word 0x4A280030"); } while (0)
#define RR_GTE_RTPS_LDV(p) { __asm__ volatile ("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(p)); __asm__ volatile ("nop; nop"); __asm__ volatile (".word 0x4A180001"); }
#define RR_GTE_SWC2_SXYZ(p) __asm__ volatile ("swc2 $12, 0(%0); swc2 $13, 4(%0); swc2 $14, 8(%0)" : : "r"(p) : "memory")
#define RR_GTE_SWC2_SXY2(p) __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(p) : "memory")
/* Store three projected XY words in a POLY_G3 packet, then read those same
 * GTE registers for the CPU viewport test. */
#define RR_GTE_STORE_READ_SXY3(p, x0, x1, x2) __asm__ volatile ( \
    "swc2 $12, 8(%3)\n\tswc2 $13, 16(%3)\n\tswc2 $14, 24(%3)\n\t" \
    "mfc2 %0, $12\n\tmfc2 %1, $13\n\tmfc2 %2, $14" \
    : "=r"(x0), "=r"(x1), "=r"(x2) : "r"(p) : "memory")
/* DrawTrackScenery: RR_GTE_CLRTR(): TR = 0; RR_GTE_MTC2_IR1(x): mtc2 $9; RR_GTE_CTC2_FCR(x): ctc2 $21; RR_GTE_SWC2_IR1(p): swc2 $9. */
#define RR_GTE_CLRTR() __asm__ volatile ("ctc2 $0, $5; ctc2 $0, $6; ctc2 $0, $7")
#define RR_GTE_MTC2_IR1(x) __asm__ volatile ("mtc2 %0, $9" : : "r"(x))
#define RR_GTE_CTC2_FCR(x) __asm__ volatile ("ctc2 %0, $21" : : "r"(x))
#define RR_GTE_SWC2_IR1(p) __asm__ volatile ("swc2 $9, 0(%0)" : : "r"(p) : "memory")
/* EmitRroLitGouraudQuads GTE helpers (one macro per distinct instruction/sequence). */
#define RR_GTE14_LWC2_V012(p) __asm__ volatile ("lwc2 $0, 0(%0); lwc2 $1, 4(%0); lwc2 $2, 8(%0); lwc2 $3, 12(%0); lwc2 $4, 16(%0); lwc2 $5, 20(%0)" : : "r"(p) : "memory")
#define RR_GTE14_LWC2_V0(p) __asm__ volatile ("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(p) : "memory")
#define RR_GTE14_LWC2_V0_V1_V2(a, b, c) __asm__ volatile ("lwc2 $0, 0(%0); lwc2 $1, 4(%0); lwc2 $2, 0(%1); lwc2 $3, 4(%1); lwc2 $4, 0(%2); lwc2 $5, 4(%2)" : : "r"(a), "r"(b), "r"(c) : "memory")
#define RR_GTE14_LWC2_RGB(p) __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(p) : "memory")
#define RR_GTE14_NOP2() __asm__ volatile ("nop; nop")
#define RR_GTE14_RTPT() __asm__ volatile (".word 0x4A280030")
/* SDK RTPT command with the two CPU load-delay slots in the same block. */
#define RR_GTE_RTPT_NOP2() __asm__ volatile ("nop\n\tnop\n\t.word 0x4A280030")
#define RR_GTE14_RTPS() __asm__ volatile (".word 0x4A180001")
#define RR_GTE14_NCLIP() __asm__ volatile (".word 0x4B400006")
#define RR_GTE14_AVSZ4() __asm__ volatile (".word 0x4B68002E")
#define RR_GTE14_SWC2_MAC0(p) __asm__ volatile ("swc2 $24, 0(%0)" : : "r"(p) : "memory")
#define RR_GTE14_SWC2_OTZ(p) __asm__ volatile ("swc2 $7, 0(%0)" : : "r"(p) : "memory")
#define RR_GTE14_SWC2_SXYZ(p) __asm__ volatile ("swc2 $12, 0(%0); swc2 $13, 4(%0); swc2 $14, 8(%0)" : : "r"(p) : "memory")
#define RR_GTE14_SWC2_SXY2(p) __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(p) : "memory")
#define RR_GTE14_LIGHT0() __asm__ volatile (".word 0x4A4A6412; .word 0x4AA80428; .word 0x4AA80428; .word 0x4B38041C")
#define RR_GTE14_LIGHT1() __asm__ volatile (".word 0x4A4AE412; .word 0x4AA80428; .word 0x4AA80428; .word 0x4B38041C")
#define RR_GTE14_LIGHT2() __asm__ volatile (".word 0x4A4B6412; .word 0x4AA80428; .word 0x4AA80428; .word 0x4B38041C")
#define RR_GTE14_SWC2_RGB012(p) __asm__ volatile ("swc2 $20, 4(%0); swc2 $21, 12(%0); swc2 $22, 20(%0)" : : "r"(p) : "memory")
#define RR_GTE14_SWC2_RGB2(p) __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(p) : "memory")
/* FindWheelTrackNodes: MVMVA rot-matrix x V0, no translation, shift 12 (sf=1,mx=0,v=0,cv=3,lm=0); preceded by the V0 load from p. */
#define RR_GTE_MVMVA_ROT_V0_NOCV(p) { __asm__ volatile ("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(p)); __asm__ volatile ("nop; nop"); __asm__ volatile (".word 0x4A486012"); }

/* SQR: square signed IR1..IR3, with fixed-point shift 12 or no shift.
 * MAC1..MAC3 retain the arithmetic results; IR saturation and FLAG are hardware side effects. */
#define RR_GTE_SQR12() RR_GTE_OP(0x4AA80428)
#define RR_GTE_SQR0() RR_GTE_OP(0x4AA00428)
#define RR_GTE_SWC2_MAC123(p) __asm__ volatile ("swc2 $25, 0(%0); swc2 $26, 4(%0); swc2 $27, 8(%0)" : : "r"(p) : "memory")

/* Memory-clobbering GTE command barriers used by polygon packet builders (DrawMeshQuadGT4..DrawMeshQuadF4). */
#define RR_GTE_RTPT_MEM() __asm__ volatile (".word 0x4A280030" : : : "memory")
#define RR_GTE_RTPS_MEM() __asm__ volatile (".word 0x4A180001" : : : "memory")
#define RR_GTE_NCLIP_MEM() __asm__ volatile (".word 0x4B400006" : : : "memory")
#define RR_GTE_AVSZ4_MEM() __asm__ volatile (".word 0x4B68002E" : : : "memory")

/* SetupSkyLightMatrices: column-wise IR1..IR3 load/MVMVA and short store helpers. */
#define RR_GTE_MV3_COL(src) __asm__ volatile ("lhu $12, 0(%0); lhu $13, 6(%0); lhu $14, 12(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11\n\tnop\n\tnop\n\t.word 0x4A49E012" : : "r"(src) : "$12", "$13", "$14")
#define RR_GTE_ST3_COL(dst) __asm__ volatile ("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 6(%0); sh $14, 12(%0)" : : "r"(dst) : "$12", "$13", "$14", "memory")

/* UpdateComCarSpeed: store IR1..IR3 ($9-$11) as contiguous 16-bit components. */
#define RR_GTE_STORE_IR_S16(p) __asm__ volatile ("mfc2 $12, $9\n\tmfc2 $13, $10\n\tmfc2 $14, $11\n\tsh $12, 0(%0)\n\tsh $13, 2(%0)\n\tsh $14, 4(%0)" : : "r"(p) : "$12", "$13", "$14", "memory")

/* SoundCalcCarParams / SetTrackObjectTransform: combined rotation + translation matrix loaders and FLAG reader. */
#define RR_GTE_SETROT_TR(p) __asm__ volatile ("lw $12, 0(%0)\n\tlw $13, 4(%0)\n\tctc2 $12, $0\n\tctc2 $13, $1\n\tlw $12, 8(%0)\n\tlw $13, 12(%0)\n\tlw $14, 16(%0)\n\tctc2 $12, $2\n\tctc2 $13, $3\n\tctc2 $14, $4\n\tlw $12, 20(%0)\n\tlw $13, 24(%0)\n\tctc2 $12, $5\n\tlw $14, 28(%0)\n\tctc2 $13, $6\n\tctc2 $14, $7" : : "r"(p) : "$12", "$13", "$14")
#define RR_GTE_SETTR_AT20(p) __asm__ volatile ("lw $12, 20(%0)\n\tlw $13, 24(%0)\n\tctc2 $12, $5\n\tlw $14, 28(%0)\n\tctc2 $13, $6\n\tctc2 $14, $7" : : "r"(p) : "$12", "$13", "$14")
#define RR_GTE_MVMVA_ROT_TR_V0(p) __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(p)); __asm__ volatile ("nop\n\tnop\n\t.word 0x4A480012")
#define RR_GTE_STORE_FLAG(p) __asm__ volatile ("cfc2 $12, $31\n\tnop\n\tsw $12, 0(%0)" : : "r"(p) : "$12", "memory")
#define RR_GTE_STORE_IR0_THEN_FLAG(p) __asm__ volatile ("swc2 $8, 0(%0)\n\tcfc2 $12, $31\n\tnop\n\tsw $12, 0(%0)" : : "r"(p) : "$12", "memory")
#define RR_GTE_SWC2_IR0(p) __asm__ volatile ("swc2 $8, 0(%0)" : : "r"(p) : "memory")
#define RR_GTE_STORE_SZ3_SRA2(p) __asm__ volatile ("mfc2 $12, $19\n\tnop\n\tsra $12, $12, 2\n\tsw $12, 0(%0)" : : "r"(p) : "$12", "memory")

/* PsyQ libgte hand-coded matrix multiply helpers (MulMatrix0..ApplyMatrixSV). */
#define RR_GTE_MASK_HI16(x) __asm__ volatile ("and %0, %0, 0xFFFF0000" : "+r"(x))
#define RR_GTE_MVMVA_ROT_V0_1NOP() __asm__ volatile ("nop\n\t.word 0x4A486012")
#define RR_GTE_SW_AT(val, base, idx) __asm__ volatile ("sw %0, " #idx "*4(%1)" : : "r"(val), "r"(base))
#define RR_GTE_SH_AT(val, base, idx) __asm__ volatile ("sh %0, " #idx "*2(%1)" : : "r"(val), "r"(base))

/* Generic single-register and single-offset GTE transfers and commands. */
#define RR_GTE_CTC2(val, reg) __asm__ volatile ("ctc2 %0, $" #reg : : "r"(val))
#define RR_GTE_CFC2(out, reg) __asm__ volatile ("cfc2 %0, $" #reg : "=r"(out))
#define RR_GTE_MTC2(val, reg) __asm__ volatile ("mtc2 %0, $" #reg : : "r"(val))
#define RR_GTE_MFC2(out, reg) __asm__ volatile ("mfc2 %0, $" #reg : "=r"(out))
#define RR_GTE_LWC2_AT(reg, off, base) __asm__ volatile ("lwc2 $" #reg ", " #off "(%0)" : : "r"(base))
#define RR_GTE_LWC2_SP(reg, off) __asm__ volatile ("lwc2 $" #reg ", " #off "($29)")
#define RR_GTE_SWC2_AT(reg, off, base) __asm__ volatile ("swc2 $" #reg ", " #off "(%0)" : : "r"(base))
#define RR_GTE_SWC2_AT_MEM(reg, off, base) __asm__ volatile ("swc2 $" #reg ", " #off "(%0)" : : "r"(base) : "memory")
#define RR_GTE_CMD_1NOP(op) __asm__ volatile ("nop\n\t.word " #op)
#define RR_GTE_CMD_2NOP(op) __asm__ volatile ("nop\n\tnop\n\t.word " #op)
#define RR_GTE_LWC2_PAIR_AT(r0, off0, r1, off1, base) __asm__ volatile ("lwc2 $" #r0 ", " #off0 "(%0)\n\tlwc2 $" #r1 ", " #off1 "(%0)" : : "r"(base))
#define RR_GTE_SWC2_PAIR_AT(r0, off0, r1, off1, base) __asm__ volatile ("swc2 $" #r0 ", " #off0 "(%0)\n\tswc2 $" #r1 ", " #off1 "(%0)" : : "r"(base))
#define RR_GTE_SWC2_TRIPLE_AT(r0, off0, r1, off1, r2, off2, base) __asm__ volatile ("swc2 $" #r0 ", " #off0 "(%0)\n\tswc2 $" #r1 ", " #off1 "(%0)\n\tswc2 $" #r2 ", " #off2 "(%0)" : : "r"(base))
#define RR_GTE_LWC2_V012_3P(v0, v1, v2) __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)\n\tlwc2 $2, 0(%1)\n\tlwc2 $3, 4(%1)\n\tlwc2 $4, 0(%2)\n\tlwc2 $5, 4(%2)" : : "r"(v0), "r"(v1), "r"(v2))

/* gtepoly1.c GTE helpers. */
#define GTE_STORE_SXY_SZ(p) \
    __asm__ volatile( \
        "swc2 $12, 0(%0); swc2 $17, 4(%0); swc2 $13, 8(%0); swc2 $18, 12(%0); swc2 $14, 16(%0); swc2 $19, 20(%0)" \
        : : "r"(p) : "memory")
#define GTE_SETROT_AND_SAVE(src, dst) \
    __asm__ volatile( \
        "lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; sw $12, 0(%1); sw $13, 4(%1); " \
        "lw $12, 8(%0); lw $13, 12(%0); ctc2 $12, $2; ctc2 $13, $3; sw $12, 8(%1); " \
        "lw $12, 16(%0); sw $13, 12(%1); ctc2 $12, $4; sw $12, 16(%1)" \
        : : "r"(src), "r"(dst) : "$12", "$13", "memory")
#define GTE_CTC2_27(val) __asm__ volatile("ctc2 %0, $27" : : "r"(val))
#define GTE_CTC2_28(val) __asm__ volatile("ctc2 %0, $28" : : "r"(val))
#define GTE_LWC2_IR0(p) __asm__ volatile("lwc2 $8, 0(%0)" : : "r"(p))
#define GTE_LWC2_IR0_MEM(p) __asm__ volatile("lwc2 $8, 0(%0)" : : "r"(p) : "memory")
#define GTE_DPCS_COL(col, dc) __asm__ volatile("mtc2 %1, $8\n\tmtc2 %0, $6\n\tnop\n\tnop\n\t.word 0x4A780010\n\tmfc2 %0, $22" : "+r"(col) : "r"(dc))
#define GTE_MTC2_RGB_DPCS(c) __asm__ volatile("mtc2 %0, $6\n\tnop\n\tnop\n\t.word 0x4A780010" : : "r"(c))
#define RR_GTE_MFC2_IR1_IR2(x, y) __asm__ volatile("mfc2 %0, $9; mfc2 %1, $10" : "=r"(x), "=r"(y))
#define RR_GTE_SWC2_RGB012_ENV(p) __asm__ volatile("swc2 $20, 4(%0); swc2 $21, 16(%0); swc2 $22, 28(%0)" : : "r"(p) : "memory")
#define RR_GTE_LDSXY3(sxy0, sxy1, sxy2) __asm__ volatile("mtc2 %0, $12\n\tmtc2 %2, $14\n\tmtc2 %1, $13" : : "r"(sxy0), "r"(sxy1), "r"(sxy2))
#define RR_GTE_MFC2_SXY2_NOP(out) __asm__ volatile("mfc2 %0, $14; nop" : "=r"(out) : : "memory")

/* ring.c GTE helper: RTPT projects the inner endpoint twice; the last two SXY stores share a slot. */
#define RING_STORE_SXY_PAIR(outer, inner) \
    __asm__ volatile( \
        "swc2 $12,0(%0); swc2 $13,0(%1); swc2 $14,0(%1)" \
        : : "r"(outer), "r"(inner) : "memory")

/* sky.c GTE helpers. */
#define RR_GTE_LOAD_SKY_TRI_VERTS(a1) __asm__ volatile( \
    "lw $13, 0x18(%0)\n\tlw $14, 0x1C(%0)\n\tlw $15, 0x20(%0)\n\t" \
    "lwc2 $0, 0($13)\n\tlwc2 $1, 4($13)\n\tlwc2 $2, 0($14)\n\tlwc2 $3, 4($14)\n\tlwc2 $4, 0($15)\n\tlwc2 $5, 4($15)" \
    : : "r"(a1) : "$13", "$14", "$15")
#define RR_GTE_DPCT_SKY_TRI(a1, v0, v1, a3, pkt) __asm__ volatile( \
    "lwc2 $8, 0(%0)\n\tmtc2 %1, $6\n\tmtc2 %1, $20\n\tmtc2 %2, $21\n\tmtc2 %3, $22\n\t" \
    "nop\n\tnop\n\t.word 0x4AF8002A\n\tswc2 $20, 4(%4)\n\tswc2 $21, 12(%4)\n\tswc2 $22, 20(%4)" \
    : : "r"(a1), "r"(v0), "r"(v1), "r"(a3), "r"(pkt) : "memory")
#define RR_GTE_RTPT_SKY_QUAD_VERTS(a1) __asm__ volatile( \
    "lw $15, 0x18(%0)\n\tlw $24, 0x1C(%0)\n\tlw $25, 0x20(%0)\n\t" \
    "lwc2 $0, 0($15)\n\tlwc2 $1, 4($15)\n\tlwc2 $2, 0($24)\n\tlwc2 $3, 4($24)\n\tlwc2 $4, 0($25)\n\tlwc2 $5, 4($25)\n\t" \
    "nop\n\tnop\n\t.word 0x4A280030" \
    : : "r"(a1) : "$15", "$24", "$25")
#define RR_GTE_DPCT_DPCS_SKY_QUAD(a1, v1, t0, a3, t1, pkt) __asm__ volatile( \
    "lwc2 $8, 0(%0)\n\tmtc2 %1, $6\n\tmtc2 %1, $20\n\tmtc2 %2, $21\n\tmtc2 %3, $22\n\t" \
    "nop\n\tnop\n\t.word 0x4AF8002A\n\tswc2 $20, 4(%5)\n\tswc2 $21, 12(%5)\n\tswc2 $22, 20(%5)\n\t" \
    "mtc2 %4, $6\n\tnop\n\tnop\n\t.word 0x4A780010" \
    : : "r"(a1), "r"(v1), "r"(t0), "r"(a3), "r"(t1), "r"(pkt) : "memory")
#define RR_GTE_DPCS_SKY_FT4(a1, t1) __asm__ volatile( \
    "lwc2 $8, 0(%0)\n\tmtc2 %1, $6\n\tnop\n\tnop\n\t.word 0x4A780010" \
    : : "r"(a1), "r"(t1) : "memory")
#define RR_GTE_DPCT_SKY_GT3(a1, v0, v1, a3, pkt) __asm__ volatile( \
    "lwc2 $8, 0(%0)\n\tmtc2 %1, $6\n\tmtc2 %1, $20\n\tmtc2 %2, $21\n\tmtc2 %3, $22\n\t" \
    "nop\n\tnop\n\t.word 0x4AF8002A\n\tswc2 $20, 4(%4)\n\tswc2 $21, 16(%4)\n\tswc2 $22, 28(%4)" \
    : : "r"(a1), "r"(v0), "r"(v1), "r"(a3), "r"(pkt) : "memory")
#define RR_GTE_STORE_IR_MEM(p) __asm__ volatile("swc2 $9, 0(%0); swc2 $10, 4(%0); swc2 $11, 8(%0)" : : "r"(p) : "memory")

/* cdfile2.c GTE helpers. */
#define RR_GTE_INTPL_TRACK_TABLE(v0, v1, a1, a2, a3, t2) __asm__ volatile( \
    "mtc2 %4, $8\n\t" \
    "ctc2 %0, $21\n\t" \
    "ctc2 %2, $22\n\t" \
    "mtc2 %3, $9\n\t" \
    "mtc2 %1, $10\n\t" \
    "nop\n\t" \
    "nop\n\t" \
    ".word 0x4A980011\n\t" \
    "mtc2 %5, $8\n\t" \
    "mfc2 %1, $10\n\t" \
    "nop\n\t" \
    "ctc2 %1, $21\n\t" \
    "nop\n\t" \
    "nop\n\t" \
    ".word 0x4A980011\n\t" \
    "mfc2 %1, $9\n\t" \
    "nop\n\t" \
    "mtc2 %1, $8" \
    : "+r"(v0), "+r"(v1), "+r"(a1), "+r"(a2) \
    : "r"(a3), "r"(t2))
#define RR_GTE_MTC2_IR123_INTPL(a0, t1, t2) __asm__ volatile( \
    "mtc2 %0, $9\n\t" \
    "mtc2 %1, $10\n\t" \
    "mtc2 %2, $11\n\t" \
    "nop\n\t" \
    "nop\n\t" \
    ".word 0x4A980011" \
    : : "r"(a0), "r"(t1), "r"(t2))
#define RR_GTE_BK_FROM_IR() __asm__ volatile( \
    "mfc2 $12, $9\n\t" \
    "mfc2 $13, $10\n\t" \
    "mfc2 $14, $11\n\t" \
    "ctc2 $12, $13\n\t" \
    "ctc2 $13, $14\n\t" \
    "ctc2 $14, $15" \
    : : : "$12", "$13", "$14")
#define RR_GTE_SWC2_SXY_3P(p0, p1, p2) __asm__ volatile ("swc2 $12, 0(%0)\n\tswc2 $13, 0(%1)\n\tswc2 $14, 0(%2)" : : "r"(p0), "r"(p1), "r"(p2))

/* poly1.c GTE helper: save far colour (control regs $21-$23) to three words. */
#define RR_GTE_SAVE_FC(p0, p1, p2) __asm__ volatile ( \
    "cfc2 $8, $21\n\tcfc2 $9, $22\n\tcfc2 $10, $23\n\tsw $8, 0(%0)\n\tsw $9, 0(%1)\n\tsw $10, 0(%2)" \
    : : "r"(p0), "r"(p1), "r"(p2) : "$8", "$9", "$10", "memory")

/* tilerect.c GTE INTPL helper. */
#define RR_GTE_TILERECT_INTPL(ir2, p) __asm__ volatile( \
    "lhu $8, 0x4A(%1)\n\tnop\n\tmtc2 $8, $8\n\t" \
    "addiu $9, $0, 0x720\n\tmtc2 $9, $9\n\t" \
    "addu $10, $0, $0\n\tctc2 $10, $21\n\t" \
    "addiu $11, $0, 0x90\n\tmtc2 $11, $10\n\t" \
    "addiu $8, $0, 0xFF\n\tctc2 $8, $22\n\t" \
    "nop\n\tnop\n\t.word 0x4A980011\n\t" \
    "addiu %1, %1, 0x30\n\tswc2 $9, 0(%1)\n\tmfc2 %0, $10" \
    : "=r"(ir2), "+r"(p) : : "memory")

/* objdraw.c GTE helpers. */
#define RR_GTE_COPY_ROT_TR_TO_LLM_BK() __asm__ volatile ( \
    "cfc2 $12, $0; cfc2 $13, $1; ctc2 $12, $8; ctc2 $13, $9; " \
    "cfc2 $12, $2; cfc2 $13, $3; ctc2 $12, $10; ctc2 $13, $11; " \
    "cfc2 $12, $4; cfc2 $13, $5; ctc2 $12, $12; ctc2 $13, $13; " \
    "cfc2 $12, $6; cfc2 $13, $7; ctc2 $12, $14; ctc2 $13, $15" \
    : : : "$12", "$13")
#define RR_GTE_SET_IDENTITY_ROT_ZERO_TR(one) __asm__ volatile ( \
    "ctc2 %0, $0; ctc2 $0, $1; ctc2 %0, $2; ctc2 $0, $3; " \
    "ctc2 %0, $4; ctc2 $0, $5; ctc2 $0, $6; ctc2 $0, $7" \
    : : "r"(one))

#define RR_GTE_SWC2_RGB012_A3_SP16_20(a3) __asm__ volatile( \
    "lw $8, 16($29)\n\tlw $9, 20($29)\n\tswc2 $20, 0(%0)\n\tswc2 $21, 0($8)\n\tswc2 $22, 0($9)" \
    : : "r"(a3) : "$8", "$9", "memory")
#define RR_GTE_SWC2_RGB012_SP(off0, off1, off2) __asm__ volatile( \
    "lw $8, " #off0 "($29)\n\tlw $9, " #off1 "($29)\n\tlw $10, " #off2 "($29)\n\tswc2 $20, 0($8)\n\tswc2 $21, 0($9)\n\tswc2 $22, 0($10)" \
    : : : "$8", "$9", "$10", "memory")
#define RR_GTE_OUTER_PRODUCT(a0, a1, a2, op) __asm__ volatile( \
    "cfc2 $13, $0\n\tcfc2 $14, $2\n\tcfc2 $15, $4\n\t" \
    "lw $8, 0(%0)\n\tlw $9, 4(%0)\n\tlw $10, 8(%0)\n\t" \
    "ctc2 $8, $0\n\tctc2 $9, $2\n\tctc2 $10, $4\n\t" \
    "lwc2 $11, 8(%1)\n\tlwc2 $9, 0(%1)\n\tlwc2 $10, 4(%1)\n\t" \
    "nop\n\t.word " #op "\n\t" \
    "swc2 $25, 0(%2)\n\tswc2 $26, 4(%2)\n\tswc2 $27, 8(%2)\n\t" \
    "ctc2 $13, $0\n\tctc2 $14, $2\n\tctc2 $15, $4" \
    : : "r"(a0), "r"(a1), "r"(a2) : "$8", "$9", "$10", "$13", "$14", "$15", "memory")
#define RR_GTE_SETROT_5W(p) __asm__ volatile( \
    "lw $8, 0(%0)\n\tlw $9, 4(%0)\n\tlw $10, 8(%0)\n\tlw $11, 12(%0)\n\tlw $12, 16(%0)\n\t" \
    "ctc2 $8, $0\n\tctc2 $9, $1\n\tctc2 $10, $2\n\tctc2 $11, $3\n\tctc2 $12, $4" \
    : : "r"(p) : "$8", "$9", "$10", "$11", "$12")
#define RR_GTE_SETLIGHT_5W(p) __asm__ volatile( \
    "lw $8, 0(%0)\n\tlw $9, 4(%0)\n\tlw $10, 8(%0)\n\tlw $11, 12(%0)\n\tlw $12, 16(%0)\n\t" \
    "ctc2 $8, $8\n\tctc2 $9, $9\n\tctc2 $10, $10\n\tctc2 $11, $11\n\tctc2 $12, $12" \
    : : "r"(p) : "$8", "$9", "$10", "$11", "$12")
#define RR_GTE_LDV0_2W(p) __asm__ volatile( \
    "lw $8, 0(%0)\n\tlw $9, 4(%0)\n\tmtc2 $8, $0\n\tmtc2 $9, $1" \
    : : "r"(p) : "$8", "$9")
#define RR_GTE_STORE_IR_S16_T012(p) __asm__ volatile( \
    "mfc2 $8, $9\n\tmfc2 $9, $10\n\tmfc2 $10, $11\n\tsh $8, 0(%0)\n\tsh $9, 2(%0)\n\tsh $10, 4(%0)" \
    : : "r"(p) : "$8", "$9", "$10", "memory")
#define RR_GTE_SETCOL_5W(p) __asm__ volatile( \
    "lw $8, 0(%0)\n\tlw $9, 4(%0)\n\tlw $10, 8(%0)\n\tlw $11, 12(%0)\n\tlw $12, 16(%0)\n\t" \
    "ctc2 $8, $16\n\tctc2 $9, $17\n\tctc2 $10, $18\n\tctc2 $11, $19\n\tctc2 $12, $20" \
    : : "r"(p) : "$8", "$9", "$10", "$11", "$12")
#define RR_GTE_SETTR_3W(p) __asm__ volatile( \
    "lw $8, 20(%0)\n\tlw $9, 24(%0)\n\tlw $10, 28(%0)\n\t" \
    "ctc2 $8, $5\n\tctc2 $9, $6\n\tctc2 $10, $7" \
    : : "r"(p) : "$8", "$9", "$10")
#define RR_GTE_ROT_AVG3_STORE(a3) __asm__ volatile( \
    "lw $8, 16($29)\n\tlw $9, 20($29)\n\tlw $10, 24($29)\n\tlw $11, 28($29)\n\t" \
    "swc2 $12, 0(%0)\n\tswc2 $13, 0($8)\n\tswc2 $14, 0($9)\n\t" \
    "cfc2 $8, $31\n\tswc2 $8, 0($10)\n\tsw $8, 0($11)" \
    : : "r"(a3) : "$8", "$9", "$10", "$11", "memory")
#define RR_GTE_ROT_AVG4_STORE(a3) __asm__ volatile( \
    "lw $8, 16($29)\n\tlw $9, 20($29)\n\tlw $10, 24($29)\n\t" \
    "swc2 $12, 0($8)\n\tswc2 $13, 0($9)\n\tswc2 $14, 0($10)\n\t" \
    "cfc2 $3, $31\n\tlwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)\n\t" \
    "nop\n\t.word 0x4A180001\n\t" \
    "lw $8, 28($29)\n\tlw $9, 32($29)\n\tlw $10, 36($29)\n\t" \
    "swc2 $14, 0($8)\n\tcfc2 $8, $31\n\tswc2 $8, 0($9)\n\t" \
    "or $8, $8, $3\n\tsw $8, 0($10)" \
    : : "r"(a3) : "$3", "$8", "$9", "$10", "memory")

#endif
