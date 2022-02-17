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

#ifndef zhostcmmode_h_INCLUDED
#define zhostcmmode_h_INCLUDED

#ifdef INCLUDE_SD_SHELL
#ifdef INCLUDE_HOSTCM
#include "proto_hostcm.h"
#include "zmode.h"

class ZHostCMMode : public ZMode
{
  private:
    void switchBackToCommandMode();
    HostCM *proto = 0;

  public:
    void switchTo();
    void serialIncoming();
    void loop();
};

#endif
#endif

#endif