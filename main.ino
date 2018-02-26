/*
 */

#include <LiquidCrystal.h>

int PotPinL = A0;  // Pin for left potentiometer knob (analog)
int PotPinR = A1;  // Pin for right potentionmeter knob (analog)
int SysSwitch = 6; // Pin for system toggle switch, turns motors on/off (digital)
int DirSwitch = 5; // Pin for direction toggle switch, same or opposite directions (digital)

int SysRead, DirRead;
int PotReadL, PotReadR;

int direction = 0; // Will store rotation direction of two motors, passed to void printArrows()

int SysState = HIGH, DirState = HIGH;
int SysPrev = LOW, DirPrev = LOW;

LiquidCrystal lcd(7,8,9,10,11,12);

void setup() {
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
  lcd.setCursor(0,1); lcd.print("L:");
  lcd.setCursor(9,1); lcd.print("R:");  
  lcd.setCursor(4,1); lcd.print("rpm");  
  lcd.setCursor(13,1); lcd.print("rpm");  
}


void loop() {
  /* 
   * Read the current system state (motors ON or OFF), and
   * switch state accordingly. Save previous state to compare.
   */
  SysRead = digitalRead(SysSwitch);
  if(SysRead == HIGH && SysPrev == LOW) {
    if(SysState == HIGH) { SysState = LOW; }
    else { SysState = HIGH; }    
    lcd.setCursor(6,0);
    lcd.print(" ON ");
  } else if(SysRead == LOW && SysPrev == HIGH) {
    if(SysState == HIGH) { SysState = LOW; }
    else { SysState = HIGH; }    
    lcd.setCursor(6,0);
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
  if(DirRead == HIGH && DirPrev == LOW) {
    if(DirState == HIGH) { DirState = LOW; }
    else { DirState = HIGH; }    
    direction = 0;
  } else if(DirRead == LOW && DirPrev == HIGH) {
    if(DirState == HIGH) { DirState = LOW; }
    else { DirState = HIGH; }    
    direction = 1;
  }    
  DirPrev = DirRead;

  /*
   * Read and display current potentiometer values on LCD.
   * Values are cutoff to be between 0 and 99.
   */
  PotReadL = analogRead(PotPinL)/10; if(PotReadL>99) PotReadL = 99;
  PotReadR = analogRead(PotPinR)/10; if(PotReadR>99) PotReadR = 99;
  lcd.setCursor(2,1);  lcd.print("  ");
  lcd.setCursor(2,1);  lcd.print(PotReadL);
  lcd.setCursor(11,1); lcd.print("  ");
  lcd.setCursor(11,1); lcd.print(PotReadR);
  
  lcd.setCursor(7, 1);
  if((millis()/500)%2 == 0) {
    lcd.print("<>");
  } else {
    lcd.print("><");
  }

  printArrows(direction);
}

/*
 * Print direction arrows on first row 
 * 0 -- motors spin in same direction, [->- ->-]
 * 1 -- right motor spins in opposite direction, [->- -<-]
 */
void printArrows(int direction) {
  int ttime = millis();
  if( (ttime/300)%2 == 0) {
    if(direction == 0) {
      lcd.setCursor(0,0);  lcd.print(">->->");
      lcd.setCursor(11,0); lcd.print("->->-");
    } else {
      lcd.setCursor(0,0);  lcd.print(">->->");
      lcd.setCursor(11,0); lcd.print("-<-<-");
    }
  } else {
    if(direction == 0) {
      lcd.setCursor(0,0); lcd.print("->->-");
      lcd.setCursor(11,0); lcd.print(">->->");
    } else {
      lcd.setCursor(0,0);  lcd.print("->->-");
      lcd.setCursor(11,0); lcd.print("<-<-<");
    }
  }
}
