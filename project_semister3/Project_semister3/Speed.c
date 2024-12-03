#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "MOTOR.h"
#include "Speed.h"

// Global Variable
volatile int16_t counter = 0; // Counter variable, signed for increasing and decreasing

int gg;

// Define states
#define IDLE 0
#define FB_MODE 1
#define RB_MODE 2

volatile int stateF = 0;    // Current state of operation


// Button Pins
#define BUTTON_INCREASE_PIN PIND2 // Increase counter button (connected to PD2)
#define BUTTON_DECREASE_PIN PIND3 // Decrease counter button (connected to PD3)

// Button Pins
#define BUTTON_LEFT_PIN  PINB0  // Move left button (connected to PB0)
#define BUTTON_RIGHT_PIN PINB1  // Move right button (connected to PB1)

// Constants
#define COUNTER_MAX 5  // Maximum value for the counter
#define COUNTER_MIN -5 // Minimum value for the counter

void setup_interrupts1() {
	// Configure button pins as input
	DDRD &= ~(1 << BUTTON_INCREASE_PIN); // Set PD2 as input
	DDRD &= ~(1 << BUTTON_DECREASE_PIN); // Set PD3 as input
	
	// Configure button pins as input
	DDRB &= ~((1 << BUTTON_LEFT_PIN) | (1 << BUTTON_RIGHT_PIN)); // Set PB0 and PB1 as input

	// Enable pull-up resistors for buttons
	PORTD |= (1 << BUTTON_INCREASE_PIN);
	PORTD |= (1 << BUTTON_DECREASE_PIN);
	
	// Enable pull-up resistors for buttons
	PORTB |= (1 << BUTTON_LEFT_PIN) | (1 << BUTTON_RIGHT_PIN);

	// Configure external interrupts
	EICRA |= (1 << ISC01) | (1 << ISC00); // Rising edge for INT0 (PD2)
	EICRA |= (1 << ISC11) | (1 << ISC10); // Rising edge for INT1 (PD3)
	EIMSK |= (1 << INT0) | (1 << INT1);   // Enable INT0 and INT1 interrupts
	
	// Enable pin change interrupts for PB0 and PB1
	PCICR |= (1 << PCIE0);         // Enable pin change interrupt for PCINT[7:0] (Port B)
	PCMSK0 |= (1 << PCINT0) | (1 << PCINT1); // Enable interrupt for PB0 (PCINT0) and PB1 (PCINT1)

	// Enable global interrupts
	sei();
}



// Interrupt Service Routine for pin change interrupt on Port B
ISR(PCINT0_vect) {
	// Check which button was pressed
	if (!(PINB & (1 << BUTTON_LEFT_PIN))) {
		// Left button pressed
	    stateF = RB_MODE; // Set state to RB_MODE
		gg = 1;			
		} else if (!(PINB & (1 << BUTTON_RIGHT_PIN))) {
		// Right button pressed
		stateF = RB_MODE; // Set state to RB_MODE
		gg = 2;			
	    }
}

// Interrupt Service Routine for INT0 (Increase counter)
ISR(INT0_vect) {
	if (counter < COUNTER_MAX) {
		counter++;
		stateF = FB_MODE; // Set state to FB_MODE
	}
}

// Interrupt Service Routine for INT1 (Decrease counter)
ISR(INT1_vect) {
	if (counter > COUNTER_MIN) {
		counter--;
		stateF = FB_MODE; // Set state to FB_MODE
	}
}

// This is the gear shifting base on the button forward and backward
// it move forward from 1 to 5 with different speed and for -1 to -5 in backward on different speed 
// note this motor driver already had 5V on PWM pin of its own
// so for controlling the speed higher speed can be achieve by lower PWM power and for lower vis versa
void FBfunction() {
	
	if (stateF != FB_MODE) return; // Only execute if in FB_MODE

	switch (counter){
		case 0 : 
		        motorOff();
				speedMotors = 255;
				break;
		case 1 :
		        motorOn(); 
				PORTD &= ~(1<<PORTD4);
		        speedMotors = 252;
				forward();
				break;
		case 2 :
		        motorOn();
				speedMotors = 249;
				forward();
				break;
		case 3:
		        motorOn(); 
				speedMotors = 246;
				forward();
				break;
		case 4:
		        motorOn(); 
				speedMotors = 243;
				forward();
				break;
		case 5:
		        motorOn(); 
				speedMotors = 240;
				forward();
				break;
        case -1:
		        motorOn();
        		speedMotors = 252;
        		backward();
        		break;
		case -2:
		        motorOn();
				speedMotors = 249;
				backward();
				break;
		case -3:
		        motorOn();
				speedMotors = 246;
				backward();
				break;
		case -4:
		        motorOn();
				speedMotors = 243;
				backward();
				break;
		case -5:
		        motorOn();
				speedMotors = 240;
				backward();
				break;
	}
}


void RBfunction (){
	
	    if (stateF != RB_MODE) return; // Only execute if in RB_MODE
	
	switch (gg){
		case 1 : 
					motorOn();
					PORTD &= ~(1<<PORTD4);
		            leftTank();
					counter = 0;
					break;
		case 2 :
					motorOn();
					PORTD &= ~(1<<PORTD4);
		            rightTank();
					counter = 0;
					break;
	}
}

void statemachine(){
			// Call appropriate function based on state
			if (stateF == FB_MODE) {
				FBfunction();
				//stateF = 0;
				} else if (stateF == RB_MODE) {
				RBfunction();
				//stateF = 0;
			}
}
