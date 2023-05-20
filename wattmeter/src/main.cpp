#include <Arduino.h> // remove this line to use with Arduino ide, and rename .cpp file to .ino
#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define SERIAL_BITRATE 4800
#define LCD_ADDRESS 0x3F // some display use 0x27 address

const String Version = "1.0";

LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);
int Watt1, Watt2 = 0;

void lcdStrings(String, String);
void lcdInfo();
int32_t conv8to32(int32_t, int32_t, int32_t, int32_t);
char getData();

void setup() {
  Serial.begin(SERIAL_BITRATE, SERIAL_8N1);
  lcd.init();
  lcd.backlight();
  lcdStrings("WattMeter", "Version " + Version);
  delay(5000);
}

void loop() {
  if (getData()) {
    lcdInfo();
  } else {
    lcdStrings("JSY Error!", "Not enough data");
  }

  delay(100);
}

void lcdStrings(String msg0, String msg1) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(msg0);
  lcd.setCursor(0, 1);
  lcd.print(msg1);
}

void lcdInfo() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("P1: ");
  lcd.print(Watt1);
  lcd.print("w");
  lcd.setCursor(0, 1);
  lcd.print("P2: ");
  lcd.print(Watt2);
  lcd.print("w");
}

int32_t conv8to32(int32_t hi_byte, int32_t mid_byte_2, int32_t mid_byte_1,
                  int32_t lo_byte) {
  int32_t result = lo_byte;
  result += (mid_byte_1 << 8);
  result += (mid_byte_2 << 16);
  result += (hi_byte << 24);
  return result;
}

char getData() {
  byte dataRead[64];
  byte dataWrite[] = {0x01, 0x03, 0x00, 0x48, 0x00, 0x0E, 0x44, 0x18};
  uint8_t i;

  // segment to JSY-MK-194
  for (i = 0; i < 8; i++) {
    Serial.write(dataWrite[i]);
  }

  // recieve results from JSY-MK-194
  i = 0;
  while (Serial.available() && i < 64) {
    dataRead[i] = Serial.read();
    i++;
  }
  if (i != 61) {
    return false;
  }

  // voltage = 3, 4, 5, 6
  // frequancy = 31, 32, 33, 34
  // intensity1 = 7, 8, 9, 10
  // watt1 = 11, 12, 13, 14
  // kwh1 = 15, 16, 17, 18
  // negative1 = 27
  // intensity2 = 39, 40, 41, 42
  // watt2 = 43, 44, 45, 46
  // kwh2 = 47, 48, 49, 50
  // negative2 = 28

  Watt2 = conv8to32(dataRead[43], dataRead[44], dataRead[45], dataRead[46]) *
          0.0001;
  Watt1 = conv8to32(dataRead[11], dataRead[12], dataRead[13], dataRead[14]) *
          0.0001;

  if (dataRead[27] == 1 && Watt1 > 0) { // negative flow ?
    Watt1 = -Watt1;
  }
  if (dataRead[28] == 1 && Watt2 > 0) { // negative flow ?
    Watt2 = -Watt2;
  }
  return true;
}
