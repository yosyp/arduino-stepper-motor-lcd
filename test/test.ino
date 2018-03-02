// Shows how to run three Steppers at once with varying speeds
//
// Requires the Adafruit_Motorshield v2 library
//   https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
// And AccelStepper with AFMotor support
//   https://github.com/adafruit/AccelStepper

// This tutorial is for Adafruit Motorshield v2 only!
// Will not work with v1 shields

#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

Adafruit_MotorShield AFMSbot(0x61); // Rightmost jumper closed
Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *myStepperL = AFMStop.getStepper(200, 1);
Adafruit_StepperMotor *myStepper2 = AFMStop.getStepper(200, 2);

// Connect one stepper with 200 steps per revolution (1.8 degree)
// to the bottom shield

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() { myStepperL->onestep(FORWARD, DOUBLE); }
void backwardstep1() { myStepperL->onestep(BACKWARD, DOUBLE); }
// wrappers for the second motor!
void forwardstep2() { myStepper2->onestep(FORWARD, DOUBLE); }
void backwardstep2() { myStepper2->onestep(BACKWARD, DOUBLE); }
// wrappers for the third motor!

// Now we'll wrap the 3 steppers in an AccelStepper object
AccelStepper stepperL(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);

void setup() {

  pinMode(6, INPUT);

  AFMSbot.begin(); // Start the bottom shield
  AFMStop.begin(); // Start the top shield

  stepperL.setMaxSpeed(100.0);
  stepperL.setAcceleration(100.0);
  stepperL.moveTo(24);

  stepper2.setMaxSpeed(100.0);
  stepper2.setAcceleration(100.0);
  stepper2.moveTo(50000);
}

void loop() {

  int sys = digitalRead(6);

  // Change direction at the limits
  if (stepperL.distanceToGo() == 0)
    stepperL.moveTo(-stepperL.currentPosition());

  if (stepper2.distanceToGo() == 0)
    stepper2.moveTo(-stepper2.currentPosition());

  if (sys == HIGH) {
    stepperL.run();
    stepper2.run();
  } else {
    stepperL.setSpeed(0);
    stepper2.setSpeed(0);
  }
}
