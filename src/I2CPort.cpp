#include "I2CPort.h"

I2CPort::I2CPort()
{
    pcfWire = new TwoWire();
    
    pcf8574 = new PCF857x(PCF8574_ADR, pcfWire, false);

    pcfWire->begin(SDA_PIN, SCL_PIN);
    pcfWire->setClock(100000L);

    pcf8574->begin();

    pinMode(PCF_IRQ_PIN, INPUT_PULLUP);
    pcf8574->resetInterruptPin();

    led1On = false;
    led2On = false;
    led3On = false;
    
    buzzerOn = false;

    button1Pressed = false;
    button2Pressed = false;
    button3Pressed = false;
}

void I2CPort::setLED(uint8_t led, bool on)
{
    if(led > 0 && led < 4)
    {
        uint8_t ledPin = LED1_PIN;
        uint8_t ledOn;

        if(on)
        {
            ledOn = LOW;
        }
        else
        {
            ledOn = HIGH;
        }

        switch(led)
        {
            case 1:
            this->led1On = on;
            ledPin = LED1_PIN;
            break;

            case 2:
            this->led2On = on;
            ledPin = LED2_PIN;
            break;

            case 3:
            this->led3On = on;
            ledPin = LED3_PIN;
            break;
        }

        pcf8574->write(ledPin, ledOn);
    }
}

void I2CPort::turnLEDOn(uint8_t led)
{
    setLED(led, true);
}

void I2CPort::turnLEDOf(uint8_t led)
{
    setLED(led, false);
}

void I2CPort::toggleLED(uint8_t led)
{
    if(led > 0 && led < 4)
    {
        bool ledOn;

        switch(led)
        {
            case 1:
            ledOn = !this->led1On;
            break;

            case 2:
            ledOn = !this->led2On;
            break;

            case 3:
            ledOn = !this->led3On;
            break;
        }

        setLED(led,ledOn);
    }   
}

void I2CPort::turnBuzzerOn()
{
    this->buzzerOn = true;
    pcf8574->write(BUZZER_PIN, LOW);    
}

void I2CPort::turnBuzzerOff()
{
    this->buzzerOn = false;
    pcf8574->write(BUZZER_PIN, HIGH);   
}

void I2CPort::toggleBuzzer()
{
    this->buzzerOn = !this->buzzerOn;

    if(this->buzzerOn)
    {
        pcf8574->write(BUZZER_PIN, LOW);   
    }
    else
    {
        pcf8574->write(BUZZER_PIN, HIGH);   
    }
}