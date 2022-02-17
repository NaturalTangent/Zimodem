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

#ifndef pet2asc_h_INCLUDED
#define pet2asc_h_INCLUDED

#include "board.h"
#include <FS.h>

extern char petToAsc(char c);
extern bool ascToPet(char *c, Stream *stream);
extern char ascToPetcii(char c);
extern bool handleAsciiIAC(char *c, Stream *stream);
extern int modifierCompare(const char *match1, const char *match2);

#endif