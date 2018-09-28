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
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/*-----( Declare objects )-----*/
#define SERIAL_BAUD     115200

/*-----( Declare WiFi objects )-----*/
#define WLAN_SSID       "TP-Link_IOT"
#define WLAN_PASS       "iLikeTurtles"

/*-----( Declare MQTT objects )-----*/
#define MQTT_SERVER_ADDR       "10.71.7.92"
#define MQTT_SERVER_PORT       1883
#define MQTT_APP               "kitchen/cabinet-lights"
#define MQTT_COMMAND_TOPIC     "/cmd"
#define MQTT_STATE_TOPIC       "-controller/state"

/*-----( Declare IR objects )-----*/
#define IR_RECEIVER_PIN        11

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

/*-----( Initialise IR objects )-----*/
// create instance of 'irrecv'
IRrecv irrecv(IR_RECEIVER_PIN);
// create instance of 'decode_results'
decode_results results;      

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

void connectIR() {
    // Start the IR Receiver
    irrecv.enableIRIn();
}

void connectSerial() {
    // Begin Serial connection for debugging/logging
    Serial.begin(SERIAL_BAUD);
    delay(10);
}

void publishMQTTCommand(String value) {

}

void decodeIRResult() {
    String command = "";
    switch(results.value) {
		case 0xFF3AC5: 
			Serial.println("Brightness Up"); 
            command = "FF3AC5";
			break;
		case 0xFFBA45: 
			Serial.println("Brightness Down"); 
            command = "FFBA45";
			break;
		case 0xFF827D: 
			Serial.println("Pause Run"); 
            command = "FF827D";
			break;
		case 0xFF02FD: 
			Serial.println("Power"); 
            command = "FF02FD";
			break;
        case 0xFF1AE5: 
            Serial.println("Red"); 
            command = "FF1AE5";
            break;
        case 0xFF9A65: 
            Serial.println("Green"); 
            command = "FF9A65";
            break;
        case 0xFFA25D: 
            Serial.println("Blue"); 
            command = "FFA25D";
            break;
        case 0xFF22DD: 
            Serial.println("White"); 
            command = "FF22DD";
            break;
        case 0xFF2AD5: 
            Serial.println("A1"); 
            command = "FF2AD5";
            break;
        case 0xFFAA55: 
            Serial.println("B1"); 
            command = "FFAA55";
            break;
        case 0xFF926D: 
            Serial.println("C1"); 
            command = "FF926D";
            break;
        case 0xFF12ED: 
            Serial.println("D1"); 
            command = "FF12ED";
            break;
        case 0xFF0AF5: 
            Serial.println("A2"); 
            command = "FF0AF5";
            break;
        case 0xFF8A75: 
            Serial.println("B2"); 
            command = "FF8A75";
            break;
        case 0xFFB24D: 
            Serial.println("C2"); 
            command = "FFB24D";
            break;
        case 0xFF32CD: 
            Serial.println("D2"); 
            command = "FF32CD";
            break;
        case 0xFF38C7: 
            Serial.println("A3"); 
            command = "FF38C7";
            break;
        case 0xFFB847: 
            Serial.println("B3"); 
            command = "FFB847";
            break;
        case 0xFF7887: 
            Serial.println("C3"); 
            command = "FF7887";
            break;
        case 0xFFF807: 
            Serial.println("D3"); 
            command = "FFF807";
            break;
        case 0xFF18E7: 
            Serial.println("A4"); 
            command = "FF18E7";
            break;
        case 0xFF9867: 
            Serial.println("B4"); 
            command = "FF9867";
            break;
        case 0xFF58A7: 
            Serial.println("C4"); 
            command = "FF58A7";
            break;
        case 0xFFD827: 
            Serial.println("D4"); 
            command = "FFD827";
            break;
        case 0xFFE817: 
            Serial.println("Quick"); 
            command = "FFE817";
            break;
        case 0xFFC837: 
            Serial.println("Slow"); 
            command = "FFC837";
            break;
        case 0xFF28D7: 
            Serial.println("Red Up"); 
            command = "FF28D7";
            break;
        case 0xFFA857: 
            Serial.println("Green Up"); 
            command = "FFA857";
            break;
        case 0xFF6897: 
            Serial.println("Blue Up"); 
            command = "FF6897";
            break;
        case 0xFF08F7: 
            Serial.println("Red Down"); 
            command = "FF08F7";
            break;
        case 0xFF8877: 
            Serial.println("Green Down"); 
            command = "FF8877";
            break;
        case 0xFF48B7: 
            Serial.println("Blue Down"); 
            command = "FF48B7";
            break;
        case 0xFF30CF: 
            Serial.println("DIY1"); 
            command = "FF30CF";
            break;
        case 0xFFB04F: 
            Serial.println("DIY2"); 
            command = "FFB04F";
            break;
        case 0xFF708F: 
            Serial.println("DIY3"); 
            command = "FF708F";
            break;
        case 0xFF10EF: 
            Serial.println("DIY4"); 
            command = "FF10EF";
            break;
        case 0xFF906F: 
            Serial.println("DIY5"); 
            command = "FF906F";
            break;
        case 0xFF50AF: 
            Serial.println("DIY6"); 
            command = "FF50AF";
            break;
        case 0xFFF00F: 
            Serial.println("Auto"); 
            command = "FFF00F";
            break;
        case 0xFFD02F: 
            Serial.println("Flash"); 
            command = "FFD02F";
            break;
        case 0xFF20DF: 
            Serial.println("Jump3"); 
            command = "FF20DF";
            break;
        case 0xFFA05F: 
            Serial.println("Jump7"); 
            command = "FFA05F";
            break;
        case 0xFF609F: 
            Serial.println("Fade3"); 
            command = "FF609F";
            break;
        case 0xFFE01F: 
            Serial.println("Fade7"); 
            command = "FFE01F";
            break;
        default:
            // Decode Infrared Code
            Serial.println("Decoded Button");
            // Serial.println(results.value);    
	}
	
    // If We've decoded correctly now publish this to the MQTT Topic
    if (command != "") {
        publishMQTTCommand(command);
    }

    // Do not get immediate repeat
	delay(1000); 
}

void decodeIR() {
    // Have we received an IR signal?
    if (irrecv.decode(&results)) {
        // We got results so now decode that input
        decodeIRResult();

        // Receive the next value
        irrecv.resume(); 
    }
}

void setup() {
    // put your setup code here, to run once:

    // Connect the Serial Monitor
    connectSerial();
    Serial.println(F("NodeMCU MQTT IR Receiver"));

    // Connect the IR Receiver
    connectIR();

    // Connect to the WiFi
    connectWiFi();
}

void loop() {
    // put your main code here, to run repeatedly:

    // Ensure the connection to the MQTT server is alive (this will make the first
    // connection and automatically reconnect when disconnected).  See the MQTT_connect
    // function definition further below.
    connectMQTT();

    decodeIR();
}