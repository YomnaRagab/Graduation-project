/*
 * GPS_priv.h
 *
 *  Created on: Feb 3, 2020
 *      Author: Dell
 */

#ifndef GPS_PRIV_H_
#define GPS_PRIV_H_

typedef enum {
    idle=0,
    suspended,
    busy
}GPS_status;

typedef enum {
    Not_completed=0,
    completed
}enumPrivFrameCheck;

void GPS_vidprivframehandling(void);
void GPS_vidPrivTime(u8 loopIndex);
void GPS_vidPrivLatLong(u8 loopIndex);
void GPS_vidPrivSpeed(u8 loopIndex);
void GPS_vidPrivDate(u8 loopIndex);

#endif /* GPS_PRIV_H_ */
