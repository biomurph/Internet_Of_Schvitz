

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
#define OFFFSET 524       // found emperically

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


