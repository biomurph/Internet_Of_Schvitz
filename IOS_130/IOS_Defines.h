

/*
 *  PINS
 */
 // THING
//#define HEATER 5     // Sauna Power Control
//#define WIRE  13   // 1Wire temp sensor com
//#define PWR 4    // toggle switch for turning it all on
//#define LCD_RED 12    // RED LED backlight
//#define LCD_GREEN 16  // GREEN LED backlight
//#define LCD_BLUE  15  // BLUE LED backlight


// FEATHER
#define HEATER 0     // Sauna Power Control
#define WIRE  2   // 1Wire temp sensor com
#define PWR 16    // toggle switch for turning it all on
#define LCD_RED 14    // RED LED backlight
#define LCD_GREEN 13  // GREEN LED backlight
#define LCD_BLUE  12  // BLUE LED backlight


/*
 *  USEFUL STUFF
 */
#define ON  HIGH
#define OFF LOW
#define MAX_LED 0
#define MIN_LED 1023
#define MAX_PWM 1023
#define MIN_PWM 0


/*
 * stuff to set the AC current sensor
 *
 */
//#define CALIBRATION 64.3  // see AC_Current_Notes.h
//#define PRIMARY_V 205.0   // empirically tested at measurement site
#define OFFSET 524       // found emperically

#define TEST_INT 1024
#define TEST_FLOAT 10.24

// LCD Display stuff
#define LCD_ADDRESS 0x3C  // 0x78
#define DISP_CMD  0x00
#define SET_DDRAM_CMD  = 0x80
#define RAM_WRITE_CMD 0x40
#define CLEAR_DISP_CMD 0x01
#define HOME_CMD 0x02
#define LINE_2  0xC0
//#define DOUBLE_HEIGHT 0x3C
//#define SINGLE_HEIGHT 0x38

// Thermothing Stuff
#define ONE_WIRE_BUS 2                // Data wire is plugged into this pin
OneWire oneWire(ONE_WIRE_BUS);        // Setup a oneWire instance
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature.
int oneWireDevices;                   // Number of temperature devices found
DeviceAddress Therm1, Therm2;         // using two thermomolators
boolean t1 = false;                   // used to tell if thermathing 1 is attached
boolean t2 = false;                   // used to tell if thermathing 2 is attached
float latest_t1 = 0.0;                // arbitrary temp to start
float last_t1 = 0.0;
float latest_t2 = 0.0;                // arbitrary temp to start
float last_t2 = 0.0;

//  Sauna Stuff
boolean schvitzing = false;
int HEATERstate = OFF;
unsigned long schvitzTime = 0;
long schvinterval = 20000; // milliseconds

// LED Stuff
const long blinkTime = 500;
int rgb = 0;
int targetFade = 0;
int targetPWM[3] = {0,0,0};
int targetLED[3] = {LCD_RED,LCD_GREEN,LCD_BLUE};
int seed0[3] = {1023.0,1023.0,0.0}; // MIN_LED,MIN_LED,MAX_LED};  // blue
int seed1[3] = {0.0,1023.0,1023.0}; // MAX_LED,MIN_LED,MIN_LED};  // red

int n;  //mutipurpose LCD debugger
int eLog = 0;
char inChar;  // Serial reads into this
boolean logEvent = false;

//  Testing related
boolean testing = false;
unsigned int testTime;
int testInterval = 5000;
int rawADC;



// AC current related
double lastI = 0;
double Irms = 0;
float Irms10x;
unsigned long sample, startTime;
int newSample;
int sampleCount = 0;
double sumI = 0;

// WiFi Stuff
boolean WiFi_Connected = false;
boolean MQTT_Connected = false;
//time_t WiFiDisconnectTime;
//time_t MQTTdisconnectTime;

// EEPROM Stuff
byte EEdress = 0;

//NTP Stuff
boolean NTP_Sync = false;
boolean NTP_Set = false;
