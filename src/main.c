#include "maindefs.h"
#include <stdio.h>
#ifndef __XC8
#include <usart.h>
#include <i2c.h>
#include <timers.h>
#include <adc.h>
#include <math.h>
#else
#include <plib/usart.h>
#include <plib/i2c.h>
#include <plib/timers.h>
#include <plib/adc.h>
#endif
#include "interrupts.h"
#include "messages.h"
#include "my_uart.h"
#include "my_i2c.h"
#include "my_lcd.h"
#include "uart_thread.h"
#include "timer1_thread.h"
#include "timer0_thread.h"
#include "my_gpio.h"
#ifdef USE_ADC_TEST
#include "my_adc.h"
#endif //ifdef USE_ADC_TEST

#ifdef __USE18F45J10
// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on SWDTEN bit))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow disabled)
#ifndef __XC8
// Have to turn this off because I don't see how to enable this in the checkboxes for XC8 in this IDE
#pragma config XINST = ON       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#else
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#endif

// CONFIG1H
#pragma config CP0 = OFF        // Code Protection bit (Program memory is not code-protected)

// CONFIG2L
#pragma config FOSC = HSPLL     // Oscillator Selection bits (HS oscillator, PLL enabled and under software control)
#pragma config FOSC2 = ON       // Default/Reset System Clock Select bit (Clock selected by FOSC as system clock is enabled when OSCCON<1:0> = 00)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = DEFAULT // CCP2 MUX bit (CCP2 is multiplexed with RC1)

#else
#ifdef __USE18F2680
#pragma config OSC = IRCIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bits (1K words (2K bytes) Boot Block)
#ifndef __XC8
// Have to turn this off because I don't see how to enable this in the checkboxes for XC8 in this IDE
#pragma config XINST = ON       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#endif
#else
#ifdef __USE18F26J50

// PIC18F26J50 Configuration Bit Settings

// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer (Disabled - Controlled by SWDTEN bit)
#pragma config PLLDIV = 3       // PLL Prescaler Selection bits (Divide by 3 (12 MHz oscillator input))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset  (Disabled)
#pragma config XINST = ON       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config CPUDIV = OSC1    // CPU System Clock Postscaler (No CPU system clock divide)
#pragma config CP0 = OFF        // Code Protect (Program memory is not code-protected)

// CONFIG2L
#pragma config OSC = HSPLL      // Oscillator (HS+PLL, USB-HS+PLL)
#pragma config T1DIG = OFF      // T1OSCEN Enforcement (Secondary Oscillator clock source may not be selected)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator (High-power operation)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = ON        // Internal External Oscillator Switch Over Mode (Enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Postscaler (1:32768)

// CONFIG3L
#pragma config DSWDTOSC = T1OSCREF// DSWDT Clock Select (DSWDT uses T1OSC/T1CKI)
#pragma config RTCOSC = T1OSCREF// RTCC Clock Select (RTCC uses T1OSC/T1CKI)
#pragma config DSBOREN = OFF    // Deep Sleep BOR (Disabled)
#pragma config DSWDTEN = OFF    // Deep Sleep Watchdog Timer (Disabled)
#pragma config DSWDTPS = G2     // Deep Sleep Watchdog Postscaler (1:2,147,483,648 (25.7 days))

// CONFIG3H
#pragma config IOL1WAY = ON     // IOLOCK One-Way Set Enable bit (The IOLOCK bit (PPSCON<0>) can be set once)
#pragma config MSSP7B_EN = MSK7 // MSSP address masking (7 Bit address masking mode)

// CONFIG4L
#pragma config WPFP = PAGE_63   // Write/Erase Protect Page Start/End Location (Write Protect Program Flash Page 63)
#pragma config WPEND = PAGE_WPFP// Write/Erase Protect Region Select (valid when WPDIS = 0) (Page WPFP<5:0> through Configuration Words erase/write protected)
#pragma config WPCFG = OFF      // Write/Erase Protect Configuration Region (Configuration Words page not erase/write-protected)

// CONFIG4H
#pragma config WPDIS = OFF      // Write Protect Disable bit (WPFP<5:0>/WPEND region ignored)
#else

Something is messed up
#endif
#endif
#endif

