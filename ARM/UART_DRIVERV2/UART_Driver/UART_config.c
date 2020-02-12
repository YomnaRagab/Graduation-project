/*
 * UART_config.c
 *
 *  Created on: 3 Dec 2019
 *      Author: WEST
 */
#include "UART_priv.h"
#include "UART_int.h"
#include "UART_Config.h"
 UART_module UART_ConfigArray [NUMBER_OF_MODULES];



//    typedef struct
  //    {
  //        u8 UARTNum;
  //        u32 BaudRate ;
  //        UART_DataLengthType DataLength;
  //        UART_StopBitLengthType StopLength ;
  //        UART_ParityBitType ParityBit ;
  //        UART_ParityType Parity_Type ;
  //        TxType TransmitterFifo_Config;
  //        RxType RecieverFifo_Config;
  //        u8 Speed_Mode;
  //    }UART_module;

UART_module UART_ConfigArray [NUMBER_OF_MODULES] =

{
 {UART1 ,
  9600,
  UART_8BitData ,
  UART_ONEStopBit ,
  UART_FIFO_Disabled,
  UART_ParityBitDisabled ,
  UART_EvenParity ,
  { UART_8ByteTriggerLength,IDLE_State,INT_ENABLED,NULL },
  { UART_8ByteTriggerLength,IDLE_State,INT_ENABLED,NULL } ,
  NORMAL_SPEED_MODE   }

};
