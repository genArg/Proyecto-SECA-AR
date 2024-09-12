//! Macros
#ifndef CAUDAL_H
#define CAUDAL_H

#include "tiempo.h"
#include "define.h"

#ifdef __cplusplus
extern "C" {
#endif

  //! puntero a un caudalimetro de 1 señal
  typedef struct caudalimetro_s* caudalimetro_t;

  //! verifica señal del caudalimetro
  void CaudalGuardarTiempo(caudalimetro_t cauda, uint32_t tiempo);

  //! Inicializa los valores del caudalimetro
  caudalimetro_t InicializarCaudal();

  //
  void ParametrizarCaudalimetro(caudalimetro_t cauda, uint16_t constante);

  //! Realiza el calculo del caudal en litros por hora en funcion al tiempo
  void CalculoCaudal(caudalimetro_t cauda);

  //! Realiza el calculo del caudal en litros por hora en funcion al tiempo sin añadir ningun elemento
  void RecalcularCaudal(caudalimetro_t cauda, uint16_t valor);

  float ValoresCaudal(caudalimetro_t cauda, uint16_t valor);

  float CalculoTemporal(caudalimetro_t caudalimetro);

  void IniciarValoresCaudal(caudalimetro_t cauda);

  /////////////////////////////////////////////////////////////////////////////////////////////////////

  // Estructra
  struct caudalimetro_s {
    uint8_t tiempo_max_cont;
    float delta_tiempo;
    float tiempo;
    float tiempo_prev;
    uint16_t constante;
    uint16_t indice;
    float caudal_promedio;
    float caudal[NUMB_ELEMENTS];
    uint8_t habilitacion;
    //
    uint8_t milisegundo;  // son decenas de milisegundos
    uint16_t segundo;
    uint16_t minuto;
    uint16_t hora;
    uint16_t dia;
    uint16_t mes;
    uint16_t year;
    //
    uint8_t milisegundo_ant;  // son decenas de milisegundos
    uint16_t segundo_ant;
    uint16_t minuto_ant;
    uint16_t hora_ant;
    uint16_t dia_ant;
    uint16_t mes_ant;
    uint16_t year_ant;
    uint8_t primera_muestra;  // bandera que se cambia despues de la primera muestra
  };


  // Inicia los valores del caudalimetro
  caudalimetro_t InicializarCaudal() {
    caudalimetro_t cauda = (caudalimetro_t)malloc(sizeof(struct caudalimetro_s));

    if (cauda) {
      memset(cauda, 0, sizeof(cauda));
      IniciarValoresCaudal(cauda);
      cauda->tiempo_max_cont = CAUDA_TIEMPO_MAXIMO;
    }
    return &cauda[0];
  }

  void IniciarValoresCaudal(caudalimetro_t cauda) {
    cauda->tiempo = 0;
    cauda->tiempo_prev = 0;
    cauda->constante = 1;
    cauda->indice = 0;
    cauda->caudal_promedio = 0;
    cauda->habilitacion = 1;
    cauda->segundo_ant = 0;
    cauda->minuto_ant = 0;
    cauda->hora_ant = 0;
    cauda->dia_ant = 0;
    cauda->mes_ant = 0;
    cauda->year_ant = 0;
    cauda->primera_muestra = 1;
    for (uint8_t i = 0; i < NUMB_ELEMENTS; i++) {
      cauda->caudal[i] = 0;
    }
  }

  void CaudalGuardarTiempo(caudalimetro_t cauda, uint32_t tiempo) {
    cauda->tiempo_prev = cauda->tiempo;
    cauda->tiempo = tiempo;
  }

  void ParametrizarCaudalimetro(caudalimetro_t cauda, uint16_t constante) {
    if (1 == constante) {
      cauda->constante = 1;
    } else {
      cauda->constante = 10;
    }
  }

  float ValoresCaudal(caudalimetro_t cauda, uint16_t valor) {

    float auxiliar;
    //*
    switch (valor) {
      case 1:
        auxiliar = cauda->caudal_promedio;
        break;
      case 2:
        auxiliar = cauda->constante;
        break;
      default:
        auxiliar = 380;
        break;
    }  //*/
    return auxiliar;
  }

  // realiza caulculos del caudal en */horas
  void CalculoCaudal(caudalimetro_t cauda) {
    //float delta_tiempo = cauda->tiempo - cauda->tiempo_prev;
    cauda->delta_tiempo = CalculoTemporal(cauda);
    if (cauda->delta_tiempo > 0) {
      cauda->delta_tiempo = cauda->delta_tiempo / SEGUNDO_POR_HORA;  // tiempo en hora
      cauda->caudal[cauda->indice] = cauda->constante / cauda->delta_tiempo;
      cauda->indice = (cauda->indice + 1) % NUMB_ELEMENTS;
    }
    // calculo del caudal medio
    uint8_t cantidad = 0;
    float sumatoria = 0;
    for (uint8_t i = 0; i < NUMB_ELEMENTS; i++) {
      if (cauda->caudal[i] > CAUDAL_MINIMO) {
        sumatoria += cauda->caudal[i];
        cantidad++;
      }
    }
    if (cantidad > 0) {
      cauda->caudal_promedio = sumatoria / cantidad;
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  // realiza caulculos del tiempo en entre dos tomas en segundos
  float CalculoTemporal(caudalimetro_t caudalimetro) {
    float total = 0;
    int16_t milisegundo = caudalimetro->milisegundo - caudalimetro->milisegundo_ant;  //son decenas de mili segundos
#if DEBUG_RTC_2 == TRUE
    Serial.print("m1------------------------------>  ");
    Serial.println(milisegundo);
#endif
    if (milisegundo < 0) milisegundo = 100 + milisegundo;
#if DEBUG_RTC_2 == TRUE
    Serial.print("m2------------------------------>  ");
    Serial.println(milisegundo);
#endif

    int16_t segundo = caudalimetro->segundo - caudalimetro->segundo_ant;
#if DEBUG_RTC_2 == TRUE
    Serial.print("S1------------------------------>  ");
    Serial.println(segundo);
#endif
    if (segundo < 0) segundo = 60 + segundo;
    if (segundo >= 1) segundo--;
#if DEBUG_RTC_2 == TRUE
    Serial.print("S2------------------------------>  ");
    Serial.println(segundo);
#endif

    int16_t minuto = caudalimetro->minuto - caudalimetro->minuto_ant;
#if DEBUG_RTC_2 == TRUE
    Serial.print("M1------------------------------>  ");
    Serial.println(minuto);
#endif
    if (minuto < 0) minuto = 60 + minuto;
    if (minuto >= 1) minuto--;
#if DEBUG_RTC_2 == TRUE
    Serial.print("M2------------------------------>  ");
    Serial.println(minuto);
#endif

    int16_t hora = caudalimetro->hora - caudalimetro->hora_ant;
    if (hora < 0) hora = 24 + hora;
    if (hora >= 1) hora--;

    if (caudalimetro->dia == caudalimetro->dia_ant && caudalimetro->mes == caudalimetro->mes_ant && caudalimetro->year == caudalimetro->year_ant && caudalimetro->primera_muestra == 0) {
      total = (milisegundo * 0.01) + (segundo) + (minuto * 60) + (hora * 3600);  // en segundos
    }
    if (caudalimetro->primera_muestra == 1) {
      caudalimetro->primera_muestra = 0;
    }
#if DEBUG_RTC_2 == TRUE
    Serial.print("T------------------------------>  ");
    Serial.println(total);
#endif
    return total;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  // realiza caulculos del caudal sin garegar un valor
  void RecalcularCaudal(caudalimetro_t cauda, uint16_t valor) {
    for (uint8_t i = 0; i < NUMB_ELEMENTS; i++) {
      cauda->caudal[cauda->indice] = (cauda->constante * cauda->caudal[cauda->indice]) / valor;
    }

    // calculo del caudal medio
    uint8_t cantidad = 0;
    float sumatoria = 0;
    for (uint8_t i = 0; i < NUMB_ELEMENTS; i++) {
      if (cauda->caudal[i] > CAUDAL_MINIMO) {
        sumatoria += cauda->caudal[i];
        cantidad++;
      }
    }
    if (cantidad > 0) {
      cauda->caudal_promedio = sumatoria / cantidad;
    }
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#endif