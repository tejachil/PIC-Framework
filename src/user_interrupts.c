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

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer0 interrupt

void timer0_int_handler() {
    unsigned int val;
    int length, msgtype;

    // toggle an LED
#ifdef __USE18F2680
    LATBbits.LATB0 = !LATBbits.LATB0;
#endif
    // reset the timer
    WriteTimer0(0);
    // try to receive a message and, if we get one, echo it back
    length = FromMainHigh_recvmsg(sizeof (val), (unsigned char *) &msgtype, (void *) &val);
    if (length == sizeof (val)) {
        ToMainHigh_sendmsg(sizeof (val), MSGT_TIMER0, (void *) &val);
    }
}

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer1 interrupt

int timer1_counter = 0;

void timer1_int_handler() {
    // read the timer and then send an empty message to main()
#ifdef __USE18F2680
    LATBbits.LATB1 = !LATBbits.LATB1;
#endif

    // Send the timer update message to main
    //ToMainLow_sendmsg(0, MSGT_TIMER1, (void *) 0);

    // Reset the timer for 10ms period
    //WriteTimer1(50535);
    //if (timer1_counter == 200) {
      //  timer1_motor_counter();
        //timer1_counter =0;
    //}
    //timer1_counter++;
}

#ifdef SENSOR_PIC
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

#endif //ifdef SENSOR_PIC

unsigned int encData;
unsigned int tickC = 0;
int totalRevolution = 0;

static char f[] = {0x52, 0xAE};

void encoder_interrupt_handler() {

    LATBbits.LATB0 ^= 1;
    encData = PORTBbits.RB4;
    INTCONbits.RBIF = 0;

    /*tickC++;
    if (tickC == 5250) {
        totalRevolution += 1;
        tickC = 0;
        uart_send_bytes(&f, 2);
    }
    encoder_distance_calc(tickC, totalRevolution);*/
}