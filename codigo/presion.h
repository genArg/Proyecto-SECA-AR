//! Macros
#ifndef PRESION_H
#define PRESION_H

#include "define.h"

typedef struct manometro_s* manometro_t;

void InicializarManometro(manometro_t man);

void TomarValor(manometro_t man);

void ParametrizarPresion(manometro_t man);


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif