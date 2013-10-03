#include "maindefs.h"
#ifndef __XC8
#include <i2c.h>
#else
#include <plib/i2c.h>
#endif
#include "my_i2c.h"
#include <string.h> // for memcpy

#ifndef __USE18F45J10
#error "I2C library not implemented for this device"
#endif

static i2c_comm *ic_ptr;

/**
 * I2C Slave mode interrupt handler.
 */
void i2c_slave_handler(void);
/**
 * I2C Master mode interrupt handler.
 */
void i2c_master_handler(void);
/**
 * Handles an error in the I2C master driver.  Currently a placeholder.
 */
void i2c_master_handle_error(void);

// set up the data structures for this i2c code
// should be called once before any i2c routines are called

void init_i2c(i2c_comm *ic) {
    ic_ptr = ic;
    ic_ptr->state = I2C_IDLE;
#ifdef I2C_SLAVE
    ic_ptr->buflen = 0;
    ic_ptr->event_count = 0;
    ic_ptr->error_count = 0;
#endif
}

void i2c_int_handler() {
#if defined(I2C_SLAVE)
    i2c_slave_handler();
#elif defined(I2C_MASTER)
    i2c_master_handler();
#endif
}

// PLACE ALL MASTER FUCTIONS INSIDE THIS IFDEF
#ifdef I2C_MASTER

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
        memcpy(ic_ptr->outbuffer, data, data_length);

        // Save the length of the copied data
        ic_ptr->outbuflen = data_length;

        // Reset the buffer index
        ic_ptr->outbufind = 0;

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
                    SSPBUF = ic_ptr->outbuffer[ic_ptr->outbufind];

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
                    ic_ptr->outbufind++;

                    // Check if there is more data to send
                    if (ic_ptr->outbufind < ic_ptr->outbuflen) {
                        // Send the next byte
                        SSPBUF = ic_ptr->outbuffer[ic_ptr->outbufind];

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

// PLACE ALL SLAVE FUCTIONS INSIDE THIS IFDEF
#ifdef I2C_SLAVE

void start_i2c_slave_reply(unsigned char length, unsigned char *msg) {

    for (ic_ptr->outbuflen = 0; ic_ptr->outbuflen < length; ic_ptr->outbuflen++) {
        ic_ptr->outbuffer[ic_ptr->outbuflen] = msg[ic_ptr->outbuflen];
    }
    ic_ptr->outbuflen = length;
    ic_ptr->outbufind = 1; // point to the second byte to be sent

    // put the first byte into the I2C peripheral
    SSPBUF = ic_ptr->outbuffer[0];
    // we must be ready to go at this point, because we'll be releasing the I2C
    // peripheral which will soon trigger an interrupt
    SSPCON1bits.CKP = 1;

}

// an internal subroutine used in the slave version of the i2c_int_handler

void handle_start(unsigned char data_read) {
    ic_ptr->event_count = 1;
    ic_ptr->buflen = 0;
    // check to see if we also got the address
    if (data_read) {
        if (SSPSTATbits.D_A == 1) {
            // this is bad because we got data and
            // we wanted an address
            ic_ptr->state = I2C_IDLE;
            ic_ptr->error_count++;
            ic_ptr->error_code = I2C_ERR_NOADDR;
        } else {
            if (SSPSTATbits.R_W == 1) {
                ic_ptr->state = I2C_SLAVE_SEND;
            } else {
                ic_ptr->state = I2C_RCV_DATA;
            }
        }
    } else {
        ic_ptr->state = I2C_STARTED;
    }
}

// setup the PIC to operate as a slave
// the address must include the R/W bit

void i2c_configure_slave(unsigned char addr) {

    // ensure the two lines are set for input (we are a slave)
#ifdef __USE18F26J50
    PORTBbits.SCL1 = 1;
    PORTBbits.SDA1 = 1;
#else
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
#endif

    // set the address
    SSPADD = addr;
    //OpenI2C(SLAVE_7,SLEW_OFF); // replaced w/ code below
    SSPSTAT = 0x0;
    SSPCON1 = 0x0;
    SSPCON2 = 0x0;
    SSPCON1 |= 0x0E; // enable Slave 7-bit w/ start/stop interrupts
    SSPSTAT |= SLEW_OFF;
#ifdef I2C_V3
    I2C1_SCL = 1;
    I2C1_SDA = 1;
#else
#ifdef I2C_V1
    I2C_SCL = 1;
    I2C_SDA = 1;
#else
#ifdef __USE18F26J50
    PORTBbits.SCL1 = 1;
    PORTBbits.SDA1 = 1;
#else
    __dummyXY = 35; // Something is messed up with the #ifdefs; this line is designed to invoke a compiler error
#endif
#endif
#endif
    // enable clock-stretching
    SSPCON2bits.SEN = 1;
    SSPCON1 |= SSPENB;
    // end of i2c configure
}

void i2c_slave_handler() {
    unsigned char i2c_data;
    unsigned char data_read = 0;
    unsigned char data_written = 0;
    unsigned char msg_ready = 0;
    unsigned char msg_to_send = 0;
    unsigned char overrun_error = 0;
    unsigned char error_buf[3];

    // clear SSPOV
    if (SSPCON1bits.SSPOV == 1) {
        SSPCON1bits.SSPOV = 0;
        // we failed to read the buffer in time, so we know we
        // can't properly receive this message, just put us in the
        // a state where we are looking for a new message
        ic_ptr->state = I2C_IDLE;
        overrun_error = 1;
        ic_ptr->error_count++;
        ic_ptr->error_code = I2C_ERR_OVERRUN;
    }
    // read something if it is there
    if (SSPSTATbits.BF == 1) {
        i2c_data = SSPBUF;
        data_read = 1;
    }

    if (!overrun_error) {
        switch (ic_ptr->state) {
            case I2C_IDLE:
            {
                // ignore anything except a start
                if (SSPSTATbits.S == 1) {
                    handle_start(data_read);
                    // if we see a slave read, then we need to handle it here
                    if (ic_ptr->state == I2C_SLAVE_SEND) {
                        data_read = 0;
                        msg_to_send = 1;
                    }
                }
                break;
            }
            case I2C_STARTED:
            {
                // in this case, we expect either an address or a stop bit
                if (SSPSTATbits.P == 1) {
                    // we need to check to see if we also read an
                    // address (a message of length 0)
                    ic_ptr->event_count++;
                    if (data_read) {
                        if (SSPSTATbits.D_A == 0) {
                            msg_ready = 1;
                        } else {
                            ic_ptr->error_count++;
                            ic_ptr->error_code = I2C_ERR_NODATA;
                        }
                    }
                    ic_ptr->state = I2C_IDLE;
                } else if (data_read) {
                    ic_ptr->event_count++;
                    if (SSPSTATbits.D_A == 0) {
                        if (SSPSTATbits.R_W == 0) { // slave write
                            ic_ptr->state = I2C_RCV_DATA;
                        } else { // slave read
                            ic_ptr->state = I2C_SLAVE_SEND;
                            msg_to_send = 1;
                            // don't let the clock stretching bit be let go
                            data_read = 0;
                        }
                    } else {
                        ic_ptr->error_count++;
                        ic_ptr->state = I2C_IDLE;
                        ic_ptr->error_code = I2C_ERR_NODATA;
                    }
                }
                break;
            }
            case I2C_SLAVE_SEND:
            {
                if (ic_ptr->outbufind < ic_ptr->outbuflen) {
                    SSPBUF = ic_ptr->outbuffer[ic_ptr->outbufind];
                    ic_ptr->outbufind++;
                    data_written = 1;
                } else {
                    // we have nothing left to send
                    ic_ptr->state = I2C_IDLE;
                }
                break;
            }
            case I2C_RCV_DATA:
            {
                // we expect either data or a stop bit or a (if a restart, an addr)
                if (SSPSTATbits.P == 1) {
                    // we need to check to see if we also read data
                    ic_ptr->event_count++;
                    if (data_read) {
                        if (SSPSTATbits.D_A == 1) {
                            ic_ptr->buffer[ic_ptr->buflen] = i2c_data;
                            ic_ptr->buflen++;
                            msg_ready = 1;
                        } else {
                            ic_ptr->error_count++;
                            ic_ptr->error_code = I2C_ERR_NODATA;
                            ic_ptr->state = I2C_IDLE;
                        }
                    } else {
                        msg_ready = 1;
                    }
                    ic_ptr->state = I2C_IDLE;
                } else if (data_read) {
                    ic_ptr->event_count++;
                    if (SSPSTATbits.D_A == 1) {
                        ic_ptr->buffer[ic_ptr->buflen] = i2c_data;
                        ic_ptr->buflen++;
                    } else /* a restart */ {
                        if (SSPSTATbits.R_W == 1) {
                            ic_ptr->state = I2C_SLAVE_SEND;
                            msg_ready = 1;
                            msg_to_send = 1;
                            // don't let the clock stretching bit be let go
                            data_read = 0;
                        } else { /* bad to recv an address again, we aren't ready */
                            ic_ptr->error_count++;
                            ic_ptr->error_code = I2C_ERR_NODATA;
                            ic_ptr->state = I2C_IDLE;
                        }
                    }
                }
                break;
            }
        }
    }

    // release the clock stretching bit (if we should)
    if (data_read || data_written) {
        // release the clock
        if (SSPCON1bits.CKP == 0) {
            SSPCON1bits.CKP = 1;
        }
    }

    // must check if the message is too long, if
    if ((ic_ptr->buflen > MAXI2CBUF - 2) && (!msg_ready)) {
        ic_ptr->state = I2C_IDLE;
        ic_ptr->error_count++;
        ic_ptr->error_code = I2C_ERR_MSGTOOLONG;
    }

    if (msg_ready) {
        ic_ptr->buffer[ic_ptr->buflen] = ic_ptr->event_count;
        ToMainHigh_sendmsg(ic_ptr->buflen + 1, MSGT_I2C_DATA, (void *) ic_ptr->buffer);
        ic_ptr->buflen = 0;
    } else if (ic_ptr->error_count >= I2C_ERR_THRESHOLD) {
        error_buf[0] = ic_ptr->error_count;
        error_buf[1] = ic_ptr->error_code;
        error_buf[2] = ic_ptr->event_count;
        ToMainHigh_sendmsg(sizeof (unsigned char) *3, MSGT_I2C_DBG, (void *) error_buf);
        ic_ptr->error_count = 0;
    }
    if (msg_to_send) {
        // send to the queue to *ask* for the data to be sent out
        ToMainHigh_sendmsg(0, MSGT_I2C_RQST, (void *) ic_ptr->buffer);
        msg_to_send = 0;
    }
}

#endif //ifdef I2C_SLAVE
