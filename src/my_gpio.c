#include "my_gpio.h"
#include "adc.h"
#include "my_encoder.h"

/*
 * Initialize PORTB pins as output.
 */
void gpio_init_portb_output() {
    unsigned char trisb = 0x00;
    
    LATB = 0x0; // clear the output latch
    ADCON1 = 0xFF; // turn off the A2D

    // Don't set encoder inputs to output mode for the motor PIC
#ifdef MOTOR_PIC
    trisb |= (ENCODER_1_BIT | ENCODER_2_BIT);
#endif

    TRISB = trisb;
}

/*
 * Write a byte to the PORTB pins.  RB0 will output the MOST significant bit
 * and RB7 will output the LEAST significant bit.
 */
void gpio_write_portb(const unsigned char val) {
    LATB = val;
}