/*
 * GPS_int.h
 *
 *  Created on: Feb 3, 2020
 *      Author: Dell
 */

#ifndef GPS_INT_H_
#define GPS_INT_H_
typedef enum{
    Done=0,
    Error=1
}enumcheckerror;
void GPS_init(void);
void GPS_main(void);
enumcheckerror GPS_enumGetLatLong(u8 * Lat , u8 * Long); // lat must be 11 :ddmm.mmmmm, long must be 12:dddmm.mmmmm

enumcheckerror GPS_enumGetTime(u8 * time); //it must be 9 , hhmmss.ss

enumcheckerror GPS_enumGetSpeed(u8 speed);

enumcheckerror GPS_enumGetDate(u8 * day , u8 * month , u8 * year );

#endif /* GPS_INT_H_ */
