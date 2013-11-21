#ifndef MY_MOTOR_H
#define	MY_MOTOR_H
#include "public_messages.h"

extern int tickCount;
extern int totalRevolutions;
extern int countFlag;
extern int tickCountTwo;
extern int totalRevolutionsTwo;

void encoders_init(void);
void motor_control_thread(public_message_t *msg);
void motor_forward_both_slow(void);
void motor_forward_both(void);
void motor_forward_both_fast(void);
void motor_stop_both(void);
void motor_turn(void);
void motor_fix_left_slow(void);
void motor_fix_left(void);
void motor_fix_left_fast(void);
void motor_fix_right_slow(void);
void motor_fix_right(void);
void motor_fix_right_fast(void);

#endif	/* MY_MOTOR_H */

