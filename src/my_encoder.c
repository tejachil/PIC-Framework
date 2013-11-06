#ifndef __XC8
#include <adc.h>
#else
#include <plib/adc.h>
#endif
#include "maindefs.h"
#include "my_adc.h"
#include "my_gpio.h"
#include "my_encoder.h"
#include "my_motor.h"


unsigned char tickCounter = 0;
int delayCount;

void encoder_lthread(int msgtype, int length, unsigned char *msgbuffer) {

    switch (msgtype) {
        case MSGT_ENC:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}