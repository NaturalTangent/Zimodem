#ifndef zimodem_h_INCLUDED
#define zimodem_h_INCLUDED

#include "rt_clock.h"
#include "zstream.h"

enum BaudState
{
  BS_NORMAL,
  BS_SWITCH_TEMP_NEXT,
  BS_SWITCHED_TEMP,
  BS_SWITCH_NORMAL_NEXT
};

extern void checkFactoryReset();

extern RealTimeClock zclock;
extern  ZStream streamMode;
extern int baudRate;
extern int tempBaud;
extern String wifiSSI;

extern String wifiPW;
extern String hostname;
extern IPAddress *staticIP;
extern IPAddress *staticDNS;
extern IPAddress *staticGW;
extern IPAddress *staticSN;

extern BaudState baudState; 
extern SerialConfig serialConfig;
extern  bool wifiConnected;

extern bool connectWifi(const char* ssid, const char* password, IPAddress *ip, IPAddress *dns, IPAddress *gateWay, IPAddress *subNet);
extern void setNewStaticIPs(IPAddress *ip, IPAddress *dns, IPAddress *gateWay, IPAddress *subNet);
extern void changeBaudRate(int baudRate);
extern void changeSerialConfig(SerialConfig conf);
extern void checkBaudChange();

#endif