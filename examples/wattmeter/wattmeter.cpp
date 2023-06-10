#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <jsy-mk-194.h>

#define SERIAL_BITRATE 4800 // default value of jsy-mk-194
#define LCD_ADDRESS 0x3F // some lcd use 0x27 address

JSY_MK_194 jsy;
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

void setup() {
  Serial.begin(SERIAL_BITRATE, SERIAL_8N1); // shared beetween usb and by jsy-mk-194, unplug jsy-mk-194 to upload program
  lcd.init();
  lcd.backlight();
  delay(1000);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (jsy.read()) {
    lcd.print(jsy.getPower_1(true));
    lcd.setCursor(0, 1);
    lcd.print(jsy.getPower_2(true));
  } else {
    lcd.print("Not enough data");
  }

  delay(100);
}