#include "board.h"
#include "zbrowser.h"
#include "zcommand.h"
#include "phonebook.h"
#include "zconfigmode.h"
#include "zimodem.h"

void setup() 
{
  for(int i=0;i<MAX_PIN_NO;i++)
    pinSupport[i]=false;

#ifdef ZIMODEM_ESP32
  Serial.begin(115200); //the debug port
  Serial.setDebugOutput(true);
  debugPrintf("Debug port open and ready.\n");
  for(int i=12;i<=23;i++)
    pinSupport[i]=true;
  for(int i=25;i<=27;i++)
    pinSupport[i]=true;
  for(int i=32;i<=33;i++)
    pinSupport[i]=true;
  pinSupport[36]=true;
  pinSupport[39]=true;
#else


#ifndef ESP8266_D1_MINI
  pinSupport[0]=true;
  pinSupport[2]=true;
  if((ESP.getFlashChipRealSize()/1024)>=4096) // assume this is a strykelink/esp12e
  {
    pinSupport[4]=true;
    pinSupport[5]=true;
    for(int i=9;i<=16;i++)
      pinSupport[i]=true;
    pinSupport[11]=false;
  }
#else
  pinSupport[pinDCD] = true;
  pinSupport[pinCTS] = true;
  pinSupport[pinRTS] = true;
  pinSupport[pinDSR] = true;
  pinSupport[pinDTR] = true;
  pinSupport[pinRI] = false;
#endif

#endif    
  initSDShell();
  currMode = &commandMode;
  if(!SPIFFS.begin())
  {
    SPIFFS.format();
    SPIFFS.begin();
    debugPrintf("SPIFFS Formatted.");
  }
  HWSerial.begin(DEFAULT_BAUD_RATE, DEFAULT_SERIAL_CONFIG);  //Start Serial
#ifdef ZIMODEM_ESP8266
  HWSerial.setRxBufferSize(1024);
#endif
  commandMode.loadConfig();
  PhoneBookEntry::loadPhonebook();
  dcdStatus = dcdInactive;
  s_pinWrite(pinDCD,dcdStatus);
  flushSerial();
#ifdef SUPPORT_LED_PINS
  s_pinWrite(DEFAULT_PIN_WIFI,(WiFi.status() == WL_CONNECTED)?DEFAULT_WIFI_ACTIVE:DEFAULT_WIFI_INACTIVE);
  s_pinWrite(DEFAULT_PIN_HS,(baudRate>=DEFAULT_HS_BAUD)?DEFAULT_HS_ACTIVE:DEFAULT_HS_INACTIVE);
#endif
}

void loop() 
{
  checkFactoryReset();
  if(HWSerial.available())
  {
    currMode->serialIncoming();
  }
  currMode->loop();
  zclock.tick();
}
