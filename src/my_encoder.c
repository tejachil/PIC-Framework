#ifndef __XC8
#include <adc.h>
#else
#include <plib/adc.h>
#endif
#include "maindefs.h"
#include "my_adc.h"
#include "my_gpio.h"
#include "my_encoder.h"


unsigned char tickCounter = 0;

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

void encoder_distance_calc(int tickCount, int totalRevolutions) {
    double totalDistance = 0;
    int totalRevolutionsHolder = totalRevolutions;
    int tickCountHolder = tickCount;
    for (; totalRevolutionsHolder > 0; totalRevolutionsHolder--) {
        totalDistance += 35.908;
    }
    totalDistance += (((double) tickCountHolder) / 5250)*35.908;

    if (totalDistance > 37.3447) {
        totalDistance = 5;
    }
}