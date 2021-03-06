#ifndef __my_i2c_h
#define __my_i2c_h

#include "maindefs.h"
#include "public_messages.h"

// Ensure the device isn't expecting to be both master and slave (or neither)
#if !(defined(I2C_MASTER) ^ defined(I2C_SLAVE))
#error "Must select either I2C master or slave (not neither or both)"
#endif

#define MAXI2CBUF (PUB_MSG_MAX_SIZE)

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
} i2c_master_state;

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
    I2C_SUBSTATE_STOP_SENT,
    /** An error has occurred and a STOP has been sent to free the bus. */
    I2C_SUBSTATE_ERROR
} i2c_master_substate;
#endif // ifdef I2C_SLAVE - else

typedef struct __i2c_comm {
#ifdef I2C_SLAVE
    unsigned char inbuffer[MAXI2CBUF];
    unsigned char inbuflen;
    unsigned char event_count;
    i2c_slave_status state;
    unsigned char error_code;
    unsigned char error_count;
    /** Buffer used for bytes to be sent on I2C. */
    unsigned char outbuffer[MAXI2CBUF];
    /** Number of bytes buffered in outbuffer. */
    unsigned char outbuflen;
    /** Index of next byte to use from outbuffer. */
    unsigned char outbufind;
#else
    /** Main state for I2C master operations. */
    i2c_master_state state;
    /** Substate for I2C master operations. */
    i2c_master_substate substate;
    /** Buffer used for bytes to be sent or bytes received on I2C */
    unsigned char buffer[MAXI2CBUF];
    /** Number of bytes buffered or to be buffered in 'buffer'. */
    unsigned char buffer_length;
    /** Index of next byte to use in 'buffer'. */
    unsigned char buffer_index;
    /** "Register address" being read from slave. */
    unsigned char register_byte;
#endif
    /**
     * I2C_SLAVE: Slave address for this device.
     * I2C_MASTER: Address of the device currently being communicated with.
     */
    unsigned char slave_addr;
} i2c_comm;

/** Maximum number of errors before the I2C slave driver stops functioning. */
#define I2C_ERR_THRESHOLD (1)

/** Error codes which may be returned by the I2C driver. */
typedef enum {
    /** Indicates no error. */
    I2C_ERR_NONE = 0,
    /** The bus or the peripheral is busy */
    I2C_ERR_BUSY,
    /** The I2C buffer was not read in time, data was lost. */
    I2C_ERR_OVERRUN,
    /** An address was not available when expected. */
    I2C_ERR_NOADDR,
    /** I2C data was not available when expected. */
    I2C_ERR_NODATA,
    /** A provided message or received message is too long. */
    I2C_ERR_MSGTOOLONG,
    /** No data was provided to a function which requires data. */
    I2C_ERR_ZERO_DATA
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
 * sent in the high-priority queue: <p>
 * MSGT_MASTER_SEND_COMPLETE, length 0: The write was successful.
 * MSGT_MASTER_SEND_FAILED, length 0:  The write failed due to an error such as
 * a NACK from the slave.
 * @param slave_addr The address of the slave.  This should <b>not</b> be
 *          shifted to account for the R/W bit (it will be shifted internally).
 * @param data Pointer to the data to write after the address.  Providing NULL
 *          will indicate no additional data should be sent.
 * @param data_length Number of bytes to send from 'data'.  Providing 0 will
 *          indicate no additional data should be sent.
 * @return i2c_error code indicating error status.
 */
i2c_error_code i2c_master_write(unsigned char slave_addr, unsigned char const * const data, unsigned char data_length);

/**
 * Read data from a given slave.  First a single byte is written to provide the
 * slave with information about what data is being requested.  Then the given
 * number of bytes are read from the slave.  Once the read is complete, a
 * message will be sent in the high-priority queue: <p>
 * MSGT_MASTER_RECV_COMPLETE, length varies: The read was successful.  The
 * message will include the data received from the slave.
 * MSGT_MASTER_RECV_FAILED, length 0: The read failed.
 * @param slave_addr The address of the slave.  This should <b>not</b> be
 *          shifted to account for the R/W bit (it will be shifted internally).
 * @param reg The data byte (usually a register address) to write before the
 *          read is started.
 * @param data_length The maximum number of bytes to read from the slave.
 * @return i2c_error code indicating error status.
 */
i2c_error_code i2c_master_read(unsigned char slave_addr, unsigned char reg, unsigned char data_length);

/**
 * Check if the I2C Master driver is busy.  Because I2C_ERR_NONE is zero, this
 * method may be used in a boolean expression [i.e. if(i2c_master_busy())].
 * @return i2c_error_code indicating driver status.  May only return: <br>
 *          <b>I2C_ERR_NONE</b>: Driver is not busy and is ready to begin an
 *              operation.
 *          <b>I2C_ERR_BUSY</b>: Driver is busy, no new operations may begin.
 */
i2c_error_code i2c_master_busy(void);

#endif