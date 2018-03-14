/* arduino-stepper-motor-lcd main.ino
 *
 * See https://github.com/yosyp/arduino-stepper-motor-lcd for parts list and
 * other information.
 *
 * Using Adafruit Motor/Stepper/Servo Shield v2 and AccelStepper library, this
 * code controls two NEMA-17 stepper motors using two analog potentiometers. The
 * speed and and direction of the motors are displayed on a 16x2 LCD display.
 *
 * Yosyp Schwab
 */

#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

#define MAX_POTS 2
FILE serial_stdout;

float stepperSpeed[MAX_POTS];

float stepperSteps = 200.0; // Physical value of the stepper motor
float max_v = 10000.0;      // Maximum stepper motor velocity (steps)
float max_a = 10000.0;      // Maximum stepper motor acceleration (steps)

long int timeout = calibrationTime + 1000;

Adafruit_MotorShield AFMSbot(0x61);
Adafruit_MotorShield AFMStop(0x60);

/* Notice that we define two MotorShield variables, but for the steppers
 * only use AFMStop, which is the top shield. If stacking multiple motor
 * shields, remember to map the steppers to the approapriate shield.
 */
Adafruit_StepperMotor *myStepper[] = {AFMStop.getStepper(200, 1),
                                      AFMStop.getStepper(200, 2)};

void forwardStep0() { myStepper[0]->onestep(FORWARD, DOUBLE); }
void forwardStep1() { myStepper[1]->onestep(FORWARD, DOUBLE); }
void backwardStep0() { myStepper[0]->onestep(BACKWARD, DOUBLE); }
void backwardStep1() { myStepper[1]->onestep(BACKWARD, DOUBLE); }

AccelStepper *stepper[MAX_POTS];

void loop() {

  for (int i = 0; i < MAX_POTS; i++) {
    /*
     * Update the Responsive AnalogRead objects at every loop.
     * It stores the last few values and eliminates circuit noise from analogue
     * values.
     */
    //    pot[i]->update();

    /*
     * Read and display current potentiometer values on LCD.
     * Values are cutoff to be between 0 and 200.
     */
    // Remaps min/max potentiometer values to stepper steps
    PotRead[i] =
        map(analogRead(PotentiometerPin[i]), potMin[i], potMax[i], 1, max_v);
    //    PotRead[i] = pot[i]->getValue();
    // In case sensor value is out of range, constrain it.
    PotRead[i] = constrain(PotRead[i], 1, (int)max_v);
    PotRead[i] = round_up(PotRead[i], 5);

    // speed() returns steps/second, converted to RPM:
    stepperSpeed[i] = ((float)PotRead[i] / (float)stepperSteps) * 60.0;

    if (SysState == LOW) {
      stepper[i]->setSpeed(0);
    } else {
      stepper[i]->setSpeed(PotRead[i]);
      //      stepper[i]->setSpeed(10000.0);
      if (stepper[i]->distanceToGo() == 0) {
        stepper[i]->setCurrentPosition(0);
        stepper[i]->moveTo(100);
        // stepper[i]->moveTo(-stepper[i]->currentPosition());
      }
      stepper[i]->run();
    }
    if (PotRead[i] <= 1) {
      stepper[i]->setSpeed(0);
    }
  }



  if (millis() % 250 == 0) {
    for (int i = 0; i < MAX_POTS; i++) {
      printf("Time:%ld POT:%d Min:%d Max:%d CurrentPotValue:%d CurrentSpeed:",
             millis(), i, potMin[i], potMax[i], PotRead[i]);
      Serial.print(stepper[i]->speed());
      printf("\n");
    }
    //    timeout = millis() + 2000;
  }
}

