#include "maindefs.h"
#include "public_messages.h"
#include "my_gpio.h"
#include "my_i2c.h"

void i2c_queue_lthread(int msgtype, int length, unsigned char *msgbuffer) {
    if (MSGT_I2C_QUEUED_MSG == msgtype) {
        // Cast the msgbuffer data to the public message type
        public_message_t * msg = (public_message_t *) msgbuffer;

        // Copy important message fields
        const unsigned char type = msg->message_type;
        const unsigned char *raw_msg = msg->raw_message_bytes;

        // Check the message type to determine where to send the message and
        // send it along.
        switch (type) {
            case PUB_MSG_T_SENS_DIST:
            {
                // Determine the expected size for the message when it is received
                // from the I2C slave device.
                const unsigned char exp_msg_size = public_message_data_size[type] + PUB_MSG_MIN_SIZE;

                // Read the full message from the slave
                i2c_master_read(SENSOR_PIC_ADDR, type, exp_msg_size);

                break;
            } // End requests for Proximity Sensors PIC

            case PUB_MSG_T_MOV_CMD:
            case PUB_MSG_T_TURN_CMD:
            case PUB_MSG_T_FIX_CMD:
            {
                // Forward the full message to the slave
                i2c_master_write(MOTOR_PIC_ADDR, raw_msg, length);

                break;
            } // End commands for Motor Controller PIC

            default:
            {
                // We shouldn't be receiving any other message types over
                // UART
                SET_UART_MESSAGE_ERROR_PIN();
            }
        } // End switch message_type
    }
}
