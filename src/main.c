#include "maindefs.h"
#include <stdio.h>
#include <usart.h>
#include <i2c.h>
#include <timers.h>
#include <adc.h>
#include "interrupts.h"
#include "messages.h"
#include "my_uart.h"
#include "my_i2c.h"
#include "uart_thread.h"
#include "my_gpio.h"
#include "my_adc.h"
#include "public_messages.h"
#include "i2c_thread.h"
#include "i2c_queue_thread.h"
#include "uart_queue_thread.h"
#include "my_motor.h"
#include "user_interrupts.h"
#include "my_i2c2.h"
#include "i2c2_thread.h"
#include "my_gyro.h"

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

static i2c2_comm ic2;

void main(void) {
    signed char length;
    unsigned char msgtype;
    uart_comm uc;
    i2c_comm ic;
    unsigned char msgbuffer[MSGLEN + 1];

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

    // Setup PORTB as output
    gpio_init_portb_output();

    // Timer0 is on by default, turn it off
    CloseTimer0();

    // Initialize UART driver (Rx and Tx)
    uart_init(&uc);

    // initialize the i2c code
    init_i2c(&ic);

    // initialize message queues before enabling any interrupts
    init_queues();

    // Initialize encoder tick counter and revolution counter
    encoders_init();

    // Initialize the gyro
    init_myGyro();

    // Decide on the priority of the enabled peripheral interrupts
    // 0 is low, 1 is high
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
    // I2C2 interrupt
    IPR3bits.SSP2IP = 1;

#if defined(I2C_SLAVE)
#if defined(MOTOR_PIC)
    i2c_configure_slave(MOTOR_PIC_ADDR << 1);
#elif defined(SENSOR_PIC)
    i2c_configure_slave(SENSOR_PIC_ADDR << 1);
#endif

#elif defined(I2C_MASTER)
    // Configure the hardware I2C device as a master
    i2c_configure_master();
#endif

#ifdef I2C2_MASTER
    init_i2c2(&ic2);
    i2c2_configure_master();
#endif // I2C2_MASTER

    // must specifically enable the I2C interrupts
    PIE1bits.SSPIE = 1;
#ifdef I2C2_MASTER
    PIE3bits.SSP2IE = 1;
#endif

    // Peripheral interrupts can have their priority set to high or low
    // enable high-priority interrupts and low-priority interrupts
    enable_interrupts();

#ifdef USE_ADC_TEST
    // Initialize and start the ADC driver
    adc_init();
    adc_start();
#endif //ifdef USE_ADC_TEST

    {
        unsigned char const test_data[2] = {0xAA, 0xBB};
        //i2c2_master_write(0x11, test_data, 2);
        i2c2_master_read(0x11, 0x22, 2);
    }

    // loop forever
    // This loop is responsible for "handing off" messages to the subroutines
    // that should get them.  Although the subroutines are not threads, but
    // they can be equated with the tasks in your task diagram if you
    // structure them properly.
    while (1) {
        // Check the high priority queue
        length = ToMainHigh_recvmsg(MSGLEN, &msgtype, (void *) msgbuffer);
        if (length < 0) {
            // no message, check the error code to see if it is concern
            if (length != MSGQUEUE_EMPTY) {
                // This case be handled by your code.
            }
        } else {
            switch (msgtype) {
                case MSGT_I2C_DATA:
                case MSGT_I2C_DBG:
                case MSGT_I2C_RQST:
                case MSGT_I2C_MASTER_SEND_COMPLETE:
                case MSGT_I2C_MASTER_SEND_FAILED:
                case MSGT_I2C_MASTER_RECV_COMPLETE:
                case MSGT_I2C_MASTER_RECV_FAILED:
                {
                    i2c_lthread(msgtype, length, msgbuffer);
                    break;
                } // End I2C cases

#ifdef I2C2_MASTER
                case MSGT_I2C2_MASTER_SEND_COMPLETE:
                case MSGT_I2C2_MASTER_SEND_FAILED:
                case MSGT_I2C2_MASTER_RECV_COMPLETE:
                case MSGT_I2C2_MASTER_RECV_FAILED:
                {
                    i2c2_lthread(msgtype, length, msgbuffer);
                    break;
                } // End I2C2 cases
#endif // I2C2_MASTER

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
                case MSGT_OVERRUN:
                case MSGT_UART_DATA:
                {
                    uart_lthread(msgtype, length, msgbuffer);
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

        // If the UART Tx peripheral is available, send the next queued message
        if (!uart_tx_busy()) {
            length = ToUART_recvmsg(MSGLEN, &msgtype, (void *) msgbuffer);
            if (length < 0) {
                // no message, check the error code to see if it is concern
                if (length != MSGQUEUE_EMPTY) {
                    // Your code should handle this situation
                }
            } else {
                switch (msgtype) {
                    case MSGT_UART_QUEUED_MSG:
                    {
                        uart_queue_lthread(msgtype, length, msgbuffer);
                        break;
                    }
                    default:
                    {
                        SET_UART_QUEUE_ERROR_PIN();
                        break;
                    }
                }
            }
        }

#ifdef MASTER_PIC
        // If the I2C peripheral is available, send the next queued message
        if (!i2c_master_busy()) {
            length = ToI2C_recvmsg(&msgtype, (public_message_t *) msgbuffer);
            if (length < 0) {
                // no message, check the error code to see if it is concern
                if (length != MSGQUEUE_EMPTY) {
                    // Your code should handle this situation
                }
            } else {
                switch (msgtype) {
                    case MSGT_I2C_QUEUED_MSG:
                    {
                        i2c_queue_lthread(msgtype, length, msgbuffer);
                        break;
                    }
                    default:
                    {
                        SET_I2C_QUEUE_ERROR_PIN();
                        break;
                    }
                }
            }
        }
#endif // MASTER_PIC
    }
}