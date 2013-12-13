#include "my_motor.h"
#include "my_uart.h"
#include "messages.h"
#include <timers.h>
#include "my_encoder.h"
#include "my_gyro.h"
#include "my_i2c2.h"

static unsigned char forwardFast[] = {1, 128};
static unsigned char forward[] = {0x34, 0xB3};
static unsigned char forwardSlow[] = {0x37, 0xB6};
static unsigned char backward[] = {127, 255};
static unsigned char turnLeftSlow[] = {0x2E, 0xD2};
static unsigned char turnLeft[] = {1, 255};
static unsigned char turnLeftFast[] = {0x28, 0xD8};
static unsigned char turnRightSlow[] = {0x52, 0xAE};
static unsigned char turnRight[] = {127, 128};
static unsigned char turnRightFast[] = {0x5B, 0xA5};
static unsigned char stop[] = {0, 0};
int timer1_counter = 0;
int countFlag;
int angleCalc;

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
                case MOV_CMD_GO_DIST:
                {
                    unsigned int target_ticks;
                    unsigned char target_rotations;

                    // Clear encoder counters
                    encoders_reset();

                    // Tell the encoders when to stop
                    target_ticks = msg->data[1] | (((unsigned int) msg->data[2]) << 8);
                    target_rotations = msg->data[3];
                    encoders_set_stop_target(target_rotations, target_ticks);

                    // Start moving forward
                    motor_forward_both();

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
    uart_send_bytes(backward, 2);
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
    countFlag = 1;
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