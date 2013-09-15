/*
 * ADC Driver definitions.
 */
#ifndef MY_ADC_H
#define	MY_ADC_H

// ADC data register number for I2C reads
#define ADC_DATA_REGISTER (0x69)

void adc_init(void);
void adc_int_handler(void);
void adc_lthread(int msgtype, int length, unsigned char *msgbuffer);

#endif	/* MY_ADC_H */

