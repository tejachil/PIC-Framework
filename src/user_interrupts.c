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
    length = FromMainHigh_recvmsg(sizeof(val), (unsigned char *)&msgtype, (void *) &val);
    if (length == sizeof (val)) {
        ToMainHigh_sendmsg(sizeof (val), MSGT_TIMER0, (void *) &val);
    }
}

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer1 interrupt

void timer1_int_handler() {
    // read the timer and then send an empty message to main()
#ifdef __USE18F2680
    LATBbits.LATB1 = !LATBbits.LATB1;
#endif

    // Send the timer update message to main
    ToMainLow_sendmsg(0, MSGT_TIMER1, (void *) 0);

    // Reset the timer for 10ms period
    WriteTimer1(50535);
}

#ifdef SENSOR_PIC
// ADC conversion complete interrupt handler
void adc_int_handler()
{
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
unsigned char encoderD;
int i = 0;
int test = 1;
char char_test[] = {0x39, 0xB8};
char char_test0[] = {0x47, 0xC6};


void encoder_interrupt_handler(){

    LATBbits.LATB0 ^= 1;
    encData = PORTBbits.RB4;
    INTCONbits.RBIF = 0;
    //if(test != encData){
        encoderD = (char)(encData);
        //uart_send_bytes(encoderD, 1);
        //tickC += encData;
        tickC++;
        if (tickC == 6250){
            i = 0;
            for(i;i < 1; i++){
              if (test == 1){
                tickC = 0;
                //uart_send_bytes(&char_test, 2);
                test = 0;
                break;
            }
            if (test == 0){
                tickC = 10;
                //uart_send_bytes(&char_test0, 2);
                test = 1;
                break;
            }
            }
            /*if (test == 1){
                uart_send_bytes(char_test, 1);
                test = 0;
            }
            if (test == 0){
                uart_send_bytes(char_test0, 1);
                test = 1;
            }*/
        }
        //uart_send_bytes(encoderD, 1); //THIS WORKS!
        //ToMainHigh_sendmsg(1, MSGT_ENC, (void *) encoderD);
        //ToMainHigh_sendmsg(1, MSGT_ENC, encoderD);
    //}
    //uart_send_bytes((char)(tickC), 8);
    //PORTB & 10000000;
    //encoderData = encoderData >> 7;
}