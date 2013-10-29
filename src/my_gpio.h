/*
 * GPIO driver definition.
 * Allows initialization of PORTB as output and allows writing a byte to the
 * PORTB pins.
 */
#ifndef MY_GPIO_H
#define	MY_GPIO_H

#if defined(MASTER_PIC) || defined(MOTOR_PIC)
/** Pin toggled on UART Rx timeout. */
#define TOGGLE_UART_RX_TIMEOUT_PIN() (LATBbits.LATB0 ^= 1)
/** Pin toggled when the UART Rx timeout is stoppped. */
#define TOGGLE_UART_RX_TIMEOUT_STOP_PIN() (LATBbits.LATB1 ^= 1)
/** Pin toggled when the UART Rx timeout is started. */
#define TOGGLE_UART_RX_TIMEOUT_START_PIN() (LATBbits.LATB2 ^= 1)
#endif // MASTER_PIC || MOTOR_PIC

void gpio_init_portb_output(void);
void gpio_write_portb(const unsigned char val);

#endif	/* MY_GPIO_H */

