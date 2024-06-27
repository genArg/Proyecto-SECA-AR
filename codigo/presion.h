//! Macros
#ifndef PRESION_H
#define PRESION_H

#include "define.h"

typedef struct manometro_s* manometro_t;

manometro_t InicializarManometro(manometro_t);

void TomarValor(manometro_t man, uint16_t entrada);

void ParametrizarPresionMax(manometro_t man, uint16_t valor);

void ParametrizarCorrienteMax(manometro_t man, uint16_t valor);

void ParametrizarPresionMin(manometro_t man, uint16_t valor);

void ParametrizarCorrienteMin(manometro_t man, uint16_t valor);

void ParametrizarResistencia(manometro_t man, uint16_t valor);

uint16_t ValoresManometro(manometro_t man, uint16_t valor);

/////////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t RealizarCalculos(manometro_t man, uint16_t adc_presion);

struct manometro_s {
  uint16_t presion[NUMB_PRESS];
  uint16_t presion_media;
  uint16_t indice;
  uint16_t corriente_min;  //!> en mili ampers, tipicamente 4 mA
  uint16_t corriente_max;  //!> tipicamente 20 mA
  uint16_t resistencia;    //!> tipicamente de 250 ohm para tener todo el alcance
  uint16_t presion_min;    //!> establecido para cada manometro
  uint16_t presion_max;    //!> establecido para cada manometro
  uint16_t tension_max;    //!> no se parametriza, se calcula automaticamente
  uint16_t tension_min;    //!> no se parametriza, se calcula automaticamente
  uint16_t adc_min;        //!> no se parametriza, se calcula automaticamente
  uint16_t adc_max;        //!> no se parametriza, se calcula automaticamente
};

manometro_t InicializarManometro() {
  static struct manometro_s man[1];
  memset(man, 0, sizeof(man));
  if (man) {
    man->presion_media = 0;
    man->indice = 0;
    man->corriente_min = 4;
    man->corriente_max = 20;
    man->resistencia = 250;
    man->presion_min = 0;
    man->presion_max = 1;
    man->tension_min = man->corriente_min * man->resistencia;
    man->tension_max = man->corriente_max * man->resistencia;
    man->adc_min = CUENTAS_ADC * man->tension_min / TENSION_ADC;
    man->adc_max = CUENTAS_ADC * man->tension_max / TENSION_ADC;
    for (uint8_t i; i < NUMB_PRESS; i++) {
      man->presion[i] = 0;
    }
  }
  return man;
}

void TomarValor(manometro_t man, uint16_t entrada) {
  man->presion[man->indice] = RealizarCalculos(man, entrada);
  man->indice = (man->indice + 1) % NUMB_PRESS;
  // calculo de la presion media
  uint8_t cantidad = 0;
  uint16_t sumatoria = 0;
  for (uint8_t i = 0; i < NUMB_PRESS; i++) {
    if (man->presion[i] > PRESION_MINIMA) {
      sumatoria += man->presion[i];
      cantidad++;
    }
  }
  if (cantidad > 0) {
    man->presion_media = sumatoria / cantidad;
  }
}

/*realizar el el codigo de la logica para realizar la parametrizacion
  del medidor de presion y el hardware adicional a la placa como la reistencia*/
void ParametrizarPresionMax(manometro_t man, uint16_t valor) {
  //presion maxima
  man->presion_max = valor;
}

void ParametrizarCorrienteMax(manometro_t man, uint16_t valor) {
  //corriente maxima
  man->corriente_max = valor;
}

void ParametrizarPresionMin(manometro_t man, uint16_t valor) {
  //presion minima
  man->presion_min = valor;
}

void ParametrizarCorrienteMin(manometro_t man, uint16_t valor) {
  //corriente minima
  man->corriente_min = valor;
}

void ParametrizarResistencia(manometro_t man, uint16_t valor) {
  //resistencia
  man->resistencia = valor;
}

// realiza el caulculo de prsion real en funcion de los parametros del manometro
uint16_t RealizarCalculos(manometro_t man, uint16_t entrada) {
  uint16_t adc_presion;
  if (entrada) {
    adc_presion = LEER_PRESION_1;
  } else {
    adc_presion = LEER_PRESION_0;
  }
  uint16_t presion = 0;
  uint16_t adc_minimo = 0;
  uint16_t pendiente = 0;
  pendiente = (man->presion_max - man->presion_min) / (man->adc_max - man->adc_min);
  // realizar logica si el la tension es mayor que el permitido por el adc
  //y tambien si el adc entrega un valor menor que el menor permitido
  presion = man->presion_min + (pendiente * (adc_presion - man->adc_min));
  return presion;
}

uint16_t ValoresManometro(manometro_t man, uint16_t valor) {
  uint16_t auxiliar;
  //*
  switch (valor) {
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