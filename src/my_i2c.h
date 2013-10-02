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

typedef enum {
    I2C_IDLE
} i2c_master_status;
#endif // ifdef I2C_SLAVE - else

typedef struct __i2c_comm {
    unsigned char buffer[MAXI2CBUF];
    unsigned char buflen;
    unsigned char event_count;
#ifdef I2C_SLAVE
    i2c_slave_status status;
#else
    i2c_master_status status;
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
void i2c_configure_master();
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
i2c_error_code i2c_master_initiate_read(unsigned char slave_addr, unsigned char reg, unsigned char data_length);

#endif