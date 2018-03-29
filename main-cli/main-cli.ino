

/* arduino-stepper-motor-lcd main-cli.ino

   See https://github.com/yosyp/arduino-stepper-motor-lcd for parts list and
   other information.

   Using Adafruit Motor/Stepper/Servo Shield v2 and AccelStepper library, this
   code controls four NEMA-17 stepper motors using a command line interface.

   Yosyp Schwab
*/

#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

#define STEPPER_NUM 4
#define STEPPER_STEPS 200

/*
 *  setSpeed() requires input in steps/sec
 *  To convert RPM to steps/sec = 
 *      (200 steps/1 rotation) * (1 min / 60 sec) 
 */

Adafruit_MotorShield AFMSbot(0x61); // Rightmost jumper closed
Adafruit_MotorShield AFMS1(0x60); // Default address, no jumpers
Adafruit_MotorShield AFMS2(0x59); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the both shields
Adafruit_StepperMotor *myStepper[] = {
  AFMS1.getStepper(STEPPER_STEPS, 1),
  AFMS1.getStepper(STEPPER_STEPS, 2),
  AFMS2.getStepper(STEPPER_STEPS, 1),
  AFMS2.getStepper(STEPPER_STEPS, 2)
};

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() {
  myStepper[0]->onestep(FORWARD, DOUBLE);
}
void backwardstep1() {
  myStepper[0]->onestep(BACKWARD, DOUBLE);
}
// wrappers for the second motor!
void forwardstep2() {
  myStepper[1]->onestep(FORWARD, DOUBLE);
}
void backwardstep2() {
  myStepper[1]->onestep(BACKWARD, DOUBLE);
}
// wrappers for the third motor!
void forwardstep3() {
  myStepper[2]->onestep(FORWARD, DOUBLE);
}
void backwardstep3() {
  myStepper[2]->onestep(BACKWARD, DOUBLE);
}
// wrappers for the third motor!
void forwardstep4() {
  myStepper[3]->onestep(FORWARD, DOUBLE);
}
void backwardstep4() {
  myStepper[3]->onestep(BACKWARD, DOUBLE);
}

// Now we'll wrap the 4 steppers in an AccelStepper object
AccelStepper *stepper[STEPPER_NUM];

#define LINE_BUF_SIZE 128   // Maximum input string length
#define ARG_BUF_SIZE 64     // Maximum argument string length
#define MAX_NUM_ARGS 8      // Maximum number of arguments

//int LEDpin = 13;
//int blink_cycles = 10;      //How many times the LED will blink
bool error_flag = false;

char line[LINE_BUF_SIZE];
char args[MAX_NUM_ARGS][ARG_BUF_SIZE];

// Function declarations
int cmd_help();
int cmd_motor();
int cmd_on();
int cmd_off();

// List of functions pointers corresponding to each command
int (*commands_func[])() {
  &cmd_help,
  &cmd_motor,
  &cmd_on,
  &cmd_off
};

// List of command names
const char *commands_str[] = {
  "help",
  "motor",
  "on",
  "off"
};

//// List of LED sub command names
//const char *led_args[] = {
//    "on",
//    "off",
//    "blink"
//};

int num_commands = sizeof(commands_str) / sizeof(char *);

/************************** SETUP ******************************/
void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);

  stepper[0] = new AccelStepper(forwardstep1, backwardstep1);
  stepper[1] = new AccelStepper(forwardstep2, backwardstep2);
  stepper[2] = new AccelStepper(forwardstep3, backwardstep3);
  stepper[3] = new AccelStepper(forwardstep4, backwardstep4);

  for (int i=0;i<STEPPER_NUM;i++) {
    stepper[i]->setMaxSpeed(12000.0);
    stepper[i]->setAcceleration(10000.0);
    stepper[i]->moveTo(14000000);
  }

  AFMSbot.begin(); // Start the bottom shield
  AFMS1.begin(); // Start the middle shield
  AFMS2.begin(); // Start the top shield

  cli_init();
}

/************************** LOOP ******************************/
void loop() {
  my_cli();
}

/************** CLI FUNCTIONS ******************/
void cli_init() {
  Serial.println(F("Welcome to this simple Arduino command line interface (CLI).")); delay(50);
  Serial.println(F("Type \"help\" to see a list of commands.")); delay(50);
}


