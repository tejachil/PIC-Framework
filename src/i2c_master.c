/*
 * This file contains function definitions for I2C Master operations.
 */

#include "maindefs.h"
#include <i2c.h>
#include "my_i2c.h"
#include "messages.h"
#include "my_gpio.h"
#include <string.h> // for memcpy

extern i2c_comm *ic_ptr;

#ifdef I2C_MASTER

/**
 * Handles an error in the I2C master driver.
 */
void i2c_master_handle_error(void);

void i2c_configure_master() {
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

i2c_error_code i2c_master_busy() {
    if (ic_ptr->state != I2C_IDLE) {
        return I2C_ERR_BUSY;
    } else {
        return I2C_ERR_NONE;
    }
}

i2c_error_code i2c_master_write(unsigned char slave_addr, unsigned char const * const data, unsigned char data_length) {
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
    i2c_error_code ret_code = I2C_ERR_NONE;

    // Check if the driver is busy
    if (ic_ptr->state != I2C_IDLE) {
        ret_code = I2C_ERR_BUSY;

    }// Check the requested data length against the maximum length
    else if (data_length > MAXI2CBUF) {
        ret_code = I2C_ERR_MSGTOOLONG;

    }// Check that there is some data requested
    else if (0 == data_length) {
        ret_code = I2C_ERR_ZERO_DATA;

    }// Otherwise proceed
    else {
        // Set the main state to indicate a read in progress
        ic_ptr->state = I2C_READ;

        // Save the requested data length
        ic_ptr->buffer_length = data_length;

        // Reset the buffer index
        ic_ptr->buffer_index = 0;

        // Save the slave address
        ic_ptr->slave_addr = slave_addr;

        // Save the register
        ic_ptr->register_byte = reg;

        // Assert a Start condition and move to the next substate
        SSP1CON2bits.SEN = 1;
        ic_ptr->substate = I2C_SUBSTATE_START_SENT;

    }

    return ret_code;
}

void i2c_master_handler() {
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
                    unsigned char first_byte;

                    // The first byte differs for reads and writes
                    if (I2C_WRITE == ic_ptr->state) {
                        first_byte = ic_ptr->buffer[ic_ptr->buffer_index];
                    } else if (I2C_READ == ic_ptr->state) {
                        first_byte = ic_ptr->register_byte;
                    }

                    // Send the first byte
                    SSPBUF = first_byte;

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
                    // The path differs for reads and writes
                    if (I2C_WRITE == ic_ptr->state) {
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
                    }
                    else if (I2C_READ == ic_ptr->state) {
                        // Assert a Restart condition
                        SSPCON2bits.RSEN = 1;

                        // Move to the next substate
                        ic_ptr->substate = I2C_SUBSTATE_RESTART_SENT;
                    }

                } // End ACK check - else

                break;
            } // End case I2C_SUBSTATE_DATA_SENT

            case I2C_SUBSTATE_ERROR:
                // Both substates indicate a stop has completed
            case I2C_SUBSTATE_STOP_SENT:
            {
                // The Stop has completed, return to idle and send a message to
                // main as appropriate

                // Send a success message to main only if this point was reached
                // through normal operation.
                if (ic_ptr->substate != I2C_SUBSTATE_ERROR) {
                    // Send a success message to main
                    if (I2C_WRITE == ic_ptr->state) {
                        ToMainHigh_sendmsg(0, MSGT_I2C_MASTER_SEND_COMPLETE, NULL);
                    } else if (I2C_READ == ic_ptr->state) {
                        ToMainHigh_sendmsg(ic_ptr->buffer_index, MSGT_I2C_MASTER_RECV_COMPLETE, ic_ptr->buffer);
                    }

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

            case I2C_SUBSTATE_RESTART_SENT:
            {
                // The restart has completed, send the address and R bit (1)
                SSPBUF = (ic_ptr->slave_addr << 1) | 0x01;

                // Move to the next substate
                ic_ptr->substate = I2C_SUBSTATE_ADDR_R_SENT;

                break;
            } // End case I2C_SUBSTATE_RESTART_SENT

            case I2C_SUBSTATE_ADDR_R_SENT:
            {
                // Sending of the address has completed, check the ACK status
                // and prepare to receive data.

#ifndef I2C_MASTER_IGNORE_NACK
                // Check for a NACK (ACKSTAT 0 indicates ACK received)
                if (1 == SSPCON2bits.ACKSTAT) {
                    // NACK probably means there is no slave with that address
                    i2c_master_handle_error();
                } else
#endif
                {
                    // Prepare to receive a byte
                    SSPCON2bits.RCEN = 1;

                    // Move to the next substate
                    ic_ptr->substate = I2C_SUBSTATE_WAITING_TO_RECEIVE;
                }

                break;
            } // End cases I2C_SUBSTATE_ADDR_R_SENT and I2C_SUBSTATE_ACK_SENT

            case I2C_SUBSTATE_WAITING_TO_RECEIVE:
            {
                // A byte has been received, ACK or NACK it as appropriate.

                // Save the byte
                ic_ptr->buffer[ic_ptr->buffer_index] = SSPBUF;

                // Increment the index (received byte counter)
                ic_ptr->buffer_index++;

                // Check if all requested bytes have been received
                if (ic_ptr->buffer_index < ic_ptr->buffer_length) {
                    // Prepare to ACK the byte
                    SSPCON2bits.ACKDT = 0;

                    // Move to the next substate
                    ic_ptr->substate = I2C_SUBSTATE_ACK_SENT;

                }// Otherwise no more data is needed from the slave
                else {
                    // Prepare to NACK the byte
                    SSPCON2bits.ACKDT = 1;

                    // Move to the next substate
                    ic_ptr->substate = I2C_SUBSTATE_NACK_SENT;
                }

                // Send the ACK/NACK as configured above
                SSPCON2bits.ACKEN = 1;

                break;
            } // End case I2C_SUBSTATE_WAITING_TO_RECEIVE

            case I2C_SUBSTATE_ACK_SENT:
            {
                // Sending of the ACK has completed, prepare to receive a byte
                SSPCON2bits.RCEN = 1;

                // Move to the next substate
                ic_ptr->substate = I2C_SUBSTATE_WAITING_TO_RECEIVE;

                break;
            } // End case I2C_SUBSTATE_ACK_SENT

            case I2C_SUBSTATE_NACK_SENT:
            {
                // Sending of the NACK has completed, assert a Stop to complete
                // the read.
                SSPCON2bits.PEN = 1;

                // Move to the next substate
                ic_ptr->substate = I2C_SUBSTATE_STOP_SENT;

                break;
            } // End case I2C_SUBSTATE_NACK_SENT

            default:
            {
                i2c_master_handle_error();
                break;
            } // End default case
        }
    } else {
        // The only time we should get an interrupt while in IDLE is if a Stop
        // was asserted after an error.  If that's not what heppened, indicate
        // an error
        if (I2C_SUBSTATE_ERROR != ic_ptr->substate) {
            SET_I2C_ERROR_PIN();
        }

        ic_ptr->state = I2C_IDLE;
        ic_ptr->substate = I2C_SUBSTATE_IDLE;
    }
}

void i2c_master_handle_error() {
    // Assert a Stop condition to reset the bus
    SSPCON2bits.PEN = 1;

    // Move to the error substate
    ic_ptr->substate = I2C_SUBSTATE_ERROR;
}

#endif //ifdef I2C_MASTER
