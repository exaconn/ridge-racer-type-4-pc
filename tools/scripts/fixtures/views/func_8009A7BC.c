/* GCC_VERSION: 2.7.2 */

#include "rr/channels.h"

s32 _padChannelBusy(void *channel) {
    const ChannelStatusView *state = channel;
    if (state->pending == 0 || state->phase != 0xFF) return 1;
    return 0;
}
