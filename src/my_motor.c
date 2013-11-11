#include "my_motor.h"
#include "my_uart.h"
#include "messages.h"
#include <timers.h>
#include "my_encoder.h"

static unsigned char forward[] = {0x34, 0xB3};
static unsigned char backward[] = {0x4C, 0xCB};
static unsigned char turnLeft[] = {0x55, 0xAB};
static unsigned char turnRight[] = {0x2B, 0xD5};
static unsigned char stop[] = {0, 0};
int timer1_counter = 0;
int tickCount;
int totalRevolutions;
int countFlag;

void encoders_init() {
    tickCount = 0;
    totalRevolutions = 0;
}

void motor_control_thread(public_message_t *msg) {
    switch (msg->message_type) {
        case PUB_MSG_T_MOV_CMD:
        {
            // Check if it's a move or a stop
            switch (msg->data[0]) {
                case MOV_CMD_GO:
                {
                    motor_forward_both();
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
            motor_turn();
            break;
        }
        case PUB_MSG_T_FIX_CMD:
        {
            switch (msg->data[0]) {
                case FIX_CMD_LEFT:
                {
                    motor_fix_left();

                    break;
                }
                case FIX_CMD_RIGHT:
                {
                    motor_fix_right();
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

void motor_forward_both() {
    countFlag = 1;
    uart_send_bytes(forward, 2);
}

void motor_stop_both() {
    uart_send_bytes(stop, 2);
}

void motor_turn() {
    countFlag = 0;
    uart_send_bytes(turnRight, 2);
}

void motor_fix_left() {
    countFlag = 0;
    uart_send_bytes(turnLeft, 2);
}

void motor_fix_right() {
    countFlag = 0;
    uart_send_bytes(turnRight, 2);
}