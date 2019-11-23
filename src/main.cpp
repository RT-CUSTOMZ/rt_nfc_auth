#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "I2CPort.h"
#include <PubSubClient.h>
#include <Ticker.h>

I2CPort i2cPort;
Ticker buttonDebounce;

const char *ssid = "ssid";
const char *password = "password";
const char *mqttServer = "mqtt.somedomain.de";
const String inputTopic = "nfcauthin";
const String  outputTopic = "nfcauthout";

bool lastOn1 = false;
bool lastOn2 = false;
bool lastOn3 = false;

String macAdr;

WiFiClient espClient;
PubSubClient mqtttClient(espClient);

// mqtt subscribe callback
void callback(char* topic, byte* payload, unsigned int length) {
 
}

void buttonCheckIrq(){
  i2cPort.readButtons();

  if(i2cPort.isButton1Pressed() && lastOn1 == false){
    i2cPort.turnLEDOn(1);
    lastOn1 = true;
  } 

  if (!i2cPort.isButton1Pressed() && lastOn1 == true){
    i2cPort.turnLEDOff(1);
    lastOn1 = false;
  }

  if(i2cPort.isButton2Pressed() && lastOn2 == false){
    i2cPort.turnLEDOn(2);
    lastOn2 = true;
  } 

  if (!i2cPort.isButton2Pressed() && lastOn2 == true){
    i2cPort.turnLEDOff(2);
    lastOn2 = false;
  }

  if(i2cPort.isButton3Pressed() && lastOn3 == false){
    i2cPort.turnLEDOn(3);
    lastOn3 = true;
  } 

  if (!i2cPort.isButton3Pressed() && lastOn3 == true){
    i2cPort.turnLEDOff(3);
    lastOn3 = false;
  }
}

void setup() {
  
  i2cPort = I2CPort();

  i2cPort.turnLEDOn(1);
  i2cPort.turnLEDOn(3);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    while(1)
    {
      i2cPort.toggleLED(1);
      i2cPort.toggleLED(3);
      delay(500);
    }
  }
  else
  {
    i2cPort.turnLEDOff(1);
    i2cPort.turnLEDOff(3);
    macAdr = WiFi.macAddress();
  }

  randomSeed(micros());

  mqtttClient.setServer(mqttServer, 1883);
  mqtttClient.setCallback(callback);

  // Ceck buttons interval everey 20ms if
  buttonDebounce.attach_ms(15,buttonCheckIrq);
}

void reconnect() {
  String outTopic = outputTopic + macAdr;

  String inTopic = inputTopic + macAdr;

  // Loop until we're reconnected
  while (!mqtttClient.connected()) {
    // Create a random client ID
    String clientId = "RTNFCAuth-";
    clientId += WiFi.macAddress();

    // Attempt to connect
    if (mqtttClient.connect(clientId.c_str())) {
      // Once connected, publish an announcement...
      mqtttClient.publish(outputTopic.c_str(), "hello world");

      // ... and resubscribe
      mqtttClient.subscribe(inTopic.c_str());

    } 
    else 
    {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!mqtttClient.connected()) {
    reconnect();
  }
  mqtttClient.loop();
}