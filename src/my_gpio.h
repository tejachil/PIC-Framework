/*
 * GPIO driver definition.
 * Allows initialization of PORTB as output and allows writing a byte to the
 * PORTB pins.
 */
#ifndef MY_GPIO_H
#define	MY_GPIO_H

#ifdef MASTER_PIC
/** Pin toggled on UART Rx timeout. */
#define TOGGLE_UART_RX_TIMEOUT_PIN() (LATBbits.LATB0 ^= 1)
/** Pin toggled when the UART Rx timeout is stoppped. */
#define TOGGLE_UART_RX_TIMEOUT_STOP_PIN() (LATBbits.LATB1 ^= 1)
/** Pin toggled when the UART Rx timeout is started. */
#define TOGGLE_UART_RX_TIMEOUT_START_PIN() (LATBbits.LATB2 ^= 1)
/** Pin set when an error occurs with the UART queue. */
#define SET_UART_QUEUE_ERROR_PIN() (LATBbits.LATB5 = 1);
/** Pin set when an invalid message type is received on UART. */
#define SET_UART_MESSAGE_ERROR_PIN() (LATBbits.LATB6 = 1);
/** Pin set when an I2C error occurs. */
#define SET_I2C_ERROR_PIN() (LATBbits.LATB7 = 1)
#else
// Define blank macros so they will compile but do nothing
#define TOGGLE_UART_RX_TIMEOUT_PIN()
#define TOGGLE_UART_RX_TIMEOUT_STOP_PIN()
#define TOGGLE_UART_RX_TIMEOUT_START_PIN()
#define SET_UART_QUEUE_ERROR_PIN()
#define SET_UART_MESSAGE_ERROR_PIN()
#define SET_I2C_ERROR_PIN()
#endif // MASTER_PIC - else

void gpio_init_portb_output(void);
void gpio_write_portb(const unsigned char val);

#endif	/* MY_GPIO_H */

