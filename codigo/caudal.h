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
  //void Verificar(caudalimetro_t cauda);

  //! Inicializa los valores del caudalimetro
  caudalimetro_t InicializarCaudal();

  //
  void ParametrizarCaudalimetro(caudalimetro_t cauda, uint16_t constante);

  //! Realiza el calculo del caudal en litros por hora en funcion al tiempo
  void CalculoCaudal(caudalimetro_t cauda);

  uint16_t ValoresCaudal(caudalimetro_t cauda, uint16_t valor);


  /////////////////////////////////////////////////////////////////////////////////////////////////////

  // Estructra
  struct caudalimetro_s {
    bool entrada;
    bool entrada_prev;
    uint32_t tiempo;
    uint32_t tiempo_prev;
    uint16_t constante;
    uint16_t indice;
    uint16_t caudal_promedio;
    uint16_t caudal[NUMB_ELEMENTS];
    bool logica;
  };

  
  // Inicia los valores del caudalimetro
  caudalimetro_t InicializarCaudal() {
    static struct caudalimetro_s cauda[1];
    memset(cauda, 0, sizeof(cauda));
    if (cauda) {
      cauda->entrada = false;
      cauda->entrada_prev = false;
      cauda->tiempo = 0;
      cauda->tiempo_prev = 0;
      cauda->constante = 100;
      cauda->indice = 0;
      cauda->caudal_promedio = 0;
      cauda->logica = false;
      for (uint8_t i = 0; i < NUMB_ELEMENTS; i++) {
        cauda->caudal[i] = 0;
      }
    }
    return cauda;
  }

  void ParametrizarCaudalimetro(caudalimetro_t cauda, uint16_t constante) {
    if (100 == constante) {
      cauda->constante = 100;
    } else {
      cauda->constante = 1000;
    }
  }

  uint16_t ValoresCaudal(caudalimetro_t cauda, uint16_t valor) {

    uint16_t auxiliar;
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

  // realiza caulculos del caudal
  void CalculoCaudal(caudalimetro_t cauda) {
    uint16_t delta_tiempo = cauda->tiempo - cauda->tiempo_prev;
    delta_tiempo = delta_tiempo / 3600;  // tiempo en hora
    cauda->caudal[cauda->indice] = cauda->constante / delta_tiempo;
    cauda->indice = (cauda->indice + 1) % NUMB_ELEMENTS;
    // calculo del caudal medio
    uint8_t cantidad = 0;
    uint16_t sumatoria = 0;
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