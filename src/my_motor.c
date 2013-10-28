#include "my_motor.h"
#include "my_uart.h"

static char forward[] = {0x39, 0xB8};
static char backward[] = {0x47, 0xC6};
static char stop[] = {0, 0};

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
    }
}

void motor_forward_both() {
    uart_send_bytes(&forward, 2);
}

void motor_stop_both() {
    uart_send_bytes(&stop, 2);
}