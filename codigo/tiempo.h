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
  //! puntero a un reloj de 1 señal
  typedef struct reloj_s* reloj_t;

  // Estructra
  struct reloj_s {
    uint16_t segundo;
    uint16_t minuto;
    uint16_t hora;
    uint16_t dia;
    uint16_t mes;
    uint16_t year;
  };


  // Inicia los valores del reloj
  reloj_t InicializarReloj() {
    static struct reloj_s reloj[1];
    memset(reloj, 0, sizeof(reloj));
    if (reloj) {
      reloj->segundo = 0;
      reloj->minuto = 0;
      reloj->hora = 0;
      reloj->dia = DIA;
      reloj->mes = MES;
      reloj->year = YEAR;
    }
    return reloj;
  }

  void UnSegundoReloj(reloj_t reloj) {
    //presion maxima
    reloj->segundo++;
    if (reloj->segundo >= 60) {
      reloj->segundo = 0;
      reloj->minuto++;
      if (reloj->minuto >= 60) {
        reloj->minuto = 0;
        reloj->hora++;
        if (reloj->hora >= 24) {
          reloj->dia++;
          reloj->hora = 0;
          if (reloj->dia >= 31) {
            reloj->mes++;
            reloj->dia = 0;
            if (reloj->mes >= 13) {
              reloj->year++;
              reloj->mes = 0;
            }
          }
        }
      }
    }
  }

  uint16_t TomarTiempo() {
    extern reloj_t reloj_1;
    uint16_t a, b, c;
    c = reloj_1->segundo;
    b = reloj_1->minuto;
    a = reloj_1->hora;
    uint32_t tiempo = (a * 3600) + (b * 60) + c;

    return tiempo;
  }

#endif


  /////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif