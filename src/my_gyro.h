/* 
 * File:   my_gyro.h
 * Author: TY
 *
 * Created on November 13, 2013, 6:09 PM
 */

#ifndef MY_GYRO_H
#define	MY_GYRO_H

#define ZaxisGyro 0x21
#define lengthGyroZ 0x02

extern int gyro_finished_flag;

void init_myGyro(void);
void timer0_gyro_trigger(void);
void gyro_angleData(unsigned char ZaxisHigh, unsigned char ZaxisLow);
void timer0_counter_start(void);
void timer0_counter_stop(void);

#endif	/* MY_GYRO_H */

