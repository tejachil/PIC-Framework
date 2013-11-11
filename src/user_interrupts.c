// This is where the "user" interrupts handlers should go
// The *must* be declared in "user_interrupts.h"

#include "maindefs.h"
#ifndef __XC8
#include <timers.h>
#include "adc.h"
#else
#include <plib/timers.h>
#include <plib/adc.h>
#endif
#include "user_interrupts.h"
#include "messages.h"
#include "my_encoder.h"
#include "my_motor.h"

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer0 interrupt

void timer0_int_handler() {
}

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer1 interrupt

void timer1_int_handler() {
    // Call the UART timeout function
    uart_timeout_triggered();
}

#ifdef USE_ADC_TEST
// ADC conversion complete interrupt handler

void adc_int_handler() {
    int adc_value;
    unsigned char adc_bytes[2];

    // Read the conversion value
    adc_value = ReadADC();

    // Store each byte into the byte array, to make sure that the order is
    // as expected: high byte in index 0
    adc_bytes[0] = (adc_value & 0xFF00) >> 8;
    adc_bytes[1] = adc_value & 0x00FF;
    ToMainLow_sendmsg(2, MSGT_ADC, (void *) adc_bytes);
}
#endif //ifdef USE_ADC_TEST

void encoder_interrupt_handler() {
    unsigned int encData = PORTBbits.RB4;
    INTCONbits.RBIF = 0;
    if (countFlag == 1) {
        tickCount++;
        if (tickCount == 5250) {
            totalRevolutions += 1;
            tickCount = 0;
        }
    }
}
