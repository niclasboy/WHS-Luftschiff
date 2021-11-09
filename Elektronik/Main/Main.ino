#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <Arduino_FreeRTOS.h>
#include <avr/pgmspace.h>
#include "Servo.h"

//PWM Motor
#define F_CPU 16000000L                  // Systemtakt in Hz
#define F_PWM 100                       // PWM-Frequenz in Hz
#define PWM_STEPS 64                   // PWM-Schritte pro Zyklus(1..256)
#define PWM_PORT PORTA                  // Port für PWM
#define PWM_DDR DDRA                    // Datenrichtungsregister für PWM
volatile uint8_t pwm_setting[8];                    // Einstellungen für die einzelnen PWM-Kanäle
//PWM Motor
//Servos
Servo mainServo[4];
//Servos
// ab hier nichts ändern, wird alles berechnet

#define T_PWM (F_CPU/(F_PWM*PWM_STEPS)) // Systemtakte pro PWM-Takt

#if (T_PWM<(93+5))
    #error T_PWM zu klein, F_CPU muss vergrössert werden oder F_PWM oder PWM_STEPS verkleinert werden
#endif

static byte newData = 0;
byte rxDataIn[18];
uint8_t currentBlinkCode = 0; //Fast Blink (200ms on, 200ms off) = NO DATA ; Short Blink (50ms on, 950ms off) = DATA - Ready to Fly ; 
bool failSafe = true; // Wenn true dann kein Signal vom Reciver

#define ThrottleChannel 0      //Throttel Kanal
#define PitchChannel 1         //Pitch Kanal
#define YawChannel 2           //Yaw Kanal
#define RollChannel 3          //Roll Kanal

#define MinPitch -20 //Eingang kann Servo um 20° negativ Verändern
#define MaxPitch 20 //Eingang kann Servo um 20° positiv Verändern

#define HeadroomYaw 14 //Motorleistung Begrenezen um Headroom beim Yawen zu haben
#define MinYaw -20 //Abnahme der PWM Leistung um 20 "Stufen" von 64. 
#define MaxYaw 20 //Zunahme der PWM Leistung um 20 "Stufen" von 64. 

#define MinRoll -20 //Eingang kann Servo um 20° negativ Verändern
#define MaxRoll 20 //Eingang kann Servo um 20° positiv Verändern

#define Mode -1         //Reserviert für Modi bsp. Pitch/Roll Stabilizer
#define Lights -1       //Reserviert Beleuchtung einschalten 
