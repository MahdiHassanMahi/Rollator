#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>





//#define F_CPU 16000000UL // 16 MHz clock speed
#define PRESCALER 64
#define TOP_VALUE 24999

volatile uint32_t ms = 0; // Millisecond counter
uint32_t previousMillis = 0;
uint32_t previousMillis1 = 0;





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
#define COUNTER_MAX 2  // Maximum value for the counter
#define COUNTER_MIN -2 // Minimum value for the counter

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

	// Enable pin change interrupts for PD2 and PD3 (PCINT18 and PCINT19)
	PCICR |= (1 << PCIE2);         // Enable pin change interrupt for PCINT[23:16] (Port D)
	PCMSK2 |= (1 << PCINT18) | (1 << PCINT19); // Enable interrupt for PD2 (PCINT18) and PD3 (PCINT19)
	
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



// Interrupt Service Routine for pin change interrupt on Port D (PD2 and PD3)
ISR(PCINT2_vect) {
	
	uint32_t currentMillis1 = ms;

	// Check which button was pressed
	if (!(PIND & (1 << BUTTON_INCREASE_PIN))) {
	    if (currentMillis1 - previousMillis1 >= 1500) {
			// Increase counter button pressed
			if (counter < COUNTER_MAX) {
				counter+= 1;
				stateF = FB_MODE; // Set state to FB_MODE
			}
		 previousMillis1 = currentMillis1;
		}
	}
	if (!(PIND & (1 << BUTTON_DECREASE_PIN))) {
	   if (currentMillis1 - previousMillis1 >= 1500) {
		// Decrease counter button pressed
			if (counter > COUNTER_MIN) {
				counter-= 1;
				stateF = FB_MODE; // Set state to FB_MODE
			}
		previousMillis1 = currentMillis1;
	   }
	}
}

// Interrupt Service Routine for INT0 (Increase counter)
/*ISR(INT0_vect) {
	
    uint32_t currentMillis1 = ms;

    if (currentMillis1 - previousMillis1 >= 1500) {
		if (counter < COUNTER_MAX) {
			counter++;
			stateF = FB_MODE; // Set state to FB_MODE
		}
		previousMillis1 = currentMillis1;
	}
}*/

// Interrupt Service Routine for INT1 (Decrease counter)
/*ISR(INT1_vect) {

    uint32_t currentMillis = ms;
     
	if (currentMillis - previousMillis >= 1500) {
		if (counter > COUNTER_MIN) {
			counter--;
			stateF = FB_MODE; // Set state to FB_MODE
		}
		previousMillis = currentMillis;
	}
	
}*/

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
				PORTD |=  (1<<PORTD1);
				speedMotors = 250;
				forward();
				break;
		case 2 :
				motorOn();
				PORTD &= ~(1<<PORTD4);
				PORTD |=  (1<<PORTD1);				
				speedMotors = 245;
				forward();
				break;
		case -1:				
				motorOn();
				PORTD |=  (1<<PORTD4);
				PORTD &= ~(1<<PORTD1);
				speedMotors = 250;
				backward();
				break;
		case -2:
				motorOn();
				PORTD |=  (1<<PORTD4);
				PORTD &= ~(1<<PORTD1);
				speedMotors = 245;
				backward();
				break;
	}
}





void RBfunction (){
	
	if (stateF != RB_MODE) return; // Only execute if in RB_MODE
	
	/*switch (gg){
		case 1 :
				motorOff();
				motorOn();
				PORTD &= ~(1<<PORTD4);
				PORTD |= (1<<PORTD1);
				leftTank();
				counter = 0;
				break;
		case 2 :
				motorOff();
				motorOn();
				PORTD &= ~(1<<PORTD4);
				PORTD |= (1<<PORTD1);
				rightTank();
				counter = 0;
				break;*/
	
	
	    if (gg == 1) {
		    motorOff();
		    motorOn();
		    PORTD &= ~(1 << PORTD4);
		    PORTD |= (1 << PORTD1);
		    leftTank();
		    counter = 0;
		    } else if (gg == 2) {
		    motorOff();
		    motorOn();
		    PORTD &= ~(1 << PORTD4);
		    PORTD |= (1 << PORTD1);
		    rightTank();
		    counter = 0;
	    }
	
}

void statemachine(){
	  
	
	// Call appropriate function based on state
	if (stateF == FB_MODE) {
		FBfunction();

		} else if (stateF == RB_MODE) {
		RBfunction();

	    }
}




void timer1_init() {
	ms = 0; // Initialize millisecond counter

	// Set Timer 1 to Fast PWM mode with ICR1 as top
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM13) | (1 << WGM12);

	// Set prescaler to 64
	TCCR1B |= (1 << CS11) | (1 << CS10);

	// Set TOP value for 10 Hz overflow
	ICR1 = TOP_VALUE;

	// Enable Timer 1 overflow interrupt
	TIMSK1 |= (1 << TOIE1);

}

// Timer 1 Overflow Interrupt Service Routine
ISR(TIMER1_OVF_vect) {
	ms += 100; // Increment milliseconds by 100 (0.1 s overflow)
}




