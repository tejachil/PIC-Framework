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
    I2C_ERR_MSG_TRUNC
} i2c_error_code;

void init_i2c(i2c_comm *);
void i2c_int_handler(void);
void start_i2c_slave_reply(unsigned char, unsigned char *);
void i2c_configure_slave(unsigned char);
void i2c_configure_master(unsigned char);
unsigned char i2c_master_send(unsigned char, unsigned char *);
unsigned char i2c_master_recv(unsigned char);

#endif