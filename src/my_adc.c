/*
 * ADC Driver implementation
 */
#ifndef __XC8
#include <adc.h>
#else
#include <plib/adc.h>
#endif
#include "maindefs.h"
#include "my_adc.h"
#include "my_gpio.h"

// Most recent ADC conversion value
static int adc_val;

/*
 * ADC initializer.  Sets up ADC channel 0 for conversions with an interrupt.
 */
void adc_init() {
    // Configure ADC for a read on channel 0
    OpenADC(ADC_FOSC_16 & ADC_RIGHT_JUST & ADC_2_TAD, ADC_CH0 & ADC_INT_ON & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0b1110);
    SetChanADC(ADC_CH0);
    // Initialize adc_val to 0
    adc_val = 0;
}

/*
 * Start ADC conversions.  Must be called after interrupts are enabled so that
 * the conversion complete interrupt will be properly handled.
 */
void adc_start() {
    ConvertADC();
}

/*
 * ADC thread method.  Writes the lower byte of the ADC value to PORTB
 */
void adc_lthread(int msgtype, int length, unsigned char *msgbuffer) {
    switch (msgtype) {
        case MSGT_ADC:
        {
            // Message must contain only the ADC value, 16 bits
            if (2 == length) {
                // Save the ADC value
                unsigned char adc_val_high = msgbuffer[0];
                unsigned char adc_val_low = msgbuffer[1];
                adc_val = adc_val_low + (((int) adc_val_high) << 8);
                // Start a new conversion
                ConvertADC();
            } else {
                // This is an error - incorrect message length
#warning "Unhandled error condition"
            }

            break;
        }
        default:
        {
            // Any other message type will simply be ignored
            break;
        }
    }
}

/*
 * Retrieve most recent ADC value.
 */
int adc_read() {
    return adc_val;
}