void main(void) {
    char c;
    signed char length;
    unsigned char msgtype;
    unsigned char last_reg_recvd;
    uart_comm uc;
    i2c_comm ic;
    unsigned char msgbuffer[MSGLEN + 1];
    char lcdBuf[10] = {0};
    unsigned char i;
    uart_thread_struct uthread_data; // info for uart_lthread
    timer1_thread_struct t1thread_data; // info for timer1_lthread
    timer0_thread_struct t0thread_data; // info for timer0_lthread

#ifdef __USE18F2680
    OSCCON = 0xFC; // see datasheet
    // We have enough room below the Max Freq to enable the PLL for this chip
    OSCTUNEbits.PLLEN = 1; // 4x the clock speed in the previous line
#else
#ifdef __USE18F45J10
    OSCCON = 0x82; // see datasheeet
    OSCTUNEbits.PLLEN = 0; // Makes the clock exceed the PIC's rated speed if the PLL is on
#else
#ifdef __USE18F26J50
    OSCCON = 0xE0; // see datasheeet
    OSCTUNEbits.PLLEN = 1;
#else
    Something is wrong
#endif
#endif
#endif

    // Initialize UART driver (Rx and Tx)
    uart_init(&uc);

    // initialize the i2c code
    init_i2c(&ic);

    // init the timer1 lthread
    init_timer1_lthread(&t1thread_data);

    // initialize message queues before enabling any interrupts
    init_queues();

    // Setup PORTB as output
    gpio_init_portb_output();

    // how to set up PORTA for input (for the V4 board with the PIC2680)
    /*
            PORTA = 0x0;	// clear the port
            LATA = 0x0;		// clear the output latch
            ADCON1 = 0x0F;	// turn off the A2D function on these pins
            // Only for 40-pin version of this chip CMCON = 0x07;	// turn the comparator off
            TRISA = 0x0F;	// set RA3-RA0 to inputs
     */

    // initialize Timers
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_128);
#ifdef __USE18F26J50
    // MTJ added second argument for OpenTimer1()
    OpenTimer1(TIMER_INT_ON & T1_SOURCE_FOSC_4 & T1_PS_1_2 & T1_16BIT_RW & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF, 0x0);
#else
    // Setup Timer1 for 10ms period
    // Timer1 clock is Fosc / 4, so 12 MHz / 4 = 3MHz
    // Prescaler of 2 sets timer count freq to 1.5MHz
    // 1.5MHz / 100 Hz = 15000 counts needed for 10ms period
    // (2^16 - 1) - 15000 = 50535, this is the timer count initial value
    WriteTimer1(50535);
    OpenTimer1(TIMER_INT_ON & T1_PS_1_2 & T1_16BIT_RW & T1_SOURCE_INT & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF);
#endif

    // Decide on the priority of the enabled peripheral interrupts
    // 0 is low, 1 is high
    // Timer1 interrupt
    IPR1bits.TMR1IP = 0;
    // USART RX interrupt
    IPR1bits.RCIP = 0;
    // I2C interrupt
    IPR1bits.SSPIP = 1;
#ifdef USE_ADC_TEST
    // ADC interrupt
    IPR1bits.ADIP = 1;
#endif //ifdef USE_ADC_TEST
    // USART Tx interrupt
    IPR1bits.TXIP = 0;

    // configure the hardware i2c device as a slave (0x9E becomes address 0x4F
    // because the lowest bit is the read/write bit)
    i2c_configure_slave(0x9E);

    // must specifically enable the I2C interrupts
    PIE1bits.SSPIE = 1;

    // Peripheral interrupts can have their priority set to high or low
    // enable high-priority interrupts and low-priority interrupts
    enable_interrupts();

#ifdef USE_ADC_TEST
    // Initialize and start the ADC driver
    adc_init();
    adc_start();
#endif //ifdef USE_ADC_TEST

    /* Junk to force an I2C interrupt in the simulator (if you wanted to)
    PIR1bits.SSPIF = 1;
    _asm
    goto 0x08
    _endasm;
     */

    // printf() is available, but is not advisable.  It goes to the UART pin
    // on the PIC and then you must hook something up to that to view it.
    // It is also slow and is blocking, so it will perturb your code's operation
    // Here is how it looks: printf("Hello\r\n");

    // loop forever
    // This loop is responsible for "handing off" messages to the subroutines
    // that should get them.  Although the subroutines are not threads, but
    // they can be equated with the tasks in your task diagram if you
    // structure them properly.

    while (1) {
        // Call a routine that blocks until either on the incoming
        // messages queues has a message (this may put the processor into
        // an idle mode)
        block_on_To_msgqueues();
        // At this point, one or both of the queues has a message.  It
        // makes sense to check the high-priority messages first -- in fact,
        // you may only want to check the low-priority messages when there
        // is not a high priority message.  That is a design decision and
        // I haven't done it here.
        length = ToMainHigh_recvmsg(MSGLEN, &msgtype, (void *) msgbuffer);
        if (length < 0) {
            // no message, check the error code to see if it is concern
            if (length != MSGQUEUE_EMPTY) {
                // This case be handled by your code.
            }
        } else {
            switch (msgtype) {
                case MSGT_TIMER0:
                {
                    timer0_lthread(&t0thread_data, msgtype, length, msgbuffer);
                    break;
                };
                case MSGT_I2C_DATA:
                case MSGT_I2C_DBG:
                {
                    // Here is where you could handle debugging, if you wanted
                    // keep track of the first byte received for later use (if desired)
                    last_reg_recvd = msgbuffer[0];
                    break;
                };
                case MSGT_I2C_RQST:
                {
                    // Generally, this is *NOT* how I recommend you handle an I2C slave request
                    // I recommend that you handle it completely inside the i2c interrupt handler
                    // by reading the data from a queue (i.e., you would not send a message, as is done
                    // now, from the i2c interrupt handler to main to ask for data).
                    //
                    // The last byte received is the "register" that is trying to be read
                    // The response is dependent on the register.
                    switch (last_reg_recvd) {
                        case 0xaa:
                        {
                            length = 2;
                            msgbuffer[0] = 0x55;
                            msgbuffer[1] = 0xAA;
                            break;
                        }
                        case 0xa8:
                        {
                            length = 1;
                            msgbuffer[0] = 0x3A;
                            break;
                        }
                        case 0xa9:
                        {
                            length = 1;
                            msgbuffer[0] = 0xA3;
                            break;
                        }
#ifdef USE_ADC_TEST
                            // ADC data register - 16 bit value
                        case ADC_FULL_DATA_REGISTER:
                        {
                            // Get the latest ADC reading
                            const int adc_val = adc_read(0);
                            length = 2;
                            // Place the low byte in the first msg byte
                            msgbuffer[0] = adc_val & 0x00FF;
                            // Place the high byte in the second msg byte
                            msgbuffer[1] = (adc_val & 0xFF00) >> 8;

                            break;
                        }
                            // ADC low byte register
                        case ADC_LOW_DATA_REGISTER:
                        {
                            // Get the latest ADC reading
                            const int adc_val = adc_read(0);
                            length = 1;
                            // Place the low byte in the first msg byte
                            msgbuffer[0] = adc_val & 0x00FF;

                            break;
                        }
                            // ADC low byte register
                        case ADC_HIGH_DATA_REGISTER:
                        {
                            // Get the latest ADC reading
                            const int adc_val = adc_read(0);
                            length = 1;
                            // Place the high byte in the first msg byte
                            msgbuffer[0] = (adc_val & 0xFF00) >> 8;
                            //int voltage = (3.3/1024)*adc_read(0);
                            //int x = pow(voltage, -1.15);
                           // int adc = 1000 * 12.21 * pow(voltage, -1.15);
                            sprintf(lcdBuf, "IR: %d$   " , (adc_read(0)) );
                            //uart_send_bytes(lcdBuf, 1);
                            printLCD(lcdBuf,10);
                            break;
                        }
#endif //ifdef USE_ADC_TEST
                    };
                    start_i2c_slave_reply(length, msgbuffer);
                    break;
                };
                default:
                {
                    // Your code should handle this error
                    break;
                };
            };
        }

        // Check the low priority queue
        length = ToMainLow_recvmsg(MSGLEN, &msgtype, (void *) msgbuffer);
        if (length < 0) {
            // no message, check the error code to see if it is concern
            if (length != MSGQUEUE_EMPTY) {
                // Your code should handle this situation
            }
        } else {
            switch (msgtype) {
                case MSGT_TIMER1:
                {
                    timer1_lthread(&t1thread_data, msgtype, length, msgbuffer);
                    break;
                };
                case MSGT_OVERRUN:
                case MSGT_UART_DATA:
                {
                    uart_lthread(&uthread_data, msgtype, length, msgbuffer);
                    break;
                };
                case MSGT_ADC:
                {
#ifdef USE_ADC_TEST
                    adc_lthread(msgtype, length, msgbuffer);
#endif //ifdef USE_ADC_TEST
                    break;
                }
                default:
                {
                    // Your code should handle this error
                    break;
                };
            };
        }
    }

}