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
typedef enum{
    Req_accepted,
    Req_notaccepted
}enumRequestStatus;
void GPS_init(void);
void GPS_main(void);

enumRequestStatus GPS_RequestToHandle(void);

void GPS_enumGetData(u8 * Latitude , u8 * Longitude , u8 * TIME , u8 * speed ,u8 * DATE); // latitude must be 11 :ddmm.mmmmm, longitude must be 12:dddmm.mmmmm ,
                                                                                          // time must be 9 , hhmmss.ss

#endif /* GPS_INT_H_ */
