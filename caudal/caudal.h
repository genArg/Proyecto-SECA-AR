//! Macros
#ifndef CAUDAL_H
#define CAUDAL_H

#define LEE_ENTRADA digitalRead(pin_a)
#define NUMB_ELEMENTS 5 // numero de elementos del vector
#define CAUDAL_MINIMO 0 //caudal minimo como filtro
//! puntero a un caudalimetro de 1 señal
typedef struct caudalimetro_s * caudalimetro_t;

//! verifica señal del caudalimetro
void Verificar(caudalimetro_t cauda);

//! Inicializa los valores del caudalimetro
void IniciarValores(caudalimetro_t cauda);

//
void ParametrizarCaudalimetro();

//! Realiza el calculo del caudal en litros por hora en funcion al tiempo
void CalculoCaudal();

#endif