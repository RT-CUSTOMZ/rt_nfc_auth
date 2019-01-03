#include "I2CPort.h"

// puplic:

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

    buttonLock = false;

    button1DebounceOverflow = 0;
    button2DebounceOverflow = 0;
    button3DebounceOverflow = 0;

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

void I2CPort::turnLEDOff(uint8_t led)
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

void I2CPort::i2cInterrupt(){  
    if (buttonLock == false)
    {
        uint8_t portValue = pcf8574->read8();
        portValue = portValue >> 4;
        
        // Button1 clicked
        if(0b00001110 == (portValue | 0b00001110)){
            button1Pressed = true;
        }else{
            button1Pressed = false;
        }

        // Button2 clicked
        if(0b00001101 == (portValue | 0b00001101)){
            button2Pressed = true;
        } else {
            button2Pressed = false;
        }

        // Button3 clicked
        if(0b00001011 == (portValue | 0b00001011)){
            button3Pressed = true;
        } else {
            button3Pressed = false;
        }

        buttonLock = true;
    } else {
        // Lcking noch mal überdenken
    }

    pcf8574->resetInterruptPin();
}

void I2CPort::resetInterrupt(){
    pcf8574->resetInterruptPin();
}

bool I2CPort::isButton1Pressed(){
    return this->button1Pressed;
}

bool I2CPort::isButton2Pressed(){
    return this->button2Pressed;
}

bool I2CPort::isButton3Pressed(){
    return this->button3Pressed;
}

void I2CPort::unlockButtons(){
    this->buttonLock = false;
}

void I2CPort::readButtons(){

    uint8_t portValue = pcf8574->read8();
    portValue = portValue >> 4;

    // --- Button1 ---
    // Check if Button1 is pressed
    if(0b00001110 == (portValue | 0b00001110) && button1DebounceOverflow < 3){
        button1DebounceOverflow++;
    }

    // Check if Button1 is released
    if( 0b00001111 == (portValue | 0b00001110) && button1DebounceOverflow > 0){
        button1DebounceOverflow--;
    }

    // Check if Threshold for Button1 pressed is exceeded and Transition is neccessary
    if(button1Pressed == false && button1DebounceOverflow == 3){
        button1Pressed = true;
    } 

    // Check if Threshold for Button1 released is exceeded and Transition is neccessary
    if(button1Pressed == true && button1DebounceOverflow == 0){
        button1Pressed = false;
    } 

    // --- Button2 ---
    // Check if Button1 is pressed
    if(0b00001101 == (portValue | 0b00001101) && button2DebounceOverflow < 3){
        button2DebounceOverflow++;
    }

    // Check if Button1 is released
    if( 0b00001111 == (portValue | 0b00001101) && button2DebounceOverflow > 0){
        button2DebounceOverflow--;
    }

    // Check if Threshold for Button1 pressed is exceeded and Transition is neccessary
    if(button2Pressed == false && button2DebounceOverflow == 3){
        button2Pressed = true;
    } 

    // Check if Threshold for Button1 released is exceeded and Transition is neccessary
    if(button2Pressed == true && button2DebounceOverflow == 0){
        button2Pressed = false;
    } 

    // --- Button3 ---
    // Check if Button1 is pressed
    if(0b00001011 == (portValue | 0b00001011) && button3DebounceOverflow < 3){
        button3DebounceOverflow++;
    }

    // Check if Button1 is released
    if( 0b00001111 == (portValue | 0b00001011) && button3DebounceOverflow > 0){
        button3DebounceOverflow--;
    }

    // Check if Threshold for Button1 pressed is exceeded and Transition is neccessary
    if(button3Pressed == false && button3DebounceOverflow == 3){
        button3Pressed = true;
    } 

    // Check if Threshold for Button1 released is exceeded and Transition is neccessary
    if(button3Pressed == true && button3DebounceOverflow == 0){
        button3Pressed = false;
    } 
}