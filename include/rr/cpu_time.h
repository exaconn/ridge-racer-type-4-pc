#ifndef RR_CPU_TIME_H
#define RR_CPU_TIME_H

/* PS1 CPU time of a game routine. Native code runs in no emulated time, so
 * the routines whose work decides whether a frame fits in its field (the
 * loading screen's particles and text, a load's bank setup) spend what they
 * took on the PS1: their own cost, without the routines they call, measured
 * per call on DuckStation in system clocks (33.8688 MHz). SDK and BIOS calls
 * take theirs in PsyZ. */
void Psyz_SpendCycles(unsigned cycles);

#define RR_CPU_CYCLES(cycles) Psyz_SpendCycles(cycles)

#endif
