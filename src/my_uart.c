#include "maindefs.h"
#ifndef __XC8
#include <usart.h>
#else
#include <plib/usart.h>
#endif
#include "my_uart.h"
#include "messages.h"
#include <string.h> // for memcpy

static uart_comm *uc_ptr;

// Private function prototypes
/**
 * Initializes the UART peripheral for transmitting and receiving.
 * @author amosolgo
 */
static void uart_init_tx(void);

void uart_init(uart_comm * uc) {
    // initialize uart tx
    uart_init_tx();

    // Initialize UART data struct
    uc_ptr = uc;
    uc_ptr->rx_count = 0;
    uc_ptr->tx_count = 0;
    uc_ptr->tx_cur_byte = 0;
}

UART_error_code uart_send_bytes(unsigned char const * const data, unsigned int const count) {
    UART_error_code ret_code = UART_ERR_NONE;

    // Make sure there is data to send and that it is within the buffer size
    if ((count > 0) && (count <= UART_MAX_TX_BUF)) {
        // Make sure there is not a transmission in progress
        if (!uart_tx_busy()) {
            // Copy the data into the transmit buffer
            memcpy(uc_ptr->tx_buffer, data, count);
            // Set the number of bytes to be sent
            uc_ptr->tx_count = count;
            // Reset the current byte index
            uc_ptr->tx_cur_byte = 0;
            // Enable the Tx interrupt.  This kicks off the interrupt-driven
            // transmission sequence.
            UART_ENABLE_TX_INT();
        }// If there is a transmission in progress, return an error
        else {
            ret_code = UART_ERR_BUSY;
        }

    }// If there is no data or too much data, return an error
    else {
        ret_code = UART_ERR_DATA_SIZE;
    }

    return ret_code;
}

UART_error_code uart_tx_busy() {
    if (uc_ptr->tx_count != 0) {
        return UART_ERR_BUSY;
    } else {
        return UART_ERR_NONE;
    }
}

void uart_rx_int_handler() {
#ifdef __USE18F26J50
    if (DataRdy1USART()) {
        uc_ptr->rx_buffer[uc_ptr->rx_count] = Read1USART();
#else
    if (DataRdyUSART()) {
        const unsigned char rx_byte = ReadUSART();
#endif

        // Place the byte in the appropriate message field
        if (uc_ptr->rx_count == PUB_MSG_FIELD_OFFSET(message_type)) {
            uc_ptr->rx_message.message_type = rx_byte;
        } else if (uc_ptr->rx_count == PUB_MSG_FIELD_OFFSET(message_count)) {
            uc_ptr->rx_message.message_count = rx_byte;
        } else if (uc_ptr->rx_count == PUB_MSG_FIELD_OFFSET(data_length)) {
            uc_ptr->rx_message.data_length = rx_byte;
        } else {
            uc_ptr->rx_message.data[uc_ptr->rx_count - PUB_MSG_MIN_SIZE] = rx_byte;
        }

        // Increment received byte counter
        uc_ptr->rx_count++;

        // Check if a complete message has been received
        if ((uc_ptr->rx_message.data_length + PUB_MSG_MIN_SIZE) == uc_ptr->rx_count) {
            ToMainLow_sendmsg(uc_ptr->rx_count, MSGT_UART_DATA, (void *) uc_ptr->rx_message.raw_message_bytes);
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
    // Start the next byte
    uart_send_next_byte();
}

void uart_send_next_byte() {
    // Make sure there is a byte to send (otherwise do nothing)
    if (uc_ptr->tx_count > 0) {
        // Get the next byte to be sent
        const unsigned char next_byte = uc_ptr->tx_buffer[uc_ptr->tx_cur_byte];
        // Send it over UART
        WriteUSART(next_byte);
        // Reenable the Tx interrupt so that it will fire when the byte has
        // been sent
        UART_ENABLE_TX_INT();

        // Move the byte index to the next byte
        uc_ptr->tx_cur_byte++;
        // Decrement the number of bytes remaining
        uc_ptr->tx_count--;
    }
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
    // Using spbrg = 77 provides the closest approximation for 9600:
    // 12,000,000 / (16 * (77 + 1)) = 9615.38
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT &
            USART_CONT_RX & USART_BRGH_HIGH, 77);
#endif
}