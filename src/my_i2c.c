#include "maindefs.h"
#include "my_i2c.h"

i2c_comm *ic_ptr;

/**
 * I2C Slave mode interrupt handler.  Defined in i2c_slave.c.
 */
extern void i2c_slave_handler(void);
/**
 * I2C Master mode interrupt handler.  Defined in i2c_master.c.
 */
extern void i2c_master_handler(void);

// set up the data structures for this i2c code
// should be called once before any i2c routines are called

void init_i2c(i2c_comm *ic) {
    ic_ptr = ic;
    ic_ptr->state = I2C_IDLE;
#ifdef I2C_SLAVE
    ic_ptr->inbuflen = 0;
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