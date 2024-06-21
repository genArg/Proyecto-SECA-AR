#include "tiempo.h"

#if RTC == true

uint16_t TomarTiempo() {
  return 1;
}

#endif

//////////////////////////////////////////////////////
#if RTC == false

uint16_t TomarTiempo() {

unsigned long tiempo = millis();

  return (uint32_t)tiempo;
}

#endif