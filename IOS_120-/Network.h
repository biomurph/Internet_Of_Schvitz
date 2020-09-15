

// ADAFRUIT_IO_KEY "d91fec2e1cb067ced5952727a3b5d34bce5d3635"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Tentacle"
#define WLAN_PASS       "agentofthesquid"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "biomurph"
#define AIO_KEY         "d91fec2e1cb067ced5952727a3b5d34bce5d3635"

/************************* UDP to get NTP *********************************/

// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";
unsigned int localPort = 8888;  // local port to listen for UDP packets
WiFiUDP Udp;

const int timeZone = -5;  // Eastern Standard Time (USA)
