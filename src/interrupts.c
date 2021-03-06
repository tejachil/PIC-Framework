#include "maindefs.h"
#include "interrupts.h"
#include "user_interrupts.h"
#include "messages.h"
#include "my_uart.h"
#include "my_i2c.h"
#include "my_adc.h"

//----------------------------------------------------------------------------
// Note: This code for processing interrupts is configured to allow for high and
//       low priority interrupts.  The high priority interrupt can interrupt the
//       the processing of a low priority interrupt.  However, only one of each type
//       can be processed at the same time.  It is possible to enable nesting of low
//       priority interrupts, but this code is not setup for that and this nesting is not
//       enabled.

void enable_interrupts() {
    // Peripheral interrupts can have their priority set to high or low
    // enable high-priority interrupts and low-priority interrupts
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    INTCONbits.RBIE = 1;
    INTCON2bits.RBIP = 1;
    TRISBbits.RB4 = 1;
    
}

int in_high_int() {
    return (!INTCONbits.GIEH);
}

int low_int_active() {
    return (!INTCONbits.GIEL);
}

int in_low_int() {
    if (INTCONbits.GIEL == 1) {
        return (0);
    } else if (in_high_int()) {
        return (0);
    } else {
        return (1);
    }
}

int in_main() {
    if ((!in_low_int()) && (!in_high_int())) {
        return (1);
    } else {
        return (0);
    }
}

#ifdef __XC8
// Nothing is needed for this compiler
#else
// Set up the interrupt vectors
void InterruptHandlerHigh();
void InterruptHandlerLow();

#pragma code InterruptVectorLow = 0x18

void
InterruptVectorLow(void) {
    _asm
    goto InterruptHandlerLow //jump to interrupt routine
            _endasm
}

#pragma code InterruptVectorHigh = 0x08

void
InterruptVectorHigh(void) {
    _asm
    goto InterruptHandlerHigh //jump to interrupt routine
            _endasm
}
#endif
//----------------------------------------------------------------------------
// High priority interrupt routine
// this parcels out interrupts to individual handlers
#pragma code
#pragma interrupt InterruptHandlerHigh


void InterruptHandlerHigh() {
    // check to see if we have an I2C interrupt
    if (PIR1bits.SSPIF) {
        // clear the interrupt flag
        PIR1bits.SSPIF = 0;
        // call the handler
        i2c_int_handler();
    }

    //check to see if there is an interrupt on RBIF for encoders
    if (INTCONbits.RBIF){
        encoder_interrupt_handler();
    }
}

//----------------------------------------------------------------------------
// Low priority interrupt routine
// this parcels out interrupts to individual handlers
// This works the same way as the "High" interrupt handler
#pragma code
#pragma interruptlow InterruptHandlerLow

void InterruptHandlerLow() {
    // check to see if we have an interrupt on timer 1
    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0; //clear interrupt flag
        timer1_int_handler();
    }

    // check to see if we have an interrupt on USART RX
    if (PIR1bits.RCIF) {
        PIR1bits.RCIF = 0; //clear interrupt flag
        uart_rx_int_handler();
    }


    #ifdef SENSOR_PIC
    // Check to see if we have an ADC interrupt
    if (PIR1bits.ADIF) {
        // Clear the interrupt flag
        PIR1bits.ADIF = 0;
        // Call the ADC interrupt handler
        adc_int_handler();
    }
    #endif //ifdef SENSOR_PIC

    // Check if we have a USART Tx interrupt (TXIF will remain set as long as
    // the peripheral is ready to transmit, so we must also check if the
    // interrupt is enabled).
    if (UART_TX_INT_ENABLED() && PIR1bits.TXIF) {
        // The TXIF flag cannot be cleared in software, so the interrupt must be
        // disabled so it doesn't trigger repeatedly.  It's reenabled once a new
        // byte has been started so that the interrupt will trigger when that
        // new byte is done.
        UART_DISABLE_TX_INT();
        // Call the handler
        uart_tx_int_handler();
    }

    // check to see if we have an interrupt on timer 0
    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0; // clear this interrupt flag
        timer0_int_handler();
    }
}

