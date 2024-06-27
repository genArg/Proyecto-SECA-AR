//! Macros
#ifndef TIEMPO_H
#define TIEMPO_H

#include "define.h"

#ifdef __cplusplus
extern "C" {
#endif

// es el tiempo que se toma del RTC
uint16_t TomarTiempo();




/////////////////////////////////////////////////////////////////////////////////////////////////////

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


/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif