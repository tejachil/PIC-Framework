#ifndef __my_uart_h
#define __my_uart_h

#include "messages.h"

// Number of bytes received over UART before a MSGT_UART_DATA message is sent
#define UART_MAX_RX_BUF 1
// Maximum number of bytes which may be sent over UART at one time
#define UART_MAX_TX_BUF 10

#if (UART_MAX_RX_BUF > MSGLEN)
#warning "UART Rx buffer larger than message length"
#endif

typedef struct __uart_comm {
    /** Buffer to hold received bytes */
    unsigned char rx_buffer[UART_MAX_RX_BUF];
    /** Number of bytes currently in the rx_buffer */
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
    UART_ERR_DATA_SIZE
} UART_error_code;

/**
 * Initializes the UART driver for transmitting and receiving.
 * @param uc The UART driver data structure to be used internally.
 */
void uart_init(uart_comm * uc);

/**
 * Send a series of bytes over UART.
 * @param data Pointer to array of bytes to send.  The data is copied to an
 *              internal buffer.
 * @param count Number of bytes to send (size of data array).
 * @returns UART_error_code indicating success or cause of failure.
 */
UART_error_code uart_send_bytes(unsigned char const * const data, unsigned int const count);

void uart_rx_int_handler(void);
#ifdef USE_UART_TEST
void uart_tx_int_handler(void);
#endif //ifdef USE_UART_TEST

#endif
