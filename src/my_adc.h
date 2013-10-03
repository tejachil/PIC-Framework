/*
 * ADC Driver definitions.
 */
#ifndef MY_ADC_H
#define	MY_ADC_H

// ADC data register number for I2C reads
#define ADC_FULL_DATA_REGISTER (0x60) // 2-byte full ADC value
#define ADC_LOW_DATA_REGISTER (0x61) // Low byte of ADC value
#define ADC_HIGH_DATA_REGISTER (0x62) // High byte of ADC value

void adc_init(void);
void adc_start(void);
void adc_int_handler(void);
void adc_lthread(int msgtype, int length, unsigned char *msgbuffer);
int adc_read(int channel);

#endif	/* MY_ADC_H */

