/* 
 * File:   export.h
 * Author: bocal
 *
 * Created on July 27, 2018, 3:16 PM
 */

#ifndef EXPORT_H
#define	EXPORT_H

extern u8  devicePowered;
extern u16 countTime;
extern u8  countTimeEnable;
extern unsigned char  rst, on_off, gps;
extern unsigned short gpsTmp;

extern u8 powerOnProcess;
extern u8 powerOffProcess;

/* Calibrate MAG3110 */
extern s16	x_min, x_max, y_min, y_max, offset_x, offset_y;

extern float x_scale, y_scale;

extern s32 TimerCalMode;
extern u8 IsCalMode;

extern s16 mag_x;
extern s16 mag_y;

#endif

