//! Macros
#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdbool.h>
#include <pins_arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TRUE (1)
#define FALSE (0)
#define DEBUG TRUE
#define DEBUG_RTC FALSE
#define DEBUG_RTC_2 TRUE

#define OFFSET (7)  // usado para mover el texto

#define N_TOTAL (4)
#define DECIMALES (0)

#define MINPRESSURE 200
#define MAXPRESSURE 1000

// Definición de colores en formato RGB565
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x867d
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF



//! definiciones del codigo de caudal
#define INTERRUPCION_C1 (29)                       // pin de interrupcion
#define INTERRUPCION_C2 (33)                       // pin de interrupcion
#define LEER_ENTRADA digitalRead(INTERRUPCION_C1)  //!> bsp //--------------------------------------------------------->
#define NUMB_ELEMENTS 5                            // numero de elementos del vector
#define CAUDAL_MINIMO 0                            //caudal minimo como filtro
#define SEGUNDO_POR_HORA (3600)                    //3600

//! definiciones del codigo de tiempo
#define DIA 28
#define MES 6
#define YEAR 2024

//! definiciones del codigo del manometros
#define NUMB_PRESS 5
#define PIN_PRESION (A13)  // pin analogico A13 // presion
//#define LEER_PRESION_0 analogRead(PIN_PRESION)  // Leer el valor del pin analógico A13 // presion
#define PIN_NIVEL (A15)                       // pin analogico A15 // nivel
#define LEER_PRESION_1 analogRead(PIN_NIVEL)  // Leer el valor del pin analógico A15 // nivel
#define PRESION_MINIMA 0
#define CUENTAS_ADC 1024  // caracteristicas del ADC de la placa utilizada
#define TENSION_ADC 5     // tension maxima del ADC

//! definiciones para entradas y salidas digitales
#define PIN_HAPPY_LED (45)
#define PIN_READY (37)
#define PIN_OUT_COMPRESOR (41)   //--------------------------------------------------------->
#define INTERRUPCION_PULSE (25)  // pin de interrupcion

//! definiciones de modulo tarjeta SD interface SPI
#define SD_ACTIVE TRUE  // compila el codigo en true
#define PIN_CS (53)     // pin de habilitacion en la placa
#define TIEMPO_SD (15)  // TIEMPO ENTRE GUARDADOS
#define INTENTOS (5)    // numero de intentos para abrir la tarjeta sd

//! definiciones de tiempos ----> UNIDAD DE 0.1 SEG  --> EJ 1 seg EQ 10 o tambien 10 seg EQ 100
#define TIME_BASE (10000)             // -> 10000 equivale a 100 veces en un segundo aproximado  -> 10 milisegundos
#define TIME_VALOR_COMPRESOR (18000)  // valor en segundos cada cuanto se enciende el compresor
#define TIME_100MS (10)
#define TIME_1S (100)
#define TIME_10S (10 * TIME_1S)
#define TIME_60S (60 * TIME_1S)
#define TIME_INT_PULSE (5 * TIME_100MS)  //TIEMPO ENTRE DOS POSBLES INTERRUPCIONES DEL MISMO PIN
#define TIME_INT_C1 (5 * TIME_100MS)
#define TIME_INT_C2 (5 * TIME_100MS)

#ifdef __cplusplus
}
#endif

#endif