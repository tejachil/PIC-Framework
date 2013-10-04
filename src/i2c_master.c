/*
 * This file contains function definitions for I2C Master operations.
 */

#include "maindefs.h"
#ifndef __XC8
#include <i2c.h>
#else
#include <plib/i2c.h>
#endif
#include "my_i2c.h"
#include <string.h> // for memcpy

extern i2c_comm *ic_ptr;

#ifdef I2C_MASTER

/**
 * Handles an error in the I2C master driver.  Currently a placeholder.
 */
void i2c_master_handle_error(void);

void i2c_configure_master() {
#warning "Master configuration not tested"
    // Make sure the pins are set as input
    I2C1_SCL = 1;
    I2C1_SDA = 1;

    // Set the config bits located in the status register
    SSP1STATbits.SMP = 1;
    SSP1STATbits.CKE = 0;

    // Reset the control registers (just in case)
    SSP1CON1 = 0;
    SSP1CON2 = 0;

    // Set the module for I2C master mode (SSPM field of SSP1CON1)
    SSP1CON1 |= 0b01000;

    // Set the module for 100kHz operation using the given formula:
    // Fscl = Fosc / (4*(SSPxADD+1))
    // Solved for SSPxADD:
    // SSPxADD = (Fosc / (4*Fscl)) - 1
    // With Fscl = 100kHz and Fosc = 12MHz,  SSPxADD = 29
    SSP1ADD = 29;

    // Enable the module
    SSP1CON1bits.SSPEN = 1;
}

i2c_error_code i2c_master_write(unsigned char slave_addr, unsigned char const * const data, unsigned char data_length) {
#warning "Master write not completed"
    i2c_error_code ret_code = I2C_ERR_NONE;

    // Check if the driver is busy
    if (ic_ptr->state != I2C_IDLE) {
        ret_code = I2C_ERR_BUSY;

    }// Check the length of the provided data against the maximum length
    else if (data_length > MAXI2CBUF) {
        ret_code = I2C_ERR_MSGTOOLONG;

    }// Check that there is some data provided
    else if (0 == data_length) {
        ret_code = I2C_ERR_ZERO_DATA;

    }// Otherwise proceed
    else {
        // Set the main state to indicate a write in progress
        ic_ptr->state = I2C_WRITE;

        // Copy the provided data to the internal buffer
        memcpy(ic_ptr->buffer, data, data_length);

        // Save the length of the copied data
        ic_ptr->buffer_length = data_length;

        // Reset the buffer index
        ic_ptr->buffer_index = 0;

        // Save the slave address
        ic_ptr->slave_addr = slave_addr;

        // Assert a Start condition and move to the next substate
        SSP1CON2bits.SEN = 1;
        ic_ptr->substate = I2C_SUBSTATE_START_SENT;

    }

    return ret_code;
}

i2c_error_code i2c_master_read(unsigned char slave_addr, unsigned char reg, unsigned char data_length) {
#warning "Master read not implemented"
    i2c_error_code ret_code;
    return ret_code;
}

void i2c_master_handler() {
#warning "Master handler not completed"
    // Make sure we are not in an idle state, or else we don't know why this
    // interrupt triggered.
    if (ic_ptr->state != I2C_IDLE) {
        switch (ic_ptr->substate) {
            case I2C_SUBSTATE_START_SENT:
            {
                // The Start has completed, send the address and W bit (0)
                SSPBUF = (ic_ptr->slave_addr << 1);

                // Move to the next substate
                ic_ptr->substate = I2C_SUBSTATE_ADDR_W_SENT;

                break;
            } // End case I2C_SUBSTATE_START_SENT

            case I2C_SUBSTATE_ADDR_W_SENT:
            {
                // Sending of the address has completed, check the ACK status
                // and send some data.

#ifndef I2C_MASTER_IGNORE_NACK
                // Check for a NACK (ACKSTAT 0 indicates ACK received)
                if (1 == SSPCON2bits.ACKSTAT) {
                    // NACK probably means there is no slave with that address
                    i2c_master_handle_error();
                } else
#endif
                {
                    // Send the first byte
                    SSPBUF = ic_ptr->buffer[ic_ptr->buffer_index];

                    // Move to the next substate
                    ic_ptr->substate = I2C_SUBSTATE_DATA_SENT;
                }
                break;
            } // End case I2C_SUBSTATE_ADDR_W_SENT

            case I2C_SUBSTATE_DATA_SENT:
            {
                // A data byte has completed, send the next one or finish the
                // write.

#ifndef I2C_MASTER_IGNORE_NACK
                // Check for a NACK (ACKSTAT 0 indicates ACK received)
                if (1 == SSPCON2bits.ACKSTAT) {
                    // NACK probably means there is no slave with that address
                    i2c_master_handle_error();
                } else
#endif
                {
                    // Increment the data index
                    ic_ptr->buffer_index++;

                    // Check if there is more data to send
                    if (ic_ptr->buffer_index < ic_ptr->buffer_length) {
                        // Send the next byte
                        SSPBUF = ic_ptr->buffer[ic_ptr->buffer_index];

                        // Stay in the same substate (we just sent data)
                        ic_ptr->substate = I2C_SUBSTATE_DATA_SENT;

                    }// Otherwise there is no more data
                    else {
                        // Assert a Stop condition (the write is complete)
                        SSPCON2bits.PEN = 1;

                        // Move to the next substate
                        ic_ptr->substate = I2C_SUBSTATE_STOP_SENT;
                    }

                } // End ACK check - else

                break;
            } // End case I2C_SUBSTATE_DATA_SENT

            case I2C_SUBSTATE_ERROR:
            case I2C_SUBSTATE_STOP_SENT:
            {
                // The Stop has completed, return to idle and send a message to
                // main if appropriate

                // Send a success message to main only if this point was reached
                // through normal operation.
                if (ic_ptr->substate != I2C_SUBSTATE_ERROR) {
                    ToMainHigh_sendmsg(0, MSGT_I2C_MASTER_SEND_COMPLETE, NULL);

                }// Otherwise send the appropriate error message to main
                else {
                    // Send an error message to main
                    if (I2C_WRITE == ic_ptr->state) {
                        ToMainHigh_sendmsg(0, MSGT_I2C_MASTER_SEND_FAILED, NULL);
                    } else if (I2C_READ == ic_ptr->state) {
                        ToMainHigh_sendmsg(0, MSGT_I2C_MASTER_RECV_FAILED, NULL);
                    }
                }

                // Return to idle states
                ic_ptr->state = I2C_IDLE;
                ic_ptr->substate = I2C_SUBSTATE_IDLE;

                break;
            } // End case I2C_SUBSTATE_STOP_SENT

            default:
            {
                i2c_master_handle_error();
                break;
            } // End default case
        }
    } else {
        i2c_master_handle_error();
    }
}

void i2c_master_handle_error() {
#warning "I2C errors not handled completely"
    // Assert a Stop condition to reset the bus
    SSPCON2bits.PEN = 1;

    // Move to the error substate
    ic_ptr->substate = I2C_SUBSTATE_ERROR;
}

#endif //ifdef I2C_MASTER
