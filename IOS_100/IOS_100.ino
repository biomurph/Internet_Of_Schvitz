

/*
  Internet Of Schvitz
  Target: SparkFun 8266 Thing Dev
          Adafruit Feather

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

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <OneWire.h>
#include <DallasTemperature.h>
//#include "EmonLib.h"                 // Include Emon Library
#include <Wire.h> // SDA 2>3 SCL 14>2
#include "IOS_Defines.h"
#include "IO_config.h"  // magic numbers

//EnergyMonitor escape;                 // Create an Emon instance

// Thermothing Stuff
#define ONE_WIRE_BUS 2                // Data wire is plugged into this pin
//#define TEMPERATURE_PRECISION 9       // Default should be 12
OneWire oneWire(ONE_WIRE_BUS);        // Setup a oneWire instance
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature.
int oneWireDevices;                   // Number of temperature devices found
DeviceAddress Therm1, Therm2;         // using two thermomolators
boolean t1 = false;                   // used to tell if thermathing 1 is attached
boolean t2 = false;                   // used to tell if thermathing 2 is attached
float latest_t1 = 0.0;              // arbitrary temp to start
float last_t1 = 0.0;
float latest_t2 = 0.0;              // arbitrary temp to start
float last_t2 = 0.0;

//  Sauna Stuff
boolean schvitzing = false;
int HEATERstate = OFF;
unsigned long lastTime = 0;
unsigned long thisTime = 0;

// LED Stuff
const long blinkTime = 500;
int rgb = 0;
int targetFade = 0;
int targetPWM[3] = {0,0,0};
int targetLED[3] = {LCD_RED,LCD_GREEN,LCD_BLUE};
int seed0[3] = {0.0,1023.0,1023.0}; // MAX_LED,MIN_LED,MIN_LED};  // red
int seed1[3] = {1023.0,1023.0,0.0}; // MIN_LED,MIN_LED,MAX_LED};  // blue

int n;  //mutipurpose LCD debugger

char inChar;  // Serial reads into this

//  Testing related
boolean testing = false;
unsigned int testTime;
int testInterval = 5000;
int rawADC;

// Schvitz related
unsigned long schvitzTime;
long schvinterval = 5000;

// AC current related
double lastI = 0;
double Irms = 0;
int sumI;
unsigned long sample, start;
int newSample;
int sampleCount = 0;
double sumI = 0;

// Adafruit IO Stuff
/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'insideSauna' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish insideSauna = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/insideSauna");
Adafruit_MQTT_Publish outsideSauna = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/outsideSauna");
Adafruit_MQTT_Publish heaterCurrent = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/heaterCurrent");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(230400);
  Wire.begin();       // begin OneWire
  sensors.begin();    // begin Dallas tempo sensers
  setThings();        // pin directions, LED color
  printRules();       // serial print key commands
  ST7036_init();      // fire up the LCD
  // escapeInit();       // set things for the AC current sensor
  countBirds();       // get report of oneWire devices on the wire
  getTemps();         // get the first temp readings
  printStateToLCD();  // print latest info to the LCD

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());


}


void loop()
{
  checkPWRswitch(); // is the power switch on?

  if(testing){  // testing things
    readAndPrintIsensor();
//    Serial.print("Irms "); Serial.println(calcIrms());
    testing = false;
    if(millis() - testTime > testInterval){
      testTime = millis();
//      getAmps();
//      testLCD_LEDs(); // scroll R,G,B
//      fadeSeeds();  // fade red to blue
//      Serial.println(testTime);
    }
  } else {
    schvitzLoop();

  }

  serial();                 // go see if we have serial stuff to do

}

void schvitzLoop(){
  if(millis() - schvitzTime >= schvinterval){
    schvitzTime = millis();
    getTemps();           // get temp readings
    calcIrms();
    printToSerial();
    printStateToLCD();    // print latest info to LCD
    if(schvitzing){
      fadeLEDs();           // fade to t2 reading (inside Sauna)
    } else {
      LEDwhite();  
    }
//    sendToIO(); // broken?
  }
}


void printToSerial(){
//  csv: temp1 | temp2 | AC_current |
  Serial.print(latest_t1); Serial.print("\t");
  Serial.print(latest_t2); Serial.print("\t");
  Serial.print(Irms); Serial.print("\t");
  
  Serial.println();
}



void serial(){
  while(Serial.available()){
    inChar = Serial.read();

    switch(inChar){

      case'?':
        printRules();
        break;
      case 'a':
        rawADC = analogRead(A0);
        Serial.print("rawADC = "); Serial.println(rawADC);
        break;
      case 't':
        testing = true;
        testTime = millis();
        sampleCounter = 0;
        Serial.println("starting test");
        break;
      case 'p':
        testing = false;
        Serial.println("end test");
        break;
      case 'x':
        endSchvitz();
        break;
      case 'i':
        Serial.print("Irms = "); Serial.println(Irms);
        break;
      case 's':
        startSchvitz();
        break;
      case 'b':
        countBirds();
        break;  // beak;
      case 'd':  // for degrees...
        Serial.println("getting temps... ");
        sensors.requestTemperatures();
        printData(Therm1);
        printData(Therm2);
        break;
      default: Serial.print("echo "); Serial.println(inChar);
        break;
    }
  }
}


void toggleHEATER(){
  thisTime = millis();
  if(thisTime - lastTime >= blinkTime) {
    lastTime = thisTime;
    HEATERstate = !HEATERstate;
    digitalWrite(HEATER,HEATERstate);
  }
}

//double currentOnly(){
//    double Irms = escape.calcIrms(1480);  // Calculate Irms only for that many samples
//    return Irms;
//}

void setThings(){
  pinMode(PWR,INPUT);
  pinMode(HEATER,OUTPUT); digitalWrite(HEATER,OFF);
  pinMode(LCD_RED,OUTPUT); pinMode(LCD_GREEN,OUTPUT); pinMode(LCD_BLUE,OUTPUT);
  LEDwhite();

}



//void getAmps(){
//  thisI = currentOnly();
//  if(thisI >= lastI+0.01 || thisI <= lastI-0.01){
//    lastI = thisI;
//    Serial.print(thisI*PRIMARY_V);         // Apparent power P=VI
//    Serial.print("W, ");
//    Serial.print(thisI);          // Irms
//    Serial.println("A");
//  }
//}


void printRules(){
  Serial.println("\nInternet Of Schvitz  v1.0.0");

        Serial.println("Press 'a' to get raw ADC reading");
        Serial.println("Press 't' to start test");
        Serial.println("Press 'x' to soft reset");
        Serial.println("Press 'i' to run currentOnly()");
        Serial.println("Press 's' to schvitz!");
        Serial.println("Press 'b' to find 1 wire temp sensors");
        Serial.println("Press 'd' to measure degrees");
//        Serial.println("");
        Serial.println("Press ? to print this again");
}



//void escapeInit(){
//  escape.voltage(A0, PRIMARY_V, 0.6);   // Voltage: input pin, 205V, phase_shift
//  escape.current(A0, CALIBRATION);      // get current: input pin, 64.3
//}


void getTemps(){
    if(oneWireDevices > 0){
      sensors.requestTemperatures();
      if(t1){
//        Serial.print("t_1  ");
//        printData(Therm1);
        latest_t1 = getFahrenheit(Therm1);
        latest_t1 = constrain(latest_t1,-10.0,250.0);
//        Serial.print(latest_t1);
//      }else{
//        Serial.println("No Therm1");
      }
//      Serial.print('\t');
      if(t2){
//        Serial.print("\tt_2  ");
//        printData(Therm2);
        latest_t2 = getFahrenheit(Therm2);
        latest_t2 = constrain(latest_t2,-10.0,250.0);
//        Serial.println(latest_t2);
//      } else {
//        Serial.println("No Therm2");
      }
    } else {
      Serial.println("No Thermomathings");
      latest_t1 = latest_t2 = -10.0;
    }

}


void fadeLEDs(){
//  if(t1){
    float f = map(latest_t2,-10.0,250.0,1023.0,0.0);
    setPWMfade(f);
//    Serial.print("fade "); Serial.println(f);
//  } else {
//    LEDwhite();
//  }
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
  schvitzTime = millis();
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

void sendToIO(){
  if (! insideSauna.publish(latest_t2)) {
    Serial.println(F("IO: t2 Failed"));
  } else {
    Serial.println(F("IO: t2 OK!"));
  }
  if (! outsideSauna.publish(latest_t1)) {
    Serial.println(F("IO: t1 Failed"));
  } else {
    Serial.println(F("IO: t1 OK!"));
  }
  if (! heaterCurrent.publish(Irms)) {
    Serial.println(F("IO: Irms Failed"));
  } else {
    Serial.println(F("IO: Irms OK!"));
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
