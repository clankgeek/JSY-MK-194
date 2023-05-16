# JSY-MK-194

## Change the default speed of serial port

*actualSpeed*: the actual bitrate of JSY-MK-194 serial port, default is 4800

*newSpeed*: the target bitrate, you can choose 4800, 9600, 19200 or 38400.

Note : before sending the firmware, disconnect the serial port of the JSY-MK-194, the UNO has only one serial port and it is shared with the USB.

- Connect JSY-MK-194 TX pin to pin 0 (RX) of Arduino Uno
- Connect JSY-MK-194 TX pin to pin 1 (TX) of Arduino Uno
