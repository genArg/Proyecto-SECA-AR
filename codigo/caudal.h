//! Macros
#ifndef CAUDAL_H
#define CAUDAL_H

#include "tiempo.h"
#include "define.h"

#ifdef __cplusplus
extern "C" {
#endif

//! puntero a un caudalimetro de 1 señal
typedef struct caudalimetro_s * caudalimetro_t;

//! verifica señal del caudalimetro
void Verificar(caudalimetro_t cauda);

//! Inicializa los valores del caudalimetro
void IniciarValores(caudalimetro_t cauda);

//
void ParametrizarCaudalimetro(caudalimetro_t cauda, uint16_t constante);

//! Realiza el calculo del caudal en litros por hora en funcion al tiempo
void CalculoCaudal(caudalimetro_t cauda);

#ifdef __cplusplus
}
#endif

#endif