#ifndef __my_uart_h
#define __my_uart_h

#include "messages.h"

#define UART_MAX_RX_BUF 1

#if (UART_MAX_RX_BUF > MSGLEN)
#warning "UART Rx buffer larger than message length"
#endif

typedef struct __uart_comm {
    // Buffer to hold received bytes
    unsigned char rx_buffer[UART_MAX_RX_BUF];
    // Number of bytes currently in the rx_buffer
    unsigned char rx_count;
} uart_comm;

void init_uart(uart_comm *);
void uart_rx_int_handler(void);
#ifdef USE_UART_TEST
void uart_tx_int_handler(void);
#endif //ifdef USE_UART_TEST

#endif
