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
#include "public_messages.h"

// State of PORTB the last time the encoder interrupt was triggered
static unsigned char portB_prev;

// Counters for encoder readings
unsigned char encoder_1_rotations;
unsigned int encoder_1_ticks;
unsigned char encoder_2_rotations;
unsigned int encoder_2_ticks;

// Target counts for moving forward a specified distance
static unsigned char encoder_target_rotations = 0;
static unsigned int encoder_target_ticks = 0;
// Flag set when waiting to reach above target counts
static unsigned char encoder_waiting_for_target = 0;

/** If encoder target is set, stops motors if the target has been reached. */
static void encoders_check_target(void);

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

void encoders_set_stop_target(unsigned char target_rotations, unsigned int target_ticks) {
    // Set the target counts
    encoder_target_rotations = target_rotations;
    encoder_target_ticks = target_ticks;
    // Set the flag to stop at the targets set above
    encoder_waiting_for_target = 1;
}

unsigned char encoders_reached_target() {
    // Return the inverse of the internal flag because the internal flag is
    // cleared when complete rather than set when complete
    if (encoder_waiting_for_target !=0 ) {
        // waiting_for_target is set, haven't reached the target yet
        return 0;
    } else {
        // waiting_for_target is cleared, target reached
        return 1;
    }
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

        // Check the stop target and stop if needed
        encoders_check_target();
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

void encoders_check_target() {
    // Only check targets if the flag is set
    if (encoder_waiting_for_target != 0) {

        // Check if the rotations are at or past the target
        if (encoder_1_rotations >= encoder_target_rotations) {

            // Check if the ticks are also at or past the target
            if (encoder_1_ticks >= encoder_target_ticks) {

                // Stop the motors and clear the flag
                motor_stop_both();
                encoder_waiting_for_target = 0;
            }
        }
    }
}