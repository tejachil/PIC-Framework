#include "maindefs.h"
#ifndef __XC8
#include <usart.h>
#else
#include <plib/usart.h>
#endif
#include "my_uart.h"

static uart_comm *uc_ptr;

// Private function prototypes
void uart_init_rx(uart_comm *);
void uart_init_tx(void);

void uart_init(uart_comm * uc) {
    // initialize uart recv handling code
    uart_init_rx(uc);

    // initialize uart tx
    uart_init_tx();
}

void uart_rx_int_handler() {
#ifdef __USE18F26J50
    if (DataRdy1USART()) {
        uc_ptr->rx_buffer[uc_ptr->rx_count] = Read1USART();
#else
    if (DataRdyUSART()) {
        uc_ptr->rx_buffer[uc_ptr->rx_count] = ReadUSART();
#endif

        uc_ptr->rx_count++;
        // check if a message should be sent
        if (uc_ptr->rx_count == UART_MAX_RX_BUF) {
            ToMainLow_sendmsg(uc_ptr->rx_count, MSGT_UART_DATA, (void *) uc_ptr->rx_buffer);
            uc_ptr->rx_count = 0;
        }
    }
#ifdef __USE18F26J50
    if (USART1_Status.OVERRUN_ERROR == 1) {
#else
    if (USART_Status.OVERRUN_ERROR == 1) {
#endif
        // we've overrun the USART and must reset
        // send an error message for this
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        ToMainLow_sendmsg(0, MSGT_OVERRUN, (void *) 0);
    }
}

void uart_tx_int_handler() {
    // Disable UART Tx interrupt so it doesn't trigger repeatedly
    PIE1bits.TXIE = 0;
}

void uart_init_rx(uart_comm *uc) {
    uc_ptr = uc;
    uc_ptr->rx_count = 0;
}

void uart_init_tx() {
    // With a system clock of 12 MHz and the following formula for baud rate
    // generation (high-speed):
#ifdef __USE18F26J50
    // Fosc / (4 * (spbrg + 1))
    // Using sppbrg = 155 proveds the closest approximation for 19200:
    // 12,000,000 / (4 * (155 + 1)) = 19230.76923077
    Open1USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT &
            USART_CONT_RX & USART_BRGH_HIGH, 155);
#else
    // Using spbrg = 38 provides the closest approximation for 19200:
    // 12,000,000 / (16 * (38 + 1)) = 19230.76923077
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT &
            USART_CONT_RX & USART_BRGH_HIGH, 38);
#endif
}