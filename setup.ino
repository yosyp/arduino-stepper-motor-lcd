void setup() {
  for (int i=0; i<MAX_POTS; i++) {
    pot[i] = new ResponsiveAnalogRead(PotentiometerPin[i], true);
    stepper[i] = new AccelStepper(
        myStepper[i]->onestep(FORWARD, DOUBLE),
        myStepper[i]->onestep(BACKWARD, DOUBLE));
    stepper[i]->setMaxSpeed(max_v);
    stepper[i]->setAcceleration(max_a);
    stepper[i]->moveTo(24);
  }

  //  Serial.begin(57600);
  AFMSbot.begin(); // Start the bottom shield
  AFMStop.begin(); // Start the top shield
  //  TWBR = ((F_CPU / 400000l) - 16) / 2; // Change the i2c clock to 400KHz


  pinMode(SysSwitch, INPUT); // Global motor on/off switch
  pinMode(DirSwitch, INPUT); // Motor direction switch
  pinMode(PotentiometerPin[0], INPUT);   // Left potentiometer knob
  pinMode(PotentiometerPin[1], INPUT);   // Right potentiometer knob

  /*
   * Set up the LCD's number of columns and rows.
   * LCD Layout is as follows:
   * ******************************************************
   *  COL: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
   * ROW0: -  >  -  >  -     S  T  O  P     -  >  -  >  -
   * ROW1: L  :  x  x  R  P  M  /  /  R  :  x  x  R  P  M
   * ******************************************************
   */
  lcd.begin(16, 2);
  lcd.setCursor(0, 1);
  lcd.print("L");
  lcd.setCursor(9, 1);
  lcd.print("R");
  lcd.setCursor(4, 1);
  lcd.print("rpm");
  lcd.setCursor(13, 1);
  lcd.print("rpm");
}