void my_cli() {
  Serial.print(F("> ")); delay(50);

  if(Serial.available() > 0) {
    read_line();
    if (!error_flag) {
      parse_line();
    }
    if (!error_flag) {
      execute();
    }
    memset(line, 0, LINE_BUF_SIZE);
    memset(args, 0, sizeof(args[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE);
  
    error_flag = false;    
  }
}

void read_line() {
  String line_string;

  while (!Serial.available());

  if (Serial.available()) {
    line_string = Serial.readStringUntil("\n");
    if (line_string.length() < LINE_BUF_SIZE) {
      line_string.toCharArray(line, LINE_BUF_SIZE);
      Serial.println(line_string);
    }
    else {
      Serial.println(F("Input string too long."));
      error_flag = true;
    }
  }
}

void parse_line() {
  char *argument;
  int counter = 0;

  argument = strtok(line, " ");

  while ((argument != NULL)) {
    if (counter < MAX_NUM_ARGS) {
      if (strlen(argument) < ARG_BUF_SIZE) {
        strcpy(args[counter], argument);
        argument = strtok(NULL, " ");
        counter++;
      }
      else {
        Serial.println(F("Input string too long."));
        error_flag = true;
        break;
      }
    }
    else {
      break;
    }
  }
}

int execute() {
  for (int i = 0; i < num_commands; i++) {
    if (strcmp(args[0], commands_str[i]) == 0) {
      return (*commands_func[i])();
    }
  }

  Serial.println(F("Invalid command. Type \"help\" for more."));
  return 0;
}

int cmd_help() {
  if (args[1] == NULL) {
    help_help();
  }
  else if (strcmp(args[1], commands_str[0]) == 0) {
    help_help();
  }
  else if (strcmp(args[1], commands_str[1]) == 0) {
    help_motor();
  }
  else if (strcmp(args[1], commands_str[2]) == 0) {
    help_on();
  }
  else if (strcmp(args[1], commands_str[3]) == 0) {
    help_off();
  }
  else {
    help_help();
  }
}

void help_help() {
  Serial.println(F("The following commands are available:"));

  for (int i = 0; i < num_commands; i++) {
    Serial.print(F("  "));
    Serial.println(commands_str[i]);
  }
  Serial.println(F(""));
  Serial.println(F("You can for instance type \"help motor\" for more info on the motor command."));
}

void help_motor() {
  Serial.println(F("Control the speed of each motor."));
  Serial.println(F("Usage: motor [motor-#] [speed-RPM]"));
}

int cmd_motor() {
  int motor_num;
  float motor_speed;

  motor_num = atoi(args[1]) - 1;
  motor_speed = atof(args[2]);

  if( motor_speed >= 0 ) {
    if( motor_num < STEPPER_NUM && motor_num >= 0) {
      stepper[motor_num]->setCurrentPosition(0);
      stepper[motor_num]->setSpeed(motor_speed * (float)STEPPER_STEPS);
      stepper[motor_num]->moveTo(14000000);    
      stepper[motor_num]->run();
      
      Serial.print(F("! Motor #"));
      Serial.print(motor_num+1);
      Serial.print(F(" set to "));
      Serial.print((stepper[motor_num]->speed()) / float(STEPPER_STEPS));
      Serial.println(F(" RPM"));
    } else {
      Serial.println(F("Invalid motor number! See \"help motor\""));
    }
  } else {
    Serial.println(F("Invalid speed! See \"help motor\""));    
  }
}

void help_on() {
  Serial.println(F("All motors on at 1 RPM or as specified."));
  Serial.println(F("Usage: on <optional:RPM>"));
}

int cmd_on() {
  float opt_speed;
  
  for(int i = 0; i<STEPPER_NUM; i++) {
    if (args[1]) {
      opt_speed = atof(args[1]);
      stepper[i]->setSpeed(opt_speed * (float)STEPPER_STEPS);
    } 
    else if(stepper[i]->speed() < (float)STEPPER_STEPS) {
      stepper[i]->setSpeed(STEPPER_STEPS);
    }
    stepper[i]->setCurrentPosition(0);  
    stepper[i]->moveTo(14000000);    
    stepper[i]->run();
    Serial.print(F("! Motor #"));
    Serial.print(i+1);
    Serial.print(F(" ON at "));
    Serial.print((stepper[i]->speed()) / float(STEPPER_STEPS));
    Serial.println(F(" RPM"));
  }
}

void help_off() {
  Serial.println(F("All motors OFF"));
}

int cmd_off() {
  for(int i = 0; i<STEPPER_NUM; i++) {
    stepper[i]->setSpeed(0);
    stepper[i]->moveTo(14000000);    
    stepper[i]->run();
    Serial.print(F("! Motor #"));
    Serial.print(i+1);
    Serial.println(F(" OFF"));
  }  
}

