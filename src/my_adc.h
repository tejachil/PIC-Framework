/*
 * ADC Driver definitions.
 */
#ifndef MY_ADC_H
#define	MY_ADC_H

// Number of ADC channels the driver will sample
#define NUMBER_OF_CHANNELS 6

void adc_init(void);
void adc_start(void);
void adc_int_handler(void);
void adc_lthread(int msgtype, int length, unsigned char *msgbuffer);
int adc_read(int channel);

#endif	/* MY_ADC_H */

