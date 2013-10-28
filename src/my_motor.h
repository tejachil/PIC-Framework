#ifndef MY_MOTOR_H
#define	MY_MOTOR_H
#include "public_messages.h"

void motor_control_thread(public_message_t *msg);
void motor_forward_both(void);
void motor_stop_both(void);

#endif	/* MY_MOTOR_H */

