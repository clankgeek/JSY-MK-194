#include <Arduino.h>
#include <HardwareSerial.h>

uint32_t actualSpeed = 4800;
uint32_t newSpeed = 9600; // you can choose 4800, 9600, 19200 or 38400

byte segment[] = {0x00, 0x10, 0x00, 0x04, 0x00, 0x01,
                  0x02, 0x01, 0x00, 0x00, 0x00};

void updateValues(byte value, byte crc1, byte crc2) {
  segment[8] = value;
  segment[9] = crc1;
  segment[10] = crc2;
}

void setSegment(uint32_t speed) {
  // 5-4800bps (default), 6-9600bps，7-19200bps，8-38400bps
  switch (speed) {
  case 4800:
    updateValues(0x05, 0x6B, 0xD7);
    break;
  case 19200:
    updateValues(0x07, 0xEA, 0x16);
    break;
  case 38400:
    updateValues(0x08, 0xAA, 0x12);
    break;
  default:
    updateValues(0x06, 0x2B, 0xD6);
  }
}

void setup() {
  Serial.begin(actualSpeed, SERIAL_8N1);
  setSegment(newSpeed);
}

void loop() {
  delay(1000);

  uint8_t i;
  for (i = 0; i < 11; i++) {
    Serial.write(segment[i]);
  }
}
