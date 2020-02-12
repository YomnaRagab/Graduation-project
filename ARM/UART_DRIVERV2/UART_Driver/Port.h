/*
 * Port.h
 *
 *  Created on: Nov 23, 2019
 *      Author: AhmedShawky
 */

#ifndef PORT_H_
#define PORT_H_
#include "std_types.h"
#define EVENT_FALL_EDGE  0
#define EVENT_RISE_EDGE  1
#define EVENT_BOTH_EDGE  2
#define EVENT_LOW_LEVEL  3
#define EVENT_HIGH_LEVEL 4
#define INT_PRI_HIGHEST              0b000
#define INT_PRI_VERY_HIGH            0b001
#define INT_PRI_HIGH                 0b010
#define INT_PRI_MODERATE_HIGH        0b011
#define INT_PRI_MODERATE             0b100
#define INT_PRI_LOW                  0b101
#define INT_PRI_VERY_LOW             0b110
#define INT_PRI_LOWEST               0b111

typedef enum{
	OK,
	NOK
}Port_enumerr;
typedef enum{
	PORT_PIN_IN,
	PORT_PIN_OUT
}Port_PinDirectionType;
typedef void(*GPIO_FnPtrType)(void);
typedef enum{
	Pending=0xff,
	Inactive=0x00
}Port_IntStatustype;

typedef struct
{
u8 PortId;
u8 PortMask;   /*this field is used to select the pins that we are working on*/
u8 PinsDirection;
u8 PinType;
u8 GPIODR2R;
u8 GPIODR4R;
u8 GPIODR8R;
u8 GPIOODR;
u8 GPIOPUR;
u8 GPIOPDR;
}Port_ConfigType;

//===============================================PORTA PINS=================================================================================
#define PORTA_PIN0                0
#define PORTA_PIN1                1
#define PORTA_PIN2                2
#define PORTA_PIN3                3
#define PORTA_PIN4                4
#define PORTA_PIN5                5
#define PORTA_PIN6                6
#define PORTA_PIN7                7
//===============================================PORTB PINS=================================================================================
#define PORTB_PIN0                8
#define PORTB_PIN1                9
#define PORTB_PIN2                10
#define PORTB_PIN3                11
#define PORTB_PIN4                12
#define PORTB_PIN5                13
#define PORTB_PIN6                14
#define PORTB_PIN7                15
//===============================================PORTC PINS=================================================================================
#define PORTC_PIN0                16
#define PORTC_PIN1                17
#define PORTC_PIN2                18
#define PORTC_PIN3                19
#define PORTC_PIN4                20
#define PORTC_PIN5                21
#define PORTC_PIN6                22
#define PORTC_PIN7                23
//===============================================PORTD PINS=================================================================================
#define PORTD_PIN0                24
#define PORTD_PIN1                25
#define PORTD_PIN2                26
#define PORTD_PIN3                27
#define PORTD_PIN4                28
#define PORTD_PIN5                29
#define PORTD_PIN6                30
#define PORTD_PIN7                31
//===============================================PORTE PINS=================================================================================
#define PORTE_PIN0                32
#define PORTE_PIN1                33
#define PORTE_PIN2                34
#define PORTE_PIN3                35
#define PORTE_PIN4                36
#define PORTE_PIN5                37
//===============================================PORTF PINS=================================================================================
#define PORTF_PIN0                38
#define PORTF_PIN1                39
#define PORTF_PIN2                40
#define PORTF_PIN3                41
#define PORTF_PIN4                42
//=====================================================PORTS===================================================================================
#define PORTA   0
#define PORTB   1
#define PORTC   2
#define PORTD   3
#define PORTE   4
#define PORTF   5

/*--------------------------------------------------------------------
 *                          Special consideration pins                |
 * These pins are locked to these functions and needs to be unlocked  |
 * and reconfigured in case of alternate use                          |
 *                                                                    |
 *  PA[1:0]   -->   UART0                                             |
 *  PA[5:2]   -->   SSI0                                              |
 *  PB[3:2]   -->   I1C0                                              |
 *  PC[3:0]   -->   JTAG                                              |
 *  PD[7]     -->   GPIO                                              |
 *  PF[0]     -->   GPIO                                              |
 --------------------------------------------------------------------*/

void Port_Init( const Port_ConfigType* ConfigPtr );
Port_enumerr Port_SetPinDirection( u8 Pin, Port_PinDirectionType Direction );
void Port_SetPinMode( u8 Pin, u8 Mode );
Port_enumerr Port_SetInterruptEvent(u8 PortId,u8 IntEvent,u8 IntMask);   /*the IntMaskStatus is sent to correspond to the pins that I want the action to effect */
Port_enumerr Port_ClrInterruptFlag(u8 PinId);
Port_enumerr Port_GetInterruptStatus(u8 PinId,Port_IntStatustype *IntStatusPtr);
Port_enumerr Port_PinIntInit(u8 PinId,u8 IntEvent,u8 priority);
Port_enumerr Port_IntEnable(u8 PinId);
Port_enumerr Port_IntDisable(u8 PinId);
Port_enumerr Port_SetIntPriority(u8 PortId,u8 Priority);

void PORTA_ISR(void);
void PORTA_PIN0_ISR(void(*Local_PIN0_ISR1)(void),void(*Local_PIN0_ISR2)(void));
void PORTA_PIN1_ISR(void(*Local_PIN1_ISR1)(void),void(*Local_PIN1_ISR2)(void));
void PORTA_PIN2_ISR(void(*Local_PIN2_ISR1)(void),void(*Local_PIN2_ISR2)(void));
void PORTA_PIN3_ISR(void(*Local_PIN3_ISR1)(void),void(*Local_PIN3_ISR2)(void));
void PORTA_PIN4_ISR(void(*Local_PIN4_ISR1)(void),void(*Local_PIN4_ISR2)(void));
void PORTA_PIN5_ISR(void(*Local_PIN5_ISR1)(void),void(*Local_PIN5_ISR2)(void));
void PORTA_PIN6_ISR(void(*Local_PIN6_ISR1)(void),void(*Local_PIN6_ISR2)(void));
void PORTA_PIN7_ISR(void(*Local_PIN7_ISR1)(void),void(*Local_PIN7_ISR2)(void));


#endif /* PORT_H_ */
