#include "DllExp.h"
#pragma once

typedef   void   (CALLBACK   *EventCallback)();
DLLIMP void  WINAPI SetEventCallback(EventCallback func); 
void WINAPI FireEvent();