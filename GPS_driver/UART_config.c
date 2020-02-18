/*
 * UART_config.c
 *
 *  Created on: 3 Dec 2019
 *      Author: WEST
 */
#include "UART_priv.h"
#include "UART_int.h"
#include "UART_Config.h"
#include "GPS_priv.h"
 UART_module UART_ConfigArray [NUMBER_OF_MODULES];
extern void (*const GPS_ptr)(void);
UART_module UART_ConfigArray [NUMBER_OF_MODULES] =

{
 {UART1 ,
  9600,
  UART_8BitData ,
  UART_ONEStopBit ,
  UART_FIFO_Disabled,
  UART_ParityBitDisabled ,
  UART_EvenParity ,
  { UART_8ByteTriggerLength,IDLE_State,INT_DISABLED,NULL },
  { UART_8ByteTriggerLength,IDLE_State,INT_DISABLED,GPS_ptr} ,
  NORMAL_SPEED_MODE   }

};


