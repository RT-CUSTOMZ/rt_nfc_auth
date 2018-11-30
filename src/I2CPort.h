#ifndef _I2CPORT_H
#define _I2CPORT_H

#include <Arduino.h>
#include "pcf8574_esp.h"
#include <Wire.h>

#define PCF8574_ADR 0x20

#define SDA_PIN 2
#define SCL_PIN 0

#define LED1_PIN 3
#define LED2_PIN 2
#define LED3_PIN 1

#define BUTTON1_PIN 4
#define BUTTON2_PIN 5
#define BUTTON3_PIN 6

#define PCF_IRQ_PIN 5

#define BUZZER_PIN 0

class I2CPort
{
    private:
        TwoWire *pcfWire;
        PCF857x *pcf8574;

        bool led1On;
        bool led2On;
        bool led3On;
        bool buzzerOn;
        bool button1Pressed;
        bool button2Pressed;
        bool button3Pressed;

    public:
        I2CPort();

        void turnLEDOn(uint8_t led);
        void turnLEDOf(uint8_t led);
        void toggleLED(uint8_t led);
        void setLED(uint8_t led, bool on);
        void turnBuzzerOn();
        void turnBuzzerOff();
        void toggleBuzzer();
};

#endif