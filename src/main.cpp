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
#define MQTT_STATE_TOPIC       "/state"

/*-----( Initialise objects )-----*/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER_ADDR, MQTT_SERVER_PORT);

void setup() {
    // put your setup code here, to run once:
}

void loop() {
    // put your main code here, to run repeatedly:
}