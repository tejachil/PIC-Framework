#include "maindefs.h"
#include "my_i2c.h"
#include "my_gpio.h"
#include <stdio.h>
#include "uart_thread.h"
#include "public_messages.h"
#include "my_uart.h"
#include "my_gpio.h"
#include "messages.h"

int uart_lthread(int msgtype, int length, unsigned char *msgbuffer) {
    if (msgtype == MSGT_OVERRUN) {

    } else if (msgtype == MSGT_UART_DATA) {

#if defined(MASTER_PIC)
        // Check if this is a valid message
        if (length >= PUB_MSG_MIN_SIZE) {
            // Cast the msgbuffer data to the public message type
            public_message_t * msg = (public_message_t *) msgbuffer;

            // Add the message to the queue to be sent on I2C
            if (MSGSEND_OKAY != ToI2C_sendmsg(MSGT_I2C_QUEUED_MSG, msg)) {
                SET_I2C_QUEUE_ERROR_PIN();
            }

        } // End if(length >= PUB_MSG_MIN_SIZE)
#endif // defined(MASTER_PIC)

    } // End if(msgtype == MSGT_UART_DATA)
}