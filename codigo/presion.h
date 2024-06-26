//! Macros
#ifndef PRESION_H
#define PRESION_H

#include "define.h"

typedef struct manometro_s* manometro_t;

void InicializarManometro(manometro_t man);

void TomarValor(manometro_t man, uint16_t entrada);

void ParametrizarPresionMax(manometro_t man, uint16_t valor);

void ParametrizarCorrienteMax(manometro_t man, uint16_t valor);

void ParametrizarPresionMin(manometro_t man, uint16_t valor);

void ParametrizarCorrienteMin(manometro_t man, uint16_t valor);

void ParametrizarResistencia(manometro_t man, uint16_t valor);

uint16_t ValoresManometro(manometro_t man, uint16_t valor);

uint16_t PruevaFuncion(){
return 999;
}

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif