#include "io74hc59x.h"

IO74hc59x::IO74hc59x()
{
    Serial.begin(115200);
    Serial.write("IO74hc59x init...");
    lock = false;
    //init pins
    pinMode(SCLK_PIN, OUTPUT);
    pinMode(MOSI_PIN, OUTPUT);
    pinMode(MISO_PIN, INPUT);
    pinMode(LATCH_PIN, OUTPUT);

    //reset all registers
    digitalWrite(MOSI_PIN,LOW);
    for(int i = 0;i<1000;i++)
    {
        delayMicroseconds(5);
        digitalWrite(SCLK_PIN, HIGH);
        delayMicroseconds(5);
        digitalWrite(SCLK_PIN, LOW);
    }

    //push one bit
    digitalWrite(MOSI_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(SCLK_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(MOSI_PIN,LOW);
    digitalWrite(SCLK_PIN, LOW);

    regCount = 1;

    Serial.println("counting registers...");

    for(int i = 1;i<8;i++)
        {
            delayMicroseconds(5);
            digitalWrite(SCLK_PIN, HIGH);
            delayMicroseconds(5);
            digitalWrite(SCLK_PIN, LOW);
        }
    //count total bits
    while (digitalRead(MISO_PIN)==0)
    {
        for(int i = 0;i<8;i++)
        {
            delayMicroseconds(5);
            digitalWrite(SCLK_PIN, HIGH);
            delayMicroseconds(5);
            digitalWrite(SCLK_PIN, LOW);
            if(digitalRead(MISO_PIN)==1)
            {
                break;
            }
        }
        delayMicroseconds(20);
        regCount++;
        Serial.println(regCount);
    }

    Serial.printf("count set to: %d\n",regCount);
    //init register array
    registers = (uint8_t *)malloc(regCount * sizeof(uint8_t));

    for(int i =0; i<regCount;i++)
    {
        Serial.printf("register no %d",i);
        Serial.println(registers[i]);
    }

    //set ticker callback to execute transmit every 3ms
    poll.attach(3, IO74hc59x::transmitCallback, static_cast<void*>(this));
}



void IO74hc59x::transmit()
{
    if (lock)
    {
        return;
    }
    lock = true;
    bool change = false;
    
    //load inputs to shift registers
    digitalWrite(LATCH_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(LATCH_PIN, LOW);

    Serial.println("transmitting...");
    for(int i = 0;i<regCount;i++)
    {
        for (int bit = 0; bit < 8; bit++)
        {
            delayMicroseconds(5);
            // write
            Serial.printf("%d",((registers[i]>>bit)&1) );
            digitalWrite(MOSI_PIN, (registers[i]>>bit)&1 );
            // read
            uint8_t previous = (registers[i]>>bit)&1;
            registers[i] &= ~(1>>bit); // clear bit
            registers[i] |= (digitalRead(MISO_PIN)>>bit); // set bit

            // check for changes
            if (previous != ((registers[i]>>bit)&1))
            {
                change = true;
            }
            

            // clock
            delayMicroseconds(5);
            digitalWrite(SCLK_PIN, HIGH);
            delayMicroseconds(5);
            digitalWrite(SCLK_PIN, LOW);

        }
        
    }
    Serial.println("\n");

    if(change && changeCallback){
        changeCallback();
    }
    //refresh outputs
    digitalWrite(LATCH_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(LATCH_PIN, LOW);

    lock = false;
}

void IO74hc59x::setBit(int bit, int value){
    while(lock){
        delay(10);
    }
    lock = true;

    if(bit >= regCount*8){
        //return for invalid bits
        return;
    }

    registers[(int)bit/8] &= ~(1<<(7-bit%8)); // clear bit
    registers[(int)bit/8] |= (value<<(7-bit%8)); // set bit

    lock = false;
}

unsigned int IO74hc59x::getBit(int bit){
    while(lock){
        delay(10);
    }
    if(bit >= regCount*8){
        //return for invalid bits
        return 0;
    }
    return (registers[(int)bit/8]>>bit%8)&1;
}

int IO74hc59x::getRegCount()
{
    return regCount;
}

void IO74hc59x::setChangeCallback(callback* callback)
{
    if(callback){
        changeCallback = callback;
    }
}