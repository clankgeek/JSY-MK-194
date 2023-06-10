#include "jsy-mk-194.h"

JSY_MK_194::JSY_MK_194() {}

/*
fill data from JSY_MK_194
@return true if success
*/
boolean JSY_MK_194::read() {
  uint8_t i;

  // send segment to JSY-MK-194
  for (i = 0; i < 8; i++) {
    Serial.write(segmentWrite[i]);
  }

  i = 0;
  while (Serial.available() && i < 64) {
    segmentRead[i] = Serial.read();
    i++;
  }
  if (i != 61) {
    return false;
  }
  return true;
}

// voltage1 = 3, 4, 5, 6
// current1 = 7, 8, 9, 10
// power1 = 11, 12, 13, 14
// kwh1 = 15, 16, 17, 18
// powerfactor1 = 19, 20, 21, 22
// negativekwh1 = 23, 24, 25, 26
// negative1 = 27
// negative2 = 28
// not used = 29, 30
// frequency = 31, 32, 33, 34
// voltage2 = 35, 36, 37, 38
// current2 = 39, 40, 41, 42
// power2 = 43, 44, 45, 46
// kwh2 = 47, 48, 49, 50
// powerfactor2 = 51, 52, 53, 54
// negativekwh2 = 55, 56, 57, 58

int32_t JSY_MK_194::conv8to32(int32_t hi_byte, int32_t mid_byte_2,
                              int32_t mid_byte_1, int32_t lo_byte) {
  int32_t result = lo_byte;
  result += (mid_byte_1 << 8);
  result += (mid_byte_2 << 16);
  result += (hi_byte << 24);
  return result;
}

int32_t JSY_MK_194::getData(uint8_t i) {
  return JSY_MK_194::conv8to32(segmentRead[i], segmentRead[i + 1],
                               segmentRead[i + 2], segmentRead[i + 3]);
}

/*
return the voltage of first channel in volt
*/
int32_t JSY_MK_194::getVoltage_1() { return JSY_MK_194::getData(3) * 0.0001; }

/*
return the voltage of second channel in volt
*/
int32_t JSY_MK_194::getVoltage_2() { return JSY_MK_194::getData(35) * 0.0001; }

/*
return frequency in hz
*/
int32_t JSY_MK_194::getFrequency() { return JSY_MK_194::getData(31) * 0.01; }

/*
return current in A of channel 1
*/
int32_t JSY_MK_194::getCurrent_1() { return JSY_MK_194::getData(7) * 0.0001; }

/*
return current in A of channel 2
*/
int32_t JSY_MK_194::getCurrent_2() { return JSY_MK_194::getData(39) * 0.0001; }

int32_t JSY_MK_194::getPower(boolean direction, uint8_t i, uint8_t j) {
  int32_t p = JSY_MK_194::getData(i) * 0.0001;
  if (direction && segmentRead[j] == 1 && p > 0) {
    p = -p;
  }
  return p;
}

/*
return the power of channel 1 in watt
direction false return always a positive value
*/
int32_t JSY_MK_194::getPower_1(boolean direction) {
  return getPower(direction, 11, 27);
}

/*
return the power of channel 2 in watt
direction false return always a positive value
*/
int32_t JSY_MK_194::getPower_2(boolean direction) {
  return getPower(direction, 43, 28);
}

void JSY_MK_194::updateSegment(byte *seg, byte value, byte crc1, byte crc2) {
  seg[8] = value;
  seg[9] = crc1;
  seg[10] = crc2;
}

/*
Default bitrate is 4800, you can update the bitrate of module
the available values are : 4800, 9600, 19200, 38400
@param newbitrate speed in bps of new bitrate
*/
void JSY_MK_194::changeBitrate(uint16_t newbitrate) {
  byte segment[] = {0x00, 0x10, 0x00, 0x04, 0x00, 0x01,
                    0x02, 0x01, 0x00, 0x00, 0x00};
  uint8_t i;

  switch (newbitrate) {
  case 4800:
    updateSegment(segment, 0x05, 0x6B, 0xD7);
    break;
  case 19200:
    updateSegment(segment, 0x07, 0xEA, 0x16);
    break;
  case 38400:
    updateSegment(segment, 0x08, 0xAA, 0x12);
    break;
  default: // 9600
    updateSegment(segment, 0x06, 0x2B, 0xD6);
  }

  delay(1000);

  for (i = 0; i < 11; i++) {
    Serial.write(segment[i]);
  }
}