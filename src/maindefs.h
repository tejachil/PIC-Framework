#ifndef MAINDEFS_H
#define MAINDEFS_H

#ifdef __18F45J10
#define __USE18F45J10 1
#else
#error "Only the PIC18F45J10 is supported"
#endif

#include <p18cxxx.h>

// Message type definitions
#define MSGT_MAIN1 20
#define	MSGT_OVERRUN 30
#define MSGT_UART_DATA 31
#define MSGT_I2C_DBG 41
#define	MSGT_I2C_DATA 40
#define MSGT_I2C_RQST 42
#define MSGT_I2C_MASTER_SEND_COMPLETE 43
#define MSGT_I2C_MASTER_SEND_FAILED 44
#define MSGT_I2C_MASTER_RECV_COMPLETE 45
#define MSGT_I2C_MASTER_RECV_FAILED 46
#define MSGT_ADC 50 // ADC conversion value
#define MSGT_ENC 51
#define MSGT_I2C_QUEUED_MSG (60)
#define MSGT_UART_QUEUED_MSG (61)
#define MSGT_I2C2_MASTER_SEND_COMPLETE (70)
#define MSGT_I2C2_MASTER_SEND_FAILED (71)
#define MSGT_I2C2_MASTER_RECV_COMPLETE (72)
#define MSGT_I2C2_MASTER_RECV_FAILED (73)

// Main device ID definitions

/** Define MASTER_PIC to compile for the Master PIC. */
//#define MASTER_PIC
/** Define MOTOR_PIC to compile for the Motor Controller PIC. */
#define MOTOR_PIC
/** Define SENSOR_PIC to compile for the Proximity Sensors PIC. */
//#define SENSOR_PIC

// Specific functionality enable/disable definitions

// Define USE_ADC_TEST to enable reporting ADC readings over I2C.
//#define USE_ADC_TEST

// Define either I2C_MASTER or I2C_SLAVE (but not both) to select I2C role.
#if defined(MASTER_PIC)
#define I2C_MASTER
// Define I2C_MASTER_IGNORE_NACK to allow limited testing with no slave.
//#define I2C_MASTER_IGNORE_NACK
#elif defined(MOTOR_PIC)
#define I2C_SLAVE
#define I2C2_MASTER
// Define I2C2_MASTER_IGNORE_NACK to allow limited I2C2 testing with no slave.
//#define I2C2_MASTER_IGNORE_NACK
#elif defined(SENSOR_PIC)
#define I2C_SLAVE
#endif

// Check that exactly one target is defined
#if (!defined(MASTER_PIC) && !defined(MOTOR_PIC) && !defined(SENSOR_PIC))
#error "Compiling without a defined target device"
#elif (defined(MASTER_PIC) && defined(MOTOR_PIC))
#error "Compiling with two or more defined target devices"
#elif (defined(MASTER_PIC) && defined(SENSOR_PIC))
#error "Compiling with two or more defined target devices"
#elif (defined(MOTOR_PIC) && defined(SENSOR_PIC))
#error "Compiling with two or more defined target devices"
#endif

#endif // ifndef MAINDEFS_H

