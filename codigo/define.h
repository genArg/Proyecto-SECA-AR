//! Macros
#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdbool.h>
#include <pins_arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

//! definiciones del codigo de grafica
#define PANTALLA UTFTGLUE myGLCD(0, A2, A1, A3, A4);  // Asegúrate de usar el controlador y pines correctos para tu pantalla

//! definiciones del codigo de caudal
#define pin_entrada 51 //!> bsp
#define LEER_ENTRADA digitalRead(pin_entrada) //!> bsp
#define NUMB_ELEMENTS 5 // numero de elementos del vector
#define CAUDAL_MINIMO 0 //caudal minimo como filtro

//! definiciones del codigo de tiempo
#define RTC false

//! definiciones del codigo del manometro
#define NUMB_PRESS 5
#define pin_analogico A15
#define LEER_PRESION analogRead(pin_analogico) // Leer el valor del pin analógico A15
#define PRESION_MINIMA 0
#define CUENTAS_ADC 1024 // caracteristicas del ADC de la placa utilizada
#define TENSION_ADC 5 // tension maxima del ADC

#ifdef __cplusplus
}
#endif

#endif