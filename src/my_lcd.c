#include "my_lcd.h"
#include "my_uart.h"

void clearLCD(){
    unsigned char buf[2] = {0xFE, 0x01};
    uart_send_bytes(buf, 2);
}

void printLCD(unsigned char *msg, int length){
    static int timer=0;
    if(timer==0){
        clearLCD();
        uart_send_bytes(msg, length);
    }
    timer = (timer+1)%15; // delay of 15 calls
}
