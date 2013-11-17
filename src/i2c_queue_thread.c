#include "maindefs.h"
#include "public_messages.h"
#include "my_gpio.h"
#include "my_i2c.h"

void i2c_queue_lthread(int msgtype, int length, unsigned char *msgbuffer) {
    if (MSGT_I2C_QUEUED_MSG == msgtype) {
#ifdef MASTER_PIC
        // Cast the msgbuffer data to the public message type
        public_message_t * msg = (public_message_t *) msgbuffer;

        // Copy important message fields
        const unsigned char type = msg->message_type;
        const unsigned char *raw_msg = msg->raw_message_bytes;

        // Determine the total size for this message
        const unsigned char msg_total_size = public_message_data_size[type] + PUB_MSG_MIN_SIZE;

        // Check the message type to determine where to send the message and
        // send it along.
        switch (type) {
            case PUB_MSG_T_SENS_DIST:
            {
                // Read the full message from the slave
                if (I2C_ERR_NONE != i2c_master_read(SENSOR_PIC_ADDR, type, msg_total_size)) {
                    SET_I2C_QUEUE_ERROR_PIN();
                }

                break;
            } // End requests for Proximity Sensors PIC

            case PUB_MSG_T_MOV_CMD:
            case PUB_MSG_T_TURN_CMD:
            case PUB_MSG_T_FIX_CMD:
            {
                // Forward the full message to the slave
                if (I2C_ERR_NONE != i2c_master_write(MOTOR_PIC_ADDR, raw_msg, msg_total_size)) {
                    SET_I2C_QUEUE_ERROR_PIN();
                }

                break;
            } // End commands for Motor Controller PIC

            case PUB_MSG_T_ENCODER_DATA:
            case PUB_MSG_T_GYRO_DATA:
            case PUB_MSG_T_TURN_STATUS:
            {
                // Read the full message from the slave
                if (I2C_ERR_NONE != i2c_master_read(MOTOR_PIC_ADDR, type, msg_total_size)) {
                    SET_I2C_QUEUE_ERROR_PIN();
                }

                break;
            } // End requests for Motor Controller PIC

            default:
            {
                // We shouldn't be receiving any other message types over
                // UART
                SET_UART_MESSAGE_ERROR_PIN();
            }
        } // End switch message_type
#endif // ifdef MASTER_PIC
    }
}
