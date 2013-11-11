#include "maindefs.h"
#include <usart.h>
#include "my_uart.h"
#include "messages.h"
#include "my_gpio.h"
#include <timers.h>
#include <string.h> // for memcpy

/**
 * Timeout timer count register value.  Calculation is based on the
 * following:<br>
 * Timer1 clock is Fosc / 4: 12 MHz / 4 = 3 MHz
 * Prescaler of 2: 3 MHz / 2 = 1.5 MHz
 * 1.5 MHz / (Timeout freq) = N counts needed for timeout period
 * (2^16 - 1) - N = I, the initial count value
 */
#define UART_RX_TIMEOUT_INITAL_COUNT (65535 - (1500000 / UART_RX_TIMEOUT_FREQ))

static uart_comm *uc_ptr;

// Private function prototypes
/**
 * Initializes the UART peripheral for transmitting and receiving.
 * @author amosolgo
 */
static void uart_init_tx(void);
/** Sends the next byte from the tx_buffer. */
static void uart_send_next_byte(void);
/** 
 * Initializes the UART Rx timeout timer.  The timer is started by this function.
 */
static void uart_timeout_init(void);
/** Restarts the UART Rx timeout. */
static void uart_timeout_restart(void);
/** Stops the UART Rx timeout. */
static void uart_timeout_stop(void);
/** Resets the UART Rx driver to prepare for a new message. */
static void uart_rx_reset(void);

void uart_init(uart_comm * uc) {
    // initialize uart tx
    uart_init_tx();

    // Initialize UART data struct
    uc_ptr = uc;
    memset(uc_ptr, 0, sizeof(uart_comm));

    // Setup timeout timer
    uart_timeout_init();
    // Init will start the timer, stop it now
    uart_timeout_stop();
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

        // If a complete message has been received, send it to main and reset
        if ((uc_ptr->rx_message.data_length + PUB_MSG_MIN_SIZE) == uc_ptr->rx_count) {
            ToMainLow_sendmsg(uc_ptr->rx_count, MSGT_UART_DATA, (void *) uc_ptr->rx_message.raw_message_bytes);
            uart_rx_reset();
        }// Otherwise, restart the timeout in case the next byte doesn't come
        else {
            uart_timeout_restart();

            // If this byte reached or exceeded the buffer size without
            // completing a message, the receiver must be reset.
            if (uc_ptr->rx_count >= PUB_MSG_MAX_SIZE) {
                TOGGLE_UART_RX_OVERFLOW_PIN();
                uart_rx_reset();
            }
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

void uart_rx_reset() {
    uc_ptr->rx_count = 0;
    uart_timeout_stop();
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
    unsigned baud_value;
    // With a system clock of 12 MHz and the following formula for baud rate
    // generation (high-speed):
#if (19200 == UART_BAUD_RATE)
    // Using spbrg = 38 provides the closest approximation for 19200:
    // 12,000,000 / (16 * (38 + 1)) = 19230.76923077
    baud_value = 38;
#elif (9600 == UART_BAUD_RATE)
    // Using spbrg = 77 provides the closest approximation for 9600:
    // 12,000,000 / (16 * (77 + 1)) = 9615.38
    baud_value = 77;
#else
#error "UART driver not configured for selected baud rate"
#endif // UART_BAUD_RATE check
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT &
            USART_CONT_RX & USART_BRGH_HIGH, baud_value);
}

void uart_timeout_init() {
    // Set Timer1 interrupt priority
    IPR1bits.TMR1IP = 0;
    // Setup Timer1 for Rx timeout period
    // DO NOT CHANGE TIMER PARAMETERS WITHOUT CORRECTING THE INITIAL COUNT
    // CALCULATION
    WriteTimer1(0);
    OpenTimer1(TIMER_INT_ON & T1_PS_1_2 & T1_16BIT_RW & T1_SOURCE_INT & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF);
}

void uart_timeout_restart() {
    // Toggle the IO pin
    TOGGLE_UART_RX_TIMEOUT_START_PIN();
    // Write the timeout initial count to the timer
    WriteTimer1(UART_RX_TIMEOUT_INITAL_COUNT);
    // Enable the timer
    T1CONbits.TMR1ON = 1;
}

void uart_timeout_stop() {
    // Toggle the IO pin
    TOGGLE_UART_RX_TIMEOUT_STOP_PIN();
    // Disable the timer
    T1CONbits.TMR1ON = 0;
}

void uart_timeout_triggered() {
    // Toggle the IO pin
    TOGGLE_UART_RX_TIMEOUT_PIN();
    // Reset the Rx driver
    uart_rx_reset();
}