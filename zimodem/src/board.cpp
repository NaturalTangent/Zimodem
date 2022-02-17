#include "board.h"

bool pinSupport[MAX_PIN_NO];

int pinDCD = DEFAULT_PIN_DCD;
int pinCTS = DEFAULT_PIN_CTS;
int pinRTS = DEFAULT_PIN_RTS;
int pinDSR = DEFAULT_PIN_DSR;
int pinDTR = DEFAULT_PIN_DTR;
int pinRI = DEFAULT_PIN_RI;
int dcdStatus = LOW;
int dcdActive = DEFAULT_DCD_HIGH;
int dcdInactive = DEFAULT_DCD_LOW;
int ctsActive = DEFAULT_CTS_HIGH;
int ctsInactive = DEFAULT_CTS_LOW;
int rtsActive = DEFAULT_RTS_HIGH;
int rtsInactive = DEFAULT_RTS_LOW;
int riActive = DEFAULT_RI_HIGH;
int riInactive = DEFAULT_RI_LOW;
int dtrActive = DEFAULT_DTR_HIGH;
int dtrInactive = DEFAULT_DTR_LOW;
int dsrActive = DEFAULT_DSR_HIGH;
int dsrInactive = DEFAULT_DSR_LOW;


void s_pinWrite(uint8_t pinNo, uint8_t value)
{
  if(pinSupport[pinNo])
  {
    digitalWrite(pinNo, value);
  }
}

int getDefaultCtsPin()
{
#if defined(ZIMODEM_ESP32) || defined(ESP8266_D1_MINI)
  return DEFAULT_PIN_CTS;
#else
  if((ESP.getFlashChipRealSize()/1024)>=4096) // assume this is a striketerm/esp12e
    return DEFAULT_PIN_CTS;
  else
    return 0;
#endif 
}