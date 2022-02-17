#ifndef board_h_INCLUDED
#define board_h_INCLUDED

#include "Arduino.h"
#include <stdint.h>

#define ESP8266_D1_MINI

//#define TCP_SND_BUF                     4 * TCP_MSS
#define ZIMODEM_VERSION "3.6.5"
const char compile_date[] = __DATE__ " " __TIME__;
#define DEFAULT_NO_DELAY true
#define null 0
//# define USE_DEVUPDATER true // only enable this if your name is Bo

#ifdef ARDUINO_ESP32_DEV
# define ZIMODEM_ESP32
#elif defined(ESP32)
# define ZIMODEM_ESP32
#elif defined(ARDUINO_ESP320)
# define ZIMODEM_ESP32
#elif defined(ARDUINO_NANO32)
# define ZIMODEM_ESP32
#elif defined(ARDUINO_LoLin32)
# define ZIMODEM_ESP32
#elif defined(ARDUINO_ESPea32)
# define ZIMODEM_ESP32
#elif defined(ARDUINO_QUANTUM)
# define ZIMODEM_ESP32
#else
# define ZIMODEM_ESP8266
#endif

#ifdef SUPPORT_LED_PINS
# ifdef GPIO_NUM_0
#   define DEFAULT_PIN_AA GPIO_NUM_16
#   define DEFAULT_PIN_HS GPIO_NUM_15
#   define DEFAULT_PIN_WIFI GPIO_NUM_0
# else
#   define DEFAULT_PIN_AA 16
#   define DEFAULT_PIN_HS 15
#   define DEFAULT_PIN_WIFI 0
# endif
# define DEFAULT_HS_BAUD 38400
# define DEFAULT_AA_ACTIVE LOW
# define DEFAULT_AA_INACTIVE HIGH
# define DEFAULT_HS_ACTIVE LOW
# define DEFAULT_HS_INACTIVE HIGH
# define DEFAULT_WIFI_ACTIVE LOW
# define DEFAULT_WIFI_INACTIVE HIGH
#endif

#ifdef ZIMODEM_ESP32
# define PIN_FACTORY_RESET GPIO_NUM_0
# define DEFAULT_PIN_DCD GPIO_NUM_14
# define DEFAULT_PIN_CTS GPIO_NUM_13
# define DEFAULT_PIN_RTS GPIO_NUM_15 // unused
# define DEFAULT_PIN_RI GPIO_NUM_32
# define DEFAULT_PIN_DSR GPIO_NUM_12
# define DEFAULT_PIN_DTR GPIO_NUM_27
# define debugPrintf Serial.printf
# define INCLUDE_SD_SHELL true
# define DEFAULT_FCT FCT_DISABLED
# define SerialConfig uint32_t
# define UART_CONFIG_MASK 0x8000000
# define UART_NB_BIT_MASK      0B00001100 | UART_CONFIG_MASK
# define UART_NB_BIT_5         0B00000000 | UART_CONFIG_MASK
# define UART_NB_BIT_6         0B00000100 | UART_CONFIG_MASK
# define UART_NB_BIT_7         0B00001000 | UART_CONFIG_MASK
# define UART_NB_BIT_8         0B00001100 | UART_CONFIG_MASK
# define UART_PARITY_MASK      0B00000011
# define UART_PARITY_NONE      0B00000000
# define UART_NB_STOP_BIT_MASK 0B00110000
# define UART_NB_STOP_BIT_0    0B00000000
# define UART_NB_STOP_BIT_1    0B00010000
# define UART_NB_STOP_BIT_15   0B00100000
# define UART_NB_STOP_BIT_2    0B00110000
# define preEOLN serial.prints
# define echoEOLN serial.write
//# define HARD_DCD_HIGH 1
//# define HARD_DCD_LOW 1
//# define INCLUDE_HOSTCM true // do this for special SP9000 modems only
#else  // ESP-8266, e.g. ESP-01, ESP-12E, inverted for C64Net WiFi Modem
#ifndef ESP8266_D1_MINI
# define DEFAULT_PIN_DSR 13
# define DEFAULT_PIN_DTR 12
# define DEFAULT_PIN_RI 14
# define DEFAULT_PIN_RTS 4
# define DEFAULT_PIN_CTS 5 // is 0 for ESP-01, see getDefaultCtsPin() below.
# define DEFAULT_PIN_DCD 2
# define DEFAULT_FCT FCT_RTSCTS
# define RS232_INVERTED 1
# define debugPrintf(...) 
# define preEOLN(...)
# define echoEOLN(...) serial.prints(EOLN)
#else
// Config for the D1 Mini (lite?) without inverted RS232
# define DEFAULT_PIN_DSR 13
# define DEFAULT_PIN_DTR 12
# define DEFAULT_PIN_RI 14
# define DEFAULT_PIN_RTS 4
# define DEFAULT_PIN_CTS 5
# define DEFAULT_PIN_DCD 0 // moved from 2 since 2 has the built-in LED attached. 
# define DEFAULT_FCT FCT_RTSCTS
# define debugPrintf(...) 
# define preEOLN(...)
# define echoEOLN(...) serial.prints(EOLN)
#endif
#endif

