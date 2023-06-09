A simple library to use module JSY-MK-194 with Arduino.

![JSY-MK-194.jpg](JSY-MK-194.jpg)

Note : Due to limitation of Uno/Nano at only once serial port, before upload firmware on Uno, you must unplug the serial port of JSY-MK-194.

- Connect JSY-MK-194 TX pin to pin RX of Arduino
- Connect JSY-MK-194 RX pin to pin TX of Arduino
- Connect JSY-MK-194 GND pin to pin GND of Arduino
- Connect JSY-MK-194 VCC pin to pin 3.3v of Arduino
- Connect JSY-MK-194 N pin to 250v neutral
- Connect JSY-MK-194 L pin to 250v positive (WARNING ELECTRICAL RISK)

## VS Code & PlatformIO

- Install [VS Code](https://code.visualstudio.com)
- Install [Platform IO](https://platformio.org/install/ide?install=vscode)
- Install JSY-MK-194 library by using Library Manager. Search for JSY-MK-194 in Platform.io Author's Libraries

## Usage

1. JSY-MK-194 Class

1.1. Function List

```
void changeBitrate(uint16_t newBitrate = 9600);
boolean read(); // read data, return true if success
int32_t getFrequency();
int32_t getVoltage_1();
int32_t getVoltage_2();
int32_t getPower_1(boolean direction);
int32_t getPower_2(boolean direction);
int32_t getCurrent_1();
int32_t getCurrent_2();
```

2.2. Usage

```
#include <Arduino.h>
#include <HardwareSerial.h>
#include <jsy-mk-194.h>

JSY_MK_194 jsy;

void setup() {
    Serial.begin(4800, SERIAL_8N1);
}

void loop() {
    if (jsy.read()) {
        int32_t p = getPower_1(true);
        // use some actions
    }
}
```
