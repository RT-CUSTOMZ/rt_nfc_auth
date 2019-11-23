#ifndef _IO74HC59X_H
#define _IO74HC59X_H

#include<Arduino.h>
#include<Ticker.h>


#define SCLK_PIN 14
#define MOSI_PIN 13
#define MISO_PIN 12
#define LATCH_PIN 16

class IO74hc59x
{
    private:
        uint8_t *registers; // MSBit first, left to right
        int regCount;
        bool lock;

        Ticker poll;
        void transmit();

        static void transmitCallback(void * this_pointer)
        {
            IO74hc59x * self = static_cast<IO74hc59x*>(this_pointer);
            self->transmit();
        }
    public:
        IO74hc59x();
        int getRegCount();
        void setBit(int bit, int value);
        unsigned int getBit(int);

};

#endif