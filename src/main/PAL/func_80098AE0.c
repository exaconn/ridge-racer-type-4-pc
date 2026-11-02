/* GCC_VERSION: 2.7.2 */
#include "rr/channels.h"
void func_80098AE0(u8 *channel) {
    RrChannelStatusView *state=(RrChannelStatusView *)channel;
    switch(state->phase) {
    case 2:func_80099638(state);break;
    case 3:func_8009964C(state,state->argumentE4);break;
    case 4:func_8009968C(state,state->state47);break;
    }
}
