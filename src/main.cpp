/**
 * Arduino Infrared Decoder
 * 
 * @architecture NodeMCU ESP8266 ESP-12E
 * @framework Arduino
 * 
 * @version 2.0
 * @date 18/03/2018
 * 
 * @author Andrew Towensend <andrew@exclusivewebsystems.com>
 * @description Decodes data from the Kitchen Lighting IR Remote Control into its
 *              respective hex commands, this is published into a MQTT Topic via 
 *              the built in WiFi.
 * 
 */

/*-----( Include Require Libraries )-----*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/*-----( Declare WiFi objects )-----*/
#define WLAN_SSID       "TP-Link_IOT"
#define WLAN_PASS       "iLikeTurtles"

/*-----( Declare MQTT objects )-----*/
#define MQTT_SERVER_ADDR       "10.71.7.92"
#define MQTT_SERVER_PORT       1883
#define MQTT_APP               "kitchen/cabinet-lights"
#define MQTT_COMMAND_TOPIC     "/cmd"
#define MQTT_STATE_TOPIC       "-controller/state"

/*-----( Initialise Global objects )-----*/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER_ADDR, MQTT_SERVER_PORT);

/*-----( Initialise MQTT topics )-----*/
// Setup a feed called 'stateTopic' for publishing.
// Feed: kitchen/cabinet-lights-controller/state (ONLINE|OFFLINE)
Adafruit_MQTT_Publish stateTopic = Adafruit_MQTT_Publish(&mqtt,  MQTT_APP MQTT_STATE_TOPIC);

// Setup a feed called 'commandTopic' for publishing.
// Feed: kitchen/cabinet-lights-controller/state ($HEX)
Adafruit_MQTT_Publish commandTopic = Adafruit_MQTT_Publish(&mqtt,  MQTT_APP MQTT_STATE_TOPIC);

/*-----( Sketch Functions )-----*/
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void connectMQTT() {
    int8_t ret;

    // Stop if already connected.
    if (mqtt.connected()) {
        return;
    }

    Serial.print("Connecting to MQTT... ");

    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 10 seconds...");
        mqtt.disconnect();
        delay(10000);  // wait 10 seconds
        retries--;
        if (retries == 0) {
            // basically die and wait for WDT to reset me
            while (1);
        }
    }
    Serial.println("MQTT Connected!");
}

void connectWiFi() {
    // Connect to the WiFi access point.
    Serial.println(); 
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WLAN_SSID);

    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.println("WiFi connected");   
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    delay(10);

    Serial.println(F("NodeMCU MQTT IR Receiver"));

    // Connect to the WiFi
    connectWiFi();
}

void loop() {
    // put your main code here, to run repeatedly:

    // Ensure the connection to the MQTT server is alive (this will make the first
    // connection and automatically reconnect when disconnected).  See the MQTT_connect
    // function definition further below.
    connectMQTT();
}