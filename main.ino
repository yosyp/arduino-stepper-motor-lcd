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
#include <Wire.h>

float max_v = 10000.0; // Maximum stepper motor velocity
float max_a = 10000.0; // Maximum stepper motor acceleration

int PotPinL = A0;  // Pin for left potentiometer knob (analog)
int PotPinR = A1;  // Pin for right potentionmeter knob (analog)
int SysSwitch = 6; // Pin for system toggle switch, turns motors on/off (digital)
int DirSwitch = 5; // Pin for direction toggle switch, same or opposite directions (digital)

int calibrationTime = 7500; // Startup calibration time before main sequence (milliseconds)
int potRMax = 1;
int potRMin = 100;
int potLMax = 1;
int potLMin = 100;


int SysRead, DirRead;
int PotReadL, PotReadR;

int direction = 0; // Will store rotation direction of two motors, passed to
                   // void printArrows()

int SysState = HIGH, DirState = HIGH;
int SysPrev = LOW, DirPrev = LOW;

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // Initialize LCD pins

Adafruit_MotorShield AFMSL = Adafruit_MotorShield(0x61);
Adafruit_MotorShield AFMSR = Adafruit_MotorShield(0x60);

Adafruit_StepperMotor *myStepperL = AFMSL.getStepper(200, 1);
Adafruit_StepperMotor *myStepperR = AFMSR.getStepper(200, 2);


void forwardStepL() { myStepperL->onestep(FORWARD, DOUBLE); }
void forwardStepR() { myStepperR->onestep(FORWARD, DOUBLE); }
void backwardStepL() { myStepperL->onestep(BACKWARD, DOUBLE); }
void backwardStepR() { myStepperR->onestep(BACKWARD, DOUBLE); }

AccelStepper stepperL(forwardStepL, backwardStepL);
AccelStepper stepperR(forwardStepR, backwardStepR);

void setup() {
  //  Serial.begin(57600);
  AFMSL.begin(); // Start the bottom shield
  AFMSR.begin(); // Start the top shield
  TWBR = ((F_CPU /400000l) - 16) / 2; // Change the i2c clock to 400KHz

  stepperL.setMaxSpeed(max_v);
  stepperR.setMaxSpeed(max_v);

  stepperL.setAcceleration(max_a);
  stepperR.setAcceleration(max_a);

  //  stepperL.moveTo(50000);
  //  stepperR.moveTo(50000);  
  
  pinMode(SysSwitch, INPUT); // Global motor on/off switch
  pinMode(DirSwitch, INPUT); // Motor direction switch
  pinMode(PotPinL, INPUT);   // Left potentiometer knob
  pinMode(PotPinR, INPUT);   // Right potentiometer knob

  /*
   * Set up the LCD's number of columns and rows.
   * LCD Layout is as follows:
   * ********************************************

   *  COL: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
   * ROW0: -  >  -  >  -     S  T  O  P     -  >  -  >  -
   * ROW1: L  :  x  x  R  P  M  /  /  R  :  x  x  R  P  M
   * ********************************************
   */
  lcd.begin(16, 2);
  lcd.setCursor(0, 1);  lcd.print("L:");
  lcd.setCursor(9, 1);  lcd.print("R:");
  lcd.setCursor(4, 1);  lcd.print("rpm");
  lcd.setCursor(13, 1); lcd.print("rpm");
}

void loop() {

  calibratePotentiometers();
  
  /*
   * Read the current system state (motors ON or OFF), and
   * switch state accordingly. Save previous state to compare.
   */
  SysRead = digitalRead(SysSwitch);
  if (SysRead == HIGH && SysPrev == LOW) {
    if (SysState == HIGH) {
      SysState = LOW;
    } else {
      SysState = HIGH;
    }
    lcd.setCursor(6, 0);
    lcd.print(" ON ");
  } else if (SysRead == LOW && SysPrev == HIGH) {
    if (SysState == HIGH) {
      SysState = LOW;
    } else {
      SysState = HIGH;
    }
    lcd.setCursor(6, 0);
    lcd.print("STOP");
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
    if (DirState == HIGH) {
      DirState = LOW;
    } else {
      DirState = HIGH;
    }
    direction = 0;
  } else if (DirRead == LOW && DirPrev == HIGH) {
    if (DirState == HIGH) {
      DirState = LOW;
    } else {
      DirState = HIGH;
    }
    direction = 1;
  }
  DirPrev = DirRead;

  /*
   * Read and display current potentiometer values on LCD.
   * Values are cutoff to be between 0 and 99.
   */
  PotReadL = map(analogRead(PotPinL), potLMin, potLMax, 0, 50);
  PotReadR = map(analogRead(PotPinR), potRMin, potRMax, 0, 100);
  lcd.setCursor(2, 1);  lcd.print("  ");
  lcd.setCursor(2, 1);  lcd.print(PotReadL);
  lcd.setCursor(11, 1); lcd.print("  ");
  lcd.setCursor(11, 1); lcd.print(PotReadR);

  lcd.setCursor(7, 1);
  if ((millis() / 500) % 2 == 0) {
    lcd.print("<>");
  } else {
    lcd.print("><");
  }

  printArrows(direction);

  stepperR.moveTo(1000000);
  stepperR.setSpeed(PotReadR);
  stepperR.runSpeedToPosition();
  /* if(PotReadR > 0) { */
  /*   stepperR.setSpeed(PotReadR); */
  /*   stepperR.run(); */
  /* } */
  //  stepperL.run();
  //stepperR.run();  
}

/*
 *
 */
void calibratePotentiometers() {
  while(millis() < calibrationTime) {
    lcd.setCursor(0,0); lcd.print("CALIBRATION");
    PotReadR = analogRead(PotPinR);
    PotReadL = analogRead(PotPinL);

    if (PotReadR > potRMax) {
      potRMax = PotReadR;
    }

    if (PotReadR < potRMin) {
      potRMin = PotReadR;
    }

    if (PotReadL > potLMax) {
      potLMax = PotReadL;
    }

    if (PotReadL < potLMin) {
      potLMin = PotReadL;
    }    
  }
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
      lcd.setCursor(0, 0);  lcd.print(">->->");
      lcd.setCursor(11, 0); lcd.print("->->-");
    } else {
      lcd.setCursor(0, 0);  lcd.print(">->->");
      lcd.setCursor(11, 0); lcd.print("-<-<-");
    }
  } else {
    if (direction == 0) {
      lcd.setCursor(0, 0);  lcd.print("->->-");
      lcd.setCursor(11, 0); lcd.print(">->->");
    } else {
      lcd.setCursor(0, 0);  lcd.print("->->-");
      lcd.setCursor(11, 0); lcd.print("<-<-<");
    }
  }
}

