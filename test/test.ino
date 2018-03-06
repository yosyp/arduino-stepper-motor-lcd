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
Adafruit_StepperMotor *myStepper[] = {
  AFMStop.getStepper(200, 1),
  AFMStop.getStepper(200, 2)
};

// Connect one stepper with 200 steps per revolution (1.8 degree)
// to the bottom shield

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() { myStepper[0]->onestep(FORWARD, DOUBLE); }
void backwardstep1() { myStepper[0]->onestep(BACKWARD, DOUBLE); }
// wrappers for the second motor!
void forwardstep2() { myStepper[1]->onestep(FORWARD, DOUBLE); }
void backwardstep2() { myStepper[1]->onestep(BACKWARD, DOUBLE); }
// wrappers for the third motor!

// Now we'll wrap the 3 steppers in an AccelStepper object
AccelStepper *stepper[2];

void setup() {
  pinMode(6, INPUT);

  stepper[0] = new AccelStepper(forwardstep1,backwardstep1);
  stepper[1] = new AccelStepper(forwardstep2,backwardstep2);
  
  for (int i=0;i<2;i++) {
    stepper[i]->setMaxSpeed(10000.0);
    stepper[i]->setAcceleration(10000.0);
    stepper[i]->moveTo(14000000);
  }        
  
  AFMSbot.begin(); // Start the bottom shield
  AFMStop.begin(); // Start the top shield
}

void loop() {

  int sys = digitalRead(6);

  for (int i=0; i<2; i++) {
    if (stepper[i]->distanceToGo() == 0) {
      stepper[i]->setCurrentPosition(0);
      stepper[i]->moveTo(100);
    }
    if (sys == HIGH) {
      stepper[i]->run();
    } else {
      stepper[i]->setSpeed(0);
    }
  }
}
