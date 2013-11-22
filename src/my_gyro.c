#include "my_gyro.h"
#include "my_i2c2.h"
#include "i2c2_thread.h"
#include <timers.h>
#include "my_motor.h"

/**
 * Timeout timer count register value.  Calculation is based on the
 * following:<br>
 * Timer1 clock is Fosc / 4: 12 MHz / 4 = 3 MHz
 * Prescaler of 2: 3 MHz / 2 = 1.5 MHz
 * 1.5 MHz / (Timeout freq) = N counts needed for timeout period
 * (2^16 - 1) - N = I, the initial count value
 */

//set timer 0 to 60HZ
#define TIMER_0_COUNT 40500

const unsigned char gyro_init_data[] = {0x15, 0x07, 0x1B};
unsigned char firstMessageLength = 0x03;

int timer0_counter = 0;
int gyro_finished_flag = 0;

static int targetTickCount;
static int angleToTurn;
static int gyroDataHolder[10];
static int Zaxis;
static int sumOfGyroData;
static int averageOfGyroData;
static double degreesPerSecond;

void init_myGyro() {
    // Timer0 interrupt
    INTCON2bits.TMR0IP = 0;
    // Setup Timer0
    // DO NOT CHANGE TIMER PARAMETERS WITHOUT CORRECTING THE INITIAL COUNT
    // CALCULATION
    WriteTimer0(TIMER_0_COUNT);
    OpenTimer0(TIMER_INT_ON & T0_PS_1_2 & T0_16BIT & T0_SOURCE_INT);
    //disable timer0
    T0CONbits.TMR0ON = 0;
    i2c2_master_write(GYRO_SLAVE_ADDRESS, gyro_init_data, firstMessageLength);
}

void timer0_counter_start(int angleCalc) {
    targetTickCount = 360;
    angleToTurn = angleCalc;
    degreesPerSecond = 0.0;
    T0CONbits.TMR0ON = 1;
    timer0_counter = 0;
    //targetTickCount = (int)angleCalc*10/9; // will be moved into timer trigger logic
    gyro_finished_flag = 0;
}

void timer0_counter_stop() {
    T0CONbits.TMR0ON = 0;
    gyro_finished_flag = 1;
    motor_stop_both();
}

void timer0_gyro_trigger() {
    //Logic to calc counter value for angle based on gyro readings
    WriteTimer0(TIMER_0_COUNT);
    LATBbits.LATB0 ^= 1;
    timer0_counter++;
    if (timer0_counter < 31) {
        i2c2_master_read(GYRO_SLAVE_ADDRESS, ZaxisGyro, lengthGyroZ);
        gyro_angleData(gyroDataHigh, gyroDataLow);
    }
    if (timer0_counter >= targetTickCount) {
        timer0_counter_stop();
    }
}

void gyro_angleData(unsigned char ZaxisHigh, unsigned char ZaxisLow) {
    //ZaxisHigh = 0x02;
    //ZaxisLow = 0x4E;
    if (timer0_counter < 30) {
        Zaxis = (int) ((((int) ZaxisHigh) << 8) | ((int) ZaxisLow));
        if(Zaxis < 0){
            Zaxis *= -1;
        }
        gyroDataHolder[timer0_counter] = Zaxis;
    } else if (timer0_counter >= 30) {
        int i = 8;
        sumOfGyroData = 0;
        for (i; i < 30; i++) {
            sumOfGyroData += (int) gyroDataHolder[i];
        }
        averageOfGyroData = sumOfGyroData / 22;
        gyro_angleCalculation(averageOfGyroData);
    }
}

void gyro_angleCalculation(int AverageOfGyroData) {
    degreesPerSecond = ((double) AverageOfGyroData) / 16.3835;
    //targetTickCount = angleToTurn;
    targetTickCount = (int) ((60 * angleToTurn) / degreesPerSecond);
}