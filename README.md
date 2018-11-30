# RT NFC Auth
RT NFC Auth is part of the controll and authentication environment for machines in the hardware laboratory of the Westfälische Hochschule Campus Bocholt. 

This Repository contains the firmware for the RT NFC Authenticator based on the **ESP 8266** and the **Grove - NFC** module. The firmware is written in Arduino.

The RT NFC Authenticator acts as an dump NFC reader transmitting json encoded IDs over mqtt. Additional to the NFC reader the board contains 3 LEDs, 3 buttons and 1 buzzer that can be controlled with mqtt messges.

JSON strings transmitted from the authenticator look like this

**TAG ID Message**
```json
{
    id : 'some nfc tag id'
}
```

**Buttons Pressed Message**
```json
{
    button : 'button1',
    state : 'pressed'
}
```

**Buttons released Message**
```json
{
    button : 'button2',
    state : 'released'
}
```

JSON strings to control the LEDs and buzzer look like this

**LED Buzzer control message**
```json
{
    led1 : 'on',
    led2 : 'off',
    led3 : 'on',
    buzzer : 'on'
}
```