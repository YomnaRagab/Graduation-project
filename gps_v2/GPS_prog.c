/*
 * GPS_ptog.c
 *
 *  Created on: Feb 3, 2020
 *      Author: Dell
 */


#include "GPS_int.h"
#include "UART_int.h"
#include "GPS_config"
#include "GPS_priv.h"
#define INFO_BYTES 70
#define TIME_POS 7
#define LOC_BYTES 40
#define LAT_POS 19
#define LONG_POS 32
#define SPD_POS 46
#define DATE_POS 58
u8 Lati[11]=0;
u8 Long[12]=0;
u8 data[255]=0;
u8 time[10]=0;
u8 spd[8]=0;
u8 date[7]=0;

enumPrivFrameCheck(*ptr)(void)=GPS_vidprivframehandling; // ptr to func which is must be called after uart reception is finished
void GPS_init(void)
{
    UART_init () ;
}

void GPS_main(void)
{
    u8 i=0;
    while( UART_enumRequestToRecieve (GPS_CH, 255, data) && i<20 ) // i is a counter to avoid stucking , as long as the request is denied and i is less than 20 , keep trying
    {
        i++;
    }

}

enumPrivFrameCheck GPS_vidprivframehandling(void)
{
    enumPrivFrameCheck check=completed;
    loopIndex=0;
    for(loopIndex=0; loopIndex<255; loopIndex++)
    {
        if((data[loopIndex]=='$') && (data[loopIndex+1]=='G') && (data[loopIndex+2]=='N') && (data[loopIndex+3]=='R'))
        {
            if (loopIndex <255 - INFO_BYTES) // the rest of frame is here ;)
            {
                GPS_vidPrivTime(loopIndex);
                GPS_vidPrivLatLong(loopIndex);
                GPS_vidPrivSpeed(loopIndex);
                GPS_vidPrivDate(loopIndex);
                check=completed;
            }
            else if (loopIndex <255 - LOC_BYTES)
            {
                GPS_vidPrivTime(loopIndex);
                GPS_vidPrivLatLong(loopIndex);
                check=semi_completed;
            }
            else
            {
                  check=Not_completed;
            }
            break;
        }
    }

    return(check);
}
void GPS_vidPrivTime(u8 loopIndex)
{
	u8 timeIndex=loopIndex+TIME_POS;
	for (i=0; i<9; i++)
	    {
	 time[i]=data[timeIndex+i]; // assign the TIME by data array , which has the required frame ,  TIME is 9 char :hhmmss.ss
	    }
}
void GPS_vidPrivLatLong(u8 loopIndex)
{
    u8 latIndex=loopIndex+LAT_POS;
    u8 longIndex=loopIndex+LONG_POS;
    for (i=0; i<10; i++)
    {
        Lati[i]=data[latIndex+i]; // assign the latitude by data array , which has the required frame ,  lat is 10 char :ddmm.mmmmm
        Long[i]=data[longIndex+i]; // assign the longitude by data array , which has the required frame ,  long is 11 char :dddmm.mmmmm
    }
    Long[i]=data[longIndex+i];//to assign the last digit in longitude


}
void GPS_vidPrivSpeed(u8 loopIndex)
{
	u8 spdindex=loopIndex+SPD_POS;
		for (i=0; i<7; i++)
		    {
		 spd[i]=data[spdIndex+i]; // assign the SPEED by data array , which has the required frame ,  SPEED is 7 numeric 000.000
		    }

}
void GPS_vidPrivDate(u8 loopIndex)
{
	u8 dateindex=loopIndex+DATE_POS;
		for (i=0; i<6; i++)
		  {
		 date[i]=data[dateindex+i]; // assign the date by data array , which has the required frame ,  date is 6 numeric ddmmyy
		   }
}
enumcheckerror GPS_enumGetLatLong(u8 * Latitude , u8 * Longitude , u8 * TIME , u8 * speed ,u8 * DATE)
{
    Latitude=Lati;
    Longitude=Long;
    TIME=time;
    speed=spd;
    DATE=date;
}



