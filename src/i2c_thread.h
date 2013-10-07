#ifndef I2C_THREAD_H
#define	I2C_THREAD_H

/**
 * Thread to handle all I2C-related messages.
 * @param msgtype
 * @param length
 * @param msgbuffer
 */
void i2c_lthread(int msgtype, int length, unsigned char *msgbuffer);

#endif	/* I2C_THREAD_H */

