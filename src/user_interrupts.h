#ifndef USER_INTERRUPTS_H
#define USER_INTERRUPTS_H

extern int encoderOne;
extern int encoderTwo;


void timer0_int_handler(void);
void timer1_int_handler(void);

void encoder_interrupt_handler(void);

#endif // ifndef USER_INTERRUPTS_H
