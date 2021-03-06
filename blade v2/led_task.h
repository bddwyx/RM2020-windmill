#ifndef _LED_H
#define _LED_H

#include <STC8.H>
#define Fosc 11059200

extern void WS2812WR(unsigned char Re,unsigned char Gr,unsigned char Bl);
extern void complete();
extern void waiting();

extern void delays(unsigned int m);

extern unsigned char code arr[3][5];
extern unsigned char color,state;
extern unsigned char color_buf[3];

#endif