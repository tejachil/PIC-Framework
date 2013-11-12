#include "maindefs.h"
#include "my_i2c2.h"

i2c2_comm *ic2_ptr;

/**
 * I2C2 Master mode interrupt handler.  Defined in i2c2_master.c.
 */
extern void i2c2_master_handler(void);

// set up the data structures for this i2c code
// should be called once before any i2c routines are called

void init_i2c2(i2c2_comm *ic) {
    ic2_ptr = ic;
    ic2_ptr->state = I2C_MASTER_IDLE;
}

void i2c2_int_handler() {
#ifdef I2C2_MASTER
    i2c2_master_handler();
#endif
}