#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "I2CPort.h"
#include <PubSubClient.h>

I2CPort i2cPort;

const char *ssid = "ssid";
const char *password = "pwpw";
const char *mqttServer = "mqtt.somedomain.de";
const String inputTopic = "nfcauthin";
const String  outputTopic = "nfcauthout";

String macAdr;

WiFiClient espClient;
PubSubClient mqtttClient(espClient);

void callback(char* topic, byte* payload, unsigned int length) 
{
  
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
    i2cPort.turnLEDOf(1);
    i2cPort.turnLEDOf(3);
    macAdr = WiFi.macAddress();
  }

  randomSeed(micros());

  mqtttClient.setServer(mqttServer, 1883);
  mqtttClient.setCallback(callback);
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
  i2cPort.toggleLED(1);

  if (!mqtttClient.connected()) {
    reconnect();
  }
  mqtttClient.loop();
  
  delay(1000);
}