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

  float ValoresCaudal(caudalimetro_t cauda, uint16_t valor);


  /////////////////////////////////////////////////////////////////////////////////////////////////////

  // Estructra
  struct caudalimetro_s {
    uint32_t tiempo;
    uint32_t tiempo_prev;
    uint16_t constante;
    uint16_t indice;
    float caudal_promedio;
    float caudal[NUMB_ELEMENTS];
  };


  // Inicia los valores del caudalimetro
  caudalimetro_t InicializarCaudal() {
    caudalimetro_t cauda = (caudalimetro_t)malloc(sizeof(struct caudalimetro_s));

    if (cauda) {
      memset(cauda, 0, sizeof(cauda));
      cauda->tiempo = 0;
      cauda->tiempo_prev = 0;
      cauda->constante = 100;
      cauda->indice = 0;
      cauda->caudal_promedio = 0;
      for (uint8_t i = 0; i < NUMB_ELEMENTS; i++) {
        cauda->caudal[i] = 0;
      }
    }
    return &cauda[0];
  }

  void CaudalGuardarTiempo(caudalimetro_t cauda, uint32_t tiempo) {
    cauda->tiempo_prev = cauda->tiempo;
    cauda->tiempo = tiempo;
  }

  void ParametrizarCaudalimetro(caudalimetro_t cauda, uint16_t constante) {
    if (100 == constante) {
      cauda->constante = 100;
    } else {
      cauda->constante = 1000;
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

  // realiza caulculos del caudal
  void CalculoCaudal(caudalimetro_t cauda) {
    float delta_tiempo = cauda->tiempo - cauda->tiempo_prev;
    delta_tiempo = delta_tiempo / SEGUNDO_POR_HORA;  // tiempo en hora
    cauda->caudal[cauda->indice] = cauda->constante / delta_tiempo;
    cauda->indice = (cauda->indice + 1) % NUMB_ELEMENTS;
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