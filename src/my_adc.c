/*
 * ADC Driver implementation
 */
#include "maindefs.h"
#include "my_adc.h"
#include "my_gpio.h"

static unsigned char msg_count = 0;

/*
 * ADC thread method.  Writes the lower byte of the ADC value to PORTB
 */
void adc_lthread(int msgtype, int length, unsigned char *msgbuffer) {
    switch (msgtype) {
        case MSGT_ADC:
        {
            // Message must contain only the ADC value, 16 bits
            if (2 == length) {
                if (++msg_count >= 5) {
                    unsigned char adc_val_high = msgbuffer[0];
                    unsigned char adc_val_low = msgbuffer[1];
                    msg_count = 0;
                    // Write the low byte to PORTB
                    gpio_write_portb(adc_val_low);
                }
            }
            break;
        }
    }
}