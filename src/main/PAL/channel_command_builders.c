#include "rr/channels.h"

void func_80099618(void *channel, s32 value) {
    RrChannelStatusView *state=channel;
    state->command37=0x43;
    state->data2C=&state->argument24;
    state->argument24=value;
    state->length36=1;
}

void func_80099638(void *channel) {
    RrChannelStatusView *state=channel;
    state->command37=0x45;
    state->data2C=0;
    state->length36=0;
}

void func_8009964C(void *channel, s32 value) {
    RrChannelStatusView *state=channel;
    state->command37=0x4C;
    state->data2C=&state->argument24;
    state->argument24=value;
    state->length36=1;
}

void func_8009966C(void *channel, s32 value) {
    RrChannelStatusView *state=channel;
    state->command37=0x46;
    state->data2C=&state->argument24;
    state->argument24=value;
    state->length36=1;
}

void func_8009968C(void *channel, s32 value) {
    RrChannelStatusView *state=channel;
    state->command37=0x47;
    state->data2C=&state->argument24;
    state->argument24=value;
    state->length36=1;
}

void func_800996AC(void *channel) {
    RrChannelStatusView *state=channel;
    state->command37=0x4B;
    state->data2C=0;
    state->length36=0;
}