#ifdef RS232_INVERTED
# define DEFAULT_DCD_HIGH  HIGH
# define DEFAULT_DCD_LOW  LOW
# define DEFAULT_CTS_HIGH  HIGH
# define DEFAULT_CTS_LOW  LOW
# define DEFAULT_RTS_HIGH  HIGH
# define DEFAULT_RTS_LOW  LOW
# define DEFAULT_RI_HIGH  HIGH
# define DEFAULT_RI_LOW  LOW
# define DEFAULT_DSR_HIGH  HIGH
# define DEFAULT_DSR_LOW  LOW
# define DEFAULT_DTR_HIGH  HIGH
# define DEFAULT_DTR_LOW  LOW
#else
# define DEFAULT_DCD_HIGH  LOW
# define DEFAULT_DCD_LOW  HIGH
# define DEFAULT_CTS_HIGH  LOW
# define DEFAULT_CTS_LOW  HIGH
# define DEFAULT_RTS_HIGH  LOW
# define DEFAULT_RTS_LOW  HIGH
# define DEFAULT_RI_HIGH  LOW
# define DEFAULT_RI_LOW  HIGH
# define DEFAULT_DSR_HIGH  LOW
# define DEFAULT_DSR_LOW  HIGH
# define DEFAULT_DTR_HIGH  LOW
# define DEFAULT_DTR_LOW  HIGH
#endif

#define DEFAULT_BAUD_RATE 1200
#define DEFAULT_SERIAL_CONFIG SERIAL_8N1
#define MAX_PIN_NO 50
#define INTERNAL_FLOW_CONTROL_DIV 380


#ifdef ZIMODEM_ESP32
# include <WiFi.h>
# define ENC_TYPE_NONE WIFI_AUTH_OPEN
# include <HardwareSerial.h>
# include <SPIFFS.h>
# include <Update.h>
# include "SD.h"
# include "SPI.h"
# include "driver/uart.h"
  static HardwareSerial HWSerial(UART_NUM_2);
#else
# include "ESP8266WiFi.h"
# define HWSerial Serial
#endif


#ifdef ZIMODEM_ESP8266
#include <ESP8266WiFi.h>
#endif

extern bool pinSupport[MAX_PIN_NO];
extern int pinDCD;
extern int pinCTS;
extern int pinRTS;
extern int pinDSR;
extern int pinDTR;
extern int pinRI;
extern int dcdStatus;
extern int dcdActive;
extern int dcdInactive;
extern int ctsActive;
extern int ctsInactive;
extern int rtsActive;
extern int rtsInactive;
extern int riActive;
extern int riInactive;
extern int dtrActive;
extern int dtrInactive;
extern int dsrActive;
extern int dsrInactive;

extern void s_pinWrite(uint8_t pinNo, uint8_t value);
extern int getDefaultCtsPin();

#endif