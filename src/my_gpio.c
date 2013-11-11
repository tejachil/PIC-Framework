#include "my_gpio.h"
#include "adc.h"

/*
 * Initialize PORTB pins as output.
 */
void gpio_init_portb_output() {
    LATB = 0x0; // clear the output latch
    ADCON1 = 0x0F; // turn off the A2D
    TRISB = 0x00; // set RB<7:0> to outputs
}

/*
 * Write a byte to the PORTB pins.  RB0 will output the MOST significant bit
 * and RB7 will output the LEAST significant bit.
 */
void gpio_write_portb(const unsigned char val) {
    LATB = val;
}