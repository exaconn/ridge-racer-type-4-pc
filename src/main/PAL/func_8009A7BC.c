/* GCC_VERSION: 2.7.2 */
#include "rr/channels.h"

s32 func_8009A7BC(void *channel) {
    const RrChannelStatusView *state = channel;
    if (state->pending == 0 || state->phase != 0xFF) return 1;
    return 0;
}
