#ifndef __XC8
#include <adc.h>
#else
#include <plib/adc.h>
#endif
#include "maindefs.h"
#include "my_adc.h"
#include "my_gpio.h"
#include "my_encoder.h"
#include "my_motor.h"

// State of PORTB the last time the encoder interrupt was triggered
static unsigned char portB_prev;

// Counters for encoder readings
unsigned char encoder_1_rotations;
unsigned int encoder_1_ticks;
unsigned char encoder_2_rotations;
unsigned int encoder_2_ticks;

void encoders_init() {
    // Set up PORTB change interrupt
    INTCONbits.RBIE = 1; // Enable interrupt
    INTCON2bits.RBIP = 1; // Set interrupt priority high

    // Change encoder input pins to input
    TRISB |= (ENCODER_1_BIT | ENCODER_2_BIT);

    // Save the initial PORTB state
    portB_prev = PORTB;

    // Initialize the encoder counters
    encoders_reset();
}

void encoders_reset() {
    encoder_1_rotations = 0;
    encoder_1_ticks = 0;
    encoder_2_rotations = 0;
    encoder_2_ticks = 0;
}

void encoder_lthread(int msgtype, int length, unsigned char *msgbuffer) {

    switch (msgtype) {
        case MSGT_ENC:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

void encoders_int_handler(unsigned char portB_new) {
    // If we're supposed to be counting encoders, check the new PORTB value
    if (countFlag == 1) {

        // If encoder 1 changed, increment its counter
        if ((portB_new & ENCODER_1_BIT) != (portB_prev & ENCODER_1_BIT)) {
            encoder_1_ticks++;

            // If encoder 1 has rotated one revolution, increment its revolution
            // counter and reset its tick counter
            if (encoder_1_ticks >= ENCODER_TICKS_PER_REVOLUTION) {
                encoder_1_rotations++;
                encoder_1_ticks = 0;
            }
        }

        // If encoder 2 changed, increment its counter
        if ((portB_new & ENCODER_2_BIT) != (portB_prev & ENCODER_2_BIT)) {
            encoder_2_ticks++;

            // If encoder 2 has rotated one revolution, increment its revolution
            // counter and reset its tick counter
            if (encoder_2_ticks >= ENCODER_TICKS_PER_REVOLUTION) {
                encoder_2_rotations++;
                encoder_2_ticks = 0;
            }
        }
    }

    // Reset the previous PORTB value to the new value
    portB_prev = portB_new;
}

void encoders_get_encoder_1(unsigned char *rotations, unsigned int *ticks) {
    *rotations = encoder_1_rotations;
    *ticks = encoder_1_ticks;
}

void encoders_get_encoder_2(unsigned char *rotations, unsigned int *ticks) {
    *rotations = encoder_2_rotations;
    *ticks = encoder_2_ticks;
}