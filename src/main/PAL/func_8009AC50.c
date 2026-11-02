/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */

typedef struct {
    volatile unsigned char data;
    unsigned char pad01[3];
    volatile unsigned short status;
    unsigned char pad06[2];
    volatile unsigned short mode;
    volatile unsigned short control;
    unsigned char pad0C[2];
    volatile unsigned short baud;
} RrSerialPortRegisters;

typedef struct {
    int active;
    int first, second;
} RrSerialRequest;

typedef struct {
    RrSerialRequest request;
    unsigned int receive_status;
} RrSerialReceiveRequest;

extern volatile RrSerialReceiveRequest D_801146D8;
extern RrSerialPortRegisters *D_800ABD18;
extern RrSerialPortRegisters D_801146C0;
extern int (*D_801146D0)(int, int);
extern short D_800ABD48[];
extern int func_80096E64(unsigned int, int);

int func_8009AC50(int first, int second) {
    int s0 = 0;
    int s1;
    int s5;
    /* ASM_BARRIER: retain the request base in $v1 before the loop. */
    volatile RrSerialReceiveRequest *p1 = &D_801146D8;

    /* ASM_BARRIER: keep $v1 live for the pre-loop stores and load. */
    __asm__("" : "=r"(p1) : "0"(p1));

    if (p1->request.active != 0) {
        return -1;
    }

    s5 = D_800ABD48[(D_801146C0.control & 0x300) >> 8];
    p1->request.second = second;
    p1->request.first = first;
    s1 = 0;

    if (p1->request.second != 0) {
        volatile RrSerialReceiveRequest *b = p1;
        do {
            if ((D_800ABD18->status & 5) != 5) {
                int five;
                /* ASM_BARRIER: hoist constant 5 into $s2 for the inner wait loop. */
                __asm__("" : "=r"(five) : "0"(5));
                do {
                    if (D_801146D0 != 0) {
                        if (D_801146D0(2, s0++) == 0) {
                            func_80096E64(0xF000000B, 0x100);
                            goto done;
                        }
                    }
                } while ((D_800ABD18->status & 5) != five);
            }
            if (s1 == 0) {
                b->receive_status = D_800ABD18->status & 0x80;
            }
            D_800ABD18->data = *(unsigned char *)D_801146D8.request.first;
            b->request.first += 1;
            s1 += 1;
            b->request.second -= 1;
            if (s1 == s5) {
                if ((D_800ABD18->status & 0x80) == b->receive_status) {
                    volatile unsigned int *rs;
                    /* ASM_BARRIER: hoist receive_status address into $s1 for the inner wait loop. */
                    __asm__("" : "=r"(rs) : "0"(&D_801146D8.receive_status));
                    do {
                        if (D_801146D0 != 0) {
                            if (D_801146D0(2, s0++) == 0) {
                                func_80096E64(0xF000000B, 0x100);
                                return second - D_801146D8.request.second - 1;
                            }
                        }
                    } while ((D_800ABD18->status & 0x80) == *rs);
                }
                s1 = 0;
            }
        } while (D_801146D8.request.second != 0);
    }
done:
    return second - D_801146D8.request.second;
}
