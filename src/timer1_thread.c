#ifndef __XC8
#include <usart.h>
#else
#include <plib/usart.h>
#endif
#include "maindefs.h"
#include <stdio.h>
#include "messages.h"
#include "timer1_thread.h"

// This is a "logical" thread that processes messages from TIMER1
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

int timer1_lthread(int msgtype, int length, unsigned char *msgbuffer) {
    // Nothing to do
    return 0;
}