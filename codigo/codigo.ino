#include "define.h"
#include "caudal.h"
#include "tiempo.h"
// Macros

caudalimetro_t caudalimetro;  // crea un caudalimetro

void setup() {
  // put your setup code here, to run once:
  pinMode(pin_entrada, INPUT);
  IniciarValores(caudalimetro);
}

void loop() {
  // put your main code here, to run repeatedly:
  Verificar(caudalimetro);
}

