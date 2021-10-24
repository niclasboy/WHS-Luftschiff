#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <Arduino_FreeRTOS.h>
#include <RingBuffer.h>
#include <avr/pgmspace.h>

RingBuffer<uint8_t, 64> rxIn;
byte rxDataIn[18];
uint8_t currentBlinkCode = 0; //Fast Blink (200ms on, 200ms off) = NO DATA ; Short Blink (50ms on, 950ms off) = DATA - Ready to Fly ; 
bool failSafe = true; // Wenn true dann kein Signal vom Reciver

#define Throttle 0      //Throttel Kanal
#define Pitch 1         //Pitch Kanal
#define Yaw 2           //Yaw Kanal
#define Roll 3          //Roll Kanal

#define Mode -1         //Reserviert für Modi bsp. Pitch/Roll Stabilizer
#define Lights -1       //Reserviert Beleuchtung einschalten 
#define Hover -1    //Reserviert Anteiliger Hovermodus
