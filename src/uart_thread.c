#include "maindefs.h"
#include "my_i2c.h"
#include "my_gpio.h"
#include <stdio.h>
#include "uart_thread.h"
#include "public_messages.h"
#include "my_uart.h"
#include "my_gpio.h"
#include "messages.h"

// Create blank GPIO definitions if they aren't defined
#ifndef SET_UART_MESSAGE_ERROR_PIN
#warning "Blank GPIO definition"
#define SET_UART_MESSAGE_ERROR_PIN()
#endif

int uart_lthread(uart_thread_struct *uptr, int msgtype, int length, unsigned char *msgbuffer) {
    if (msgtype == MSGT_OVERRUN) {

    } else if (msgtype == MSGT_UART_DATA) {

#if defined(MASTER_PIC)
        // Check if this is a valid message
        if (length >= PUB_MSG_MIN_SIZE) {
            // Cast the msgbuffer data to the public message type
            public_message_t * msg = (public_message_t *) msgbuffer;

            // Add the message to the queue to be sent on I2C
            ToI2C_sendmsg(MSGT_I2C_QUEUED_MSG, msg);

        } // End if(length >= PUB_MSG_MIN_SIZE)
#endif // defined(MASTER_PIC)

    } // End if(msgtype == MSGT_UART_DATA)
}