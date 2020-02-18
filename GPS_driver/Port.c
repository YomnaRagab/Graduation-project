/*
 * Port.c
 *
 *  Created on: Nov 23, 2019
 *      Author: AhmedShawky
 */

/*================================================== Set Pin Direction Function ================================================================*/
#include "GPIO_MemMap.h"
#include "Priv.h"
#include "Port.h"
#include "Port_Config.h"
#include "Port_Priv.h"
#include "utils.h"
#define GPIO_UNLOCK_VAL                                   0x4C4F434B
#define VECTKEY                                           0x05FA
void (*PA_PIN0_ISR1)(void)='\0';
void (*PA_PIN1_ISR1)(void)='\0';
void (*PA_PIN2_ISR1)(void)='\0';
void (*PA_PIN3_ISR1)(void)='\0';
void (*PA_PIN4_ISR1)(void)='\0';
void (*PA_PIN5_ISR1)(void)='\0';
void (*PA_PIN6_ISR1)(void)='\0';
void (*PA_PIN7_ISR1)(void)='\0';
void (*PA_PIN0_ISR2)(void)='\0';
void (*PA_PIN1_ISR2)(void)='\0';
void (*PA_PIN2_ISR2)(void)='\0';
void (*PA_PIN3_ISR2)(void)='\0';
void (*PA_PIN4_ISR2)(void)='\0';
void (*PA_PIN5_ISR2)(void)='\0';
void (*PA_PIN6_ISR2)(void)='\0';
void (*PA_PIN7_ISR2)(void)='\0';
u32 PORTS_BASES[PORTS_NUMBER]={GPIO_PORTA_BASE,GPIO_PORTB_BASE,GPIO_PORTC_BASE,GPIO_PORTD_BASE,GPIO_PORTE_BASE,GPIO_PORTF_BASE};
const u8 PortIntNum[PORTS_NUMBER]={0,1,2,3,4,30};
const u8 PRIReg_BitShifts[PORTS_NUMBER]={5,13,21,29,5,21};
extern const u8 PinsPerPort[PORTS_NUMBER];
u8 BothEdgesIntPinMask=0;   /*This global variable carries the pin number of the pin that it's INT is configured to sense both edges*/
/*============================================= Port initialization function ==================================================*/

void Port_Init( const Port_ConfigType* ConfigPtr )
{
    SET_BIT(RCGCGPIO,ConfigPtr->PortId); //ENABLING THE CLOCK
    GPIODIR(ConfigPtr->PortId)|=(ConfigPtr->PinsDirection);
    GPIODEN(ConfigPtr->PortId)|=(ConfigPtr->PinType);
    GPIOAMSEL(ConfigPtr->PortId)|=~(ConfigPtr->PinType);
    GPIODR2R(ConfigPtr->PortId)|=(ConfigPtr->GPIODR2R);
    GPIODR4R(ConfigPtr->PortId)|=(ConfigPtr->GPIODR4R);
    GPIODR8R(ConfigPtr->PortId)|=(ConfigPtr->GPIODR8R);
    GPIOODR(ConfigPtr->PortId)|=(ConfigPtr->GPIOODR);
    GPIOPDR(ConfigPtr->PortId)|=(ConfigPtr->GPIOPDR);
    GPIOPUR(ConfigPtr->PortId)|=(ConfigPtr->GPIOPUR);
}

/*=========================================================== Set pin direction function ===============================================================*/

Port_enumerr Port_SetPinDirection( u8 Pin, Port_PinDirectionType Direction )
{
    u8 Loc_PinNumber;
    u8 Loc_PortNumber;
    if(Pin<=MaxPinNum)
    {
        GPIO_enumGetPinNum(Pin,&Loc_PinNumber);
        GPIO_enumGetPortNum(Pin,&Loc_PortNumber);
        GPIODIR(Loc_PortNumber)|=(1<<Loc_PinNumber);
        return OK;
    }
    else
    {
        return NOK;
    }

}

