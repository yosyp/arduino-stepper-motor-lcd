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

Adafruit_MotorShield AFMSbot(0x60); // Rightmost jumper closed
Adafruit_MotorShield AFMStop(0x61); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *myStepper[] = {
  AFMSbot.getStepper(200, 1),
  AFMSbot.getStepper(200, 2),
  AFMStop.getStepper(200, 1),
  AFMStop.getStepper(200, 2)  
};

// Connect one stepper with 200 steps per revolution (1.8 degree)
// to the bottom shield

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() { myStepper[0]->onestep(FORWARD, INTERLEAVE); }
void backwardstep1() { myStepper[0]->onestep(BACKWARD, INTERLEAVE); }
// wrappers for the second motor!
void forwardstep2() { myStepper[1]->onestep(FORWARD, INTERLEAVE); }
void backwardstep2() { myStepper[1]->onestep(BACKWARD, INTERLEAVE); }
// wrappers for the third motor!
void forwardstep3() { myStepper[2]->onestep(FORWARD, INTERLEAVE); }
void backwardstep3() { myStepper[2]->onestep(BACKWARD, INTERLEAVE); }

void forwardstep4() { myStepper[3]->onestep(FORWARD, INTERLEAVE); }
void backwardstep4() { myStepper[3]->onestep(BACKWARD, INTERLEAVE); }
// Now we'll wrap the 3 steppers in an AccelStepper object
AccelStepper *stepper[4];

void setup() {
  pinMode(6, INPUT);
  pinMode(5, INPUT);

  stepper[0] = new AccelStepper(forwardstep1,backwardstep1);
  stepper[1] = new AccelStepper(forwardstep2,backwardstep2);
  stepper[2] = new AccelStepper(forwardstep3,backwardstep3);
  stepper[3] = new AccelStepper(forwardstep4,backwardstep4);
  
  for (int i=0;i<4;i++) {
    stepper[i]->setMaxSpeed(10000.0);
    stepper[i]->setAcceleration(1000000.0);
    stepper[i]->moveTo(14000000);
  }        
  
  AFMSbot.begin(); // Start the bottom shield
  AFMStop.begin(); // Start the top shield
}

void loop() {

      stepper[0]->setCurrentPosition(0);
      stepper[0]->moveTo(1000000);
      stepper[0]->setSpeed(45.0);
      stepper[0]->run();

      stepper[1]->setCurrentPosition(0);
      stepper[1]->moveTo(1000000);
      stepper[1]->setSpeed(45.0);
      stepper[1]->run();

      stepper[2]->setCurrentPosition(0);
      stepper[2]->moveTo(1000000);
      stepper[2]->setSpeed(45.0);
      stepper[2]->run();

      stepper[3]->setCurrentPosition(0);
      stepper[3]->moveTo(1000000);
      stepper[3]->setSpeed(45.0);
      stepper[3]->run();

}
