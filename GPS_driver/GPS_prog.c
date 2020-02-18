/*
 * GPS_ptog.c
 *
 *  Created on: Feb 3, 2020
 *      Author: Dell
 */

#include "std_types.h"
#include "GPS_int.h"
#include "UART_int.h"
#include "GPS_config.h"
#include "GPS_priv.h"
/*
 * Frame form:
 *  $xxRMC,time,status,lat,NS,lon,EW,spd,cog,date,mv,mvEW,posMode,navStatus*cs<CR><LF>     */
#define INFO_BYTES 70
#define TIME_POS 7
#define LOC_BYTES 40
#define LAT_POS 19
#define LONG_POS 32
#define SPD_POS 46
#define DATE_POS 58
#define LAT_SIZE 11
#define LONG_SIZE 12
#define DATA_SIZE 1000
#define TIME_SIZE 10
#define SPD_SIZE 8
#define DATE_SIZE 7
static u8 Lati[LAT_SIZE]={0};
static u8 Long[LONG_SIZE]={0};
static u8 data[DATA_SIZE]={0};
static u8 time[TIME_SIZE]={0};
static u8 spd[SPD_SIZE]={0};
static u8 date[DATE_SIZE]={0};
static u8 flag=0;
static GPS_status status;
static u16 comm_pos=0;
void (* const GPS_ptr)(void)=GPS_vidprivframehandling;
/*ptr to func which is must be called after uart reception is finished,it's a parameter of uart channel configuration*/
void GPS_init(void)
{
    status=idle;
    Lati[LAT_SIZE]={0};
    Long[LONG_SIZE]={0};
    data[DATA_SIZE]={0};
    time[TIME_SIZE]={0};
    spd[SPD_SIZE]={0};
    date[DATE_SIZE]={0};
    flag=0;
}

/************************************************************/
enumRequestStatus GPS_RequestToHandle(void)
{
    enumRequestStatus request_status=Req_notaccepted;
    if(status==idle) /*if gps is in idle status , accept the request and rise the flag.*/
    {
        flag=1; /*Rise the flag*/
        status=suspended; /*Change the GPS status*/
        request_status=Req_accepted; /*return that the request is accepted*/
    }
    return(request_status);

}
/************************************************************/
void GPS_main(void)
{
    u8 i=0;
    if (flag==1)
    {
        while( UART_enumRequestToRecieve (GPS_CH, 1000, data) && i<20 ) /* i is a counter to avoid stucking , as long as the request is denied and i is less than 20
                                                                          , keep trying. it calls the reception function of UART and gives it the GPS channel ,
                                                                          length of data , and pointer to the buffer*/
        {
            i++;
        }
    }
}

void GPS_vidprivframehandling(void)
{
    //enumPrivFrameCheck check=Not_completed;
    status=busy;
    u16 loopIndex=0;
    for(loopIndex=0; loopIndex<1000; loopIndex++)
    {
        if((data[loopIndex]=='$') && (data[loopIndex+1]=='G') && (data[loopIndex+2]=='N') && (data[loopIndex+3]=='R'))
        {
            GPS_vidPrivTime(loopIndex);
            GPS_vidPrivLatLong(loopIndex);
            GPS_vidPrivSpeed(loopIndex);
            GPS_vidPrivDate(loopIndex);
            // check=completed;
            status=idle;
            flag=0;
            break;
        }

    }

    //return(check);
}
void GPS_vidPrivTime(u8 loopIndex)
{
    u8 i;
    u16 timeIndex=loopIndex+TIME_POS;
    for (i=0; i<TIME_SIZE-1; i++)
    {
        time[i]=data[timeIndex+i];
        /*assign the TIME by data array , which has the required frame ,  TIME is 9 char :hhmmss.ss*/
    }
}
void GPS_vidPrivLatLong(u8 loopIndex)
{
    u8 i;
    u16 latIndex=loopIndex+LAT_POS;
    u16 longIndex=loopIndex+LONG_POS;
    for (i=0; i<LAT_SIZE-1; i++)
    {
        Lati[i]=data[latIndex+i];
        /*assign the latitude by data array , which has the required frame ,  lat is 10 char :ddmm.mmmmm*/
        Long[i]=data[longIndex+i];
        /*assign the longitude by data array , which has the required frame ,  long is 11 char :dddmm.mmmmm*/
    }
    Long[i]=data[longIndex+i];
    /*to assign the last digit in longitude as it's greater by 1 thn latitude*/


}
void GPS_vidPrivSpeed(u8 loopIndex)
{
    u8 i=0;
    u16 spdIndex=loopIndex+SPD_POS;
    while(data[spdIndex+i] != ',') /*As speed form is not constant as previous parameters*/
    {
        spd[i]=data[spdIndex+i];
        /*assign the SPEED by data array , which has the required frame ,  SPEED is 7 numeric 000.000(almost)*/
        i++;
    }
    comm_pos=spdIndex+i; /*it helps to get the next data positions in the frame*/


}
void GPS_vidPrivDate(u8 loopIndex)
{
    u8 i=1;
    u16 dateindex=0;
    while (data[comm_pos+i] != ',') /*Search for the next comma to get th position of date*/
    {
        i++;
    }
    dateindex=comm_pos+i+1;
    for (i=0; i<DATE_SIZE-1; i++)
    {
        date[i]=data[dateindex+i];
        /* assign the date by data array , which has the required frame ,  date is 6 numeric ddmmyy*/
    }
}
void GPS_enumGetData(u8 * Latitude , u8 * Longitude , u8 * TIME , u8 * speed ,u8 * DATE)
{
    Latitude=Lati;
    Longitude=Long;
    TIME=time;
    speed=spd;
    DATE=date;
}



