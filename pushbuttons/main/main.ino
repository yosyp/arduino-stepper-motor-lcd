/*
  Buttons PINOUT:
  4   Stepper 1 increase speed (up)
  5   Stepper 1 decrease speed (down)
  6   Stepper 2 increase speed (up)
  7   Stepper 2 decrease speed (down)    
  A3  Stepper 3 increase speed (up) addressed at 17
  A1  Stepper 3 decrease speed (down) addressed as 15
  A2  Stepper 4 increase speed (up) addressed at 16
  A0  Stepper 4 decrease speed (down) addressed at 14
*/
#include <LiquidCrystal.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

/******************* LCD Variables *******************************/
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
char lcd_string[16];
char serial_buf[50];
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/******************** Pushbutton Variables *********************/
const int buttonPin[4][2] = {
  {4, 6},   // stepper 1 up/down button
  {5, 7},   // stepper 2 up/down button    
  {15, 17}, // stepper 3 up/down button (addressing A1 A3 analog pins as digital input)
  {14, 16}  // stepper 4 up/down button (addressing A0 A2 analog pins as digital input)
};

float stepperSpeed[4] = {0, 0, 0, 0};

const int debounceDelay = 50;
unsigned int lastDebounceTime = 0;
int buttonState[4][2];
int lastButtonState[4][2] = {
  {LOW, LOW},
  {LOW, LOW},
  {LOW, LOW},
  {LOW, LOW}
};

/********************* Stepper Motor Setup ********************/
int STEPPERS = 3;
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
void forwardstep1() { myStepper[0]->onestep(FORWARD, DOUBLE); }
void backwardstep1() { myStepper[0]->onestep(BACKWARD, DOUBLE); }
// wrappers for the second motor!
void forwardstep2() { myStepper[1]->onestep(FORWARD, DOUBLE); }
void backwardstep2() { myStepper[1]->onestep(BACKWARD, DOUBLE); }
// wrappers for the third motor!
void forwardstep3() { myStepper[2]->onestep(FORWARD, DOUBLE); }
void backwardstep3() { myStepper[2]->onestep(BACKWARD, DOUBLE); }
// wrappers for the fourth motor!
void forwardstep4() { myStepper[3]->onestep(FORWARD, DOUBLE); }
void backwardstep4() { myStepper[3]->onestep(BACKWARD, DOUBLE); }

// Now we'll wrap the 4 steppers in an AccelStepper object
AccelStepper *stepper[4];

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 2; ++j)
      pinMode(buttonPin[i][j], INPUT);

  lcd.begin(16, 2);   // set up the LCD's number of columns and rows:
  lcd.setCursor(0, 0);
  lcd.print("M1   M2   M3  M4");

  stepper[0] = new AccelStepper(forwardstep1,backwardstep1);
  stepper[1] = new AccelStepper(forwardstep2,backwardstep2);
  stepper[2] = new AccelStepper(forwardstep3,backwardstep3);
  stepper[3] = new AccelStepper(forwardstep4,backwardstep4);
  
  for (int i=0;i<STEPPERS;i++) {
    stepper[i]->setMaxSpeed(10000.0);
    stepper[i]->setAcceleration(1000000.0);
    stepper[i]->moveTo(14000000);
    stepper[i]->setSpeed(stepperSpeed[i]);    
  }        
  
  AFMSbot.begin(); // Start the bottom shield
  AFMStop.begin(); // Start the top shield  
}

void loop()
{
  for (int i=0; i < STEPPERS; ++i) {
    stepper[i]->setCurrentPosition(0);
    stepper[i]->moveTo(1000000);
    stepper[i]->setSpeed(stepperSpeed[i]);
    stepper[i]->run();
  }

  int buttonReading[4][2];
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 2; ++j)
    {
      buttonReading[i][j] = digitalRead(buttonPin[i][j]);
      if (buttonReading[i][j] != lastButtonState[i][j])
        lastDebounceTime = millis();
    }      
          
  if ((millis() - lastDebounceTime) > debounceDelay)
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 2; ++j)
      {
        
        if (buttonReading[i][j] != buttonState[i][j]) {
          buttonState[i][j] = buttonReading[i][j];
          if (buttonState[i][j] == HIGH) {
            switch (j) {
              case 0: if (stepperSpeed[i] < 99) stepperSpeed[i] = stepperSpeed[i] + float(1 * (1)); break;
              case 1: if (stepperSpeed[i] > 0)  stepperSpeed[i] = stepperSpeed[i] - float(1 * (1)); break;
            }    
            stepper[i]->setSpeed(stepperSpeed[i]); stepper[i]->run();
            sprintf(serial_buf,"EVENT: i=%d j=%d, stepperSpeed[%d]=%d\n",i,j,i,(int)stepperSpeed[i]);
            Serial.print(serial_buf);
            sprintf(lcd_string, "%02d   %02d   %02d  %02d",
                    (int)stepperSpeed[0], (int)stepperSpeed[1], (int)stepperSpeed[2], (int)stepperSpeed[3]);   
            lcd.setCursor(0, 1); lcd.print("                ");                         
            lcd.setCursor(0, 1); lcd.print(lcd_string);
          }
        }

      }
  
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 2; ++j)
      lastButtonState[i][j] = buttonReading[i][j];
        
}





