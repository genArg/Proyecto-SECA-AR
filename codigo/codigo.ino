#include "define.h"
#include "caudal.h"
#include "grafica.h"
#include "tiempo.h"
#include "presion.h"

// Macros

caudalimetro_t caudalimetro;  // crea un caudalimetro
char mensaje[] = "mensaje";
char letra;

void setup() {
  // put your setup code here, to run once:
  //pinMode(pin_entrada, INPUT);
  //IniciarValores(caudalimetro);
  Serial.begin(9600);

  InicioPantalla();
  delay(1000);
  Imprimir(mensaje);
  delay(1000);
  if (TomarTiempo())
    Imprimir("mensaje 1");
  else
    Imprimir("mensaje 0");
}

void loop() {
  // put your main code here, to run repeatedly:
  //Verificar(caudalimetro);
  // Leer el valor del pin analógico A0
  int valorAnalogico = analogRead(A15);

  // Imprimir el valor leído en el monitor serie
  Serial.print("Valor analógico leído: ");
  Serial.println(valorAnalogico);

  // Esperar 500 milisegundos antes de la próxima lectura
  delay(500);
}
