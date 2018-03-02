## Arduino controller with two stepper motors and an LCD display

<project description added later here>

### 

### Parts List

| *Part*                                                                                                                                                          | *Qty* | *Price* `($)` |
| -------------------------------------------------------------------------------------------------------------------------------------------------------------  | -----: | ----------: |
| [Arduino Uno R3 (Atmega328 - assembled)](https://www.adafruit.com/product/50)                                                                                 |   1 |     24.95 |
| [Adafruit Motor/Stepper/Servo Shield for Arduino v2 Kit - v2.3](https://www.adafruit.com/product/1438)                                                        |   1 |     19.95 |
| [Stepper motor - NEMA-17 size - 200 steps/rev, 12V 350mA](https://www.adafruit.com/product/324)                                                               |   2 |     28.00 |
| [Large Solderless Breadboard](https://www.adafruit.com/product/443)                                                                                           |   1 |     19.95 |
| [Standard LCD 16x2 + extras - white on blue](https://www.adafruit.com/product/181)                                                                            |   1 |      9.95 |
| [WINOMO Nema 17 Stepper Motor Mounting Bracket w/ M3 Screws 2 Pack](https://www.amazon.com/gp/product/B01HHPD7LY/ref=oh_aui_detailpage_o07_s00?ie=UTF8&psc=1) |   1 |      8.29 |
| [Premium Male/Male Jumper Wires - 40 x 6" (150mm)](https://www.adafruit.com/product/758)                                                                      |   1 |      3.95 |
| [Shield stacking headers for Arduino (R3 Compatible)](https://www.adafruit.com/product/85)                                                                    |   1 |      1.95 |
| [Panel Mount 10K potentiometer (Breadboard Friendly) - 10K Linear](https://www.adafruit.com/product/562)                                                      |   2 |      1.90 |
| [Potentiometer Knob - Soft Touch T18 - Blue](https://www.adafruit.com/product/2048)                                                                           |   2 |      1.00 |
| Switch single pole double throw, breadboard mountable                                                                                                         |   2 |    ------ |
| 10 kOhm resistors (brown-black-gold-yellow)                                                                                                                   |   2 |    ------ |
| Misc. length breadboard jumper wires                                                                                                                          |  10 |    ------ |
| *Total:*                                                                                                                                                        |  -- |    *119.89* |


### Schematic Diagram

### Compiling, uploading
```
git clone https://github.com/adafruit/AccelStepper.git
```
```
git clone https://github.com/dxinteractive/ResponsiveAnalogRead.git
```

If using a external power source for the stepper motors, you can increase the maximum speed (RPM) of the stepper. Modify `arduino-src/hardware/arduino/avr/libraries/Wire/src/utility/twi.h:27:20` to read:
```
	#define TWI_FREQ 400000L
```

```
arduino --upload main.ino
```

### Resources


