//! Macros
#ifndef PRESION_H
#define PRESION_H

#include "define.h"

typedef struct manometro_s* manometro_t;

manometro_t InicializarManometro();

void TomarValor(manometro_t man, uint16_t pin_adc);

void ParametrizarConstante(manometro_t man, uint16_t valor);

void ParametrizarPresionMax(manometro_t man, uint16_t valor);

void ParametrizarCorrienteMax(manometro_t man, uint16_t valor);

void ParametrizarPresionMin(manometro_t man, uint16_t valor);

void ParametrizarCorrienteMin(manometro_t man, uint16_t valor);

void ParametrizarResistencia(manometro_t man, uint16_t valor);

uint16_t ValoresManometro(manometro_t man, uint16_t valor);

/////////////////////////////////////////////////////////////////////////////////////////////////////

float RealizarCalculos(manometro_t man, uint16_t entrada);
void Recalcular(manometro_t man);

struct manometro_s {
  float presion[NUMB_PRESS];
  float presion_media;
  uint16_t indice;
  uint16_t constante;      //!> para ajustar el valor que muestra
  uint16_t corriente_min;  //!> en mili ampers, tipicamente 4 mA
  uint16_t corriente_max;  //!> tipicamente 20 mA
  uint16_t resistencia;    //!> tipicamente de 250 ohm para tener todo el alcance
  uint16_t presion_min;    //!> establecido para cada manometro
  uint16_t presion_max;    //!> establecido para cada manometro
  float tension_max;       //!> no se parametriza, se calcula automaticamente
  float tension_min;       //!> no se parametriza, se calcula automaticamente
  float adc_min;           //!> no se parametriza, se calcula automaticamente
  float adc_max;           //!> no se parametriza, se calcula automaticamente
  float pendiente;         //!> no se parametriza, se calcula automaticamente
};

manometro_t InicializarManometro() {
  manometro_t man = (manometro_t)malloc(sizeof(struct manometro_s));
  
  if (man) {
    memset(man, 0, sizeof(man));
    man->presion_media = 0;
    man->indice = 0;
    man->constante = 1;
    man->corriente_min = 4;
    man->corriente_max = 20;
    man->resistencia = 250;
    man->presion_min = 0;
    man->presion_max = 100;
    man->tension_min = (man->corriente_min * man->resistencia) / 1000;
    man->tension_max = (man->corriente_max * man->resistencia) / 1000;
    man->adc_min = CUENTAS_ADC * man->tension_min / TENSION_ADC;
    man->adc_max = CUENTAS_ADC * man->tension_max / TENSION_ADC;
    for (uint8_t i; i < NUMB_PRESS; i++) {
      man->presion[i] = 0;
    }
    man->pendiente = (man->presion_max - man->presion_min) / (man->adc_max - man->adc_min);
  }
  return &man[0];
}

void TomarValor(manometro_t man, uint16_t pin_adc) {
  uint8_t cantidad = 0;
  float sumatoria = 0;
  uint16_t valor_adc;
  for (int i = 0; i < 5; i++) {
    valor_adc = analogRead(pin_adc);
    man->presion[man->indice] = RealizarCalculos(man, valor_adc);
    man->indice = (man->indice + 1) % NUMB_PRESS;
  }
  // calculo de la presion media
  for (uint8_t i = 0; i < NUMB_PRESS; i++) {
    if (man->presion[i] > PRESION_MINIMA) {
      sumatoria += man->presion[i];
      cantidad++;
    }
  }
  if (cantidad > 0) {
    man->presion_media = man->constante * (sumatoria / cantidad);
  } else {
    man->presion_media = 0;
  }
}

/*realizar el el codigo de la logica para realizar la parametrizacion
  del medidor de presion y el hardware adicional a la placa como la reistencia*/

void ParametrizarConstante(manometro_t man, uint16_t valor) {
  //constante
  if (valor >= 0) {
    man->constante = valor;
    Recalcular(man);
  }
}

void ParametrizarPresionMax(manometro_t man, uint16_t valor) {
  //presion maxima
  if (valor >= 0) {
    man->presion_max = valor;
    Recalcular(man);
  }
}

void ParametrizarCorrienteMax(manometro_t man, uint16_t valor) {
  //corriente maxima
  if (valor >= 0) {
    man->corriente_max = valor;
    Recalcular(man);
  }
}

void ParametrizarPresionMin(manometro_t man, uint16_t valor) {
  //presion minima
  if (valor >= 0) {
    man->presion_min = valor;
    Recalcular(man);
  }
}

void ParametrizarCorrienteMin(manometro_t man, uint16_t valor) {
  //corriente minima
  if (valor >= 0) {
    man->corriente_min = valor;
    Recalcular(man);
  }
}

void ParametrizarResistencia(manometro_t man, uint16_t valor) {
  //resistencia
  if (valor >= 0) {
    man->resistencia = valor;
    Recalcular(man);
  }
}

void Recalcular(manometro_t man) {
  man->tension_min = (man->corriente_min * man->resistencia) / 1000;
  man->tension_max = (man->corriente_max * man->resistencia) / 1000;
  man->adc_min = CUENTAS_ADC * man->tension_min / TENSION_ADC;
  man->adc_max = CUENTAS_ADC * man->tension_max / TENSION_ADC;
  man->pendiente = (man->presion_max - man->presion_min) / (man->adc_max - man->adc_min);
}

// realiza el caulculo de prsion real en funcion de los parametros del manometro
float RealizarCalculos(manometro_t man, uint16_t entrada) {
  float presion = 0;
  // realizar logica si el la tension es mayor que el permitido por el adc
  //y tambien si el adc entrega un valor menor que el menor permitido
  presion = man->presion_min + (man->pendiente * (entrada - man->adc_min));
  return presion;
}

uint16_t ValoresManometro(manometro_t man, uint16_t valor) {
  uint16_t auxiliar;
  //*
  switch (valor) {
    case 0:
      auxiliar = man->constante;
      break;
    case 1:
      auxiliar = man->presion_media;
      break;
    case 2:
      auxiliar = man->corriente_min;
      break;
    case 3:
      auxiliar = man->corriente_max;
      break;
    case 4:
      auxiliar = man->resistencia;
      break;
    case 5:
      auxiliar = man->presion_min;
      break;
    case 6:
      auxiliar = man->presion_max;
      break;
    case 7:
      auxiliar = man->tension_min;
      break;
    case 8:
      auxiliar = man->tension_max;
      break;
    case 9:
      auxiliar = man->adc_min;
      break;
    case 10:
      auxiliar = man->adc_max;
      break;
    default:
      auxiliar = 420;
      break;
  }  //*/

  //Serial.println(auxiliar);
  return auxiliar;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif