/*
 * main.c
 *
 *  Created on: Oct 9, 2019
 *      Author: AhmedShawky
 */





/*
 * main.c
 *
 *  Created on: Oct 6, 2019
 *      Author: AhmedShawky
 */
#include "DIO.h"
#include "DIO_Config.h"
#include "DIO_types.h"
#include "DIO_Utils.h"
#include "DIO_Priv.H"
#include "Port_Config.h"
#include "Port_Priv.h"
#include "GPIO_MemMap.h"
#include "utils.h"
#include "std_types.h"
#include"Priv.h"
#include "UART_int.h"
#include "UART_priv.h"
#include "UART_Config.h"
#include "UART_MemMap.h"

//typedef struct
//{
//    u8 PortId;
//    u8 PortMask;   /*this field is used to select the pins that we are working on*/
//    u8 PinsDirection;
//    u8 PinType;
//    u8 GPIODR2R;
//    u8 GPIODR4R;
//    u8 GPIODR8R;
//    u8 GPIOODR;
//    u8 GPIOPUR;
//    u8 GPIOPDR;
//}Port_ConfigType;
void delay(u32 count);
extern const Port_ConfigType PORTB_Config;
extern const Port_ConfigType PORTF_Config;
//void Port_SetPinMode( u8 Pin, u8 Mode );
//void Port_Init( const Port_ConfigType* ConfigPtr );
void main()
{

    Port_Init(&PORTB_Config);
    Port_Init(&PORTF_Config);
    Port_SetPinMode(9,0x01);
    Port_SetPinMode(8,0x01);
    UART_init () ;
    SET_BIT(UARTCTL(UART1),9);
    u8 x='a';
    u8 z;
    while(1)
    {
       // UART_privSendChar(UART1, x);
        while( GET_BIT(UARTRIS(UART1),4)==0 );
        z= UART_privRecieveChar(UART1);
        if(z=='a')
        {
            Dio_WriteChannel(40, HIGH);
        }


    }

}

void delay(u32 count)
{
    u32 volatile  i=0;
    for(i=0; i<count; i++);
}











