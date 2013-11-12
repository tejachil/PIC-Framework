#ifndef MY_I2C2_H
#define MY_I2C2_H

#include "maindefs.h"
#include "public_messages.h"
#include "my_i2c.h"

typedef struct __i2c2_comm {
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
    /** Address of the device currently being communicated with. */
    unsigned char slave_addr;
} i2c2_comm;

void init_i2c2(i2c2_comm *);
void i2c2_int_handler(void);

// Master functions
/**
 * Set up the I2C peripheral as a master.
 */
void i2c2_configure_master(void);

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
i2c_error_code i2c2_master_write(unsigned char slave_addr, unsigned char const * const data, unsigned char data_length);

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
i2c_error_code i2c2_master_read(unsigned char slave_addr, unsigned char reg, unsigned char data_length);

/**
 * Check if the I2C Master driver is busy.  Because I2C_ERR_NONE is zero, this
 * method may be used in a boolean expression [i.e. if(i2c_master_busy())].
 * @return i2c_error_code indicating driver status.  May only return: <br>
 *          <b>I2C_ERR_NONE</b>: Driver is not busy and is ready to begin an
 *              operation.
 *          <b>I2C_ERR_BUSY</b>: Driver is busy, no new operations may begin.
 */
i2c_error_code i2c2_master_busy(void);

#endif // ifndef MY_I2C2_H