#ifndef __my_uart_h
#define __my_uart_h

#include "messages.h"

#define MAXUARTBUF 1
#if (MAXUARTBUF > MSGLEN)
#define MAXUARTBUF MSGLEN
#endif
typedef struct __uart_comm {
	unsigned char buffer[MAXUARTBUF];
	unsigned char	buflen;
} uart_comm;

void init_uart_rx(uart_comm *);
void init_uart_tx(void);
void uart_rx_int_handler(void);
#ifdef USE_UART_TEST
void uart_tx_int_handler(void);
#endif //ifdef USE_UART_TEST

#endif
