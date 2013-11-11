#ifndef __interrupts
#define __interrupts

// enable the interrupts (high and low priority)
void enable_interrupts(void);
// Are we calling from a high-priority interrupt?
int in_high_int(void);
// Is a low-priority interrupt active?
int low_int_active(void);
// Are we calling from a low-priority interrupt?
int in_low_int(void);
// Are we calling this from the "main" routine (i.e., not an
// interrrupt handler?
int in_main(void);

#endif
