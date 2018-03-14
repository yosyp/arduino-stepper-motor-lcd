int serial_putchar(char c, FILE *f) {
  if (c == '\n')
    serial_putchar('\r', f);
  return Serial.write(c) == 1 ? 0 : 1;
}

void setup() {
  Serial.begin(57600);
  //  TWBR = ((F_CPU / 400000l) - 16) / 2; // Change the i2c clock to 400KHz

  fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &serial_stdout;
  printf("\nSetup serial out, can now use printf() directly!\n\n");

  /* Should use lambda functions here for campactness, but for readability: */
  stepper[0] = new AccelStepper(forwardStep0, backwardStep0);
  stepper[1] = new AccelStepper(forwardStep1, backwardStep1);

  for (int i = 0; i < MAX_POTS; i++) {
    printf("Setup %d!\n", i);
    stepper[i]->setMaxSpeed(max_v);
    stepper[i]->setAcceleration(max_a);
    stepper[i]->moveTo(14000000);
  }

  AFMSbot.begin(); // Start the bottom shield
  AFMStop.begin(); // Start the top shield
  //  TWBR = ((F_CPU / 400000l) - 16) / 2; // Change the i2c clock to 400KHz

}
