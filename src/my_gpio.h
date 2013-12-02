/*
 * GPIO driver definition.
 * Allows initialization of PORTB as output and allows writing a byte to the
 * PORTB pins.
 */
#ifndef MY_GPIO_H
#define	MY_GPIO_H

// Pin definitions for debug output (should probably be unique to each target
// device)
#if defined(MASTER_PIC)
/** Pin toggled on UART Rx timeout. */
#define TOGGLE_UART_RX_TIMEOUT_PIN() (LATBbits.LATB0 ^= 1)
/** Pin toggled when the UART Rx timeout is stoppped. */
#define TOGGLE_UART_RX_TIMEOUT_STOP_PIN() (LATBbits.LATB1 ^= 1)
/** Pin toggled when the UART Rx timeout is started. */
#define TOGGLE_UART_RX_TIMEOUT_START_PIN() (LATBbits.LATB2 ^= 1)
/** Pin toggled when the UART Rx driver resets due to an overflow. */
#define TOGGLE_UART_RX_OVERFLOW_PIN() (LATBbits.LATB3 ^= 1)
/** Pin set when an error occurs with the I2C queue. */
#define SET_I2C_QUEUE_ERROR_PIN() (LATBbits.LATB4 ^= 1)
/** Pin set when an error occurs with the UART queue. */
#define SET_UART_QUEUE_ERROR_PIN() (LATBbits.LATB5 = 1)
/** Pin set when an invalid message type is received on UART. */
#define SET_UART_MESSAGE_ERROR_PIN() (LATBbits.LATB6 = 1)
/** Pin set when an I2C error occurs. */
#define SET_I2C_ERROR_PIN() (LATBbits.LATB7 = 1)
#elif defined(MOTOR_PIC)
/** Pin set when an error occurs on I2C2. */
#define SET_I2C2_ERROR_PIN() (LATBbits.LATB0 = 1)
/** Pin toggled when turn timer is triggered. */
#define TOGGLE_TIMER0_PIN() (LATBbits.LATB1 ^= 1)
/** Ping toggled when the gyro data is averaged. */
#define TOGGLE_GYRO_AVG_PIN() (LATBbits.LATB2 ^= 1)
#endif // MASTER_PIC - else

// If macros aren't defined, define blank macros so they will compile but do
// nothing
#ifndef TOGGLE_UART_RX_TIMEOUT_PIN
#define TOGGLE_UART_RX_TIMEOUT_PIN()
#endif
#ifndef TOGGLE_UART_RX_TIMEOUT_STOP_PIN
#define TOGGLE_UART_RX_TIMEOUT_STOP_PIN()
#endif
#ifndef TOGGLE_UART_RX_TIMEOUT_START_PIN
#define TOGGLE_UART_RX_TIMEOUT_START_PIN()
#endif
#ifndef TOGGLE_UART_RX_OVERFLOW_PIN
#define TOGGLE_UART_RX_OVERFLOW_PIN()
#endif
#ifndef SET_I2C_QUEUE_ERROR_PIN
#define SET_I2C_QUEUE_ERROR_PIN()
#endif
#ifndef SET_UART_QUEUE_ERROR_PIN
#define SET_UART_QUEUE_ERROR_PIN()
#endif
#ifndef SET_UART_MESSAGE_ERROR_PIN
#define SET_UART_MESSAGE_ERROR_PIN()
#endif
#ifndef SET_I2C_ERROR_PIN
#define SET_I2C_ERROR_PIN()
#endif
#ifndef SET_I2C2_ERROR_PIN
#define SET_I2C2_ERROR_PIN()
#endif
#ifndef TOGGLE_TIMER0_PIN
#define TOGGLE_TIMER0_PIN()
#endif
#ifndef TOGGLE_GYRO_AVG_PIN
#define TOGGLE_I2C2_INT_PIN()
#endif

void gpio_init_portb_output(void);
void gpio_write_portb(const unsigned char val);

#endif	/* MY_GPIO_H */

