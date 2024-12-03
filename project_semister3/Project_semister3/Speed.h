#ifndef SPEED_H
#define SPEED_H

#include <avr/io.h>

// Global variable to store the motor speed




// Function prototypes
void setup_interrupts1(void);
void FBfunction(void);
void RBfunction (void);
void statemachine(void);

#endif // SPEED_H
