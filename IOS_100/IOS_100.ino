/* 
  Internet Of Schwitz
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

#include <OneWire.h>
#include <DallasTemperature.h>
#include "EmonLib.h"                 // Include Emon Library
#include <Wire.h> // SDA 2>3 SCL 14>2
#include "IOS_Defines.h"

EnergyMonitor escape;                   // Create an instance

// Data wire is plugged into this pin
#define ONE_WIRE_BUS 2
//#define TEMPERATURE_PRECISION 9
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
int oneWireDevices; // Number of temperature devices found
//  using two thermomolators
DeviceAddress Therm1, Therm2;
boolean t1 = false;
boolean t2 = false;
float latest_t1 = 500.0;
float latest_t2 = 500.0;


boolean schvitzing = false;

int HEATERstate = OFF;    
unsigned long lastTime = 0;
unsigned long thisTime = 0;
const long blinkTime = 500;
int rgb = 0;
int targetFade = 0;
int targetPWM[3] = {0,0,0};
int targetLED[3] = {LCD_RED,LCD_GREEN,LCD_BLUE};
int seed0[3] = {0.0,1023.0,1023.0}; // MAX_LED,MIN_LED,MIN_LED};  // red
int seed1[3] = {1023.0,1023.0,0.0}; // MIN_LED,MIN_LED,MAX_LED};  // blue

int n;  //mutipurpose LCD debugger

char inChar;
boolean testing = false;
unsigned int testTime;
int testInterval = 2000;
int rawADC;

unsigned long schvitzTime;
long schvinterval = 500;

double lastI = 0;
double thisI = 0;
int meanI;
int sampleCounter;
int sumI;

void setup() {
  Serial.begin(230400);
  Wire.begin();
  sensors.begin();
  setThings();
  printRules();
  ST7036_init();
  escapeInit();
  
  // Get report of oneWire devices on the wire
  countBirds();

  
  getTemps();
  printStateToLCD();
  
}


void loop()
{
  checkPWRswitch();
  
  if(schvitzing){
    if(millis() - schvitzTime > schvinterval){
      schvitzTime = millis();
      getAmps();
      getTemps();
      fadeLEDs(); // fade to temp reading
      printStateToLCD();
    }
  }

  serial();

  if(testing){
    if(millis() - testTime > testInterval){
      testTime = millis();  
//      getAmps();
      testLCD_LEDs(); // scroll R,G,B
//      fadeSeeds();  // fade red to blue
//      Serial.println(testTime);
    }
  }
  
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
      case 'x':
        endSchvitz();
        break;
      case 'i':
        Serial.print("Irms = "); Serial.println(currentOnly());
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

double currentOnly(){
    double Irms = escape.calcIrms(1480);  // Calculate Irms only for that many samples
    return Irms;
}

void setThings(){
  pinMode(PWR,INPUT); 
  pinMode(HEATER,OUTPUT); digitalWrite(HEATER,OFF);
  pinMode(LCD_RED,OUTPUT); pinMode(LCD_GREEN,OUTPUT); pinMode(LCD_BLUE,OUTPUT);
  LEDwhite();
//  LEDoff();
  
}



void getAmps(){
  thisI = currentOnly();
  if(thisI >= lastI+0.01 || thisI <= lastI-0.01){
    lastI = thisI;
    Serial.print(thisI*PRIMARY_V);         // Apparent power P=VI
    Serial.print("W, ");
    Serial.print(thisI);          // Irms
    Serial.println("A");
  }
}


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



void escapeInit(){
  escape.voltage(A0, PRIMARY_V, 0.6);   // Voltage: input pin, calibration, phase_shift
  escape.current(A0, CALIBRATION);      // get current: input pin, calibration
}


void getTemps(){
    if(oneWireDevices > 0){  
      sensors.requestTemperatures();
      if(t1){
        Serial.print("t_1  ");
//        printData(Therm1);
        latest_t1 = getFahrenheit(Therm1);
        latest_t1 = constrain(latest_t1,-10.0,250.0);
        Serial.print(latest_t1);
        
      }else{
        Serial.println("No Therm1");
      }
      if(t2){
        Serial.print("\tt_2  ");
//        printData(Therm2);
        latest_t2 = getFahrenheit(Therm2);
        latest_t2 = constrain(latest_t2,-10.0,250.0);
        Serial.println(latest_t2);
      } else {
        Serial.println("\tNo Therm2");
      }
    } else {
      Serial.println("No Thermomathings");
      latest_t1 = latest_t2 = 0.0;
    }
  
}


void fadeLEDs(){
//  if(t1){
    float f = map(latest_t1,-10.0,250.0,1023.0,0.0);
    setPWMfade(f);
    Serial.print("fade "); Serial.println(f);
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

