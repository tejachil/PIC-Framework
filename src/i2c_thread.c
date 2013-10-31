#include "i2c_thread.h"
#include "my_i2c.h"
#include "my_adc.h"
#include "maindefs.h"
#include "public_messages.h"
#include "my_uart.h"

/** Last received message request.  Initialized to an invalid value. */
static public_message_type_t last_message_request = NUM_PUB_MSG_T;

/**
 * Accept an incoming message or store a requested message type.
 * @param length Number of bytes received over I2C.
 * @param msgbuffer Bytes received over I2C.
 */
void i2c_lthread_handle_slave_write(int length, unsigned char *msgbuffer);

/**
 * Compose and send a response to a data request.
 * @param type The message type being requested.
 */
void i2c_lthread_send_slave_response(const public_message_type_t type);

void i2c_lthread(int msgtype, int length, unsigned char *msgbuffer) {
    switch (msgtype) {

#if defined(I2C_SLAVE)

            // DATA indicates a write to this slave device
        case MSGT_I2C_DATA:
        {
            // Not worried about the event count that comes at the end of this
            // message type, just decrement the length to ignore it
            length--;
            
            // Handle the write
            i2c_lthread_handle_slave_write(length, msgbuffer);

            break;
        } // End case DATA

            // RQST indicates a read from this slave device
        case MSGT_I2C_RQST:
        {
            // Respond to the request
            i2c_lthread_send_slave_response(last_message_request);

            break;
        } // End case RQST

#elif defined(I2C_MASTER)

            // RECV_COMPLETE indicates a completed read by this master device
        case MSGT_I2C_MASTER_RECV_COMPLETE:
        {
            // Make sure the message is long enough to be valid
            if (length >= PUB_MSG_MIN_SIZE) {
                // Send the message directly to UART
                uart_send_bytes(msgbuffer, length);
            }

            break;
        } // End case RECV_COMPLETE

            // SEND_COMPLETE indicates a completed write from this master device
        case MSGT_I2C_MASTER_SEND_COMPLETE:
            // SEND_FAILED indicates a failed write from this master device
        case MSGT_I2C_MASTER_SEND_FAILED:
            // RECV_FAILED indicates a failed read by this master device
        case MSGT_I2C_MASTER_RECV_FAILED:

#endif // I2C_SLAVE / I2C_MASTER

            // The following cases (and the cases above) indicate an error
        case MSGT_I2C_DBG:
        default:
        {
#warning "Unhandled error case"
        } // End cases DBG and default

    } // End switch(msgtype)
}

#if defined(I2C_SLAVE)

void i2c_lthread_handle_slave_write(int length, unsigned char *msgbuffer) {
    // Check the length to determine if this is a complete message or just a
    // message request
    if (length >= PUB_MSG_MIN_SIZE) {
        // Cast the received data as a public message
        public_message_t *msg = (public_message_t *) msgbuffer;

        // Check the message type to determine what to do with it
        switch (msg->message_type) {

#if defined(SENSOR_PIC)
                // Any messages which may be written to the Proximity Sensors PIC
                // should be added here.
#elif defined(MOTOR_PIC)

            case PUB_MSG_T_MOV_CMD:
            {
                // Handle the MOV_CMD here
                //LATBbits.LATB0 ^= 1;
                motor_control_thread(msg);

                break;
            } // End case MOV_CMD

            case PUB_MSG_T_TURN_CMD:
            {
                // Handle the TURN_CMD here
                LATBbits.LATB1 ^= 1;

                break;
            } // End case TURN_CMD

#endif // SENSOR_PIC / MOTOR_PIC

            default:
            {
#warning "Unhandled error case"
            } // End default case

        } // End switch(msg->message_type)

    } else if (length == 1) {
        // Simply save the requested message type ("register address")
        last_message_request = *msgbuffer;
    }
}

void i2c_lthread_send_slave_response(const public_message_type_t type) {
    // Message structure for the response
    public_message_t response;

    // Flag to indicate if a response should be sent
    unsigned char send_response = 1;

    // Check the message type to determine what data to send back
    switch (type) {

#if defined(SENSOR_PIC)

            // SENS_DIST is a request for sensor data
        case PUB_MSG_T_SENS_DIST:
        {
            int i = 0;

            // Fill in the message data
            for (i = 0; i < NUMBER_OF_CHANNELS; ++i) {
                int adc_val = adc_read(i);
                response.data[2*i] = (adc_val & 0xFF00) >> 8;
                response.data[(2 * i) + 1] = adc_val & 0x00FF;
            }

            break;
        } // End case SENS_DIST

#elif defined (MOTOR_PIC)
            // Any responses from the Motor Controller PIC would be here.
#endif // SENSOR_PIC / MOTOR_PIC

            // Invalid message type
        case NUM_PUB_MSG_T:
        default:
        {
            // Do not send a response
            send_response = 0;
        } // End error cases

    } // End switch(type)

    // Send the response if appropriate
    if (send_response) {
        // Fill in the message type, count, and data length
        response.message_type = type;
        response.message_count = public_message_get_count(type);
        response.data_length = public_message_data_size[type];

        // Send the full message as the reply
        start_i2c_slave_reply(response.data_length + PUB_MSG_MIN_SIZE, response.raw_message_bytes);
    }
}

#endif // defined(I2C_SLAVE)
