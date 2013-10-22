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

void encoder_lthread(int msgtype, int length, unsigned char *msgbuffer){

                /*unsigned char EncData = msgbuffer[0];
                tickCounter += EncData;
                uart_send_bytes((char)(tickCounter), 8);
                if(tickCounter == 100){
                    tickCounter = 0;
                }*/
   /* if (msgtype == MSGT_ENC){
    unsigned char EncData = msgbuffer[0];
            uart_send_bytes(EncData, 1);
    }*/
    switch(msgtype) {
        case MSGT_ENC:
        {
            //keeps track of the number of ticks
            //there are 100 ticks per revolution so it resets to 0 when it gets
            //to 100
            /*if (length == 1){
                unsigned char EncData = msgbuffer[0];
                unsigned char tickCounter;
                tickCounter += EncData;
                uart_send_bytes(EncData, 1);
                //uart_send_bytes((char)(tickCounter), 8);
                if(tickCounter == 100){
                    tickCounter = 0;
                }
            }*/
            break;
        }
        default:
        {
            break;
        }
    }
}
