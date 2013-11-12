#ifndef I2C2_THREAD_H
#define	I2C2_THREAD_H

/**
 * Thread to handle all I2C2-related messages.
 */
void i2c2_lthread(int msgtype, int length, unsigned char *msgbuffer);

#endif	/* I2C2_THREAD_H */

