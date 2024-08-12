//! Macros
#ifndef TIEMPO_H
#define TIEMPO_H

#include "define.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct reloj_s* reloj_t;

  // es el tiempo que se toma del RTC
  uint32_t TomarTiempo(reloj_t reloj);

//////////////////////////////////////////////////////
  //! puntero a un reloj de 1 señal

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
    
    reloj_t reloj = (reloj_t)malloc(sizeof(struct reloj_s));

    if (reloj) {
      memset(reloj, 0, sizeof(reloj));
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


  /////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif