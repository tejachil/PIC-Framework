#include "maindefs.h"
#include "my_gpio.h"
#include <stdio.h>
#include "uart_thread.h"
#include "public_messages.h"
#include "my_uart.h"

// This is a "logical" thread that processes messages from the UART
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

int uart_lthread(uart_thread_struct *uptr, int msgtype, int length, unsigned char *msgbuffer) {
    if (msgtype == MSGT_OVERRUN) {
    } else if (msgtype == MSGT_UART_DATA) {
        if (length > 0) {
            public_message_t * msg = (public_message_t *) msgbuffer;

            // Echo received message
            uart_send_bytes(msg->raw_message_bytes, length);
        }
    }
}