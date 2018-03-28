/* arduino-stepper-motor-lcd main-cli.ino
 *
 * See https://github.com/yosyp/arduino-stepper-motor-lcd for parts list and
 * other information.
 *
 * Using Adafruit Motor/Stepper/Servo Shield v2 and AccelStepper library, this
 * code controls four NEMA-17 stepper motors using a command line interface.
 *
 * Yosyp Schwab
 */

/* TODO:
 * - Implement CLI like this:
      https://www.norwegiancreations.com/2018/02/creating-a-command-line-interface-in-arduinos-serial-monitor/ 
 */ 

#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

Adafruit_MotorShield AFMSbot(0x61); // Rightmost jumper closed
Adafruit_MotorShield AFMS1(0x60); // Default address, no jumpers
Adafruit_MotorShield AFMS2(0x59); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the both shields
Adafruit_StepperMotor *myStepper[] = {
  AFMS1.getStepper(200, 1),
  AFMS1.getStepper(200, 2),
  AFMS2.getStepper(200, 1),
  AFMS2.getStepper(200, 2)  
};

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() { myStepper[0]->onestep(FORWARD, DOUBLE); }
void backwardstep1() { myStepper[0]->onestep(BACKWARD, DOUBLE); }
// wrappers for the second motor!
void forwardstep2() { myStepper[1]->onestep(FORWARD, DOUBLE); }
void backwardstep2() { myStepper[1]->onestep(BACKWARD, DOUBLE); }
// wrappers for the third motor!
void forwardstep3() { myStepper[2]->onestep(FORWARD, DOUBLE); }
void backwardstep3() { myStepper[2]->onestep(BACKWARD, DOUBLE); }
// wrappers for the third motor!
void forwardstep4() { myStepper[3]->onestep(FORWARD, DOUBLE); }
void backwardstep4() { myStepper[3]->onestep(BACKWARD, DOUBLE); }

// Now we'll wrap the 4 steppers in an AccelStepper object
AccelStepper *stepper[4];

void setup() {
  stepper[0] = new AccelStepper(forwardstep1,backwardstep1);
  stepper[1] = new AccelStepper(forwardstep2,backwardstep2);
  stepper[2] = new AccelStepper(forwardstep3,backwardstep3);
  stepper[3] = new AccelStepper(forwardstep4,backwardstep4);
  
  for (int i=0;i<3;i++) {
    stepper[i]->setMaxSpeed(100.0);
    stepper[i]->setAcceleration(10000.0);
    stepper[i]->moveTo(14000000);
  }        
  
  AFMSbot.begin(); // Start the bottom shield
  AFMS1.begin(); // Start the middle shield
  AFMS2.begin(); // Start the top shield
}

void loop() {

  int sys = digitalRead(6);
int sys2 = digitalRead(5);

  for (int i=0; i<2; i++) {
    if (stepper[i]->distanceToGo() == 0) {
      stepper[i]->setCurrentPosition(0);
      stepper[i]->moveTo(100);
    }
    if (sys == HIGH) {
  if(sys2 == HIGH) {
    stepper[i]->setSpeed(50.0);
}
      stepper[i]->run();
    } else {
      stepper[i]->setSpeed(0);
    }
  }
}
