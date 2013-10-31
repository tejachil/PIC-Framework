#ifndef __maindefs
#define __maindefs

#ifdef __XC8
#include <xc.h>
#ifdef _18F45J10
#define __USE18F45J10 1
#else
#ifdef _18F2680
#define __USE18F2680 1
#else
#ifdef _18F26J50
#define __USE18F26J50 1
#endif
#endif
#endif
#else
#ifdef __18F45J10
#define __USE18F45J10 1
#else
#ifdef __18F2680
#define __USE18F2680 1
#else
#ifdef __18F26J50
#define __USE18F26J50 1
#endif
#endif
#endif
#include <p18cxxx.h>
#endif

// Message type definitions
#define MSGT_TIMER0 10
#define MSGT_TIMER1 11
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

// Main device ID definitions

/** Define MASTER_PIC to compile for the Master PIC. */
//#define MASTER_PIC
/** Define MOTOR_PIC to compile for the Motor Controller PIC. */
#define MOTOR_PIC
//#define USE_LCD
/** Define SENSOR_PIC to compile for the Proximity Sensors PIC. */
//#define SENSOR_PIC

#if (!defined(MASTER_PIC) && !defined(MOTOR_PIC) && !defined(SENSOR_PIC))
#error "Compiling without a defined target device"
#endif

// Functionality enable/disable definitions

// Define either I2C_MASTER or I2C_SLAVE (but not both) to select I2C role.
#if defined(MASTER_PIC)
#define I2C_MASTER
// Define I2C_MASTER_IGNORE_NACK to allow limited testing with no slave.
#define I2C_MASTER_IGNORE_NACK
#else
#define I2C_SLAVE
#endif

#endif

