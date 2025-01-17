
#ifndef MOTOR_H
#define MOTOR_H

#include <avr/io.h>

// Definitions
#define OFF 255

int speedMotors;


void motor_setup(void); //Sets up the motor control pins and initializes the PWM for motor control.

void forward(void);     //Activates forward movement by setting appropriate direction and PWM values.

void backward(void);    //Activates backward movement by reversing the motor direction and setting PWM values.

void leftTank(void);    //Rotates the motors for a left tank turn.

void rightTank(void);   //Rotates the motors for a right tank turn.

void motorOn(void);     //Turns the motor ON by enabling the motor control switch.

void motorOff(void);    //Turns the motor OFF by enabling the motor control switch.

#endif // MOTOR_H
