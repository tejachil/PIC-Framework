#ifndef __my_uart_h
#define __my_uart_h

#include "public_messages.h"

// This driver does not work on PICs with 2 UARTs
#ifdef __USE18F26J50
#error "UART driver not completely implemented for 2-UART devices"
#endif

/** UART baud rate. */
#ifndef USE_LCD
#define UART_BAUD_RATE (19200)
#else
#define UART_BAUD_RATE (9600)
#endif

/**
 * Timeout frequency.  If UART is receiving a message and does not receive a
 * byte within this timeout, the receive driver will reset and prepare to
 * receive a new message. <p>
 * The baud rate is divided by 10 (number of UART bits per data byte) to get the
 * data byte frequency.  This frequency is again divided by 10 to allow 10 times
 * the data byte frequency to elapse before timing out.
 */
#define UART_RX_TIMEOUT_FREQ (UART_BAUD_RATE / 100)

/**
 * Check if the UART Tx interrupt is enabled.  Use this as a boolean check.
 * @author amosolgo
 */
#define UART_TX_INT_ENABLED() (PIE1bits.TXIE)
/**
 * Enables the Tx interrupt.  The interrupt will fire as soon as the peripheral
 * is ready to transmit a byte.
 * @author amosolgo
 */
#define UART_ENABLE_TX_INT() (PIE1bits.TXIE = 1)
/**
 * Disables the Tx interrupt.
 * @author amosolgo
 */
#define UART_DISABLE_TX_INT() (PIE1bits.TXIE = 0)

// Number of bytes received over UART before a MSGT_UART_DATA message is sent
#define UART_MAX_RX_BUF (PUB_MSG_MAX_SIZE)
// Maximum number of bytes which may be sent over UART at one time
#define UART_MAX_TX_BUF (PUB_MSG_MAX_SIZE)

typedef struct __uart_comm {
    /** Structure to hold received message. */
    public_message_t rx_message;
    /** Number of byte received. */
    unsigned char rx_count;

    /** Buffer to hold bytes to be transmitted */
    unsigned char tx_buffer[UART_MAX_TX_BUF];
    /** Number of bytes remaining to be transmitted */
    unsigned int tx_count;
    /** Index of current byte being transmitted */
    unsigned int tx_cur_byte;
} uart_comm;

/**
 * Error codes which may be returned by UART driver functions.
 */
typedef enum {
    // Returned on success
    UART_ERR_NONE = 0,
    // Data size exceeds maximum allowed size
    UART_ERR_DATA_SIZE,
    // The requested component (Rx or Tx) is busy
    UART_ERR_BUSY
} UART_error_code;

/**
 * Initializes the UART driver for transmitting and receiving.
 * @author amosolgo
 * @param uc The UART driver data structure to be used internally.
 */
void uart_init(uart_comm * uc);

/**
 * Send a series of bytes over UART.
 * @author amosolgo
 * @param data Pointer to array of bytes to send.  The data is copied to an
 *              internal buffer.
 * @param count Number of bytes to send (size of data array).
 * @returns UART_error_code indicating success or cause of failure.
 */
UART_error_code uart_send_bytes(unsigned char const * const data, unsigned int const count);

/**
 * Check if the transmitter is busy.  Returns UART_ERR_BUSY if there are bytes
 * queued for transmission.  Because UART_ERR_BUSY is defined as 0, this can
 * be used in a conditional check, i.e. if(uart_tx_busy()).
 * @author amosolgo
 * @return UART_ERR_BUSY or UART_ERR_NONE
 */
UART_error_code uart_tx_busy(void);

void uart_rx_int_handler(void);
void uart_tx_int_handler(void);

#endif
