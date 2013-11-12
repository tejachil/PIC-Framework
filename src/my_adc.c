/*
 * ADC Driver implementation
 */
#include <adc.h>
#include "maindefs.h"
#include "my_adc.h"
#include "my_gpio.h"

// Most recent ADC conversion value
static int adc_val[NUMBER_OF_CHANNELS];
//this variable keeps track of which ADC channel is selected
static int currentChannel;

/*
 * ADC initializer.  Sets up ADC channel 0,1 for conversions with an interrupt.
 */
void adc_init() {
    // counter for loop
    int i = 0;
    // Configure ADC for a read on channel 0,1
    OpenADC(ADC_FOSC_16 & ADC_RIGHT_JUST & ADC_2_TAD, ADC_CH0 & ADC_CH1 & ADC_CH2 & ADC_CH3 & ADC_INT_ON & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0b1110);
    SetChanADC(ADC_CH0);
    currentChannel = 0;
    // Initialize all channels to 0
    for (i = 0; i < NUMBER_OF_CHANNELS; ++i)
        adc_val[i] = 0;
}

/*
 * Start ADC conversions.  Must be called after interrupts are enabled so that
 * the conversion complete interrupt will be properly handled.
 */
void adc_start() {
    ConvertADC();
}

/*
 * ADC thread method.  Saves the latest reading and starts a new conversion.
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
                adc_val[currentChannel] = adc_val_low + (((int) adc_val_high) << 8);

                // prepare ADC for next conversion
                // toggle between channels to read them sequentially
                switch(currentChannel){
                    case 0:
                        SetChanADC(ADC_CH1);
                        currentChannel = 1;
                        break;
                    case 1:
                        SetChanADC(ADC_CH2);
                        currentChannel = 2;
                        break;
                    case 2:
                        SetChanADC(ADC_CH3);
                        currentChannel = 3;
                        break;
                    case 3:
                        SetChanADC(ADC_CH0);
                        currentChannel = 0;
                        break;
                }
                
                // Start a new conversion of the selected channel
                ConvertADC();


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
int adc_read(int channel) {
    if (channel < NUMBER_OF_CHANNELS)
        return adc_val[channel];
    return 0;
}