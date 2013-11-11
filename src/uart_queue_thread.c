#include "uart_queue_thread.h"
#include "maindefs.h"
#include "my_uart.h"
#include "my_gpio.h"

void uart_queue_lthread(int msgtype, int length, unsigned char *msgbuffer) {
    if (MSGT_UART_QUEUED_MSG == msgtype) {
        // Send the message over UART
        if (UART_ERR_NONE != uart_send_bytes(msgbuffer, length)) {
            SET_UART_QUEUE_ERROR_PIN();
        }
    }
}
