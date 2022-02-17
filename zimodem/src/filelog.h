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

#ifndef filelog_h_included
#define filelog_h_included


#include "board.h"
#include <stdint.h>

enum LogOutputState
{
  LOS_NADA=0,
  LOS_SocketIn=1,
  LOS_SocketOut=2,
  LOS_SerialIn=3,
  LOS_SerialOut=4
};

extern unsigned long expectedSerialTime;
extern boolean logFileOpen;
extern bool logFileDebug;
extern File logFile; 

extern void logSerialOut(const uint8_t c);
extern void logSocketOut(const uint8_t c);
extern void logSerialIn(const uint8_t c);
extern void logSocketIn(const uint8_t c);
extern void logSocketIn(const uint8_t *c, int n);
extern void logPrint(const char* msg);
extern void logPrintln(const char* msg);
extern void logPrintf(const char* format, ...);
extern void logPrintfln(const char* format, ...);
extern char *TOHEX(const char *s, char *hex, const size_t len);
extern char *TOHEX(long a);
extern char *TOHEX(int a);
extern char *TOHEX(unsigned int a);
extern char *TOHEX(unsigned long a);
extern char *tohex(uint8_t a);
extern char *TOHEX(uint8_t a);
extern uint8_t FROMHEX(uint8_t a1, uint8_t a2);
extern char *FROMHEX(const char *hex, char *s, const size_t len);

#endif