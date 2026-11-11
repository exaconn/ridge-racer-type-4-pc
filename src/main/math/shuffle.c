#include "game/math/shuffle.h"
#include "rr/frame_loop.h"
#include "psyq/libc/rand.h"
void ShuffleSlotOrder(void) {
    int i, a, b, x, y, t;
    PsyqSrand(FrameCounter);
    for (i = 7; i >= 0; i--)
        SlotOrder.order04[i] = i;
    i = 0;
    do {
        a = PsyqRand();
        x = a % 8;
        b = PsyqRand();
        y = (x + b % 4) % 7;
        t = SlotOrder.order04[x];
        i++;
        SlotOrder.order04[x] = SlotOrder.order04[y];
        SlotOrder.order04[y] = t;
    } while (i < 0x20);
    SlotOrder.cursor24 = 0;
}

/* Initial values of the globals this module owns. */
SlotOrder28 SlotOrder;
