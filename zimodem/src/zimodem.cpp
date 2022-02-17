/*
   Copyright 2016-2019 Bo Zimmerman

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. 
*/


#include "board.h"
#include "zimodem.h"
#include "zmode.h"
#include "pet2asc.h"
#include "rt_clock.h"
#include "filelog.h"
#include "serout.h"
#include "connSettings.h"
#include "wificlientnode.h"
#include "stringstream.h"
#include "phonebook.h"
#include "wifiservernode.h"
#include "zstream.h"
#include "proto_http.h"
#include "proto_ftp.h"
#include "zcommand.h"
#include "zconfigmode.h"
#include "zprint.h"
#include "zbrowser.h"

#ifdef INCLUDE_SD_SHELL
#  ifdef INCLUDE_HOSTCM
#    include "zhostcmmode.h"
#  endif
#  include "proto_xmodem.h"
#  include "proto_zmodem.h"
#  include "proto_kermit.h"
static bool browseEnabled = false;
#endif


ZStream streamMode;
//static ZSlip slipMode; // not yet implemented
RealTimeClock zclock(0);

#ifdef INCLUDE_SD_SHELL
#  ifdef INCLUDE_HOSTCM
     static ZHostCMMode hostcmMode;
#  endif
   static ZBrowser browseMode;
#endif


bool wifiConnected =false;
String wifiSSI;
String wifiPW;
String hostname;
IPAddress *staticIP = null;
IPAddress *staticDNS = null;
IPAddress *staticGW = null;
IPAddress *staticSN = null;
SerialConfig serialConfig = DEFAULT_SERIAL_CONFIG;
int baudRate=DEFAULT_BAUD_RATE;
static int dequeSize=1+(DEFAULT_BAUD_RATE/INTERNAL_FLOW_CONTROL_DIV);
BaudState baudState = BS_NORMAL; 
int tempBaud = -1; // -1 do nothing


#ifdef ZIMODEM_ESP32
static unsigned long resetPushTimer=0;
#endif


static void setHostName(const char *hname)
{
#ifdef ZIMODEM_ESP32
      tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, hname);
#else
      WiFi.hostname(hname);
#endif
}

void setNewStaticIPs(IPAddress *ip, IPAddress *dns, IPAddress *gateWay, IPAddress *subNet)
{
  if(staticIP != null)
    free(staticIP);
  staticIP = ip;
  if(staticDNS != null)
    free(staticDNS);
  staticDNS = dns;
  if(staticGW != null)
    free(staticGW);
  staticGW = gateWay;
  if(staticSN != null)
    free(staticSN);
  staticSN = subNet;
}

bool connectWifi(const char* ssid, const char* password, IPAddress *ip, IPAddress *dns, IPAddress *gateWay, IPAddress *subNet)
{
  while(WiFi.status() == WL_CONNECTED)
  {
    WiFi.disconnect();
    delay(100);
    yield();
  }
#ifndef ZIMODEM_ESP32
  if(hostname.length() > 0)
    setHostName(hostname.c_str());
#endif
  WiFi.mode(WIFI_STA);
  if((ip != null)&&(gateWay != null)&&(dns != null)&&(subNet!=null))
  {
    if(!WiFi.config(*ip,*gateWay,*subNet,*dns))
      return false;
  }
  WiFi.begin(ssid, password);
  if(hostname.length() > 0)
    setHostName(hostname.c_str());
  bool amConnected = (WiFi.status() == WL_CONNECTED) && (strcmp(WiFi.localIP().toString().c_str(), "0.0.0.0")!=0);
  int WiFiCounter = 0;
  while ((!amConnected) && (WiFiCounter < 30))
  {
    WiFiCounter++;
    if(!amConnected)
      delay(500);
    amConnected = (WiFi.status() == WL_CONNECTED) && (strcmp(WiFi.localIP().toString().c_str(), "0.0.0.0")!=0);
  }
  wifiConnected = amConnected;
  if(!amConnected)
    WiFi.disconnect();
#ifdef SUPPORT_LED_PINS
  s_pinWrite(DEFAULT_PIN_WIFI,wifiConnected?DEFAULT_WIFI_ACTIVE:DEFAULT_WIFI_INACTIVE);
#endif
  return wifiConnected;
}

void checkBaudChange()
{
  switch(baudState)
  {
    case BS_SWITCH_TEMP_NEXT:
      changeBaudRate(tempBaud);
      baudState = BS_SWITCHED_TEMP;
      break;
    case BS_SWITCH_NORMAL_NEXT:
      changeBaudRate(baudRate);
      baudState = BS_NORMAL;
      break;
    default:
      break;
  }
}

void changeBaudRate(int baudRate)
{
  flushSerial(); // blocking, but very very necessary
  delay(500); // give the client half a sec to catch up
  logPrintfln("Baud change to %d.\n",baudRate);
  debugPrintf("Baud change to %d.\n",baudRate);
  dequeSize=1+(baudRate/INTERNAL_FLOW_CONTROL_DIV);
  debugPrintf("Deque constant now: %d\n",dequeSize);
#ifdef ZIMODEM_ESP32
  HWSerial.changeBaudRate(baudRate);
#else
  HWSerial.begin(baudRate, serialConfig);  //Change baud rate
#endif
#ifdef SUPPORT_LED_PINS
  s_pinWrite(DEFAULT_PIN_HS,(baudRate>=DEFAULT_HS_BAUD)?DEFAULT_HS_ACTIVE:DEFAULT_HS_INACTIVE);
#endif  
}

void changeSerialConfig(SerialConfig conf)
{
  flushSerial(); // blocking, but very very necessary
  delay(500); // give the client half a sec to catch up
  debugPrintf("Config changing %d.\n",(int)conf);
  dequeSize=1+(baudRate/INTERNAL_FLOW_CONTROL_DIV);
  debugPrintf("Deque constant now: %d\n",dequeSize);
#ifdef ZIMODEM_ESP32
  HWSerial.changeConfig(conf);
#else
  HWSerial.begin(baudRate, conf);  //Change baud rate
#endif  
  debugPrintf("Config changed.\n");
}


void checkFactoryReset()
{
#ifdef ZIMODEM_ESP32
    if(!digitalRead(PIN_FACTORY_RESET))
    {
      if(resetPushTimer != 1)
      {
        if(resetPushTimer==0)
        {
          resetPushTimer=millis();
          if(resetPushTimer==1)
            resetPushTimer++;
        }
        else
        if((millis() - resetPushTimer) > 5000)
        {
          SPIFFS.remove(CONFIG_FILE);
          SPIFFS.remove(CONFIG_FILE_OLD);
          SPIFFS.remove("/zphonebook.txt");
          SPIFFS.remove("/zlisteners.txt");
          PhoneBookEntry::clearPhonebook();
          SPIFFS.end();
          SPIFFS.format();
          SPIFFS.begin();
          PhoneBookEntry::clearPhonebook();
          if(WiFi.status() == WL_CONNECTED)
            WiFi.disconnect();
          baudRate = DEFAULT_BAUD_RATE;
          commandMode.loadConfig();
          PhoneBookEntry::loadPhonebook();
          dcdStatus = dcdInactive;
          s_pinWrite(pinDCD,dcdStatus);
          wifiSSI="";
          wifiConnected=false;
          delay(500);
          zclock.reset();
          commandMode.reset();
          resetPushTimer=1;
        }
      }
    }
    else
    if(resetPushTimer != 0)
      resetPushTimer=0;
#endif
}

