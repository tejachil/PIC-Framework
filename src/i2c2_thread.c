#include "i2c2_thread.h"
#include "my_i2c2.h"
#include "maindefs.h"
#include "my_gyro.h"

void i2c2_lthread(int msgtype, int length, unsigned char *msgbuffer) {
    switch (msgtype) {

#if defined(I2C2_MASTER)
            // RECV_COMPLETE indicates a completed read by this master device
        case MSGT_I2C2_MASTER_RECV_COMPLETE:
        {
            LATBbits.LATB2 ^= 1;
            break;
        } // End case RECV_COMPLETE

            // SEND_COMPLETE indicates a completed write from this master device
        case MSGT_I2C2_MASTER_SEND_COMPLETE:
        {
            i2c2_master_read(GYRO_SLAVE_ADDRESS, ZaxisGyro, lengthGyroZ);
            LATBbits.LATB3 ^= 1;
            break;
        }
            // SEND_FAILED indicates a failed write from this master device
        case MSGT_I2C2_MASTER_SEND_FAILED:
            // RECV_FAILED indicates a failed read by this master device
        case MSGT_I2C2_MASTER_RECV_FAILED:
#endif // I2C2_MASTER

            // The following cases (and the cases above) indicate an error
        default:
        {
            LATBbits.LATB4 ^= 1;
        } // End cases DBG and default

    } // End switch(msgtype)
}
