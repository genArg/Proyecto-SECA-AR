//! Macros
#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdbool.h>
#include <pins_arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TRUE 1
#define FALSE 0
#define DEBUG TRUE

#define N_TOTAL (7)
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
#define INTERRUPCION_C1 (20)                       // pin de interrupcion
#define INTERRUPCION_C2 (21)                       // pin de interrupcion
#define LEER_ENTRADA digitalRead(INTERRUPCION_C1)  //!> bsp //--------------------------------------------------------->
#define NUMB_ELEMENTS 5                            // numero de elementos del vector
#define CAUDAL_MINIMO 0                            //caudal minimo como filtro
#define SEGUNDO_POR_HORA (10)                       //3600

//! definiciones del codigo de tiempo
#define RTC false
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
#define PIN_HAPPY_LED (37)
#define PIN_READY (31)
#define PIN_OUT_COMPRESOR (45)   //--------------------------------------------------------->
#define INTERRUPCION_PULSE (19)  // pin de interrupcion

//! definiciones de modulo tarjeta SD interface SPI
#define PIN_SPI_SS (53)
#define PIN_SPI_MOSI (51)
#define PIN_SPI_MISO (50)
#define PIN_SPI_SCK (52)

//! definiciones de tiempos ----> UNIDAD DE 0.1 SEG  --> EJ 1 seg EQ 10 o tambien 10 seg EQ 100
#define TIME_BASE (1000 * 1000)       // EN micro segundos
#define TIME_VALOR_COMPRESOR (18000)  // valor en segundos cada cuanto se enciende el compresor
#define TIME_INT_PULSE (5)            //TIEMP ENTRE DOS POSBLES INTERRUPCIONES DEL MISMO PIN
#define TIME_INT_C1 (5)
#define TIME_INT_C2 (5)
#define TIME_HAPPY_LED (100000)
#define TIME_1S (6)
#define TIME_10S (66)
#define TIME_60S (66 * 6)


#ifdef __cplusplus
}
#endif

#endif