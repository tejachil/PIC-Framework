#include "my_motor.h"
#include "my_uart.h"
#include "messages.h"
#include <timers.h>

static char forward[] = {0x39, 0xB8};
static char backward[] = {0x47, 0xC6};
static char turnRight[] = {0x52, 0xAE};
static char turnLeft[] = {0x2E, 0xD2};
static char stop[] = {0, 0};

void timer1_motor_counter() {
    //CloseTimer1();
    //LATBbits.LATB5 ^= 1;
    uart_send_bytes(&stop, 2);

}

void motor_control_thread(public_message_t *msg) {
    LATBbits.LATB3 ^= 1;
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
    LATBbits.LATB3 ^= 1;
}

void motor_forward_both() {
    uart_send_bytes(&forward, 2);
}

void motor_stop_both() {
    uart_send_bytes(&stop, 2);
}

void motor_turn() {
    uart_send_bytes(&turnRight, 2);
}

void motor_fix_left() {
    uart_send_bytes(&turnLeft, 2);
    //WriteTimer1(50535);
    //OpenTimer1(TIMER_INT_ON & T1_PS_1_2 & T1_16BIT_RW & T1_SOURCE_INT & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF);
}

void motor_fix_right() {

}