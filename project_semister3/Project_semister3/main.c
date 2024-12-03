#include <avr/io.h>
#include <stdbool.h>
#include "MOTOR.h"
#include "Speed.h"

#define BUTTON_LEFT     0b00000001 // PB0
#define BUTTON_RIGHT    0b00000010 // PB1




// PD2 for forward and increasing speed with the button
// PD3 for backward and decreasing speed with the button
// PB0 for moving left with the button
// PB1 for moving right with the button
// PD5 OCR0B-> PWM for left (timer 0)
// PD6 OCR0A-> PWM for right (timer 0)
// PD4 is the pin for controlling direction with the motor driver 
// PD7 is the pin for switching on/off on    


int main(void) {
	// Setup motor and speed modules
	motor_setup();
	//speed_setup();
	setup_interrupts1();

	// Button pins as input
	DDRB &= ~( BUTTON_LEFT | BUTTON_RIGHT );

	// Enable pull-up resistors for buttons
	PORTB |= ( BUTTON_LEFT | BUTTON_RIGHT );
	
	
	
	while (1) {	
		
        statemachine();
			
	}
}
