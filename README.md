## Arduino controller with two stepper motors and an LCD display

Previously the project plan included adding breadboard mounted
potentionmeters and reading their analog signal to set the stepper
speeds. This turns out to be a bad approach, as the potentiometers
introduce inherent noise, and breadboard mounted components are not
resiliant for long term use. Instead, we can interface with the
arduino directly from the computer, and set the stepper motor speeds
through a homebrewed command line interface (CLI). 

### 

### Parts List

| *Part*                                                                                                                                                          | *Qty* | *Price* `($)` |
| -------------------------------------------------------------------------------------------------------------------------------------------------------------  | -----: | ----------: |
| [Arduino Uno R3 (Atmega328 - assembled)](https://www.adafruit.com/product/50)                                                                                 |   1 |     24.95 |
| [Adafruit Motor/Stepper/Servo Shield for Arduino v2 Kit - v2.3](https://www.adafruit.com/product/1438)                                                        |   2 |    39.90 |
| [Stepper motor - NEMA-17 size - 200 steps/rev, 12V 350mA](https://www.adafruit.com/product/324)                                                               |   4 |    56.00 |
| [WINOMO Nema 17 Stepper Motor Mounting Bracket w/ M3 Screws 2 Pack](https://www.amazon.com/gp/product/B01HHPD7LY/ref=oh_aui_detailpage_o07_s00?ie=UTF8&psc=1) |   2 |      16.58 |
| [Premium Male/Male Jumper Wires - 40 x 6" (150mm)](https://www.adafruit.com/product/758)                                                                      |   1 |      3.95 |
| [Shield stacking headers for Arduino (R3 Compatible)](https://www.adafruit.com/product/85)                                                                    |   2 |      3.90 |
| Switch single pole double throw, breadboard mountable                                                                                                         |   2 |    ------ |
| *Total:*                                                                                                                                                        |  -- |    *119.89* |

### Schematic Diagram

### Compiling, uploading
To use the the Arduino with the command line (CLI) instead of the Java applet, first clone the latest repo (or download the stable source) to some directory:
```
git clone https://github.com/arduino/Arduino.git
```
Next, navigate to the library directory:
```
cd arduino-nightly/libraries
```
Clone the following three libraries into the `library/` directory:
```
git clone https://github.com/adafruit/AccelStepper.git
```
```
git clone https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library.git
```
Then you can compile, verify, and upload with a single command:
```
arduino-nightly/arduino --upload main.ino
```
Note that you may need to allow the current user access to the proper `/dev/tty*` device as specified by the `~./arduino/preferences.txt` or `~./arduino15/preferences.txt` file, with the `serial.port=` parameter.

To view messages printed using `Serial.print()` in the code, you can open the corresponding tty device with `cu`. Make sure that the baud rate matches the rate set in `setup()` using `Serial.begin(57600)`. To exit `cu`, type `~.`. This may end your SSH session if you are programming over SSH, in which case the escape character can also be `~~.`. 
```
cu -l /dev/ttyACM0 -s 57600
```

If using a external power source for the stepper motors, you can increase the maximum speed (RPM) of the stepper. Modify `arduino-src/hardware/arduino/avr/libraries/Wire/src/utility/twi.h:27:20` to read:
```
	#define TWI_FREQ 400000L
```


### Resources


