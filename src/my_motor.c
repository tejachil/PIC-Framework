#include "my_motor.h"
#include "my_uart.h"
#include "messages.h"
#include <timers.h>
#include "my_encoder.h"
#include "my_gyro.h"
#include "user_interrupts.h"
#include "my_i2c2.h"

static unsigned char forwardFast[] = {0x37, 0xB0};
static unsigned char forward[] = {0x34, 0xB3};
static unsigned char forwardSlow[] = {0x37, 0xB6};
static unsigned char backward[] = {0x4C, 0xCB};
static unsigned char turnLeftSlow[] = {0x2E, 0xD2};
static unsigned char turnLeft[] = {0x2B, 0xD5};
static unsigned char turnLeftFast[] = {0x28, 0xD8};
static unsigned char turnRightSlow[] = {0x52, 0xAE};
static unsigned char turnRight[] = {0x55, 0xAB};
static unsigned char turnRightFast[] = {0x5B, 0xA5};
static unsigned char stop[] = {0, 0};
int timer1_counter = 0;
int tickCount;
int tickCountTwo;
int totalRevolutions;
int totalRevolutionsTwo;
int countFlag;
int angleCalc;

void encoders_init() {
    tickCount = 0;
    tickCountTwo = 0;
    totalRevolutions = 0;
    totalRevolutionsTwo = 0;
    encoderOne = PORTBbits.RB4;
    encoderTwo = PORTBbits.RB5;
}

void motor_control_thread(public_message_t *msg) {
    switch (msg->message_type) {
        case PUB_MSG_T_MOV_CMD:
        {
            // Check if it's a move or a stop
            switch (msg->data[0]) {
                case MOV_CMD_GO_SLOW:
                {
                    motor_forward_both_slow();
                    break;
                }
                case MOV_CMD_GO:
                {
                    motor_forward_both();
                    break;
                }
                case MOV_CMD_GO_FAST:
                {
                    motor_forward_both_fast();
                    break;
                }
                case MOV_CMD_STOP:
                {
                    motor_stop_both();
                    break;
                }
            }

            break;
        }
        case PUB_MSG_T_TURN_CMD:
        {
            i2c2_master_write(GYRO_SLAVE_ADDRESS, gyro_init_data, firstMessageLength);
            angleCalc = (int) msg->data[0];
            timer0_counter_start(angleCalc);
            motor_turn();
            break;
        }
        case PUB_MSG_T_FIX_CMD:
        {
            switch (msg->data[0]) {
                case FIX_CMD_LEFT_SLOW:
                {
                    motor_fix_left_slow();
                    break;
                }
                case FIX_CMD_LEFT:
                {
                    motor_fix_left();
                    break;
                }
                case FIX_CMD_LEFT_FAST:
                {
                    motor_fix_left_fast();
                    break;
                }
                case FIX_CMD_RIGHT_SLOW:
                {
                    motor_fix_right_slow();
                    break;
                }
                case FIX_CMD_RIGHT:
                {
                    motor_fix_right();
                    break;
                }
                case FIX_CMD_RIGHT_FAST:
                {
                    motor_fix_right_fast();
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        default:
        {
            break;
        }
    }
}

void motor_forward_both_slow() {
    countFlag = 1;
    uart_send_bytes(forwardSlow, 2);
}

void motor_forward_both() {
    countFlag = 1;
    uart_send_bytes(forward, 2);
}

void motor_forward_both_fast() {
    countFlag = 1;
    uart_send_bytes(forwardFast, 2);
}

void motor_stop_both() {
    uart_send_bytes(stop, 2);
}

void motor_turn() {
    countFlag = 0;
    uart_send_bytes(turnRight, 2);
}

void motor_fix_left_slow() {
    countFlag = 0;
    uart_send_bytes(turnLeftSlow, 2);
}

void motor_fix_left() {
    countFlag = 0;
    uart_send_bytes(turnLeft, 2);
}

void motor_fix_left_fast() {
    countFlag = 0;
    uart_send_bytes(turnLeftFast, 2);
}

void motor_fix_right_slow() {
    countFlag = 0;
    uart_send_bytes(turnRightSlow, 2);
}

void motor_fix_right() {
    countFlag = 0;
    uart_send_bytes(turnRight, 2);
}

void motor_fix_right_fast() {
    countFlag = 0;
    uart_send_bytes(turnRightFast, 2);
}