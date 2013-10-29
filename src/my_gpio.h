/*
 * GPIO driver definition.
 * Allows initialization of PORTB as output and allows writing a byte to the
 * PORTB pins.
 */
#ifndef MY_GPIO_H
#define	MY_GPIO_H

/** Pin toggled on UART Rx timeout. */
#define UART_RX_TIMEOUT_PIN (LATBbits.LATB0)
/** Pin toggled when the UART Rx timeout is stoppped. */
#define UART_RX_TIMEOUT_STOP_PIN (LATBbits.LATB1)
/** Pin toggled when the UART Rx timeout is started. */
#define UART_RX_TIMEOUT_START_PIN (LATBbits.LATB2)

void gpio_init_portb_output(void);
void gpio_write_portb(const unsigned char val);

#endif	/* MY_GPIO_H */

