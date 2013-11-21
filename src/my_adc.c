/*
 * ADC Driver implementation
 */
#include <adc.h>
#include "maindefs.h"
#include "my_adc.h"
#include "my_gpio.h"

// Array to hold ADC channel names, indexed by channel number
static const unsigned char ADC_CHANNEL[16] = {
    ADC_CH0, ADC_CH1, ADC_CH2, ADC_CH3, ADC_CH4, ADC_CH5, ADC_CH6, ADC_CH7,
    ADC_CH8, ADC_CH9, ADC_CH10, ADC_CH11, ADC_CH12, ADC_CH13, ADC_CH14, ADC_CH15
};

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
    // Configure ADC for a read on all needed channels
    unsigned char channel_selection = ADC_CHANNEL[0];
    for (i = 0; i < NUMBER_OF_CHANNELS; i++) {
        channel_selection &= ADC_CHANNEL[i];
    }
    OpenADC(ADC_FOSC_16 & ADC_RIGHT_JUST & ADC_2_TAD, channel_selection & ADC_INT_ON & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0b1110);

    // Start on channel 0
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
                currentChannel = (currentChannel + 1) % NUMBER_OF_CHANNELS;
                SetChanADC(ADC_CHANNEL[currentChannel]);

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