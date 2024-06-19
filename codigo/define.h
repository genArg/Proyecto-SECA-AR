//! Macros
#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

//! definiciones del codigo de caudal
#define pin_entrada 51 //!> bsp
#define LEER_ENTRADA digitalRead(pin_entrada) //!> bsp
#define NUMB_ELEMENTS 5 // numero de elementos del vector
#define CAUDAL_MINIMO 0 //caudal minimo como filtro

//! definiciones del codigo de tiempo

#ifdef __cplusplus
}
#endif

#endif