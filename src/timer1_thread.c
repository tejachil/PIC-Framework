#ifndef __XC8
#include <adc.h>
#else
#include <plib/adc.h>
#endif
#include "maindefs.h"
#include <stdio.h>
#include "messages.h"
#include "timer1_thread.h"

void init_timer1_lthread(timer1_thread_struct *tptr) {
    tptr->msgcount = 0;
}

// This is a "logical" thread that processes messages from TIMER1
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

int timer1_lthread(timer1_thread_struct *tptr, int msgtype, int length, unsigned char *msgbuffer) {
#ifdef USE_UART_TEST
    // Send an incrementing byte over UART
    static unsigned char uart_byte = 0;
    WriteUSART(uart_byte++);
#endif //ifdef USE_UART_TEST

    return 0;
}