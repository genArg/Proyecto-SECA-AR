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

//! definiciones del codigo del manometros
#define NUMB_PRESS 5
#define pin_analogico_0 A14
#define LEER_PRESION_0 analogRead(pin_analogico_0) // Leer el valor del pin analógico A14
#define pin_analogico_1 A15
#define LEER_PRESION_1 analogRead(pin_analogico_1) // Leer el valor del pin analógico A15
#define PRESION_MINIMA 0
#define CUENTAS_ADC 1024 // caracteristicas del ADC de la placa utilizada
#define TENSION_ADC 5 // tension maxima del ADC

//! definiciones para entradas y salidas digitales
#define PIN_HAPPY 22
#define PIN_READY 24
#define PIN_OUT_COMPRESOR 26
#define PIN_FORZAR_MEDICION 28 // tentativo a modificarlo como interrupcion


//! definiciones de modulo tarjeta SD interface SPI
#define PIN_SPI_SS    (53)
#define PIN_SPI_MOSI  (51)
#define PIN_SPI_MISO  (50)
#define PIN_SPI_SCK   (52)

#ifdef __cplusplus
}
#endif

#endif