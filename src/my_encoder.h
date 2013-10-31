/* 
 * File:   my_encoder.h
 * Author: TY
 *
 * Created on October 7, 2013, 8:44 PM
 */

#ifndef MY_ENCODER_H
#define	MY_ENCODER_H

#ifdef	__cplusplus
extern "C" {
#endif
 

void encoder_lthread(int msgtype, int length, unsigned char *msgbuffer);

void encoder_distance_calc(int tickCount, int totalRevolutions);

#ifdef	__cplusplus
}
#endif

#endif	/* MY_ENCODER_H */

