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
#include <LiquidCrystal.h>
#include <ResponsiveAnalogRead.h>
#include <Wire.h>

#define MAX_POTS 2

/* LCD pins depend on physical configuration */
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

/* Analog IO pins */
int PotentiometerPin[] = { A0, A1 }; // Potentiometer values (analog)

/* Placeholders to store calibrated min/max of potentiometers */
int potMax[] = {1, 1};
int potMin[] = {100, 100};

/* */
int PotRead[MAX_POTS];
int stepperSpeed[MAX_POTS];

/* Digital IO pins */
int SysSwitch = 6; // Pin for system toggle switch, motors on/off (digital)
int DirSwitch = 5; // Pin for direction toggle switch (digital)

int direction = 0; // Will store rotation direction of two motors, passed to
                   // void printArrows()

int SysState, DirState; // Default state for system is off, motors not moving.
int SysPrev = HIGH,
    DirPrev = LOW; // Default direction for two motors is both forward.

int SysRead, DirRead;
int calibrationTime = 7500; // Startup calibration time before main loop (ms)

int stepperSteps = 200; // Physical value of the stepper motor
float max_v = 100.0;    // Maximum stepper motor velocity (steps)
float max_a = 100.0;    // Maximum stepper motor acceleration (steps)

ResponsiveAnalogRead *pot[MAX_POTS];
//L(PotPinL, true);
//ResponsiveAnalogRead potR(PotPinR, true);

Adafruit_MotorShield AFMSbot(0x61);
Adafruit_MotorShield AFMStop(0x60);

/* Notice that we define two MotorShield variables, but for the steppers
 * only use AFMStop, which is the top shield. If stacking multiple motor
 * shields, remember to map the steppers to the approapriate shield.
 */
Adafruit_StepperMotor *myStepper[] = {
  AFMStop.getStepper(200, 1),
  AFMStop.getStepper(200, 2)
};

void forwardStep0() { myStepper[0]->onestep(FORWARD, DOUBLE); }
void forwardStep1() { myStepper[0]->onestep(FORWARD, DOUBLE); }
void backwardStep0() { myStepper[1]->onestep(BACKWARD, DOUBLE); }
void backwardStep1() { myStepper[1]->onestep(BACKWARD, DOUBLE); }

AccelStepper *stepper[MAX_POTS];

void loop() {

  // Calibrate the potentiometers by manually finding min/max values
  while (millis() < calibrationTime) {
    calibratePotentiometers();
  }

  /*
   * Update the Responsive AnalogRead objects at every loop.
   * It stores the last few values and eliminates circuit noise from analogue
   * values.
   */
  for (int i=0; i<MAX_POTS; i++) {
    pot[i]->update();
  }

  /*
   * Read the current system state (motors ON or OFF), and
   * switch state accordingly. Save previous state to compare.
   */
  SysRead = digitalRead(SysSwitch);
  if (SysRead == HIGH && SysPrev == LOW) {
    SysState = LOW;
    lcd.setCursor(5, 0);
    lcd.print(" STOP");
  } else if (SysRead == LOW && SysPrev == HIGH) {
    SysState = HIGH;
    lcd.setCursor(5, 0);
    lcd.print("  ON  ");
  }
  SysPrev = SysRead;

  /*
   * Read the current motor direction state (can be either
   * both in the same direction, or rotate in opposite
   * directions). Switch states accordingly and save previous
   * state to compare.
   */
  DirRead = digitalRead(DirSwitch);
  if (DirRead == HIGH && DirPrev == LOW) {
    DirState = LOW;
    direction = 0;
  } else if (DirRead == LOW && DirPrev == HIGH) {
    DirState = HIGH;
    direction = 1;
  }
  DirPrev = DirRead;

  /*
   * Read and display current potentiometer values on LCD.
   * Values are cutoff to be between 0 and 99.
   */
  for (int i=0; i<MAX_POTS; i++) {
    // Remaps min/max potentiometer values to stepper steps
    PotRead[i] = map(pot[i]->getValue(), potMin[i], potMax[i], 1, max_v);

    // In case sensor value is out of range, constrain it.
    PotRead[i] = constrain(PotRead[i], 1, max_v);

    // speed() returns steps/second, convert to RPM:
    stepperSpeed[i] = (stepper[i]->speed() / stepperSteps) * 60;    
  }

  /* TODO: 
   *  fix this using a struct to store positions of cursors for each
   *  stepper 
   */ 
  /* lcd.setCursor(1, 1); */
  /* lcd.print("  "); */
  /* lcd.setCursor(1, 1); */
  /* lcd.print(stepperLSpeed); */
  /* lcd.setCursor(10, 1); */
  /* lcd.print("  "); */
  /* lcd.setCursor(10, 1); */
  /* lcd.print(stepperRSpeed); */

  printArrows(direction);

  for (int i = 0; i<MAX_POTS; i++) {

    if (SysState == LOW) {
      stepper[i]->setSpeed(0);
    } else {
      if (stepper[i]->distanceToGo() == 0) {
        stepper[i]->moveTo(-stepper[i]->currentPosition());
      }
      stepper[i]->run();

      for (int i=0; i<MAX_POTS; i++) {
        if (PotRead[i] <= 1) {
          stepper[i]->setSpeed(0);
        }
      }

    }
  }

  
}

/*
 *
 */
void calibratePotentiometers() {
  lcd.setCursor(0, 0);
  lcd.print("CALIBRATION");
  PotRead[MAX_POTS];

  for (int i=0; i<MAX_POTS; i++) {
    PotRead[i] = analogRead(PotentiometerPin[i]);
  }

  for (int i=0; i<MAX_POTS; i++) {  
    if (PotRead[i] > potMax[i]) {
      potMax[i] = PotRead[i];
    }

    if (PotRead[i] < potMin[i]) {
      potMin[i] = PotRead[i];
    }

    if (PotRead[i] > potMax[i]) {
      potMax[i] = PotRead[i];
    }

    if (PotRead[i] < potMin[i]) {
      potMin[i] = PotRead[i];
    }
  }
  lcd.setCursor(5, 0);
  lcd.print("      "); // Cleanup the "CALIBRATION" message left over.
}

/*
 * Print direction arrows on first row
 * 0 -- motors spin in same direction, [->- ->-]
 * 1 -- right motor spins in opposite direction, [->- -<-]
 */
void printArrows(int direction) {
  int ttime = millis();
  if ((ttime / 300) % 2 == 0) {
    if (direction == 0) {
      lcd.setCursor(0, 0);
      lcd.print(">->->");
      lcd.setCursor(11, 0);
      lcd.print("->->-");
    } else {
      lcd.setCursor(0, 0);
      lcd.print(">->->");
      lcd.setCursor(11, 0);
      lcd.print("-<-<-");
    }
  } else {
    if (direction == 0) {
      lcd.setCursor(0, 0);
      lcd.print("->->-");
      lcd.setCursor(11, 0);
      lcd.print(">->->");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("->->-");
      lcd.setCursor(11, 0);
      lcd.print("<-<-<");
    }
  }
  lcd.setCursor(7, 1);
  if ((ttime / 500) % 2 == 0) {
    lcd.print("<>");
  } else {
    lcd.print("><");
  }
}
