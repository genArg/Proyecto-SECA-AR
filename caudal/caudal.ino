#include "caudal.h"
// Macros
#define pin_a 51
#define pin_b 52


// Estructra
struct caudalimetro_s {
  bool entrada;
  bool entrada_prev;
  uint16_t tiempo;
  uint16_t tiempo_prev;
  uint16_t constante;
  uint16_t indice;
  uint16_t caudal_promedio;
  uint16_t caudal[NUMB_ELEMENTS];
};

caudalimetro_t caudalimetro;  // crea un caudalimetro

void setup() {
  // put your setup code here, to run once:
  pinMode(pin_a, INPUT);
  IniciarValores(caudalimetro);
}

void loop() {
  // put your main code here, to run repeatedly:
  Verificar(caudalimetro);
}

// Verifica que la señal sea
void Verificar(caudalimetro_t cauda) {
  cauda->entrada = LEER_ENTRADA;
  if (cauda->entrada != cauda->entrada_prev) {
    cauda->tiempo_prev = cauda->tiempo;
    cauda->tiempo = TomarTiempo();
    cauda->entrada_prev = cauda->entrada;
  }
}

// Inicia los valores del caudalimetro
void IniciarValores(caudalimetro_t cauda) {
  if (cauda) {
    cauda->entrada = false;
    cauda->entrada_prev = false;
    cauda->tiempo = 0;
    cauda->tiempo_prev = 0;
    cauda->constante = 100;
    cauda->indice = 0;
    cauda->caudal_promedio = 0;
    for (uint8_t i = 0; i < NUMB_ELEMENTS; i++) {
      cauda->caudal[i] = 0;
    }
  }
}

void ParametrizarCaudalimetro(caudalimetro_t cauda, uint16_t constante) {
  if (100 == constante) {
    cauda->constante = 100;
  } else {
    cauda->constante = 1000;
  }
}

void CalculoCaudal(caudalimetro_t cauda) {
  uint16_t delta_tiempo = cauda->tiempo - cauda->tiempo_prev;
  delta_tiempo = delta_tiempo / 3600;  // tiempo en hora
  cauda->caudal[cauda->indice] = cauda->constante / delta_tiempo;
  cauda->indice++;
  // calculo del caudal medio
  uint8_t cantidad = 0;
  uint16_t sumatoria = 0;
  for (uint8_t i = 0; i < 5; i++) {
    if (cauda->caudal[i] > CAUDAL_MINIMO)
      sumatoria = +cauda->caudal[i];
    cantidad++;
  }
  if (cantidad > 0) {
    cauda->caudal_promedio = sumatoria / cantidad;
  }
}


// es el tiempo que se toma del RTC
uint16_t TomarTiempo() {}