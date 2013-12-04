/* 
 * File:   my_encoder.h
 * Author: TY
 *
 * Created on October 7, 2013, 8:44 PM
 */

#ifndef MY_ENCODER_H
#define	MY_ENCODER_H

// Bitmasks for the encoder inputs (used to mask a single bit of PORTB)
#define ENCODER_1_BIT (0b00010000)
#define ENCODER_2_BIT (0b00100000)

/**
 * Initializes the encoder driver.  <b>This function changes some PORTB pins to
 * input.</b>
 */
void encoders_init(void);

/**
 * Resets the counters for the encoders.
 */
void encoders_reset(void);

/**
 * Updates encoder counts when PORTB changes.
 * @param portB_new The new (changed) value of PORTB.
 */
void encoders_int_handler(unsigned char portB_new);

/**
 * Gets the values for encoder 1 and stores them at the given locations.
 */
void encoders_get_encoder_1(unsigned char *rotations, unsigned int *ticks);

/**
 * Gets the values for encoder 1 and stores them at the given locations.
 */
void encoders_get_encoder_2(unsigned char *rotations, unsigned int *ticks);

void encoder_lthread(int msgtype, int length, unsigned char *msgbuffer);

#endif	/* MY_ENCODER_H */

