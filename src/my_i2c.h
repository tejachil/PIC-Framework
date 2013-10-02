#ifndef __my_i2c_h
#define __my_i2c_h

#include "messages.h"

// Ensure the device isn't expecting to be both master and slave (or neither)
#if (defined(I2C_MASTER) && defined(I2C_SLAVE))
#error "Cannot be both I2C Master and Slave"
#elif (!defined(I2C_MASTER) && !defined(I2C_SLAVE))
#error "Must select either I2C Master or Slave"
#endif

#define MAXI2CBUF MSGLEN

#ifdef I2C_SLAVE

typedef enum {
    I2C_IDLE = 0x5,
    I2C_STARTED = 0x6,
    I2C_RCV_DATA = 0x7,
    I2C_SLAVE_SEND = 0x8
} i2c_slave_status;

#else // I2C_MASTER

/**
 * High-level states for the I2C Master driver.  These do not fully describe the
 * state of the driver, only which major operation is in progress.
 */
typedef enum {
    /** No activity, ready for a new operation to begin */
    I2C_IDLE,
    /** A write to a slave is in progress (i2c_master_write() has been called)*/
    I2C_WRITE,
    /** A read from a slave is in progress(i2c_master_read() has been called)*/
    I2C_READ
} i2c_master_status;

/**
 * Specific status for the current operation.  Each substate is waiting for an
 * I2C interrupt to proceed to the next substate.
 */
typedef enum {
    /** Substate machine is idle.  Next substate is always START_SENT. */
    I2C_SUBSTATE_IDLE = 0,
    /** Start has been sent. */
    I2C_SUBSTATE_START_SENT,
    /** Restart has been sent. */
    I2C_SUBSTATE_RESTART_SENT,
    /** Slave address and Write bit have been sent. */
    I2C_SUBSTATE_ADDR_W_SENT,
    /** Slave address and Read bit have been sent. */
    I2C_SUBSTATE_ADDR_R_SENT,
    /** A data byte has been sent. */
    I2C_SUBSTATE_DATA_SENT,
    /** I2C has been placed into receive mode and is awaiting a byte. */
    I2C_SUBSTATE_WAITING_TO_RECEIVE,
    /** An ACK has been sent. */
    I2C_SUBSTATE_ACK_SENT,
    /** A NACK has been sent. */
    I2C_SUBSTATE_NACK_SENT,
    /** A STOP has been sent.  Next substate is always IDLE. */
    I2C_SUBSTATE_STOP_SENT
} i2c_master_substatus;
#endif // ifdef I2C_SLAVE - else

typedef struct __i2c_comm {
    unsigned char buffer[MAXI2CBUF];
    unsigned char buflen;
    unsigned char event_count;
#ifdef I2C_SLAVE
    i2c_slave_status status;
#else
    i2c_master_status status;
    i2c_master_substatus substatus;
#endif
    unsigned char error_code;
    unsigned char error_count;
    unsigned char outbuffer[MAXI2CBUF];
    unsigned char outbuflen;
    unsigned char outbufind;
    unsigned char slave_addr;
} i2c_comm;

typedef enum {
    I2C_ERR_NONE = 0,
    I2C_ERR_THRESHOLD,
    I2C_ERR_OVERRUN,
    I2C_ERR_NOADDR,
    I2C_ERR_NODATA,
    I2C_ERR_MSGTOOLONG,
    I2C_ERR_MSG_TRUNC,
    // The bus or the peripheral is busy
    I2C_ERR_BUSY
} i2c_error_code;

void init_i2c(i2c_comm *);
void i2c_int_handler(void);

// Slave functions
void start_i2c_slave_reply(unsigned char, unsigned char *);
void i2c_configure_slave(unsigned char);

// Master functions
/**
 * Set up the I2C peripheral as a master.
 */
void i2c_configure_master(void);

/**
 * Write data to a given slave.  Once the write is complete, a message will be
 * sent in the low-priority queue: <p>
 * MSGT_MASTER_SEND_COMPLETE, length 0: The write was successful.
 * MSGT_MASTER_SEND_FAILED, length 0:  The write failed due to an error such as
 * a NACK from the slave.
 * @param slave_addr The address of the slave.  This should <b>not</b> be
 *          shifted to account for the R/W bit (it will be shifted internally).
 * @param data Pointer to the data to write after the address.  Providing NULL
 *          will indicate no additional data should be sent.
 * @param data_length Number of bytes to send from 'data'.  Providing 0 will
 *          indicate no additional data should be sent.
 * @return i2c_error code indicating error status.  Should only return
 *          I2C_ERR_NONE or I2C_ERR_BUSY.
 */
i2c_error_code i2c_master_write(unsigned char slave_addr, unsigned char *data, unsigned char data_length);

/**
 * Read data from a given slave.  First a single byte is written to provide the
 * slave with information about what data is being requested.  Then the given
 * number of bytes are read from the slave.  Once the read is complete, a
 * message will be sent in the low-priority queue: <p>
 * MSGT_MASTER_RECV_COMPLETE, length varies: The read was successful.  The
 * message will include the data received from the slave.
 * MSGT_MASTER_RECV_FAILED, length 0: The read failed.
 * @param slave_addr The address of the slave.  This should <b>not</b> be
 *          shifted to account for the R/W bit (it will be shifted internally).
 * @param reg The data byte (usually a register address) to write before the
 *          read is started.
 * @param data_length The number of byte to read from the slave.
 * @return i2c_error code indicating error status.  Should only return
 *          I2C_ERR_NONE or I2C_ERR_BUSY.
 */
i2c_error_code i2c_master_read(unsigned char slave_addr, unsigned char reg, unsigned char data_length);

#endif