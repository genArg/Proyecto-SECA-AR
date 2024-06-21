#include "presion.h"

uint16_t RealizarCalculos(manometro_t man);

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

void InicializarManometro(manometro_t man) {
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

void TomarValor(manometro_t man) {
  man->presion[man->indice] = RealizarCalculos(man);
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

void ParametrizarPresion(manometro_t man) {
  /*realizar el el codigo de la logica para realizar la parametrizacion
  del medidor de presion y el hardware adicional a la placa como la reistencia*/
}

// realiza el caulculo de prsion real en funcion de los parametros del manometro
uint16_t RealizarCalculos(manometro_t man) {
  uint16_t adc_presion = LEER_PRESION;
  uint16_t presion = 0;
  uint16_t adc_minimo = 0;
  uint16_t pendiente = 0;
  pendiente = (man->presion_max - man->presion_min) / (man->adc_max - man->adc_min);
  // realizar logica si el la tension es mayor que el permitido por el adc
  //y tambien si el adc entrega un valor menor que el menor permitido 
  presion = man->presion_min + (pendiente * (adc_presion - man->adc_min)); 
  return presion;
}