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

// Functionality enable/disable definitions

// Define USE_ADC_TEST to enable reporting ADC readings over I2C.
//#define USE_ADC_TEST

// Define either I2C_MASTER or I2C_SLAVE (but not both) to select I2C role.
#define I2C_MASTER
//#define I2C_SLAVE

// Define I2C_MASTER_IGNORE_NACK to allow limited testing with no slave.
//#define I2C_MASTER_IGNORE_NACK

#endif