/*=========================================================== Set alternate function ========================================================*/
void Port_SetPinMode( u8 Pin, u8 Mode )
{
    u8 Loc_PinNumber;
    u8 Loc_PortNumber;
    GPIO_enumGetPinNum(Pin,&Loc_PinNumber);
    GPIO_enumGetPortNum(Pin,&Loc_PortNumber);
    GPIOLOCK(Loc_PortNumber)=GPIO_UNLOCK_VAL;
    GPIOCR(Loc_PortNumber)|=(1<<Loc_PinNumber);
    GPIOAFSEL(Loc_PortNumber)|=(1<<Loc_PinNumber);
    GPIOPCTL(Loc_PortNumber)|=(Mode<<(Loc_PinNumber*4));
}

/*=================================================== Setting Interrupt event type to some pins selected by the mask ============================================*/
Port_enumerr Port_SetInterruptEvent(u8 PortId,u8 IntEvent,u8 IntMask)
{
    if(PortId<PORTS_NUMBER)
    {
        switch(IntEvent)
        {
        case EVENT_FALL_EDGE :
            GPIOIS(PortId)&=(~(IntMask));  /*detection is configured to detect edges */
            GPIOIEV(PortId)&=(~(IntMask)); /*detection is configured to detect falling edges */
            break;
        case EVENT_RISE_EDGE :
            GPIOIS(PortId)&=(~(IntMask));
            GPIOIEV(PortId)|=(IntMask);  /*detection is configured to detect rising edges */
            break;
        case EVENT_BOTH_EDGE :
            GPIOIS(PortId)&=(~(IntMask));
            GPIOIBE(PortId)|=(IntMask);
            BothEdgesIntPinMask|=IntMask;                       /*saving the pins that are configured to sense both edges as it needs */
            break;
        case EVENT_LOW_LEVEL :
            GPIOIS(PortId)|=(IntMask);     /*detection is configured to detect levels */
            GPIOIEV(PortId)&=(~(IntMask)); /*detection is configured to detect low levels */
            break;
        case EVENT_HIGH_LEVEL :
            GPIOIS(PortId)|=(IntMask);   /*detection is configured to detect levels */
            GPIOIEV(PortId)|=(IntMask);  /*detection is configured to detect high levels */
            break;
        }
        return OK;
    }
    else
    {
        return NOK;
    }
}
/*========================================================= Clear Interrupt Flag function for a specific pin ================================================================*/
Port_enumerr Port_ClrInterruptFlag(u8 PinId)
{
    u8 Loc_PinNumber;
    u8 Loc_PortNumber;
    if(PinId<MaxPinNum)
    {
        GPIO_enumGetPinNum(PinId,&Loc_PinNumber);
        GPIO_enumGetPortNum(PinId,&Loc_PortNumber);
        GPIOICR(Loc_PortNumber)=1<<Loc_PinNumber;
        return OK;
    }
    else
    {
        return NOK;
    }
}
/*==================================================== get pin interrupt status ======================================================================*/
Port_enumerr Port_GetInterruptStatus(u8 PinId,Port_IntStatustype *IntStatusPtr)
{
    u8 Loc_PinNumber;
    u8 Loc_PortNumber;
    u8 retval;
    if(PinId<MaxPinNum)
    {
        GPIO_enumGetPinNum(PinId,&Loc_PinNumber);
        GPIO_enumGetPortNum(PinId,&Loc_PortNumber);
        retval=((GPIORIS(Loc_PortNumber))&(1<<Loc_PinNumber))>>Loc_PinNumber;
        if(retval==1)
        {
            *(IntStatusPtr)=Pending;
        }
        else
        {
            *(IntStatusPtr)=Inactive;
        }
        return OK;
    }
    else
    {
        return NOK;
    }
}
/*==================================================== Interrupt initialization function for each pin =====================================================================*/
Port_enumerr Port_PinIntInit(u8 PinId,u8 IntEvent,u8 priority)
{
    u8 Loc_PinNumber;
    u8 Loc_PortNumber;
    if(PinId<MaxPinNum)
    {
        GPIO_enumGetPinNum(PinId,&Loc_PinNumber);
        GPIO_enumGetPortNum(PinId,&Loc_PortNumber);
        /*to avoid false interrupt we follow the steps in the datasheet*/
        GPIOIM(Loc_PortNumber)=0x00;                                   /*first we disable the interrupt for the port*/
        Port_SetInterruptEvent(Loc_PortNumber,IntEvent,1<<Loc_PinNumber);      /*selecting the interrupt event,the mask here is 1<<(Pin I want to select)*/
        GPIOICR(Loc_PortNumber)=0xFF;                                  /*clearing GPIORIS by setting GPIOICR*/
        Port_SetIntPriority(Loc_PortNumber,priority);                  /*setting port priority*/
        //ENABELE PORT'S INTERRUPT
        SET_BIT(EN0,PortIntNum[Loc_PortNumber]);
        return OK;
    }
    else
    {
        return NOK;
    }
}

/*==================================================== Interrupt set priority function for each port ==============================================================*/
Port_enumerr Port_SetIntPriority(u8 PortId,u8 Priority)
{
    if(PortId<PORTS_NUMBER)
    {
        if(PortId<=3)
        {
            PRI0=Priority<<PRIReg_BitShifts[PortId];
        }
        else if(PortId<=4)
        {
            PRI1=Priority<<PRIReg_BitShifts[PortId];
        }
        else
        {
            PRI7=Priority<<PRIReg_BitShifts[PortId];
        }
        return OK;
    }
    else
    {
        return NOK;
    }
}
/*=================================================Interrupt enable/Disable=========================================================================*/
Port_enumerr Port_IntEnable(u8 PinId)
{
    u8 Loc_PinNumber;
    u8 Loc_PortNumber;
    if(PinId<MaxPinNum)
    {
        GPIO_enumGetPinNum(PinId,&Loc_PinNumber);
        GPIO_enumGetPortNum(PinId,&Loc_PortNumber);
        GPIOIM(Loc_PortNumber)|=1<<Loc_PinNumber;
        return OK;
    }
    else
    {
        return NOK;
    }
}
Port_enumerr Port_IntDisable(u8 PinId)
{
    u8 Loc_PinNumber;
    u8 Loc_PortNumber;
    if(PinId<MaxPinNum)
    {
        GPIO_enumGetPinNum(PinId,&Loc_PinNumber);
        GPIO_enumGetPortNum(PinId,&Loc_PortNumber);
        GPIOIM(Loc_PortNumber)&=~(1<<Loc_PinNumber);
        return OK;
    }
    else
    {
        return NOK;
    }
}
/*================================================PORTA _ISR=================================================================================*/
void PORTA_ISR(void)
{
    u8 local_Int_Pin;
    u8 local_bit_num;
    u8 local_GPIOIBE_status;
    static u8 local_arr_counter[PORTA_PIN_NUM]={0};
    for(local_bit_num=0;local_bit_num<PinsPerPort[PORTA];local_bit_num++)
    {
        local_GPIOIBE_status=(BothEdgesIntPinMask&(1<<local_bit_num));              /*This will allow me to know which pins are configured to sense both edges*/
        local_Int_Pin=(GPIORIS(PORTA)&(1<<local_bit_num));                          /*This will allow me to know which pin has fired the interrupt*/
        if(local_Int_Pin==1) //then an event from porta pin0 has occured
        {
            if(local_GPIOIBE_status==1) //then the GPIOIBE is enabeled for this pin
            {
                if(local_arr_counter[local_bit_num]==0)
                {
                    PA_PIN0_ISR1();
                    local_arr_counter[local_bit_num]=1;
                    SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
                }
                else if(local_arr_counter[local_bit_num]==1)
                {

                    PA_PIN0_ISR2();
                    local_arr_counter[local_bit_num]=0;
                    SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
                }
            }
            else
            {

                PA_PIN0_ISR1();
                SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE

            }
        }
        if(local_Int_Pin==2) //then an event from porta pin1 has occured
        {
            /*
             * write the code here then don't forget to set the corresponding pin in the GPIOICR in case of edge triggered
             */
        }
        if(local_Int_Pin==4) //then an event from porta pin2 has occured
        {
            /*
             * write the code here then don't forget to set the corresponding pin in the GPIOICR in case of edge triggered
             */
        }
        if(local_Int_Pin==8) //then an event from porta pin3 has occured
        {
            /*
             * write the code here then don't forget to set the corresponding in the GPIOICR in case of edge triggered
             */
        }
        if(local_Int_Pin==16) //then an event from porta pin4 has occured
        {
            /*
             * write the code here then don't forget to set the corresponding in the GPIOICR in case of edge triggered
             */
        }
        if(local_Int_Pin==32) //then an event from porta pin5 has occured
        {
            if(local_GPIOIBE_status==32) //then the GPIOIBE is enabeled for this pin
            {
                if(local_arr_counter[local_bit_num]==0)
                {
                    PA_PIN5_ISR1();
                    local_arr_counter[local_bit_num]++;
                    SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
                }
                else if(local_arr_counter[local_bit_num]==1)
                {
                    PA_PIN5_ISR2();
                    local_arr_counter[local_bit_num]=0;
                    SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
                }
            }
            else //THEN IBE IS NOT ENABELED FOR THIS PIN
            {

                PA_PIN5_ISR1();
                SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE

            }
        }
        if(local_Int_Pin==64) //then an event from porta pin6 has occured
        {
            if(local_GPIOIBE_status==64) //then the GPIOIBE is enabeled for this pin
            {
                if(local_arr_counter[local_bit_num]==0)
                {
                    PA_PIN6_ISR1();
                    local_arr_counter[local_bit_num]++;
                    SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
                }
                else if(local_arr_counter[local_bit_num]==1)
                {
                    PA_PIN6_ISR2();
                    local_arr_counter[local_bit_num]=0;
                    SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
                }
            }
            else //THEN NO IBE IS ENABELED FOR THIS PIN
            {

                PA_PIN6_ISR1();
                SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE

            }
        }
        if(local_Int_Pin==128) //then an event from porta pin7 has occured
        {
            if(local_GPIOIBE_status==128) //then the GPIOIBE is enabeled for this pin
            {
                if(local_arr_counter[local_bit_num]==0)
                {
                    PA_PIN7_ISR1();
                    local_arr_counter[local_bit_num]++;
                    SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
                }
                else if(local_arr_counter[local_bit_num]==1)
                {
                    PA_PIN7_ISR2();
                    local_arr_counter[local_bit_num]=0;
                    SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
                }
            }
            else //THEN NO IBE IS ENABELED FOR THIS PIN
            {

                PA_PIN7_ISR1();
                SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE

            }
        }
    }
}


//===================================================ISR func for each pin====================================================================================

void PORTA_PIN0_ISR(void(*Local_PIN0_ISR1)(void),void(*Local_PIN0_ISR2)(void))
{
    PA_PIN0_ISR1=Local_PIN0_ISR1;
    PA_PIN0_ISR2=Local_PIN0_ISR2;
}



void PORTA_PIN1_ISR(void(*Local_PIN1_ISR1)(void),void(*Local_PIN1_ISR2)(void))
{
    PA_PIN1_ISR1=Local_PIN1_ISR1;
    PA_PIN1_ISR2=Local_PIN1_ISR2;
}



void PORTA_PIN2_ISR(void(*Local_PIN2_ISR1)(void),void(*Local_PIN2_ISR2)(void))
{
    PA_PIN2_ISR1=Local_PIN2_ISR1;
    PA_PIN2_ISR2=Local_PIN2_ISR2;
}



void PORTA_PIN3_ISR(void(*Local_PIN3_ISR1)(void),void(*Local_PIN3_ISR2)(void))
{
    PA_PIN3_ISR1=Local_PIN3_ISR1;
    PA_PIN3_ISR2=Local_PIN3_ISR2;
}



void PORTA_PIN4_ISR(void(*Local_PIN4_ISR1)(void),void(*Local_PIN4_ISR2)(void))
{
    PA_PIN4_ISR1=Local_PIN4_ISR1;
    PA_PIN4_ISR2=Local_PIN4_ISR2;
}



void PORTA_PIN5_ISR(void(*Local_PIN5_ISR1)(void),void(*Local_PIN5_ISR2)(void))
{
    PA_PIN5_ISR1=Local_PIN5_ISR1;
    PA_PIN5_ISR2=Local_PIN5_ISR2;
}



void PORTA_PIN6_ISR(void(*Local_PIN6_ISR1)(void),void(*Local_PIN6_ISR2)(void))
{
    PA_PIN6_ISR1=Local_PIN6_ISR1;
    PA_PIN6_ISR2=Local_PIN6_ISR2;
}



void PORTA_PIN7_ISR(void(*Local_PIN7_ISR1)(void),void(*Local_PIN7_ISR2)(void))
{
    PA_PIN7_ISR1=Local_PIN7_ISR1;
    PA_PIN7_ISR2=Local_PIN7_ISR2;
}


