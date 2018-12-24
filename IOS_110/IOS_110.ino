

/*
  Internet Of Schvitz
  Target: Adafruit Feather HUZZAH ESP8266

  Bath Controller par excellence

  THING PIN ASSIGNMENTS
  A0 = Sauna current monitor
  2 = SDA for LCD control
  14 = SCL for LCD control
  13 = 1Wire Temperature com
  5 = HEATER Sauna Power Control (on board blue LED inverted)
  4 = PWR power switch for turning it all on (has pulldown)
  12 = LCD_RED LED backlight
  16 = LCD_GREEN LED backlight
  15 = LCD_BLUE LED backlight

  FEATHER PIN ASSIGNMENTS
  A0 = Sauna current monitor
  4 = SDA for LCD control
  5 = SCL for LCD control
  2 = 1Wire Temperature com
  0 = HEATER Sauna Power Control (on board blue LED inverted)
  16 = PWR power switch for turning it all on
  14 = LCD_RED LED backlight
  13 = LCD_GREEN LED backlight
  12 = LCD_BLUE LED backlight

*/

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> // SDA 2>3 SCL 14>2
#include "IOS_Defines.h"
#include "Network.h"
#include <EEPROM.h>


// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'insideSauna' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish insideSauna = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/internet-of-schvitz.insidesauna");
Adafruit_MQTT_Publish outsideSauna = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/internet-of-schvitz.outsidesauna");
Adafruit_MQTT_Publish heaterCurrent = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/internet-of-schvitz.heatercurrent");
Adafruit_MQTT_Publish errorLog = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/internet-of-schvitz.errorlog");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
//void MQTT_connect();

void setup() {
  Serial.begin(115200);
  EEPROM.begin(256);  // set aside space for storing error times and codes
  getEEdress();       // get ready to place error data in next available EEPROM address
  Wire.begin();       // begin OneWire
  sensors.begin();    // begin Dallas tempo sensers
  setThings();        // pin directions, LED color
  printRules();       // serial print key commands
  ST7036_init();      // fire up the LCD
  countBirds();       // get report of oneWire devices on the wire
  getTemps();         // get the first temp readings
  WiFiConnect();      // try to connect to the WiFi access point
  Udp.begin(localPort);
  getAndSyncNTP();    // get the latest time from NTP server
  MQTTconnect();     // try to connect to the IO
  printStateToLCD();  // print latest info to the LCD
  

}


void loop()
{
  checkPWRswitch(); // is the sauna power switch on?

  schvitzLoop();    // measure stuff and send data to other people's computers

  checkSerial();    // go see if we have serial stuff to do

}

void schvitzLoop(){
  if(millis() - schvitzTime >= schvinterval){
    schvitzTime = millis();
    getTemps();                 // get temp readings
    calcIrms();  //might be useful for graphing on the AIO
    printToSerial();
    printStateToLCD();          // print latest info to LCD
    if(schvitzing){
      fadeLEDs();               // fade to t1 reading (inside Sauna) from blue to red
    } else {
      LEDwhite();  
    }
    if(MQTT_Connected){
      sendToIO();
    }
  }
}

void setThings(){
  pinMode(PWR,INPUT);
  pinMode(HEATER,OUTPUT); digitalWrite(HEATER,OFF);
  pinMode(LCD_RED,OUTPUT); pinMode(LCD_GREEN,OUTPUT); pinMode(LCD_BLUE,OUTPUT);
  LEDwhite();

}




void checkPWRswitch(){
  int schwitch = digitalRead(PWR);
  if(schwitch && !schvitzing){
    startSchvitz();
  }else if(!schwitch && schvitzing){
    endSchvitz();
  }
}

void startSchvitz(){
  schvitzTime = millis() - schvinterval;
  schvitzing = true;
  Serial.print("schvitzing = "); Serial.println(schvitzing);
  digitalWrite(HEATER,ON);
  printStateToLCD();
}

void endSchvitz(){
  testing = false;
  schvitzing = false;
  digitalWrite(HEATER,OFF);
  Serial.print("schvitzing = "); Serial.println(schvitzing);
  printStateToLCD();
  LEDwhite();
}


