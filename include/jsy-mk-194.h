#ifndef JSY_MK_194_h
#define JSY_MK_194_h
#include <Arduino.h>
#include <HardwareSerial.h>

class JSY_MK_194 {
    private:
        const byte segmentWrite[8] = {0x01, 0x03, 0x00, 0x48, 0x00, 0x0E, 0x44, 0x18};
        byte segmentRead[64];
        int32_t conv8to32(int32_t, int32_t, int32_t, int32_t);
        int32_t getPower(boolean, uint8_t, uint8_t);
        int32_t getData(uint8_t);
        void updateSegment(byte [], byte, byte, byte);
    public:
        JSY_MK_194();
        void changeBitrate(uint16_t newBitrate = 9600);
        boolean read();
        int32_t getVoltage_1();
        int32_t getVoltage_2();
        int32_t getFrequency();
        int32_t getPower_1(boolean direction);
        int32_t getPower_2(boolean direction);
        int32_t getCurrent_1();
        int32_t getCurrent_2();
};

#endif