#ifndef USER_INTERRUPTS_H
#define USER_INTERRUPTS_H

static int tickC = 0;
static int totalRevolution = 0;

void timer0_int_handler(void);
void timer1_int_handler(void);

#endif // ifndef USER_INTERRUPTS_H
