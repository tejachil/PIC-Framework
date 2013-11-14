#include "my_gyro.h"
#include "my_i2c2.h"
#include "i2c2_thread.h"

const unsigned char gyro_init_data[] = {0x15, 0x07, 0x03, 0x03};
unsigned char firstMessageLength = 0x04;

void init_myGyro(){
    i2c2_master_write(GYRO_SLAVE_ADDRESS, gyro_init_data, firstMessageLength);
}
