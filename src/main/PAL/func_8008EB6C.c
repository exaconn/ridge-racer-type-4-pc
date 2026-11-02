#include "rr/cd_read_request.h"
#include "rr/cd.h"

extern s32 func_8008B154(s32);
extern u8 func_8008BF94(void);
extern s32 func_8008BFA4(void);
extern u8 *func_8008BFC4(void);
extern void func_8008C03C(void);
extern RrLibCompletionCallback func_8008C118(RrLibCompletionCallback);
extern RrLibCompletionCallback func_8008C12C(RrLibCompletionCallback);
extern RrLibCompletionCallback func_8008C55C(RrLibCompletionCallback);
extern s32 func_8008C6A4(void *);
extern s32 func_800963DC(const char *, ...);
extern const char D_80011D40[], D_80011D58[];
extern volatile u32 D_800A67BC;
extern void func_8008E828(s32, u8 *);
extern void func_8008EA9C(void);

s32 func_8008EB6C(s32 restart) {
    u8 mode_byte;
    register u32 requested;
    register RrLibReadRequestState *state;
    /* ASM_BARRIER: keep the captured mode address in s1 and preserve the raw
     * restart argument in s2 through the command and status helpers. */
    register volatile u32 *mode_slot __asm__("$17");
    register s32 retry __asm__("$18") = restart;
    register volatile RrLibReadRequestState *shell_state;
    register volatile RrLibReadRequestState *failed_state;
    u32 timestamp;
    s32 completed;
    register RrLibCompletionCallback callback;
    /* ASM_BARRIER: a0/a1 stage independent command/null-parameter values;
     * neither bound value is used across a call that can overwrite it. */
    register s32 command __asm__("$4");
    register const u8 *parameters __asm__("$5");

    func_8008C118(0);
    func_8008C12C(0);
    if (D_800A63F4 & 1) {
        func_8008C55C(0);
    }
    if (func_8008BF94() & 0x10) {
        if (!(func_8008B154(-1) & 63)) {
            func_800963DC(D_80011D40);
        }
        func_8008C27C(1, 0);
        timestamp = func_8008B154(-1);
        shell_state = &D_800A67B0;
        shell_state->start1C = timestamp;
        shell_state->remaining14 = -1;
        return shell_state->remaining14;
    }
    if (retry != 0) {
        /* ASM_BARRIER: keep retry-only diagnostic address formation in this
         * branch instead of copying it into its predecessor's delay slot. */
        __asm__("" : );
        func_800963DC(D_80011D58);
        command = 9;
        parameters = 0;
        /* ASM_BARRIER: stage the null parameter after the command value. */
        __asm__("" : "+r"(parameters) : "r"(command));
        func_8008C140(command, parameters, 0);
        if (!func_8008C140(2, func_8008BFC4(), 0)) {
            goto failed;
        }
    }
    func_8008C03C();
    /* ASM_BARRIER: form the captured mode address after reset returns. */
    __asm__("" : : : "$17");
    mode_slot = &D_800A67BC;
    /* ASM_BARRIER: preserve the complete address before the mode load. */
    __asm__("" : "+r"(mode_slot));
    requested = *mode_slot;
    mode_byte = requested;
    requested &= 255;
    if (requested != func_8008BFA4() || retry != 0) {
        if (!func_8008C140(14, &mode_byte, 0)) {
failed:
            /* ASM_BARRIER: retain one shared failure block with fresh state
             * publication and reload, rather than duplicating address setup. */
            __asm__("" : );
            failed_state = &D_800A67B0;
            failed_state->remaining14 = -1;
            return failed_state->remaining14;
        }
    }
    completed = func_8008C6A4(func_8008BFC4());
    /* ASM_BARRIER: the success-state address is formed after decoding. */
    __asm__("" : : : "$16");
    callback = (RrLibCompletionCallback)func_8008E828;
    state = &D_800A67B0;
    /* ASM_BARRIER: stage the callback address before the state address, then
     * publish the decoded sector in the callback setter's delay slot. */
    __asm__("" : "+r"(state) : "r"(callback));
    state->completed20 = completed;
    func_8008C12C(callback);
    command = 6;
    if (D_800A63F4 & 1) {
        func_8008C55C(func_8008EA9C);
        command = 6;
    }
    state->position08 = state->buffer04;
    func_8008C27C(command, 0);
    state->remaining14 = state->sectors00;
    ((volatile RrLibReadRequestState *)state)->retry18 = func_8008B154(-1);
    return ((volatile RrLibReadRequestState *)state)->remaining14;
}
