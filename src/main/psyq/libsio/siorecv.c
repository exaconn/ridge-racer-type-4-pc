#include "rr/serial.h"
#include "psyq/libsio/siorecv.h"

extern int (*SioPollHook)(int, int);
extern int DeliverEvent(unsigned int, int);

int _sio_read(u8 *buffer, int length) {
    int s0 = 0;
    int s1;
    int s5;
    volatile SerialReceiveRequest *p1 = &SioReceiveRequest;

    if (p1->request.active != 0) {
        return -1;
    }

    s5 = SioRxFifoSizeTable[(SioShadowRegs.control & 0x300) >> 8];
    p1->request.remaining = length;
    p1->request.cursor = buffer;
    s1 = 0;

    if (p1->request.remaining != 0) {
        volatile SerialReceiveRequest *b = p1;
        do {
            if ((SIO1_REGS->status & 5) != 5) {
                int five;
                five = 5;
                do {
                    if (SioPollHook != 0) {
                        if (SioPollHook(2, s0++) == 0) {
                            DeliverEvent(0xF000000B, 0x100);
                            goto done;
                        }
                    }
                } while ((SIO1_REGS->status & 5) != five);
            }
            if (s1 == 0) {
                b->receive_status = SIO1_REGS->status & 0x80;
            }
            SIO1_REGS->data = *SioReceiveRequest.request.cursor;
            b->request.cursor += 1;
            s1 += 1;
            b->request.remaining -= 1;
            if (s1 == s5) {
                if ((SIO1_REGS->status & 0x80) == b->receive_status) {
                    volatile u32 *rs;
                    rs = &SioReceiveRequest.receive_status;
                    do {
                        if (SioPollHook != 0) {
                            if (SioPollHook(2, s0++) == 0) {
                                DeliverEvent(0xF000000B, 0x100);
                                return length - SioReceiveRequest.request.remaining - 1;
                            }
                        }
                    } while ((SIO1_REGS->status & 0x80) == *rs);
                }
                s1 = 0;
            }
        } while (SioReceiveRequest.request.remaining != 0);
    }
done:
    return length - SioReceiveRequest.request.remaining;
}
