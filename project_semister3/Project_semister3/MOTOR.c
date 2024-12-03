#include <avr/interrupt.h>
#include <avr/io.h>
#include "MOTOR.h"
#include "Speed.h"




void motor_setup()
{
	// PD5 OCR0B-> PWM for left (timer 0)
	// PD6 OCR0A-> PWM for right (timer 0)
	// PD4 is for controlling direction
	// PD7 is for switching on/off

	DDRD |= (1 << DDD4) | (1 << DDD5) | ( 1<< DDD6) | (1<< DDD7);
	DDRB |= (1 << DDB4) | (1 << DDB5);				
	TCCR0A |= (1 << WGM01) | (1 << WGM00);			 // Fast PWM
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	TCCR0B |= (1 << CS02);							// clk I/O 256 pr e-scaler
	OCR0B = 255;
	OCR0A = 255;	
}

void forward()
{
	//PORTD |= (1<<PORTD4);
	//PORTD &= ~(1<<PORTD4);
	OCR0A = speedMotors;
	OCR0B = speedMotors;
	
}

void backward()
{
	//PORTD &= ~(1<<PORTD4);
	PORTD |= (1<<PORTD4);
	OCR0A = speedMotors;
	OCR0B = speedMotors;
}

void leftTank()
{
	OCR0A = 247;
	OCR0B = OFF;
}


void rightTank()
{
	OCR0A = OFF;
	OCR0B = 247;
}



void motorOn()
{
  
    PORTD |= (1<<PORTD7);

}

void motorOff()
{
	PORTD &= ~ (1<<PORTD7);
}
